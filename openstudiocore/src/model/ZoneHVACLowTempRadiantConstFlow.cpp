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

#include "CoilHeatingLowTempRadiantConstFlow.hpp"
#include "CoilHeatingLowTempRadiantConstFlow_Impl.hpp"
#include "CoilCoolingLowTempRadiantConstFlow.hpp"
#include "CoilCoolingLowTempRadiantConstFlow_Impl.hpp"
#include "ConstructionWithInternalSource.hpp"
#include "ConstructionWithInternalSource_Impl.hpp"
#include "HVACComponent.hpp"
#include "HVACComponent_Impl.hpp"
#include "WaterToAirComponent.hpp"
#include "WaterToAirComponent_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "Space.hpp"
#include "Space_Impl.hpp"
#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "ZoneHVACLowTempRadiantConstFlow.hpp"
#include "ZoneHVACLowTempRadiantConstFlow_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlow_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ZoneHVACLowTempRadiantConstFlow_Impl::ZoneHVACLowTempRadiantConstFlow_Impl(const IdfObject& idfObject,
                                                                             Model_Impl* model,
                                                                             bool keepHandle)
    : ZoneHVACComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ZoneHVACLowTempRadiantConstFlow::iddObjectType());
  }

  ZoneHVACLowTempRadiantConstFlow_Impl::ZoneHVACLowTempRadiantConstFlow_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                             Model_Impl* model,
                                                                             bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ZoneHVACLowTempRadiantConstFlow::iddObjectType());
  }

  ZoneHVACLowTempRadiantConstFlow_Impl::ZoneHVACLowTempRadiantConstFlow_Impl(const ZoneHVACLowTempRadiantConstFlow_Impl& other,
                                                                             Model_Impl* model,
                                                                             bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {}

  ModelObject ZoneHVACLowTempRadiantConstFlow_Impl::clone(Model model) const
  {
    ZoneHVACLowTempRadiantConstFlow LowTempRadiantConstFlowClone = ZoneHVACComponent_Impl::clone(model).cast<ZoneHVACLowTempRadiantConstFlow>();

    auto t_coolingCoil = coolingCoil();
    HVACComponent coolingCoilClone = t_coolingCoil.clone(model).cast<HVACComponent>();

    auto t_heatingCoil = heatingCoil();
    HVACComponent heatingCoilClone = t_heatingCoil.clone(model).cast<HVACComponent>();

    LowTempRadiantConstFlowClone.setHeatingCoil(heatingCoilClone);

    LowTempRadiantConstFlowClone.setCoolingCoil(coolingCoilClone);

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

    return LowTempRadiantConstFlowClone;
  }

  std::vector<IdfObject> ZoneHVACLowTempRadiantConstFlow_Impl::remove()
  {
    if( boost::optional<CoilHeatingLowTempRadiantConstFlow> waterHeatingCoil =
          heatingCoil().optionalCast<CoilHeatingLowTempRadiantConstFlow>() )
    {
      if( boost::optional<PlantLoop> plantLoop = waterHeatingCoil->plantLoop() )
      {
        plantLoop->removeDemandBranchWithComponent( waterHeatingCoil.get() );
      }
    }
    if( boost::optional<CoilCoolingLowTempRadiantConstFlow> waterCoolingCoil =
          coolingCoil().optionalCast<CoilCoolingLowTempRadiantConstFlow>() )
    {
      if( boost::optional<PlantLoop> plantLoop = waterCoolingCoil->plantLoop() )
      {
        plantLoop->removeDemandBranchWithComponent( waterCoolingCoil.get() );
      }
    }
    return ZoneHVACComponent_Impl::remove();
  }

  const std::vector<std::string>& ZoneHVACLowTempRadiantConstFlow_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ZoneHVACLowTempRadiantConstFlow_Impl::iddObjectType() const {
    return ZoneHVACLowTempRadiantConstFlow::iddObjectType();
  }

  std::vector<ScheduleTypeKey> ZoneHVACLowTempRadiantConstFlow_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneHVACLowTempRadiantConstFlow","Availability"));
    }
    if (std::find(b,e,OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::PumpFlowRateScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneHVACLowTempRadiantConstFlow","Pump Flow Rate"));
    }
    return result;
  }

   std::vector<ModelObject> ZoneHVACLowTempRadiantConstFlow_Impl::children() const
  {
    std::vector<ModelObject> result;
    if (OptionalHVACComponent intermediate = optionalHeatingCoil()) {
      result.push_back(*intermediate);
    }
    if (OptionalHVACComponent intermediate = optionalCoolingCoil()) {
      result.push_back(*intermediate);
    }
    return result;
  }

  unsigned ZoneHVACLowTempRadiantConstFlow_Impl::inletPort() const
  {
    return 0;
  }

  unsigned ZoneHVACLowTempRadiantConstFlow_Impl::outletPort() const
  {
    return 0;
  }

  Schedule ZoneHVACLowTempRadiantConstFlow_Impl::availabilitySchedule() const {
    boost::optional<Schedule> value = optionalAvailabilitySchedule();
    if (!value){
      // it is an error if we get here, however we don't want to crash
      // so we hook up to global always on schedule
      LOG(Error, "Required availability schedule not set, using 'Always On' schedule");
      value = this->model().alwaysOnDiscreteSchedule();
      OS_ASSERT(value);
      const_cast<ZoneHVACLowTempRadiantConstFlow_Impl*>(this)->setAvailabilitySchedule(*value);
      value = optionalAvailabilitySchedule();
    }
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<std::string> ZoneHVACLowTempRadiantConstFlow_Impl::radiantSurfaceType() const {
    return getString(OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::RadiantSurfaceType,true);
  }

  std::vector<Surface> ZoneHVACLowTempRadiantConstFlow_Impl::surfaces() const {

    //vector to hold all of the surfaces that this radiant system is attached to
    std::vector<Surface> surfaces;

    //get the thermal zone this equipment belongs to
    if (boost::optional<ThermalZone> thermalZone = this->thermalZone()) {

      //loop through all the spaces in this zone
      for (const Space& space : thermalZone->spaces()){

        //loop through all the surfaces in this space
        for (const Surface& surface : space.surfaces()){

          //skip surfaces whose construction is not internal source
          if(boost::optional<ConstructionWithInternalSource> construction = surface.construction()->optionalCast<ConstructionWithInternalSource>()){

            //TODO change this to not optional when idd change is made
            //get the strings for requested surface types and current surface type
            std::string surfGrpName = this->radiantSurfaceType().get();
            std::string surfaceType = surface.surfaceType();

            //if the current surface is of the type requested, add it to the vector of surfaces
            if(istringEqual("RoofCeiling", surfaceType) && istringEqual("Ceilings",surfGrpName)){
              surfaces.push_back(surface);
            }
            else if(istringEqual("Floor", surfaceType) && istringEqual("Floors",surfGrpName)){
              surfaces.push_back(surface);
            }
            else if((istringEqual("Floor", surfaceType) || istringEqual("RoofCeiling", surfaceType)) && istringEqual("CeilingsandFloors",surfGrpName)){
              surfaces.push_back(surface);
            }
            else if(istringEqual("AllSurfaces",surfGrpName)){
              surfaces.push_back(surface);
            }
          }
        }
      }
    }

    return surfaces;
  }

  double ZoneHVACLowTempRadiantConstFlow_Impl::hydronicTubingInsideDiameter() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::HydronicTubingInsideDiameter,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ZoneHVACLowTempRadiantConstFlow_Impl::isHydronicTubingInsideDiameterDefaulted() const {
    return isEmpty(OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::HydronicTubingInsideDiameter);
  }

  double ZoneHVACLowTempRadiantConstFlow_Impl::hydronicTubingLength() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::HydronicTubingLength,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string ZoneHVACLowTempRadiantConstFlow_Impl::temperatureControlType() const {
    boost::optional<std::string> value = getString(OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::TemperatureControlType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ZoneHVACLowTempRadiantConstFlow_Impl::isTemperatureControlTypeDefaulted() const {
    return isEmpty(OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::TemperatureControlType);
  }

  HVACComponent ZoneHVACLowTempRadiantConstFlow_Impl::heatingCoil() const {
    boost::optional<HVACComponent> coil = optionalHeatingCoil();
    OS_ASSERT(coil);
    return coil.get();
  }

  HVACComponent ZoneHVACLowTempRadiantConstFlow_Impl::coolingCoil() const {
    boost::optional<HVACComponent> coil = optionalCoolingCoil();
    OS_ASSERT(coil);
    return coil.get();
  }

  boost::optional<double> ZoneHVACLowTempRadiantConstFlow_Impl::ratedFlowRate() const {
    return getDouble(OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::RatedFlowRate,true);
  }

  boost::optional<Schedule> ZoneHVACLowTempRadiantConstFlow_Impl::pumpFlowRateSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::PumpFlowRateScheduleName);
  }

  double ZoneHVACLowTempRadiantConstFlow_Impl::ratedPumpHead() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::RatedPumpHead,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ZoneHVACLowTempRadiantConstFlow_Impl::isRatedPumpHeadDefaulted() const {
    return isEmpty(OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::RatedPumpHead);
  }

  boost::optional<double> ZoneHVACLowTempRadiantConstFlow_Impl::ratedPowerConsumption() const {
    return getDouble(OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::RatedPowerConsumption,true);
  }

  double ZoneHVACLowTempRadiantConstFlow_Impl::motorEfficiency() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::MotorEfficiency,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ZoneHVACLowTempRadiantConstFlow_Impl::isMotorEfficiencyDefaulted() const {
    return isEmpty(OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::MotorEfficiency);
  }

  double ZoneHVACLowTempRadiantConstFlow_Impl::fractionofMotorInefficienciestoFluidStream() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::FractionofMotorInefficienciestoFluidStream,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ZoneHVACLowTempRadiantConstFlow_Impl::isFractionofMotorInefficienciestoFluidStreamDefaulted() const {
    return isEmpty(OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::FractionofMotorInefficienciestoFluidStream);
  }

  std::string ZoneHVACLowTempRadiantConstFlow_Impl::numberofCircuits() const {
    boost::optional<std::string> value = getString(OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::NumberofCircuits,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ZoneHVACLowTempRadiantConstFlow_Impl::circuitLength() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::CircuitLength,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ZoneHVACLowTempRadiantConstFlow_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::AvailabilityScheduleName,
                              "ZoneHVACLowTempRadiantConstFlow",
                              "Availability",
                              schedule);
    return result;
  }

  bool ZoneHVACLowTempRadiantConstFlow_Impl::setRadiantSurfaceType(boost::optional<std::string> radiantSurfaceType) {
    bool result(false);
    if (radiantSurfaceType) {
      result = setString(OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::RadiantSurfaceType, radiantSurfaceType.get());
    }
    else {
      resetRadiantSurfaceType();
      result = true;
    }
    return result;
  }

  void ZoneHVACLowTempRadiantConstFlow_Impl::resetRadiantSurfaceType() {
    bool result = setString(OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::RadiantSurfaceType, "Ceilings");
    OS_ASSERT(result);
  }

  bool ZoneHVACLowTempRadiantConstFlow_Impl::setHydronicTubingInsideDiameter(double hydronicTubingInsideDiameter) {
    bool result = setDouble(OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::HydronicTubingInsideDiameter, hydronicTubingInsideDiameter);
    return result;
  }

  void ZoneHVACLowTempRadiantConstFlow_Impl::resetHydronicTubingInsideDiameter() {
    bool result = setString(OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::HydronicTubingInsideDiameter, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACLowTempRadiantConstFlow_Impl::setHydronicTubingLength(boost::optional<double> hydronicTubingLength) {
    bool result(false);
    if (hydronicTubingLength) {
      result = setDouble(OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::HydronicTubingLength, hydronicTubingLength.get());
    }
    else {
      resetHydronicTubingLength();
      result = true;
    }
    return result;
  }

  void ZoneHVACLowTempRadiantConstFlow_Impl::resetHydronicTubingLength() {
    bool result = setString(OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::HydronicTubingLength, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACLowTempRadiantConstFlow_Impl::setTemperatureControlType(std::string temperatureControlType) {
    bool result = setString(OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::TemperatureControlType, temperatureControlType);
    return result;
  }

  void ZoneHVACLowTempRadiantConstFlow_Impl::resetTemperatureControlType() {
    bool result = setString(OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::TemperatureControlType, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACLowTempRadiantConstFlow_Impl::setHeatingCoil(HVACComponent& heatingCoil) {
    bool result = setPointer(OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::LowTempRadiantConstantFlowHeatingCoilName, heatingCoil.handle());
    return result;
  }

  bool ZoneHVACLowTempRadiantConstFlow_Impl::setCoolingCoil(HVACComponent& coolingCoil) {
    bool result = setPointer(OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::LowTempRadiantConstantFlowCoolingCoilName, coolingCoil.handle());
    return result;
  }

  void ZoneHVACLowTempRadiantConstFlow_Impl::setRatedFlowRate(boost::optional<double> ratedFlowRate) {
    bool result(false);
    if (ratedFlowRate) {
      result = setDouble(OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::RatedFlowRate, ratedFlowRate.get());
    }
    else {
      resetRatedFlowRate();
      result = true;
    }
    OS_ASSERT(result);
  }

  void ZoneHVACLowTempRadiantConstFlow_Impl::resetRatedFlowRate() {
    bool result = setString(OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::RatedFlowRate, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACLowTempRadiantConstFlow_Impl::setPumpFlowRateSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::PumpFlowRateScheduleName,
                              "ZoneHVACLowTempRadiantConstFlow",
                              "Pump Flow Rate",
                              schedule);
    return result;
  }

  void ZoneHVACLowTempRadiantConstFlow_Impl::resetPumpFlowRateSchedule() {
    bool result = setString(OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::PumpFlowRateScheduleName, "");
    OS_ASSERT(result);
  }

  void ZoneHVACLowTempRadiantConstFlow_Impl::setRatedPumpHead(double ratedPumpHead) {
    bool result = setDouble(OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::RatedPumpHead, ratedPumpHead);
    OS_ASSERT(result);
  }

  void ZoneHVACLowTempRadiantConstFlow_Impl::resetRatedPumpHead() {
    bool result = setString(OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::RatedPumpHead, "");
    OS_ASSERT(result);
  }

  void ZoneHVACLowTempRadiantConstFlow_Impl::setRatedPowerConsumption(boost::optional<double> ratedPowerConsumption) {
    bool result(false);
    if (ratedPowerConsumption) {
      result = setDouble(OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::RatedPowerConsumption, ratedPowerConsumption.get());
    }
    else {
      resetRatedPowerConsumption();
      result = true;
    }
    OS_ASSERT(result);
  }

  void ZoneHVACLowTempRadiantConstFlow_Impl::resetRatedPowerConsumption() {
    bool result = setString(OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::RatedPowerConsumption, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACLowTempRadiantConstFlow_Impl::setMotorEfficiency(double motorEfficiency) {
    bool result = setDouble(OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::MotorEfficiency, motorEfficiency);
    return result;
  }

  void ZoneHVACLowTempRadiantConstFlow_Impl::resetMotorEfficiency() {
    bool result = setString(OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::MotorEfficiency, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACLowTempRadiantConstFlow_Impl::setFractionofMotorInefficienciestoFluidStream(double fractionofMotorInefficienciestoFluidStream) {
    bool result = setDouble(OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::FractionofMotorInefficienciestoFluidStream, fractionofMotorInefficienciestoFluidStream);
    return result;
  }

  void ZoneHVACLowTempRadiantConstFlow_Impl::resetFractionofMotorInefficienciestoFluidStream() {
    bool result = setString(OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::FractionofMotorInefficienciestoFluidStream, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACLowTempRadiantConstFlow_Impl::setNumberofCircuits(std::string numberofCircuits) {
    bool result = setString(OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::NumberofCircuits, numberofCircuits);
    return result;
  }

  void ZoneHVACLowTempRadiantConstFlow_Impl::setCircuitLength(double circuitLength) {
    bool result = setDouble(OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::CircuitLength, circuitLength);
    OS_ASSERT(result);
  }

  boost::optional<Schedule> ZoneHVACLowTempRadiantConstFlow_Impl::optionalAvailabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::AvailabilityScheduleName);
  }

  boost::optional<HVACComponent> ZoneHVACLowTempRadiantConstFlow_Impl::optionalHeatingCoil() const {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::LowTempRadiantConstantFlowHeatingCoilName);
  }

  boost::optional<HVACComponent> ZoneHVACLowTempRadiantConstFlow_Impl::optionalCoolingCoil() const {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::LowTempRadiantConstantFlowCoolingCoilName);
  }

  std::vector<std::string> ZoneHVACLowTempRadiantConstFlow_Impl::radiantSurfaceTypeValues() const {
    return ZoneHVACLowTempRadiantConstFlow::radiantSurfaceTypeValues();
  }

  std::vector<std::string> ZoneHVACLowTempRadiantConstFlow_Impl::temperatureControlTypeValues() const {
    return ZoneHVACLowTempRadiantConstFlow::temperatureControlTypeValues();
  }

  boost::optional<ThermalZone> ZoneHVACLowTempRadiantConstFlow_Impl::thermalZone() const
  {
    ModelObject thisObject = this->getObject<ModelObject>();
    std::vector<ThermalZone> thermalZones = this->model().getConcreteModelObjects<ThermalZone>();
    for( const auto & thermalZone : thermalZones )
    {
      std::vector<ModelObject> equipment = thermalZone.equipment();

      if( std::find(equipment.begin(),equipment.end(),thisObject) != equipment.end() )
      {
        return thermalZone;
      }
    }
    return boost::none;
  }

  //reimplemented to override the base-class method in ZoneHVACComponent
  //because this component doesn't get attached to the zone inlet and zone outlet nodes
  bool ZoneHVACLowTempRadiantConstFlow_Impl::addToThermalZone(ThermalZone & thermalZone)
  {
    Model m = this->model();

    if( thermalZone.model() != m )
    {
      return false;
    }

    removeFromThermalZone();

    thermalZone.setUseIdealAirLoads(false);

    thermalZone.addEquipment(this->getObject<ZoneHVACComponent>());

    return true;
  }

  //reimplemented to override the base-class method in ZoneHVACComponent
  //because this component doesn't get attached to the zone inlet and zone outlet nodes
  //and therefore doesn't need to be removed from them when removed from the zone
  void ZoneHVACLowTempRadiantConstFlow_Impl::removeFromThermalZone()
  {
    if ( boost::optional<ThermalZone> thermalZone = this->thermalZone() ) {
      thermalZone->removeEquipment(this->getObject<ZoneHVACComponent>());
    }
  }
} // detail

ZoneHVACLowTempRadiantConstFlow::ZoneHVACLowTempRadiantConstFlow(const Model& model,
                                                                 Schedule& availabilitySchedule,
                                                                 HVACComponent& heatingCoil,
                                                                 HVACComponent& coolingCoil,
                                                                 double hydronicTubingLength)
  : ZoneHVACComponent(ZoneHVACLowTempRadiantConstFlow::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>());

  bool ok = setAvailabilitySchedule(availabilitySchedule);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s availability schedule to "
                  << availabilitySchedule.briefDescription() << ".");
  }
  ok = setHeatingCoil(heatingCoil);
  OS_ASSERT(ok);

  ok = setCoolingCoil(coolingCoil);
  OS_ASSERT(ok);

  ok = setHydronicTubingLength(hydronicTubingLength);
  OS_ASSERT(ok);

}

IddObjectType ZoneHVACLowTempRadiantConstFlow::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlow);
}

std::vector<std::string> ZoneHVACLowTempRadiantConstFlow::radiantSurfaceTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::RadiantSurfaceType);
}

std::vector<std::string> ZoneHVACLowTempRadiantConstFlow::temperatureControlTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlowFields::TemperatureControlType);
}

Schedule ZoneHVACLowTempRadiantConstFlow::availabilitySchedule() const {
  return getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->availabilitySchedule();
}

boost::optional<std::string> ZoneHVACLowTempRadiantConstFlow::radiantSurfaceType() const {
  return getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->radiantSurfaceType();
}

std::vector<Surface> ZoneHVACLowTempRadiantConstFlow::surfaces() const {
  return getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->surfaces();
}

double ZoneHVACLowTempRadiantConstFlow::hydronicTubingInsideDiameter() const {
  return getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->hydronicTubingInsideDiameter();
}

bool ZoneHVACLowTempRadiantConstFlow::isHydronicTubingInsideDiameterDefaulted() const {
  return getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->isHydronicTubingInsideDiameterDefaulted();
}

double ZoneHVACLowTempRadiantConstFlow::hydronicTubingLength() const {
  return getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->hydronicTubingLength();
}

std::string ZoneHVACLowTempRadiantConstFlow::temperatureControlType() const {
  return getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->temperatureControlType();
}

bool ZoneHVACLowTempRadiantConstFlow::isTemperatureControlTypeDefaulted() const {
  return getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->isTemperatureControlTypeDefaulted();
}

HVACComponent ZoneHVACLowTempRadiantConstFlow::heatingCoil() const {
  return getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->heatingCoil();
}

HVACComponent ZoneHVACLowTempRadiantConstFlow::coolingCoil() const {
  return getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->coolingCoil();
}

boost::optional<double> ZoneHVACLowTempRadiantConstFlow::ratedFlowRate() const {
  return getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->ratedFlowRate();
}

boost::optional<Schedule> ZoneHVACLowTempRadiantConstFlow::pumpFlowRateSchedule() const {
  return getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->pumpFlowRateSchedule();
}

double ZoneHVACLowTempRadiantConstFlow::ratedPumpHead() const {
  return getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->ratedPumpHead();
}

bool ZoneHVACLowTempRadiantConstFlow::isRatedPumpHeadDefaulted() const {
  return getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->isRatedPumpHeadDefaulted();
}

boost::optional<double> ZoneHVACLowTempRadiantConstFlow::ratedPowerConsumption() const {
  return getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->ratedPowerConsumption();
}

double ZoneHVACLowTempRadiantConstFlow::motorEfficiency() const {
  return getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->motorEfficiency();
}

bool ZoneHVACLowTempRadiantConstFlow::isMotorEfficiencyDefaulted() const {
  return getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->isMotorEfficiencyDefaulted();
}

double ZoneHVACLowTempRadiantConstFlow::fractionofMotorInefficienciestoFluidStream() const {
  return getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->fractionofMotorInefficienciestoFluidStream();
}

bool ZoneHVACLowTempRadiantConstFlow::isFractionofMotorInefficienciestoFluidStreamDefaulted() const {
  return getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->isFractionofMotorInefficienciestoFluidStreamDefaulted();
}

std::string ZoneHVACLowTempRadiantConstFlow::numberofCircuits() const {
  return getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->numberofCircuits();
}

double ZoneHVACLowTempRadiantConstFlow::circuitLength() const {
  return getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->circuitLength();
}

bool ZoneHVACLowTempRadiantConstFlow::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->setAvailabilitySchedule(schedule);
}

bool ZoneHVACLowTempRadiantConstFlow::setRadiantSurfaceType(std::string radiantSurfaceType) {
  return getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->setRadiantSurfaceType(radiantSurfaceType);
}

void ZoneHVACLowTempRadiantConstFlow::resetRadiantSurfaceType() {
  getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->resetRadiantSurfaceType();
}

bool ZoneHVACLowTempRadiantConstFlow::setHydronicTubingInsideDiameter(double hydronicTubingInsideDiameter) {
  return getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->setHydronicTubingInsideDiameter(hydronicTubingInsideDiameter);
}

void ZoneHVACLowTempRadiantConstFlow::resetHydronicTubingInsideDiameter() {
  getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->resetHydronicTubingInsideDiameter();
}

bool ZoneHVACLowTempRadiantConstFlow::setHydronicTubingLength(double hydronicTubingLength) {
  return getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->setHydronicTubingLength(hydronicTubingLength);
}

void ZoneHVACLowTempRadiantConstFlow::resetHydronicTubingLength() {
  getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->resetHydronicTubingLength();
}

bool ZoneHVACLowTempRadiantConstFlow::setTemperatureControlType(std::string temperatureControlType) {
  return getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->setTemperatureControlType(temperatureControlType);
}

void ZoneHVACLowTempRadiantConstFlow::resetTemperatureControlType() {
  getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->resetTemperatureControlType();
}

bool ZoneHVACLowTempRadiantConstFlow::setHeatingCoil(HVACComponent& heatingCoil) {
  return getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->setHeatingCoil(heatingCoil);
}

bool ZoneHVACLowTempRadiantConstFlow::setCoolingCoil(HVACComponent& coolingCoil) {
  return getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->setCoolingCoil(coolingCoil);
}

void ZoneHVACLowTempRadiantConstFlow::setRatedFlowRate(double ratedFlowRate) {
  getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->setRatedFlowRate(ratedFlowRate);
}

void ZoneHVACLowTempRadiantConstFlow::resetRatedFlowRate() {
  getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->resetRatedFlowRate();
}

bool ZoneHVACLowTempRadiantConstFlow::setPumpFlowRateSchedule(Schedule& schedule) {
  return getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->setPumpFlowRateSchedule(schedule);
}

void ZoneHVACLowTempRadiantConstFlow::resetPumpFlowRateSchedule() {
  getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->resetPumpFlowRateSchedule();
}

void ZoneHVACLowTempRadiantConstFlow::setRatedPumpHead(double ratedPumpHead) {
  getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->setRatedPumpHead(ratedPumpHead);
}

void ZoneHVACLowTempRadiantConstFlow::resetRatedPumpHead() {
  getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->resetRatedPumpHead();
}

void ZoneHVACLowTempRadiantConstFlow::setRatedPowerConsumption(double ratedPowerConsumption) {
  getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->setRatedPowerConsumption(ratedPowerConsumption);
}

void ZoneHVACLowTempRadiantConstFlow::resetRatedPowerConsumption() {
  getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->resetRatedPowerConsumption();
}

bool ZoneHVACLowTempRadiantConstFlow::setMotorEfficiency(double motorEfficiency) {
  return getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->setMotorEfficiency(motorEfficiency);
}

void ZoneHVACLowTempRadiantConstFlow::resetMotorEfficiency() {
  getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->resetMotorEfficiency();
}

bool ZoneHVACLowTempRadiantConstFlow::setFractionofMotorInefficienciestoFluidStream(double fractionofMotorInefficienciestoFluidStream) {
  return getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->setFractionofMotorInefficienciestoFluidStream(fractionofMotorInefficienciestoFluidStream);
}

void ZoneHVACLowTempRadiantConstFlow::resetFractionofMotorInefficienciestoFluidStream() {
  getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->resetFractionofMotorInefficienciestoFluidStream();
}

bool ZoneHVACLowTempRadiantConstFlow::setNumberofCircuits(std::string numberofCircuits) {
  return getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->setNumberofCircuits(numberofCircuits);
}

void ZoneHVACLowTempRadiantConstFlow::setCircuitLength(double circLength) {
  getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->setCircuitLength(circLength);
}

boost::optional<ThermalZone> ZoneHVACLowTempRadiantConstFlow::thermalZone() const
{
  return getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->thermalZone();
}

bool ZoneHVACLowTempRadiantConstFlow::addToThermalZone(ThermalZone & thermalZone)
{
  return getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->addToThermalZone(thermalZone);
}

void ZoneHVACLowTempRadiantConstFlow::removeFromThermalZone()
{
  return getImpl<detail::ZoneHVACLowTempRadiantConstFlow_Impl>()->removeFromThermalZone();
}
/// @cond
ZoneHVACLowTempRadiantConstFlow::ZoneHVACLowTempRadiantConstFlow(std::shared_ptr<detail::ZoneHVACLowTempRadiantConstFlow_Impl> impl)
  : ZoneHVACComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

