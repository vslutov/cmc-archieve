#include <iostream>
#include <vector>
#include <cmath>
#include <memory>
#include <ctime>

const double PI = 3.141592653589793;

double
sqr(double x) {
  return x * x;
}

const double Lx = PI * 50;
const double Ly = PI * 50;
const double Lz = PI * 50;
const double Lt = PI * 0.32;

const ssize_t nelems = NELEMS;
const ssize_t Nx = nelems;
const ssize_t Ny = nelems;
const ssize_t Nz = nelems;
const ssize_t Nt = 20;

const double Hx = Lx / (Nx + 1);
const double Hy = Ly / Ny;
const double Hz = Lz / (Nz + 1);
const double Ht = Lt / Nt;

const double A = std::sqrt(sqr(2 * PI / Lx) + sqr(2 * PI / Ly) + sqr(PI / Lz));

static inline double
u(double x, double y, double z, double t)
{
  return std::sin(2 * PI / Lx * x) * std::sin(PI / Lz * z) * std::cos(A * t + 2 * PI / Ly * y);
}

struct Layer {
public:
  Layer() :
    data(Nx * Ny * Nz)
  {
  }

  void
  set(ssize_t i, ssize_t j, ssize_t k, double v) {
    this->data[i * Ny * Nz + j * Nz + k] = v;
  }

  double
  operator()(ssize_t i, ssize_t j, ssize_t k) const {
    if (i < 0 || i >= Nx || k < 0 || k >= Nz) {
      return 0;
    }

    if (j < 0) {
      j = Ny - 1;
    } else if (j >= Ny) {
      j = 0;
    }

    return this->data[i * Ny * Nz + j * Nz + k];
  }

private:
  std::vector<double> data;
};

#define LayerPtr Layer *

static inline LayerPtr
init_layer()
{
  return new Layer();
}

static inline LayerPtr
init_prev()
{
  LayerPtr p = init_layer();
  for (ssize_t i = 0; i < Nx; ++ i) {
    for (ssize_t j = 0; j < Ny; ++ j) {
      for (ssize_t k = 0; k < Nz; ++ k) {
        p->set(i, j, k, u((i + 1) * Hx, j * Hy, (k + 1) * Hz, -Ht));
      }
    }
  }
  return p;
}

static inline LayerPtr
init_current()
{
  LayerPtr c = init_layer();
  for (ssize_t i = 0; i < Nx; ++ i) {
    for (ssize_t j = 0; j < Ny; ++ j) {
      for (ssize_t k = 0; k < Nz; ++ k) {
        c->set(i, j, k, u((i + 1) * Hx, j * Hy, (k + 1) * Hz, 0));
      }
    }
  }
  return c;
}

static inline void
calc_next_layer(const Layer &p, const Layer &c, Layer &n)
{
  for (ssize_t i = 0; i < Nx; ++ i) {
    for (ssize_t j = 0; j < Ny; ++ j) {
      for (ssize_t k = 0; k < Nz; ++ k) {
        double c_val = 2 * c(i, j, k);
        double d2u_dx2 = (c(i - 1, j, k) + c(i + 1, j, k) - c_val) / sqr(Hx);
        double d2u_dy2 = (c(i, j - 1, k) + c(i, j + 1, k) - c_val) / sqr(Hy);
        double d2u_dz2 = (c(i, j, k - 1) + c(i, j, k + 1) - c_val) / sqr(Hz);
        double u_H = d2u_dx2 + d2u_dy2 + d2u_dz2;

        n.set(i, j, k, c_val - p(i, j, k) + u_H * sqr(Ht));
      }
    }
  }
}

static inline double
evaluate(const Layer &layer, double t=Lt)
{
  double l2 = 0;
  for (ssize_t i = 0; i < Nx; ++ i) {
    for (ssize_t j = 0; j < Ny; ++ j) {
      for (ssize_t k = 0; k < Nz; ++ k) {
        l2 += sqr(layer(i, j, k) - u((i + 1) * Hx, j * Hy, (k + 1) * Hz, t));
      }
    }
  }

  return l2;
}

static inline void
calc_last_layer(LayerPtr &p, LayerPtr &c, LayerPtr &n)
{
  LayerPtr t;
  for (ssize_t i = 0; i < Nt; ++ i) {
    calc_next_layer(*p, *c, *n);
    t = p;
    p = c;
    c = n;
    n = t;
  }
}

int
main(void)
{
  std::ios_base::sync_with_stdio(false);

  LayerPtr p = init_prev();
  LayerPtr c = init_current();
  LayerPtr n = init_layer();

  volatile clock_t clock_count = std::clock();
  calc_last_layer(p, c, n);
  clock_count = std::clock() - clock_count;

  double seconds = static_cast<double>(clock_count) / CLOCKS_PER_SEC;

  std::cout << nelems << "," << seconds << "," << 0 << "," << evaluate(*c) << std::endl;

  delete p;
  delete c;
  delete n;
}
