#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UNDEFINED ((int64_t)1 << 32) + 1
#define MULTIPLIER 101
// 49 * 128 -> 64 * 128
#define MAX_STATES (64 * 128)
#define HASHTABLE_SIZE (MAX_STATES * 2)
#define COLUMN_COUNT 7
#define MAX_FORMATLEN 10

#define max(a,b) \
   ({__typeof__ (a) _a = (a); \
     __typeof__ (b) _b = (b); \
     _a ^ ((_a ^ _b) & -(_a < _b)); })

#define min(a,b) \
   ({__typeof__ (a) _a = (a); \
     __typeof__ (b) _b = (b); \
     _b ^ ((_a ^ _b) & -(_a < _b)); })

// # Headers

typedef struct State {
    int64_t h;
    int64_t f_x, f_y;
    int64_t g_x, g_y;
    int8_t c_f, c_g;
} State;

typedef struct Pair {
    State state;
    bool init;
    bool seen;
} *Pair;

typedef enum OutputType {
    BEGIN,
    END
} OutputType;

typedef enum PrintStateType {
    NORMAL,
    LTS
} PrintStateType;

typedef struct Edge {
    const Pair begin, end;
    const char *action;
} Edge;

/**
 * Get key/value pair in hashtable.
 */
Pair
get_pair(const State *state);

/**
 * Get state by parameters.
 */
State
get_state(int8_t c_f, int8_t c_g, int64_t h, int64_t f_x, int64_t f_y, int64_t g_x, int64_t g_y);

/**
 * Get hash of state.
 */
ssize_t
get_hash(const State *);

/**
 * Print variable to stdout (maybe undefined)
 */
void
print_variable(int64_t, ssize_t);

/**
 * Print state to stdout.
 */
void
print_state(const Pair, PrintStateType);

/**
 * Main function
 */
int
main(int, char **);

/**
 * Parse command arguments.
 */
void
argparse(ssize_t, char **);

/**
 * Calc header length for table.
 */
void
precalc_header_length(void);

/**
 * Calc length of integer
 */
int length_helper(int32_t);

/**
 * Output function.
 */
void
output(OutputType);

/**
 * Calc all states
 */
void
bfs(void);

/**
 * Make step in f and return new state
 */
Edge
step_f(const Pair);

/**
 * Make step in g and return new state
 */
Edge
step_g(const Pair);

// # Преамбула

// Если заставлять программистов писать комментарии не ниже нормы, то
// полезность таких комментариев будет уменьшаться. Сначала
// программисты начнут в комментариях писать о том, как они не любят писать
// бесполезные комментарии, потом потратят свое рабочее время на
// написание программы, которая за них будет генерировать бессмысленные
// комментарии и расставлять их по исходному коду во всех местах, которые
// вы указали как обязательные к коментированию.
//
// Не делайте так, пожалуйста.

// # Implementation

int32_t f_a = 0,
        f_b = 0,
        g_a = 0,
        g_b = 0;
bool only_count = false;
bool only_lts = false;

ssize_t column_width[COLUMN_COUNT] = {0, 0, 0, 0, 0, 0, 0};
char *headers[COLUMN_COUNT] = {"c_f", "c_g", "h", "f.x", "f.y", "g.x", "g.y"};

ssize_t state_count = 0;
Pair hashtable = NULL;
Pair *queue = NULL;
ssize_t head = 0,
        tail = 0;

Pair
get_pair(const State *state)
{
    // Тело функции
    ssize_t index = get_hash(state);
    while (hashtable[index].init) {
        // Тело цикла
        if (memcmp(&hashtable[index].state, state, sizeof(*state)) == 0) { // Условный оператор
            return &hashtable[index];
        } else { // Иначе
            index = (index + 1) % HASHTABLE_SIZE;
        }
    }

    ++ state_count;
    hashtable[index].state = *state;
    hashtable[index].init = true;
    return &hashtable[index];
}

State
get_state(int8_t c_f, int8_t c_g, int64_t h, int64_t f_x, int64_t f_y, int64_t g_x, int64_t g_y)
{
    // Тело функции
    State state = {
        .c_f = c_f,
        .c_g = c_g,
        .h = h,
        .f_x = f_x,
        .f_y = f_y,
        .g_x = g_x,
        .g_y = g_y
    };

    return state;
}

ssize_t
get_hash(const State *state)
{
    // Тело функции
    size_t hash = 0;
    char *buffer = (char *)state;
    for (ssize_t i = 0; i < sizeof(*state); ++ i) {
        // Тело цикла
        hash *= MULTIPLIER;
        hash += buffer[i];
    }
    return hash % HASHTABLE_SIZE;
}

void
print_variable(int64_t var, ssize_t length)
{
    // Тело функции
    char format[MAX_FORMATLEN];
    if (var == UNDEFINED) { // Условный оператор
        // Тело условного оператора
        sprintf(format, "%%%zds", length);
        printf(format, "#");
    } else { // Иначе
        // Тело ветки "иначе" условного оператора
        sprintf(format, "%%%zdlld", length);
        printf(format, var);
    }
}

void
print_state(const Pair pair, PrintStateType print_state_type)
{
    // Тело функции
    State *state = &pair->state;

    switch (print_state_type) {
case NORMAL:
        print_variable(state->c_f, column_width[0]);
        print_variable(state->c_g, column_width[1]);
        print_variable(state->h, column_width[2]);
        print_variable(state->f_x, column_width[3]);
        print_variable(state->f_y, column_width[4]);
        print_variable(state->g_x, column_width[5]);
        print_variable(state->g_y, column_width[6]);
        putchar('\n');
        break;
case LTS:
        printf("%zu [label=\"", (size_t)pair);
        print_variable(state->c_f, 0);
        putchar(',');
        print_variable(state->c_g, 0);
        putchar(',');
        print_variable(state->h, 0);
        putchar(',');
        print_variable(state->f_x, 0);
        putchar(',');
        print_variable(state->f_y, 0);
        putchar(',');
        print_variable(state->g_x, 0);
        putchar(',');
        print_variable(state->g_y, 0);
        puts("\"];");
        break;
default:
        assert(false);
    }
}

void
argparse(ssize_t argc, char **argv)
{
    // Тело функции
    for (ssize_t i = 1; i < argc; ++ i) {
        // Тело цикла
        ssize_t delete_count = 0;
        if (strcmp(argv[i], "--count") == 0 || strcmp(argv[i], "-c") == 0) { // Условный оператор
            // Тело условного оператора
            delete_count = 1;
            only_count = true;
            assert(!only_lts);
        } else if (strcmp(argv[i], "--lts") == 0) { // Условный оператор
            // Тело условного оператора
            delete_count = 1;
            only_lts = true;
            assert(!only_count);
        } else if (strcmp(argv[i], "--file") == 0 || strcmp(argv[i], "-f") == 0) { // Условный оператор
            // Тело условного оператора
            if (i + 1 == argc) { // Условный оператор
                // Тело условного оператора
                fprintf(stderr, "Wrong usage '%s'\n", argv[i]);
                exit(EXIT_FAILURE);
            } else { // Иначе
                // Тело ветки "иначе" условного оператора
                freopen(argv[i + 1], "w", stdout);
                delete_count = 2;
            }
        }

        if (delete_count > 0) { // Условный оператор
            // Тело условного оператора
            for (ssize_t j = i + delete_count; j < argc; ++ j) { // Цикл
                argv[j - delete_count] = argv[j];
            }
            argc -= delete_count;
            -- i;
        }
    }
    if (argc != 5) { // Условный оператор
        // Тело условного оператора
        fprintf(stderr,
                "Reliability model program check\n"
                "Vladimir Lutov <vslutov@yandex.ru> 2016\n"
                "\n"
                "USAGE: %s [options] f_a f_b g_a g_b\n"
                "Available options:\n"
                "              f_a f_b g_a g_b   functions arguments\n"
                "  -f, --file  file              write result to file\n"
                "  -c, --count                   write states count\n"
                "      --lts                     write lts diagram\n"
                "Options --count and --lts are mutually exclusive.\n",
                argv[0]);
        exit(EXIT_FAILURE);
    } else { // Иначе
        // Тело ветки "иначе" условного оператора
        f_a = atoi(argv[1]);
        f_b = atoi(argv[2]);
        g_a = atoi(argv[3]);
        g_b = atoi(argv[4]);
    }
}

void
precalc_header_length(void)
{
    // Тело функции
    for (ssize_t i = 0; i < COLUMN_COUNT; ++ i) {
        column_width[i] = strlen(headers[i]);
    }
    for (ssize_t i = 0; i < HASHTABLE_SIZE; ++ i) {
        if (hashtable[i].init) { // Условный оператор
            // Тело условного оператора
            column_width[0] = max(column_width[0], length_helper(hashtable[i].state.c_f));
            column_width[1] = max(column_width[1], length_helper(hashtable[i].state.c_g));
            column_width[2] = max(column_width[2], length_helper(hashtable[i].state.h));
            column_width[3] = max(column_width[3], length_helper(hashtable[i].state.f_x));
            column_width[4] = max(column_width[4], length_helper(hashtable[i].state.f_y));
            column_width[5] = max(column_width[5], length_helper(hashtable[i].state.g_x));
            column_width[6] = max(column_width[6], length_helper(hashtable[i].state.g_y));
        }
    }
    for (ssize_t i = 0; i < COLUMN_COUNT; ++ i) {
        ++ column_width[i];
    }
}

/**
 * Calc length of integer
 */
int
length_helper(int32_t x)
{
    // Тело функции
    bool negate = false;
    if (x < 0) { // Условный оператор
        // Тело условного оператора
        x = -x;
        negate = true;
    }
    if (x>=100000) { // Условный оператор
        // Тело условного оператора
        if (x>=10000000) { // Условный оператор
            // Тело условного оператора
            if (x>=1000000000) return 10 + negate; // Условный оператор
            if (x>=100000000) return 9 + negate; // Условный оператор
            return 8 + negate;
        }
        if (x>=1000000) return 7 + negate; // Условный оператор
        return 6 + negate; // Условный оператор
    } else { // Иначе
        // Тело ветки "иначе" условного оператора
        if (x>=1000) { // Условный оператор
            // Тело условного оператора
            if (x>=10000) return 5 + negate; // Условный оператор
            return 4 + negate; // Условный оператор
        } else { // Условный оператор
            // Тело условного оператора
            if (x>=100) return 3 + negate; // Условный оператор
            if (x>=10) return 2 + negate; // Условный оператор
            return 1 + negate; // Условный оператор
        }
    }
}

void
output(OutputType output_type)
{
    switch (output_type) {
case BEGIN:
        if (only_lts) {
            puts("strict digraph G {");
        }
        break;
case END:
        // Тело функции
        if (only_count) { // Условный оператор
            printf("%zd\n", state_count);
        } else if (only_lts) { // Условный оператор
            // Тело условного оператора
            for (ssize_t i = 0; i < HASHTABLE_SIZE; ++ i) {
                if (hashtable[i].init) { // Условный оператор
                    print_state(&hashtable[i], LTS);
                }
            }
            puts("}");
        } else {
            // Тело ветки "иначе" условного оператора
            precalc_header_length();

            char format[MAX_FORMATLEN];
            for (ssize_t i = 0; i < COLUMN_COUNT; ++ i) {
                sprintf(format, "%%%zds", column_width[i]);
                printf(format, headers[i]);
            }
            putchar('\n');
            for (ssize_t i = 0; i < HASHTABLE_SIZE; ++ i) {
                if (hashtable[i].init) { // Условный оператор
                    print_state(&hashtable[i], NORMAL);
                }
            }
        }
        break;
default:
        assert(false);
    }
}

Edge
step_f(const Pair current)
{
    // Тело функции
    char *action = "";
    State state = current->state;

    switch (state.c_f) {
case 0:
        action = "";
        state.c_f = 1;
        break;
case 1:
        state.f_x = 9;
        action = "x = 9";
        state.c_f = 2;
        break;
case 2:
        state.f_y = 2;
        action = "y = 2";
        state.c_f = 3;
        break;
case 3:
        state.h = 4;
        action = "h = 4";
        state.c_f = 4;
        break;
case 4:
        if (state.h > state.f_x - f_a) { // Условный оператор
            action = "h > x - a";
            state.c_f = 5;
        } else { // Иначе
            action = "!(h > x - a)";
            state.c_f = 6;
        }
        break;
case 5:
        state.h = state.f_x;
        action = "h = x";
        state.c_f = 6;
        break;
case 6:
        if (state.h > state.f_y - state.f_x) { // Условный оператор
            action = "h > y - x";
            state.c_f = 7;
        } else { // Иначе
            action = "!(h > y - x)";
            state.c_f = 8;
        }
        break;
case 7:
        state.f_x = 8;
        action = "x = 8";
        state.c_f = 8;
        break;
case 8:
        if (state.f_y > 6) { // Условный оператор
            action = "y > 6";
            state.c_f = 9;
        } else { // Иначе
            action = "!(y > 6)";
            state.c_f = 10;
        }
        break;
case 9:
        state.f_y = 1;
        action = "y = 1";
        state.c_f = 10;
        assert(false);
        break;
case 10:
        state.h = f_b;
        action = "h = b";
        state.c_f = 11;
        break;
case 11:
        break;
default:
        assert(false);
    };

    Edge result = {begin: current, end:get_pair(&state), action: action};
    return result;
}

Edge
step_g(const Pair current)
{
    // Тело функции
    State state = current->state;
    char *action = NULL;

    switch (state.c_g) {
case 0:
        state.c_g = 1;
        break;
case 1:
        state.g_x = 2;
        action = "x = 2";
        state.c_g = 2;
        break;
case 2:
        state.g_y = 7;
        action = "y = 7";
        state.c_g = 3;
        break;
case 3:
        state.h = 4;
        action = "h = 4";
        state.c_g = 4;
        break;
case 4:
        state.g_x = 8;
        action = "x = 8";
        state.c_g = 5;
        break;
case 5:
        if (state.g_x > 7) { // Условный оператор
            action = "x > 7";
            state.c_g = 6;
        } else { // Иначе
            action = "!(x > 7)";
            state.c_g = 7;
        }
        break;
case 6:
        state.g_x = 4;
        action = "x = 4";
        state.c_g = 8;
        break;
case 7:
        state.g_y = 1;
        action = "y = 1";
        state.c_g = 8;
        assert(false);
        break;
case 8:
        state.g_y = 6;
        action = "y = 6";
        state.c_g = 9;
        break;
case 9:
        if (state.g_x > 5) { // Условный оператор
            action = "x > 5";
            state.c_g = 10;
        } else { // Иначе
            action = "!(x > 5)";
            state.c_g = 17;
        }
        break;
case 10:
        assert(false);
        if (state.h > 0) { // Условный оператор
            action = "h > 0";
            state.c_g = 11;
        } else { // Иначе
            action = "!(h > 0)";
            state.c_g = 12;
        }
        break;
case 11:
        assert(false);
        state.c_g = 17;
        break;
case 12:
        assert(false);
        if (state.h > 5) { // Условный оператор
            action = "h > 5";
            state.c_g = 13;
        } else { // Иначе
            action = "!(h > 5)";
            state.c_g = 14;
        }
        break;
case 13:
        assert(false);
        state.h = 2;
        action = "h = 2";
        state.c_g = 9;
        break;
case 14:
        assert(false);
        if (state.h < g_a + g_b) { // Условный оператор
            action = "h < a + b";
            state.c_g = 15;
        } else { // Иначе
            action = "!(h < a + b)";
            state.c_g = 16;
        }
        break;
case 15:
        assert(false);
        state.h = 1;
        action = "h = 1";
        state.c_g = 16;
        break;
case 16:
        assert(false);
        state.g_y = 3;
        action = "y = 3";
        state.c_g = 9;
        break;
case 17:
        action = NULL;
        break;
default:
        assert(false);
    };

    Edge result = {begin: current, end: get_pair(&state), action: action};
    return result;
}

void
bfs(void)
{
    // Тело функции
    State init = get_state(0, 0, UNDEFINED, UNDEFINED, UNDEFINED, UNDEFINED, UNDEFINED);
    head = 0;
    tail = 1;
    queue[head] = get_pair(&init);

    while (head < tail) {
        // Тело цикла
        Pair current = queue[head];
        ++ head;

        Edge edge_f = step_f(current);
        if (only_lts && edge_f.action != NULL) {
            printf("%zu -> %zu [label=\"%s\", color=\"red\"];\n", (size_t) edge_f.begin, (size_t) edge_f.end, edge_f.action);
        };

        Pair next = edge_f.end;
        if (next->seen == false) { // Условный оператор
            // Тело условного оператора
            queue[tail] = next;
            next->seen = true;
            ++ tail;
        }

        Edge edge_g = step_g(current);
        if (only_lts && edge_g.action != NULL) {
            printf("%zu -> %zu [label=\"%s\", color=\"blue\"];\n", (size_t) edge_g.begin, (size_t) edge_g.end, edge_g.action);
        };

        next = edge_g.end;
        if (next->seen == false) { // Условный оператор
            // Тело условного оператора
            queue[tail] = next;
            next->seen = true;
            ++ tail;
        }
    }
}

int
main (int argc, char **argv)
{
    // Тело функции
    argparse(argc, argv);

    // Init
    hashtable = calloc(HASHTABLE_SIZE, sizeof(*hashtable));
    queue = calloc(HASHTABLE_SIZE, sizeof(*queue));

    output(BEGIN);
    bfs();
    output(END);

    // Release
    free(hashtable);
    free(queue);
    exit(EXIT_SUCCESS);
}
