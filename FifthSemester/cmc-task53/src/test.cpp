#include "test.h"

#include "hog.h"
#include "image.h"

#include <ctime>
#include "gtest/gtest.h"
#include <functional>
#include <iomanip>
#include <ios>
#include <iostream>
#include <sstream>

static const float EPS = 1e-4;
static const ssize_t ITERATIONS = 20;

typedef std::function<void()> VoidFunction;

std::string
profile(VoidFunction test_function, ssize_t iterations)
{
    ++ iterations; // first value hack

    auto times = new clock_t[iterations];
    clock_t last_time;

    last_time = clock();
    for (ssize_t i = 0; i < iterations; ++ i) {
        test_function();
        times[i] = clock() - last_time;
        last_time = times[i] + last_time;
    }

    double average_time = 0;
    for (ssize_t i = 1; i < iterations; ++ i) {
        average_time += times[i];
    }
    average_time /= iterations - 1;

    double disp_time = 0;
    for (ssize_t i = 1; i < iterations; ++ i) {
        disp_time += (times[i] - average_time) * (times[i] - average_time);
    }

    delete[] times;
    average_time /= CLOCKS_PER_SEC;
    disp_time /= CLOCKS_PER_SEC * CLOCKS_PER_SEC * (iterations - 2);

    std::stringstream result;
    result << std::fixed << std::setprecision(2)
           << "Average execution time: "
           << average_time << "s (+/- "
           << 6 * sqrt(disp_time) << "s).";

    return result.str();
}

class HOGTest : public ::testing::Test {
public:
    const Matrix<float> gray = Matrix<float>();

    HOGTest() :
        ::testing::Test(),
        gray(load_image("data/lenna.bmp").unary_map(Grayscale()))
    {
    }
};

class RunGrad {
public:
    HOGTest *hog_test;
    bool sse;
    Matrix<std::pair<float, float>> result = Matrix<std::pair<float, float>>();

    RunGrad(HOGTest *in_hog_test, bool in_sse) :
        hog_test(in_hog_test),
        sse(in_sse)
    {
    }

    RunGrad &
    operator=(const RunGrad &src)
    {
        this->hog_test = src.hog_test;
        this->sse = src.sse;
        return *this;
    }

    RunGrad(const RunGrad &src) :
        hog_test(src.hog_test),
        sse(src.sse)
    {
    }

    void
    operator() ()
    {
        result = grad(this->hog_test->gray, this->sse);
    }
};

TEST_F(HOGTest, CheckSpeed)
{
    std::cerr << "Without sse: " << profile(RunGrad(this, false), ITERATIONS)
              << std::endl;
    std::cerr << "With sse: " << profile(RunGrad(this, true), ITERATIONS)
              << std::endl;
}

TEST_F(HOGTest, CheckGrad)
{
    auto grad_without_sse = grad(this->gray, false);
    auto grad_with_sse = grad(this->gray, true);

    EXPECT_EQ(grad_with_sse.n_rows, grad_without_sse.n_rows);
    EXPECT_EQ(grad_with_sse.n_cols, grad_without_sse.n_cols);

    for (ssize_t i = 0; i < grad_with_sse.n_rows; ++ i) {
        for (ssize_t j = 0; j < grad_with_sse.n_cols; ++ j) {
            EXPECT_LE(abs(grad_with_sse(i, j).second -
                          grad_without_sse(i, j).second), EPS);
            if (abs(grad_with_sse(i, j).second) > EPS) {
                EXPECT_LE(abs(grad_with_sse(i, j).first -
                              grad_without_sse(i, j).first), EPS);
            }
        }
    }
}

int
main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
