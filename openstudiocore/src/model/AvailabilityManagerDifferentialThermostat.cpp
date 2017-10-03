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

#include "AvailabilityManagerDifferentialThermostat.hpp"
#include "AvailabilityManagerDifferentialThermostat_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AvailabilityManager_DifferentialThermostat_FieldEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  AvailabilityManagerDifferentialThermostat_Impl::AvailabilityManagerDifferentialThermostat_Impl(const IdfObject& idfObject,
                                                                                                 Model_Impl* model,
                                                                                                 bool keepHandle)
    : AvailabilityManager_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AvailabilityManagerDifferentialThermostat::iddObjectType());
  }

  AvailabilityManagerDifferentialThermostat_Impl::AvailabilityManagerDifferentialThermostat_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                 Model_Impl* model,
                                                                                                 bool keepHandle)
    : AvailabilityManager_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AvailabilityManagerDifferentialThermostat::iddObjectType());
  }

  AvailabilityManagerDifferentialThermostat_Impl::AvailabilityManagerDifferentialThermostat_Impl(const AvailabilityManagerDifferentialThermostat_Impl& other,
                                                                                                 Model_Impl* model,
                                                                                                 bool keepHandle)
    : AvailabilityManager_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& AvailabilityManagerDifferentialThermostat_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType AvailabilityManagerDifferentialThermostat_Impl::iddObjectType() const {
    return AvailabilityManagerDifferentialThermostat::iddObjectType();
  }

  boost::optional<Node> AvailabilityManagerDifferentialThermostat_Impl::hotNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_AvailabilityManager_DifferentialThermostatFields::HotNode);
  }

  boost::optional<Node> AvailabilityManagerDifferentialThermostat_Impl::coldNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_AvailabilityManager_DifferentialThermostatFields::ColdNode);
  }

  double AvailabilityManagerDifferentialThermostat_Impl::temperatureDifferenceOnLimit() const {
    boost::optional<double> value = getDouble(OS_AvailabilityManager_DifferentialThermostatFields::TemperatureDifferenceOnLimit,true);
    OS_ASSERT(value);
    return value.get();
  }

  double AvailabilityManagerDifferentialThermostat_Impl::temperatureDifferenceOffLimit() const {
    boost::optional<double> value = getDouble(OS_AvailabilityManager_DifferentialThermostatFields::TemperatureDifferenceOffLimit,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AvailabilityManagerDifferentialThermostat_Impl::setHotNode(const boost::optional<Node>& node) {
    bool result(false);
    if (node) {
      result = setPointer(OS_AvailabilityManager_DifferentialThermostatFields::HotNode, node.get().handle());
    }
    else {
      resetHotNode();
      result = true;
    }
    return result;
  }

  void AvailabilityManagerDifferentialThermostat_Impl::resetHotNode() {
    bool result = setString(OS_AvailabilityManager_DifferentialThermostatFields::HotNode, "");
    OS_ASSERT(result);
  }

  bool AvailabilityManagerDifferentialThermostat_Impl::setColdNode(const boost::optional<Node>& node) {
    bool result(false);
    if (node) {
      result = setPointer(OS_AvailabilityManager_DifferentialThermostatFields::ColdNode, node.get().handle());
    }
    else {
      resetColdNode();
      result = true;
    }
    return result;
  }

  void AvailabilityManagerDifferentialThermostat_Impl::resetColdNode() {
    bool result = setString(OS_AvailabilityManager_DifferentialThermostatFields::ColdNode, "");
    OS_ASSERT(result);
  }

  void AvailabilityManagerDifferentialThermostat_Impl::setTemperatureDifferenceOnLimit(double temperatureDifferenceOnLimit) {
    bool result = setDouble(OS_AvailabilityManager_DifferentialThermostatFields::TemperatureDifferenceOnLimit, temperatureDifferenceOnLimit);
    OS_ASSERT(result);
  }

  void AvailabilityManagerDifferentialThermostat_Impl::setTemperatureDifferenceOffLimit(double temperatureDifferenceOffLimit) {
    bool result = setDouble(OS_AvailabilityManager_DifferentialThermostatFields::TemperatureDifferenceOffLimit, temperatureDifferenceOffLimit);
    OS_ASSERT(result);
  }

} // detail

AvailabilityManagerDifferentialThermostat::AvailabilityManagerDifferentialThermostat(const Model& model)
  : AvailabilityManager(AvailabilityManagerDifferentialThermostat::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AvailabilityManagerDifferentialThermostat_Impl>());

  setTemperatureDifferenceOnLimit(10.0);
  setTemperatureDifferenceOffLimit(2.0);
}

IddObjectType AvailabilityManagerDifferentialThermostat::iddObjectType() {
  return IddObjectType(IddObjectType::OS_AvailabilityManager_DifferentialThermostat);
}

boost::optional<Node> AvailabilityManagerDifferentialThermostat::hotNode() const {
  return getImpl<detail::AvailabilityManagerDifferentialThermostat_Impl>()->hotNode();
}

boost::optional<Node> AvailabilityManagerDifferentialThermostat::coldNode() const {
  return getImpl<detail::AvailabilityManagerDifferentialThermostat_Impl>()->coldNode();
}

double AvailabilityManagerDifferentialThermostat::temperatureDifferenceOnLimit() const {
  return getImpl<detail::AvailabilityManagerDifferentialThermostat_Impl>()->temperatureDifferenceOnLimit();
}

double AvailabilityManagerDifferentialThermostat::temperatureDifferenceOffLimit() const {
  return getImpl<detail::AvailabilityManagerDifferentialThermostat_Impl>()->temperatureDifferenceOffLimit();
}

bool AvailabilityManagerDifferentialThermostat::setHotNode(const Node& node) {
  return getImpl<detail::AvailabilityManagerDifferentialThermostat_Impl>()->setHotNode(node);
}

void AvailabilityManagerDifferentialThermostat::resetHotNode() {
  getImpl<detail::AvailabilityManagerDifferentialThermostat_Impl>()->resetHotNode();
}

bool AvailabilityManagerDifferentialThermostat::setColdNode(const Node& node) {
  return getImpl<detail::AvailabilityManagerDifferentialThermostat_Impl>()->setColdNode(node);
}

void AvailabilityManagerDifferentialThermostat::resetColdNode() {
  getImpl<detail::AvailabilityManagerDifferentialThermostat_Impl>()->resetColdNode();
}

void AvailabilityManagerDifferentialThermostat::setTemperatureDifferenceOnLimit(double temperatureDifferenceOnLimit) {
  getImpl<detail::AvailabilityManagerDifferentialThermostat_Impl>()->setTemperatureDifferenceOnLimit(temperatureDifferenceOnLimit);
}

void AvailabilityManagerDifferentialThermostat::setTemperatureDifferenceOffLimit(double temperatureDifferenceOffLimit) {
  getImpl<detail::AvailabilityManagerDifferentialThermostat_Impl>()->setTemperatureDifferenceOffLimit(temperatureDifferenceOffLimit);
}

/// @cond
AvailabilityManagerDifferentialThermostat::AvailabilityManagerDifferentialThermostat(std::shared_ptr<detail::AvailabilityManagerDifferentialThermostat_Impl> impl)
  : AvailabilityManager(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

