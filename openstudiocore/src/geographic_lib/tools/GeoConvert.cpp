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
#include <sstream>
#include <string>
#include <sstream>
#include <fstream>
#include <GeographicLib/GeoCoords.hpp>
#include <GeographicLib/DMS.hpp>
#include <GeographicLib/Utility.hpp>
#include <GeographicLib/MGRS.hpp>

#if defined(_MSC_VER)
// Squelch warnings about constant conditional expressions
#  pragma warning (disable: 4127)
#endif

#include "GeoConvert.usage"

int main(int argc, char* argv[]) {
  try {
    using namespace GeographicLib;
    typedef Math::real real;
    Utility::set_digits();
    enum { GEOGRAPHIC, DMS, UTMUPS, MGRS, CONVERGENCE };
    int outputmode = GEOGRAPHIC;
    int prec = 0;
    int zone = UTMUPS::MATCH;
    bool centerp = true, longfirst = false;
    std::string istring, ifile, ofile, cdelim;
    char lsep = ';', dmssep = char(0);
    bool sethemisphere = false, northp = false, abbrev = true;

    for (int m = 1; m < argc; ++m) {
      std::string arg(argv[m]);
      if (arg == "-g")
        outputmode = GEOGRAPHIC;
      else if (arg == "-d") {
        outputmode = DMS;
        dmssep = '\0';
      } else if (arg == "-:") {
        outputmode = DMS;
        dmssep = ':';
      } else if (arg == "-u")
        outputmode = UTMUPS;
      else if (arg == "-m")
        outputmode = MGRS;
      else if (arg == "-c")
        outputmode = CONVERGENCE;
      else if (arg == "-n")
        centerp = false;
      else if (arg == "-z") {
        if (++m == argc) return usage(1, true);
        std::string zonestr(argv[m]);
        try {
          UTMUPS::DecodeZone(zonestr, zone, northp);
          sethemisphere = true;
        }
        catch (const std::exception&) {
          std::istringstream str(zonestr);
          char c;
          if (!(str >> zone) || (str >> c)) {
            std::cerr << "Zone " << zonestr
                      << " is not a number or zone+hemisphere\n";
            return 1;
          }
          if (!(zone >= UTMUPS::MINZONE && zone <= UTMUPS::MAXZONE)) {
            std::cerr << "Zone " << zone << " not in [0, 60]\n";
            return 1;
          }
          sethemisphere = false;
        }
      } else if (arg == "-s") {
        zone = UTMUPS::STANDARD;
        sethemisphere = false;
      } else if (arg == "-t") {
        zone = UTMUPS::UTM;
        sethemisphere = false;
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
      } else if (arg == "-l")
        abbrev = false;
      else if (arg == "-a")
        abbrev = true;
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
        MGRS::Check();
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

    GeoCoords p;
    std::string s, eol;
    std::string os;
    int retval = 0;

    while (std::getline(*input, s)) {
      eol = "\n";
      try {
        if (!cdelim.empty()) {
          std::string::size_type m = s.find(cdelim);
          if (m != std::string::npos) {
            eol = " " + s.substr(m) + "\n";
            s = s.substr(0, m);
          }
        }
        p.Reset(s, centerp, longfirst);
        p.SetAltZone(zone);
        switch (outputmode) {
        case GEOGRAPHIC:
          os = p.GeoRepresentation(prec, longfirst);
          break;
        case DMS:
          os = p.DMSRepresentation(prec, longfirst, dmssep);
          break;
        case UTMUPS:
          os = (sethemisphere
                ? p.AltUTMUPSRepresentation(northp, prec, abbrev)
                : p.AltUTMUPSRepresentation(prec, abbrev));
          break;
        case MGRS:
          os = p.AltMGRSRepresentation(prec);
          break;
        case CONVERGENCE:
          {
            real
              gamma = p.AltConvergence(),
              k = p.AltScale();
            int prec1 = std::max(-5, std::min(Math::extra_digits() + 8, prec));
            os = Utility::str(gamma, prec1 + 5) + " "
              + Utility::str(k, prec1 + 7);
          }
        }
      }
      catch (const std::exception& e) {
        // Write error message to cout so output lines match input lines
        os = std::string("ERROR: ") + e.what();
        retval = 1;
      }
      *output << os << eol;
    }
    return retval;
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
