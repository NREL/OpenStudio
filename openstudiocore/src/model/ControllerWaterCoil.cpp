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

#include "ControllerWaterCoil.hpp"
#include "ControllerWaterCoil_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Controller_WaterCoil_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"

namespace openstudio {

namespace model {

namespace detail {

ControllerWaterCoil_Impl::ControllerWaterCoil_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
  : HVACComponent_Impl(idfObject,model,keepHandle)
{
  OS_ASSERT(idfObject.iddObject().type() == ControllerWaterCoil::iddObjectType());
}

ControllerWaterCoil_Impl::ControllerWaterCoil_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                   Model_Impl* model,
                                                   bool keepHandle)
  : HVACComponent_Impl(other,model,keepHandle)
{
  OS_ASSERT(other.iddObject().type() == ControllerWaterCoil::iddObjectType());
}

ControllerWaterCoil_Impl::ControllerWaterCoil_Impl(const ControllerWaterCoil_Impl& other,
                                                   Model_Impl* model,
                                                   bool keepHandle)
  : HVACComponent_Impl(other,model,keepHandle)
{}

const std::vector<std::string>& ControllerWaterCoil_Impl::outputVariableNames() const
{
  static std::vector<std::string> result;
  if (result.empty()){
  }
  return result;
}

IddObjectType ControllerWaterCoil_Impl::iddObjectType() const {
  return ControllerWaterCoil::iddObjectType();
}

boost::optional<std::string> ControllerWaterCoil_Impl::controlVariable() const {
  return getString(OS_Controller_WaterCoilFields::ControlVariable,true);
}

boost::optional<std::string> ControllerWaterCoil_Impl::action() const {
  return getString(OS_Controller_WaterCoilFields::Action,true);
}

boost::optional<std::string> ControllerWaterCoil_Impl::actuatorVariable() const {
  return getString(OS_Controller_WaterCoilFields::ActuatorVariable,true);
}

boost::optional<double> ControllerWaterCoil_Impl::controllerConvergenceTolerance() const {
  return getDouble(OS_Controller_WaterCoilFields::ControllerConvergenceTolerance,true);
}

bool ControllerWaterCoil_Impl::isControllerConvergenceToleranceDefaulted() const {
  return isEmpty(OS_Controller_WaterCoilFields::ControllerConvergenceTolerance);
}

bool ControllerWaterCoil_Impl::isControllerConvergenceToleranceAutosized() const {
  bool result = false;
  boost::optional<std::string> value = getString(OS_Controller_WaterCoilFields::ControllerConvergenceTolerance, true);
  if (value) {
    result = openstudio::istringEqual(value.get(), "Autosize");
  }
  return result;
}

boost::optional<double> ControllerWaterCoil_Impl::maximumActuatedFlow() const {
  return getDouble(OS_Controller_WaterCoilFields::MaximumActuatedFlow,true);
}

bool ControllerWaterCoil_Impl::isMaximumActuatedFlowAutosized() const {
  bool result = false;
  boost::optional<std::string> value = getString(OS_Controller_WaterCoilFields::MaximumActuatedFlow, true);
  if (value) {
    result = openstudio::istringEqual(value.get(), "Autosize");
  }
  return result;
}

double ControllerWaterCoil_Impl::minimumActuatedFlow() const {
  boost::optional<double> value = getDouble(OS_Controller_WaterCoilFields::MinimumActuatedFlow,true);
  OS_ASSERT(value);
  return value.get();
}

bool ControllerWaterCoil_Impl::isMinimumActuatedFlowDefaulted() const {
  return isEmpty(OS_Controller_WaterCoilFields::MinimumActuatedFlow);
}

bool ControllerWaterCoil_Impl::setControlVariable(boost::optional<std::string> controlVariable) {
  bool result = false;
  if (controlVariable) {
    result = setString(OS_Controller_WaterCoilFields::ControlVariable, controlVariable.get());
  } else {
    result = setString(OS_Controller_WaterCoilFields::ControlVariable, "");
  }
  return result;
}

void ControllerWaterCoil_Impl::resetControlVariable() {
  bool result = setString(OS_Controller_WaterCoilFields::ControlVariable, "");
  OS_ASSERT(result);
}

bool ControllerWaterCoil_Impl::setAction(boost::optional<std::string> action) {
  bool result = false;
  if (action) {
    result = setString(OS_Controller_WaterCoilFields::Action, action.get());
  } else {
    result = setString(OS_Controller_WaterCoilFields::Action, "");
  }
  return result;
}

void ControllerWaterCoil_Impl::resetAction() {
  bool result = setString(OS_Controller_WaterCoilFields::Action, "");
  OS_ASSERT(result);
}

bool ControllerWaterCoil_Impl::setActuatorVariable(boost::optional<std::string> actuatorVariable) {
  bool result = false;
  if (actuatorVariable) {
    result = setString(OS_Controller_WaterCoilFields::ActuatorVariable, actuatorVariable.get());
  } else {
    result = setString(OS_Controller_WaterCoilFields::ActuatorVariable, "");
  }
  return result;
}

void ControllerWaterCoil_Impl::resetActuatorVariable() {
  bool result = setString(OS_Controller_WaterCoilFields::ActuatorVariable, "");
  OS_ASSERT(result);
}

void ControllerWaterCoil_Impl::setControllerConvergenceTolerance(boost::optional<double> controllerConvergenceTolerance) {
  bool result = false;
  if (controllerConvergenceTolerance) {
    result = setDouble(OS_Controller_WaterCoilFields::ControllerConvergenceTolerance, controllerConvergenceTolerance.get());
  } else {
    result = setString(OS_Controller_WaterCoilFields::ControllerConvergenceTolerance, "");
  }
  OS_ASSERT(result);
}

void ControllerWaterCoil_Impl::resetControllerConvergenceTolerance() {
  bool result = setString(OS_Controller_WaterCoilFields::ControllerConvergenceTolerance, "");
  OS_ASSERT(result);
}

void ControllerWaterCoil_Impl::autosizeControllerConvergenceTolerance() {
  bool result = setString(OS_Controller_WaterCoilFields::ControllerConvergenceTolerance, "Autosize");
  OS_ASSERT(result);
}

void ControllerWaterCoil_Impl::setMaximumActuatedFlow(boost::optional<double> maximumActuatedFlow) {
  bool result = false;
  if (maximumActuatedFlow) {
    result = setDouble(OS_Controller_WaterCoilFields::MaximumActuatedFlow, maximumActuatedFlow.get());
  } else {
    result = setString(OS_Controller_WaterCoilFields::MaximumActuatedFlow, "");
  }
  OS_ASSERT(result);
}

void ControllerWaterCoil_Impl::resetMaximumActuatedFlow() {
  bool result = setString(OS_Controller_WaterCoilFields::MaximumActuatedFlow, "");
  OS_ASSERT(result);
}

void ControllerWaterCoil_Impl::autosizeMaximumActuatedFlow() {
  bool result = setString(OS_Controller_WaterCoilFields::MaximumActuatedFlow, "Autosize");
  OS_ASSERT(result);
}

void ControllerWaterCoil_Impl::setMinimumActuatedFlow(double minimumActuatedFlow) {
  bool result = setDouble(OS_Controller_WaterCoilFields::MinimumActuatedFlow, minimumActuatedFlow);
  OS_ASSERT(result);
}

void ControllerWaterCoil_Impl::resetMinimumActuatedFlow() {
  bool result = setString(OS_Controller_WaterCoilFields::MinimumActuatedFlow, "");
  OS_ASSERT(result);
}

boost::optional<Node> ControllerWaterCoil_Impl::sensorNode() const
{
  return this->getObject<ModelObject>().getModelObjectTarget<Node>(OS_Controller_WaterCoilFields::SensorNodeName);
}

boost::optional<Node> ControllerWaterCoil_Impl::actuatorNode() const
{
  return this->getObject<ModelObject>().getModelObjectTarget<Node>(OS_Controller_WaterCoilFields::ActuatorNodeName);
}

void ControllerWaterCoil_Impl::setSensorNode( Node & node )
{
  this->setPointer(OS_Controller_WaterCoilFields::SensorNodeName,node.handle());
}

void ControllerWaterCoil_Impl::setActuatorNode( Node & node )
{
  this->setPointer(OS_Controller_WaterCoilFields::ActuatorNodeName,node.handle());
}

void ControllerWaterCoil_Impl::setWaterCoil( const HVACComponent & comp )
{
  auto result = setPointer(OS_Controller_WaterCoilFields::WaterCoilName,comp.handle());
  OS_ASSERT(result);
}

boost::optional<HVACComponent> ControllerWaterCoil_Impl::waterCoil() const
{
  return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_Controller_WaterCoilFields::WaterCoilName);
}

} // detail

ControllerWaterCoil::ControllerWaterCoil(const Model& model)
  : HVACComponent(ControllerWaterCoil::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ControllerWaterCoil_Impl>());
  setMinimumActuatedFlow(0.0);
}

IddObjectType ControllerWaterCoil::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Controller_WaterCoil);
  return result;
}

std::vector<std::string> ControllerWaterCoil::validControlVariableValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Controller_WaterCoilFields::ControlVariable);
}

std::vector<std::string> ControllerWaterCoil::validActionValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Controller_WaterCoilFields::Action);
}

std::vector<std::string> ControllerWaterCoil::validActuatorVariableValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Controller_WaterCoilFields::ActuatorVariable);
}

boost::optional<std::string> ControllerWaterCoil::controlVariable() const {
  return getImpl<detail::ControllerWaterCoil_Impl>()->controlVariable();
}

boost::optional<std::string> ControllerWaterCoil::action() const {
  return getImpl<detail::ControllerWaterCoil_Impl>()->action();
}

boost::optional<std::string> ControllerWaterCoil::actuatorVariable() const {
  return getImpl<detail::ControllerWaterCoil_Impl>()->actuatorVariable();
}

boost::optional<double> ControllerWaterCoil::controllerConvergenceTolerance() const {
  return getImpl<detail::ControllerWaterCoil_Impl>()->controllerConvergenceTolerance();
}

bool ControllerWaterCoil::isControllerConvergenceToleranceDefaulted() const {
  return getImpl<detail::ControllerWaterCoil_Impl>()->isControllerConvergenceToleranceDefaulted();
}

bool ControllerWaterCoil::isControllerConvergenceToleranceAutosized() const {
  return getImpl<detail::ControllerWaterCoil_Impl>()->isControllerConvergenceToleranceAutosized();
}

boost::optional<double> ControllerWaterCoil::maximumActuatedFlow() const {
  return getImpl<detail::ControllerWaterCoil_Impl>()->maximumActuatedFlow();
}

bool ControllerWaterCoil::isMaximumActuatedFlowAutosized() const {
  return getImpl<detail::ControllerWaterCoil_Impl>()->isMaximumActuatedFlowAutosized();
}

double ControllerWaterCoil::minimumActuatedFlow() const {
  return getImpl<detail::ControllerWaterCoil_Impl>()->minimumActuatedFlow();
}

bool ControllerWaterCoil::isMinimumActuatedFlowDefaulted() const {
  return getImpl<detail::ControllerWaterCoil_Impl>()->isMinimumActuatedFlowDefaulted();
}

bool ControllerWaterCoil::setControlVariable(std::string controlVariable) {
  return getImpl<detail::ControllerWaterCoil_Impl>()->setControlVariable(controlVariable);
}

void ControllerWaterCoil::resetControlVariable() {
  getImpl<detail::ControllerWaterCoil_Impl>()->resetControlVariable();
}

bool ControllerWaterCoil::setAction(std::string action) {
  return getImpl<detail::ControllerWaterCoil_Impl>()->setAction(action);
}

void ControllerWaterCoil::resetAction() {
  getImpl<detail::ControllerWaterCoil_Impl>()->resetAction();
}

bool ControllerWaterCoil::setActuatorVariable(std::string actuatorVariable) {
  return getImpl<detail::ControllerWaterCoil_Impl>()->setActuatorVariable(actuatorVariable);
}

void ControllerWaterCoil::resetActuatorVariable() {
  getImpl<detail::ControllerWaterCoil_Impl>()->resetActuatorVariable();
}

void ControllerWaterCoil::setControllerConvergenceTolerance(double controllerConvergenceTolerance) {
  getImpl<detail::ControllerWaterCoil_Impl>()->setControllerConvergenceTolerance(controllerConvergenceTolerance);
}

void ControllerWaterCoil::resetControllerConvergenceTolerance() {
  getImpl<detail::ControllerWaterCoil_Impl>()->resetControllerConvergenceTolerance();
}

void ControllerWaterCoil::autosizeControllerConvergenceTolerance() {
  getImpl<detail::ControllerWaterCoil_Impl>()->autosizeControllerConvergenceTolerance();
}

void ControllerWaterCoil::setMaximumActuatedFlow(double maximumActuatedFlow) {
  getImpl<detail::ControllerWaterCoil_Impl>()->setMaximumActuatedFlow(maximumActuatedFlow);
}

void ControllerWaterCoil::resetMaximumActuatedFlow() {
  getImpl<detail::ControllerWaterCoil_Impl>()->resetMaximumActuatedFlow();
}

void ControllerWaterCoil::autosizeMaximumActuatedFlow() {
  getImpl<detail::ControllerWaterCoil_Impl>()->autosizeMaximumActuatedFlow();
}

void ControllerWaterCoil::setMinimumActuatedFlow(double minimumActuatedFlow) {
  getImpl<detail::ControllerWaterCoil_Impl>()->setMinimumActuatedFlow(minimumActuatedFlow);
}

void ControllerWaterCoil::resetMinimumActuatedFlow() {
  getImpl<detail::ControllerWaterCoil_Impl>()->resetMinimumActuatedFlow();
}

boost::optional<Node> ControllerWaterCoil::sensorNode() const
{
  return getImpl<detail::ControllerWaterCoil_Impl>()->sensorNode();
}

boost::optional<Node> ControllerWaterCoil::actuatorNode() const
{
  return getImpl<detail::ControllerWaterCoil_Impl>()->actuatorNode();
}

void ControllerWaterCoil::setSensorNode( Node & node )
{
  getImpl<detail::ControllerWaterCoil_Impl>()->setSensorNode( node );
}

void ControllerWaterCoil::setActuatorNode( Node & node )
{
  getImpl<detail::ControllerWaterCoil_Impl>()->setActuatorNode( node );
}

/// @cond
ControllerWaterCoil::ControllerWaterCoil(std::shared_ptr<detail::ControllerWaterCoil_Impl> impl)
  : HVACComponent(std::move(impl))
{}
/// @endcond


} // model

} // openstudio

