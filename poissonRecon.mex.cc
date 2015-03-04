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
  mem_data.clear();

  N_IN_RANGE(2, 8);
  N_OUT(2);

  mwSize point_ncols = mxGetN(in[0]);
  mwSize point_nrows = mxGetM(in[0]);
  mwSize normal_ncols = mxGetN(in[1]);
  mwSize normal_nrows = mxGetM(in[1]);

  double *points = (double *)GetArg<kDouble, EQ, EQ>(0, in, 0, 3);
  double *normals = (double *)GetArg<kDouble, EQ, EQ>(1, in, 0, 3);

  // Default values
  double depth =
      (nargin < 3) ? 8.0 : *((double *)GetArg<kDouble, EQ, EQ>(2, in, 1, 1));
  double full_depth =
      (nargin < 4) ? 5.0 : *((double *)GetArg<kDouble, EQ, EQ>(3, in, 1, 1));
  double scale =
      (nargin < 5) ? 1.10 : *((double *)GetArg<kDouble, EQ, EQ>(4, in, 1, 1));
  double samples_per_node =
      (nargin < 6) ? 1.0 : *((double *)GetArg<kDouble, EQ, EQ>(5, in, 1, 1));
  double cg_depth =
      (nargin < 7) ? 0.0 : *((double *)GetArg<kDouble, EQ, EQ>(6, in, 1, 1));
  bool print_info =
      (nargin < 8) ? true
                   : (0 < *((double *)GetArg<kDouble, EQ, EQ>(7, in, 1, 1)));
  is_verbose = &print_info;

  if (point_nrows != normal_nrows) {
    ERR_EXIT("InputSizeError",
             "size(input[0], 1) can be equal size(input[1], 1)");
  }
  if (depth < 1 || depth > 20) {
    ERR_EXIT("InputSizeError", "Depth must be in [1, 20]");
  }
  if (full_depth < 1 || full_depth > 20) {
    ERR_EXIT("InputSizeError", "fullDepth must be in [1, 20]");
  }
  if (samples_per_node < 1 || samples_per_node > 500) {
    ERR_EXIT("InputSizeError", "samplesPerNode must be in [1, 500]");
  }
  if (full_depth < 0 || full_depth > 20) {
    ERR_EXIT("InputSizeError", "cgDepth must be in [0, 20]");
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

  std::string arg_depth = std::to_string((int)(depth + 0.5));
  std::string arg_full_depth = std::to_string((int)(full_depth + 0.5));
  std::string arg_scale = std::to_string((double)scale);
  std::string arg_samples_per_node =
      std::to_string((int)(samples_per_node + 0.5));
  std::string arg_cg_depth = std::to_string((int)(cg_depth + 0.5));

  const char *argv[] =

      {"PoissonRecon",

       "--in",             "none",

       "--out",            "none",

       "--depth",          arg_depth.c_str(),

       "--fullDepth",      arg_full_depth.c_str(),

       "--scale",          arg_scale.c_str(),

       "--samplesPerNode", arg_samples_per_node.c_str(),

       "--cgDepth",        arg_cg_depth.c_str(),

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
