#include "mex.h"
#include "mexutil.h"

using namespace mexutil;

void mexFunction(int nargout, mxArray *out[], int nargin, const mxArray *in[]) {
  N_IN(2);
  N_OUT(3);

  mwSize ncols;
  mwSize nrows;

  ncols = mxGetN(in[0]);
  nrows = mxGetM(in[0]);
}
