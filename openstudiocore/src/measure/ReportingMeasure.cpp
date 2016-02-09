/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#include "ReportingMeasure.hpp"

#include "OSArgument.hpp"
#include "OSOutput.hpp"
#include "OSRunner.hpp"

#include <vector>

namespace openstudio {
namespace measure {

ReportingMeasure::~ReportingMeasure() {}

std::vector<OSArgument> ReportingMeasure::arguments() const {
  return OSArgumentVector();
}

std::vector<OSOutput> ReportingMeasure::outputs() const {
  return OSOutputVector();
}

bool ReportingMeasure::run(OSRunner& runner,
                           const std::map<std::string, OSArgument>& user_arguments) const
{
  runner.prepareForMeasureRun(*this);
  return true;
}

std::vector<IdfObject> ReportingMeasure::energyPlusOutputRequests(OSRunner& runner,
                              const std::map<std::string, OSArgument>& user_arguments) const
{
  return std::vector<IdfObject>();
}

} // measure
} // openstudio
