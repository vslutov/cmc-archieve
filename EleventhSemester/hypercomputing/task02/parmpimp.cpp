#include <iostream>
#include <vector>
#include <cmath>
#include <memory>
#include <ctime>
#include <mpi.h>
#include <omp.h>

const double PI = 3.141592653589793;

double
sqr(double x) {
  return x * x;
}

const double Lx = PI * 50;
const double Ly = PI * 50;
const double Lz = PI * 50;

const long nelems = NELEMS;
const long Nx = nelems;
const long Ny = nelems;
const long Nz = nelems;
const long Nt = 20;

const double Hx = Lx / (Nx + 1);
const double Hy = Ly / Ny;
const double Hz = Lz / (Nz + 1);
const double Ht = std::min(std::min(Hx, Hy), Hz) * 0.5;
const double Lt = Ht * Nt;

static long Px, Py, Pz;
static long Mx, My, Mz;
static long DCx, DCy, DCz;

const double A = std::sqrt(sqr(4) + sqr(1) + sqr(2));

static inline double
u(double x, double y, double z, double t)
{
  return std::sin(4 * x) * std::sin(z) * std::cos(A * t + 2 * y);
}

static std::vector<double> buffer;

struct Layer {
public:
  Layer() :
    data(DCx * DCy * DCz),
    px(DCy * DCz),
    nx(DCy * DCz),
    py(DCx * DCz),
    ny(DCx * DCz),
    pz(DCx * DCy),
    nz(DCx * DCy)
  {
  }

  void
  set(long i, long j, long k, double v) {
    data[(i * DCy + j) * DCz + k] = v;
  }

  double
  operator()(long i, long j, long k) const {
    if (i < 0) {
      return px[j * DCz + k];
    }

    if (i >= DCx) {
      return nx[j * DCz + k];
    }

    if (j < 0) {
      return py[i * DCz + k];
    }

    if (j >= DCy) {
      return ny[i * DCz + k];
    }

    if (k < 0) {
      return pz[i * DCy + j];
    }

    if (k >= DCz) {
      return nz[i * DCy + j];
    }

    return data[(i * DCy + j) * DCz + k];
  }

  std::vector<double> data, px, nx, py, ny, pz, nz;
};

static inline Layer
init_prev(long Sx, long Sy, long Sz)
{
  Layer p;
  for (long i = 0; i < DCx; ++ i) {
    for (long j = 0; j < DCy; ++ j) {
      for (long k = 0; k < DCz; ++ k) {
        p.set(i, j, k, u((i + 1 + Sx) * Hx, (j + Sy) * Hy, (k + 1 + Sz) * Hz, -Ht));
      }
    }
  }
  return p;
}

static inline Layer
init_current(long Sx, long Sy, long Sz)
{
  Layer c;
  for (long i = 0; i < DCx; ++ i) {
    for (long j = 0; j < DCy; ++ j) {
      for (long k = 0; k < DCz; ++ k) {
        c.set(i, j, k, u((i + 1 + Sx) * Hx, (j + Sy) * Hy, (k + 1 + Sz) * Hz, 0));
      }
    }
  }
  return c;
}

// y transport
static inline void
send_y_forward(const Layer &layer)
{
  #pragma omp parallel for
  for (long i = 0; i < DCx; ++ i) {
    for (long k = 0; k < DCz; ++ k) {
      buffer[i * DCz + k] = layer(i, DCy - 1, k);
    }
  }

  const long reciever = (Mx * Py + (My + 1) % Py) * Pz + Mz;
  MPI_Send(buffer.data(), layer.ny.size(), MPI_DOUBLE, reciever, 0, MPI_COMM_WORLD);
}

static inline void
recieve_y_forward(Layer &layer)
{
  const long sender = (Mx * Py + (Py + My - 1) % Py) * Pz + Mz;
  MPI_Recv(layer.py.data(), layer.py.size(), MPI_DOUBLE, sender, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}

static inline void
send_y_backward(const Layer &layer)
{
  #pragma omp parallel for
  for (long i = 0; i < DCx; ++ i) {
    for (long k = 0; k < DCz; ++ k) {
      buffer[i * DCz + k] = layer(i, 0, k);
    }
  }

  const long reciever = (Mx * Py + (Py + My - 1) % Py) * Pz + Mz;
  MPI_Send(buffer.data(), layer.py.size(), MPI_DOUBLE, reciever, 0, MPI_COMM_WORLD);
}

static inline void
recieve_y_backward(Layer &layer)
{
  const long sender = (Mx * Py + (My + 1) % Py) * Pz + Mz;
  MPI_Recv(layer.ny.data(), layer.ny.size(), MPI_DOUBLE, sender, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}

static inline void
contact_y_forward(Layer &layer)
{
  send_y_forward(layer);
  recieve_y_backward(layer);
}

static inline void
contact_y_backward(Layer &layer)
{
  recieve_y_forward(layer);
  send_y_backward(layer);
}

// x transport
static inline void
send_x_forward(const Layer &layer)
{
  #pragma omp parallel for
  for (long j = 0; j < DCy; ++ j) {
    for (long k = 0; k < DCz; ++ k) {
      buffer[j * DCz + k] = layer(DCx - 1, j, k);
    }
  }

  const long reciever = ((Mx + 1) % Px * Py + My) * Pz + Mz;
  MPI_Send(buffer.data(), layer.nx.size(), MPI_DOUBLE, reciever, 0, MPI_COMM_WORLD);
}

static inline void
recieve_x_forward(Layer &layer)
{
  const long sender = ((Px + Mx - 1) % Px * Py + My) * Pz + Mz;
  MPI_Recv(layer.px.data(), layer.px.size(), MPI_DOUBLE, sender, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}

static inline void
send_x_backward(const Layer &layer)
{
  #pragma omp parallel for
  for (long j = 0; j < DCy; ++ j) {
    for (long k = 0; k < DCz; ++ k) {
      buffer[j * DCz + k] = layer(0, j, k);
    }
  }

  const long reciever = ((Px + Mx - 1) % Px * Py + My) * Pz + Mz;
  MPI_Send(buffer.data(), layer.px.size(), MPI_DOUBLE, reciever, 0, MPI_COMM_WORLD);
}

static inline void
recieve_x_backward(Layer &layer)
{
  const long sender = ((Mx + 1) % Px * Py + My) * Pz + Mz;
  MPI_Recv(layer.nx.data(), layer.nx.size(), MPI_DOUBLE, sender, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}

static inline void
contact_x_forward(Layer &layer)
{
  send_x_forward(layer);
  recieve_x_backward(layer);
}

static inline void
contact_x_backward(Layer &layer)
{
  recieve_x_forward(layer);
  send_x_backward(layer);
}

// x transport
static inline void
send_z_forward(const Layer &layer)
{
  #pragma omp parallel for
  for (long i = 0; i < DCx; ++ i) {
    for (long j = 0; j < DCy; ++ j) {
      buffer[i * DCy + j] = layer(i, j, DCz - 1);
    }
  }

  const long reciever = (Mx * Py + My) * Pz + (Mz + 1) % Pz;
  MPI_Send(buffer.data(), layer.nz.size(), MPI_DOUBLE, reciever, 0, MPI_COMM_WORLD);
}

static inline void
recieve_z_forward(Layer &layer)
{
  const long sender = (Mx * Py + My) * Pz + (Pz + Mz - 1) % Pz;
  MPI_Recv(layer.pz.data(), layer.pz.size(), MPI_DOUBLE, sender, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}

static inline void
send_z_backward(const Layer &layer)
{
  #pragma omp parallel for
  for (long i = 0; i < DCx; ++ i) {
    for (long j = 0; j < DCy; ++ j) {
      buffer[i * DCy + j] = layer(i, j, 0);
    }
  }

  const long reciever = (Mx * Py + My) * Pz + (Pz + Mz - 1) % Pz;
  MPI_Send(buffer.data(), layer.pz.size(), MPI_DOUBLE, reciever, 0, MPI_COMM_WORLD);
}

static inline void
recieve_z_backward(Layer &layer)
{
  const long sender = (Mx * Py + My) * Pz + (Mz + 1) % Pz;
  MPI_Recv(layer.nz.data(), layer.nz.size(), MPI_DOUBLE, sender, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}

static inline void
contact_z_forward(Layer &layer)
{
  send_z_forward(layer);
  recieve_z_backward(layer);
}

static inline void
contact_z_backward(Layer &layer)
{
  recieve_z_forward(layer);
  send_z_backward(layer);
}

static inline void
sync(Layer &layer)
{
  // Transport along y axis
  if (Py % 2) {
    if (My % 2) {
      contact_y_backward(layer);
      contact_y_forward(layer);
    } else {
      if (My != Py - 1) {
        contact_y_forward(layer);
      }
      if (My != 0) {
        contact_y_backward(layer);
      }

      if (My == Py - 1) {
        contact_y_forward(layer);
      }
      if (My == 0) {
        contact_y_backward(layer);
      }
    }
  } else {
    if (My % 2) {
      contact_y_backward(layer);
      contact_y_forward(layer);
    } else {
      contact_y_forward(layer);
      contact_y_backward(layer);
    }
  }

  // Transport along x axis
  if (Mx % 2) {
    contact_x_backward(layer);
    if (Mx != Px - 1) {
      contact_x_forward(layer);
    }
  } else {
    if (Mx != Px - 1) {
      contact_x_forward(layer);
    }
    if (Mx != 0) {
      contact_x_backward(layer);
    }
  }

  // Transport along z axis
  if (Mz % 2) {
    contact_z_backward(layer);
    if (Mz != Pz - 1) {
      contact_z_forward(layer);
    }
  } else {
    if (Mz != Pz - 1) {
      contact_z_forward(layer);
    }
    if (Mz != 0) {
      contact_z_backward(layer);
    }
  }
}

static inline void
calc_next_layer(const Layer &p, const Layer &c, Layer &n)
{
  #pragma omp parallel for
  for (long i = 0; i < DCx; ++ i) {
    for (long j = 0; j < DCy; ++ j) {
      for (long k = 0; k < DCz; ++ k) {
        double c_val = 2 * c(i, j, k);
        double d2u_dx2 = (c(i - 1, j, k) + c(i + 1, j, k) - c_val) / sqr(Hx);
        double d2u_dy2 = (c(i, j - 1, k) + c(i, j + 1, k) - c_val) / sqr(Hy);
        double d2u_dz2 = (c(i, j, k - 1) + c(i, j, k + 1) - c_val) / sqr(Hz);
        double u_H = d2u_dx2 + d2u_dy2 + d2u_dz2;

        n.set(i, j, k, c_val - p(i, j, k) + u_H * sqr(Ht));
      }
    }
  }
  sync(n);
}

static inline double
raw_evaluate(long Sx, long Sy, long Sz, const Layer &layer, double t) {
  double l2 = 0;
  for (long i = 0; i < DCx; ++ i) {
    for (long j = 0; j < DCy; ++ j) {
      for (long k = 0; k < DCz; ++ k) {
        l2 += sqr(layer(i, j, k) - u((i + 1 + Sx) * Hx, (j + Sy) * Hy, (k + 1 + Sz) * Hz, t));
      }
    }
  }
  return l2 / Nx / Ny / Nz;
}

static inline double
evaluate(long world_size, const Layer &layer, double t=Lt)
{
  double l2 = raw_evaluate(0, 0, 0, layer, t);
  for (long i = 1; i < world_size; ++ i) {
    double elem;
    MPI_Recv(&elem, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    l2 += elem;
  }
  return l2;
}

static inline void
help_evaluate(long Sx, long Sy, long Sz, const Layer &layer, double t=Lt)
{
  double l2 = raw_evaluate(Sx, Sy, Sz, layer, t);
  MPI_Send(&l2, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
}

static inline void
calc_last_layer(Layer *p, Layer *c, Layer *n)
{
  Layer *t;
  for (long i = 0; i < Nt; ++ i) {
    calc_next_layer(*p, *c, *n);
    t = p;
    p = c;
    c = n;
    n = t;
  }
}

// Utils
static inline int
npow(int e, long p)
{
  int val = 1;
  for (long i = 0; i < p; ++ i) {
    val *= e;
  }
  return val;
}

static inline long
root(int x, long base)
{
  long a = 1;
  for(; npow(a, base) <= x; ++a) {
  }
  return a - 1;
}

int
main(int argc, char **argv)
{
  std::ios_base::sync_with_stdio(false);

  // mpi init
  MPI_Init(&argc, &argv);
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // Init coords
  // World size
  switch (world_size) {
    case 32:
      Px = 4; Py = 4; Pz = 2; break;
    case 64:
      Px = 4; Py = 4; Pz = 4; break;
    case 128:
      Px = 4; Py = 4; Pz = 8; break;
    case 256:
      Px = 8; Py = 4; Pz = 8; break;
    case 512:
      Px = 8; Py = 8; Pz = 8; break;
    case 1024:
      Px = 8; Py = 8; Pz = 16; break;
    case 2048:
      Px = 8; Py = 16; Pz = 16; break;
    default:
      Px = root(world_size, 3);
      Py = root(world_size / Px, 2);
      Pz = world_size / (Px * Py);
  }

  // Process coord in world
  Mx = world_rank % Px;
  My = world_rank / Px % Py;
  Mz = world_rank / (Py * Px);

  long process_limit = Px * Py * Pz;
  if (world_size != process_limit)
  {
    if (world_rank == 0) {
      std::cerr << "Please run this program with " << process_limit << " MPI processes" << std::endl;
    }
    MPI_Finalize();
    return 1;
  }

  // World cell size
  const long Dx = Nx / Px + (Nx % Px != 0);
  const long Dy = Ny / Py + (Ny % Py != 0);
  const long Dz = Nz / Pz + (Nz % Pz != 0);

  // Current cell size
  DCx = Mx < Px - 1 ? Dx : Nx - Mx * Dx;
  DCy = My < Py - 1 ? Dy : Ny - My * Dy;
  DCz = Mz < Pz - 1 ? Dz : Nz - Mz * Dz;
  const long max_size = (DCx >= DCy && DCx >= DCz) ? DCx : (
                            (DCy >= DCz) ? DCy : DCz
                           );

  // Init buffer
  buffer.resize(max_size * max_size);

  // Init layers
  Layer c = init_current(Mx * Dx, My * Dy, Mz * Dz);
  sync(c);
  Layer p = init_prev(Mx * Dx, My * Dy, Mz * Dz);
  sync(p);
  Layer n;

  MPI_Barrier(MPI_COMM_WORLD);
  volatile clock_t clock_count = std::clock();
  calc_last_layer(&p, &c, &n);
  MPI_Barrier(MPI_COMM_WORLD);
  clock_count = std::clock() - clock_count;

  double seconds = static_cast<double>(clock_count) / CLOCKS_PER_SEC;

  if (world_rank == 0) {
    std::cout << world_size << "," << nelems << "," << seconds << "," << 0 << "," << evaluate(Px * Py * Pz, c) << std::endl;
  } else {
    help_evaluate(Mx * Dx, My * Dy, Mz * Dz, c);
  }

  // release
  MPI_Finalize();
}
