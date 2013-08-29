/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <model/WeatherFileDays.hpp>
#include <model/WeatherFileDays_Impl.hpp>
#include <model/Site.hpp>
#include <model/Site_Impl.hpp>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model  {

namespace detail {

  WeatherFileDays_Impl::WeatherFileDays_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : SizingPeriod_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == WeatherFileDays::iddObjectType());
  }

  WeatherFileDays_Impl::WeatherFileDays_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle)
    : SizingPeriod_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == WeatherFileDays::iddObjectType());
  }

  WeatherFileDays_Impl::WeatherFileDays_Impl(const WeatherFileDays_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle)
    : SizingPeriod_Impl(other,model,keepHandle)
  {
  }

  // Get all output variable names that could be associated with this object.
  const std::vector<std::string>& WeatherFileDays_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType WeatherFileDays_Impl::iddObjectType() const {
    return WeatherFileDays::iddObjectType();
  }

} // detail

/// constructor
WeatherFileDays::WeatherFileDays(const Model& model)
  : SizingPeriod(WeatherFileDays::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::WeatherFileDays_Impl>());
}

// constructor
WeatherFileDays::WeatherFileDays(boost::shared_ptr<detail::WeatherFileDays_Impl> impl)
  : SizingPeriod(impl)
{}

IddObjectType WeatherFileDays::iddObjectType() {
  IddObjectType result(IddObjectType::OS_SizingPeriod_WeatherFileDays);
  return result;
}

} // model
} // openstudio
