#include <algorithm>
#include <iostream>
#include <memory>
#include <random>
#include <vector>
#include <cstdlib>
#include <cassert>

#include <mpi.h>

#include "alglib/linalg.h"

extern "C" void blacs_pinfo_(const int &, const int &);
extern "C" void blacs_get_(const int &, const int &, int &);
extern "C" void blacs_gridinit_(const int &, const char &, const int &, const int &);
extern "C" void blacs_gridinfo_(const int &, const int &, const int &, int &, int &);
extern "C" void blacs_gridexit_(const int &);
extern "C" void blacs_exit_(const int &);
extern "C" int numroc_(const int &, const int &, const int &, const int &, const int &);
extern "C" int descinit_(const int *, const int &, const int &, const int &, const int &, const int &, const int &, const int &, const int &, int &);
extern "C" int psgesvd_(const char &, const char &, const int &, const int &, float *, const int &, const int &,
                        const int *, float *, float *, const int &, const int &, const int *,
                        float *, const int &, const int &, const int *, float *, const int &, int &);

using VALUE_TYPE = float;
const VALUE_TYPE EPS = 1e-4;
const ssize_t DLEN = 9;

void
finish(int icontxt)
{
  blacs_gridexit_(icontxt);
  blacs_exit_(0);
}

int
get_bsize(ssize_t MSIZE, int pcount)
{
  return ceil(1.0 * MSIZE / pcount);
}

int
get_size(ssize_t MSIZE, int pindex, int pcount)
{
  const int bsize = get_bsize(MSIZE, pcount);
  return numroc_(MSIZE, bsize, pindex, 0, pcount);
}

std::unique_ptr<int[]>
init_desc(int icontxt, ssize_t mrows, ssize_t mcols, int prows, int pcols, int prow, int pcol)
{
  std::unique_ptr<int[]> desc(new int[DLEN]);
  int rbsize = get_bsize(mrows, prows);
  int cbsize = get_bsize(mcols, pcols);
  int mxrows = get_size(mrows, prow, prows);
  int info;
  descinit_(desc.get(), mrows, mcols, rbsize, cbsize, 0, 0, icontxt, mxrows, info);
  if (info != 0) {
    finish(icontxt);
    std::cerr << "Unexpected descinit info return " << info << std::endl;
    MPI_Abort(MPI_COMM_WORLD, -1);
  }
  return desc;
}

std::unique_ptr<VALUE_TYPE[]>
alloc_matrix(ssize_t mrows, ssize_t mcols, int prows, int pcols, int prow, int pcol)
{
  int mxrows = get_size(mrows, prow, prows);
  int mxcols = get_size(mcols, pcol, pcols);
  ssize_t length = mxrows * mxcols;
  std::unique_ptr<VALUE_TYPE[]> matrix(new VALUE_TYPE[length]);
  return matrix;
}

int
main(int argc, char **argv)
{
  // mpi init
  MPI_Init(&argc, &argv);
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // blacs init
  blacs_pinfo_(world_rank, world_size);
  const int prows = floor(sqrt(world_size));
  const int pcols = floor(sqrt(world_size));

  int icontxt;
  blacs_get_(0, 0, icontxt);

  blacs_gridinit_(icontxt, 'R', prows, pcols);

  int prow, pcol;
  blacs_gridinfo_(icontxt, prows, pcols, prow, pcol);

  if (prow == -1 || pcol == -1) {
    finish(icontxt);
    return 0;
  }

  if (argc != 2) {
    finish(icontxt);
    std::cerr << "Set argv[1]" << std::endl;
    return -1;
  }

  // get sizes
  const int mrows = std::stoi(argv[1]);
  const int mcols = mrows;

  // Generate matrix
  std::unique_ptr<double[]> double_content(new double[mrows * mcols]);
  const ssize_t MDIAG = std::min(mrows, mcols);
  std::vector<double> diag(MDIAG);

  // seed
  srand(42);
  std::default_random_engine generator(42);
  std::uniform_real_distribution<double> distribution(0, 1);

  for (ssize_t i = 0; i < MDIAG; ++i) {
    double_content[i * MDIAG + i] = diag[i] = distribution(generator);
  }

  std::sort(diag.begin(), diag.end(), std::greater<double>());

  alglib::real_2d_array alglib_matrix;
  alglib_matrix.attach_to_ptr(mrows, mcols, double_content.get());

  alglib::rmatrixrndorthogonalfromtheleft(alglib_matrix, mrows, mrows);
  alglib::rmatrixrndorthogonalfromtheright(alglib_matrix, mcols, mcols);

  // Create blacs matrix
  auto desc_a = init_desc(icontxt, mrows, mcols, prows, pcols, prow, pcol);
  auto local_a = alloc_matrix(mrows, mcols, prows, pcols, prow, pcol);
  int rbsize = get_bsize(mrows, prows);
  int cbsize = get_bsize(mcols, pcols);
  int mxrows_a = get_size(mrows, prow, prows);
  int mxcols_a = get_size(mcols, pcol, pcols);

  auto desc_u = init_desc(icontxt, mrows, mrows, prows, pcols, prow, pcol);
  auto desc_vt = init_desc(icontxt, mcols, mcols, prows, pcols, prow, pcol);

  for (ssize_t i = 0; i < mxrows_a; ++i) {
    for (ssize_t j = 0; j < mxcols_a; ++j) {
      local_a[i + j * mxrows_a] = alglib_matrix(prow * rbsize + i, pcol * cbsize + j);
    }
  }

  // Check output
  std::unique_ptr<VALUE_TYPE[]> computed_diag(new VALUE_TYPE[MDIAG]);
  int info = 0;
  std::unique_ptr<VALUE_TYPE[]> work(new VALUE_TYPE[1]);

  psgesvd_('N', 'N', mrows, mcols, local_a.get(), 1, 1, desc_a.get(), computed_diag.get(), nullptr, 1, 1, desc_u.get(), nullptr, 1, 1, desc_vt.get(), work.get(), -1, info);

  if (info != 0) {
    finish(icontxt);
    std::cerr << "Unexpected p?gesvd info return " << info << std::endl;
    return -1;
  }

  int lwork = work[0];
  work.reset(new VALUE_TYPE[lwork]);

  // Init complete
  MPI_Barrier(MPI_COMM_WORLD);

  // Calc svd
  volatile auto start_time = MPI_Wtime();
  psgesvd_('N', 'N', mrows, mcols, local_a.get(), 1, 1, desc_a.get(), computed_diag.get(), nullptr, 1, 1, desc_u.get(), nullptr, 1, 1, desc_vt.get(), work.get(), lwork, info);
  volatile auto elapsed_time = MPI_Wtime() - start_time;

  if (world_rank == 0) {
    std::cout << mrows << "," << mcols << "," << elapsed_time << std::endl;

    for (ssize_t i = 0; i < MDIAG; ++ i) {
      if (std::abs(diag[i] - computed_diag[i]) > EPS) {
        finish(icontxt);
        std::cerr << "Unexpected sigma[" << i << "]: " << computed_diag[i] << ", expected " << diag[i] << std::endl;
        return -1;
      }
    }
  }

  finish(icontxt);
  return 0;
}
