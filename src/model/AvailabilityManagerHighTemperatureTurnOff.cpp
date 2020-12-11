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

#include "AvailabilityManagerHighTemperatureTurnOff.hpp"
#include "AvailabilityManagerHighTemperatureTurnOff_Impl.hpp"
#include "Model.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AvailabilityManager_HighTemperatureTurnOff_FieldEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    AvailabilityManagerHighTemperatureTurnOff_Impl::AvailabilityManagerHighTemperatureTurnOff_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                                   bool keepHandle)
      : AvailabilityManager_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AvailabilityManagerHighTemperatureTurnOff::iddObjectType());
    }

    AvailabilityManagerHighTemperatureTurnOff_Impl::AvailabilityManagerHighTemperatureTurnOff_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : AvailabilityManager_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AvailabilityManagerHighTemperatureTurnOff::iddObjectType());
    }

    AvailabilityManagerHighTemperatureTurnOff_Impl::AvailabilityManagerHighTemperatureTurnOff_Impl(
      const AvailabilityManagerHighTemperatureTurnOff_Impl& other, Model_Impl* model, bool keepHandle)
      : AvailabilityManager_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& AvailabilityManagerHighTemperatureTurnOff_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Availability Manager High Temperature Turn Off Control Status"};
      return result;
    }

    IddObjectType AvailabilityManagerHighTemperatureTurnOff_Impl::iddObjectType() const {
      return AvailabilityManagerHighTemperatureTurnOff::iddObjectType();
    }

    boost::optional<Node> AvailabilityManagerHighTemperatureTurnOff_Impl::sensorNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Node>(OS_AvailabilityManager_HighTemperatureTurnOffFields::SensorNodeName);
    }

    double AvailabilityManagerHighTemperatureTurnOff_Impl::temperature() const {
      boost::optional<double> value = getDouble(OS_AvailabilityManager_HighTemperatureTurnOffFields::Temperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AvailabilityManagerHighTemperatureTurnOff_Impl::setSensorNode(const Node& node) {
      bool result = setPointer(OS_AvailabilityManager_HighTemperatureTurnOffFields::SensorNodeName, node.handle());
      return result;
    }

    void AvailabilityManagerHighTemperatureTurnOff_Impl::resetSensorNode() {
      bool result = setString(OS_AvailabilityManager_HighTemperatureTurnOffFields::SensorNodeName, "");
      OS_ASSERT(result);
    }

    bool AvailabilityManagerHighTemperatureTurnOff_Impl::setTemperature(double temperature) {
      bool result = setDouble(OS_AvailabilityManager_HighTemperatureTurnOffFields::Temperature, temperature);
      OS_ASSERT(result);
      return result;
    }

  }  // namespace detail

  AvailabilityManagerHighTemperatureTurnOff::AvailabilityManagerHighTemperatureTurnOff(const Model& model)
    : AvailabilityManager(AvailabilityManagerHighTemperatureTurnOff::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AvailabilityManagerHighTemperatureTurnOff_Impl>());

    setTemperature(30);
  }

  IddObjectType AvailabilityManagerHighTemperatureTurnOff::iddObjectType() {
    return IddObjectType(IddObjectType::OS_AvailabilityManager_HighTemperatureTurnOff);
  }

  boost::optional<Node> AvailabilityManagerHighTemperatureTurnOff::sensorNode() const {
    return getImpl<detail::AvailabilityManagerHighTemperatureTurnOff_Impl>()->sensorNode();
  }

  double AvailabilityManagerHighTemperatureTurnOff::temperature() const {
    return getImpl<detail::AvailabilityManagerHighTemperatureTurnOff_Impl>()->temperature();
  }

  bool AvailabilityManagerHighTemperatureTurnOff::setSensorNode(const Node& node) {
    return getImpl<detail::AvailabilityManagerHighTemperatureTurnOff_Impl>()->setSensorNode(node);
  }

  void AvailabilityManagerHighTemperatureTurnOff::resetSensorNode() {
    getImpl<detail::AvailabilityManagerHighTemperatureTurnOff_Impl>()->resetSensorNode();
  }

  bool AvailabilityManagerHighTemperatureTurnOff::setTemperature(double temperature) {
    return getImpl<detail::AvailabilityManagerHighTemperatureTurnOff_Impl>()->setTemperature(temperature);
  }

  /// @cond
  AvailabilityManagerHighTemperatureTurnOff::AvailabilityManagerHighTemperatureTurnOff(
    std::shared_ptr<detail::AvailabilityManagerHighTemperatureTurnOff_Impl> impl)
    : AvailabilityManager(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
