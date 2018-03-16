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
#include <string>
#include <sstream>
#include <fstream>
#include <GeographicLib/PolygonArea.hpp>
#include <GeographicLib/DMS.hpp>
#include <GeographicLib/Utility.hpp>
#include <GeographicLib/GeoCoords.hpp>
#include <GeographicLib/Ellipsoid.hpp>

#if defined(_MSC_VER)
// Squelch warnings about constant conditional expressions
#  pragma warning (disable: 4127)
#endif

#include "Planimeter.usage"

int main(int argc, char* argv[]) {
  try {
    using namespace GeographicLib;
    typedef Math::real real;
    Utility::set_digits();
    enum { GEODESIC, EXACT, AUTHALIC, RHUMB };
    real
      a = Constants::WGS84_a(),
      f = Constants::WGS84_f();
    bool reverse = false, sign = true, polyline = false, longfirst = false;
    int linetype = GEODESIC;
    int prec = 6;
    std::string istring, ifile, ofile, cdelim;
    char lsep = ';';

    for (int m = 1; m < argc; ++m) {
      std::string arg(argv[m]);
      if (arg == "-r")
        reverse = !reverse;
      else if (arg == "-s")
        sign = !sign;
      else if (arg == "-l")
        polyline = !polyline;
      else if (arg == "-e") {
        if (m + 2 >= argc) return usage(1, true);
        try {
          a = Utility::num<real>(std::string(argv[m + 1]));
          f = Utility::fract<real>(std::string(argv[m + 2]));
        }
        catch (const std::exception& e) {
          std::cerr << "Error decoding arguments of -e: " << e.what() << "\n";
          return 1;
        }
        m += 2;
      } else if (arg == "-w")
        longfirst = true;
      else if (arg == "-p") {
        if (++m == argc) return usage(1, true);
        try {
          prec = Utility::num<int>(std::string(argv[m]));
        }
        catch (const std::exception&) {
          std::cerr << "Precision " << argv[m] << " is not a number\n";
          return 1;
        }
      } else if (arg == "-G")
        linetype = GEODESIC;
      else if (arg == "-E")
        linetype = EXACT;
      else if (arg == "-Q")
        linetype = AUTHALIC;
      else if (arg == "-R")
        linetype = RHUMB;
      else if (arg == "--input-string") {
        if (++m == argc) return usage(1, true);
        istring = argv[m];
      } else if (arg == "--input-file") {
        if (++m == argc) return usage(1, true);
        ifile = argv[m];
      } else if (arg == "--output-file") {
        if (++m == argc) return usage(1, true);
        ofile = argv[m];
      } else if (arg == "--line-separator") {
        if (++m == argc) return usage(1, true);
        if (std::string(argv[m]).size() != 1) {
          std::cerr << "Line separator must be a single character\n";
          return 1;
        }
        lsep = argv[m][0];
      } else if (arg == "--comment-delimiter") {
        if (++m == argc) return usage(1, true);
        cdelim = argv[m];
      } else if (arg == "--version") {
        std::cout << argv[0] << ": GeographicLib version "
                  << GEOGRAPHICLIB_VERSION_STRING << "\n";
        return 0;
      } else
        return usage(!(arg == "-h" || arg == "--help"), arg != "--help");
    }

    if (!ifile.empty() && !istring.empty()) {
      std::cerr << "Cannot specify --input-string and --input-file together\n";
      return 1;
    }
    if (ifile == "-") ifile.clear();
    std::ifstream infile;
    std::istringstream instring;
    if (!ifile.empty()) {
      infile.open(ifile.c_str());
      if (!infile.is_open()) {
        std::cerr << "Cannot open " << ifile << " for reading\n";
        return 1;
      }
    } else if (!istring.empty()) {
      std::string::size_type m = 0;
      while (true) {
        m = istring.find(lsep, m);
        if (m == std::string::npos)
          break;
        istring[m] = '\n';
      }
      instring.str(istring);
    }
    std::istream* input = !ifile.empty() ? &infile :
      (!istring.empty() ? &instring : &std::cin);

    std::ofstream outfile;
    if (ofile == "-") ofile.clear();
    if (!ofile.empty()) {
      outfile.open(ofile.c_str());
      if (!outfile.is_open()) {
        std::cerr << "Cannot open " << ofile << " for writing\n";
        return 1;
      }
    }
    std::ostream* output = !ofile.empty() ? &outfile : &std::cout;

    const Ellipsoid ellip(a, f);
    if (linetype == AUTHALIC) {
      using std::sqrt;
      a = sqrt(ellip.Area() / (4 * Math::pi()));
      f = 0;
    }
    const Geodesic geod(a, f);
    const GeodesicExact geode(a, f);
    const Rhumb rhumb(a, f);
    PolygonArea poly(geod, polyline);
    PolygonAreaExact polye(geode, polyline);
    PolygonAreaRhumb polyr(rhumb, polyline);
    GeoCoords p;

    // Max precision = 10: 0.1 nm in distance, 10^-15 deg (= 0.11 nm),
    // 10^-11 sec (= 0.3 nm).
    prec = std::min(10 + Math::extra_digits(), std::max(0, prec));
    std::string s, eol("\n");
    real perimeter, area;
    unsigned num;
    while (std::getline(*input, s)) {
      if (!cdelim.empty()) {
        std::string::size_type m = s.find(cdelim);
        if (m != std::string::npos) {
          eol = " " + s.substr(m) + "\n";
          s = s.substr(0, m);
        }
      }
      bool endpoly = s.empty();
      if (!endpoly) {
        try {
          p.Reset(s, longfirst);
          if (Math::isnan(p.Latitude()) || Math::isnan(p.Longitude()))
            endpoly = true;
        }
        catch (const GeographicErr&) {
          endpoly = true;
        }
      }
      if (endpoly) {
        num =
          linetype == EXACT ? polye.Compute(reverse, sign, perimeter, area) :
          linetype == RHUMB ? polyr.Compute(reverse, sign, perimeter, area) :
          poly.Compute(reverse, sign, perimeter, area); // geodesic + authalic
        if (num > 0) {
          *output << num << " " << Utility::str(perimeter, prec);
          if (!polyline) {
            *output << " " << Utility::str(area, std::max(0, prec - 5));
          }
          *output << eol;
        }
        linetype == EXACT ? polye.Clear() :
          linetype == RHUMB ? polyr.Clear() : poly.Clear();
        eol = "\n";
      } else {
        linetype == EXACT ? polye.AddPoint(p.Latitude(), p.Longitude()) :
          linetype == RHUMB ? polyr.AddPoint(p.Latitude(), p.Longitude()) :
          poly.AddPoint(linetype == AUTHALIC ?
                        ellip.AuthalicLatitude(p.Latitude()) :
                        p.Latitude(),
                        p.Longitude());
      }
    }
    num =
      linetype == EXACT ? polye.Compute(reverse, sign, perimeter, area) :
      linetype == RHUMB ? polyr.Compute(reverse, sign, perimeter, area) :
      poly.Compute(reverse, sign, perimeter, area);
    if (num > 0) {
      *output << num << " " << Utility::str(perimeter, prec);
      if (!polyline) {
        *output << " " << Utility::str(area, std::max(0, prec - 5));
      }
      *output << eol;
    }
    linetype == EXACT ? polye.Clear() :
      linetype == RHUMB ? polyr.Clear() : poly.Clear();
    eol = "\n";
    return 0;
  }
  catch (const std::exception& e) {
    std::cerr << "Caught exception: " << e.what() << "\n";
    return 1;
  }
  catch (...) {
    std::cerr << "Caught unknown exception\n";
    return 1;
  }
}
