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
const double Lt = PI * 0.32;

const long nelems = NELEMS;
const long Nx = nelems;
const long Ny = nelems;
const long Nz = nelems;
const long Nt = 20;

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

static std::vector<double> buffer;

struct Layer {
public:
  Layer(long _DCx, long _DCy, long _DCz) :
    DCx(_DCx),
    DCy(_DCy),
    DCz(_DCz),
    data(_DCx * _DCy * _DCz),
    px(_DCy * _DCz),
    nx(_DCy * _DCz),
    py(_DCx * _DCz),
    ny(_DCx * _DCz),
    pz(_DCx * _DCy),
    nz(_DCx * _DCy)
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

  const long DCx, DCy, DCz;
  std::vector<double> data, px, nx, py, ny, pz, nz;
};

static inline Layer
init_layer(long DCx, long DCy, long DCz)
{
  return Layer(DCx, DCy, DCz);
}

static inline Layer
init_prev(long Sx, long Sy, long Sz, long DCx, long DCy, long DCz)
{
  Layer p = init_layer(DCx, DCy, DCz);
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
init_current(long Sx, long Sy, long Sz, long DCx, long DCy, long DCz)
{
  Layer c = init_layer(DCx, DCy, DCz);
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
send_y_forward(long Mx, long My, long Mz, long Px, long Py, long Pz, const Layer &layer)
{
  #pragma omp parallel for
  for (long i = 0; i < layer.DCx; ++ i) {
    for (long k = 0; k < layer.DCz; ++ k) {
      buffer[i * layer.DCz + k] = layer(i, layer.DCy - 1, k);
    }
  }

  const long reciever = (Mx * Py + (My + 1) % Py) * Pz + Mz;
  MPI_Send(buffer.data(), layer.ny.size(), MPI_DOUBLE, reciever, 0, MPI_COMM_WORLD);
}

static inline void
recieve_y_forward(long Mx, long My, long Mz, long Px, long Py, long Pz, Layer &layer)
{
  const long sender = (Mx * Py + (Py + My - 1) % Py) * Pz + Mz;
  MPI_Recv(layer.py.data(), layer.py.size(), MPI_DOUBLE, sender, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}

static inline void
send_y_backward(long Mx, long My, long Mz, long Px, long Py, long Pz, const Layer &layer)
{
  #pragma omp parallel for
  for (long i = 0; i < layer.DCx; ++ i) {
    for (long k = 0; k < layer.DCz; ++ k) {
      buffer[i * layer.DCz + k] = layer(i, 0, k);
    }
  }

  const long reciever = (Mx * Py + (Py + My - 1) % Py) * Pz + Mz;
  MPI_Send(buffer.data(), layer.py.size(), MPI_DOUBLE, reciever, 0, MPI_COMM_WORLD);
}

static inline void
recieve_y_backward(long Mx, long My, long Mz, long Px, long Py, long Pz, Layer &layer)
{
  const long sender = (Mx * Py + (My + 1) % Py) * Pz + Mz;
  MPI_Recv(layer.ny.data(), layer.ny.size(), MPI_DOUBLE, sender, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}

static inline void
contact_y_forward(long Mx, long My, long Mz, long Px, long Py, long Pz, Layer &layer)
{
  send_y_forward(Mx, My, Mz, Px, Py, Pz, layer);
  recieve_y_backward(Mx, My, Mz, Px, Py, Pz, layer);
}

static inline void
contact_y_backward(long Mx, long My, long Mz, long Px, long Py, long Pz, Layer &layer)
{
  recieve_y_forward(Mx, My, Mz, Px, Py, Pz, layer);
  send_y_backward(Mx, My, Mz, Px, Py, Pz, layer);
}

// x transport
static inline void
send_x_forward(long Mx, long My, long Mz, long Px, long Py, long Pz, const Layer &layer)
{
  #pragma omp parallel for
  for (long j = 0; j < layer.DCy; ++ j) {
    for (long k = 0; k < layer.DCz; ++ k) {
      buffer[j * layer.DCz + k] = layer(layer.DCx - 1, j, k);
    }
  }

  const long reciever = ((Mx + 1) % Px * Py + My) * Pz + Mz;
  MPI_Send(buffer.data(), layer.nx.size(), MPI_DOUBLE, reciever, 0, MPI_COMM_WORLD);
}

static inline void
recieve_x_forward(long Mx, long My, long Mz, long Px, long Py, long Pz, Layer &layer)
{
  const long sender = ((Px + Mx - 1) % Px * Py + My) * Pz + Mz;
  MPI_Recv(layer.px.data(), layer.px.size(), MPI_DOUBLE, sender, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}

static inline void
send_x_backward(long Mx, long My, long Mz, long Px, long Py, long Pz, const Layer &layer)
{
  #pragma omp parallel for
  for (long j = 0; j < layer.DCy; ++ j) {
    for (long k = 0; k < layer.DCz; ++ k) {
      buffer[j * layer.DCz + k] = layer(0, j, k);
    }
  }

  const long reciever = ((Px + Mx - 1) % Px * Py + My) * Pz + Mz;
  MPI_Send(buffer.data(), layer.px.size(), MPI_DOUBLE, reciever, 0, MPI_COMM_WORLD);
}

static inline void
recieve_x_backward(long Mx, long My, long Mz, long Px, long Py, long Pz, Layer &layer)
{
  const long sender = ((Mx + 1) % Px * Py + My) * Pz + Mz;
  MPI_Recv(layer.nx.data(), layer.nx.size(), MPI_DOUBLE, sender, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}

static inline void
contact_x_forward(long Mx, long My, long Mz, long Px, long Py, long Pz, Layer &layer)
{
  send_x_forward(Mx, My, Mz, Px, Py, Pz, layer);
  recieve_x_backward(Mx, My, Mz, Px, Py, Pz, layer);
}

static inline void
contact_x_backward(long Mx, long My, long Mz, long Px, long Py, long Pz, Layer &layer)
{
  recieve_x_forward(Mx, My, Mz, Px, Py, Pz, layer);
  send_x_backward(Mx, My, Mz, Px, Py, Pz, layer);
}

// x transport
static inline void
send_z_forward(long Mx, long My, long Mz, long Px, long Py, long Pz, const Layer &layer)
{
  #pragma omp parallel for
  for (long i = 0; i < layer.DCx; ++ i) {
    for (long j = 0; j < layer.DCy; ++ j) {
      buffer[i * layer.DCy + j] = layer(i, j, layer.DCz - 1);
    }
  }

  const long reciever = (Mx * Py + My) * Pz + (Mz + 1) % Pz;
  MPI_Send(buffer.data(), layer.nz.size(), MPI_DOUBLE, reciever, 0, MPI_COMM_WORLD);
}

static inline void
recieve_z_forward(long Mx, long My, long Mz, long Px, long Py, long Pz, Layer &layer)
{
  const long sender = (Mx * Py + My) * Pz + (Pz + Mz - 1) % Pz;
  MPI_Recv(layer.pz.data(), layer.pz.size(), MPI_DOUBLE, sender, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}

static inline void
send_z_backward(long Mx, long My, long Mz, long Px, long Py, long Pz, const Layer &layer)
{
  #pragma omp parallel for
  for (long i = 0; i < layer.DCx; ++ i) {
    for (long j = 0; j < layer.DCy; ++ j) {
      buffer[i * layer.DCy + j] = layer(i, j, 0);
    }
  }

  const long reciever = (Mx * Py + My) * Pz + (Pz + Mz - 1) % Pz;
  MPI_Send(buffer.data(), layer.pz.size(), MPI_DOUBLE, reciever, 0, MPI_COMM_WORLD);
}

static inline void
recieve_z_backward(long Mx, long My, long Mz, long Px, long Py, long Pz, Layer &layer)
{
  const long sender = (Mx * Py + My) * Pz + (Mz + 1) % Pz;
  MPI_Recv(layer.nz.data(), layer.nz.size(), MPI_DOUBLE, sender, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}

static inline void
contact_z_forward(long Mx, long My, long Mz, long Px, long Py, long Pz, Layer &layer)
{
  send_z_forward(Mx, My, Mz, Px, Py, Pz, layer);
  recieve_z_backward(Mx, My, Mz, Px, Py, Pz, layer);
}

static inline void
contact_z_backward(long Mx, long My, long Mz, long Px, long Py, long Pz, Layer &layer)
{
  recieve_z_forward(Mx, My, Mz, Px, Py, Pz, layer);
  send_z_backward(Mx, My, Mz, Px, Py, Pz, layer);
}

static inline void
sync(long Mx, long My, long Mz, long Px, long Py, long Pz, Layer &layer)
{
  // Transport along y axis
  if (Py % 2) {
    if (My % 2) {
      contact_y_backward(Mx, My, Mz, Px, Py, Pz, layer);
      contact_y_forward(Mx, My, Mz, Px, Py, Pz, layer);
    } else {
      if (My != Py - 1) {
        contact_y_forward(Mx, My, Mz, Px, Py, Pz, layer);
      }
      if (My != 0) {
        contact_y_backward(Mx, My, Mz, Px, Py, Pz, layer);
      }

      if (My == Py - 1) {
        contact_y_forward(Mx, My, Mz, Px, Py, Pz, layer);
      }
      if (My == 0) {
        contact_y_backward(Mx, My, Mz, Px, Py, Pz, layer);
      }
    }
  } else {
    if (My % 2) {
      contact_y_backward(Mx, My, Mz, Px, Py, Pz, layer);
      contact_y_forward(Mx, My, Mz, Px, Py, Pz, layer);
    } else {
      contact_y_forward(Mx, My, Mz, Px, Py, Pz, layer);
      contact_y_backward(Mx, My, Mz, Px, Py, Pz, layer);
    }
  }

  // Transport along x axis
  if (Mx % 2) {
    contact_x_backward(Mx, My, Mz, Px, Py, Pz, layer);
    if (Mx != Px - 1) {
      contact_x_forward(Mx, My, Mz, Px, Py, Pz, layer);
    }
  } else {
    if (Mx != Px - 1) {
      contact_x_forward(Mx, My, Mz, Px, Py, Pz, layer);
    }
    if (Mx != 0) {
      contact_x_backward(Mx, My, Mz, Px, Py, Pz, layer);
    }
  }

  // Transport along z axis
  if (Mz % 2) {
    contact_z_backward(Mx, My, Mz, Px, Py, Pz, layer);
    if (Mz != Pz - 1) {
      contact_z_forward(Mx, My, Mz, Px, Py, Pz, layer);
    }
  } else {
    if (Mz != Pz - 1) {
      contact_z_forward(Mx, My, Mz, Px, Py, Pz, layer);
    }
    if (Mz != 0) {
      contact_z_backward(Mx, My, Mz, Px, Py, Pz, layer);
    }
  }
}

static inline void
calc_next_layer(long Mx, long My, long Mz, long Px, long Py, long Pz, const Layer &p, const Layer &c, Layer &n)
{
  #pragma omp parallel for
  for (long i = 0; i < n.DCx; ++ i) {
    for (long j = 0; j < n.DCy; ++ j) {
      for (long k = 0; k < n.DCz; ++ k) {
        double c_val = 2 * c(i, j, k);
        double d2u_dx2 = (c(i - 1, j, k) + c(i + 1, j, k) - c_val) / sqr(Hx);
        double d2u_dy2 = (c(i, j - 1, k) + c(i, j + 1, k) - c_val) / sqr(Hy);
        double d2u_dz2 = (c(i, j, k - 1) + c(i, j, k + 1) - c_val) / sqr(Hz);
        double u_H = d2u_dx2 + d2u_dy2 + d2u_dz2;

        n.set(i, j, k, c_val - p(i, j, k) + u_H * sqr(Ht));
      }
    }
  }
  sync(Mx, My, Mz, Px, Py, Pz, n);
}

static inline double
raw_evaluate(long Sx, long Sy, long Sz, const Layer &layer, double t) {
  double l2 = 0;
  for (long i = 0; i < layer.DCx; ++ i) {
    for (long j = 0; j < layer.DCy; ++ j) {
      for (long k = 0; k < layer.DCz; ++ k) {
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
calc_last_layer(long Mx, long My, long Mz, long Px, long Py, long Pz, Layer *p, Layer *c, Layer *n)
{
  Layer *t;
  for (long i = 0; i < Nt; ++ i) {
    calc_next_layer(Mx, My, Mz, Px, Py, Pz, *p, *c, *n);
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
  const long Px = root(world_size, 3);
  const long Py = root(world_size / Px, 2);
  const long Pz = Py;

  // Process coord in world
  const long Mx = world_rank / (Py * Pz);
  const long My = (world_rank % (Py * Pz) / Pz);
  const long Mz = world_rank % Pz;

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
  const long DCx = Mx < Px - 1 ? Dx : Nx - Mx * Dx;
  const long DCy = My < Py - 1 ? Dy : Ny - My * Dy;
  const long DCz = Mz < Pz - 1 ? Dz : Nz - Mz * Dz;
  const long max_size = (DCx >= DCy && DCx >= DCz) ? DCx : (
                            (DCy >= DCz) ? DCy : DCz
                           );

  // Init buffer
  buffer.resize(max_size * max_size);

  // Init layers
  Layer c = init_current(Mx * Dx, My * Dy, Mz * Dz, DCx, DCy, DCz);
  sync(Mx, My, Mz, Px, Py, Pz, c);
  Layer p = init_prev(Mx * Dx, My * Dy, Mz * Dz, DCx, DCy, DCz);
  sync(Mx, My, Mz, Px, Py, Pz, p);
  Layer n = init_layer(DCx, DCy, DCz);

  MPI_Barrier(MPI_COMM_WORLD);
  volatile clock_t clock_count = std::clock();
  calc_last_layer(Mx, My, Mz, Px, Py, Pz, &p, &c, &n);
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
