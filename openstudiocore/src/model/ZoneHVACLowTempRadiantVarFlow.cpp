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

#include "CoilHeatingLowTempRadiantVarFlow.hpp"
#include "CoilHeatingLowTempRadiantVarFlow_Impl.hpp"
#include "CoilCoolingLowTempRadiantVarFlow.hpp"
#include "CoilCoolingLowTempRadiantVarFlow_Impl.hpp"
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
#include "ZoneHVACLowTempRadiantVarFlow.hpp"
#include "ZoneHVACLowTempRadiantVarFlow_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>


#include <utilities/idd/OS_ZoneHVAC_LowTemperatureRadiant_VariableFlow_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ZoneHVACLowTempRadiantVarFlow_Impl::ZoneHVACLowTempRadiantVarFlow_Impl(const IdfObject& idfObject,
                                                                         Model_Impl* model,
                                                                         bool keepHandle)
    : ZoneHVACComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ZoneHVACLowTempRadiantVarFlow::iddObjectType());
  }

  ZoneHVACLowTempRadiantVarFlow_Impl::ZoneHVACLowTempRadiantVarFlow_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                         Model_Impl* model,
                                                                         bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ZoneHVACLowTempRadiantVarFlow::iddObjectType());
  }

  ZoneHVACLowTempRadiantVarFlow_Impl::ZoneHVACLowTempRadiantVarFlow_Impl(const ZoneHVACLowTempRadiantVarFlow_Impl& other,
                                                                         Model_Impl* model,
                                                                         bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {}

  ModelObject ZoneHVACLowTempRadiantVarFlow_Impl::clone(Model model) const
  {
    ZoneHVACLowTempRadiantVarFlow lowTempRadiantVarFlowClone = ZoneHVACComponent_Impl::clone(model).cast<ZoneHVACLowTempRadiantVarFlow>();

    auto t_coolingCoil = coolingCoil();
    HVACComponent coolingCoilClone = t_coolingCoil.clone(model).cast<HVACComponent>();

    auto t_heatingCoil = heatingCoil();
    HVACComponent heatingCoilClone = t_heatingCoil.clone(model).cast<HVACComponent>();

    lowTempRadiantVarFlowClone.setHeatingCoil(heatingCoilClone);

    lowTempRadiantVarFlowClone.setCoolingCoil(coolingCoilClone);

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

    return lowTempRadiantVarFlowClone;
  }

  std::vector<IdfObject> ZoneHVACLowTempRadiantVarFlow_Impl::remove()
  {
    if( boost::optional<CoilHeatingLowTempRadiantVarFlow> waterHeatingCoil =
          heatingCoil().optionalCast<CoilHeatingLowTempRadiantVarFlow>() )
    {
      if( boost::optional<PlantLoop> plantLoop = waterHeatingCoil->plantLoop() )
      {
        plantLoop->removeDemandBranchWithComponent( waterHeatingCoil.get() );
      }
    }
    if( boost::optional<CoilCoolingLowTempRadiantVarFlow> waterCoolingCoil =
          coolingCoil().optionalCast<CoilCoolingLowTempRadiantVarFlow>() )
    {
      if( boost::optional<PlantLoop> plantLoop = waterCoolingCoil->plantLoop() )
      {
        plantLoop->removeDemandBranchWithComponent( waterCoolingCoil.get() );
      }
    }
    return ZoneHVACComponent_Impl::remove();
  }

  const std::vector<std::string>& ZoneHVACLowTempRadiantVarFlow_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty())
    {
    }
    return result;
  }

  IddObjectType ZoneHVACLowTempRadiantVarFlow_Impl::iddObjectType() const
  {
    return ZoneHVACLowTempRadiantVarFlow::iddObjectType();
  }

  std::vector<ScheduleTypeKey> ZoneHVACLowTempRadiantVarFlow_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneHVACLowTempRadiantVarFlow","Availability"));
    }
    return result;
  }

  Schedule ZoneHVACLowTempRadiantVarFlow_Impl::availabilitySchedule() const
  {
    boost::optional<Schedule> value = optionalAvailabilitySchedule();
    if (!value)
    {
      LOG(Error, "Required availability schedule not set, using 'Always On' schedule");
      value = this->model().alwaysOnDiscreteSchedule();
      OS_ASSERT(value);
      const_cast<ZoneHVACLowTempRadiantVarFlow_Impl*>(this)->setAvailabilitySchedule(*value);
      value = optionalAvailabilitySchedule();
    }
    return value.get();
  }

  std::vector<ModelObject> ZoneHVACLowTempRadiantVarFlow_Impl::children() const
  {
    std::vector<ModelObject> result;
    if (OptionalHVACComponent intermediate = optionalHeatingCoil())
    {
      result.push_back(*intermediate);
    }
    if (OptionalHVACComponent intermediate = optionalCoolingCoil())
    {
      result.push_back(*intermediate);
    }
    return result;
  }

  unsigned ZoneHVACLowTempRadiantVarFlow_Impl::inletPort() const
  {
    return 0;
  }

  unsigned ZoneHVACLowTempRadiantVarFlow_Impl::outletPort() const
  {
    return 0;
  }

  HVACComponent ZoneHVACLowTempRadiantVarFlow_Impl::heatingCoil() const
  {
    boost::optional<HVACComponent> coil = optionalHeatingCoil();
    OS_ASSERT(coil);
    return coil.get();
  }

  HVACComponent ZoneHVACLowTempRadiantVarFlow_Impl::coolingCoil() const
  {
    boost::optional<HVACComponent> coil = optionalCoolingCoil();
    OS_ASSERT(coil);
    return coil.get();
  }

  bool ZoneHVACLowTempRadiantVarFlow_Impl::setHeatingCoil(HVACComponent& heatingCoil)
  {
    bool result = setPointer(OS_ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::LowTempRadiantVariableFlowHeatingCoilName, heatingCoil.handle());
    return result;
  }

  bool ZoneHVACLowTempRadiantVarFlow_Impl::setCoolingCoil(HVACComponent& coolingCoil)
  {
    bool result = setPointer(OS_ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::LowTempRadiantVariableFlowCoolingCoilName, coolingCoil.handle());
    return result;
  }

  boost::optional<std::string> ZoneHVACLowTempRadiantVarFlow_Impl::radiantSurfaceType() const
  {
    return getString(OS_ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::RadiantSurfaceType,true);
  }

  std::vector<Surface> ZoneHVACLowTempRadiantVarFlow_Impl::surfaces() const {

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

  double ZoneHVACLowTempRadiantVarFlow_Impl::hydronicTubingInsideDiameter() const
  {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::HydronicTubingInsideDiameter,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ZoneHVACLowTempRadiantVarFlow_Impl::isHydronicTubingInsideDiameterDefaulted() const
  {
    return isEmpty(OS_ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::HydronicTubingInsideDiameter);
  }

  boost::optional<double> ZoneHVACLowTempRadiantVarFlow_Impl::hydronicTubingLength() const
  {
    return getDouble(OS_ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::HydronicTubingLength,true);
  }

  bool ZoneHVACLowTempRadiantVarFlow_Impl::isHydronicTubingLengthDefaulted() const
  {
    return isEmpty(OS_ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::HydronicTubingLength);
  }

  bool ZoneHVACLowTempRadiantVarFlow_Impl::isHydronicTubingLengthAutosized() const
  {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::HydronicTubingLength, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  std::string ZoneHVACLowTempRadiantVarFlow_Impl::temperatureControlType() const
  {
    boost::optional<std::string> value = getString(OS_ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::TemperatureControlType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ZoneHVACLowTempRadiantVarFlow_Impl::isTemperatureControlTypeDefaulted() const
  {
    return isEmpty(OS_ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::TemperatureControlType);
  }

  std::string ZoneHVACLowTempRadiantVarFlow_Impl::numberofCircuits() const {
    boost::optional<std::string> value = getString(OS_ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::NumberofCircuits,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ZoneHVACLowTempRadiantVarFlow_Impl::circuitLength() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::CircuitLength,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ZoneHVACLowTempRadiantVarFlow_Impl::setAvailabilitySchedule(Schedule& schedule)
  {
    bool result = setSchedule(OS_ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::AvailabilityScheduleName,
                              "ZoneHVACLowTempRadiantVarFlow",
                              "Availability",
                              schedule);
    return result;
  }

  bool ZoneHVACLowTempRadiantVarFlow_Impl::setRadiantSurfaceType(boost::optional<std::string> radiantSurfaceType)
  {
    bool result(false);
    if (radiantSurfaceType)
    {
      result = setString(OS_ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::RadiantSurfaceType, radiantSurfaceType.get());
    }
    else
    {
      resetRadiantSurfaceType();
      result = true;
    }
    return result;
  }

  void ZoneHVACLowTempRadiantVarFlow_Impl::resetRadiantSurfaceType()
  {
    bool result = setString(OS_ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::RadiantSurfaceType, "Ceilings");
    OS_ASSERT(result);
  }

  bool ZoneHVACLowTempRadiantVarFlow_Impl::setHydronicTubingInsideDiameter(double hydronicTubingInsideDiameter) \
  {
    bool result = setDouble(OS_ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::HydronicTubingInsideDiameter, hydronicTubingInsideDiameter);
    return result;
  }

  void ZoneHVACLowTempRadiantVarFlow_Impl::resetHydronicTubingInsideDiameter()
  {
    bool result = setString(OS_ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::HydronicTubingInsideDiameter, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACLowTempRadiantVarFlow_Impl::setHydronicTubingLength(boost::optional<double> hydronicTubingLength)
  {
    bool result(false);
    if (hydronicTubingLength)
    {
      result = setDouble(OS_ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::HydronicTubingLength, hydronicTubingLength.get());
    }
    else
    {
      resetHydronicTubingLength();
      result = true;
    }
    return result;
  }

  void ZoneHVACLowTempRadiantVarFlow_Impl::resetHydronicTubingLength()
  {
    bool result = setString(OS_ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::HydronicTubingLength, "");
    OS_ASSERT(result);
  }

  void ZoneHVACLowTempRadiantVarFlow_Impl::autosizeHydronicTubingLength()
  {
    bool result = setString(OS_ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::HydronicTubingLength, "autosize");
    OS_ASSERT(result);
  }

  bool ZoneHVACLowTempRadiantVarFlow_Impl::setTemperatureControlType(std::string temperatureControlType)
  {
    bool result = setString(OS_ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::TemperatureControlType, temperatureControlType);
    return result;
  }

  void ZoneHVACLowTempRadiantVarFlow_Impl::resetTemperatureControlType()
  {
    bool result = setString(OS_ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::TemperatureControlType, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACLowTempRadiantVarFlow_Impl::setNumberofCircuits(std::string numberofCircuits) {
    bool result = setString(OS_ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::NumberofCircuits, numberofCircuits);
    return result;
  }

  void ZoneHVACLowTempRadiantVarFlow_Impl::setCircuitLength(double circuitLength) {
    setDouble(OS_ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::CircuitLength, circuitLength);
  }

  boost::optional<Schedule> ZoneHVACLowTempRadiantVarFlow_Impl::optionalAvailabilitySchedule() const
  {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::AvailabilityScheduleName);
  }

  boost::optional<HVACComponent> ZoneHVACLowTempRadiantVarFlow_Impl::optionalHeatingCoil() const
  {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::LowTempRadiantVariableFlowHeatingCoilName);
  }

  boost::optional<HVACComponent> ZoneHVACLowTempRadiantVarFlow_Impl::optionalCoolingCoil() const
  {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::LowTempRadiantVariableFlowCoolingCoilName);
  }

  std::vector<std::string> ZoneHVACLowTempRadiantVarFlow_Impl::radiantSurfaceTypeValues() const
  {
    return ZoneHVACLowTempRadiantVarFlow::radiantSurfaceTypeValues();
  }

  std::vector<std::string> ZoneHVACLowTempRadiantVarFlow_Impl::temperatureControlTypeValues() const
  {
    return ZoneHVACLowTempRadiantVarFlow::temperatureControlTypeValues();
  }

  boost::optional<ThermalZone> ZoneHVACLowTempRadiantVarFlow_Impl::thermalZone() const
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
  bool ZoneHVACLowTempRadiantVarFlow_Impl::addToThermalZone(ThermalZone & thermalZone)
  {
    Model m = this->model();

    if( thermalZone.model() != m )
    {
      return false;
    }

    if( thermalZone.isPlenum() )
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
  void ZoneHVACLowTempRadiantVarFlow_Impl::removeFromThermalZone()
  {
    if ( boost::optional<ThermalZone> thermalZone = this->thermalZone() ) {
      thermalZone->removeEquipment(this->getObject<ZoneHVACComponent>());
    }
  }

} // detail

ZoneHVACLowTempRadiantVarFlow::ZoneHVACLowTempRadiantVarFlow(const Model& model,
                                                             Schedule& availabilitySchedule,
                                                             HVACComponent& heatingCoil,
                                                             HVACComponent& coolingCoil)

  : ZoneHVACComponent(ZoneHVACLowTempRadiantVarFlow::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ZoneHVACLowTempRadiantVarFlow_Impl>());

  bool ok = setAvailabilitySchedule(availabilitySchedule);

  if (!ok)
  {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s availability schedule to "
                  << availabilitySchedule.briefDescription() << ".");
  }

  ok = setHeatingCoil(heatingCoil);
  OS_ASSERT(ok);

  ok = setCoolingCoil(coolingCoil);
  OS_ASSERT(ok);
}

IddObjectType ZoneHVACLowTempRadiantVarFlow::iddObjectType()
{
  return IddObjectType(IddObjectType::OS_ZoneHVAC_LowTemperatureRadiant_VariableFlow);
}

std::vector<std::string> ZoneHVACLowTempRadiantVarFlow::radiantSurfaceTypeValues()
{
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::RadiantSurfaceType);
}

std::vector<std::string> ZoneHVACLowTempRadiantVarFlow::temperatureControlTypeValues()
{
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ZoneHVAC_LowTemperatureRadiant_VariableFlowFields::TemperatureControlType);
}

Schedule ZoneHVACLowTempRadiantVarFlow::availabilitySchedule() const
{
  return getImpl<detail::ZoneHVACLowTempRadiantVarFlow_Impl>()->availabilitySchedule();
}

HVACComponent ZoneHVACLowTempRadiantVarFlow::heatingCoil() const
{
  return getImpl<detail::ZoneHVACLowTempRadiantVarFlow_Impl>()->heatingCoil();
}

HVACComponent ZoneHVACLowTempRadiantVarFlow::coolingCoil() const
{
  return getImpl<detail::ZoneHVACLowTempRadiantVarFlow_Impl>()->coolingCoil();
}

boost::optional<std::string> ZoneHVACLowTempRadiantVarFlow::radiantSurfaceType() const
{
  return getImpl<detail::ZoneHVACLowTempRadiantVarFlow_Impl>()->radiantSurfaceType();
}

std::vector<Surface> ZoneHVACLowTempRadiantVarFlow::surfaces() const {
  return getImpl<detail::ZoneHVACLowTempRadiantVarFlow_Impl>()->surfaces();
}

double ZoneHVACLowTempRadiantVarFlow::hydronicTubingInsideDiameter() const
{
  return getImpl<detail::ZoneHVACLowTempRadiantVarFlow_Impl>()->hydronicTubingInsideDiameter();
}

bool ZoneHVACLowTempRadiantVarFlow::isHydronicTubingInsideDiameterDefaulted() const
{
  return getImpl<detail::ZoneHVACLowTempRadiantVarFlow_Impl>()->isHydronicTubingInsideDiameterDefaulted();
}

boost::optional<double> ZoneHVACLowTempRadiantVarFlow::hydronicTubingLength() const
{
  return getImpl<detail::ZoneHVACLowTempRadiantVarFlow_Impl>()->hydronicTubingLength();
}

bool ZoneHVACLowTempRadiantVarFlow::isHydronicTubingLengthDefaulted() const
{
  return getImpl<detail::ZoneHVACLowTempRadiantVarFlow_Impl>()->isHydronicTubingLengthDefaulted();
}

bool ZoneHVACLowTempRadiantVarFlow::isHydronicTubingLengthAutosized() const
{
  return getImpl<detail::ZoneHVACLowTempRadiantVarFlow_Impl>()->isHydronicTubingLengthAutosized();
}

std::string ZoneHVACLowTempRadiantVarFlow::temperatureControlType() const
{
  return getImpl<detail::ZoneHVACLowTempRadiantVarFlow_Impl>()->temperatureControlType();
}

bool ZoneHVACLowTempRadiantVarFlow::isTemperatureControlTypeDefaulted() const
{
  return getImpl<detail::ZoneHVACLowTempRadiantVarFlow_Impl>()->isTemperatureControlTypeDefaulted();
}

std::string ZoneHVACLowTempRadiantVarFlow::numberofCircuits() const {
  return getImpl<detail::ZoneHVACLowTempRadiantVarFlow_Impl>()->numberofCircuits();
}

double ZoneHVACLowTempRadiantVarFlow::circuitLength() const {
  return getImpl<detail::ZoneHVACLowTempRadiantVarFlow_Impl>()->circuitLength();
}

bool ZoneHVACLowTempRadiantVarFlow::setAvailabilitySchedule(Schedule& schedule)
{
  return getImpl<detail::ZoneHVACLowTempRadiantVarFlow_Impl>()->setAvailabilitySchedule(schedule);
}

bool ZoneHVACLowTempRadiantVarFlow::setHeatingCoil(HVACComponent& heatingCoil) {
  return getImpl<detail::ZoneHVACLowTempRadiantVarFlow_Impl>()->setHeatingCoil(heatingCoil);
}

bool ZoneHVACLowTempRadiantVarFlow::setCoolingCoil(HVACComponent& coolingCoil) {
  return getImpl<detail::ZoneHVACLowTempRadiantVarFlow_Impl>()->setCoolingCoil(coolingCoil);
}

bool ZoneHVACLowTempRadiantVarFlow::setRadiantSurfaceType(std::string radiantSurfaceType)
{
  return getImpl<detail::ZoneHVACLowTempRadiantVarFlow_Impl>()->setRadiantSurfaceType(radiantSurfaceType);
}

void ZoneHVACLowTempRadiantVarFlow::resetRadiantSurfaceType()
{
  getImpl<detail::ZoneHVACLowTempRadiantVarFlow_Impl>()->resetRadiantSurfaceType();
}

bool ZoneHVACLowTempRadiantVarFlow::setHydronicTubingInsideDiameter(double hydronicTubingInsideDiameter)
{
  return getImpl<detail::ZoneHVACLowTempRadiantVarFlow_Impl>()->setHydronicTubingInsideDiameter(hydronicTubingInsideDiameter);
}

void ZoneHVACLowTempRadiantVarFlow::resetHydronicTubingInsideDiameter()
{
  getImpl<detail::ZoneHVACLowTempRadiantVarFlow_Impl>()->resetHydronicTubingInsideDiameter();
}

bool ZoneHVACLowTempRadiantVarFlow::setHydronicTubingLength(double hydronicTubingLength)
{
  return getImpl<detail::ZoneHVACLowTempRadiantVarFlow_Impl>()->setHydronicTubingLength(hydronicTubingLength);
}

void ZoneHVACLowTempRadiantVarFlow::resetHydronicTubingLength()
{
  getImpl<detail::ZoneHVACLowTempRadiantVarFlow_Impl>()->resetHydronicTubingLength();
}

void ZoneHVACLowTempRadiantVarFlow::autosizeHydronicTubingLength()
{
  getImpl<detail::ZoneHVACLowTempRadiantVarFlow_Impl>()->autosizeHydronicTubingLength();
}

bool ZoneHVACLowTempRadiantVarFlow::setTemperatureControlType(std::string temperatureControlType)
{
  return getImpl<detail::ZoneHVACLowTempRadiantVarFlow_Impl>()->setTemperatureControlType(temperatureControlType);
}

void ZoneHVACLowTempRadiantVarFlow::resetTemperatureControlType()
{
  getImpl<detail::ZoneHVACLowTempRadiantVarFlow_Impl>()->resetTemperatureControlType();
}

bool ZoneHVACLowTempRadiantVarFlow::setNumberofCircuits(std::string numCircs) {
  return getImpl<detail::ZoneHVACLowTempRadiantVarFlow_Impl>()->setNumberofCircuits(numCircs);
}

void ZoneHVACLowTempRadiantVarFlow::setCircuitLength(double circLength) {
  getImpl<detail::ZoneHVACLowTempRadiantVarFlow_Impl>()->setCircuitLength(circLength);
}

boost::optional<ThermalZone> ZoneHVACLowTempRadiantVarFlow::thermalZone() const
{
  return getImpl<detail::ZoneHVACLowTempRadiantVarFlow_Impl>()->thermalZone();
}

bool ZoneHVACLowTempRadiantVarFlow::addToThermalZone(ThermalZone & thermalZone)
{
  return getImpl<detail::ZoneHVACLowTempRadiantVarFlow_Impl>()->addToThermalZone(thermalZone);
}

void ZoneHVACLowTempRadiantVarFlow::removeFromThermalZone()
{
  return getImpl<detail::ZoneHVACLowTempRadiantVarFlow_Impl>()->removeFromThermalZone();
}
/// @cond
ZoneHVACLowTempRadiantVarFlow::ZoneHVACLowTempRadiantVarFlow(std::shared_ptr<detail::ZoneHVACLowTempRadiantVarFlow_Impl> impl)
  : ZoneHVACComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

