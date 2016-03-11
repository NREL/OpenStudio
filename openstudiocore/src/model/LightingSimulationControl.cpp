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

#include "LightingSimulationControl.hpp"
#include "LightingSimulationControl_Impl.hpp"

#include <utilities/idd/OS_LightingSimulationControl_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  LightingSimulationControl_Impl::LightingSimulationControl_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == LightingSimulationControl::iddObjectType());
  }

  LightingSimulationControl_Impl::LightingSimulationControl_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == LightingSimulationControl::iddObjectType());
  }

  LightingSimulationControl_Impl::LightingSimulationControl_Impl(const LightingSimulationControl_Impl& other,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& LightingSimulationControl_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType LightingSimulationControl_Impl::iddObjectType() const {
    return LightingSimulationControl::iddObjectType();
  }

  bool LightingSimulationControl_Impl::runSimulationforDesignDays() const {
    boost::optional<std::string> value = getString(OS_LightingSimulationControlFields::RunSimulationforDesignDays,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }

  bool LightingSimulationControl_Impl::isRunSimulationforDesignDaysDefaulted() const {
    return isEmpty(OS_LightingSimulationControlFields::RunSimulationforDesignDays);
  }

  bool LightingSimulationControl_Impl::runSimulationforWeatherFileRunPeriods() const {
    boost::optional<std::string> value = getString(OS_LightingSimulationControlFields::RunSimulationforWeatherFileRunPeriods,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }

  bool LightingSimulationControl_Impl::isRunSimulationforWeatherFileRunPeriodsDefaulted() const {
    return isEmpty(OS_LightingSimulationControlFields::RunSimulationforWeatherFileRunPeriods);
  }

  void LightingSimulationControl_Impl::setRunSimulationforDesignDays(bool runSimulationforDesignDays) {
    bool result = false;
    if (runSimulationforDesignDays) {
      result = setString(OS_LightingSimulationControlFields::RunSimulationforDesignDays, "Yes");
    } else {
      result = setString(OS_LightingSimulationControlFields::RunSimulationforDesignDays, "No");
    }
    OS_ASSERT(result);
  }

  void LightingSimulationControl_Impl::resetRunSimulationforDesignDays() {
    bool result = setString(OS_LightingSimulationControlFields::RunSimulationforDesignDays, "");
    OS_ASSERT(result);
  }

  void LightingSimulationControl_Impl::setRunSimulationforWeatherFileRunPeriods(bool runSimulationforWeatherFileRunPeriods) {
    bool result = false;
    if (runSimulationforWeatherFileRunPeriods) {
      result = setString(OS_LightingSimulationControlFields::RunSimulationforWeatherFileRunPeriods, "Yes");
    } else {
      result = setString(OS_LightingSimulationControlFields::RunSimulationforWeatherFileRunPeriods, "No");
    }
    OS_ASSERT(result);
  }

  void LightingSimulationControl_Impl::resetRunSimulationforWeatherFileRunPeriods() {
    bool result = setString(OS_LightingSimulationControlFields::RunSimulationforWeatherFileRunPeriods, "");
    OS_ASSERT(result);
  }

} // detail

IddObjectType LightingSimulationControl::iddObjectType() {
  IddObjectType result(IddObjectType::OS_LightingSimulationControl);
  return result;
}

bool LightingSimulationControl::runSimulationforDesignDays() const {
  return getImpl<detail::LightingSimulationControl_Impl>()->runSimulationforDesignDays();
}

bool LightingSimulationControl::isRunSimulationforDesignDaysDefaulted() const {
  return getImpl<detail::LightingSimulationControl_Impl>()->isRunSimulationforDesignDaysDefaulted();
}

bool LightingSimulationControl::runSimulationforWeatherFileRunPeriods() const {
  return getImpl<detail::LightingSimulationControl_Impl>()->runSimulationforWeatherFileRunPeriods();
}

bool LightingSimulationControl::isRunSimulationforWeatherFileRunPeriodsDefaulted() const {
  return getImpl<detail::LightingSimulationControl_Impl>()->isRunSimulationforWeatherFileRunPeriodsDefaulted();
}

void LightingSimulationControl::setRunSimulationforDesignDays(bool runSimulationforDesignDays) {
  getImpl<detail::LightingSimulationControl_Impl>()->setRunSimulationforDesignDays(runSimulationforDesignDays);
}

void LightingSimulationControl::resetRunSimulationforDesignDays() {
  getImpl<detail::LightingSimulationControl_Impl>()->resetRunSimulationforDesignDays();
}

void LightingSimulationControl::setRunSimulationforWeatherFileRunPeriods(bool runSimulationforWeatherFileRunPeriods) {
  getImpl<detail::LightingSimulationControl_Impl>()->setRunSimulationforWeatherFileRunPeriods(runSimulationforWeatherFileRunPeriods);
}

void LightingSimulationControl::resetRunSimulationforWeatherFileRunPeriods() {
  getImpl<detail::LightingSimulationControl_Impl>()->resetRunSimulationforWeatherFileRunPeriods();
}

/// @cond
LightingSimulationControl::LightingSimulationControl(std::shared_ptr<detail::LightingSimulationControl_Impl> impl)
  : ModelObject(impl)
{}
LightingSimulationControl::LightingSimulationControl(Model& model)
  : ModelObject(LightingSimulationControl::iddObjectType(),model)
{}

/// @endcond


} // model
} // openstudio

