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

using namespace System;
using namespace NETGeographicLib;

int main(array<System::String ^> ^/*args*/)
{
  try {
    // Print waypoints between JFK and SIN
    Rhumb^ rhumb = gcnew Rhumb(Constants::WGS84::MajorRadius, Constants::WGS84::Flattening, true);
    // Alternatively: const Rhumb& rhumb = Rhumb::WGS84();
    double
      lat1 = 40.640, lon1 = -73.779, // JFK
      lat2 =  1.359, lon2 = 103.989; // SIN
    double s12, azi12;
    rhumb->Inverse(lat1, lon1, lat2, lon2, s12, azi12);
    RhumbLine^ line = rhumb->Line(lat1, lon1, azi12);
    // Alternatively
    // const GeographicLib::RhumbLine line = rhumb.Line(lat1, lon1, azi1);
    double ds0 = 500e3;         // Nominal distance between points = 500 km
    int num = int(Math::Ceiling(s12 / ds0)); // The number of intervals
    {
      // Use intervals of equal length
      double ds = s12 / num;
      for (int i = 0; i <= num; ++i) {
        double lat, lon;
        line->Position(i * ds, lat, lon);
        Console::WriteLine( "{0} {1} {2}", i, lat, lon );
      }
    }
  }
  catch (GeographicErr^ e) {
      Console::WriteLine(String::Format("Caught exception: {0}", e->Message));
      return -1;
  }
  return 0;
}
