/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "ZoneHVACFourPipeFanCoil.hpp"
#include "ZoneHVACFourPipeFanCoil_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "HVACComponent.hpp"
#include "HVACComponent_Impl.hpp"
#include "CoilHeatingWater.hpp"
#include "CoilHeatingWater_Impl.hpp"
#include "CoilCoolingWater.hpp"
#include "CoilCoolingWater_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_ZoneHVAC_FourPipeFanCoil_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ZoneHVACFourPipeFanCoil_Impl::ZoneHVACFourPipeFanCoil_Impl(
      const IdfObject& idfObject,
      Model_Impl* model,
      bool keepHandle)
    : ZoneHVACComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ZoneHVACFourPipeFanCoil::iddObjectType());
  }

  ZoneHVACFourPipeFanCoil_Impl::ZoneHVACFourPipeFanCoil_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                             Model_Impl* model,
                                                             bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ZoneHVACFourPipeFanCoil::iddObjectType());
  }

  ZoneHVACFourPipeFanCoil_Impl::ZoneHVACFourPipeFanCoil_Impl(const ZoneHVACFourPipeFanCoil_Impl& other,
                                                             Model_Impl* model,
                                                             bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {}

  ModelObject ZoneHVACFourPipeFanCoil_Impl::clone(Model model) const
  {
    ZoneHVACFourPipeFanCoil fourPipeFanCoilClone = ZoneHVACComponent_Impl::clone(model).cast<ZoneHVACFourPipeFanCoil>();

    HVACComponent supplyFanClone = this->supplyAirFan().clone(model).cast<HVACComponent>();

    auto t_coolingCoil = coolingCoil();
    HVACComponent coolingCoilClone = t_coolingCoil.clone(model).cast<HVACComponent>();

    auto t_heatingCoil = heatingCoil();
    HVACComponent heatingCoilClone = t_heatingCoil.clone(model).cast<HVACComponent>();

    fourPipeFanCoilClone.setSupplyAirFan(supplyFanClone);

    fourPipeFanCoilClone.setHeatingCoil(heatingCoilClone);

    fourPipeFanCoilClone.setCoolingCoil(coolingCoilClone);

    if( model == this->model() ) {
      if( auto waterToAirComponent = t_coolingCoil.optionalCast<WaterToAirComponent>() ) {
        if( auto plant = waterToAirComponent->plantLoop() ) {
          plant->addDemandBranchForComponent(coolingCoilClone);
        }
      }
      if( auto waterToAirComponent = t_heatingCoil.optionalCast<WaterToAirComponent>() ) {
        if( auto plant = waterToAirComponent->plantLoop() ) {
          plant->addDemandBranchForComponent(heatingCoilClone);
        }
      }
    }

    return fourPipeFanCoilClone;
  }

  std::vector<IdfObject> ZoneHVACFourPipeFanCoil_Impl::remove()
  {
    // Careful here, remove could be called during the Ctor, and the coils might not be set actually!
    if (boost::optional<HVACComponent> _hc = optionalHeatingCoil()) {
      if (boost::optional<CoilHeatingWater> waterHeatingCoil = _hc->optionalCast<CoilHeatingWater>()) {
        if (boost::optional<PlantLoop> plantLoop = waterHeatingCoil->plantLoop()) {
          plantLoop->removeDemandBranchWithComponent( waterHeatingCoil.get() );
        }
      }
    }

    if (boost::optional<HVACComponent> _cc = optionalCoolingCoil()) {
      if (boost::optional<CoilCoolingWater> waterCoolingCoil = _cc->optionalCast<CoilCoolingWater>()) {
        if (boost::optional<PlantLoop> plantLoop = waterCoolingCoil->plantLoop()) {
          plantLoop->removeDemandBranchWithComponent( waterCoolingCoil.get() );
        }
      }
    }
    return ZoneHVACComponent_Impl::remove();
  }

  const std::vector<std::string>& ZoneHVACFourPipeFanCoil_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result{
      "Fan Coil Heating Rate",
      "Fan Coil Heating Energy",
      "Fan Coil Total Cooling Rate",
      "Fan Coil Total Cooling Energy",
      "Fan Coil Sensible Cooling Rate",
      "Fan Coil Sensible Cooling Energy",
      "Fan Coil Fan Electric Power",
      "Fan Coil Fan Electric Energy",
      "Fan Coil Runtime Fraction",
      "Fan Coil Fan Speed Level",
      "Fan Coil Part Load Ratio",
      "Fan Coil Availability Status",
      "Fan Coil Part Load Ratio",
      "Fan Coil Speed Ratio"
    };
    return result;
  }

  IddObjectType ZoneHVACFourPipeFanCoil_Impl::iddObjectType() const {
    return ZoneHVACFourPipeFanCoil::iddObjectType();
  }

  std::vector<ScheduleTypeKey> ZoneHVACFourPipeFanCoil_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_ZoneHVAC_FourPipeFanCoilFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneHVACFourPipeFanCoil","Availability"));
    }
    if (std::find(b,e,OS_ZoneHVAC_FourPipeFanCoilFields::OutdoorAirScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneHVACFourPipeFanCoil","Outdoor Air"));
    }
    if (std::find(b,e,OS_ZoneHVAC_FourPipeFanCoilFields::SupplyAirFanOperatingModeScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneHVACFourPipeFanCoil","Supply Air Fan Operating Mode"));
    }
    return result;
  }

  std::vector<ModelObject> ZoneHVACFourPipeFanCoil_Impl::children() const
  {
    std::vector<ModelObject> result;
    if (OptionalHVACComponent intermediate = optionalSupplyAirFan()) {
      result.push_back(*intermediate);
    }
    if (OptionalHVACComponent intermediate = optionalCoolingCoil()) {
      result.push_back(*intermediate);
    }
    if (OptionalHVACComponent intermediate = optionalHeatingCoil()) {
      result.push_back(*intermediate);
    }
    return result;
  }

  unsigned ZoneHVACFourPipeFanCoil_Impl::inletPort() const
  {
    return OS_ZoneHVAC_FourPipeFanCoilFields::AirInletNodeName;
  }

  unsigned ZoneHVACFourPipeFanCoil_Impl::outletPort() const
  {
    return OS_ZoneHVAC_FourPipeFanCoilFields::AirOutletNodeName;
  }

  Schedule ZoneHVACFourPipeFanCoil_Impl::availabilitySchedule() const
  {
    boost::optional<Schedule> value = optionalAvailabilitySchedule();
    if (!value){
      // it is an error if we get here, however we don't want to crash
      // so we hook up to global always on schedule
      LOG(Error, "Required availability schedule not set, using 'Always On' schedule");
      value = this->model().alwaysOnDiscreteSchedule();
      OS_ASSERT(value);
      const_cast<ZoneHVACFourPipeFanCoil_Impl*>(this)->setAvailabilitySchedule(*value);
      value = optionalAvailabilitySchedule();
    }
    OS_ASSERT(value);
    return value.get();
  }

  std::string ZoneHVACFourPipeFanCoil_Impl::capacityControlMethod() const {
    boost::optional<std::string> value = getString(OS_ZoneHVAC_FourPipeFanCoilFields::CapacityControlMethod,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> ZoneHVACFourPipeFanCoil_Impl::maximumSupplyAirFlowRate() const {
    return getDouble(OS_ZoneHVAC_FourPipeFanCoilFields::MaximumSupplyAirFlowRate,true);
  }

  bool ZoneHVACFourPipeFanCoil_Impl::isMaximumSupplyAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ZoneHVAC_FourPipeFanCoilFields::MaximumSupplyAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double ZoneHVACFourPipeFanCoil_Impl::lowSpeedSupplyAirFlowRatio() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_FourPipeFanCoilFields::LowSpeedSupplyAirFlowRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ZoneHVACFourPipeFanCoil_Impl::isLowSpeedSupplyAirFlowRatioDefaulted() const {
    return isEmpty(OS_ZoneHVAC_FourPipeFanCoilFields::LowSpeedSupplyAirFlowRatio);
  }

  double ZoneHVACFourPipeFanCoil_Impl::mediumSpeedSupplyAirFlowRatio() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_FourPipeFanCoilFields::MediumSpeedSupplyAirFlowRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ZoneHVACFourPipeFanCoil_Impl::isMediumSpeedSupplyAirFlowRatioDefaulted() const {
    return isEmpty(OS_ZoneHVAC_FourPipeFanCoilFields::MediumSpeedSupplyAirFlowRatio);
  }

  boost::optional<double> ZoneHVACFourPipeFanCoil_Impl::maximumOutdoorAirFlowRate() const {
    return getDouble(OS_ZoneHVAC_FourPipeFanCoilFields::MaximumOutdoorAirFlowRate,true);
  }

  bool ZoneHVACFourPipeFanCoil_Impl::isMaximumOutdoorAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ZoneHVAC_FourPipeFanCoilFields::MaximumOutdoorAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<Schedule> ZoneHVACFourPipeFanCoil_Impl::outdoorAirSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneHVAC_FourPipeFanCoilFields::OutdoorAirScheduleName);
  }

  std::string ZoneHVACFourPipeFanCoil_Impl::outdoorAirMixerObjectType() const {
    boost::optional<std::string> value = getString(OS_ZoneHVAC_FourPipeFanCoilFields::OutdoorAirMixerObjectType,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string ZoneHVACFourPipeFanCoil_Impl::outdoorAirMixerName() const {
    boost::optional<std::string> value = getString(OS_ZoneHVAC_FourPipeFanCoilFields::OutdoorAirMixerName,true);
    OS_ASSERT(value);
    return value.get();
  }

  HVACComponent ZoneHVACFourPipeFanCoil_Impl::supplyAirFan() const
  {
    boost::optional<HVACComponent> fan = optionalSupplyAirFan();
    OS_ASSERT(fan);
    return fan.get();
  }

  HVACComponent ZoneHVACFourPipeFanCoil_Impl::coolingCoil() const
  {
    boost::optional<HVACComponent> coil = optionalCoolingCoil();
    OS_ASSERT(coil);
    return coil.get();
  }

  boost::optional<double> ZoneHVACFourPipeFanCoil_Impl::maximumColdWaterFlowRate() const {
    return getDouble(OS_ZoneHVAC_FourPipeFanCoilFields::MaximumColdWaterFlowRate,true);
  }

  bool ZoneHVACFourPipeFanCoil_Impl::isMaximumColdWaterFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ZoneHVAC_FourPipeFanCoilFields::MaximumColdWaterFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double ZoneHVACFourPipeFanCoil_Impl::minimumColdWaterFlowRate() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_FourPipeFanCoilFields::MinimumColdWaterFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ZoneHVACFourPipeFanCoil_Impl::isMinimumColdWaterFlowRateDefaulted() const {
    return isEmpty(OS_ZoneHVAC_FourPipeFanCoilFields::MinimumColdWaterFlowRate);
  }

  double ZoneHVACFourPipeFanCoil_Impl::coolingConvergenceTolerance() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_FourPipeFanCoilFields::CoolingConvergenceTolerance,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ZoneHVACFourPipeFanCoil_Impl::isCoolingConvergenceToleranceDefaulted() const {
    return isEmpty(OS_ZoneHVAC_FourPipeFanCoilFields::CoolingConvergenceTolerance);
  }

  HVACComponent ZoneHVACFourPipeFanCoil_Impl::heatingCoil() const
  {
    boost::optional<HVACComponent> coil = optionalHeatingCoil();
    OS_ASSERT(coil);
    return coil.get();
  }

  boost::optional<double> ZoneHVACFourPipeFanCoil_Impl::maximumHotWaterFlowRate() const {
    return getDouble(OS_ZoneHVAC_FourPipeFanCoilFields::MaximumHotWaterFlowRate,true);
  }

  bool ZoneHVACFourPipeFanCoil_Impl::isMaximumHotWaterFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ZoneHVAC_FourPipeFanCoilFields::MaximumHotWaterFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double ZoneHVACFourPipeFanCoil_Impl::minimumHotWaterFlowRate() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_FourPipeFanCoilFields::MinimumHotWaterFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ZoneHVACFourPipeFanCoil_Impl::isMinimumHotWaterFlowRateDefaulted() const {
    return isEmpty(OS_ZoneHVAC_FourPipeFanCoilFields::MinimumHotWaterFlowRate);
  }

  double ZoneHVACFourPipeFanCoil_Impl::heatingConvergenceTolerance() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_FourPipeFanCoilFields::HeatingConvergenceTolerance,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ZoneHVACFourPipeFanCoil_Impl::isHeatingConvergenceToleranceDefaulted() const {
    return isEmpty(OS_ZoneHVAC_FourPipeFanCoilFields::HeatingConvergenceTolerance);
  }

  bool ZoneHVACFourPipeFanCoil_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ZoneHVAC_FourPipeFanCoilFields::AvailabilityScheduleName,
                              "ZoneHVACFourPipeFanCoil",
                              "Availability",
                              schedule);
    return result;
  }

  bool ZoneHVACFourPipeFanCoil_Impl::setCapacityControlMethod(std::string capacityControlMethod) {
    bool result = setString(OS_ZoneHVAC_FourPipeFanCoilFields::CapacityControlMethod, capacityControlMethod);
    return result;
  }

  bool ZoneHVACFourPipeFanCoil_Impl::setMaximumSupplyAirFlowRate(boost::optional<double> maximumSupplyAirFlowRate) {
    bool result(false);
    if (maximumSupplyAirFlowRate) {
      result = setDouble(OS_ZoneHVAC_FourPipeFanCoilFields::MaximumSupplyAirFlowRate, maximumSupplyAirFlowRate.get());
    }
    OS_ASSERT(result);
    return result;
  }

  void ZoneHVACFourPipeFanCoil_Impl::autosizeMaximumSupplyAirFlowRate() {
    bool result = setString(OS_ZoneHVAC_FourPipeFanCoilFields::MaximumSupplyAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool ZoneHVACFourPipeFanCoil_Impl::setLowSpeedSupplyAirFlowRatio(double lowSpeedSupplyAirFlowRatio) {
    bool result = setDouble(OS_ZoneHVAC_FourPipeFanCoilFields::LowSpeedSupplyAirFlowRatio, lowSpeedSupplyAirFlowRatio);
    return result;
  }

  void ZoneHVACFourPipeFanCoil_Impl::resetLowSpeedSupplyAirFlowRatio() {
    bool result = setString(OS_ZoneHVAC_FourPipeFanCoilFields::LowSpeedSupplyAirFlowRatio, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACFourPipeFanCoil_Impl::setMediumSpeedSupplyAirFlowRatio(double mediumSpeedSupplyAirFlowRatio) {
    bool result = setDouble(OS_ZoneHVAC_FourPipeFanCoilFields::MediumSpeedSupplyAirFlowRatio, mediumSpeedSupplyAirFlowRatio);
    return result;
  }

  void ZoneHVACFourPipeFanCoil_Impl::resetMediumSpeedSupplyAirFlowRatio() {
    bool result = setString(OS_ZoneHVAC_FourPipeFanCoilFields::MediumSpeedSupplyAirFlowRatio, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACFourPipeFanCoil_Impl::setMaximumOutdoorAirFlowRate(boost::optional<double> maximumOutdoorAirFlowRate) {
    bool result(false);
    if (maximumOutdoorAirFlowRate) {
      result = setDouble(OS_ZoneHVAC_FourPipeFanCoilFields::MaximumOutdoorAirFlowRate, maximumOutdoorAirFlowRate.get());
    }
    OS_ASSERT(result);
    return result;
  }

  void ZoneHVACFourPipeFanCoil_Impl::autosizeMaximumOutdoorAirFlowRate() {
    bool result = setString(OS_ZoneHVAC_FourPipeFanCoilFields::MaximumOutdoorAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool ZoneHVACFourPipeFanCoil_Impl::setOutdoorAirSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ZoneHVAC_FourPipeFanCoilFields::OutdoorAirScheduleName,
                              "ZoneHVACFourPipeFanCoil",
                              "Outdoor Air",
                              schedule);
    return result;
  }

  void ZoneHVACFourPipeFanCoil_Impl::resetOutdoorAirSchedule() {
    bool result = setString(OS_ZoneHVAC_FourPipeFanCoilFields::OutdoorAirScheduleName, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACFourPipeFanCoil_Impl::setOutdoorAirMixerObjectType(std::string outdoorAirMixerObjectType) {
    bool result = setString(OS_ZoneHVAC_FourPipeFanCoilFields::OutdoorAirMixerObjectType, outdoorAirMixerObjectType);
    return result;
  }

  bool ZoneHVACFourPipeFanCoil_Impl::setOutdoorAirMixerName(std::string outdoorAirMixerName) {
    bool result = setString(OS_ZoneHVAC_FourPipeFanCoilFields::OutdoorAirMixerName, outdoorAirMixerName);
    OS_ASSERT(result);
    return result;
  }

  bool ZoneHVACFourPipeFanCoil_Impl::setSupplyAirFan( HVACComponent & fan )
  {
    bool isAllowedType = false;


    if( istringEqual(capacityControlMethod(),"ConstantFanVariableFlow"))
    {
      if( fan.iddObjectType() == IddObjectType::OS_Fan_ConstantVolume || fan.iddObjectType() == IddObjectType::OS_Fan_OnOff)
      {
        isAllowedType = true;
      }
    }
    else if( istringEqual(capacityControlMethod(),"CyclingFan"))
    {
      if( fan.iddObjectType() == IddObjectType::OS_Fan_OnOff)
      {
        isAllowedType = true;
      }
    }
    else if( istringEqual(capacityControlMethod(),"VariableFanVariableFlow") || istringEqual(capacityControlMethod(),"VariableFanConstantFlow"))
    {
      if( fan.iddObjectType() == IddObjectType::OS_Fan_VariableVolume)
      {
        isAllowedType = true;
      }
    }

    if( isAllowedType )
    {
      return setPointer(OS_ZoneHVAC_FourPipeFanCoilFields::SupplyAirFanName,fan.handle());
    }

    return false;
  }

  bool ZoneHVACFourPipeFanCoil_Impl::setCoolingCoil( HVACComponent & coolingCoil )
  {
    bool isAllowedType = false;

    if( coolingCoil.iddObjectType() == IddObjectType::OS_Coil_Cooling_Water )
    {
      isAllowedType = true;
    }

    if( isAllowedType )
    {
      return setPointer(OS_ZoneHVAC_FourPipeFanCoilFields::CoolingCoilName,coolingCoil.handle());
    }

    return false;
  }

  bool ZoneHVACFourPipeFanCoil_Impl::setMaximumColdWaterFlowRate(boost::optional<double> maximumColdWaterFlowRate) {
    bool result(false);
    if (maximumColdWaterFlowRate) {
      result = setDouble(OS_ZoneHVAC_FourPipeFanCoilFields::MaximumColdWaterFlowRate, maximumColdWaterFlowRate.get());
    }
    OS_ASSERT(result);
    return result;
  }

  void ZoneHVACFourPipeFanCoil_Impl::autosizeMaximumColdWaterFlowRate() {
    bool result = setString(OS_ZoneHVAC_FourPipeFanCoilFields::MaximumColdWaterFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool ZoneHVACFourPipeFanCoil_Impl::setMinimumColdWaterFlowRate(double minimumColdWaterFlowRate) {
    bool result = setDouble(OS_ZoneHVAC_FourPipeFanCoilFields::MinimumColdWaterFlowRate, minimumColdWaterFlowRate);
    OS_ASSERT(result);
    return result;
  }

  void ZoneHVACFourPipeFanCoil_Impl::resetMinimumColdWaterFlowRate() {
    bool result = setString(OS_ZoneHVAC_FourPipeFanCoilFields::MinimumColdWaterFlowRate, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACFourPipeFanCoil_Impl::setCoolingConvergenceTolerance(double coolingConvergenceTolerance) {
    bool result = setDouble(OS_ZoneHVAC_FourPipeFanCoilFields::CoolingConvergenceTolerance, coolingConvergenceTolerance);
    return result;
  }

  void ZoneHVACFourPipeFanCoil_Impl::resetCoolingConvergenceTolerance() {
    bool result = setString(OS_ZoneHVAC_FourPipeFanCoilFields::CoolingConvergenceTolerance, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACFourPipeFanCoil_Impl::setHeatingCoil( HVACComponent & heatingCoil )
  {
    bool result = false;

    // Only accept if type is allowed
    if ( ( heatingCoil.iddObjectType() == IddObjectType::OS_Coil_Heating_Water ) ||
         ( heatingCoil.iddObjectType() == IddObjectType::OS_Coil_Heating_Electric ) ) {
      result = setPointer(OS_ZoneHVAC_FourPipeFanCoilFields::HeatingCoilName, heatingCoil.handle());
    }

    return result;
  }

  bool ZoneHVACFourPipeFanCoil_Impl::setMaximumHotWaterFlowRate(boost::optional<double> maximumHotWaterFlowRate) {
    bool result(false);
    if (maximumHotWaterFlowRate) {
      result = setDouble(OS_ZoneHVAC_FourPipeFanCoilFields::MaximumHotWaterFlowRate, maximumHotWaterFlowRate.get());
    }
    OS_ASSERT(result);
    return result;
  }

  void ZoneHVACFourPipeFanCoil_Impl::autosizeMaximumHotWaterFlowRate() {
    bool result = setString(OS_ZoneHVAC_FourPipeFanCoilFields::MaximumHotWaterFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool ZoneHVACFourPipeFanCoil_Impl::setMinimumHotWaterFlowRate(double minimumHotWaterFlowRate) {
    bool result = setDouble(OS_ZoneHVAC_FourPipeFanCoilFields::MinimumHotWaterFlowRate, minimumHotWaterFlowRate);
    OS_ASSERT(result);
    return result;
  }

  void ZoneHVACFourPipeFanCoil_Impl::resetMinimumHotWaterFlowRate() {
    bool result = setString(OS_ZoneHVAC_FourPipeFanCoilFields::MinimumHotWaterFlowRate, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACFourPipeFanCoil_Impl::setHeatingConvergenceTolerance(double heatingConvergenceTolerance) {
    bool result = setDouble(OS_ZoneHVAC_FourPipeFanCoilFields::HeatingConvergenceTolerance, heatingConvergenceTolerance);
    return result;
  }

  void ZoneHVACFourPipeFanCoil_Impl::resetHeatingConvergenceTolerance() {
    bool result = setString(OS_ZoneHVAC_FourPipeFanCoilFields::HeatingConvergenceTolerance, "");
    OS_ASSERT(result);
  }

  std::vector<std::string> ZoneHVACFourPipeFanCoil_Impl::capacityControlMethodValues() const {
    return ZoneHVACFourPipeFanCoil::capacityControlMethodValues();
  }

  std::vector<std::string> ZoneHVACFourPipeFanCoil_Impl::outdoorAirMixerObjectTypeValues() const {
    return ZoneHVACFourPipeFanCoil::outdoorAirMixerObjectTypeValues();
  }

  boost::optional<Schedule> ZoneHVACFourPipeFanCoil_Impl::optionalAvailabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneHVAC_FourPipeFanCoilFields::AvailabilityScheduleName);
  }
  boost::optional<HVACComponent> ZoneHVACFourPipeFanCoil_Impl::optionalSupplyAirFan() const {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_ZoneHVAC_FourPipeFanCoilFields::SupplyAirFanName);
  }
  boost::optional<HVACComponent> ZoneHVACFourPipeFanCoil_Impl::optionalHeatingCoil() const {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_ZoneHVAC_FourPipeFanCoilFields::HeatingCoilName);
  }
  boost::optional<HVACComponent> ZoneHVACFourPipeFanCoil_Impl::optionalCoolingCoil() const {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_ZoneHVAC_FourPipeFanCoilFields::CoolingCoilName);
  }

  boost::optional<ModelObject> ZoneHVACFourPipeFanCoil_Impl::availabilityScheduleAsModelObject() const {
    OptionalModelObject result = availabilitySchedule();
    return result;
  }

  boost::optional<ModelObject> ZoneHVACFourPipeFanCoil_Impl::outdoorAirScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = outdoorAirSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> ZoneHVACFourPipeFanCoil_Impl::supplyAirFanAsModelObject() const {
    OptionalModelObject result = supplyAirFan();
    return result;
  }

  boost::optional<ModelObject> ZoneHVACFourPipeFanCoil_Impl::coolingCoilAsModelObject() const {
    OptionalModelObject result = coolingCoil();
    return result;
  }

  boost::optional<ModelObject> ZoneHVACFourPipeFanCoil_Impl::heatingCoilAsModelObject() const {
    OptionalModelObject result = heatingCoil();
    return result;
  }

  bool ZoneHVACFourPipeFanCoil_Impl::setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setAvailabilitySchedule(schedule);
      }
    }
    return false;
  }

  bool ZoneHVACFourPipeFanCoil_Impl::setOutdoorAirScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setOutdoorAirSchedule(schedule);
      }
      else {
        return false;
      }
    }
    else {
      resetOutdoorAirSchedule();
    }
    return true;
  }

  bool ZoneHVACFourPipeFanCoil_Impl::setSupplyAirFanAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalHVACComponent intermediate = modelObject->optionalCast<HVACComponent>();
      if (intermediate) {
        setSupplyAirFan(*intermediate);
        return true;
      }
    }
    return false;
  }

  bool ZoneHVACFourPipeFanCoil_Impl::setCoolingCoilAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalHVACComponent intermediate = modelObject->optionalCast<HVACComponent>();
      if (intermediate) {
        setCoolingCoil(*intermediate);
        return true;
      }
    }
    return false;
  }

  bool ZoneHVACFourPipeFanCoil_Impl::setHeatingCoilAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalHVACComponent intermediate = modelObject->optionalCast<HVACComponent>();
      if (intermediate) {
        setHeatingCoil(*intermediate);
        return true;
      }
    }
    return false;
  }

  bool ZoneHVACFourPipeFanCoil_Impl::setSupplyAirFanOperatingModeSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ZoneHVAC_FourPipeFanCoilFields::SupplyAirFanOperatingModeScheduleName,
                              "ZoneHVACFourPipeFanCoil",
                              "Supply Air Fan Operating Mode",
                              schedule);
    return result;
  }

  void ZoneHVACFourPipeFanCoil_Impl::resetSupplyAirFanOperatingModeSchedule() {
    bool result = setString(OS_ZoneHVAC_FourPipeFanCoilFields::SupplyAirFanOperatingModeScheduleName, "");
    OS_ASSERT(result);
  }

  boost::optional<Schedule> ZoneHVACFourPipeFanCoil_Impl::supplyAirFanOperatingModeSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(
             OS_ZoneHVAC_FourPipeFanCoilFields::SupplyAirFanOperatingModeScheduleName);
  }

  boost::optional<double> ZoneHVACFourPipeFanCoil_Impl::minimumSupplyAirTemperatureInCoolingMode() const {
    return getDouble(OS_ZoneHVAC_FourPipeFanCoilFields::MinimumSupplyAirTemperatureinCoolingMode,true);
  }

  bool ZoneHVACFourPipeFanCoil_Impl::isMinimumSupplyAirTemperatureInCoolingModeAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ZoneHVAC_FourPipeFanCoilFields::MinimumSupplyAirTemperatureinCoolingMode, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  void ZoneHVACFourPipeFanCoil_Impl::autosizeMinimumSupplyAirTemperatureInCoolingMode() {
    bool result = setString(OS_ZoneHVAC_FourPipeFanCoilFields::MinimumSupplyAirTemperatureinCoolingMode, "autosize");
    OS_ASSERT(result);
  }

  bool ZoneHVACFourPipeFanCoil_Impl::setMinimumSupplyAirTemperatureInCoolingMode(double minimumSupplyAirTemperatureInCoolingMode) {
    return setDouble(OS_ZoneHVAC_FourPipeFanCoilFields::MinimumSupplyAirTemperatureinCoolingMode,minimumSupplyAirTemperatureInCoolingMode);
  }

  boost::optional<double> ZoneHVACFourPipeFanCoil_Impl::maximumSupplyAirTemperatureInHeatingMode() const {
    return getDouble(OS_ZoneHVAC_FourPipeFanCoilFields::MaximumSupplyAirTemperatureinHeatingMode,true);
  }

  bool ZoneHVACFourPipeFanCoil_Impl::isMaximumSupplyAirTemperatureInHeatingModeAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ZoneHVAC_FourPipeFanCoilFields::MaximumSupplyAirTemperatureinHeatingMode, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  void ZoneHVACFourPipeFanCoil_Impl::autosizeMaximumSupplyAirTemperatureInHeatingMode() {
    bool result = setString(OS_ZoneHVAC_FourPipeFanCoilFields::MaximumSupplyAirTemperatureinHeatingMode, "autosize");
    OS_ASSERT(result);
  }

  bool ZoneHVACFourPipeFanCoil_Impl::setMaximumSupplyAirTemperatureInHeatingMode(double maximumSupplyAirTemperatureInHeatingMode) {
    return setDouble(OS_ZoneHVAC_FourPipeFanCoilFields::MaximumSupplyAirTemperatureinHeatingMode,maximumSupplyAirTemperatureInHeatingMode);
  }

  boost::optional<double> ZoneHVACFourPipeFanCoil_Impl::autosizedMaximumSupplyAirFlowRate() const {
    return getAutosizedValue("Design Size Maximum Supply Air Flow Rate", "m3/s");
  }

  boost::optional<double> ZoneHVACFourPipeFanCoil_Impl::autosizedMaximumOutdoorAirFlowRate() const {
    return getAutosizedValue("Design Size Maximum Outdoor Air Flow Rate", "m3/s");
  }

  boost::optional<double> ZoneHVACFourPipeFanCoil_Impl::autosizedMaximumColdWaterFlowRate() const {
    return getAutosizedValue("Design Size Maximum Cold Water Flow", "m3/s");
  }

  boost::optional<double> ZoneHVACFourPipeFanCoil_Impl::autosizedMaximumHotWaterFlowRate() const {
    return getAutosizedValue("Design Size Maximum Hot Water Flow", "m3/s");
  }

  boost::optional<double> ZoneHVACFourPipeFanCoil_Impl::autosizedMinimumSupplyAirTemperatureinCoolingMode() const {
    return getAutosizedValue("Design Size Minimum Supply Air Temperature in Cooling Mode", "C");
  }

  boost::optional<double> ZoneHVACFourPipeFanCoil_Impl::autosizedMaximumSupplyAirTemperatureinHeatingMode() const {
    return getAutosizedValue("Design Size Maximum Supply Air Temperature in Heating Mode", "C");
  }

  void ZoneHVACFourPipeFanCoil_Impl::autosize() {
    autosizeMaximumSupplyAirFlowRate();
    autosizeMaximumOutdoorAirFlowRate();
    autosizeMaximumColdWaterFlowRate();
    autosizeMaximumHotWaterFlowRate();
    autosizeMinimumSupplyAirTemperatureInCoolingMode();
    autosizeMaximumSupplyAirTemperatureInHeatingMode();
  }

  void ZoneHVACFourPipeFanCoil_Impl::applySizingValues() {
    boost::optional<double> val;
    val = autosizedMaximumSupplyAirFlowRate();
    if (val) {
      setMaximumSupplyAirFlowRate(val.get());
    }

    val = autosizedMaximumOutdoorAirFlowRate();
    if (val) {
      setMaximumOutdoorAirFlowRate(val.get());
    }

    val = autosizedMaximumColdWaterFlowRate();
    if (val) {
      setMaximumColdWaterFlowRate(val.get());
    }

    val = autosizedMaximumHotWaterFlowRate();
    if (val) {
      setMaximumHotWaterFlowRate(val.get());
    }

    val = autosizedMinimumSupplyAirTemperatureinCoolingMode();
    if (val) {
      setMinimumSupplyAirTemperatureInCoolingMode(val.get());
    }

    val = autosizedMaximumSupplyAirTemperatureinHeatingMode();
    if (val) {
      setMaximumSupplyAirTemperatureInHeatingMode(val.get());
    }

  }

} // detail

ZoneHVACFourPipeFanCoil::ZoneHVACFourPipeFanCoil(const Model& model,
                                                 Schedule & availabilitySchedule,
                                                 HVACComponent & supplyAirFan,
                                                 HVACComponent & coolingCoil,
                                                 HVACComponent & heatingCoil )
  : ZoneHVACComponent(ZoneHVACFourPipeFanCoil::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>());

  bool ok = setAvailabilitySchedule(availabilitySchedule);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s availability schedule to "
                  << availabilitySchedule.briefDescription() << ".");
  }

  if ( supplyAirFan.iddObjectType() == IddObjectType::OS_Fan_ConstantVolume)
  {
    setCapacityControlMethod("ConstantFanVariableFlow");
  }

  else if ( supplyAirFan.iddObjectType() == IddObjectType::OS_Fan_OnOff)
  {
    setCapacityControlMethod("CyclingFan");
  }

  else if ( supplyAirFan.iddObjectType() == IddObjectType::OS_Fan_VariableVolume)
  {
    setCapacityControlMethod("VariableFanVariableFlow");
  }

  resetOutdoorAirSchedule();
  setOutdoorAirMixerObjectType("OutdoorAir:Mixer");
  ok = setSupplyAirFan(supplyAirFan);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s supply air fan to "
                  << supplyAirFan.briefDescription() << ".");
  }
  ok = setHeatingCoil(heatingCoil);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s heating coil to "
                  << heatingCoil.briefDescription() << ".");
  }
  ok = setCoolingCoil(coolingCoil);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s cooling coil to "
                  << coolingCoil.briefDescription() << ".");
  }
  autosizeMaximumSupplyAirFlowRate();
  autosizeMaximumOutdoorAirFlowRate();
  autosizeMaximumColdWaterFlowRate();
  autosizeMaximumHotWaterFlowRate();
  autosizeMinimumSupplyAirTemperatureInCoolingMode();
  autosizeMaximumSupplyAirTemperatureInHeatingMode();
}

IddObjectType ZoneHVACFourPipeFanCoil::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ZoneHVAC_FourPipeFanCoil);
}

std::vector<std::string> ZoneHVACFourPipeFanCoil::capacityControlMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ZoneHVAC_FourPipeFanCoilFields::CapacityControlMethod);
}

std::vector<std::string> ZoneHVACFourPipeFanCoil::outdoorAirMixerObjectTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ZoneHVAC_FourPipeFanCoilFields::OutdoorAirMixerObjectType);
}

Schedule ZoneHVACFourPipeFanCoil::availabilitySchedule() const {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->availabilitySchedule();
}

std::string ZoneHVACFourPipeFanCoil::capacityControlMethod() const {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->capacityControlMethod();
}

boost::optional<double> ZoneHVACFourPipeFanCoil::maximumSupplyAirFlowRate() const {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->maximumSupplyAirFlowRate();
}

bool ZoneHVACFourPipeFanCoil::isMaximumSupplyAirFlowRateAutosized() const {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->isMaximumSupplyAirFlowRateAutosized();
}

double ZoneHVACFourPipeFanCoil::lowSpeedSupplyAirFlowRatio() const {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->lowSpeedSupplyAirFlowRatio();
}

bool ZoneHVACFourPipeFanCoil::isLowSpeedSupplyAirFlowRatioDefaulted() const {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->isLowSpeedSupplyAirFlowRatioDefaulted();
}

double ZoneHVACFourPipeFanCoil::mediumSpeedSupplyAirFlowRatio() const {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->mediumSpeedSupplyAirFlowRatio();
}

bool ZoneHVACFourPipeFanCoil::isMediumSpeedSupplyAirFlowRatioDefaulted() const {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->isMediumSpeedSupplyAirFlowRatioDefaulted();
}

boost::optional<double> ZoneHVACFourPipeFanCoil::maximumOutdoorAirFlowRate() const {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->maximumOutdoorAirFlowRate();
}

bool ZoneHVACFourPipeFanCoil::isMaximumOutdoorAirFlowRateAutosized() const {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->isMaximumOutdoorAirFlowRateAutosized();
}

boost::optional<Schedule> ZoneHVACFourPipeFanCoil::outdoorAirSchedule() const {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->outdoorAirSchedule();
}

std::string ZoneHVACFourPipeFanCoil::outdoorAirMixerObjectType() const {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->outdoorAirMixerObjectType();
}

std::string ZoneHVACFourPipeFanCoil::outdoorAirMixerName() const {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->outdoorAirMixerName();
}

HVACComponent ZoneHVACFourPipeFanCoil::supplyAirFan() const
{
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->supplyAirFan();
}

HVACComponent ZoneHVACFourPipeFanCoil::coolingCoil() const
{
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->coolingCoil();
}

boost::optional<double> ZoneHVACFourPipeFanCoil::maximumColdWaterFlowRate() const {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->maximumColdWaterFlowRate();
}

bool ZoneHVACFourPipeFanCoil::isMaximumColdWaterFlowRateAutosized() const {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->isMaximumColdWaterFlowRateAutosized();
}

double ZoneHVACFourPipeFanCoil::minimumColdWaterFlowRate() const {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->minimumColdWaterFlowRate();
}

bool ZoneHVACFourPipeFanCoil::isMinimumColdWaterFlowRateDefaulted() const {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->isMinimumColdWaterFlowRateDefaulted();
}

double ZoneHVACFourPipeFanCoil::coolingConvergenceTolerance() const {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->coolingConvergenceTolerance();
}

bool ZoneHVACFourPipeFanCoil::isCoolingConvergenceToleranceDefaulted() const {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->isCoolingConvergenceToleranceDefaulted();
}

HVACComponent ZoneHVACFourPipeFanCoil::heatingCoil() const
{
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->heatingCoil();
}

boost::optional<double> ZoneHVACFourPipeFanCoil::maximumHotWaterFlowRate() const {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->maximumHotWaterFlowRate();
}

bool ZoneHVACFourPipeFanCoil::isMaximumHotWaterFlowRateAutosized() const {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->isMaximumHotWaterFlowRateAutosized();
}

double ZoneHVACFourPipeFanCoil::minimumHotWaterFlowRate() const {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->minimumHotWaterFlowRate();
}

bool ZoneHVACFourPipeFanCoil::isMinimumHotWaterFlowRateDefaulted() const {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->isMinimumHotWaterFlowRateDefaulted();
}

double ZoneHVACFourPipeFanCoil::heatingConvergenceTolerance() const {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->heatingConvergenceTolerance();
}

bool ZoneHVACFourPipeFanCoil::isHeatingConvergenceToleranceDefaulted() const {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->isHeatingConvergenceToleranceDefaulted();
}

bool ZoneHVACFourPipeFanCoil::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->setAvailabilitySchedule(schedule);
}

bool ZoneHVACFourPipeFanCoil::setCapacityControlMethod(std::string capacityControlMethod) {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->setCapacityControlMethod(capacityControlMethod);
}

bool ZoneHVACFourPipeFanCoil::setMaximumSupplyAirFlowRate(double maximumSupplyAirFlowRate) {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->setMaximumSupplyAirFlowRate(maximumSupplyAirFlowRate);
}

void ZoneHVACFourPipeFanCoil::autosizeMaximumSupplyAirFlowRate() {
  getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->autosizeMaximumSupplyAirFlowRate();
}

bool ZoneHVACFourPipeFanCoil::setLowSpeedSupplyAirFlowRatio(double lowSpeedSupplyAirFlowRatio) {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->setLowSpeedSupplyAirFlowRatio(lowSpeedSupplyAirFlowRatio);
}

void ZoneHVACFourPipeFanCoil::resetLowSpeedSupplyAirFlowRatio() {
  getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->resetLowSpeedSupplyAirFlowRatio();
}

bool ZoneHVACFourPipeFanCoil::setMediumSpeedSupplyAirFlowRatio(double mediumSpeedSupplyAirFlowRatio) {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->setMediumSpeedSupplyAirFlowRatio(mediumSpeedSupplyAirFlowRatio);
}

void ZoneHVACFourPipeFanCoil::resetMediumSpeedSupplyAirFlowRatio() {
  getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->resetMediumSpeedSupplyAirFlowRatio();
}

bool ZoneHVACFourPipeFanCoil::setMaximumOutdoorAirFlowRate(double maximumOutdoorAirFlowRate) {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->setMaximumOutdoorAirFlowRate(maximumOutdoorAirFlowRate);
}

void ZoneHVACFourPipeFanCoil::autosizeMaximumOutdoorAirFlowRate() {
  getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->autosizeMaximumOutdoorAirFlowRate();
}

bool ZoneHVACFourPipeFanCoil::setOutdoorAirSchedule(Schedule& schedule) {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->setOutdoorAirSchedule(schedule);
}

void ZoneHVACFourPipeFanCoil::resetOutdoorAirSchedule() {
  getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->resetOutdoorAirSchedule();
}

bool ZoneHVACFourPipeFanCoil::setOutdoorAirMixerObjectType(std::string outdoorAirMixerObjectType) {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->setOutdoorAirMixerObjectType(outdoorAirMixerObjectType);
}

bool ZoneHVACFourPipeFanCoil::setOutdoorAirMixerName(std::string outdoorAirMixerName) {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->setOutdoorAirMixerName(outdoorAirMixerName);
}

bool ZoneHVACFourPipeFanCoil::setSupplyAirFan( HVACComponent & fan )
{
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->setSupplyAirFan(fan);
}

bool ZoneHVACFourPipeFanCoil::setCoolingCoil( HVACComponent & coolingCoil )
{
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->setCoolingCoil(coolingCoil);
}

bool ZoneHVACFourPipeFanCoil::setMaximumColdWaterFlowRate(double maximumColdWaterFlowRate) {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->setMaximumColdWaterFlowRate(maximumColdWaterFlowRate);
}

void ZoneHVACFourPipeFanCoil::autosizeMaximumColdWaterFlowRate() {
  getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->autosizeMaximumColdWaterFlowRate();
}

bool ZoneHVACFourPipeFanCoil::setMinimumColdWaterFlowRate(double minimumColdWaterFlowRate) {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->setMinimumColdWaterFlowRate(minimumColdWaterFlowRate);
}

void ZoneHVACFourPipeFanCoil::resetMinimumColdWaterFlowRate() {
  getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->resetMinimumColdWaterFlowRate();
}

bool ZoneHVACFourPipeFanCoil::setCoolingConvergenceTolerance(double coolingConvergenceTolerance) {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->setCoolingConvergenceTolerance(coolingConvergenceTolerance);
}

void ZoneHVACFourPipeFanCoil::resetCoolingConvergenceTolerance() {
  getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->resetCoolingConvergenceTolerance();
}

bool ZoneHVACFourPipeFanCoil::setHeatingCoil( HVACComponent & heatingCoil )
{
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->setHeatingCoil(heatingCoil);
}

bool ZoneHVACFourPipeFanCoil::setMaximumHotWaterFlowRate(double maximumHotWaterFlowRate) {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->setMaximumHotWaterFlowRate(maximumHotWaterFlowRate);
}

void ZoneHVACFourPipeFanCoil::autosizeMaximumHotWaterFlowRate() {
  getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->autosizeMaximumHotWaterFlowRate();
}

bool ZoneHVACFourPipeFanCoil::setMinimumHotWaterFlowRate(double minimumHotWaterFlowRate) {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->setMinimumHotWaterFlowRate(minimumHotWaterFlowRate);
}

void ZoneHVACFourPipeFanCoil::resetMinimumHotWaterFlowRate() {
  getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->resetMinimumHotWaterFlowRate();
}

bool ZoneHVACFourPipeFanCoil::setHeatingConvergenceTolerance(double heatingConvergenceTolerance) {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->setHeatingConvergenceTolerance(heatingConvergenceTolerance);
}

void ZoneHVACFourPipeFanCoil::resetHeatingConvergenceTolerance() {
  getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->resetHeatingConvergenceTolerance();
}

bool ZoneHVACFourPipeFanCoil::setSupplyAirFanOperatingModeSchedule(Schedule& schedule) {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->setSupplyAirFanOperatingModeSchedule(schedule);
}

void ZoneHVACFourPipeFanCoil::resetSupplyAirFanOperatingModeSchedule() {
  getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->resetSupplyAirFanOperatingModeSchedule();
}

boost::optional<Schedule> ZoneHVACFourPipeFanCoil::supplyAirFanOperatingModeSchedule() const {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->supplyAirFanOperatingModeSchedule();
}

boost::optional<double> ZoneHVACFourPipeFanCoil::minimumSupplyAirTemperatureInCoolingMode() const {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->minimumSupplyAirTemperatureInCoolingMode();
}

bool ZoneHVACFourPipeFanCoil::isMinimumSupplyAirTemperatureInCoolingModeAutosized() const {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->isMinimumSupplyAirTemperatureInCoolingModeAutosized();
}

void ZoneHVACFourPipeFanCoil::autosizeMinimumSupplyAirTemperatureInCoolingMode() {
  getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->autosizeMinimumSupplyAirTemperatureInCoolingMode();
}

bool ZoneHVACFourPipeFanCoil::setMinimumSupplyAirTemperatureInCoolingMode(double minimumSupplyAirTemperatureInCoolingMode) {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->setMinimumSupplyAirTemperatureInCoolingMode(minimumSupplyAirTemperatureInCoolingMode);
}

boost::optional<double> ZoneHVACFourPipeFanCoil::maximumSupplyAirTemperatureInHeatingMode() const {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->maximumSupplyAirTemperatureInHeatingMode();
}

bool ZoneHVACFourPipeFanCoil::isMaximumSupplyAirTemperatureInHeatingModeAutosized() const {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->isMaximumSupplyAirTemperatureInHeatingModeAutosized();
}

void ZoneHVACFourPipeFanCoil::autosizeMaximumSupplyAirTemperatureInHeatingMode() {
  getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->autosizeMaximumSupplyAirTemperatureInHeatingMode();
}

bool ZoneHVACFourPipeFanCoil::setMaximumSupplyAirTemperatureInHeatingMode(double maximumSupplyAirTemperatureInHeatingMode) {
  return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->setMaximumSupplyAirTemperatureInHeatingMode(maximumSupplyAirTemperatureInHeatingMode);
}

/// @cond
ZoneHVACFourPipeFanCoil::ZoneHVACFourPipeFanCoil(std::shared_ptr<detail::ZoneHVACFourPipeFanCoil_Impl> impl)
  : ZoneHVACComponent(std::move(impl))
{}
/// @endcond

  boost::optional<double> ZoneHVACFourPipeFanCoil::autosizedMaximumSupplyAirFlowRate() const {
    return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->autosizedMaximumSupplyAirFlowRate();
  }

  boost::optional<double> ZoneHVACFourPipeFanCoil::autosizedMaximumOutdoorAirFlowRate() const {
    return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->autosizedMaximumOutdoorAirFlowRate();
  }

  boost::optional<double> ZoneHVACFourPipeFanCoil::autosizedMaximumColdWaterFlowRate() const {
    return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->autosizedMaximumColdWaterFlowRate();
  }

  boost::optional<double> ZoneHVACFourPipeFanCoil::autosizedMaximumHotWaterFlowRate() const {
    return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->autosizedMaximumHotWaterFlowRate();
  }

  boost::optional<double> ZoneHVACFourPipeFanCoil::autosizedMinimumSupplyAirTemperatureinCoolingMode() const {
    return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->autosizedMinimumSupplyAirTemperatureinCoolingMode();
  }

  boost::optional<double> ZoneHVACFourPipeFanCoil::autosizedMaximumSupplyAirTemperatureinHeatingMode() const {
    return getImpl<detail::ZoneHVACFourPipeFanCoil_Impl>()->autosizedMaximumSupplyAirTemperatureinHeatingMode();
  }

} // model
} // openstudio
