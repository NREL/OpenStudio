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
//    -lGeographic mgrsreverse.cpp
// [Windows]
// mex -I../include -L../windows/Release
//    -lGeographic mgrsreverse.cpp

#include <GeographicLib/MGRS.hpp>
#include <mex.h>

using namespace std;
using namespace GeographicLib;

void mexFunction( int nlhs, mxArray* plhs[],
                  int nrhs, const mxArray* prhs[] ) {

  if (nrhs != 1)
    mexErrMsgTxt("One input argument required.");
  else if (nlhs > 2)
    mexErrMsgTxt("More than two output arguments specified.");

  if (!mxIsCell(prhs[0]) || mxGetN(prhs[0]) != 1)
    mexErrMsgTxt("mgrs coordinates should be in a M x 1 cell array.");

  mwSize m = mxGetM(prhs[0]);

  plhs[0] = mxCreateDoubleMatrix(m, 4, mxREAL);
  double* x = mxGetPr(plhs[0]);
  double* y = x + m;
  double* zone = x + 2*m;
  double* hemi = x + 3*m;
  double* prec = NULL;
  bool precp = nlhs == 2;

  if (precp) {
    plhs[1] = mxCreateDoubleMatrix(m, 1, mxREAL);
    prec = mxGetPr(plhs[1]);
  }

  string mgrsstr;
  for (mwIndex i = 0; i < m; ++i) {
    try {
      mxArray* mgrs = mxGetCell(prhs[0], i);
      if (!mxIsChar(mgrs) || mxGetM(mgrs) != 1)
        throw GeographicErr("Cell element not a string");
      mwSize n = mxGetN(mgrs);
      mxChar* mgrschar = mxGetChars(mgrs);
      mgrsstr.resize(n);
      for (mwIndex k = 0; k < n; ++k)
        mgrsstr[k] = mgrschar[k];
      int ZONE, PREC;
      bool HEMI;
      MGRS::Reverse(mgrsstr, ZONE, HEMI, x[i], y[i], PREC);
      zone[i] = ZONE;
      hemi[i] = HEMI ? 1 : 0;
      if (precp)
        prec[i] = PREC;
    }
    catch (const std::exception& e) {
      mexWarnMsgTxt(e.what());
      x[i] = y[i] = Math::NaN<double>();
      zone[i] = UTMUPS::INVALID; hemi[i] = 0;
      if (precp) prec[i] = -2;
    }
  }
}
