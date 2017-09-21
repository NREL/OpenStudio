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

#include "ZoneHVACUnitVentilator.hpp"
#include "ZoneHVACUnitVentilator_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "HVACComponent.hpp"
#include "HVACComponent_Impl.hpp"
#include "Model.hpp"
#include "FanConstantVolume.hpp"
#include "WaterToAirComponent.hpp"
#include "WaterToAirComponent_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_ZoneHVAC_UnitVentilator_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ZoneHVACUnitVentilator_Impl::ZoneHVACUnitVentilator_Impl(const IdfObject& idfObject,
                                                           Model_Impl* model,
                                                           bool keepHandle)
    : ZoneHVACComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ZoneHVACUnitVentilator::iddObjectType());
  }

  ZoneHVACUnitVentilator_Impl::ZoneHVACUnitVentilator_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                           Model_Impl* model,
                                                           bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ZoneHVACUnitVentilator::iddObjectType());
  }

  ZoneHVACUnitVentilator_Impl::ZoneHVACUnitVentilator_Impl(const ZoneHVACUnitVentilator_Impl& other,
                                                           Model_Impl* model,
                                                           bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ZoneHVACUnitVentilator_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ZoneHVACUnitVentilator_Impl::iddObjectType() const {
    return ZoneHVACUnitVentilator::iddObjectType();
  }

  std::vector<ScheduleTypeKey> ZoneHVACUnitVentilator_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_ZoneHVAC_UnitVentilatorFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneHVACUnitVentilator","Availability"));
    }
    if (std::find(b,e,OS_ZoneHVAC_UnitVentilatorFields::MinimumOutdoorAirScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneHVACUnitVentilator","Minimum Outdoor Air"));
    }
    if (std::find(b,e,OS_ZoneHVAC_UnitVentilatorFields::MaximumOutdoorAirFractionorTemperatureScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneHVACUnitVentilator","Maximum Outdoor Air Fraction or Temperature"));
    }
    if (std::find(b,e,OS_ZoneHVAC_UnitVentilatorFields::SupplyAirFanOperatingModeScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneHVACUnitVentilator","Supply Air Fan Operating Mode"));
    }
    return result;
  }

  Schedule ZoneHVACUnitVentilator_Impl::availabilitySchedule() const {
    boost::optional<Schedule> value = optionalAvailabilitySchedule();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Availability Schedule attached.");
    }
    return value.get();
  }

  ModelObject ZoneHVACUnitVentilator_Impl::clone(Model model) const
  {
    auto objectClone = ZoneHVACComponent_Impl::clone(model).cast<ZoneHVACUnitVentilator>();

    auto const supplyFanClone = this->supplyAirFan().clone(model).cast<HVACComponent>();

    objectClone.setSupplyAirFan(supplyFanClone);

    bool sameModel = model == this->model();

    if( auto const t_coolingCoil = coolingCoil() ) {
      auto const coolingCoilClone = t_coolingCoil->clone(model).cast<HVACComponent>();
      objectClone.setCoolingCoil(coolingCoilClone);

      if ( sameModel ) {
        if( auto waterToAirComponent = t_coolingCoil->optionalCast<WaterToAirComponent>() ) {
          if( auto plant = waterToAirComponent->plantLoop() ) {
            plant->addDemandBranchForComponent(coolingCoilClone);
          }
        }
      }
    }

    if( auto const t_heatingCoil = heatingCoil() ) {
      auto const heatingCoilClone = t_heatingCoil->clone(model).cast<HVACComponent>();
      objectClone.setHeatingCoil(heatingCoilClone);

      if ( sameModel ) {
        if( auto waterToAirComponent = t_heatingCoil->optionalCast<WaterToAirComponent>() ) {
          if( auto plant = waterToAirComponent->plantLoop() ) {
            plant->addDemandBranchForComponent(heatingCoilClone);
          }
        }
      }
    }

    return objectClone;
  }

  std::vector<IdfObject> ZoneHVACUnitVentilator_Impl::remove()
  {
    if( auto const waterHeatingCoil = heatingCoil() ) {
      if( auto waterToAirComponent = waterHeatingCoil->optionalCast<WaterToAirComponent>() ) {
        if( auto plant = waterToAirComponent->plantLoop() ) {
          plant->removeDemandBranchWithComponent(waterHeatingCoil.get());
        }
      }
    }
    if( auto const waterCoolingCoil = coolingCoil() ) {
      if( auto waterToAirComponent = waterCoolingCoil->optionalCast<WaterToAirComponent>() ) {
        if( auto plant = waterToAirComponent->plantLoop() ) {
          plant->removeDemandBranchWithComponent(waterCoolingCoil.get());
        }
      }
    }
    return ZoneHVACComponent_Impl::remove();
  }

  std::vector<ModelObject> ZoneHVACUnitVentilator_Impl::children() const
  {
    std::vector<ModelObject> result;
    if (auto const intermediate = optionalSupplyAirFan()) {
      result.push_back(intermediate.get());
    }
    if (auto const intermediate = heatingCoil()) {
      result.push_back(intermediate.get());
    }
    if (auto const intermediate = coolingCoil()) {
      result.push_back(intermediate.get());
    }
    return result;
  }

  unsigned ZoneHVACUnitVentilator_Impl::inletPort() const
  {
    return OS_ZoneHVAC_UnitVentilatorFields::AirInletNodeName;
  }

  unsigned ZoneHVACUnitVentilator_Impl::outletPort() const
  {
    return OS_ZoneHVAC_UnitVentilatorFields::AirOutletNodeName;
  }

  boost::optional<double> ZoneHVACUnitVentilator_Impl::maximumSupplyAirFlowRate() const {
    return getDouble(OS_ZoneHVAC_UnitVentilatorFields::MaximumSupplyAirFlowRate,true);
  }

  bool ZoneHVACUnitVentilator_Impl::isMaximumSupplyAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ZoneHVAC_UnitVentilatorFields::MaximumSupplyAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  std::string ZoneHVACUnitVentilator_Impl::outdoorAirControlType() const {
    boost::optional<std::string> value = getString(OS_ZoneHVAC_UnitVentilatorFields::OutdoorAirControlType,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> ZoneHVACUnitVentilator_Impl::minimumOutdoorAirFlowRate() const {
    return getDouble(OS_ZoneHVAC_UnitVentilatorFields::MinimumOutdoorAirFlowRate,true);
  }

  bool ZoneHVACUnitVentilator_Impl::isMinimumOutdoorAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ZoneHVAC_UnitVentilatorFields::MinimumOutdoorAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  Schedule ZoneHVACUnitVentilator_Impl::minimumOutdoorAirSchedule() const {
    boost::optional<Schedule> value = optionalMinimumOutdoorAirSchedule();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Minimum Outdoor Air Schedule attached.");
    }
    return value.get();
  }

  boost::optional<double> ZoneHVACUnitVentilator_Impl::maximumOutdoorAirFlowRate() const {
    return getDouble(OS_ZoneHVAC_UnitVentilatorFields::MaximumOutdoorAirFlowRate,true);
  }

  bool ZoneHVACUnitVentilator_Impl::isMaximumOutdoorAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ZoneHVAC_UnitVentilatorFields::MaximumOutdoorAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  Schedule ZoneHVACUnitVentilator_Impl::maximumOutdoorAirFractionorTemperatureSchedule() const {
    boost::optional<Schedule> value = optionalMaximumOutdoorAirFractionorTemperatureSchedule();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Maximum Outdoor Air Fractionor Temperature Schedule attached.");
    }
    return value.get();
  }

  HVACComponent ZoneHVACUnitVentilator_Impl::supplyAirFan() const {
    boost::optional<HVACComponent> value = optionalSupplyAirFan();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Supply Air Fan attached.");
    }
    return value.get();
  }

  boost::optional<Schedule> ZoneHVACUnitVentilator_Impl::supplyAirFanOperatingModeSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneHVAC_UnitVentilatorFields::SupplyAirFanOperatingModeScheduleName);
  }

  boost::optional<HVACComponent> ZoneHVACUnitVentilator_Impl::heatingCoil() const {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_ZoneHVAC_UnitVentilatorFields::HeatingCoilName);
  }

  double ZoneHVACUnitVentilator_Impl::heatingConvergenceTolerance() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_UnitVentilatorFields::HeatingConvergenceTolerance,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<HVACComponent> ZoneHVACUnitVentilator_Impl::coolingCoil() const {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_ZoneHVAC_UnitVentilatorFields::CoolingCoilName);
  }

  double ZoneHVACUnitVentilator_Impl::coolingConvergenceTolerance() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_UnitVentilatorFields::CoolingConvergenceTolerance,true);
    OS_ASSERT(value);
    return value.get();
  }

  // boost::optional<ModelObject> ZoneHVACUnitVentilator_Impl::availabilityManagerList() const {
  //   return getObject<ModelObject>().getModelObjectTarget<ModelObject>(OS_ZoneHVAC_UnitVentilatorFields::AvailabilityManagerListName);
  // }

  // boost::optional<ModelObject> ZoneHVACUnitVentilator_Impl::designSpecificationZoneHVACSizingObject() const {
  //   return getObject<ModelObject>().getModelObjectTarget<ModelObject>(OS_ZoneHVAC_UnitVentilatorFields::DesignSpecificationZoneHVACSizingObjectName);
  // }

  bool ZoneHVACUnitVentilator_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ZoneHVAC_UnitVentilatorFields::AvailabilityScheduleName,
                              "ZoneHVACUnitVentilator",
                              "Availability",
                              schedule);
    return result;
  }

  bool ZoneHVACUnitVentilator_Impl::setMaximumSupplyAirFlowRate(boost::optional<double> maximumSupplyAirFlowRate) {
    bool result(false);
    if (maximumSupplyAirFlowRate) {
      result = setDouble(OS_ZoneHVAC_UnitVentilatorFields::MaximumSupplyAirFlowRate, maximumSupplyAirFlowRate.get());
    }
    return result;
  }

  void ZoneHVACUnitVentilator_Impl::autosizeMaximumSupplyAirFlowRate() {
    bool result = setString(OS_ZoneHVAC_UnitVentilatorFields::MaximumSupplyAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool ZoneHVACUnitVentilator_Impl::setOutdoorAirControlType(std::string outdoorAirControlType) {
    bool result = setString(OS_ZoneHVAC_UnitVentilatorFields::OutdoorAirControlType, outdoorAirControlType);
    return result;
  }

  bool ZoneHVACUnitVentilator_Impl::setMinimumOutdoorAirFlowRate(boost::optional<double> minimumOutdoorAirFlowRate) {
    bool result(false);
    if (minimumOutdoorAirFlowRate) {
      result = setDouble(OS_ZoneHVAC_UnitVentilatorFields::MinimumOutdoorAirFlowRate, minimumOutdoorAirFlowRate.get());
    }
    return result;
  }

  void ZoneHVACUnitVentilator_Impl::autosizeMinimumOutdoorAirFlowRate() {
    bool result = setString(OS_ZoneHVAC_UnitVentilatorFields::MinimumOutdoorAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool ZoneHVACUnitVentilator_Impl::setMinimumOutdoorAirSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ZoneHVAC_UnitVentilatorFields::MinimumOutdoorAirScheduleName,
                              "ZoneHVACUnitVentilator",
                              "Minimum Outdoor Air",
                              schedule);
    return result;
  }

  bool ZoneHVACUnitVentilator_Impl::setMaximumOutdoorAirFlowRate(boost::optional<double> maximumOutdoorAirFlowRate) {
    bool result(false);
    if (maximumOutdoorAirFlowRate) {
      result = setDouble(OS_ZoneHVAC_UnitVentilatorFields::MaximumOutdoorAirFlowRate, maximumOutdoorAirFlowRate.get());
    }
    return result;
  }

  void ZoneHVACUnitVentilator_Impl::autosizeMaximumOutdoorAirFlowRate() {
    bool result = setString(OS_ZoneHVAC_UnitVentilatorFields::MaximumOutdoorAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool ZoneHVACUnitVentilator_Impl::setMaximumOutdoorAirFractionorTemperatureSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ZoneHVAC_UnitVentilatorFields::MaximumOutdoorAirFractionorTemperatureScheduleName,
                              "ZoneHVACUnitVentilator",
                              "Maximum Outdoor Air Fraction or Temperature",
                              schedule);
    return result;
  }

  bool ZoneHVACUnitVentilator_Impl::setSupplyAirFan(const HVACComponent& supplyAirFan) {
    bool result = setPointer(OS_ZoneHVAC_UnitVentilatorFields::SupplyAirFanName, supplyAirFan.handle());
    return result;
  }

  bool ZoneHVACUnitVentilator_Impl::setSupplyAirFanOperatingModeSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ZoneHVAC_UnitVentilatorFields::SupplyAirFanOperatingModeScheduleName,
                              "ZoneHVACUnitVentilator",
                              "Supply Air Fan Operating Mode",
                              schedule);
    return result;
  }

  void ZoneHVACUnitVentilator_Impl::resetSupplyAirFanOperatingModeSchedule() {
    bool result = setString(OS_ZoneHVAC_UnitVentilatorFields::SupplyAirFanOperatingModeScheduleName, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACUnitVentilator_Impl::setHeatingCoil(const boost::optional<HVACComponent>& heatingCoil) {
    bool result(false);
    if (heatingCoil) {
      result = setPointer(OS_ZoneHVAC_UnitVentilatorFields::HeatingCoilName, heatingCoil.get().handle());
    }
    else {
      resetHeatingCoil();
      result = true;
    }
    return result;
  }

  void ZoneHVACUnitVentilator_Impl::resetHeatingCoil() {
    bool result = setString(OS_ZoneHVAC_UnitVentilatorFields::HeatingCoilName, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACUnitVentilator_Impl::setHeatingConvergenceTolerance(double heatingConvergenceTolerance) {
    bool result = setDouble(OS_ZoneHVAC_UnitVentilatorFields::HeatingConvergenceTolerance, heatingConvergenceTolerance);
    return result;
  }

  bool ZoneHVACUnitVentilator_Impl::setCoolingCoil(const boost::optional<HVACComponent>& coolingCoil) {
    bool result(false);
    if (coolingCoil) {
      result = setPointer(OS_ZoneHVAC_UnitVentilatorFields::CoolingCoilName, coolingCoil.get().handle());
    }
    else {
      resetCoolingCoil();
      result = true;
    }
    return result;
  }

  void ZoneHVACUnitVentilator_Impl::resetCoolingCoil() {
    bool result = setString(OS_ZoneHVAC_UnitVentilatorFields::CoolingCoilName, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACUnitVentilator_Impl::setCoolingConvergenceTolerance(double coolingConvergenceTolerance) {
    bool result = setDouble(OS_ZoneHVAC_UnitVentilatorFields::CoolingConvergenceTolerance, coolingConvergenceTolerance);
    return result;
  }

  // bool ZoneHVACUnitVentilator_Impl::setAvailabilityManagerList(const boost::optional<ModelObject>& systemAvailabilityManagerLists) {
  //   bool result(false);
  //   if (systemAvailabilityManagerLists) {
  //     result = setPointer(OS_ZoneHVAC_UnitVentilatorFields::AvailabilityManagerListName, systemAvailabilityManagerLists.get().handle());
  //   }
  //   else {
  //     resetAvailabilityManagerList();
  //     result = true;
  //   }
  //   return result;
  // }

  // void ZoneHVACUnitVentilator_Impl::resetAvailabilityManagerList() {
  //   bool result = setString(OS_ZoneHVAC_UnitVentilatorFields::AvailabilityManagerListName, "");
  //   OS_ASSERT(result);
  // }

  // bool ZoneHVACUnitVentilator_Impl::setDesignSpecificationZoneHVACSizingObject(const boost::optional<ModelObject>& designSpecificationZoneHVACSizingName) {
  //   bool result(false);
  //   if (designSpecificationZoneHVACSizingName) {
  //     result = setPointer(OS_ZoneHVAC_UnitVentilatorFields::DesignSpecificationZoneHVACSizingObjectName, designSpecificationZoneHVACSizingName.get().handle());
  //   }
  //   else {
  //     resetDesignSpecificationZoneHVACSizingObject();
  //     result = true;
  //   }
  //   return result;
  // }

  // void ZoneHVACUnitVentilator_Impl::resetDesignSpecificationZoneHVACSizingObject() {
  //   bool result = setString(OS_ZoneHVAC_UnitVentilatorFields::DesignSpecificationZoneHVACSizingObjectName, "");
  //   OS_ASSERT(result);
  // }

  boost::optional<Schedule> ZoneHVACUnitVentilator_Impl::optionalAvailabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneHVAC_UnitVentilatorFields::AvailabilityScheduleName);
  }

  boost::optional<Schedule> ZoneHVACUnitVentilator_Impl::optionalMinimumOutdoorAirSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneHVAC_UnitVentilatorFields::MinimumOutdoorAirScheduleName);
  }

  boost::optional<Schedule> ZoneHVACUnitVentilator_Impl::optionalMaximumOutdoorAirFractionorTemperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneHVAC_UnitVentilatorFields::MaximumOutdoorAirFractionorTemperatureScheduleName);
  }

  boost::optional<HVACComponent> ZoneHVACUnitVentilator_Impl::optionalSupplyAirFan() const {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_ZoneHVAC_UnitVentilatorFields::SupplyAirFanName);
  }

} // detail

ZoneHVACUnitVentilator::ZoneHVACUnitVentilator(const Model& model)
  : ZoneHVACComponent(ZoneHVACUnitVentilator::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ZoneHVACUnitVentilator_Impl>());

  auto always_on = model.alwaysOnDiscreteSchedule();
  auto always_off = model.alwaysOffDiscreteSchedule();
  auto supplyAirFan = FanConstantVolume( model );

  bool ok = true;
  ok = setAvailabilitySchedule( always_on );
  OS_ASSERT(ok);
  autosizeMaximumSupplyAirFlowRate();
  ok = setOutdoorAirControlType( "VariablePercent" );
  OS_ASSERT(ok);
  autosizeMinimumOutdoorAirFlowRate();
  ok = setMinimumOutdoorAirSchedule( always_off );
  OS_ASSERT(ok);
  autosizeMaximumOutdoorAirFlowRate();
  ok = setMaximumOutdoorAirFractionorTemperatureSchedule( always_on );
  OS_ASSERT(ok);
  ok = setSupplyAirFan( supplyAirFan );
  OS_ASSERT(ok);
  ok = setHeatingConvergenceTolerance( 0.001 );
  OS_ASSERT(ok);
  ok = setCoolingConvergenceTolerance( 0.001 );
  OS_ASSERT(ok);
}

ZoneHVACUnitVentilator::ZoneHVACUnitVentilator(
  const Model& model,
  const HVACComponent& supplyAirFan
  )
  : ZoneHVACComponent(ZoneHVACUnitVentilator::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ZoneHVACUnitVentilator_Impl>());

  auto always_on = model.alwaysOnDiscreteSchedule();
  auto always_off = model.alwaysOffDiscreteSchedule();

  bool ok = true;
  ok = setAvailabilitySchedule( always_on );
  OS_ASSERT(ok);
  autosizeMaximumSupplyAirFlowRate();
  ok = setOutdoorAirControlType( "VariablePercent" );
  OS_ASSERT(ok);
  autosizeMinimumOutdoorAirFlowRate();
  ok = setMinimumOutdoorAirSchedule( always_off );
  OS_ASSERT(ok);
  autosizeMaximumOutdoorAirFlowRate();
  ok = setMaximumOutdoorAirFractionorTemperatureSchedule( always_on );
  OS_ASSERT(ok);
  ok = setSupplyAirFan( supplyAirFan );
  OS_ASSERT(ok);
  ok = setHeatingConvergenceTolerance( 0.001 );
  OS_ASSERT(ok);
  ok = setCoolingConvergenceTolerance( 0.001 );
  OS_ASSERT(ok);
}

IddObjectType ZoneHVACUnitVentilator::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ZoneHVAC_UnitVentilator);
}

std::vector<std::string> ZoneHVACUnitVentilator::outdoorAirControlTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ZoneHVAC_UnitVentilatorFields::OutdoorAirControlType);
}

Schedule ZoneHVACUnitVentilator::availabilitySchedule() const {
  return getImpl<detail::ZoneHVACUnitVentilator_Impl>()->availabilitySchedule();
}

boost::optional<double> ZoneHVACUnitVentilator::maximumSupplyAirFlowRate() const {
  return getImpl<detail::ZoneHVACUnitVentilator_Impl>()->maximumSupplyAirFlowRate();
}

bool ZoneHVACUnitVentilator::isMaximumSupplyAirFlowRateAutosized() const {
  return getImpl<detail::ZoneHVACUnitVentilator_Impl>()->isMaximumSupplyAirFlowRateAutosized();
}

std::string ZoneHVACUnitVentilator::outdoorAirControlType() const {
  return getImpl<detail::ZoneHVACUnitVentilator_Impl>()->outdoorAirControlType();
}

boost::optional<double> ZoneHVACUnitVentilator::minimumOutdoorAirFlowRate() const {
  return getImpl<detail::ZoneHVACUnitVentilator_Impl>()->minimumOutdoorAirFlowRate();
}

bool ZoneHVACUnitVentilator::isMinimumOutdoorAirFlowRateAutosized() const {
  return getImpl<detail::ZoneHVACUnitVentilator_Impl>()->isMinimumOutdoorAirFlowRateAutosized();
}

Schedule ZoneHVACUnitVentilator::minimumOutdoorAirSchedule() const {
  return getImpl<detail::ZoneHVACUnitVentilator_Impl>()->minimumOutdoorAirSchedule();
}

boost::optional<double> ZoneHVACUnitVentilator::maximumOutdoorAirFlowRate() const {
  return getImpl<detail::ZoneHVACUnitVentilator_Impl>()->maximumOutdoorAirFlowRate();
}

bool ZoneHVACUnitVentilator::isMaximumOutdoorAirFlowRateAutosized() const {
  return getImpl<detail::ZoneHVACUnitVentilator_Impl>()->isMaximumOutdoorAirFlowRateAutosized();
}

Schedule ZoneHVACUnitVentilator::maximumOutdoorAirFractionorTemperatureSchedule() const {
  return getImpl<detail::ZoneHVACUnitVentilator_Impl>()->maximumOutdoorAirFractionorTemperatureSchedule();
}

HVACComponent ZoneHVACUnitVentilator::supplyAirFan() const {
  return getImpl<detail::ZoneHVACUnitVentilator_Impl>()->supplyAirFan();
}

boost::optional<Schedule> ZoneHVACUnitVentilator::supplyAirFanOperatingModeSchedule() const {
  return getImpl<detail::ZoneHVACUnitVentilator_Impl>()->supplyAirFanOperatingModeSchedule();
}

boost::optional<HVACComponent> ZoneHVACUnitVentilator::heatingCoil() const {
  return getImpl<detail::ZoneHVACUnitVentilator_Impl>()->heatingCoil();
}

double ZoneHVACUnitVentilator::heatingConvergenceTolerance() const {
  return getImpl<detail::ZoneHVACUnitVentilator_Impl>()->heatingConvergenceTolerance();
}

boost::optional<HVACComponent> ZoneHVACUnitVentilator::coolingCoil() const {
  return getImpl<detail::ZoneHVACUnitVentilator_Impl>()->coolingCoil();
}

double ZoneHVACUnitVentilator::coolingConvergenceTolerance() const {
  return getImpl<detail::ZoneHVACUnitVentilator_Impl>()->coolingConvergenceTolerance();
}

// boost::optional<ModelObject> ZoneHVACUnitVentilator::availabilityManagerList() const {
//   return getImpl<detail::ZoneHVACUnitVentilator_Impl>()->availabilityManagerList();
// }

// boost::optional<ModelObject> ZoneHVACUnitVentilator::designSpecificationZoneHVACSizingObject() const {
//   return getImpl<detail::ZoneHVACUnitVentilator_Impl>()->designSpecificationZoneHVACSizingObject();
// }

bool ZoneHVACUnitVentilator::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::ZoneHVACUnitVentilator_Impl>()->setAvailabilitySchedule(schedule);
}

bool ZoneHVACUnitVentilator::setMaximumSupplyAirFlowRate(double maximumSupplyAirFlowRate) {
  return getImpl<detail::ZoneHVACUnitVentilator_Impl>()->setMaximumSupplyAirFlowRate(maximumSupplyAirFlowRate);
}

void ZoneHVACUnitVentilator::autosizeMaximumSupplyAirFlowRate() {
  getImpl<detail::ZoneHVACUnitVentilator_Impl>()->autosizeMaximumSupplyAirFlowRate();
}

bool ZoneHVACUnitVentilator::setOutdoorAirControlType(std::string outdoorAirControlType) {
  return getImpl<detail::ZoneHVACUnitVentilator_Impl>()->setOutdoorAirControlType(outdoorAirControlType);
}

bool ZoneHVACUnitVentilator::setMinimumOutdoorAirFlowRate(double minimumOutdoorAirFlowRate) {
  return getImpl<detail::ZoneHVACUnitVentilator_Impl>()->setMinimumOutdoorAirFlowRate(minimumOutdoorAirFlowRate);
}

void ZoneHVACUnitVentilator::autosizeMinimumOutdoorAirFlowRate() {
  getImpl<detail::ZoneHVACUnitVentilator_Impl>()->autosizeMinimumOutdoorAirFlowRate();
}

bool ZoneHVACUnitVentilator::setMinimumOutdoorAirSchedule(Schedule& schedule) {
  return getImpl<detail::ZoneHVACUnitVentilator_Impl>()->setMinimumOutdoorAirSchedule(schedule);
}

bool ZoneHVACUnitVentilator::setMaximumOutdoorAirFlowRate(double maximumOutdoorAirFlowRate) {
  return getImpl<detail::ZoneHVACUnitVentilator_Impl>()->setMaximumOutdoorAirFlowRate(maximumOutdoorAirFlowRate);
}

void ZoneHVACUnitVentilator::autosizeMaximumOutdoorAirFlowRate() {
  getImpl<detail::ZoneHVACUnitVentilator_Impl>()->autosizeMaximumOutdoorAirFlowRate();
}

bool ZoneHVACUnitVentilator::setMaximumOutdoorAirFractionorTemperatureSchedule(Schedule& schedule) {
  return getImpl<detail::ZoneHVACUnitVentilator_Impl>()->setMaximumOutdoorAirFractionorTemperatureSchedule(schedule);
}

bool ZoneHVACUnitVentilator::setSupplyAirFan(const HVACComponent& supplyAirFan) {
  return getImpl<detail::ZoneHVACUnitVentilator_Impl>()->setSupplyAirFan(supplyAirFan);
}

bool ZoneHVACUnitVentilator::setSupplyAirFanOperatingModeSchedule(Schedule& schedule) {
  return getImpl<detail::ZoneHVACUnitVentilator_Impl>()->setSupplyAirFanOperatingModeSchedule(schedule);
}

void ZoneHVACUnitVentilator::resetSupplyAirFanOperatingModeSchedule() {
  getImpl<detail::ZoneHVACUnitVentilator_Impl>()->resetSupplyAirFanOperatingModeSchedule();
}

bool ZoneHVACUnitVentilator::setHeatingCoil(const HVACComponent& heatingCoil) {
  return getImpl<detail::ZoneHVACUnitVentilator_Impl>()->setHeatingCoil(heatingCoil);
}

void ZoneHVACUnitVentilator::resetHeatingCoil() {
  getImpl<detail::ZoneHVACUnitVentilator_Impl>()->resetHeatingCoil();
}

bool ZoneHVACUnitVentilator::setHeatingConvergenceTolerance(double heatingConvergenceTolerance) {
  return getImpl<detail::ZoneHVACUnitVentilator_Impl>()->setHeatingConvergenceTolerance(heatingConvergenceTolerance);
}

bool ZoneHVACUnitVentilator::setCoolingCoil(const HVACComponent& coolingCoil) {
  return getImpl<detail::ZoneHVACUnitVentilator_Impl>()->setCoolingCoil(coolingCoil);
}

void ZoneHVACUnitVentilator::resetCoolingCoil() {
  getImpl<detail::ZoneHVACUnitVentilator_Impl>()->resetCoolingCoil();
}

bool ZoneHVACUnitVentilator::setCoolingConvergenceTolerance(double coolingConvergenceTolerance) {
  return getImpl<detail::ZoneHVACUnitVentilator_Impl>()->setCoolingConvergenceTolerance(coolingConvergenceTolerance);
}

// bool ZoneHVACUnitVentilator::setAvailabilityManagerList(const ModelObject& systemAvailabilityManagerLists) {
//   return getImpl<detail::ZoneHVACUnitVentilator_Impl>()->setAvailabilityManagerList(systemAvailabilityManagerLists);
// }

// void ZoneHVACUnitVentilator::resetAvailabilityManagerList() {
//   getImpl<detail::ZoneHVACUnitVentilator_Impl>()->resetAvailabilityManagerList();
// }

// bool ZoneHVACUnitVentilator::setDesignSpecificationZoneHVACSizingObject(const ModelObject& designSpecificationZoneHVACSizingName) {
//   return getImpl<detail::ZoneHVACUnitVentilator_Impl>()->setDesignSpecificationZoneHVACSizingObject(designSpecificationZoneHVACSizingName);
// }

// void ZoneHVACUnitVentilator::resetDesignSpecificationZoneHVACSizingObject() {
//   getImpl<detail::ZoneHVACUnitVentilator_Impl>()->resetDesignSpecificationZoneHVACSizingObject();
// }

/// @cond
ZoneHVACUnitVentilator::ZoneHVACUnitVentilator(std::shared_ptr<detail::ZoneHVACUnitVentilator_Impl> impl)
  : ZoneHVACComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

