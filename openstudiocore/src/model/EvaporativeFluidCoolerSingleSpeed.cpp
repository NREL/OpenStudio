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

#include <model/EvaporativeFluidCoolerSingleSpeed.hpp>
#include <model/EvaporativeFluidCoolerSingleSpeed_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/ScheduleTypeLimits.hpp>
#include <model/ScheduleTypeRegistry.hpp>
#include <model/PlantLoop.hpp>
#include <model/PlantLoop_Impl.hpp>
#include <model/ConnectorSplitter.hpp>
#include <model/ConnectorSplitter_Impl.hpp>
#include <model/ConnectorMixer.hpp>
#include <model/ConnectorMixer_Impl.hpp>
#include <model/ModelObject.hpp>
#include <model/ModelObject_Impl.hpp>
#include <model/Model.hpp>
#include <model/Model_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/PortList.hpp>
#include <model/PortList_Impl.hpp>
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_EvaporativeFluidCooler_SingleSpeed_FieldEnums.hxx>
#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model {

namespace detail {

  EvaporativeFluidCoolerSingleSpeed_Impl::EvaporativeFluidCoolerSingleSpeed_Impl(const IdfObject& idfObject,
                                                                                 Model_Impl* model,
                                                                                 bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    BOOST_ASSERT(idfObject.iddObject().type() == EvaporativeFluidCoolerSingleSpeed::iddObjectType());
  }

  EvaporativeFluidCoolerSingleSpeed_Impl::EvaporativeFluidCoolerSingleSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                 Model_Impl* model,
                                                                                 bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    BOOST_ASSERT(other.iddObject().type() == EvaporativeFluidCoolerSingleSpeed::iddObjectType());
  }

  EvaporativeFluidCoolerSingleSpeed_Impl::EvaporativeFluidCoolerSingleSpeed_Impl(const EvaporativeFluidCoolerSingleSpeed_Impl& other,
                                                                                 Model_Impl* model,
                                                                                 bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& EvaporativeFluidCoolerSingleSpeed_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType EvaporativeFluidCoolerSingleSpeed_Impl::iddObjectType() const {
    return EvaporativeFluidCoolerSingleSpeed::iddObjectType();
  }

  std::vector<ScheduleTypeKey> EvaporativeFluidCoolerSingleSpeed_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_EvaporativeFluidCooler_SingleSpeedFields::BlowdownMakeupWaterUsageScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("EvaporativeFluidCoolerSingleSpeed","Blowdown Makeup Water Usage"));
    }
    return result;
  }

    unsigned EvaporativeFluidCoolerSingleSpeed_Impl::inletPort() 
  {
    return OS_EvaporativeFluidCooler_SingleSpeedFields::WaterInletNodeName;
  }

  unsigned EvaporativeFluidCoolerSingleSpeed_Impl::outletPort() 
  {
    return OS_EvaporativeFluidCooler_SingleSpeedFields::WaterOutletNodeName;
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::addToNode(Node& node)
  {
    Model _model = node.model();

    if( boost::optional<PlantLoop> _plantLoop = node.plantLoop() )
    {
      PlantLoop plantLoop = _plantLoop.get();
      if( plantLoop.supplyComponent(node.handle()) )
      {
        if( boost::optional<ModelObject> outlet = node.outletModelObject() )
        {
          if( outlet->optionalCast<ConnectorMixer>() )
          {
            if( boost::optional<ModelObject> inlet = node.inletModelObject() )
            {
              if( boost::optional<ConnectorSplitter> splitter = inlet->optionalCast<ConnectorSplitter>() )
              {
                boost::optional<ModelObject> sourceModelObject = inlet;
                boost::optional<unsigned> sourcePort = node.connectedObjectPort(node.inletPort());

                if( sourcePort && sourceModelObject )
                {
                  Node inletNode(_model);

                  _model.connect( sourceModelObject.get(),
                                  sourcePort.get(),
                                  inletNode,
                                  inletNode.inletPort() );
                  
                  _model.connect( inletNode,
                                  inletNode.outletPort(),
                                  this->getObject<ModelObject>(),
                                  this->inletPort() );

                  _model.connect( this->getObject<ModelObject>(),
                                  outletPort(),
                                  node,
                                  node.inletPort() );
                  return true; 
                }
              }
            }
          }
        }
      }
    }
    return false;
  }

  boost::optional<double> EvaporativeFluidCoolerSingleSpeed_Impl::designAirFlowRate() const {
    return getDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignAirFlowRate,true);
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::isDesignAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> EvaporativeFluidCoolerSingleSpeed_Impl::fanPoweratDesignAirFlowRate() const {
    return getDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::FanPoweratDesignAirFlowRate,true);
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::isFanPoweratDesignAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_EvaporativeFluidCooler_SingleSpeedFields::FanPoweratDesignAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double EvaporativeFluidCoolerSingleSpeed_Impl::designSprayWaterFlowRate() const {
    boost::optional<double> value = getDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignSprayWaterFlowRate,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  boost::optional<std::string> EvaporativeFluidCoolerSingleSpeed_Impl::performanceInputMethod() const {
    return getString(OS_EvaporativeFluidCooler_SingleSpeedFields::PerformanceInputMethod,true);
  }

  boost::optional<double> EvaporativeFluidCoolerSingleSpeed_Impl::standardDesignCapacity() const {
    return getDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::StandardDesignCapacity,true);
  }

  boost::optional<double> EvaporativeFluidCoolerSingleSpeed_Impl::ufactorTimesAreaValueatDesignAirFlowRate() const {
    return getDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::UfactorTimesAreaValueatDesignAirFlowRate,true);
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::isUfactorTimesAreaValueatDesignAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_EvaporativeFluidCooler_SingleSpeedFields::UfactorTimesAreaValueatDesignAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> EvaporativeFluidCoolerSingleSpeed_Impl::designWaterFlowRate() const {
    return getDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignWaterFlowRate,true);
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::isDesignWaterFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignWaterFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> EvaporativeFluidCoolerSingleSpeed_Impl::userSpecifiedDesignCapacity() const {
    return getDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::UserSpecifiedDesignCapacity,true);
  }

  boost::optional<double> EvaporativeFluidCoolerSingleSpeed_Impl::designEnteringWaterTemperature() const {
    return getDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignEnteringWaterTemperature,true);
  }

  boost::optional<double> EvaporativeFluidCoolerSingleSpeed_Impl::designEnteringAirTemperature() const {
    return getDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignEnteringAirTemperature,true);
  }

  boost::optional<double> EvaporativeFluidCoolerSingleSpeed_Impl::designEnteringAirWetbulbTemperature() const {
    return getDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignEnteringAirWetbulbTemperature,true);
  }

  std::string EvaporativeFluidCoolerSingleSpeed_Impl::capacityControl() const {
    boost::optional<std::string> value = getString(OS_EvaporativeFluidCooler_SingleSpeedFields::CapacityControl,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::isCapacityControlDefaulted() const {
    return isEmpty(OS_EvaporativeFluidCooler_SingleSpeedFields::CapacityControl);
  }

  double EvaporativeFluidCoolerSingleSpeed_Impl::sizingFactor() const {
    boost::optional<double> value = getDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::SizingFactor,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::isSizingFactorDefaulted() const {
    return isEmpty(OS_EvaporativeFluidCooler_SingleSpeedFields::SizingFactor);
  }

  std::string EvaporativeFluidCoolerSingleSpeed_Impl::evaporationLossMode() const {
    boost::optional<std::string> value = getString(OS_EvaporativeFluidCooler_SingleSpeedFields::EvaporationLossMode,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::isEvaporationLossModeDefaulted() const {
    return isEmpty(OS_EvaporativeFluidCooler_SingleSpeedFields::EvaporationLossMode);
  }

  boost::optional<double> EvaporativeFluidCoolerSingleSpeed_Impl::evaporationLossFactor() const {
    return getDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::EvaporationLossFactor,true);
  }

  double EvaporativeFluidCoolerSingleSpeed_Impl::driftLossPercent() const {
    boost::optional<double> value = getDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::DriftLossPercent,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::isDriftLossPercentDefaulted() const {
    return isEmpty(OS_EvaporativeFluidCooler_SingleSpeedFields::DriftLossPercent);
  }

  std::string EvaporativeFluidCoolerSingleSpeed_Impl::blowdownCalculationMode() const {
    boost::optional<std::string> value = getString(OS_EvaporativeFluidCooler_SingleSpeedFields::BlowdownCalculationMode,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::isBlowdownCalculationModeDefaulted() const {
    return isEmpty(OS_EvaporativeFluidCooler_SingleSpeedFields::BlowdownCalculationMode);
  }

  double EvaporativeFluidCoolerSingleSpeed_Impl::blowdownConcentrationRatio() const {
    boost::optional<double> value = getDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::BlowdownConcentrationRatio,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::isBlowdownConcentrationRatioDefaulted() const {
    return isEmpty(OS_EvaporativeFluidCooler_SingleSpeedFields::BlowdownConcentrationRatio);
  }

  boost::optional<Schedule> EvaporativeFluidCoolerSingleSpeed_Impl::blowdownMakeupWaterUsageSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_EvaporativeFluidCooler_SingleSpeedFields::BlowdownMakeupWaterUsageScheduleName);
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::setDesignAirFlowRate(boost::optional<double> designAirFlowRate) {
    bool result(false);
    if (designAirFlowRate) {
      result = setDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignAirFlowRate, designAirFlowRate.get());
    }
    return result;
  }

  void EvaporativeFluidCoolerSingleSpeed_Impl::autosizeDesignAirFlowRate() {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignAirFlowRate, "autosize");
    BOOST_ASSERT(result);
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::setFanPoweratDesignAirFlowRate(boost::optional<double> fanPoweratDesignAirFlowRate) {
    bool result(false);
    if (fanPoweratDesignAirFlowRate) {
      result = setDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::FanPoweratDesignAirFlowRate, fanPoweratDesignAirFlowRate.get());
    }
    return result;
  }

  void EvaporativeFluidCoolerSingleSpeed_Impl::autosizeFanPoweratDesignAirFlowRate() {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::FanPoweratDesignAirFlowRate, "autosize");
    BOOST_ASSERT(result);
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::setDesignSprayWaterFlowRate(double designSprayWaterFlowRate) {
    bool result = setDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignSprayWaterFlowRate, designSprayWaterFlowRate);
    return result;
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::setPerformanceInputMethod(boost::optional<std::string> performanceInputMethod) {
    bool result(false);
    if (performanceInputMethod) {
      result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::PerformanceInputMethod, performanceInputMethod.get());
    }
    else {
      resetPerformanceInputMethod();
      result = true;
    }
    return result;
  }

  void EvaporativeFluidCoolerSingleSpeed_Impl::resetPerformanceInputMethod() {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::PerformanceInputMethod, "");
    BOOST_ASSERT(result);
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::setStandardDesignCapacity(boost::optional<double> standardDesignCapacity) {
    bool result(false);
    if (standardDesignCapacity) {
      result = setDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::StandardDesignCapacity, standardDesignCapacity.get());
    }
    else {
      resetStandardDesignCapacity();
      result = true;
    }
    return result;
  }

  void EvaporativeFluidCoolerSingleSpeed_Impl::resetStandardDesignCapacity() {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::StandardDesignCapacity, "");
    BOOST_ASSERT(result);
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::setUfactorTimesAreaValueatDesignAirFlowRate(boost::optional<double> ufactorTimesAreaValueatDesignAirFlowRate) {
    bool result(false);
    if (ufactorTimesAreaValueatDesignAirFlowRate) {
      result = setDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::UfactorTimesAreaValueatDesignAirFlowRate, ufactorTimesAreaValueatDesignAirFlowRate.get());
    }
    else {
      resetUfactorTimesAreaValueatDesignAirFlowRate();
      result = true;
    }
    return result;
  }

  void EvaporativeFluidCoolerSingleSpeed_Impl::resetUfactorTimesAreaValueatDesignAirFlowRate() {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::UfactorTimesAreaValueatDesignAirFlowRate, "");
    BOOST_ASSERT(result);
  }

  void EvaporativeFluidCoolerSingleSpeed_Impl::autosizeUfactorTimesAreaValueatDesignAirFlowRate() {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::UfactorTimesAreaValueatDesignAirFlowRate, "autosize");
    BOOST_ASSERT(result);
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::setDesignWaterFlowRate(boost::optional<double> designWaterFlowRate) {
    bool result(false);
    if (designWaterFlowRate) {
      result = setDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignWaterFlowRate, designWaterFlowRate.get());
    }
    else {
      resetDesignWaterFlowRate();
      result = true;
    }
    return result;
  }

  void EvaporativeFluidCoolerSingleSpeed_Impl::resetDesignWaterFlowRate() {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignWaterFlowRate, "");
    BOOST_ASSERT(result);
  }

  void EvaporativeFluidCoolerSingleSpeed_Impl::autosizeDesignWaterFlowRate() {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignWaterFlowRate, "autosize");
    BOOST_ASSERT(result);
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::setUserSpecifiedDesignCapacity(boost::optional<double> userSpecifiedDesignCapacity) {
    bool result(false);
    if (userSpecifiedDesignCapacity) {
      result = setDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::UserSpecifiedDesignCapacity, userSpecifiedDesignCapacity.get());
    }
    else {
      resetUserSpecifiedDesignCapacity();
      result = true;
    }
    return result;
  }

  void EvaporativeFluidCoolerSingleSpeed_Impl::resetUserSpecifiedDesignCapacity() {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::UserSpecifiedDesignCapacity, "");
    BOOST_ASSERT(result);
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::setDesignEnteringWaterTemperature(boost::optional<double> designEnteringWaterTemperature) {
    bool result(false);
    if (designEnteringWaterTemperature) {
      result = setDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignEnteringWaterTemperature, designEnteringWaterTemperature.get());
    }
    else {
      resetDesignEnteringWaterTemperature();
      result = true;
    }
    return result;
  }

  void EvaporativeFluidCoolerSingleSpeed_Impl::resetDesignEnteringWaterTemperature() {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignEnteringWaterTemperature, "");
    BOOST_ASSERT(result);
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::setDesignEnteringAirTemperature(boost::optional<double> designEnteringAirTemperature) {
    bool result(false);
    if (designEnteringAirTemperature) {
      result = setDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignEnteringAirTemperature, designEnteringAirTemperature.get());
    }
    else {
      resetDesignEnteringAirTemperature();
      result = true;
    }
    return result;
  }

  void EvaporativeFluidCoolerSingleSpeed_Impl::resetDesignEnteringAirTemperature() {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignEnteringAirTemperature, "");
    BOOST_ASSERT(result);
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::setDesignEnteringAirWetbulbTemperature(boost::optional<double> designEnteringAirWetbulbTemperature) {
    bool result(false);
    if (designEnteringAirWetbulbTemperature) {
      result = setDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignEnteringAirWetbulbTemperature, designEnteringAirWetbulbTemperature.get());
    }
    else {
      resetDesignEnteringAirWetbulbTemperature();
      result = true;
    }
    return result;
  }

  void EvaporativeFluidCoolerSingleSpeed_Impl::resetDesignEnteringAirWetbulbTemperature() {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignEnteringAirWetbulbTemperature, "");
    BOOST_ASSERT(result);
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::setCapacityControl(std::string capacityControl) {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::CapacityControl, capacityControl);
    return result;
  }

  void EvaporativeFluidCoolerSingleSpeed_Impl::resetCapacityControl() {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::CapacityControl, "");
    BOOST_ASSERT(result);
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::setSizingFactor(double sizingFactor) {
    bool result = setDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::SizingFactor, sizingFactor);
    return result;
  }

  void EvaporativeFluidCoolerSingleSpeed_Impl::resetSizingFactor() {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::SizingFactor, "");
    BOOST_ASSERT(result);
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::setEvaporationLossMode(std::string evaporationLossMode) {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::EvaporationLossMode, evaporationLossMode);
    return result;
  }

  void EvaporativeFluidCoolerSingleSpeed_Impl::resetEvaporationLossMode() {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::EvaporationLossMode, "");
    BOOST_ASSERT(result);
  }

  void EvaporativeFluidCoolerSingleSpeed_Impl::setEvaporationLossFactor(boost::optional<double> evaporationLossFactor) {
    bool result(false);
    if (evaporationLossFactor) {
      result = setDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::EvaporationLossFactor, evaporationLossFactor.get());
    }
    else {
      resetEvaporationLossFactor();
      result = true;
    }
    BOOST_ASSERT(result);
  }

  void EvaporativeFluidCoolerSingleSpeed_Impl::resetEvaporationLossFactor() {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::EvaporationLossFactor, "");
    BOOST_ASSERT(result);
  }

  void EvaporativeFluidCoolerSingleSpeed_Impl::setDriftLossPercent(double driftLossPercent) {
    bool result = setDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::DriftLossPercent, driftLossPercent);
    BOOST_ASSERT(result);
  }

  void EvaporativeFluidCoolerSingleSpeed_Impl::resetDriftLossPercent() {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::DriftLossPercent, "");
    BOOST_ASSERT(result);
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::setBlowdownCalculationMode(std::string blowdownCalculationMode) {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::BlowdownCalculationMode, blowdownCalculationMode);
    return result;
  }

  void EvaporativeFluidCoolerSingleSpeed_Impl::resetBlowdownCalculationMode() {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::BlowdownCalculationMode, "");
    BOOST_ASSERT(result);
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::setBlowdownConcentrationRatio(double blowdownConcentrationRatio) {
    bool result = setDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::BlowdownConcentrationRatio, blowdownConcentrationRatio);
    return result;
  }

  void EvaporativeFluidCoolerSingleSpeed_Impl::resetBlowdownConcentrationRatio() {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::BlowdownConcentrationRatio, "");
    BOOST_ASSERT(result);
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::setBlowdownMakeupWaterUsageSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_EvaporativeFluidCooler_SingleSpeedFields::BlowdownMakeupWaterUsageScheduleName,
                              "EvaporativeFluidCoolerSingleSpeed",
                              "Blowdown Makeup Water Usage",
                              schedule);
    return result;
  }

  void EvaporativeFluidCoolerSingleSpeed_Impl::resetBlowdownMakeupWaterUsageSchedule() {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::BlowdownMakeupWaterUsageScheduleName, "");
    BOOST_ASSERT(result);
  }

  std::vector<std::string> EvaporativeFluidCoolerSingleSpeed_Impl::performanceInputMethodValues() const {
    return EvaporativeFluidCoolerSingleSpeed::performanceInputMethodValues();
  }

  std::vector<std::string> EvaporativeFluidCoolerSingleSpeed_Impl::capacityControlValues() const {
    return EvaporativeFluidCoolerSingleSpeed::capacityControlValues();
  }

  std::vector<std::string> EvaporativeFluidCoolerSingleSpeed_Impl::evaporationLossModeValues() const {
    return EvaporativeFluidCoolerSingleSpeed::evaporationLossModeValues();
  }

  std::vector<std::string> EvaporativeFluidCoolerSingleSpeed_Impl::blowdownCalculationModeValues() const {
    return EvaporativeFluidCoolerSingleSpeed::blowdownCalculationModeValues();
  }

  boost::optional<ModelObject> EvaporativeFluidCoolerSingleSpeed_Impl::blowdownMakeupWaterUsageScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = blowdownMakeupWaterUsageSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::setBlowdownMakeupWaterUsageScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setBlowdownMakeupWaterUsageSchedule(schedule);
      }
      else {
        return false;
      }
    }
    else {
      resetBlowdownMakeupWaterUsageSchedule();
    }
    return true;
  }

} // detail

EvaporativeFluidCoolerSingleSpeed::EvaporativeFluidCoolerSingleSpeed(const Model& model)
  : StraightComponent(EvaporativeFluidCoolerSingleSpeed::iddObjectType(),model)
{
  BOOST_ASSERT(getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>());
 
  autosizeDesignAirFlowRate();
  autosizeFanPoweratDesignAirFlowRate();
  setDesignSprayWaterFlowRate(0.03);
  setPerformanceInputMethod("UFactorTimesAreaAndDesignWaterFlowRate");
  resetStandardDesignCapacity();
  autosizeUfactorTimesAreaValueatDesignAirFlowRate();
  autosizeDesignWaterFlowRate();
  resetUserSpecifiedDesignCapacity();
  resetDesignEnteringWaterTemperature();
  resetDesignEnteringAirTemperature();
  resetDesignEnteringAirWetbulbTemperature();
  setCapacityControl("FanCycling");
  setSizingFactor(1.0);
  setEvaporationLossMode("SaturatedExit");
  setDriftLossPercent(0.008);
  setBlowdownCalculationMode("ConcentrationRatio");
  setBlowdownConcentrationRatio(3.0);
  resetBlowdownMakeupWaterUsageSchedule();
  setString(OS_EvaporativeFluidCooler_SingleSpeedFields::SupplyWaterStorageTankName,"");
}

IddObjectType EvaporativeFluidCoolerSingleSpeed::iddObjectType() {
  return IddObjectType(IddObjectType::OS_EvaporativeFluidCooler_SingleSpeed);
}

std::vector<std::string> EvaporativeFluidCoolerSingleSpeed::performanceInputMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_EvaporativeFluidCooler_SingleSpeedFields::PerformanceInputMethod);
}

std::vector<std::string> EvaporativeFluidCoolerSingleSpeed::capacityControlValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_EvaporativeFluidCooler_SingleSpeedFields::CapacityControl);
}

std::vector<std::string> EvaporativeFluidCoolerSingleSpeed::evaporationLossModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_EvaporativeFluidCooler_SingleSpeedFields::EvaporationLossMode);
}

std::vector<std::string> EvaporativeFluidCoolerSingleSpeed::blowdownCalculationModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_EvaporativeFluidCooler_SingleSpeedFields::BlowdownCalculationMode);
}

boost::optional<double> EvaporativeFluidCoolerSingleSpeed::designAirFlowRate() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->designAirFlowRate();
}

bool EvaporativeFluidCoolerSingleSpeed::isDesignAirFlowRateAutosized() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->isDesignAirFlowRateAutosized();
}

boost::optional<double> EvaporativeFluidCoolerSingleSpeed::fanPoweratDesignAirFlowRate() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->fanPoweratDesignAirFlowRate();
}

bool EvaporativeFluidCoolerSingleSpeed::isFanPoweratDesignAirFlowRateAutosized() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->isFanPoweratDesignAirFlowRateAutosized();
}

double EvaporativeFluidCoolerSingleSpeed::designSprayWaterFlowRate() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->designSprayWaterFlowRate();
}

boost::optional<std::string> EvaporativeFluidCoolerSingleSpeed::performanceInputMethod() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->performanceInputMethod();
}

boost::optional<double> EvaporativeFluidCoolerSingleSpeed::standardDesignCapacity() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->standardDesignCapacity();
}

boost::optional<double> EvaporativeFluidCoolerSingleSpeed::ufactorTimesAreaValueatDesignAirFlowRate() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->ufactorTimesAreaValueatDesignAirFlowRate();
}

bool EvaporativeFluidCoolerSingleSpeed::isUfactorTimesAreaValueatDesignAirFlowRateAutosized() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->isUfactorTimesAreaValueatDesignAirFlowRateAutosized();
}

boost::optional<double> EvaporativeFluidCoolerSingleSpeed::designWaterFlowRate() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->designWaterFlowRate();
}

bool EvaporativeFluidCoolerSingleSpeed::isDesignWaterFlowRateAutosized() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->isDesignWaterFlowRateAutosized();
}

boost::optional<double> EvaporativeFluidCoolerSingleSpeed::userSpecifiedDesignCapacity() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->userSpecifiedDesignCapacity();
}

boost::optional<double> EvaporativeFluidCoolerSingleSpeed::designEnteringWaterTemperature() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->designEnteringWaterTemperature();
}

boost::optional<double> EvaporativeFluidCoolerSingleSpeed::designEnteringAirTemperature() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->designEnteringAirTemperature();
}

boost::optional<double> EvaporativeFluidCoolerSingleSpeed::designEnteringAirWetbulbTemperature() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->designEnteringAirWetbulbTemperature();
}

std::string EvaporativeFluidCoolerSingleSpeed::capacityControl() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->capacityControl();
}

bool EvaporativeFluidCoolerSingleSpeed::isCapacityControlDefaulted() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->isCapacityControlDefaulted();
}

double EvaporativeFluidCoolerSingleSpeed::sizingFactor() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->sizingFactor();
}

bool EvaporativeFluidCoolerSingleSpeed::isSizingFactorDefaulted() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->isSizingFactorDefaulted();
}

std::string EvaporativeFluidCoolerSingleSpeed::evaporationLossMode() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->evaporationLossMode();
}

bool EvaporativeFluidCoolerSingleSpeed::isEvaporationLossModeDefaulted() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->isEvaporationLossModeDefaulted();
}

boost::optional<double> EvaporativeFluidCoolerSingleSpeed::evaporationLossFactor() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->evaporationLossFactor();
}

double EvaporativeFluidCoolerSingleSpeed::driftLossPercent() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->driftLossPercent();
}

bool EvaporativeFluidCoolerSingleSpeed::isDriftLossPercentDefaulted() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->isDriftLossPercentDefaulted();
}

std::string EvaporativeFluidCoolerSingleSpeed::blowdownCalculationMode() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->blowdownCalculationMode();
}

bool EvaporativeFluidCoolerSingleSpeed::isBlowdownCalculationModeDefaulted() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->isBlowdownCalculationModeDefaulted();
}

double EvaporativeFluidCoolerSingleSpeed::blowdownConcentrationRatio() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->blowdownConcentrationRatio();
}

bool EvaporativeFluidCoolerSingleSpeed::isBlowdownConcentrationRatioDefaulted() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->isBlowdownConcentrationRatioDefaulted();
}

boost::optional<Schedule> EvaporativeFluidCoolerSingleSpeed::blowdownMakeupWaterUsageSchedule() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->blowdownMakeupWaterUsageSchedule();
}

bool EvaporativeFluidCoolerSingleSpeed::setDesignAirFlowRate(double designAirFlowRate) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setDesignAirFlowRate(designAirFlowRate);
}

void EvaporativeFluidCoolerSingleSpeed::autosizeDesignAirFlowRate() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->autosizeDesignAirFlowRate();
}

bool EvaporativeFluidCoolerSingleSpeed::setFanPoweratDesignAirFlowRate(double fanPoweratDesignAirFlowRate) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setFanPoweratDesignAirFlowRate(fanPoweratDesignAirFlowRate);
}

void EvaporativeFluidCoolerSingleSpeed::autosizeFanPoweratDesignAirFlowRate() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->autosizeFanPoweratDesignAirFlowRate();
}

bool EvaporativeFluidCoolerSingleSpeed::setDesignSprayWaterFlowRate(double designSprayWaterFlowRate) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setDesignSprayWaterFlowRate(designSprayWaterFlowRate);
}

bool EvaporativeFluidCoolerSingleSpeed::setPerformanceInputMethod(std::string performanceInputMethod) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setPerformanceInputMethod(performanceInputMethod);
}

void EvaporativeFluidCoolerSingleSpeed::resetPerformanceInputMethod() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->resetPerformanceInputMethod();
}

bool EvaporativeFluidCoolerSingleSpeed::setStandardDesignCapacity(double standardDesignCapacity) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setStandardDesignCapacity(standardDesignCapacity);
}

void EvaporativeFluidCoolerSingleSpeed::resetStandardDesignCapacity() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->resetStandardDesignCapacity();
}

bool EvaporativeFluidCoolerSingleSpeed::setUfactorTimesAreaValueatDesignAirFlowRate(double ufactorTimesAreaValueatDesignAirFlowRate) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setUfactorTimesAreaValueatDesignAirFlowRate(ufactorTimesAreaValueatDesignAirFlowRate);
}

void EvaporativeFluidCoolerSingleSpeed::resetUfactorTimesAreaValueatDesignAirFlowRate() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->resetUfactorTimesAreaValueatDesignAirFlowRate();
}

void EvaporativeFluidCoolerSingleSpeed::autosizeUfactorTimesAreaValueatDesignAirFlowRate() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->autosizeUfactorTimesAreaValueatDesignAirFlowRate();
}

bool EvaporativeFluidCoolerSingleSpeed::setDesignWaterFlowRate(double designWaterFlowRate) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setDesignWaterFlowRate(designWaterFlowRate);
}

void EvaporativeFluidCoolerSingleSpeed::resetDesignWaterFlowRate() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->resetDesignWaterFlowRate();
}

void EvaporativeFluidCoolerSingleSpeed::autosizeDesignWaterFlowRate() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->autosizeDesignWaterFlowRate();
}

bool EvaporativeFluidCoolerSingleSpeed::setUserSpecifiedDesignCapacity(double userSpecifiedDesignCapacity) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setUserSpecifiedDesignCapacity(userSpecifiedDesignCapacity);
}

void EvaporativeFluidCoolerSingleSpeed::resetUserSpecifiedDesignCapacity() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->resetUserSpecifiedDesignCapacity();
}

bool EvaporativeFluidCoolerSingleSpeed::setDesignEnteringWaterTemperature(double designEnteringWaterTemperature) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setDesignEnteringWaterTemperature(designEnteringWaterTemperature);
}

void EvaporativeFluidCoolerSingleSpeed::resetDesignEnteringWaterTemperature() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->resetDesignEnteringWaterTemperature();
}

bool EvaporativeFluidCoolerSingleSpeed::setDesignEnteringAirTemperature(double designEnteringAirTemperature) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setDesignEnteringAirTemperature(designEnteringAirTemperature);
}

void EvaporativeFluidCoolerSingleSpeed::resetDesignEnteringAirTemperature() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->resetDesignEnteringAirTemperature();
}

bool EvaporativeFluidCoolerSingleSpeed::setDesignEnteringAirWetbulbTemperature(double designEnteringAirWetbulbTemperature) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setDesignEnteringAirWetbulbTemperature(designEnteringAirWetbulbTemperature);
}

void EvaporativeFluidCoolerSingleSpeed::resetDesignEnteringAirWetbulbTemperature() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->resetDesignEnteringAirWetbulbTemperature();
}

bool EvaporativeFluidCoolerSingleSpeed::setCapacityControl(std::string capacityControl) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setCapacityControl(capacityControl);
}

void EvaporativeFluidCoolerSingleSpeed::resetCapacityControl() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->resetCapacityControl();
}

bool EvaporativeFluidCoolerSingleSpeed::setSizingFactor(double sizingFactor) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setSizingFactor(sizingFactor);
}

void EvaporativeFluidCoolerSingleSpeed::resetSizingFactor() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->resetSizingFactor();
}

bool EvaporativeFluidCoolerSingleSpeed::setEvaporationLossMode(std::string evaporationLossMode) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setEvaporationLossMode(evaporationLossMode);
}

void EvaporativeFluidCoolerSingleSpeed::resetEvaporationLossMode() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->resetEvaporationLossMode();
}

void EvaporativeFluidCoolerSingleSpeed::setEvaporationLossFactor(double evaporationLossFactor) {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setEvaporationLossFactor(evaporationLossFactor);
}

void EvaporativeFluidCoolerSingleSpeed::resetEvaporationLossFactor() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->resetEvaporationLossFactor();
}

void EvaporativeFluidCoolerSingleSpeed::setDriftLossPercent(double driftLossPercent) {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setDriftLossPercent(driftLossPercent);
}

void EvaporativeFluidCoolerSingleSpeed::resetDriftLossPercent() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->resetDriftLossPercent();
}

bool EvaporativeFluidCoolerSingleSpeed::setBlowdownCalculationMode(std::string blowdownCalculationMode) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setBlowdownCalculationMode(blowdownCalculationMode);
}

void EvaporativeFluidCoolerSingleSpeed::resetBlowdownCalculationMode() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->resetBlowdownCalculationMode();
}

bool EvaporativeFluidCoolerSingleSpeed::setBlowdownConcentrationRatio(double blowdownConcentrationRatio) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setBlowdownConcentrationRatio(blowdownConcentrationRatio);
}

void EvaporativeFluidCoolerSingleSpeed::resetBlowdownConcentrationRatio() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->resetBlowdownConcentrationRatio();
}

bool EvaporativeFluidCoolerSingleSpeed::setBlowdownMakeupWaterUsageSchedule(Schedule& schedule) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setBlowdownMakeupWaterUsageSchedule(schedule);
}

void EvaporativeFluidCoolerSingleSpeed::resetBlowdownMakeupWaterUsageSchedule() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->resetBlowdownMakeupWaterUsageSchedule();
}

/// @cond
EvaporativeFluidCoolerSingleSpeed::EvaporativeFluidCoolerSingleSpeed(boost::shared_ptr<detail::EvaporativeFluidCoolerSingleSpeed_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

