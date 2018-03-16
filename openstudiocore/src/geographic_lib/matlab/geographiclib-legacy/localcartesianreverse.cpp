/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

// Compile in Matlab with
// [Unix]
// mex -I/usr/local/include -L/usr/local/lib -Wl,-rpath=/usr/local/lib
//    -lGeographic localcartesianreverse.cpp
// [Windows]
// mex -I../include -L../windows/Release
//    -lGeographic localcartesianreverse.cpp

#include <GeographicLib/LocalCartesian.hpp>
#include <mex.h>

using namespace std;
using namespace GeographicLib;

void mexFunction( int nlhs, mxArray* plhs[],
                  int nrhs, const mxArray* prhs[] ) {

  if (nrhs < 2)
    mexErrMsgTxt("Two input arguments required.");
  else if (nrhs > 4)
    mexErrMsgTxt("More than four input arguments specified.");
  else if (nrhs == 3)
    mexErrMsgTxt("Must specify flattening with the major radius.");
  else if (nlhs > 2)
    mexErrMsgTxt("More than two output arguments specified.");

  if (!( mxIsDouble(prhs[0]) && !mxIsComplex(prhs[0]) ))
    mexErrMsgTxt("origin is not of type double.");
  if (!( mxGetM(prhs[0]) == 1 &&
         (mxGetN(prhs[0]) == 2 || mxGetN(prhs[0]) == 3) ))
    mexErrMsgTxt("origin be 1 x 3 or 1 x 2 matrix.");

  double* origin = mxGetPr(prhs[0]);
  double lat0 = origin[0], lon0 = origin[1],
    h0 = mxGetN(prhs[0]) == 3 ? origin[2] : 0;

  if (!( mxIsDouble(prhs[1]) && !mxIsComplex(prhs[1]) ))
    mexErrMsgTxt("local cartesian coordinates are not of type double.");

  if (mxGetN(prhs[1]) != 3)
    mexErrMsgTxt("local cartesian coordinates must be M x 3 matrix.");

  double a = Constants::WGS84_a<double>(), f = Constants::WGS84_f<double>();
  if (nrhs == 4) {
    if (!( mxIsDouble(prhs[2]) && !mxIsComplex(prhs[2]) &&
           mxGetNumberOfElements(prhs[2]) == 1 ))
      mexErrMsgTxt("Major radius is not a real scalar.");
    a = mxGetScalar(prhs[2]);
    if (!( mxIsDouble(prhs[3]) && !mxIsComplex(prhs[3]) &&
           mxGetNumberOfElements(prhs[3]) == 1 ))
      mexErrMsgTxt("Flattening is not a real scalar.");
    f = mxGetScalar(prhs[3]);
  }

  mwSize m = mxGetM(prhs[1]);

  double* x = mxGetPr(prhs[1]);
  double* y = x + m;
  double* z = x + 2*m;

  plhs[0] = mxCreateDoubleMatrix(m, 3, mxREAL);
  double* lat = mxGetPr(plhs[0]);
  double* lon = lat + m;
  double* h = lat + 2*m;
  double* rot = NULL;
  bool rotp = nlhs == 2;

  if (rotp) {
    plhs[1] = mxCreateDoubleMatrix(m, 9, mxREAL);
    rot = mxGetPr(plhs[1]);
  }

  try {
    std::vector<double> rotv(rotp ? 9 : 0);
    const Geocentric c(a, f);
    if (!(abs(lat0) <= 90))
      throw GeographicErr("Invalid latitude");
    if (!(lon0 >= -540 || lon0 < 540))
      throw GeographicErr("Invalid longitude");
    const LocalCartesian l(lat0, lon0, h0, c);
    for (mwIndex i = 0; i < m; ++i) {
      l.Reverse(x[i], y[i], z[i], lat[i], lon[i], h[i], rotv);
      if (rotp) {
          for (mwIndex k = 0; k < 9; ++k)
            rot[m * k + i] = rotv[k];
      }
    }
  }
  catch (const std::exception& e) {
    mexErrMsgTxt(e.what());
  }
}
