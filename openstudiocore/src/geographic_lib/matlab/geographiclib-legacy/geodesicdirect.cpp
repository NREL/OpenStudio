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
//    -lGeographic geodesicdirect.cpp
// [Windows]
// mex -I../include -L../windows/Release
//    -lGeographic geodesicdirect.cpp

#include <algorithm>
#include <GeographicLib/Geodesic.hpp>
#include <GeographicLib/GeodesicExact.hpp>
#include <mex.h>

using namespace std;
using namespace GeographicLib;

template<class G> void
compute(double a, double f, mwSize m, const double* geodesic,
        double* latlong, double* aux) {
  const double* lat1 = geodesic;
  const double* lon1 = geodesic + m;
  const double* azi1 = geodesic + 2*m;
  const double* s12 = geodesic + 3*m;
  double* lat2 = latlong;
  double* lon2 = latlong + m;
  double* azi2 = latlong + 2*m;
  double* a12 = NULL;
  double* m12 = NULL;
  double* M12 = NULL;
  double* M21 = NULL;
  double* S12 = NULL;
  if (aux) {
    a12 = aux;
    m12 = aux + m;
    M12 = aux + 2*m;
    M21 = aux + 3*m;
    S12 = aux + 4*m;
  }

  const G g(a, f);
  for (mwIndex i = 0; i < m; ++i) {
    if (abs(lat1[i]) <= 90 &&
        lon1[i] >= -540 && lon1[i] < 540 &&
        azi1[i] >= -540 && azi1[i] < 540) {
      if (aux)
        a12[i] = g.Direct(lat1[i], lon1[i], azi1[i], s12[i],
                          lat2[i], lon2[i], azi2[i],
                          m12[i], M12[i], M21[i], S12[i]);
      else
        g.Direct(lat1[i], lon1[i], azi1[i], s12[i],
                 lat2[i], lon2[i], azi2[i]);
    }
  }
}

void mexFunction( int nlhs, mxArray* plhs[],
                  int nrhs, const mxArray* prhs[] ) {

  if (nrhs < 1)
    mexErrMsgTxt("One input argument required.");
  else if (nrhs > 3)
    mexErrMsgTxt("More than three input arguments specified.");
  else if (nrhs == 2)
    mexErrMsgTxt("Must specify flattening with the major radius.");
  else if (nlhs > 2)
    mexErrMsgTxt("More than two output arguments specified.");

  if (!( mxIsDouble(prhs[0]) && !mxIsComplex(prhs[0]) ))
    mexErrMsgTxt("geodesic coordinates are not of type double.");

  if (mxGetN(prhs[0]) != 4)
    mexErrMsgTxt("geodesic coordinates must be M x 4 matrix.");

  double a = Constants::WGS84_a<double>(), f = Constants::WGS84_f<double>();
  if (nrhs == 3) {
    if (!( mxIsDouble(prhs[1]) && !mxIsComplex(prhs[1]) &&
           mxGetNumberOfElements(prhs[1]) == 1 ))
      mexErrMsgTxt("Major radius is not a real scalar.");
    a = mxGetScalar(prhs[1]);
    if (!( mxIsDouble(prhs[2]) && !mxIsComplex(prhs[2]) &&
           mxGetNumberOfElements(prhs[2]) == 1 ))
      mexErrMsgTxt("Flattening is not a real scalar.");
    f = mxGetScalar(prhs[2]);
  }

  mwSize m = mxGetM(prhs[0]);

  const double* geodesic = mxGetPr(prhs[0]);

  double* latlong = mxGetPr(plhs[0] = mxCreateDoubleMatrix(m, 3, mxREAL));
  std::fill(latlong, latlong + 3*m, Math::NaN<double>());

  double* aux =
    nlhs == 2 ? mxGetPr(plhs[1] = mxCreateDoubleMatrix(m, 5, mxREAL)) :
    NULL;
  if (aux)
    std::fill(aux, aux + 5*m, Math::NaN<double>());

  try {
    if (std::abs(f) <= 0.02)
      compute<Geodesic>(a, f, m, geodesic, latlong, aux);
    else
      compute<GeodesicExact>(a, f, m, geodesic, latlong, aux);
  }
  catch (const std::exception& e) {
    mexErrMsgTxt(e.what());
  }
}
