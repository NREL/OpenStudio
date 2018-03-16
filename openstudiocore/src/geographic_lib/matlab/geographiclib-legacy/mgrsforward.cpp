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
//    -lGeographic mgrsforward.cpp
// [Windows]
// mex -I../include -L../windows/Release
//    -lGeographic mgrsforward.cpp

#include <GeographicLib/MGRS.hpp>
#include <mex.h>

using namespace std;
using namespace GeographicLib;

void mexFunction( int nlhs, mxArray* plhs[],
                  int nrhs, const mxArray* prhs[] ) {

  if (nrhs < 1)
    mexErrMsgTxt("One input argument required.");
  else if (nrhs > 2)
    mexErrMsgTxt("More than two input arguments specified.");
  else if (nlhs > 1)
    mexErrMsgTxt("Only one output argument can be specified.");

  if (!mxIsDouble(prhs[0]))
    mexErrMsgTxt("utmups coordinates are not of type double.");

  if (mxGetN(prhs[0]) != 4)
    mexErrMsgTxt("utmups coordinates must be M x 4 matrix.");

  int prec;
  if (nrhs == 1)
    prec = 5;
  else {
    if (!( mxIsDouble(prhs[1]) && !mxIsComplex(prhs[1]) &&
           mxGetNumberOfElements(prhs[1]) == 1 ))
      mexErrMsgTxt("Precision is not an integer.");
    double rprec = mxGetScalar(prhs[1]);
    prec = int(rprec);
    if (double(prec) != rprec)
      mexErrMsgTxt("Precision is not an integer.");
    if (prec < -1 || prec > 11)
      mexErrMsgTxt("Precision outside the legal range [-1, 11].");
  }

  mwSize m = mxGetM(prhs[0]);

  double* x = mxGetPr(prhs[0]);
  double* y = x + m;
  double* zone = x + 2*m;
  double* hemi = x + 3*m;

  string mgrsstr;
  plhs[0] = mxCreateCellArray(1, &m);
  mxArray* mgrs = plhs[0];

  for (mwIndex i = 0; i < m; ++i) {
    try {
      mgrsstr = "INVALID";
      int ZONE = int(zone[i]);
      if (double(ZONE) != zone[i])
        throw GeographicErr("Zone is not an integer");
      bool HEMI = (hemi[i] != 0);
      if (HEMI && (hemi[i] != 1))
        throw GeographicErr("Hemisphere is not 0 or 1");
      MGRS::Forward(ZONE, HEMI, x[i], y[i], prec, mgrsstr);
    }
    catch (const std::exception& e) {
      mexWarnMsgTxt(e.what());
    }
    mxSetCell(mgrs, i, mxCreateString(mgrsstr.c_str()));
  }
}
