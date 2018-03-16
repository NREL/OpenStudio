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

#include <iostream>
#include <exception>
#include <vector>
#include <GeographicLib/SphericalHarmonic2.hpp>

using namespace std;
using namespace GeographicLib;

int main() {
  try {
    int N = 3, N1 = 2, N2 = 1;                     // The maxium degrees
    double ca[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1}; // cosine coefficients
    vector<double> C(ca, ca + (N + 1) * (N + 2) / 2);
    double sa[] = {6, 5, 4, 3, 2, 1}; // sine coefficients
    vector<double> S(sa, sa + N * (N + 1) / 2);
    double cb[] = {1, 2, 3, 4, 5, 6};
    vector<double> C1(cb, cb + (N1 + 1) * (N1 + 2) / 2);
    double sb[] = {3, 2, 1};
    vector<double> S1(sb, sb + N1 * (N1 + 1) / 2);
    double cc[] = {2, 1};
    vector<double> C2(cc, cc + (N2 + 1));
    vector<double> S2;
    double a = 1;
    SphericalHarmonic2 h(C, S, N, N, N, C1, S1, N1, N1, N1,
                         C2, S2, N2, N2, 0, a);
    double tau1 = 0.1, tau2 = 0.05, x = 2, y = 3, z = 1;
    double v, vx, vy, vz;
    v = h(tau1, tau2, x, y, z, vx, vy, vz);
    cout << v << " " << vx << " " << vy << " " << vz << "\n";
  }
  catch (const exception& e) {
    cerr << "Caught exception: " << e.what() << "\n";
    return 1;
  }
  return 0;
}
