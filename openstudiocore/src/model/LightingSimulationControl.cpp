/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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
  : ModelObject(std::move(impl))
{}
LightingSimulationControl::LightingSimulationControl(Model& model)
  : ModelObject(LightingSimulationControl::iddObjectType(),model)
{}

/// @endcond


} // model
} // openstudio

