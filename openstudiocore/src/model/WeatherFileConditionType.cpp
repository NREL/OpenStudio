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

#include <model/WeatherFileConditionType.hpp>
#include <model/WeatherFileConditionType_Impl.hpp>
#include <model/Site.hpp>
#include <model/Site_Impl.hpp>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model  {

namespace detail {

  WeatherFileConditionType_Impl::WeatherFileConditionType_Impl(const IdfObject& idfObject,
                                                               Model_Impl* model, 
                                                               bool keepHandle)
    : SizingPeriod_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == WeatherFileConditionType::iddObjectType());
  }

  WeatherFileConditionType_Impl::WeatherFileConditionType_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other,Model_Impl* model,bool keepHandle)
    : SizingPeriod_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == WeatherFileConditionType::iddObjectType());
  }

  WeatherFileConditionType_Impl::WeatherFileConditionType_Impl(
      const WeatherFileConditionType_Impl& other,Model_Impl* model,bool keepHandle)
    : SizingPeriod_Impl(other,model,keepHandle)
  {
  }

  // Get all output variable names that could be associated with this object.
  const std::vector<std::string>& WeatherFileConditionType_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType WeatherFileConditionType_Impl::iddObjectType() const {
    return WeatherFileConditionType::iddObjectType();
  }

} // detail

/// constructor
WeatherFileConditionType::WeatherFileConditionType(const Model& model)
  : SizingPeriod(WeatherFileConditionType::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::WeatherFileConditionType_Impl>());
}

// constructor
WeatherFileConditionType::WeatherFileConditionType(
    boost::shared_ptr<detail::WeatherFileConditionType_Impl> impl)
  : SizingPeriod(impl)
{}

IddObjectType WeatherFileConditionType::iddObjectType() {
  IddObjectType result(IddObjectType::OS_SizingPeriod_WeatherFileConditionType);
  return result;
}

} // model
} // openstudio
