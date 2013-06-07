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

// TODO: Check the following class names against object getters and setters.
//#include <model/Connection.hpp>
//#include <model/Connection_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
//#include <model/WaterStorageTank.hpp>
//#include <model/WaterStorageTank_Impl.hpp>
#include <model/ScheduleTypeLimits.hpp>
#include <model/ScheduleTypeRegistry.hpp>

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_EvaporativeFluidCooler_SingleSpeed_FieldEnums.hxx>

#include <utilities/units/Unit.hpp>

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
    // TODO: Check schedule display names.
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

  //Connection EvaporativeFluidCoolerSingleSpeed_Impl::waterInletNode() const {
  //  boost::optional<Connection> value = optionalWaterInletNode();
  //  if (!value) {
  //    LOG_AND_THROW(briefDescription() << " does not have an Water Inlet Node attached.");
  //  }
  //  return value.get();
  //}

  //Connection EvaporativeFluidCoolerSingleSpeed_Impl::waterOutletNode() const {
  //  boost::optional<Connection> value = optionalWaterOutletNode();
  //  if (!value) {
  //    LOG_AND_THROW(briefDescription() << " does not have an Water Outlet Node attached.");
  //  }
  //  return value.get();
 // }

  boost::optional<double> EvaporativeFluidCoolerSingleSpeed_Impl::designAirFlowRate() const {
    return getDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignAirFlowRate,true);
  }

 /* OSOptionalQuantity EvaporativeFluidCoolerSingleSpeed_Impl::getDesignAirFlowRate(bool returnIP) const {
    OptionalDouble value = designAirFlowRate();
    return getQuantityFromDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignAirFlowRate, value, returnIP);
  }*/

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

 /* OSOptionalQuantity EvaporativeFluidCoolerSingleSpeed_Impl::getFanPoweratDesignAirFlowRate(bool returnIP) const {
    OptionalDouble value = fanPoweratDesignAirFlowRate();
    return getQuantityFromDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::FanPoweratDesignAirFlowRate, value, returnIP);
  }*/

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

 /* Quantity EvaporativeFluidCoolerSingleSpeed_Impl::getDesignSprayWaterFlowRate(bool returnIP) const {
    OptionalDouble value = designSprayWaterFlowRate();
    OSOptionalQuantity result = getQuantityFromDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignSprayWaterFlowRate, value, returnIP);
    BOOST_ASSERT(result.isSet());
    return result.get();
  }*/

  boost::optional<std::string> EvaporativeFluidCoolerSingleSpeed_Impl::performanceInputMethod() const {
    return getString(OS_EvaporativeFluidCooler_SingleSpeedFields::PerformanceInputMethod,true);
  }

  //boost::optional<Connection> EvaporativeFluidCoolerSingleSpeed_Impl::outdoorAirInletNode() const {
  //  return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_EvaporativeFluidCooler_SingleSpeedFields::OutdoorAirInletNodeName);
  //}

  boost::optional<double> EvaporativeFluidCoolerSingleSpeed_Impl::standardDesignCapacity() const {
    return getDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::StandardDesignCapacity,true);
  }

  /*OSOptionalQuantity EvaporativeFluidCoolerSingleSpeed_Impl::getStandardDesignCapacity(bool returnIP) const {
    OptionalDouble value = standardDesignCapacity();
    return getQuantityFromDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::StandardDesignCapacity, value, returnIP);
  }*/

  boost::optional<double> EvaporativeFluidCoolerSingleSpeed_Impl::ufactorTimesAreaValueatDesignAirFlowRate() const {
    return getDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::UfactorTimesAreaValueatDesignAirFlowRate,true);
  }

  /*OSOptionalQuantity EvaporativeFluidCoolerSingleSpeed_Impl::getUfactorTimesAreaValueatDesignAirFlowRate(bool returnIP) const {
    OptionalDouble value = ufactorTimesAreaValueatDesignAirFlowRate();
    return getQuantityFromDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::UfactorTimesAreaValueatDesignAirFlowRate, value, returnIP);
  }*/

  bool EvaporativeFluidCoolerSingleSpeed_Impl::isUfactorTimesAreaValueatDesignAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_EvaporativeFluidCooler_SingleSpeedFields::UfactorTimesAreaValueatDesignAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> EvaporativeFluidCoolerSingleSpeed_Impl::designWaterFlowRate() const {
    return getDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::designWaterFlowRate,true);
  }
  
  /*OSOptionalQuantity EvaporativeFluidCoolerSingleSpeed_Impl::getDesignWaterFlowRate(bool returnIP) const {
    OptionalDouble value = designWaterFlowRate();
    return getQuantityFromDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignWaterFlowRate, value, returnIP);
  }*/

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

  /*OSOptionalQuantity EvaporativeFluidCoolerSingleSpeed_Impl::getUserSpecifiedDesignCapacity(bool returnIP) const {
    OptionalDouble value = userSpecifiedDesignCapacity();
    return getQuantityFromDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::UserSpecifiedDesignCapacity, value, returnIP);
  }*/

  boost::optional<double> EvaporativeFluidCoolerSingleSpeed_Impl::designEnteringWaterTemperature() const {
    return getDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignEnteringWaterTemperature,true);
  }

  /*OSOptionalQuantity EvaporativeFluidCoolerSingleSpeed_Impl::getDesignEnteringWaterTemperature(bool returnIP) const {
    OptionalDouble value = designEnteringWaterTemperature();
    return getQuantityFromDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignEnteringWaterTemperature, value, returnIP);
  }*/

  boost::optional<double> EvaporativeFluidCoolerSingleSpeed_Impl::designEnteringAirTemperature() const {
    return getDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignEnteringAirTemperature,true);
  }

  /*OSOptionalQuantity EvaporativeFluidCoolerSingleSpeed_Impl::getDesignEnteringAirTemperature(bool returnIP) const {
    OptionalDouble value = designEnteringAirTemperature();
    return getQuantityFromDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignEnteringAirTemperature, value, returnIP);
  }*/

  boost::optional<double> EvaporativeFluidCoolerSingleSpeed_Impl::designEnteringAirWetbulbTemperature() const {
    return getDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignEnteringAirWetbulbTemperature,true);
  }

 /* OSOptionalQuantity EvaporativeFluidCoolerSingleSpeed_Impl::getDesignEnteringAirWetbulbTemperature(bool returnIP) const {
    OptionalDouble value = designEnteringAirWetbulbTemperature();
    return getQuantityFromDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignEnteringAirWetbulbTemperature, value, returnIP);
  }*/

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

  /*Quantity EvaporativeFluidCoolerSingleSpeed_Impl::getSizingFactor(bool returnIP) const {
    OptionalDouble value = sizingFactor();
    OSOptionalQuantity result = getQuantityFromDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::SizingFactor, value, returnIP);
    BOOST_ASSERT(result.isSet());
    return result.get();
  }*/

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

 /* OSOptionalQuantity EvaporativeFluidCoolerSingleSpeed_Impl::getEvaporationLossFactor(bool returnIP) const {
    OptionalDouble value = evaporationLossFactor();
    return getQuantityFromDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::EvaporationLossFactor, value, returnIP);
  }*/

  double EvaporativeFluidCoolerSingleSpeed_Impl::driftLossPercent() const {
    boost::optional<double> value = getDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::DriftLossPercent,true);
    BOOST_ASSERT(value);
    return value.get();
  }

 /* Quantity EvaporativeFluidCoolerSingleSpeed_Impl::getDriftLossPercent(bool returnIP) const {
    OptionalDouble value = driftLossPercent();
    OSOptionalQuantity result = getQuantityFromDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::DriftLossPercent, value, returnIP);
    BOOST_ASSERT(result.isSet());
    return result.get();
  }*/

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

  /*Quantity EvaporativeFluidCoolerSingleSpeed_Impl::getBlowdownConcentrationRatio(bool returnIP) const {
    OptionalDouble value = blowdownConcentrationRatio();
    OSOptionalQuantity result = getQuantityFromDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::BlowdownConcentrationRatio, value, returnIP);
    BOOST_ASSERT(result.isSet());
    return result.get();
  }*/

  bool EvaporativeFluidCoolerSingleSpeed_Impl::isBlowdownConcentrationRatioDefaulted() const {
    return isEmpty(OS_EvaporativeFluidCooler_SingleSpeedFields::BlowdownConcentrationRatio);
  }

  boost::optional<Schedule> EvaporativeFluidCoolerSingleSpeed_Impl::blowdownMakeupWaterUsageSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_EvaporativeFluidCooler_SingleSpeedFields::BlowdownMakeupWaterUsageScheduleName);
  }

//  boost::optional<WaterStorageTank> EvaporativeFluidCoolerSingleSpeed_Impl::supplyWaterStorageTank() const {
//    return getObject<ModelObject>().getModelObjectTarget<WaterStorageTank>(OS_EvaporativeFluidCooler_SingleSpeedFields::SupplyWaterStorageTankName);
//  }

  //bool EvaporativeFluidCoolerSingleSpeed_Impl::setWaterInletNode(const Connection& connection) {
  //  bool result = setPointer(OS_EvaporativeFluidCooler_SingleSpeedFields::WaterInletNodeName, connection.handle());
  //  return result;
  //}

  //bool EvaporativeFluidCoolerSingleSpeed_Impl::setWaterOutletNode(const Connection& connection) {
  //  bool result = setPointer(OS_EvaporativeFluidCooler_SingleSpeedFields::WaterOutletNodeName, connection.handle());
  //  return result;
  //}

  bool EvaporativeFluidCoolerSingleSpeed_Impl::setDesignAirFlowRate(boost::optional<double> designAirFlowRate) {
    bool result(false);
    if (designAirFlowRate) {
      result = setDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignAirFlowRate, designAirFlowRate.get());
    }
    return result;
  }

  /*bool EvaporativeFluidCoolerSingleSpeed_Impl::setDesignAirFlowRate(const OSOptionalQuantity& designAirFlowRate) {
    bool result(false);
    OptionalDouble value;
    if (designAirFlowRate.isSet()) {
      value = getDoubleFromQuantity(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignAirFlowRate,designAirFlowRate.get());
      if (value) {
        result = setDesignAirFlowRate(value);
      }
    }
    else {
      result = setDesignAirFlowRate(value);
    }
    return result;
  }*/

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

  /*bool EvaporativeFluidCoolerSingleSpeed_Impl::setFanPoweratDesignAirFlowRate(const OSOptionalQuantity& fanPoweratDesignAirFlowRate) {
    bool result(false);
    OptionalDouble value;
    if (fanPoweratDesignAirFlowRate.isSet()) {
      value = getDoubleFromQuantity(OS_EvaporativeFluidCooler_SingleSpeedFields::FanPoweratDesignAirFlowRate,fanPoweratDesignAirFlowRate.get());
      if (value) {
        result = setFanPoweratDesignAirFlowRate(value);
      }
    }
    else {
      result = setFanPoweratDesignAirFlowRate(value);
    }
    return result;
  }*/

  void EvaporativeFluidCoolerSingleSpeed_Impl::autosizeFanPoweratDesignAirFlowRate() {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::FanPoweratDesignAirFlowRate, "autosize");
    BOOST_ASSERT(result);
  }

  bool EvaporativeFluidCoolerSingleSpeed_Impl::setDesignSprayWaterFlowRate(double designSprayWaterFlowRate) {
    bool result = setDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignSprayWaterFlowRate, designSprayWaterFlowRate);
    return result;
  }

  /*bool EvaporativeFluidCoolerSingleSpeed_Impl::setDesignSprayWaterFlowRate(const Quantity& designSprayWaterFlowRate) {
    OptionalDouble value = getDoubleFromQuantity(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignSprayWaterFlowRate,designSprayWaterFlowRate);
    if (!value) {
      return false;
    }
    return setDesignSprayWaterFlowRate(value.get());
  }*/

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

  //bool EvaporativeFluidCoolerSingleSpeed_Impl::setOutdoorAirInletNode(const boost::optional<Connection>& connection) {
  //  bool result(false);
  //  if (connection) {
  //    result = setPointer(OS_EvaporativeFluidCooler_SingleSpeedFields::OutdoorAirInletNodeName, connection.get().handle());
  //  }
  //  else {
  //    resetOutdoorAirInletNode();
  //   result = true;
  //  }
  //  return result;
  //}

  void EvaporativeFluidCoolerSingleSpeed_Impl::resetOutdoorAirInletNode() {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::OutdoorAirInletNodeName, "");
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

  /*bool EvaporativeFluidCoolerSingleSpeed_Impl::setStandardDesignCapacity(const OSOptionalQuantity& standardDesignCapacity) {
    bool result(false);
    OptionalDouble value;
    if (standardDesignCapacity.isSet()) {
      value = getDoubleFromQuantity(OS_EvaporativeFluidCooler_SingleSpeedFields::StandardDesignCapacity,standardDesignCapacity.get());
      if (value) {
        result = setStandardDesignCapacity(value);
      }
    }
    else {
      result = setStandardDesignCapacity(value);
    }
    return result;
  }*/

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

  /*bool EvaporativeFluidCoolerSingleSpeed_Impl::setUfactorTimesAreaValueatDesignAirFlowRate(const OSOptionalQuantity& ufactorTimesAreaValueatDesignAirFlowRate) {
    bool result(false);
    OptionalDouble value;
    if (ufactorTimesAreaValueatDesignAirFlowRate.isSet()) {
      value = getDoubleFromQuantity(OS_EvaporativeFluidCooler_SingleSpeedFields::UfactorTimesAreaValueatDesignAirFlowRate,ufactorTimesAreaValueatDesignAirFlowRate.get());
      if (value) {
        result = setUfactorTimesAreaValueatDesignAirFlowRate(value);
      }
    }
    else {
      result = setUfactorTimesAreaValueatDesignAirFlowRate(value);
    }
    return result;
  }*/

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

  /*bool EvaporativeFluidCoolerSingleSpeed_Impl::setDesignWaterFlowRate(const OSOptionalQuantity& designWaterFlowRate) {
    bool result(false);
    OptionalDouble value;
    if (designWaterFlowRate.isSet()) {
      value = getDoubleFromQuantity(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignWaterFlowRate,designWaterFlowRate.get());
      if (value) {
        result = setDesignWaterFlowRate(value);
      }
    }
    else {
      result = setDesignWaterFlowRate(value);
    }
    return result;
  }*/

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

  /*bool EvaporativeFluidCoolerSingleSpeed_Impl::setUserSpecifiedDesignCapacity(const OSOptionalQuantity& userSpecifiedDesignCapacity) {
    bool result(false);
    OptionalDouble value;
    if (userSpecifiedDesignCapacity.isSet()) {
      value = getDoubleFromQuantity(OS_EvaporativeFluidCooler_SingleSpeedFields::UserSpecifiedDesignCapacity,userSpecifiedDesignCapacity.get());
      if (value) {
        result = setUserSpecifiedDesignCapacity(value);
      }
    }
    else {
      result = setUserSpecifiedDesignCapacity(value);
    }
    return result;
  }*/

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

  /*bool EvaporativeFluidCoolerSingleSpeed_Impl::setDesignEnteringWaterTemperature(const OSOptionalQuantity& designEnteringWaterTemperature) {
    bool result(false);
    OptionalDouble value;
    if (designEnteringWaterTemperature.isSet()) {
      value = getDoubleFromQuantity(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignEnteringWaterTemperature,designEnteringWaterTemperature.get());
      if (value) {
        result = setDesignEnteringWaterTemperature(value);
      }
    }
    else {
      result = setDesignEnteringWaterTemperature(value);
    }
    return result;
  }*/

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

  /*bool EvaporativeFluidCoolerSingleSpeed_Impl::setDesignEnteringAirTemperature(const OSOptionalQuantity& designEnteringAirTemperature) {
    bool result(false);
    OptionalDouble value;
    if (designEnteringAirTemperature.isSet()) {
      value = getDoubleFromQuantity(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignEnteringAirTemperature,designEnteringAirTemperature.get());
      if (value) {
        result = setDesignEnteringAirTemperature(value);
      }
    }
    else {
      result = setDesignEnteringAirTemperature(value);
    }
    return result;
  }*/

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

  /*bool EvaporativeFluidCoolerSingleSpeed_Impl::setDesignEnteringAirWetbulbTemperature(const OSOptionalQuantity& designEnteringAirWetbulbTemperature) {
    bool result(false);
    OptionalDouble value;
    if (designEnteringAirWetbulbTemperature.isSet()) {
      value = getDoubleFromQuantity(OS_EvaporativeFluidCooler_SingleSpeedFields::DesignEnteringAirWetbulbTemperature,designEnteringAirWetbulbTemperature.get());
      if (value) {
        result = setDesignEnteringAirWetbulbTemperature(value);
      }
    }
    else {
      result = setDesignEnteringAirWetbulbTemperature(value);
    }
    return result;
  }*/

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

  /*bool EvaporativeFluidCoolerSingleSpeed_Impl::setSizingFactor(const Quantity& sizingFactor) {
    OptionalDouble value = getDoubleFromQuantity(OS_EvaporativeFluidCooler_SingleSpeedFields::SizingFactor,sizingFactor);
    if (!value) {
      return false;
    }
    return setSizingFactor(value.get());
  }*/

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

 /* bool EvaporativeFluidCoolerSingleSpeed_Impl::setEvaporationLossFactor(const OSOptionalQuantity& evaporationLossFactor) {
    bool result(false);
    OptionalDouble value;
    if (evaporationLossFactor.isSet()) {
      value = getDoubleFromQuantity(OS_EvaporativeFluidCooler_SingleSpeedFields::EvaporationLossFactor,evaporationLossFactor.get());
      if (value) {
        setEvaporationLossFactor(value);
		result = true;
      }
    }
    else {
      setEvaporationLossFactor(value);
      result = true;
    }
    return result;
  }*/

  void EvaporativeFluidCoolerSingleSpeed_Impl::resetEvaporationLossFactor() {
    bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::EvaporationLossFactor, "");
    BOOST_ASSERT(result);
  }

  void EvaporativeFluidCoolerSingleSpeed_Impl::setDriftLossPercent(double driftLossPercent) {
    bool result = setDouble(OS_EvaporativeFluidCooler_SingleSpeedFields::DriftLossPercent, driftLossPercent);
    BOOST_ASSERT(result);
  }

 /* bool EvaporativeFluidCoolerSingleSpeed_Impl::setDriftLossPercent(const Quantity& driftLossPercent) {
    OptionalDouble value = getDoubleFromQuantity(OS_EvaporativeFluidCooler_SingleSpeedFields::DriftLossPercent,driftLossPercent);
    if (!value) {
      return false;
    }
    setDriftLossPercent(value.get());
    return true;
  }*/

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

 /* bool EvaporativeFluidCoolerSingleSpeed_Impl::setBlowdownConcentrationRatio(const Quantity& blowdownConcentrationRatio) {
    OptionalDouble value = getDoubleFromQuantity(OS_EvaporativeFluidCooler_SingleSpeedFields::BlowdownConcentrationRatio,blowdownConcentrationRatio);
    if (!value) {
      return false;
    }
    return setBlowdownConcentrationRatio(value.get());
  }*/

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

 /* bool EvaporativeFluidCoolerSingleSpeed_Impl::setSupplyWaterStorageTank(const boost::optional<WaterStorageTank>& waterStorageTank) {
    bool result(false);
    if (waterStorageTank) {
      result = setPointer(OS_EvaporativeFluidCooler_SingleSpeedFields::SupplyWaterStorageTankName, waterStorageTank.get().handle());
    }
    else {
      resetSupplyWaterStorageTank();
      result = true;
    }
    return result;
  }*/

  //void EvaporativeFluidCoolerSingleSpeed_Impl::resetSupplyWaterStorageTank() {
  //  bool result = setString(OS_EvaporativeFluidCooler_SingleSpeedFields::SupplyWaterStorageTankName, "");
  //  BOOST_ASSERT(result);
  //}

 // boost::optional<Connection> EvaporativeFluidCoolerSingleSpeed_Impl::optionalWaterInletNode() const {
 //   return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_EvaporativeFluidCooler_SingleSpeedFields::WaterInletNodeName);
 // }

 // boost::optional<Connection> EvaporativeFluidCoolerSingleSpeed_Impl::optionalWaterOutletNode() const {
 //   return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_EvaporativeFluidCooler_SingleSpeedFields::WaterOutletNodeName);
 // }

  /*openstudio::OSOptionalQuantity EvaporativeFluidCoolerSingleSpeed_Impl::designAirFlowRate_SI() const {
    return getDesignAirFlowRate(false);
  }*/
  /*openstudio::OSOptionalQuantity EvaporativeFluidCoolerSingleSpeed_Impl::designAirFlowRate_IP() const {
    return getDesignAirFlowRate(true);
  }*/

  /*openstudio::OSOptionalQuantity EvaporativeFluidCoolerSingleSpeed_Impl::fanPoweratDesignAirFlowRate_SI() const {
    return getFanPoweratDesignAirFlowRate(false);
  }*/

  /*openstudio::OSOptionalQuantity EvaporativeFluidCoolerSingleSpeed_Impl::fanPoweratDesignAirFlowRate_IP() const {
    return getFanPoweratDesignAirFlowRate(true);
  }*/

  /*openstudio::Quantity EvaporativeFluidCoolerSingleSpeed_Impl::designSprayWaterFlowRate_SI() const {
    return getDesignSprayWaterFlowRate(false);
  }*/

  /*openstudio::Quantity EvaporativeFluidCoolerSingleSpeed_Impl::designSprayWaterFlowRate_IP() const {
    return getDesignSprayWaterFlowRate(true);
  }*/

  std::vector<std::string> EvaporativeFluidCoolerSingleSpeed_Impl::performanceInputMethodValues() const {
    return EvaporativeFluidCoolerSingleSpeed::performanceInputMethodValues();
  }

  /*openstudio::OSOptionalQuantity EvaporativeFluidCoolerSingleSpeed_Impl::standardDesignCapacity_SI() const {
    return getStandardDesignCapacity(false);
  }*/

  /*openstudio::OSOptionalQuantity EvaporativeFluidCoolerSingleSpeed_Impl::standardDesignCapacity_IP() const {
    return getStandardDesignCapacity(true);
  }

  openstudio::OSOptionalQuantity EvaporativeFluidCoolerSingleSpeed_Impl::ufactorTimesAreaValueatDesignAirFlowRate_SI() const {
    return getUfactorTimesAreaValueatDesignAirFlowRate(false);
  }

  openstudio::OSOptionalQuantity EvaporativeFluidCoolerSingleSpeed_Impl::ufactorTimesAreaValueatDesignAirFlowRate_IP() const {
    return getUfactorTimesAreaValueatDesignAirFlowRate(true);
  }

  openstudio::OSOptionalQuantity EvaporativeFluidCoolerSingleSpeed_Impl::designWaterFlowRate_SI() const {
    return getDesignWaterFlowRate(false);
  }*/

  /*openstudio::OSOptionalQuantity EvaporativeFluidCoolerSingleSpeed_Impl::designWaterFlowRate_IP() const {
    return getDesignWaterFlowRate(true);
  }

  openstudio::OSOptionalQuantity EvaporativeFluidCoolerSingleSpeed_Impl::userSpecifiedDesignCapacity_SI() const {
    return getUserSpecifiedDesignCapacity(false);
  }

  openstudio::OSOptionalQuantity EvaporativeFluidCoolerSingleSpeed_Impl::userSpecifiedDesignCapacity_IP() const {
    return getUserSpecifiedDesignCapacity(true);
  }

  openstudio::OSOptionalQuantity EvaporativeFluidCoolerSingleSpeed_Impl::designEnteringWaterTemperature_SI() const {
    return getDesignEnteringWaterTemperature(false);
  }

  openstudio::OSOptionalQuantity EvaporativeFluidCoolerSingleSpeed_Impl::designEnteringWaterTemperature_IP() const {
    return getDesignEnteringWaterTemperature(true);
  }

  openstudio::OSOptionalQuantity EvaporativeFluidCoolerSingleSpeed_Impl::designEnteringAirTemperature_SI() const {
    return getDesignEnteringAirTemperature(false);
  }

  openstudio::OSOptionalQuantity EvaporativeFluidCoolerSingleSpeed_Impl::designEnteringAirTemperature_IP() const {
    return getDesignEnteringAirTemperature(true);
  }

  openstudio::OSOptionalQuantity EvaporativeFluidCoolerSingleSpeed_Impl::designEnteringAirWetbulbTemperature_SI() const {
    return getDesignEnteringAirWetbulbTemperature(false);
  }

  openstudio::OSOptionalQuantity EvaporativeFluidCoolerSingleSpeed_Impl::designEnteringAirWetbulbTemperature_IP() const {
    return getDesignEnteringAirWetbulbTemperature(true);
  }*/

  std::vector<std::string> EvaporativeFluidCoolerSingleSpeed_Impl::capacityControlValues() const {
    return EvaporativeFluidCoolerSingleSpeed::capacityControlValues();
  }

 /* openstudio::Quantity EvaporativeFluidCoolerSingleSpeed_Impl::sizingFactor_SI() const {
    return getSizingFactor(false);
  }*/

  /*openstudio::Quantity EvaporativeFluidCoolerSingleSpeed_Impl::sizingFactor_IP() const {
    return getSizingFactor(true);
  }*/

  std::vector<std::string> EvaporativeFluidCoolerSingleSpeed_Impl::evaporationLossModeValues() const {
    return EvaporativeFluidCoolerSingleSpeed::evaporationLossModeValues();
  }

  /*openstudio::OSOptionalQuantity EvaporativeFluidCoolerSingleSpeed_Impl::evaporationLossFactor_SI() const {
    return getEvaporationLossFactor(false);
  }

  openstudio::OSOptionalQuantity EvaporativeFluidCoolerSingleSpeed_Impl::evaporationLossFactor_IP() const {
    return getEvaporationLossFactor(true);
  }

  openstudio::Quantity EvaporativeFluidCoolerSingleSpeed_Impl::driftLossPercent_SI() const {
    return getDriftLossPercent(false);
  }

  openstudio::Quantity EvaporativeFluidCoolerSingleSpeed_Impl::driftLossPercent_IP() const {
    return getDriftLossPercent(true);
  } */

  std::vector<std::string> EvaporativeFluidCoolerSingleSpeed_Impl::blowdownCalculationModeValues() const {
    return EvaporativeFluidCoolerSingleSpeed::blowdownCalculationModeValues();
  }
 /* openstudio::Quantity EvaporativeFluidCoolerSingleSpeed_Impl::blowdownConcentrationRatio_SI() const {
    return getBlowdownConcentrationRatio(false);
  }

  openstudio::Quantity EvaporativeFluidCoolerSingleSpeed_Impl::blowdownConcentrationRatio_IP() const {
    return getBlowdownConcentrationRatio(true);
  }*/

  ///*boost::optional<ModelObject> EvaporativeFluidCoolerSingleSpeed_Impl::waterInletNodeAsModelObject() const {
  //  OptionalModelObject result = waterInletNode();
  //  return result;
  //}

  //boost::optional<ModelObject> EvaporativeFluidCoolerSingleSpeed_Impl::waterOutletNodeAsModelObject() const {
  //  OptionalModelObject result = waterOutletNode();
  //  return result;
  //}

  //boost::optional<ModelObject> EvaporativeFluidCoolerSingleSpeed_Impl::outdoorAirInletNodeAsModelObject() const {
  //  OptionalModelObject result;
  //  OptionalConnection intermediate = outdoorAirInletNode();
  //  if (intermediate) {
  //    result = *intermediate;
  //  }
  //  return result;
  //}*/

  boost::optional<ModelObject> EvaporativeFluidCoolerSingleSpeed_Impl::blowdownMakeupWaterUsageScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = blowdownMakeupWaterUsageSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  //boost::optional<ModelObject> EvaporativeFluidCoolerSingleSpeed_Impl::supplyWaterStorageTankAsModelObject() const {
  //  OptionalModelObject result;
  //  OptionalWaterStorageTank intermediate = supplyWaterStorageTank();
  //  if (intermediate) {
  //    result = *intermediate;
  //  }
  //  return result;
  //}

  //bool EvaporativeFluidCoolerSingleSpeed_Impl::setWaterInletNodeAsModelObject(const boost::optional<ModelObject>& modelObject) {
  //  if (modelObject) {
  //    OptionalConnection intermediate = modelObject->optionalCast<Connection>();
  //    if (intermediate) {
  //      Connection connection(*intermediate);
  //      return setWaterInletNode(connection);
  //    }
  //  }
  //  return false;
  //}

  //bool EvaporativeFluidCoolerSingleSpeed_Impl::setWaterOutletNodeAsModelObject(const boost::optional<ModelObject>& modelObject) {
  //  if (modelObject) {
  //    OptionalConnection intermediate = modelObject->optionalCast<Connection>();
  //    if (intermediate) {
  //      Connection connection(*intermediate);
  //      return setWaterOutletNode(connection);
  //    }
  //  }
  //  return false;
  //}

  //bool EvaporativeFluidCoolerSingleSpeed_Impl::setOutdoorAirInletNodeAsModelObject(const boost::optional<ModelObject>& modelObject) {
  //  if (modelObject) {
  //    OptionalConnection intermediate = modelObject->optionalCast<Connection>();
  //    if (intermediate) {
  //      Connection connection(*intermediate);
  //      return setOutdoorAirInletNode(connection);
  //    }
  //    else {
  //      return false;
  //    }
  //  }
  //  else {
  //    resetOutdoorAirInletNode();
  //  }
  //  return true;
  //}

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

  //bool EvaporativeFluidCoolerSingleSpeed_Impl::setSupplyWaterStorageTankAsModelObject(const boost::optional<ModelObject>& modelObject) {
  //  if (modelObject) {
  //    OptionalWaterStorageTank intermediate = modelObject->optionalCast<WaterStorageTank>();
  //    if (intermediate) {
  //      WaterStorageTank waterStorageTank(*intermediate);
  //      return setSupplyWaterStorageTank(waterStorageTank);
  //    }
  //    else {
  //      return false;
  //    }
  //  }
  //  else {
  //    resetSupplyWaterStorageTank();
  //  }
  //  return true;
  //}

} // detail

EvaporativeFluidCoolerSingleSpeed::EvaporativeFluidCoolerSingleSpeed(const Model& model)
  : StraightComponent(EvaporativeFluidCoolerSingleSpeed::iddObjectType(),model)
{
  BOOST_ASSERT(getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  //     OS_EvaporativeFluidCooler_SingleSpeedFields::WaterInletNodeName
  //     OS_EvaporativeFluidCooler_SingleSpeedFields::WaterOutletNodeName
 
   autosizeDesignAirFlowRate();
   autosizeFanPoweratDesignAirFlowRate();
   setDesignSprayWaterFlowRate(0.03);
  
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

//Connection EvaporativeFluidCoolerSingleSpeed::waterInletNode() const {
//  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->waterInletNode();
//}

//Connection EvaporativeFluidCoolerSingleSpeed::waterOutletNode() const {
//  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->waterOutletNode();
//}

boost::optional<double> EvaporativeFluidCoolerSingleSpeed::designAirFlowRate() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->designAirFlowRate();
}

//OSOptionalQuantity EvaporativeFluidCoolerSingleSpeed::getDesignAirFlowRate(bool returnIP) const {
//  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->getDesignAirFlowRate(returnIP);
//}

bool EvaporativeFluidCoolerSingleSpeed::isDesignAirFlowRateAutosized() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->isDesignAirFlowRateAutosized();
}

boost::optional<double> EvaporativeFluidCoolerSingleSpeed::fanPoweratDesignAirFlowRate() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->fanPoweratDesignAirFlowRate();
}

//OSOptionalQuantity EvaporativeFluidCoolerSingleSpeed::getFanPoweratDesignAirFlowRate(bool returnIP) const {
//  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->getFanPoweratDesignAirFlowRate(returnIP);
//}

bool EvaporativeFluidCoolerSingleSpeed::isFanPoweratDesignAirFlowRateAutosized() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->isFanPoweratDesignAirFlowRateAutosized();
}

double EvaporativeFluidCoolerSingleSpeed::designSprayWaterFlowRate() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->designSprayWaterFlowRate();
}

//Quantity EvaporativeFluidCoolerSingleSpeed::getDesignSprayWaterFlowRate(bool returnIP) const {
//  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->getDesignSprayWaterFlowRate(returnIP);
//}

boost::optional<std::string> EvaporativeFluidCoolerSingleSpeed::performanceInputMethod() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->performanceInputMethod();
}

//boost::optional<Connection> EvaporativeFluidCoolerSingleSpeed::outdoorAirInletNode() const {
//  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->outdoorAirInletNode();
//}

boost::optional<double> EvaporativeFluidCoolerSingleSpeed::standardDesignCapacity() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->standardDesignCapacity();
}

//OSOptionalQuantity EvaporativeFluidCoolerSingleSpeed::getStandardDesignCapacity(bool returnIP) const {
//  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->getStandardDesignCapacity(returnIP);
//}

boost::optional<double> EvaporativeFluidCoolerSingleSpeed::ufactorTimesAreaValueatDesignAirFlowRate() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->ufactorTimesAreaValueatDesignAirFlowRate();
}

//OSOptionalQuantity EvaporativeFluidCoolerSingleSpeed::getUfactorTimesAreaValueatDesignAirFlowRate(bool returnIP) const {
//  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->getUfactorTimesAreaValueatDesignAirFlowRate(returnIP);
//}

bool EvaporativeFluidCoolerSingleSpeed::isUfactorTimesAreaValueatDesignAirFlowRateAutosized() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->isUfactorTimesAreaValueatDesignAirFlowRateAutosized();
}

boost::optional<double> EvaporativeFluidCoolerSingleSpeed::designWaterFlowRate() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->designWaterFlowRate();
}

//OSOptionalQuantity EvaporativeFluidCoolerSingleSpeed::getDesignWaterFlowRate(bool returnIP) const {
//  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->getDesignWaterFlowRate(returnIP);
//}

bool EvaporativeFluidCoolerSingleSpeed::isDesignWaterFlowRateAutosized() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->isDesignWaterFlowRateAutosized();
}

boost::optional<double> EvaporativeFluidCoolerSingleSpeed::userSpecifiedDesignCapacity() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->userSpecifiedDesignCapacity();
}

//OSOptionalQuantity EvaporativeFluidCoolerSingleSpeed::getUserSpecifiedDesignCapacity(bool returnIP) const {
//  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->getUserSpecifiedDesignCapacity(returnIP);
//}

boost::optional<double> EvaporativeFluidCoolerSingleSpeed::designEnteringWaterTemperature() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->designEnteringWaterTemperature();
}

//OSOptionalQuantity EvaporativeFluidCoolerSingleSpeed::getDesignEnteringWaterTemperature(bool returnIP) const {
//  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->getDesignEnteringWaterTemperature(returnIP);
//}

boost::optional<double> EvaporativeFluidCoolerSingleSpeed::designEnteringAirTemperature() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->designEnteringAirTemperature();
}

//OSOptionalQuantity EvaporativeFluidCoolerSingleSpeed::getDesignEnteringAirTemperature(bool returnIP) const {
//  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->getDesignEnteringAirTemperature(returnIP);
//}

boost::optional<double> EvaporativeFluidCoolerSingleSpeed::designEnteringAirWetbulbTemperature() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->designEnteringAirWetbulbTemperature();
}

//OSOptionalQuantity EvaporativeFluidCoolerSingleSpeed::getDesignEnteringAirWetbulbTemperature(bool returnIP) const {
//  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->getDesignEnteringAirWetbulbTemperature(returnIP);
//}

std::string EvaporativeFluidCoolerSingleSpeed::capacityControl() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->capacityControl();
}

//bool EvaporativeFluidCoolerSingleSpeed::isCapacityControlDefaulted() const {
//  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->isCapacityControlDefaulted();
//}

double EvaporativeFluidCoolerSingleSpeed::sizingFactor() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->sizingFactor();
}

//Quantity EvaporativeFluidCoolerSingleSpeed::getSizingFactor(bool returnIP) const {
//  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->getSizingFactor(returnIP);
//}

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

//OSOptionalQuantity EvaporativeFluidCoolerSingleSpeed::getEvaporationLossFactor(bool returnIP) const {
//  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->getEvaporationLossFactor(returnIP);
//}

double EvaporativeFluidCoolerSingleSpeed::driftLossPercent() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->driftLossPercent();
}

//Quantity EvaporativeFluidCoolerSingleSpeed::getDriftLossPercent(bool returnIP) const {
//  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->getDriftLossPercent(returnIP);
//}

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

//Quantity EvaporativeFluidCoolerSingleSpeed::getBlowdownConcentrationRatio(bool returnIP) const {
//  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->getBlowdownConcentrationRatio(returnIP);
//}

bool EvaporativeFluidCoolerSingleSpeed::isBlowdownConcentrationRatioDefaulted() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->isBlowdownConcentrationRatioDefaulted();
}

boost::optional<Schedule> EvaporativeFluidCoolerSingleSpeed::blowdownMakeupWaterUsageSchedule() const {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->blowdownMakeupWaterUsageSchedule();
}

//boost::optional<WaterStorageTank> EvaporativeFluidCoolerSingleSpeed::supplyWaterStorageTank() const {
//  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->supplyWaterStorageTank();
//}

//bool EvaporativeFluidCoolerSingleSpeed::setWaterInletNode(const Connection& connection) {
//  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setWaterInletNode(connection);
//}

//bool EvaporativeFluidCoolerSingleSpeed::setWaterOutletNode(const Connection& connection) {
//  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setWaterOutletNode(connection);
//}

bool EvaporativeFluidCoolerSingleSpeed::setDesignAirFlowRate(double designAirFlowRate) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setDesignAirFlowRate(designAirFlowRate);
}

//bool EvaporativeFluidCoolerSingleSpeed::setDesignAirFlowRate(const Quantity& designAirFlowRate) {
//  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setDesignAirFlowRate(designAirFlowRate);
//}

void EvaporativeFluidCoolerSingleSpeed::autosizeDesignAirFlowRate() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->autosizeDesignAirFlowRate();
}

bool EvaporativeFluidCoolerSingleSpeed::setFanPoweratDesignAirFlowRate(double fanPoweratDesignAirFlowRate) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setFanPoweratDesignAirFlowRate(fanPoweratDesignAirFlowRate);
}

//bool EvaporativeFluidCoolerSingleSpeed::setFanPoweratDesignAirFlowRate(const Quantity& fanPoweratDesignAirFlowRate) {
//  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setFanPoweratDesignAirFlowRate(fanPoweratDesignAirFlowRate);
//}

void EvaporativeFluidCoolerSingleSpeed::autosizeFanPoweratDesignAirFlowRate() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->autosizeFanPoweratDesignAirFlowRate();
}

bool EvaporativeFluidCoolerSingleSpeed::setDesignSprayWaterFlowRate(double designSprayWaterFlowRate) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setDesignSprayWaterFlowRate(designSprayWaterFlowRate);
}

//bool EvaporativeFluidCoolerSingleSpeed::setDesignSprayWaterFlowRate(const Quantity& designSprayWaterFlowRate) {
//  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setDesignSprayWaterFlowRate(designSprayWaterFlowRate);
//}

bool EvaporativeFluidCoolerSingleSpeed::setPerformanceInputMethod(std::string performanceInputMethod) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setPerformanceInputMethod(performanceInputMethod);
}

void EvaporativeFluidCoolerSingleSpeed::resetPerformanceInputMethod() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->resetPerformanceInputMethod();
}

//bool EvaporativeFluidCoolerSingleSpeed::setOutdoorAirInletNode(const Connection& connection) {
 // return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setOutdoorAirInletNode(connection);
//}

void EvaporativeFluidCoolerSingleSpeed::resetOutdoorAirInletNode() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->resetOutdoorAirInletNode();
}

bool EvaporativeFluidCoolerSingleSpeed::setStandardDesignCapacity(double standardDesignCapacity) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setStandardDesignCapacity(standardDesignCapacity);
}

//bool EvaporativeFluidCoolerSingleSpeed::setStandardDesignCapacity(const Quantity& standardDesignCapacity) {
//  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setStandardDesignCapacity(standardDesignCapacity);
//}

void EvaporativeFluidCoolerSingleSpeed::resetStandardDesignCapacity() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->resetStandardDesignCapacity();
}

bool EvaporativeFluidCoolerSingleSpeed::setUfactorTimesAreaValueatDesignAirFlowRate(double ufactorTimesAreaValueatDesignAirFlowRate) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setUfactorTimesAreaValueatDesignAirFlowRate(ufactorTimesAreaValueatDesignAirFlowRate);
}

//bool EvaporativeFluidCoolerSingleSpeed::setUfactorTimesAreaValueatDesignAirFlowRate(const Quantity& ufactorTimesAreaValueatDesignAirFlowRate) {
//  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setUfactorTimesAreaValueatDesignAirFlowRate(ufactorTimesAreaValueatDesignAirFlowRate);
//}

void EvaporativeFluidCoolerSingleSpeed::resetUfactorTimesAreaValueatDesignAirFlowRate() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->resetUfactorTimesAreaValueatDesignAirFlowRate();
}

void EvaporativeFluidCoolerSingleSpeed::autosizeUfactorTimesAreaValueatDesignAirFlowRate() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->autosizeUfactorTimesAreaValueatDesignAirFlowRate();
}

bool EvaporativeFluidCoolerSingleSpeed::setDesignWaterFlowRate(double designWaterFlowRate) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setDesignWaterFlowRate(designWaterFlowRate);
}

//bool EvaporativeFluidCoolerSingleSpeed::setDesignWaterFlowRate(const Quantity& designWaterFlowRate) {
//  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setDesignWaterFlowRate(designWaterFlowRate);
//}

void EvaporativeFluidCoolerSingleSpeed::resetDesignWaterFlowRate() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->resetDesignWaterFlowRate();
}

void EvaporativeFluidCoolerSingleSpeed::autosizeDesignWaterFlowRate() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->autosizeDesignWaterFlowRate();
}

bool EvaporativeFluidCoolerSingleSpeed::setUserSpecifiedDesignCapacity(double userSpecifiedDesignCapacity) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setUserSpecifiedDesignCapacity(userSpecifiedDesignCapacity);
}

//bool EvaporativeFluidCoolerSingleSpeed::setUserSpecifiedDesignCapacity(const Quantity& userSpecifiedDesignCapacity) {
//  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setUserSpecifiedDesignCapacity(userSpecifiedDesignCapacity);
//}

void EvaporativeFluidCoolerSingleSpeed::resetUserSpecifiedDesignCapacity() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->resetUserSpecifiedDesignCapacity();
}

bool EvaporativeFluidCoolerSingleSpeed::setDesignEnteringWaterTemperature(double designEnteringWaterTemperature) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setDesignEnteringWaterTemperature(designEnteringWaterTemperature);
}

//bool EvaporativeFluidCoolerSingleSpeed::setDesignEnteringWaterTemperature(const Quantity& designEnteringWaterTemperature) {
//  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setDesignEnteringWaterTemperature(designEnteringWaterTemperature);
//}

void EvaporativeFluidCoolerSingleSpeed::resetDesignEnteringWaterTemperature() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->resetDesignEnteringWaterTemperature();
}

bool EvaporativeFluidCoolerSingleSpeed::setDesignEnteringAirTemperature(double designEnteringAirTemperature) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setDesignEnteringAirTemperature(designEnteringAirTemperature);
}

//bool EvaporativeFluidCoolerSingleSpeed::setDesignEnteringAirTemperature(const Quantity& designEnteringAirTemperature) {
//  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setDesignEnteringAirTemperature(designEnteringAirTemperature);
//}

void EvaporativeFluidCoolerSingleSpeed::resetDesignEnteringAirTemperature() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->resetDesignEnteringAirTemperature();
}

bool EvaporativeFluidCoolerSingleSpeed::setDesignEnteringAirWetbulbTemperature(double designEnteringAirWetbulbTemperature) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setDesignEnteringAirWetbulbTemperature(designEnteringAirWetbulbTemperature);
}

//bool EvaporativeFluidCoolerSingleSpeed::setDesignEnteringAirWetbulbTemperature(const Quantity& designEnteringAirWetbulbTemperature) {
//  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setDesignEnteringAirWetbulbTemperature(designEnteringAirWetbulbTemperature);
//}

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

//bool EvaporativeFluidCoolerSingleSpeed::setSizingFactor(const Quantity& sizingFactor) {
//  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setSizingFactor(sizingFactor);
//}

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

//bool EvaporativeFluidCoolerSingleSpeed::setEvaporationLossFactor(const Quantity& evaporationLossFactor) {
//  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setEvaporationLossFactor(evaporationLossFactor);
//}

void EvaporativeFluidCoolerSingleSpeed::resetEvaporationLossFactor() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->resetEvaporationLossFactor();
}

void EvaporativeFluidCoolerSingleSpeed::setDriftLossPercent(double driftLossPercent) {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setDriftLossPercent(driftLossPercent);
}

//bool EvaporativeFluidCoolerSingleSpeed::setDriftLossPercent(const Quantity& driftLossPercent) {
//  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setDriftLossPercent(driftLossPercent);
//}

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

//bool EvaporativeFluidCoolerSingleSpeed::setBlowdownConcentrationRatio(const Quantity& blowdownConcentrationRatio) {
//  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setBlowdownConcentrationRatio(blowdownConcentrationRatio);
//}

void EvaporativeFluidCoolerSingleSpeed::resetBlowdownConcentrationRatio() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->resetBlowdownConcentrationRatio();
}

bool EvaporativeFluidCoolerSingleSpeed::setBlowdownMakeupWaterUsageSchedule(Schedule& schedule) {
  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setBlowdownMakeupWaterUsageSchedule(schedule);
}

void EvaporativeFluidCoolerSingleSpeed::resetBlowdownMakeupWaterUsageSchedule() {
  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->resetBlowdownMakeupWaterUsageSchedule();
}

//bool EvaporativeFluidCoolerSingleSpeed::setSupplyWaterStorageTank(const WaterStorageTank& waterStorageTank) {
//  return getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->setSupplyWaterStorageTank(waterStorageTank);
//}

//void EvaporativeFluidCoolerSingleSpeed::resetSupplyWaterStorageTank() {
//  getImpl<detail::EvaporativeFluidCoolerSingleSpeed_Impl>()->resetSupplyWaterStorageTank();
//}

/// @cond
EvaporativeFluidCoolerSingleSpeed::EvaporativeFluidCoolerSingleSpeed(boost::shared_ptr<detail::EvaporativeFluidCoolerSingleSpeed_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

