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

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

#if defined(_OPENMP)
#define HAVE_OPENMP 1
#else
#define HAVE_OPENMP 0
#endif

#if HAVE_OPENMP
#  include <omp.h>
#endif

#include <GeographicLib/GravityModel.hpp>
#include <GeographicLib/GravityCircle.hpp>
#include <GeographicLib/Utility.hpp>

using namespace std;
using namespace GeographicLib;

int main(int argc, char* argv[]) {
  // Hardwired for 3 args:
  // 1 = the gravity model (e.g., egm2008)
  // 2 = intervals per degree
  // 3 = output GTX file
  if (argc != 4) {
    cerr << "Usage: " << argv[0]
         << " gravity-model intervals-per-degree output.gtx\n";
    return 1;
  }
  try {
    // Will need to set the precision for each thread, so save return value
    int ndigits = Utility::set_digits();
    string model(argv[1]);
    // Number of intervals per degree
    int ndeg = Utility::num<int>(string(argv[2]));
    string filename(argv[3]);
    GravityModel g(model);
    int
      nlat = 180 * ndeg + 1,
      nlon = 360 * ndeg;
    Math::real
      delta = 1 / Math::real(ndeg), // Grid spacing
      latorg = -90,
      lonorg = -180;
    // Write results as floats in binary mode
    ofstream file(filename.c_str(), ios::binary);

    // Write header
    {
      Math::real transform[] = {latorg, lonorg, delta, delta};
      unsigned sizes[] = {unsigned(nlat), unsigned(nlon)};
      Utility::writearray<double, Math::real, true>(file, transform, 4);
      Utility::writearray<unsigned, unsigned, true>(file, sizes, 2);
    }

    // Compute and store results for nbatch latitudes at a time
    const int nbatch = 64;
    vector< vector<float> > N(nbatch, vector<float>(nlon));

    for (int ilat0 = 0; ilat0 < nlat; ilat0 += nbatch) { // Loop over batches
      int nlat0 = min(nlat, ilat0 + nbatch);

#if HAVE_OPENMP
#  pragma omp parallel for
#endif
      for (int ilat = ilat0; ilat < nlat0; ++ilat) { // Loop over latitudes
        Utility::set_digits(ndigits);                // Set the precision
        Math::real
          lat = latorg + (ilat / ndeg) + delta * (ilat - ndeg * (ilat / ndeg)),
          h = 0;
        GravityCircle c(g.Circle(lat, h, GravityModel::GEOID_HEIGHT));
        for (int ilon = 0; ilon < nlon; ++ilon) { // Loop over longitudes
          Math::real lon = lonorg
            + (ilon / ndeg) + delta * (ilon - ndeg * (ilon / ndeg));
          N[ilat - ilat0][ilon] = float(c.GeoidHeight(lon));
        } // longitude loop
      }   // latitude loop -- end of parallel section

      for (int ilat = ilat0; ilat < nlat0; ++ilat) // write out data
        Utility::writearray<float, float, true>(file, N[ilat - ilat0]);
    } // batch loop
  }
  catch (const exception& e) {
    cerr << "Caught exception: " << e.what() << "\n";
    return 1;
  }
  catch (...) {
    cerr << "Caught unknown exception\n";
    return 1;
  }
  return 0;
}
