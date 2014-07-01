/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
*  All rights reserved.
*  
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*  
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*  
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

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

  for (std::vector<std::pair<ErrorType, std::string> >::const_iterator itr = allErrors.begin();
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
