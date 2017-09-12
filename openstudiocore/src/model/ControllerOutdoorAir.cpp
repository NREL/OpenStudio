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

#include "ControllerOutdoorAir.hpp"
#include "ControllerOutdoorAir_Impl.hpp"
#include "AirLoopHVACOutdoorAirSystem.hpp"
#include "AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "ControllerMechanicalVentilation.hpp"
#include "ControllerMechanicalVentilation_Impl.hpp"
#include "ScheduleCompact.hpp"
#include "ScheduleCompact_Impl.hpp"
#include "CurveQuadratic.hpp"
#include "CurveQuadratic_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/OS_Controller_OutdoorAir_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Compare.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

using openstudio::Handle;
using openstudio::OptionalHandle;
using openstudio::HandleVector;
using openstudio::IdfObject;
using openstudio::WorkspaceObject;
using openstudio::OptionalWorkspaceObject;
using openstudio::WorkspaceObjectVector;
using openstudio::Workspace;
using openstudio::istringEqual;

namespace openstudio {
namespace model {

namespace detail {

  ControllerOutdoorAir_Impl::ControllerOutdoorAir_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ParentObject_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ControllerOutdoorAir::iddObjectType());
  }

  ControllerOutdoorAir_Impl::ControllerOutdoorAir_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other,Model_Impl* model,bool keepHandle)
        : ParentObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ControllerOutdoorAir::iddObjectType());
  }

  ControllerOutdoorAir_Impl::ControllerOutdoorAir_Impl(const ControllerOutdoorAir_Impl& other,
                                                       Model_Impl* model,
                                                       bool keepHandle)
                                                         : ParentObject_Impl(other,model,keepHandle)
  {}

  // virtual destructor
  ControllerOutdoorAir_Impl::~ControllerOutdoorAir_Impl(){}

  // Get all output variable names that could be associated with this object.
  const std::vector<std::string>& ControllerOutdoorAir_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ControllerOutdoorAir_Impl::iddObjectType() const {
    return ControllerOutdoorAir::iddObjectType();
  }

  // return the parent object in the hierarchy
  boost::optional<ParentObject> ControllerOutdoorAir_Impl::parent() const
  {
    return boost::optional<ParentObject>();
  }

  // return any children objects in the hierarchy
  std::vector<ModelObject> ControllerOutdoorAir_Impl::children() const
  {
    std::vector<ModelObject> result;
    //result.push_back(this->getElectronicEnthalpyLimitCurve());
    //result.push_back(this->getMinimumOutdoorAirSchedule());
    //result.push_back(this->getMinimumFractionOfOutdoorAirSchedule());
    //result.push_back(this->getMaximumFractionOfOutdoorAirSchedule());
    //result.push_back(this->getTimeOfDayEconomizerControlSchedule());
    return result;
  }

  CurveQuadratic ControllerOutdoorAir_Impl::getElectronicEnthalpyLimitCurve() const
  {
    try
    {
      OptionalWorkspaceObject wo = this->getTarget(openstudio::OS_Controller_OutdoorAirFields::ElectronicEnthalpyLimitCurveName);
      OptionalCurveQuadratic curveQuadratic = wo->optionalCast<CurveQuadratic>();
      return *curveQuadratic;
    }
    catch(...)
    {
      LOG(Error, "Failed to retrieve electronic enthalpy limit curve" )
          throw;
    }
  }

  ScheduleCompact ControllerOutdoorAir_Impl::getMinimumOutdoorAirSchedule() const
  {
    try
    {
      OptionalWorkspaceObject wo = this->getTarget(openstudio::OS_Controller_OutdoorAirFields::MinimumOutdoorAirScheduleName);
      OptionalScheduleCompact schedule = wo->optionalCast<ScheduleCompact>();
      return *schedule;
    }
    catch(...)
    {
      LOG(Error, "Failed to retrieve minimum outdoor air schedule" )
          throw;
    }
  }

  ScheduleCompact ControllerOutdoorAir_Impl::getMinimumFractionOfOutdoorAirSchedule() const
  {
    try
    {
      OptionalWorkspaceObject wo = this->getTarget(openstudio::OS_Controller_OutdoorAirFields::MinimumFractionofOutdoorAirScheduleName);
      OptionalScheduleCompact schedule = wo->optionalCast<ScheduleCompact>();
      return *schedule;
    }
    catch(...)
    {
      LOG(Error, "Failed to retrieve minimum fraciton of outdoor air schedule" )
          throw;
    }
  }

  ScheduleCompact ControllerOutdoorAir_Impl::getMaximumFractionOfOutdoorAirSchedule() const
  {
    try
    {
      OptionalWorkspaceObject wo = this->getTarget(openstudio::OS_Controller_OutdoorAirFields::MaximumFractionofOutdoorAirScheduleName);
      OptionalScheduleCompact schedule = wo->optionalCast<ScheduleCompact>();
      return *schedule;
    }
    catch(...)
    {
      LOG(Error, "Failed to retrieve maximum fraction of outdoor air schedule" )
          throw;
    }
  }

  ScheduleCompact ControllerOutdoorAir_Impl::getTimeOfDayEconomizerControlSchedule() const
  {
    try
    {
      OptionalWorkspaceObject wo = this->getTarget(openstudio::OS_Controller_OutdoorAirFields::TimeofDayEconomizerControlScheduleName);
      OptionalScheduleCompact schedule = wo->optionalCast<ScheduleCompact>();
      return *schedule;
    }
    catch(...)
    {
      LOG(Error, "Failed to retrieve time of day economizer control schedule" )
          throw;
    }
  }

  OptionalAirLoopHVACOutdoorAirSystem ControllerOutdoorAir_Impl::airLoopHVACOutdoorAirSystem() const
  {
    AirLoopHVACOutdoorAirSystemVector oaSystems;
    oaSystems = this->model().getConcreteModelObjects<AirLoopHVACOutdoorAirSystem>();
    AirLoopHVACOutdoorAirSystemVector::iterator it;
    for( it = oaSystems.begin();
    it != oaSystems.end();
    ++it
        )
    {
      if(it->getControllerOutdoorAir().handle() == this->handle())
      {
        return OptionalAirLoopHVACOutdoorAirSystem(*it);
      }
    }
    return OptionalAirLoopHVACOutdoorAirSystem();
  }

  boost::optional<double> ControllerOutdoorAir_Impl::minimumOutdoorAirFlowRate() const {
    return getDouble(OS_Controller_OutdoorAirFields::MinimumOutdoorAirFlowRate,true);
  }

  OSOptionalQuantity ControllerOutdoorAir_Impl::getMinimumOutdoorAirFlowRate(bool returnIP) const {
    OptionalDouble value = minimumOutdoorAirFlowRate();
    return getQuantityFromDouble(OS_Controller_OutdoorAirFields::MinimumOutdoorAirFlowRate, value, returnIP);
  }

  bool ControllerOutdoorAir_Impl::isMinimumOutdoorAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Controller_OutdoorAirFields::MinimumOutdoorAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> ControllerOutdoorAir_Impl::maximumOutdoorAirFlowRate() const {
    return getDouble(OS_Controller_OutdoorAirFields::MaximumOutdoorAirFlowRate,true);
  }

  OSOptionalQuantity ControllerOutdoorAir_Impl::getMaximumOutdoorAirFlowRate(bool returnIP) const {
    OptionalDouble value = maximumOutdoorAirFlowRate();
    return getQuantityFromDouble(OS_Controller_OutdoorAirFields::MaximumOutdoorAirFlowRate, value, returnIP);
  }

  bool ControllerOutdoorAir_Impl::isMaximumOutdoorAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Controller_OutdoorAirFields::MaximumOutdoorAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  void ControllerOutdoorAir_Impl::setMinimumOutdoorAirFlowRate(boost::optional<double> minimumOutdoorAirFlowRate) {
    bool result(false);
    if (minimumOutdoorAirFlowRate) {
      result = setDouble(OS_Controller_OutdoorAirFields::MinimumOutdoorAirFlowRate, minimumOutdoorAirFlowRate.get());
    }
    OS_ASSERT(result);
  }

  bool ControllerOutdoorAir_Impl::setMinimumOutdoorAirFlowRate(const OSOptionalQuantity& minimumOutdoorAirFlowRate) {
    bool result(false);
    OptionalDouble value;
    if (minimumOutdoorAirFlowRate.isSet()) {
      value = getDoubleFromQuantity(OS_Controller_OutdoorAirFields::MinimumOutdoorAirFlowRate,minimumOutdoorAirFlowRate.get());
      if (value) {
        setMinimumOutdoorAirFlowRate(value);
        result = true;
      }
    }
    else {
      setMinimumOutdoorAirFlowRate(value);
      result = true;
    }
    return result;
  }

  void ControllerOutdoorAir_Impl::autosizeMinimumOutdoorAirFlowRate() {
    bool result = setString(OS_Controller_OutdoorAirFields::MinimumOutdoorAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

  void ControllerOutdoorAir_Impl::setMaximumOutdoorAirFlowRate(boost::optional<double> maximumOutdoorAirFlowRate) {
    bool result(false);
    if (maximumOutdoorAirFlowRate) {
      result = setDouble(OS_Controller_OutdoorAirFields::MaximumOutdoorAirFlowRate, maximumOutdoorAirFlowRate.get());
    }
    OS_ASSERT(result);
  }

  bool ControllerOutdoorAir_Impl::setMaximumOutdoorAirFlowRate(const OSOptionalQuantity& maximumOutdoorAirFlowRate) {
    bool result(false);
    OptionalDouble value;
    if (maximumOutdoorAirFlowRate.isSet()) {
      value = getDoubleFromQuantity(OS_Controller_OutdoorAirFields::MaximumOutdoorAirFlowRate,maximumOutdoorAirFlowRate.get());
      if (value) {
        setMaximumOutdoorAirFlowRate(value);
        result = true;
      }
    }
    else {
      setMaximumOutdoorAirFlowRate(value);
      result = true;
    }
    return result;
  }

  void ControllerOutdoorAir_Impl::autosizeMaximumOutdoorAirFlowRate() {
    bool result = setString(OS_Controller_OutdoorAirFields::MaximumOutdoorAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

  std::string ControllerOutdoorAir_Impl::getEconomizerControlType() const
  {
    return getString(openstudio::OS_Controller_OutdoorAirFields::EconomizerControlType).get();
  }

  void ControllerOutdoorAir_Impl::setEconomizerControlType( const std::string& value )
  {
    setString(openstudio::OS_Controller_OutdoorAirFields::EconomizerControlType, value);
  }

  std::string ControllerOutdoorAir_Impl::getEconomizerControlActionType() const
  {
    return getString(openstudio::OS_Controller_OutdoorAirFields::EconomizerControlActionType).get();
  }

  void ControllerOutdoorAir_Impl::setEconomizerControlActionType( const std::string& value )
  {
    setString(openstudio::OS_Controller_OutdoorAirFields::EconomizerControlActionType, value);
  }






  //get needs to return a boost optional double since "" is a valid input
  boost::optional<double> ControllerOutdoorAir_Impl::getEconomizerMaximumLimitDryBulbTemperature() const
  {
    return getDouble(openstudio::OS_Controller_OutdoorAirFields::EconomizerMaximumLimitDryBulbTemperature);
  }

  //set needs to return a boolean (true if successful, false if not successful)
  bool ControllerOutdoorAir_Impl::setEconomizerMaximumLimitDryBulbTemperature(boost::optional<double> value)
  {
    bool result = false;
    if (value) {
      result = setDouble(openstudio::OS_Controller_OutdoorAirFields::EconomizerMaximumLimitDryBulbTemperature, value.get());
    }
    else {
      result = setString(openstudio::OS_Controller_OutdoorAirFields::EconomizerMaximumLimitDryBulbTemperature, "");
    }
    return result;
  }








  //get needs to return a boost optional double since "" is a valid input
  boost::optional<double> ControllerOutdoorAir_Impl::getEconomizerMaximumLimitEnthalpy() const
  {
    return getDouble(openstudio::OS_Controller_OutdoorAirFields::EconomizerMaximumLimitEnthalpy);
  }

  bool ControllerOutdoorAir_Impl::setEconomizerMaximumLimitEnthalpy(boost::optional<double> value )
  {
    bool result = false;
    if (value) {
      result = setDouble(openstudio::OS_Controller_OutdoorAirFields::EconomizerMaximumLimitEnthalpy, value.get());
    }
    else {
      result = setString(openstudio::OS_Controller_OutdoorAirFields::EconomizerMaximumLimitEnthalpy, "");
    }
    return result;
  }

  //get needs to return a boost optional double since "" is a valid input
  boost::optional<double> ControllerOutdoorAir_Impl::getEconomizerMaximumLimitDewpointTemperature() const
  {
    return getDouble(openstudio::OS_Controller_OutdoorAirFields::EconomizerMaximumLimitDewpointTemperature);
  }

  bool ControllerOutdoorAir_Impl::setEconomizerMaximumLimitDewpointTemperature(boost::optional<double> value )
  {
    bool result = false;
    if (value) {
      result = setDouble(openstudio::OS_Controller_OutdoorAirFields::EconomizerMaximumLimitDewpointTemperature, value.get());
    }
    else {
      result = setString(openstudio::OS_Controller_OutdoorAirFields::EconomizerMaximumLimitDewpointTemperature, "");
    }
    return result;
  }

  //get needs to return a boost optional double since "" is a valid input
  boost::optional<double> ControllerOutdoorAir_Impl::getEconomizerMinimumLimitDryBulbTemperature() const
  {
    return getDouble(openstudio::OS_Controller_OutdoorAirFields::EconomizerMinimumLimitDryBulbTemperature);
  }

  bool ControllerOutdoorAir_Impl::setEconomizerMinimumLimitDryBulbTemperature(boost::optional<double> value )
  {
    bool result = false;
    if (value) {
      result = setDouble(openstudio::OS_Controller_OutdoorAirFields::EconomizerMinimumLimitDryBulbTemperature,value.get());
    }
    else {
      result = setString(openstudio::OS_Controller_OutdoorAirFields::EconomizerMinimumLimitDryBulbTemperature, "");
    }
    return result;
  }




  std::string ControllerOutdoorAir_Impl::getLockoutType() const
  {
    return getString(openstudio::OS_Controller_OutdoorAirFields::LockoutType).get();
  }

  void ControllerOutdoorAir_Impl::setLockoutType( const std::string& value )
  {
    setString(openstudio::OS_Controller_OutdoorAirFields::LockoutType,value);
  }

  std::string ControllerOutdoorAir_Impl::getMinimumLimitType() const
  {
    return getString(openstudio::OS_Controller_OutdoorAirFields::MinimumLimitType).get();
  }

  void ControllerOutdoorAir_Impl::setMinimumLimitType( const std::string& value )
  {
    setString(openstudio::OS_Controller_OutdoorAirFields::MinimumLimitType,value);
  }

  boost::optional<bool> ControllerOutdoorAir_Impl::getHighHumidityControl() const
  {
    boost::optional<bool> retVal;

    if( OptionalString s=getString(OS_Controller_OutdoorAirFields::HighHumidityControl) )
    {
      if( istringEqual(s.get(),"yes") )
      {
        retVal= true;
      }
      else
      {
        retVal=false;
      }
    }

    return retVal;
  }

  void ControllerOutdoorAir_Impl::setHighHumidityControl(bool val)
  {
    if(val)
    {
      setString(OS_Controller_OutdoorAirFields::HighHumidityControl,"Yes");
    }
    else
    {
      setString(OS_Controller_OutdoorAirFields::HighHumidityControl,"No");
    }
  }

  OptionalDouble ControllerOutdoorAir_Impl::getHighHumidityOutdoorAirFlowRatio() const
  {
    return getDouble(openstudio::OS_Controller_OutdoorAirFields::HighHumidityOutdoorAirFlowRatio);
  }
  void ControllerOutdoorAir_Impl::setHighHumidityOutdoorAirFlowRatio(double v)
  {
    setDouble(openstudio::OS_Controller_OutdoorAirFields::HighHumidityOutdoorAirFlowRatio,v);
  }

  boost::optional<bool> ControllerOutdoorAir_Impl::getControlHighIndoorHumidityBasedOnOutdoorHumidityRatio() const
  {
    boost::optional<bool> retVal;
    OptionalString s=getString(OS_Controller_OutdoorAirFields::ControlHighIndoorHumidityBasedonOutdoorHumidityRatio);
    if(!s)
    {
      return retVal;
    }
    std::string temp = *s;
    boost::to_lower(temp);
    if( temp == "no")
    {
      retVal = false;
    }
    else
    {
      retVal = true;
    }
    return retVal;
  }
  void ControllerOutdoorAir_Impl::setControlHighIndoorHumidityBasedOnOutdoorHumidityRatio(bool v)
  {
    if(v)
    {
      setString(OS_Controller_OutdoorAirFields::ControlHighIndoorHumidityBasedonOutdoorHumidityRatio,"No");
    }
    else
    {
      setString(OS_Controller_OutdoorAirFields::ControlHighIndoorHumidityBasedonOutdoorHumidityRatio,"Yes");
    }
  }

  OptionalString ControllerOutdoorAir_Impl::getHeatRecoveryBypassControlType() const
  {
    return getString(openstudio::OS_Controller_OutdoorAirFields::HeatRecoveryBypassControlType);
  }

  void ControllerOutdoorAir_Impl::setHeatRecoveryBypassControlType(const std::string& v)
  {
    setString(openstudio::OS_Controller_OutdoorAirFields::HeatRecoveryBypassControlType,v);
  }

  ControllerMechanicalVentilation ControllerOutdoorAir_Impl::controllerMechanicalVentilation() const {
    boost::optional<ControllerMechanicalVentilation> value = optionalControllerMechanicalVentilation();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Controller Mechanical Ventilation attached.");
    }
    return value.get();
  }

  bool ControllerOutdoorAir_Impl::setControllerMechanicalVentilation(const ControllerMechanicalVentilation& controllerMechanicalVentilation) {
    bool result = setPointer(OS_Controller_OutdoorAirFields::ControllerMechanicalVentilation, controllerMechanicalVentilation.handle());
    return result;
  }

  boost::optional<ControllerMechanicalVentilation> ControllerOutdoorAir_Impl::optionalControllerMechanicalVentilation() const {
    return getObject<ModelObject>().getModelObjectTarget<ControllerMechanicalVentilation>(OS_Controller_OutdoorAirFields::ControllerMechanicalVentilation);
  }

  boost::optional<ModelObject> ControllerOutdoorAir_Impl::controllerMechanicalVentilationAsModelObject() const {
    OptionalModelObject result = controllerMechanicalVentilation();
    return result;
  }

  bool ControllerOutdoorAir_Impl::setControllerMechanicalVentilationAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalControllerMechanicalVentilation intermediate = modelObject->optionalCast<ControllerMechanicalVentilation>();
      if (intermediate) {
        ControllerMechanicalVentilation controllerMechanicalVentilation(*intermediate);
        return setControllerMechanicalVentilation(controllerMechanicalVentilation);
      }
    }
    return false;
  }

  std::vector<openstudio::IdfObject> ControllerOutdoorAir_Impl::remove()
  {
    controllerMechanicalVentilation().remove();

    return ParentObject_Impl::remove();
  }

  ModelObject ControllerOutdoorAir_Impl::clone(Model model) const
  {
    ControllerOutdoorAir oaControllerClone = ParentObject_Impl::clone(model).cast<ControllerOutdoorAir>();

    ControllerMechanicalVentilation mechVentControllerClone = controllerMechanicalVentilation().clone(model).cast<ControllerMechanicalVentilation>();

    oaControllerClone.setControllerMechanicalVentilation(mechVentControllerClone);

    return oaControllerClone;
  }

  boost::optional<Schedule> ControllerOutdoorAir_Impl::minimumOutdoorAirSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Controller_OutdoorAirFields::MinimumOutdoorAirScheduleName);
  }

  bool ControllerOutdoorAir_Impl::setMinimumOutdoorAirSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Controller_OutdoorAirFields::MinimumOutdoorAirScheduleName,
                              "ControllerOutdoorAir",
                              "Minimum Outdoor Air",
                              schedule);
    return result;
  }

  void ControllerOutdoorAir_Impl::resetMinimumOutdoorAirSchedule() {
    bool result = setString(OS_Controller_OutdoorAirFields::MinimumOutdoorAirScheduleName, "");
    OS_ASSERT(result);
  }

  boost::optional<Schedule> ControllerOutdoorAir_Impl::minimumFractionofOutdoorAirSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Controller_OutdoorAirFields::MinimumFractionofOutdoorAirScheduleName);
  }

  bool ControllerOutdoorAir_Impl::setMinimumFractionofOutdoorAirSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Controller_OutdoorAirFields::MinimumFractionofOutdoorAirScheduleName,
                              "ControllerOutdoorAir",
                              "Minimum Fraction of Outdoor Air",
                              schedule);
    return result;
  }

  void ControllerOutdoorAir_Impl::resetMinimumFractionofOutdoorAirSchedule() {
    bool result = setString(OS_Controller_OutdoorAirFields::MinimumFractionofOutdoorAirScheduleName, "");
    OS_ASSERT(result);
  }

  boost::optional<Schedule> ControllerOutdoorAir_Impl::maximumFractionofOutdoorAirSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Controller_OutdoorAirFields::MaximumFractionofOutdoorAirScheduleName);
  }

  bool ControllerOutdoorAir_Impl::setMaximumFractionofOutdoorAirSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Controller_OutdoorAirFields::MaximumFractionofOutdoorAirScheduleName,
                              "ControllerOutdoorAir",
                              "Maximum Fraction of Outdoor Air",
                              schedule);
    return result;
  }

  void ControllerOutdoorAir_Impl::resetMaximumFractionofOutdoorAirSchedule() {
    bool result = setString(OS_Controller_OutdoorAirFields::MaximumFractionofOutdoorAirScheduleName, "");
    OS_ASSERT(result);
  }

  boost::optional<Schedule> ControllerOutdoorAir_Impl::timeofDayEconomizerControlSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Controller_OutdoorAirFields::TimeofDayEconomizerControlScheduleName);
  }

  bool ControllerOutdoorAir_Impl::setTimeofDayEconomizerControlSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Controller_OutdoorAirFields::TimeofDayEconomizerControlScheduleName,
                              "ControllerOutdoorAir",
                              "Time of Day Economizer Control",
                              schedule);
    return result;
  }

  void ControllerOutdoorAir_Impl::resetTimeofDayEconomizerControlSchedule() {
    bool result = setString(OS_Controller_OutdoorAirFields::TimeofDayEconomizerControlScheduleName, "");
    OS_ASSERT(result);
  }

  std::vector<ScheduleTypeKey> ControllerOutdoorAir_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    // TODO: Check schedule display names.
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Controller_OutdoorAirFields::MinimumOutdoorAirScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ControllerOutdoorAir","Minimum Outdoor Air"));
    }
    if (std::find(b,e,OS_Controller_OutdoorAirFields::MinimumFractionofOutdoorAirScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ControllerOutdoorAir","Minimum Fraction of Outdoor Air"));
    }
    if (std::find(b,e,OS_Controller_OutdoorAirFields::MaximumFractionofOutdoorAirScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ControllerOutdoorAir","Maximum Fraction of Outdoor Air"));
    }
    if (std::find(b,e,OS_Controller_OutdoorAirFields::TimeofDayEconomizerControlScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ControllerOutdoorAir","Time of Day Economizer Control"));
    }
    return result;
  }

} // detail

// create a new ControllerOutdoorAir object in the model's workspace
ControllerOutdoorAir::ControllerOutdoorAir(const Model& model)
  : ParentObject(ControllerOutdoorAir::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ControllerOutdoorAir_Impl>());

  setString(OS_Controller_OutdoorAirFields::MaximumOutdoorAirFlowRate,"Autosize");
  setMinimumOutdoorAirFlowRate(0.0);
  setEconomizerControlType("NoEconomizer");
  setEconomizerControlActionType("ModulateFlow");
  setEconomizerMaximumLimitDryBulbTemperature(28.0);
  setEconomizerMaximumLimitEnthalpy(64000.0);
  setString(OS_Controller_OutdoorAirFields::EconomizerMaximumLimitDewpointTemperature,"");
  setString(OS_Controller_OutdoorAirFields::ElectronicEnthalpyLimitCurveName,"");
  setEconomizerMinimumLimitDryBulbTemperature(-100.0);
  setLockoutType("NoLockout");
  setMinimumLimitType("FixedMinimum");
  setString(OS_Controller_OutdoorAirFields::MinimumOutdoorAirScheduleName,"");
  setString(OS_Controller_OutdoorAirFields::MinimumFractionofOutdoorAirScheduleName,"");
  setString(OS_Controller_OutdoorAirFields::MaximumFractionofOutdoorAirScheduleName,"");
  setString(OS_Controller_OutdoorAirFields::ControllerMechanicalVentilation,"");
  setString(OS_Controller_OutdoorAirFields::TimeofDayEconomizerControlScheduleName,"");
  setHighHumidityControl(false);
  setString(OS_Controller_OutdoorAirFields::HumidistatControlZoneName,"");
  setString(OS_Controller_OutdoorAirFields::HighHumidityOutdoorAirFlowRatio,"");
  setString(OS_Controller_OutdoorAirFields::ControlHighIndoorHumidityBasedonOutdoorHumidityRatio,"");
  setHeatRecoveryBypassControlType("BypassWhenWithinEconomizerLimits");

  ControllerMechanicalVentilation controllerMechanicalVentilation(model);
  setControllerMechanicalVentilation(controllerMechanicalVentilation);

}

ControllerOutdoorAir::ControllerOutdoorAir(std::shared_ptr<detail::ControllerOutdoorAir_Impl> impl)
  : ParentObject(std::move(impl))
{
}

CurveQuadratic ControllerOutdoorAir::getElectronicEnthalpyLimitCurve() const
{
  return getImpl<detail::ControllerOutdoorAir_Impl>()->getElectronicEnthalpyLimitCurve();
}

ScheduleCompact ControllerOutdoorAir::getMinimumOutdoorAirSchedule() const
{
  return getImpl<detail::ControllerOutdoorAir_Impl>()->getMinimumOutdoorAirSchedule();
}

ScheduleCompact ControllerOutdoorAir::getMinimumFractionOfOutdoorAirSchedule() const
{
  return getImpl<detail::ControllerOutdoorAir_Impl>()->getMinimumFractionOfOutdoorAirSchedule();
}

ScheduleCompact ControllerOutdoorAir::getMaximumFractionOfOutdoorAirSchedule() const
{
  return getImpl<detail::ControllerOutdoorAir_Impl>()->getMaximumFractionOfOutdoorAirSchedule();
}

ScheduleCompact ControllerOutdoorAir::getTimeOfDayEconomizerControlSchedule() const
{
  return getImpl<detail::ControllerOutdoorAir_Impl>()->getTimeOfDayEconomizerControlSchedule();
}

boost::optional<AirLoopHVACOutdoorAirSystem> ControllerOutdoorAir::airLoopHVACOutdoorAirSystem() const
{
  return getImpl<detail::ControllerOutdoorAir_Impl>()->airLoopHVACOutdoorAirSystem();
}

std::string ControllerOutdoorAir::getEconomizerControlType() const
{
  return getImpl<detail::ControllerOutdoorAir_Impl>()->getEconomizerControlType();
}

void ControllerOutdoorAir::setEconomizerControlType( const std::string& value )
{
  getImpl<detail::ControllerOutdoorAir_Impl>()->setEconomizerControlType(value);
}

std::string ControllerOutdoorAir::getEconomizerControlActionType() const
{
  return getImpl<detail::ControllerOutdoorAir_Impl>()->getEconomizerControlActionType();
}

void ControllerOutdoorAir::setEconomizerControlActionType( const std::string& value )
{
  getImpl<detail::ControllerOutdoorAir_Impl>()->setEconomizerControlActionType(value);
}

boost::optional<double> ControllerOutdoorAir::getEconomizerMaximumLimitDryBulbTemperature() const
{
  return getImpl<detail::ControllerOutdoorAir_Impl>()->getEconomizerMaximumLimitDryBulbTemperature();
}

bool ControllerOutdoorAir::setEconomizerMaximumLimitDryBulbTemperature( double value )
{
  return getImpl<detail::ControllerOutdoorAir_Impl>()->setEconomizerMaximumLimitDryBulbTemperature(value);
}

void ControllerOutdoorAir::resetEconomizerMaximumLimitDryBulbTemperature( )
{
  bool test = getImpl<detail::ControllerOutdoorAir_Impl>()->setEconomizerMaximumLimitDryBulbTemperature(boost::none);
  OS_ASSERT(test);
}

boost::optional<double> ControllerOutdoorAir::getEconomizerMaximumLimitEnthalpy() const
{
  return getImpl<detail::ControllerOutdoorAir_Impl>()->getEconomizerMaximumLimitEnthalpy();
}

bool ControllerOutdoorAir::setEconomizerMaximumLimitEnthalpy( double value )
{
  return getImpl<detail::ControllerOutdoorAir_Impl>()->setEconomizerMaximumLimitEnthalpy(value);
}

void ControllerOutdoorAir::resetEconomizerMaximumLimitEnthalpy( )
{
  bool test = getImpl<detail::ControllerOutdoorAir_Impl>()->setEconomizerMaximumLimitEnthalpy(boost::none);
  OS_ASSERT(test);
}

boost::optional<double> ControllerOutdoorAir::getEconomizerMaximumLimitDewpointTemperature() const
{
  return getImpl<detail::ControllerOutdoorAir_Impl>()->getEconomizerMaximumLimitDewpointTemperature();
}

bool ControllerOutdoorAir::setEconomizerMaximumLimitDewpointTemperature( double value )
{
  return getImpl<detail::ControllerOutdoorAir_Impl>()->setEconomizerMaximumLimitDewpointTemperature(value);
}

void ControllerOutdoorAir::resetEconomizerMaximumLimitDewpointTemperature( )
{
  bool test = getImpl<detail::ControllerOutdoorAir_Impl>()->setEconomizerMaximumLimitDewpointTemperature(boost::none);
  OS_ASSERT(test);
}

boost::optional<double> ControllerOutdoorAir::getEconomizerMinimumLimitDryBulbTemperature() const
{
  return getImpl<detail::ControllerOutdoorAir_Impl>()->getEconomizerMinimumLimitDryBulbTemperature();
}

bool ControllerOutdoorAir::setEconomizerMinimumLimitDryBulbTemperature( double value )
{
  return getImpl<detail::ControllerOutdoorAir_Impl>()->setEconomizerMinimumLimitDryBulbTemperature(value);
}

void ControllerOutdoorAir::resetEconomizerMinimumLimitDryBulbTemperature( )
{
  bool test = getImpl<detail::ControllerOutdoorAir_Impl>()->setEconomizerMinimumLimitDryBulbTemperature(boost::none);
  OS_ASSERT(test);
}

std::string ControllerOutdoorAir::getLockoutType() const
{
  return getImpl<detail::ControllerOutdoorAir_Impl>()->getLockoutType();
}

void ControllerOutdoorAir::setLockoutType( const std::string& value )
{
  getImpl<detail::ControllerOutdoorAir_Impl>()->setLockoutType(value);
}

std::string ControllerOutdoorAir::getMinimumLimitType() const
{
  return getImpl<detail::ControllerOutdoorAir_Impl>()->getMinimumLimitType();
}

void ControllerOutdoorAir::setMinimumLimitType( const std::string& value )
{
  getImpl<detail::ControllerOutdoorAir_Impl>()->setMinimumLimitType(value);
}

boost::optional<bool> ControllerOutdoorAir::getHighHumidityControl() const
{
  return getImpl<detail::ControllerOutdoorAir_Impl>()->getHighHumidityControl();
}
void ControllerOutdoorAir::setHighHumidityControl(bool val)
{
  getImpl<detail::ControllerOutdoorAir_Impl>()->setHighHumidityControl(val);
}

OptionalDouble ControllerOutdoorAir::getHighHumidityOutdoorAirFlowRatio() const
{
  return getImpl<detail::ControllerOutdoorAir_Impl>()->getHighHumidityOutdoorAirFlowRatio();
}
void ControllerOutdoorAir::setHighHumidityOutdoorAirFlowRatio(double v)
{
  getImpl<detail::ControllerOutdoorAir_Impl>()->setHighHumidityOutdoorAirFlowRatio(v);
}

boost::optional<bool> ControllerOutdoorAir::getControlHighIndoorHumidityBasedOnOutdoorHumidityRatio() const
{
  return getImpl<detail::ControllerOutdoorAir_Impl>()->getControlHighIndoorHumidityBasedOnOutdoorHumidityRatio();
}
void ControllerOutdoorAir::setControlHighIndoorHumidityBasedOnOutdoorHumidityRatio(bool v)
{
  getImpl<detail::ControllerOutdoorAir_Impl>()->setControlHighIndoorHumidityBasedOnOutdoorHumidityRatio(v);
}

OptionalString ControllerOutdoorAir::getHeatRecoveryBypassControlType() const
{
  return getImpl<detail::ControllerOutdoorAir_Impl>()->getHeatRecoveryBypassControlType();
}
void ControllerOutdoorAir::setHeatRecoveryBypassControlType(const std::string& v)
{
  getImpl<detail::ControllerOutdoorAir_Impl>()->setHeatRecoveryBypassControlType(v);
}

IddObjectType ControllerOutdoorAir::iddObjectType()
{
 IddObjectType result(IddObjectType::OS_Controller_OutdoorAir);
 return result;
}

boost::optional<double> ControllerOutdoorAir::minimumOutdoorAirFlowRate() const {
  return getImpl<detail::ControllerOutdoorAir_Impl>()->minimumOutdoorAirFlowRate();
}

OSOptionalQuantity ControllerOutdoorAir::getMinimumOutdoorAirFlowRate(bool returnIP) const {
  return getImpl<detail::ControllerOutdoorAir_Impl>()->getMinimumOutdoorAirFlowRate(returnIP);
}

bool ControllerOutdoorAir::isMinimumOutdoorAirFlowRateAutosized() const {
  return getImpl<detail::ControllerOutdoorAir_Impl>()->isMinimumOutdoorAirFlowRateAutosized();
}

boost::optional<double> ControllerOutdoorAir::maximumOutdoorAirFlowRate() const {
  return getImpl<detail::ControllerOutdoorAir_Impl>()->maximumOutdoorAirFlowRate();
}

OSOptionalQuantity ControllerOutdoorAir::getMaximumOutdoorAirFlowRate(bool returnIP) const {
  return getImpl<detail::ControllerOutdoorAir_Impl>()->getMaximumOutdoorAirFlowRate(returnIP);
}

bool ControllerOutdoorAir::isMaximumOutdoorAirFlowRateAutosized() const {
  return getImpl<detail::ControllerOutdoorAir_Impl>()->isMaximumOutdoorAirFlowRateAutosized();
}

void ControllerOutdoorAir::setMinimumOutdoorAirFlowRate(double minimumOutdoorAirFlowRate) {
  getImpl<detail::ControllerOutdoorAir_Impl>()->setMinimumOutdoorAirFlowRate(minimumOutdoorAirFlowRate);
}

bool ControllerOutdoorAir::setMinimumOutdoorAirFlowRate(const Quantity& minimumOutdoorAirFlowRate) {
  return getImpl<detail::ControllerOutdoorAir_Impl>()->setMinimumOutdoorAirFlowRate(minimumOutdoorAirFlowRate);
}

void ControllerOutdoorAir::autosizeMinimumOutdoorAirFlowRate() {
  getImpl<detail::ControllerOutdoorAir_Impl>()->autosizeMinimumOutdoorAirFlowRate();
}

void ControllerOutdoorAir::setMaximumOutdoorAirFlowRate(double maximumOutdoorAirFlowRate) {
  getImpl<detail::ControllerOutdoorAir_Impl>()->setMaximumOutdoorAirFlowRate(maximumOutdoorAirFlowRate);
}

bool ControllerOutdoorAir::setMaximumOutdoorAirFlowRate(const Quantity& maximumOutdoorAirFlowRate) {
  return getImpl<detail::ControllerOutdoorAir_Impl>()->setMaximumOutdoorAirFlowRate(maximumOutdoorAirFlowRate);
}

void ControllerOutdoorAir::autosizeMaximumOutdoorAirFlowRate() {
  getImpl<detail::ControllerOutdoorAir_Impl>()->autosizeMaximumOutdoorAirFlowRate();
}

ControllerMechanicalVentilation ControllerOutdoorAir::controllerMechanicalVentilation() const {
  return getImpl<detail::ControllerOutdoorAir_Impl>()->controllerMechanicalVentilation();
}

bool ControllerOutdoorAir::setControllerMechanicalVentilation(const ControllerMechanicalVentilation& controllerMechanicalVentilation) {
  return getImpl<detail::ControllerOutdoorAir_Impl>()->setControllerMechanicalVentilation(controllerMechanicalVentilation);
}

std::vector<openstudio::IdfObject> ControllerOutdoorAir::remove()
{
  return getImpl<detail::ControllerOutdoorAir_Impl>()->remove();
}

boost::optional<Schedule> ControllerOutdoorAir::minimumOutdoorAirSchedule() const
{
  return getImpl<detail::ControllerOutdoorAir_Impl>()->minimumOutdoorAirSchedule();
}

bool ControllerOutdoorAir::setMinimumOutdoorAirSchedule(Schedule& schedule)
{
  return getImpl<detail::ControllerOutdoorAir_Impl>()->setMinimumOutdoorAirSchedule(schedule);
}

void ControllerOutdoorAir::resetMinimumOutdoorAirSchedule()
{
  getImpl<detail::ControllerOutdoorAir_Impl>()->resetMinimumOutdoorAirSchedule();
}

boost::optional<Schedule> ControllerOutdoorAir::minimumFractionofOutdoorAirSchedule() const
{
  return getImpl<detail::ControllerOutdoorAir_Impl>()->minimumFractionofOutdoorAirSchedule();
}

bool ControllerOutdoorAir::setMinimumFractionofOutdoorAirSchedule(Schedule& schedule)
{
  return getImpl<detail::ControllerOutdoorAir_Impl>()->setMinimumFractionofOutdoorAirSchedule(schedule);
}

void ControllerOutdoorAir::resetMinimumFractionofOutdoorAirSchedule()
{
  getImpl<detail::ControllerOutdoorAir_Impl>()->resetMinimumFractionofOutdoorAirSchedule();
}

boost::optional<Schedule> ControllerOutdoorAir::maximumFractionofOutdoorAirSchedule() const
{
  return getImpl<detail::ControllerOutdoorAir_Impl>()->maximumFractionofOutdoorAirSchedule();
}

bool ControllerOutdoorAir::setMaximumFractionofOutdoorAirSchedule(Schedule& schedule)
{
  return getImpl<detail::ControllerOutdoorAir_Impl>()->setMaximumFractionofOutdoorAirSchedule(schedule);
}

void ControllerOutdoorAir::resetMaximumFractionofOutdoorAirSchedule()
{
  getImpl<detail::ControllerOutdoorAir_Impl>()->resetMaximumFractionofOutdoorAirSchedule();
}

boost::optional<Schedule> ControllerOutdoorAir::timeofDayEconomizerControlSchedule() const
{
  return getImpl<detail::ControllerOutdoorAir_Impl>()->timeofDayEconomizerControlSchedule();
}

bool ControllerOutdoorAir::setTimeofDayEconomizerControlSchedule(Schedule& schedule)
{
  return getImpl<detail::ControllerOutdoorAir_Impl>()->setTimeofDayEconomizerControlSchedule(schedule);
}

void ControllerOutdoorAir::resetTimeofDayEconomizerControlSchedule()
{
  getImpl<detail::ControllerOutdoorAir_Impl>()->resetTimeofDayEconomizerControlSchedule();
}

} // model
} // openstudio
