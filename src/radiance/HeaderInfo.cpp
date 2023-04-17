/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "HeaderInfo.hpp"

#include "../utilities/core/Logger.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;
using namespace openstudio;

namespace openstudio {
namespace radiance {

  /// header consists of two lines
  HeaderInfo::HeaderInfo(const std::string& line1, const std::string& line2) {

    // this will contain matches to regular expressions
    smatch matches;

    // matches a single number followed by either tabs or spaces
    string singleNumber("([-+]?[0-9]*\\.?[0-9]+)[\\s\\t]*");

    // coordinate system
    Vector origin(3);
    Vector maxX(3);
    Vector maxY(3);
    double xSpacing = 0.0;
    double ySpacing = 0.0;
    double offset = 0.0;

    // first line defines coordinate system for illuminance map
    // there will be 9 numbers separated by spaces or tabs
    // first 3 are origin point, second 3 are max x, last 3 are max y
    stringstream coordSystemSS;
    for (unsigned i = 0; i < 9; ++i) {
      coordSystemSS << singleNumber;
    }
    const regex coordSystem(coordSystemSS.str());

    if (regex_search(line1, matches, coordSystem)) {
      origin(0) = lexical_cast<double>(string(matches[1].first, matches[1].second));
      origin(1) = lexical_cast<double>(string(matches[2].first, matches[2].second));
      origin(2) = lexical_cast<double>(string(matches[3].first, matches[3].second));
      maxX(0) = lexical_cast<double>(string(matches[4].first, matches[4].second));
      maxX(1) = lexical_cast<double>(string(matches[5].first, matches[5].second));
      maxX(2) = lexical_cast<double>(string(matches[6].first, matches[6].second));
      maxY(0) = lexical_cast<double>(string(matches[7].first, matches[7].second));
      maxY(1) = lexical_cast<double>(string(matches[8].first, matches[8].second));
      maxY(2) = lexical_cast<double>(string(matches[9].first, matches[9].second));
    } else {
      LOG(Fatal, "No coordinate system defined in line1: '" << line1 << "'");
    }

    // second line defines offsets and spacing
    stringstream spacingOffsetSS;
    for (unsigned i = 0; i < 3; ++i) {
      spacingOffsetSS << singleNumber;
    }
    const regex spacingOffset(spacingOffsetSS.str());

    if (regex_search(line2, matches, spacingOffset)) {
      xSpacing = lexical_cast<double>(string(matches[1].first, matches[1].second));
      ySpacing = lexical_cast<double>(string(matches[2].first, matches[2].second));
      offset = lexical_cast<double>(string(matches[3].first, matches[3].second));
    } else {
      LOG(Fatal, "No spacing or offsets defined in line2: '" << line2 << "'");
    }

    // we can now initialize x and y vectors
    // DLM@20090803: this is not correct for arbitrary grids, revist
    m_xVector = deltaSpace(origin(0) + offset, maxX(0), xSpacing);
    m_yVector = deltaSpace(origin(1) + offset, maxY(1), ySpacing);
  }

}  // namespace radiance
}  // namespace openstudio
