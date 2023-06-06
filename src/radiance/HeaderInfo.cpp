/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
