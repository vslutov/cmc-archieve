#include <iostream>
#include <vector>
#include <cmath>
#include <memory>
#include <ctime>
#include <mpi.h>

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

static std::vector<double> buffer;

struct Layer {
public:
  Layer(ssize_t _DCx, ssize_t _DCy, ssize_t _DCz) :
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
  set(ssize_t i, ssize_t j, ssize_t k, double v) {
    data[(i * DCy + j) * DCz + k] = v;
  }

  double
  operator()(ssize_t i, ssize_t j, ssize_t k) const {
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

  const ssize_t DCx, DCy, DCz;
  std::vector<double> data, px, nx, py, ny, pz, nz;
};

static inline Layer
init_layer(ssize_t DCx, ssize_t DCy, ssize_t DCz)
{
  return Layer(DCx, DCy, DCz);
}

static inline Layer
init_prev(ssize_t Sx, ssize_t Sy, ssize_t Sz, ssize_t DCx, ssize_t DCy, ssize_t DCz)
{
  Layer p = init_layer(DCx, DCy, DCz);
  for (ssize_t i = 0; i < DCx; ++ i) {
    for (ssize_t j = 0; j < DCy; ++ j) {
      for (ssize_t k = 0; k < DCz; ++ k) {
        p.set(i, j, k, u((i + 1 + Sx) * Hx, (j + Sy) * Hy, (k + 1 + Sz) * Hz, -Ht));
      }
    }
  }
  return p;
}

static inline Layer
init_current(ssize_t Sx, ssize_t Sy, ssize_t Sz, ssize_t DCx, ssize_t DCy, ssize_t DCz)
{
  Layer c = init_layer(DCx, DCy, DCz);
  for (ssize_t i = 0; i < DCx; ++ i) {
    for (ssize_t j = 0; j < DCy; ++ j) {
      for (ssize_t k = 0; k < DCz; ++ k) {
        c.set(i, j, k, u((i + 1 + Sx) * Hx, (j + Sy) * Hy, (k + 1 + Sz) * Hz, 0));
      }
    }
  }
  return c;
}

// y transport
static inline void
send_y_forward(ssize_t Mx, ssize_t My, ssize_t Mz, ssize_t Px, ssize_t Py, ssize_t Pz, const Layer &layer)
{
  // move last y layer to buffer
  for (ssize_t i = 0; i < layer.DCx; ++ i) {
    for (ssize_t k = 0; k < layer.DCz; ++ k) {
      buffer[i * layer.DCz + k] = layer(i, layer.DCy - 1, k);
    }
  }

  const ssize_t reciever = (Mx * Py + (My + 1) % Py) * Pz + Mz;
  MPI_Send(buffer.data(), layer.ny.size(), MPI_DOUBLE, reciever, 0, MPI_COMM_WORLD);
}

static inline void
recieve_y_forward(ssize_t Mx, ssize_t My, ssize_t Mz, ssize_t Px, ssize_t Py, ssize_t Pz, Layer &layer)
{
  const ssize_t sender = (Mx * Py + (Py + My - 1) % Py) * Pz + Mz;
  MPI_Recv(layer.py.data(), layer.py.size(), MPI_DOUBLE, sender, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}

static inline void
send_y_backward(ssize_t Mx, ssize_t My, ssize_t Mz, ssize_t Px, ssize_t Py, ssize_t Pz, const Layer &layer)
{
  // move first y layer to buffer
  for (ssize_t i = 0; i < layer.DCx; ++ i) {
    for (ssize_t k = 0; k < layer.DCz; ++ k) {
      buffer[i * layer.DCz + k] = layer(i, 0, k);
    }
  }

  const ssize_t reciever = (Mx * Py + (Py + My - 1) % Py) * Pz + Mz;
  MPI_Send(buffer.data(), layer.py.size(), MPI_DOUBLE, reciever, 0, MPI_COMM_WORLD);
}

static inline void
recieve_y_backward(ssize_t Mx, ssize_t My, ssize_t Mz, ssize_t Px, ssize_t Py, ssize_t Pz, Layer &layer)
{
  const ssize_t sender = (Mx * Py + (My + 1) % Py) * Pz + Mz;
  MPI_Recv(layer.ny.data(), layer.ny.size(), MPI_DOUBLE, sender, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}

static inline void
contact_y_forward(ssize_t Mx, ssize_t My, ssize_t Mz, ssize_t Px, ssize_t Py, ssize_t Pz, Layer &layer)
{
  send_y_forward(Mx, My, Mz, Px, Py, Pz, layer);
  recieve_y_backward(Mx, My, Mz, Px, Py, Pz, layer);
}

static inline void
contact_y_backward(ssize_t Mx, ssize_t My, ssize_t Mz, ssize_t Px, ssize_t Py, ssize_t Pz, Layer &layer)
{
  recieve_y_forward(Mx, My, Mz, Px, Py, Pz, layer);
  send_y_backward(Mx, My, Mz, Px, Py, Pz, layer);
}

// x transport
static inline void
send_x_forward(ssize_t Mx, ssize_t My, ssize_t Mz, ssize_t Px, ssize_t Py, ssize_t Pz, const Layer &layer)
{
  // move last y layer to buffer
  for (ssize_t j = 0; j < layer.DCy; ++ j) {
    for (ssize_t k = 0; k < layer.DCz; ++ k) {
      buffer[j * layer.DCz + k] = layer(layer.DCx - 1, j, k);
    }
  }

  const ssize_t reciever = ((Mx + 1) % Px * Py + My) * Pz + Mz;
  MPI_Send(buffer.data(), layer.nx.size(), MPI_DOUBLE, reciever, 0, MPI_COMM_WORLD);
}

static inline void
recieve_x_forward(ssize_t Mx, ssize_t My, ssize_t Mz, ssize_t Px, ssize_t Py, ssize_t Pz, Layer &layer)
{
  const ssize_t sender = ((Px + Mx - 1) % Px * Py + My) * Pz + Mz;
  MPI_Recv(layer.px.data(), layer.px.size(), MPI_DOUBLE, sender, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}

static inline void
send_x_backward(ssize_t Mx, ssize_t My, ssize_t Mz, ssize_t Px, ssize_t Py, ssize_t Pz, const Layer &layer)
{
  // move first y layer to buffer
  for (ssize_t j = 0; j < layer.DCy; ++ j) {
    for (ssize_t k = 0; k < layer.DCz; ++ k) {
      buffer[j * layer.DCz + k] = layer(0, j, k);
    }
  }

  const ssize_t reciever = ((Px + Mx - 1) % Px * Py + My) * Pz + Mz;
  MPI_Send(buffer.data(), layer.px.size(), MPI_DOUBLE, reciever, 0, MPI_COMM_WORLD);
}

static inline void
recieve_x_backward(ssize_t Mx, ssize_t My, ssize_t Mz, ssize_t Px, ssize_t Py, ssize_t Pz, Layer &layer)
{
  const ssize_t sender = ((Mx + 1) % Px * Py + My) * Pz + Mz;
  MPI_Recv(layer.nx.data(), layer.nx.size(), MPI_DOUBLE, sender, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}

static inline void
contact_x_forward(ssize_t Mx, ssize_t My, ssize_t Mz, ssize_t Px, ssize_t Py, ssize_t Pz, Layer &layer)
{
  send_x_forward(Mx, My, Mz, Px, Py, Pz, layer);
  recieve_x_backward(Mx, My, Mz, Px, Py, Pz, layer);
}

static inline void
contact_x_backward(ssize_t Mx, ssize_t My, ssize_t Mz, ssize_t Px, ssize_t Py, ssize_t Pz, Layer &layer)
{
  recieve_x_forward(Mx, My, Mz, Px, Py, Pz, layer);
  send_x_backward(Mx, My, Mz, Px, Py, Pz, layer);
}

// x transport
static inline void
send_z_forward(ssize_t Mx, ssize_t My, ssize_t Mz, ssize_t Px, ssize_t Py, ssize_t Pz, const Layer &layer)
{
  // move last y layer to buffer
  for (ssize_t i = 0; i < layer.DCx; ++ i) {
    for (ssize_t j = 0; j < layer.DCy; ++ j) {
      buffer[i * layer.DCy + j] = layer(i, j, layer.DCz - 1);
    }
  }

  const ssize_t reciever = (Mx * Py + My) * Pz + (Mz + 1) % Pz;
  MPI_Send(buffer.data(), layer.nz.size(), MPI_DOUBLE, reciever, 0, MPI_COMM_WORLD);
}

static inline void
recieve_z_forward(ssize_t Mx, ssize_t My, ssize_t Mz, ssize_t Px, ssize_t Py, ssize_t Pz, Layer &layer)
{
  const ssize_t sender = (Mx * Py + My) * Pz + (Pz + Mz - 1) % Pz;
  MPI_Recv(layer.pz.data(), layer.pz.size(), MPI_DOUBLE, sender, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}

static inline void
send_z_backward(ssize_t Mx, ssize_t My, ssize_t Mz, ssize_t Px, ssize_t Py, ssize_t Pz, const Layer &layer)
{
  // move first y layer to buffer
  for (ssize_t i = 0; i < layer.DCx; ++ i) {
    for (ssize_t j = 0; j < layer.DCy; ++ j) {
      buffer[i * layer.DCy + j] = layer(i, j, 0);
    }
  }

  const ssize_t reciever = (Mx * Py + My) * Pz + (Pz + Mz - 1) % Pz;
  MPI_Send(buffer.data(), layer.pz.size(), MPI_DOUBLE, reciever, 0, MPI_COMM_WORLD);
}

static inline void
recieve_z_backward(ssize_t Mx, ssize_t My, ssize_t Mz, ssize_t Px, ssize_t Py, ssize_t Pz, Layer &layer)
{
  const ssize_t sender = (Mx * Py + My) * Pz + (Mz + 1) % Pz;
  MPI_Recv(layer.nz.data(), layer.nz.size(), MPI_DOUBLE, sender, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}

static inline void
contact_z_forward(ssize_t Mx, ssize_t My, ssize_t Mz, ssize_t Px, ssize_t Py, ssize_t Pz, Layer &layer)
{
  send_z_forward(Mx, My, Mz, Px, Py, Pz, layer);
  recieve_z_backward(Mx, My, Mz, Px, Py, Pz, layer);
}

static inline void
contact_z_backward(ssize_t Mx, ssize_t My, ssize_t Mz, ssize_t Px, ssize_t Py, ssize_t Pz, Layer &layer)
{
  recieve_z_forward(Mx, My, Mz, Px, Py, Pz, layer);
  send_z_backward(Mx, My, Mz, Px, Py, Pz, layer);
}

static inline void
sync(ssize_t Mx, ssize_t My, ssize_t Mz, ssize_t Px, ssize_t Py, ssize_t Pz, Layer &layer)
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
calc_next_layer(ssize_t Mx, ssize_t My, ssize_t Mz, ssize_t Px, ssize_t Py, ssize_t Pz, const Layer &p, const Layer &c, Layer &n)
{
  for (ssize_t i = 0; i < n.DCx; ++ i) {
    for (ssize_t j = 0; j < n.DCy; ++ j) {
      for (ssize_t k = 0; k < n.DCz; ++ k) {
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
raw_evaluate(ssize_t Sx, ssize_t Sy, ssize_t Sz, const Layer &layer, double t) {
  double l2 = 0;
  for (ssize_t i = 0; i < layer.DCx; ++ i) {
    for (ssize_t j = 0; j < layer.DCy; ++ j) {
      for (ssize_t k = 0; k < layer.DCz; ++ k) {
        l2 += sqr(layer(i, j, k) - u((i + 1 + Sx) * Hx, (j + Sy) * Hy, (k + 1 + Sz) * Hz, t));
      }
    }
  }
  return l2 / Nx / Ny / Nz;
}

static inline double
evaluate(ssize_t world_size, const Layer &layer, double t=Lt)
{
  double l2 = raw_evaluate(0, 0, 0, layer, t);
  for (ssize_t i = 1; i < world_size; ++ i) {
    double elem;
    MPI_Recv(&elem, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    l2 += elem;
  }
  return l2;
}

static inline void
help_evaluate(ssize_t Sx, ssize_t Sy, ssize_t Sz, const Layer &layer, double t=Lt)
{
  double l2 = raw_evaluate(Sx, Sy, Sz, layer, t);
  MPI_Send(&l2, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
}

static inline void
calc_last_layer(ssize_t Mx, ssize_t My, ssize_t Mz, ssize_t Px, ssize_t Py, ssize_t Pz, Layer *p, Layer *c, Layer *n)
{
  Layer *t;
  for (ssize_t i = 0; i < Nt; ++ i) {
    calc_next_layer(Mx, My, Mz, Px, Py, Pz, *p, *c, *n);
    t = p;
    p = c;
    c = n;
    n = t;
  }
}

// Utils
static inline int
npow(int e, ssize_t p)
{
  int val = 1;
  for (ssize_t i = 0; i < p; ++ i) {
    val *= e;
  }
  return val;
}

static inline ssize_t
root(int x, ssize_t base)
{
  ssize_t a = 1;
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
  const ssize_t Px = root(world_size, 3);
  const ssize_t Py = root(world_size / Px, 2);
  const ssize_t Pz = Py;

  // Process coord in world
  const ssize_t Mx = world_rank / (Py * Pz);
  const ssize_t My = (world_rank % (Py * Pz) / Pz);
  const ssize_t Mz = world_rank % Pz;

  if (world_rank > Px * Py * Pz) {
    MPI_Finalize();
    return 0;
  }

  // World cell size
  const ssize_t Dx = ceil(static_cast<double>(Nx) / Px);
  const ssize_t Dy = ceil(static_cast<double>(Ny) / Py);
  const ssize_t Dz = ceil(static_cast<double>(Nz) / Pz);

  // Current cell size
  const ssize_t DCx = Mx < Px - 1 ? Dx : Nx - Mx * Dx;
  const ssize_t DCy = My < Py - 1 ? Dy : Ny - My * Dy;
  const ssize_t DCz = Mz < Pz - 1 ? Dz : Nz - Mz * Dz;
  const ssize_t max_size = (DCx >= DCy && DCx >= DCz) ? DCx : (
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
