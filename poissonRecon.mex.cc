#include "mex.h"
#include "mexutil.h"
#include "src/PoissonReconLib.h"
#include <stdlib.h>

#ifdef _OPENMP
#include <omp.h>
#endif

using namespace mexutil;

void mexFunction(int nargout, mxArray *out[], int nargin, const mxArray *in[]) {
  int_data.clear();
  double_data.clear();

  N_IN(3);
  N_OUT(2);

  mwSize point_ncols = mxGetN(in[0]);
  mwSize point_nrows = mxGetM(in[0]);
  mwSize normal_ncols = mxGetN(in[1]);
  mwSize normal_nrows = mxGetM(in[1]);

  double *points = (double *)GetArg<kDouble, EQ, EQ>(0, in, 0, 3);
  double *normals = (double *)GetArg<kDouble, EQ, EQ>(1, in, 0, 3);
  double *depth = (double *)GetArg<kDouble, EQ, EQ>(2, in, 1, 1);

  if (point_nrows != normal_nrows) {
    ERR_EXIT("InputSizeError",
             "size(input[0], 1) can be equal size(input[1], 1)");
  }
  if (*depth < 1 || *depth > 20) {
    ERR_EXIT("InputSizeError", "Depth must be in [1, 20]");
  }

  mem_data.resize(point_ncols * point_nrows + normal_ncols * normal_nrows);

#pragma omp parallel for
  for (int j = 0; j < point_ncols; ++j) {
    for (int i = 0; i < point_nrows; ++i) {
      mem_data[j + i * (point_ncols + normal_ncols)] =
          points[j * point_nrows + i];
      mem_data[j + point_ncols + i * (point_ncols + normal_ncols)] =
          normals[j * point_nrows + i];
    }
  }

  const char *argv[] =

      {"PoissonRecon",

       "--in",         "none",

       "--out",        "none",

       "--depth",      std::to_string((int)*depth).c_str(),

       "--confidence",

       "--nWeights",

       NULL};

  int argc = sizeof(argv) / sizeof(char *) - 1;
  PoissonReconLibMain(argc, const_cast<char **>(argv));

  size_t face_cols = 3;
  size_t face_rows = int_data.size() / face_cols;

  size_t vertex_cols = 3;
  size_t vertex_rows = double_data.size() / vertex_cols;

  out[0] = mxCreateNumericMatrix(face_rows, face_cols, mxINT32_CLASS, mxREAL);
  out[1] = mxCreateDoubleMatrix(vertex_rows, vertex_cols, mxREAL);
  int *faces = (int *)mxGetData(out[0]);
  double *vertices = mxGetPr(out[1]);

  TransposeAddOne<3>(int_data, faces);
  Transpose<3>(double_data, vertices);

  int_data.clear();
  double_data.clear();
}
