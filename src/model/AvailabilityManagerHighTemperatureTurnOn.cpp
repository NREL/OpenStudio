/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "AvailabilityManagerHighTemperatureTurnOn.hpp"
#include "AvailabilityManagerHighTemperatureTurnOn_Impl.hpp"
#include "Model.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AvailabilityManager_HighTemperatureTurnOn_FieldEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    AvailabilityManagerHighTemperatureTurnOn_Impl::AvailabilityManagerHighTemperatureTurnOn_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                                 bool keepHandle)
      : AvailabilityManager_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AvailabilityManagerHighTemperatureTurnOn::iddObjectType());
    }

    AvailabilityManagerHighTemperatureTurnOn_Impl::AvailabilityManagerHighTemperatureTurnOn_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : AvailabilityManager_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AvailabilityManagerHighTemperatureTurnOn::iddObjectType());
    }

    AvailabilityManagerHighTemperatureTurnOn_Impl::AvailabilityManagerHighTemperatureTurnOn_Impl(
      const AvailabilityManagerHighTemperatureTurnOn_Impl& other, Model_Impl* model, bool keepHandle)
      : AvailabilityManager_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& AvailabilityManagerHighTemperatureTurnOn_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Availability Manager High Temperature Turn On Control Status"};
      return result;
    }

    IddObjectType AvailabilityManagerHighTemperatureTurnOn_Impl::iddObjectType() const {
      return AvailabilityManagerHighTemperatureTurnOn::iddObjectType();
    }

    boost::optional<Node> AvailabilityManagerHighTemperatureTurnOn_Impl::sensorNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Node>(OS_AvailabilityManager_HighTemperatureTurnOnFields::SensorNodeName);
    }

    double AvailabilityManagerHighTemperatureTurnOn_Impl::temperature() const {
      boost::optional<double> value = getDouble(OS_AvailabilityManager_HighTemperatureTurnOnFields::Temperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AvailabilityManagerHighTemperatureTurnOn_Impl::setSensorNode(const Node& node) {
      bool result = setPointer(OS_AvailabilityManager_HighTemperatureTurnOnFields::SensorNodeName, node.handle());
      return result;
    }

    void AvailabilityManagerHighTemperatureTurnOn_Impl::resetSensorNode() {
      bool result = setString(OS_AvailabilityManager_HighTemperatureTurnOnFields::SensorNodeName, "");
      OS_ASSERT(result);
    }

    bool AvailabilityManagerHighTemperatureTurnOn_Impl::setTemperature(double temperature) {
      bool result = setDouble(OS_AvailabilityManager_HighTemperatureTurnOnFields::Temperature, temperature);
      OS_ASSERT(result);
      return result;
    }

  }  // namespace detail

  AvailabilityManagerHighTemperatureTurnOn::AvailabilityManagerHighTemperatureTurnOn(const Model& model)
    : AvailabilityManager(AvailabilityManagerHighTemperatureTurnOn::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AvailabilityManagerHighTemperatureTurnOn_Impl>());

    setTemperature(30);
  }

  IddObjectType AvailabilityManagerHighTemperatureTurnOn::iddObjectType() {
    return IddObjectType(IddObjectType::OS_AvailabilityManager_HighTemperatureTurnOn);
  }

  boost::optional<Node> AvailabilityManagerHighTemperatureTurnOn::sensorNode() const {
    return getImpl<detail::AvailabilityManagerHighTemperatureTurnOn_Impl>()->sensorNode();
  }

  double AvailabilityManagerHighTemperatureTurnOn::temperature() const {
    return getImpl<detail::AvailabilityManagerHighTemperatureTurnOn_Impl>()->temperature();
  }

  bool AvailabilityManagerHighTemperatureTurnOn::setSensorNode(const Node& node) {
    return getImpl<detail::AvailabilityManagerHighTemperatureTurnOn_Impl>()->setSensorNode(node);
  }

  void AvailabilityManagerHighTemperatureTurnOn::resetSensorNode() {
    getImpl<detail::AvailabilityManagerHighTemperatureTurnOn_Impl>()->resetSensorNode();
  }

  bool AvailabilityManagerHighTemperatureTurnOn::setTemperature(double temperature) {
    return getImpl<detail::AvailabilityManagerHighTemperatureTurnOn_Impl>()->setTemperature(temperature);
  }

  /// @cond
  AvailabilityManagerHighTemperatureTurnOn::AvailabilityManagerHighTemperatureTurnOn(
    std::shared_ptr<detail::AvailabilityManagerHighTemperatureTurnOn_Impl> impl)
    : AvailabilityManager(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
