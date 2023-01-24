/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "AnnualIlluminanceMap.hpp"
#include "HeaderInfo.hpp"

#include <iostream>
#include <fstream>
#include <vector>

#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>

using namespace std;
using namespace boost;
using namespace openstudio;

namespace openstudio {
namespace radiance {

  /// default constructor
  AnnualIlluminanceMap::AnnualIlluminanceMap() = default;

  /// constructor with path
  AnnualIlluminanceMap::AnnualIlluminanceMap(const openstudio::path& path) {
    init(path);
  }

  void AnnualIlluminanceMap::init(const openstudio::path& path) {
    // file must exist
    if (!exists(path)) {
      LOG(Fatal, "File does not exist: '" << toString(path) << "'");
      return;
    }

    // open file
    openstudio::filesystem::ifstream file(path);

    // keep track of line number
    unsigned lineNum = 0;

    // keep track of matrix size
    unsigned M = 0;
    unsigned N = 0;

    // temp string to read file
    string line;

    // lines 1 and 2 are the header lines
    string line1;
    string line2;

    // this will contain matches to regular expressions
    smatch matches;

    // matches a single number followed by either tabs or spaces
    const regex singleNumber("([-+]?[0-9]*\\.?[0-9]+)[\\s\\t]*");

    // conversion from footcandles to lux
    const double footcandlesToLux(10.76);

    // read the rest of the file line by line
    while (getline(file, line)) {
      ++lineNum;

      if (lineNum == 1) {

        // save line 1
        line1 = line;

      } else if (lineNum == 2) {

        // save line 2
        line2 = line;

        // create the header info
        HeaderInfo headerInfo(line1, line2);

        // we can now initialize x and y vectors
        m_xVector = headerInfo.xVector();
        m_yVector = headerInfo.yVector();

        M = m_xVector.size();
        N = m_yVector.size();

      } else {

        // each line contains the month, day, time (in hours),
        // Solar Azimuth(degrees from south), Solar Altitude(degrees), Global Horizontal Illuminance (fc)
        // followed by M*N illuminance points

        // break the line up by spaces
        vector<string> lineVector;
        tokenizer<char_separator<char>, std::string::const_iterator, std::string> tk(line, char_separator<char>(" "));
        for (tokenizer<char_separator<char>, std::string::const_iterator, std::string>::iterator it(tk.begin()); it != tk.end(); ++it) {
          lineVector.push_back(*it);
        }

        // total number minus 6 standard header items
        unsigned numValues = lineVector.size() - 6;

        if (numValues != M * N) {
          LOG(Fatal, "Incorrect number of illuminance values read " << numValues << ", expecting " << M * N << ".");
          return;
        } else {

          MonthOfYear thisMonth = monthOfYear(lexical_cast<unsigned>(lineVector[0]));
          auto day = lexical_cast<unsigned>(lineVector[1]);
          double fracDays = lexical_cast<double>(lineVector[2]) / 24.0;

          // ignore solar angles and global horizontal for now

          // make the date time
          DateTime dateTime(Date(thisMonth, day), Time(fracDays));

          // matrix we are going to read in
          Matrix illuminanceMap(M, N);

          // read in the values
          unsigned index = 6;
          for (unsigned j = 0; j < N; ++j) {
            for (unsigned i = 0; i < M; ++i) {
              illuminanceMap(i, j) = footcandlesToLux * lexical_cast<double>(lineVector[index]);
              ++index;
            }
          }

          m_dateTimes.push_back(dateTime);
          m_dateTimeIlluminanceMap[dateTime] = illuminanceMap;
        }
      }
    }

    // close file
    file.close();
  }

  /// get the illuminance map in lux corresponding to date and time
  openstudio::Matrix AnnualIlluminanceMap::illuminanceMap(const openstudio::DateTime& dateTime) const {
    auto it = m_dateTimeIlluminanceMap.find(dateTime);
    if (it != m_dateTimeIlluminanceMap.end()) {
      return it->second;
    }

    return m_nullIlluminanceMap;
  }

}  // namespace radiance
}  // namespace openstudio
