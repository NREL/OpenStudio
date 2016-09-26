/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "JobErrors.hpp"

namespace openstudio {
namespace runmanager {

bool JobErrors::operator==(const JobErrors &t_rhs) const
{
  return (result == t_rhs.result) && (allErrors == t_rhs.allErrors);
}

int JobErrors::totalCountByType(const ErrorType &t_et) const
{
  std::vector<std::pair<int, std::string> > results = errorsByTypeWithCount(t_et);

  int sum = 0;
  for (std::vector<std::pair<int, std::string> >::const_iterator itr = results.begin();
       itr != results.end();
       ++itr)
  {
    sum += itr->first;
  }

  return sum;
}

std::vector<std::pair<int, std::string> > JobErrors::errorsByTypeWithCount(const ErrorType &t_et) const
{
  std::vector<std::pair<int, std::string> > results;

  boost::regex occurredTotalTimes(".*occurred ([0-9]+) total times.*");

  for (auto itr = allErrors.begin();
      itr != allErrors.end();
      ++itr)
  {
    if (itr->first == t_et)
    {
      int repeatCount = 0;

      boost::smatch matches;
      if (boost::regex_search(itr->second, matches, occurredTotalTimes)) {
        std::string temp = std::string(matches[1].first, matches[1].second); 
        // note that we subtract one here because the repeating EnergyPlus warnings that we are
        // parsing out are listed once when they first occur, then again with a "total times" count.
        // If we don't subtract 1 then we end up with an off by 1 error for each repeated warning
        // in the total count.
        repeatCount = atoi(temp.c_str()) - 1;
      }

      if (repeatCount < 1) repeatCount = 1;

      results.push_back(std::make_pair(repeatCount, itr->second));
    }
  }

  return results;
}


} // runmanager
} // openstudio
