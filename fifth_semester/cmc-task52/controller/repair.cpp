#include "controller/repair.h"

#include "controller/io.h"
#include "model/canvas.h"
#include "model/component.h"
#include "model/draw.h"
#include "model/messages.h"

#include <utility>

static IObject
parse_object(const Matrix<ssize_t> &m, Component &c)
{
    c.find_center(m);
    auto center = c.center;
    auto min_radius = c.min_radius,
         max_radius = c.max_radius;

    center.first += c.bbox[Component::MIN_ROW];
    center.second += c.bbox[Component::MIN_COL];

    if (max_radius - min_radius <= DELTA_READIUS) {
        return IObject(TAxis, center, c, min_radius);
    } else {
        auto bin_frame = c.bin_frame.deep_copy();
        auto rows = bin_frame.n_rows,
             cols = bin_frame.n_cols;
        for (ssize_t i = 0; i < rows; ++ i) {
            for (ssize_t j = 0; j < cols; ++ j) {
                if (sqr(i - c.center.first) + sqr(j - c.center.second) <=
                    sqr(min_radius + DELTA_READIUS)) {

                    bin_frame(i, j) = false;
                }
            }
        }

        Matrix<ssize_t> colored_cogs;
        std::vector<Component> cogs;

        tie(colored_cogs, cogs) = separate(bin_frame, 5);

        double distance = average_center_distance2(bin_frame, c.center);

        return IObject(TGear, center, c, min_radius, max_radius,
                       distance > DELTA_READIUS, cogs.size());
    }
}

static ssize_t
fix_axis(const std::vector<Image> &in,
         Matrix<bool> &binar,
         const IObject &axis)
{
    dfs_fill(binar,
             Coord(axis.component.start.first, axis.component.start.second),
             true,
             false);

    ssize_t top_score = -2;
    ssize_t result_idx = 1;
    Matrix<bool> top_gear_binar(0, 0);
    Coord top_shift;

    for (ssize_t k = 1; k < static_cast<ssize_t>(in.size()); ++ k) {

        auto gear_inten = add_canvas_border(in[k], 1, BACKGROUND)
                          .unary_map(Intensity<ssize_t>());
        auto gear_binar = gear_inten
                          .unary_map(Binarizator<ssize_t>(INTENSITY_TRESHOLD));

        Matrix<ssize_t> separ_map;
        std::vector<Component> gear_comp;
        tie(separ_map, gear_comp) = separate(gear_binar, MIN_COMPONENT);
        IObject gear = parse_object(separ_map, gear_comp[0]);
        gear_binar = separ_map.unary_map(GetComponent(1));

        auto shift = std::make_pair(axis.center.first - gear.center.first,
                               axis.center.second - gear.center.second);

        ssize_t score = 0;
        for (ssize_t i = 0; i < gear_binar.n_rows; ++ i) {
            for (ssize_t j = 0; j < gear_binar.n_cols; ++ j) {
                if (gear_binar(i, j)) {
                    Coord new_coord = std::make_pair(i + shift.first, j + shift.second);
                    if  (binar.in_matrix(new_coord) &&
                         binar(new_coord)) {
                        score = -1;
                        break;
                    } else {
                        ++ score;
                    }
                }
            }

            if (score == -1) {
                break;
            }
        }

        if (score > top_score) {
            top_score = score;
            result_idx = k;
            top_gear_binar = gear_binar;
            top_shift = shift;
        }
    }

    for (ssize_t i = 0; i < top_gear_binar.n_rows; ++ i) {
        for (ssize_t j = 0; j < top_gear_binar.n_cols; ++ j) {
            Coord new_coord = std::make_pair(i + top_shift.first, j + top_shift.second);
            if (binar.in_matrix(new_coord) && top_gear_binar(i, j)) {
                binar(new_coord) = true;
            }
        }
    }

    return result_idx;
}

static ssize_t
fix_gear(const std::vector<Image> &in,
         Matrix<bool> &binar,
         const IObject &creaken_gear)
{
    Matrix<ssize_t> broken_gear_c(binar.n_rows, binar.n_cols);
    for (ssize_t i = 0; i < binar.n_rows; ++ i) {
        for (ssize_t j = 0; j < binar.n_cols; ++ j) {
            broken_gear_c(i, j) = 0;
        }
    }

    Component c = creaken_gear.component;
    dfs(binar, broken_gear_c, c.start, c);
    auto broken_gear = broken_gear_c.unary_map(GetComponent(c.number));

    dfs_fill(binar,
             Coord(creaken_gear.component.start.first, creaken_gear.component.start.second),
             true,
             false);

    ssize_t top_score = -2;
    ssize_t result_idx = 1;
    Matrix<bool> top_gear_binar(0, 0);
    Coord top_shift;

    for (ssize_t k = 1; k < static_cast<ssize_t>(in.size()); ++ k) {

        auto gear_inten = add_canvas_border(in[k], 1, BACKGROUND)
                          .unary_map(Intensity<ssize_t>());
        auto gear_binar = gear_inten
                          .unary_map(Binarizator<ssize_t>(INTENSITY_TRESHOLD));

        Matrix<ssize_t> separ_map;
        std::vector<Component> gear_comp;
        tie(separ_map, gear_comp) = separate(gear_binar, MIN_COMPONENT);
        IObject gear = parse_object(separ_map, gear_comp[0]);
        gear_binar = separ_map.unary_map(GetComponent(1));

        auto shift = std::make_pair(creaken_gear.center.first - gear.center.first,
                               creaken_gear.center.second - gear.center.second);

        ssize_t score = 0;
        for (ssize_t i = 0; i < gear_binar.n_rows; ++ i) {
            for (ssize_t j = 0; j < gear_binar.n_cols; ++ j) {
                if (gear_binar(i, j)) {
                    Coord new_coord = std::make_pair(i + shift.first, j + shift.second);
                    if (binar.in_matrix(new_coord)) {
                        if (binar(new_coord)) {
                            score = -1;
                            break;
                        } else if (broken_gear(new_coord) && gear_binar(i, j)) {
                            ++ score;
                        }
                    }
                }
            }

            if (score == -1) {
                break;
            }
        }

        if (score > top_score) {
            top_score = score;
            result_idx = k;
            top_gear_binar = gear_binar;
            top_shift = shift;
        }
    }

    for (ssize_t i = 0; i < top_gear_binar.n_rows; ++ i) {
        for (ssize_t j = 0; j < top_gear_binar.n_cols; ++ j) {
            Coord new_coord = std::make_pair(i + top_shift.first, j + top_shift.second);
            if (binar.in_matrix(new_coord) && top_gear_binar(i, j)) {
                binar(new_coord) = true;
            }
        }
    }

    return result_idx;
}

void
repair(const std::vector<Image> &in)
{
    // Base: return array of found objects and index of the correct gear
    // Bonus: return additional parameters of gears
    std::vector<IObject> object_array;
    ssize_t result_idx = 0;
    auto listener_manager = ListenerManager::get_singleton();

    auto work_copy = add_canvas_border(in[0], 1, BACKGROUND);
    listener_manager->send(M_ADD_BORDER, "Add border", work_copy);

    auto inten = work_copy.unary_map(Intensity<ssize_t>());
    auto binar = inten.unary_map(Binarizator<ssize_t>(INTENSITY_TRESHOLD));
    listener_manager->send(M_BIN,
                          "Binarization completed",
                          binar.unary_map(Debinarizator<Color>(Color(0, 255, 0),
                                                               Color(0, 0, 0))));

    Matrix<ssize_t> separ_map;
    std::vector<Component> components;
    tie(separ_map, components) = separate(binar, MIN_COMPONENT);
    auto deseparate_map = deseparate(separ_map, components);
    listener_manager->send(M_SEPARATE,
                           "Separation completed",
                           deseparate_map);

    for (auto &c : components) {
        auto object = parse_object(separ_map, c);

        draw_circle(deseparate_map, object.center, 3, Color(0, 0, 0));
        draw_circle(deseparate_map, object.center, object.min_radius,
                    Color(255, 255, 255));
        draw_circle(deseparate_map, object.center, object.max_radius,
                    Color(255, 255, 255));

        object_array.push_back(object);

        if (object.type == TAxis) {
            result_idx = fix_axis(in, binar, object);
        } else if (object.type == TGear && object.is_broken) {
            result_idx = fix_gear(in, binar, object);
        }
    }

    listener_manager->send(M_FIND_OBJ,
                           result_text(object_array, result_idx),
                           deseparate_map);

    auto out = binar.unary_map(Debinarizator<Color>(Color(0, 255, 0), Color(0, 0, 0)));
    out = remove_canvas_border(out, 1);
    listener_manager->send(M_REPAIR, "Repair completed", out);
}
