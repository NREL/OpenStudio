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

#include "CoilHeatingGas.hpp"
#include "CoilHeatingGas_Impl.hpp"
#include "Model.hpp"
#include "AirTerminalSingleDuctParallelPIUReheat.hpp"
#include "AirTerminalSingleDuctParallelPIUReheat_Impl.hpp"
#include "AirTerminalSingleDuctVAVReheat.hpp"
#include "AirTerminalSingleDuctVAVReheat_Impl.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAir.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAir_Impl.hpp"
#include "ZoneHVACWaterToAirHeatPump.hpp"
#include "ZoneHVACWaterToAirHeatPump_Impl.hpp"
#include "AirLoopHVACUnitarySystem.hpp"
#include "AirLoopHVACUnitarySystem_Impl.hpp"
#include "AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass.hpp"
#include "AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "CurveCubic.hpp"
#include "CurveCubic_Impl.hpp"
#include "CurveQuadratic.hpp"
#include "CurveQuadratic_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include <utilities/idd/OS_Coil_Heating_Gas_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>
#include "../utilities/core/Compare.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

namespace openstudio {
namespace model {

namespace detail{

  CoilHeatingGas_Impl::CoilHeatingGas_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : StraightComponent_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilHeatingGas::iddObjectType());
  }

  CoilHeatingGas_Impl::CoilHeatingGas_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
                                             : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilHeatingGas::iddObjectType());
  }

  CoilHeatingGas_Impl::CoilHeatingGas_Impl(const CoilHeatingGas_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
                                             : StraightComponent_Impl(other,model,keepHandle)
  {}

  IddObjectType CoilHeatingGas_Impl::iddObjectType() const {
    return CoilHeatingGas::iddObjectType();
  }

  // Get all output variable names that could be associated with this object.
  const std::vector<std::string>& CoilHeatingGas_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  std::vector<ScheduleTypeKey> CoilHeatingGas_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Coil_Heating_GasFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("CoilHeatingGas","Availability"));
    }
    return result;
  }

  unsigned CoilHeatingGas_Impl::inletPort()
  {
    return OS_Coil_Heating_GasFields::AirInletNodeName;
  }

  unsigned CoilHeatingGas_Impl::outletPort()
  {
    return OS_Coil_Heating_GasFields::AirOutletNodeName;
  }

  Schedule CoilHeatingGas_Impl::availabilitySchedule() const {
    boost::optional<Schedule> value = optionalAvailabilitySchedule();
    if (!value){
      // it is an error if we get here, however we don't want to crash
      // so we hook up to global always on schedule
      LOG(Error, "Required availability schedule not set, using 'Always On' schedule");
      value = this->model().alwaysOnDiscreteSchedule();
      OS_ASSERT(value);
      const_cast<CoilHeatingGas_Impl*>(this)->setAvailabilitySchedule(*value);
      value = optionalAvailabilitySchedule();
    }
    OS_ASSERT(value);
    return value.get();
  }
  bool CoilHeatingGas_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Coil_Heating_GasFields::AvailabilityScheduleName,
                              "CoilHeatingGas",
                              "Availability",
                              schedule);
    return result;
  }

  std::string CoilHeatingGas_Impl::fuelType() const
  {
    return this->getString(OS_Coil_Heating_GasFields::FuelType, true).get();
  }

  bool CoilHeatingGas_Impl::setFuelType(const std::string &fuelType)
  {
    if (fuelType == "PropaneGas") {
        LOG(Warn, "'PropaneGas' is deprecated for Coil_Heating_GasFields:FuelType, use 'Propane' instead");
    }
    return this->setString(OS_Coil_Heating_GasFields::FuelType, fuelType);
  }

  void CoilHeatingGas_Impl::resetFuelType()
  {
    this->setString(OS_Coil_Heating_GasFields::FuelType, "");
  }

  double CoilHeatingGas_Impl::gasBurnerEfficiency() const
  {
    return this->getDouble(OS_Coil_Heating_GasFields::GasBurnerEfficiency).get();
  }

  void CoilHeatingGas_Impl::setGasBurnerEfficiency(double val)
  {
    this->setDouble(OS_Coil_Heating_GasFields::GasBurnerEfficiency,val);
  }

  double CoilHeatingGas_Impl::parasiticElectricLoad() const
  {
    return this->getDouble(OS_Coil_Heating_GasFields::ParasiticElectricLoad).get();
  }

  void CoilHeatingGas_Impl::setParasiticElectricLoad(double val)
  {
    this->setDouble(OS_Coil_Heating_GasFields::ParasiticElectricLoad,val);
  }

  double CoilHeatingGas_Impl::parasiticGasLoad() const
  {
    return this->getDouble(OS_Coil_Heating_GasFields::ParasiticGasLoad).get();
  }

  void CoilHeatingGas_Impl::setParasiticGasLoad(double val)
  {
    this->setDouble(OS_Coil_Heating_GasFields::ParasiticGasLoad,val);
  }

  boost::optional<HVACComponent> CoilHeatingGas_Impl::containingHVACComponent() const
  {
    // Process all types that might contain a CoilHeatingWater object.

    // AirLoopHVACUnitarySystem
    std::vector<AirLoopHVACUnitarySystem> airLoopHVACUnitarySystems = this->model().getConcreteModelObjects<AirLoopHVACUnitarySystem>();

    for( const auto & airLoopHVACUnitarySystem : airLoopHVACUnitarySystems )
    {
      if( boost::optional<HVACComponent> heatingCoil = airLoopHVACUnitarySystem.heatingCoil() )
      {
        if( heatingCoil->handle() == this->handle() )
        {
          return airLoopHVACUnitarySystem;
        }
      }
      if( boost::optional<HVACComponent> suppHeatingCoil = airLoopHVACUnitarySystem.supplementalHeatingCoil() )
      {
        if( suppHeatingCoil->handle() == this->handle() )
        {
          return airLoopHVACUnitarySystem;
        }
      }
    }

    // AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass
    std::vector<AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass> bypassSystems = this->model().getConcreteModelObjects<AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass>();

    for( const auto & bypassSystem : bypassSystems )
    {
      if( boost::optional<HVACComponent> heatingCoil = bypassSystem.heatingCoil() )
      {
        if( heatingCoil->handle() == this->handle() )
        {
          return bypassSystem;
        }
      }
    }

    // AirTerminalSingleDuctVAVReheat

    std::vector<AirTerminalSingleDuctVAVReheat> airTerminalSingleDuctVAVReheatObjects;

    airTerminalSingleDuctVAVReheatObjects = this->model().getConcreteModelObjects<AirTerminalSingleDuctVAVReheat>();

    for( const auto & airTerminalSingleDuctVAVReheatObject : airTerminalSingleDuctVAVReheatObjects )
    {
      if( boost::optional<HVACComponent> coil = airTerminalSingleDuctVAVReheatObject.reheatCoil() )
      {
        if( coil->handle() == this->handle() )
        {
          return airTerminalSingleDuctVAVReheatObject;
        }
      }
    }

    // AirTerminalSingleDuctParallelPIUReheat

    std::vector<AirTerminalSingleDuctParallelPIUReheat> airTerminalSingleDuctParallelPIUReheatObjects;

    airTerminalSingleDuctParallelPIUReheatObjects = this->model().getConcreteModelObjects<AirTerminalSingleDuctParallelPIUReheat>();

    for( const auto & airTerminalSingleDuctParallelPIUReheatObject : airTerminalSingleDuctParallelPIUReheatObjects )
    {
      if( boost::optional<HVACComponent> coil = airTerminalSingleDuctParallelPIUReheatObject.reheatCoil() )
      {
        if( coil->handle() == this->handle() )
        {
          return airTerminalSingleDuctParallelPIUReheatObject;
        }
      }
    }

    // AirLoopHVACUnitaryHeatPumpAirToAir

    std::vector<AirLoopHVACUnitaryHeatPumpAirToAir> airLoopHVACUnitaryHeatPumpAirToAirs;

    airLoopHVACUnitaryHeatPumpAirToAirs = this->model().getConcreteModelObjects<AirLoopHVACUnitaryHeatPumpAirToAir>();

    for( const auto & airLoopHVACUnitaryHeatPumpAirToAir : airLoopHVACUnitaryHeatPumpAirToAirs )
    {
      if( boost::optional<HVACComponent> supplementalHeatingCoil = airLoopHVACUnitaryHeatPumpAirToAir.supplementalHeatingCoil() )
      {
        if( supplementalHeatingCoil->handle() == this->handle() )
        {
          return airLoopHVACUnitaryHeatPumpAirToAir;
        }
      }
    }

    // AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed
    {
      auto systems = this->model().getConcreteModelObjects<AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed>();

      for( const auto & system : systems ) {
        auto heatingCoil = system.heatingCoil();
        if( heatingCoil.handle() == this->handle() ) {
          return system;
        }
        auto supHeatingCoil = system.supplementalHeatingCoil();
        if( supHeatingCoil.handle() == this->handle() ) {
          return system;
        }
      }
    }

    return boost::none;
  }


  boost::optional<ZoneHVACComponent> CoilHeatingGas_Impl::containingZoneHVACComponent() const
  {
    // ZoneHVACWaterToAirHeatPump

    std::vector<ZoneHVACWaterToAirHeatPump> zoneHVACWaterToAirHeatPumps;

    zoneHVACWaterToAirHeatPumps = this->model().getConcreteModelObjects<ZoneHVACWaterToAirHeatPump>();

    for( const auto & zoneHVACWaterToAirHeatPump : zoneHVACWaterToAirHeatPumps )
    {
      if( boost::optional<HVACComponent> coil = zoneHVACWaterToAirHeatPump.supplementalHeatingCoil() )
      {
        if( coil->handle() == this->handle() )
        {
          return zoneHVACWaterToAirHeatPump;
        }
      }
    }

    return boost::none;
  }

  boost::optional<Schedule> CoilHeatingGas_Impl::optionalAvailabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Heating_GasFields::AvailabilityScheduleName);
  }

  boost::optional<ModelObject> CoilHeatingGas_Impl::availabilityScheduleAsModelObject() const {
    OptionalModelObject result = availabilitySchedule();
    return result;
  }

  bool CoilHeatingGas_Impl::setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setAvailabilitySchedule(schedule);
      }
    }
    return false;
  }

  boost::optional<double> CoilHeatingGas_Impl::nominalCapacity() const {
    return getDouble(OS_Coil_Heating_GasFields::NominalCapacity,true);
  }

  OSOptionalQuantity CoilHeatingGas_Impl::getNominalCapacity(bool returnIP) const {
    OptionalDouble value = nominalCapacity();
    return getQuantityFromDouble(OS_Coil_Heating_GasFields::NominalCapacity, value, returnIP);
  }

  bool CoilHeatingGas_Impl::isNominalCapacityAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Heating_GasFields::NominalCapacity, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  void CoilHeatingGas_Impl::setNominalCapacity(boost::optional<double> nominalCapacity) {
    bool result(false);
    if (nominalCapacity) {
      result = setDouble(OS_Coil_Heating_GasFields::NominalCapacity, nominalCapacity.get());
    }
    else {
      resetNominalCapacity();
      result = true;
    }
    OS_ASSERT(result);
  }

  bool CoilHeatingGas_Impl::setNominalCapacity(const OSOptionalQuantity& nominalCapacity) {
    bool result(false);
    OptionalDouble value;
    if (nominalCapacity.isSet()) {
      value = getDoubleFromQuantity(OS_Coil_Heating_GasFields::NominalCapacity,nominalCapacity.get());
      if (value) {
        setNominalCapacity(value);
        result = true;
      }
    }
    else {
      setNominalCapacity(value);
      result = true;
    }
    return result;
  }

  void CoilHeatingGas_Impl::resetNominalCapacity() {
    bool result = setString(OS_Coil_Heating_GasFields::NominalCapacity, "");
    OS_ASSERT(result);
  }

  void CoilHeatingGas_Impl::autosizeNominalCapacity() {
    bool result = setString(OS_Coil_Heating_GasFields::NominalCapacity, "autosize");
    OS_ASSERT(result);
  }

  boost::optional<Curve> CoilHeatingGas_Impl::partLoadFractionCorrelationCurve() const
  {
    boost::optional<Curve> curve;

    curve = getObject<ModelObject>().getModelObjectTarget<Curve>(
              OS_Coil_Heating_GasFields::PartLoadFractionCorrelationCurveName);

    return curve;
  }

  bool CoilHeatingGas_Impl::setPartLoadFractionCorrelationCurve( const Curve& curve )
  {
    bool accepted = false;

    if( curve.optionalCast<CurveQuadratic>() || curve.optionalCast<CurveCubic>() )
    {
      accepted = setPointer(OS_Coil_Heating_GasFields::PartLoadFractionCorrelationCurveName,curve.handle());
    }

    return accepted;
  }

  void CoilHeatingGas_Impl::resetPartLoadFractionCorrelationCurve()
  {
    setString(OS_Coil_Heating_GasFields::PartLoadFractionCorrelationCurveName,"");
  }

  std::vector<ModelObject> CoilHeatingGas_Impl::children() const
  {
    std::vector<ModelObject> result;

    if( boost::optional<Curve> curve = partLoadFractionCorrelationCurve() )
    {
      result.push_back(curve.get());
    }

    return result;
  }

  ModelObject CoilHeatingGas_Impl::clone(Model model) const
  {
    CoilHeatingGas newCoil = StraightComponent_Impl::clone(model).cast<CoilHeatingGas>();

    return newCoil;
  }

  bool CoilHeatingGas_Impl::addToNode(Node & node)
  {
    if( boost::optional<AirLoopHVAC> airLoop = node.airLoopHVAC() )
    {
      if( ! airLoop->demandComponent(node.handle()) )
      {
        return StraightComponent_Impl::addToNode( node );
      }
    }

    return false;
  }

}// detail

// create a new CoilHeatingGas object in the model's workspace
CoilHeatingGas::CoilHeatingGas(const Model& model,
                               Schedule& schedule)
  : StraightComponent(CoilHeatingGas::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilHeatingGas_Impl>());
  bool ok = setAvailableSchedule(schedule);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s availability schedule to "
                  << schedule.briefDescription() << ".");
  }
  setGasBurnerEfficiency(0.8);
  setString(openstudio::OS_Coil_Heating_GasFields::NominalCapacity,"AutoSize");
  setParasiticElectricLoad(0);
  setParasiticGasLoad(0);
}

CoilHeatingGas::CoilHeatingGas(const Model& model)
  : StraightComponent(CoilHeatingGas::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilHeatingGas_Impl>());

  auto schedule = model.alwaysOnDiscreteSchedule();
  setAvailableSchedule(schedule);

  setGasBurnerEfficiency(0.8);
  setString(openstudio::OS_Coil_Heating_GasFields::NominalCapacity,"AutoSize");
  setParasiticElectricLoad(0);
  setParasiticGasLoad(0);
}

CoilHeatingGas::CoilHeatingGas(std::shared_ptr<detail::CoilHeatingGas_Impl> p)
  : StraightComponent(std::move(p))
{}

Schedule CoilHeatingGas::availabilitySchedule() const
{
  return getImpl<detail::CoilHeatingGas_Impl>()->availabilitySchedule();
}
Schedule CoilHeatingGas::availableSchedule() const
{
  return getImpl<detail::CoilHeatingGas_Impl>()->availabilitySchedule();
}
bool CoilHeatingGas::setAvailabilitySchedule(Schedule & schedule )
{
  return getImpl<detail::CoilHeatingGas_Impl>()->setAvailabilitySchedule( schedule );
}
bool CoilHeatingGas::setAvailableSchedule(Schedule & schedule )
{
  return getImpl<detail::CoilHeatingGas_Impl>()->setAvailabilitySchedule( schedule );
}

std::vector<std::string> CoilHeatingGas::validFuelTypeValues()
{
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Coil_Heating_GasFields::FuelType);
}

std::string CoilHeatingGas::fuelType() const
{
  return getImpl<detail::CoilHeatingGas_Impl>()->fuelType();
}

bool CoilHeatingGas::setFuelType(const std::string& fuelType)
{
  return getImpl<detail::CoilHeatingGas_Impl>()->setFuelType(fuelType);
}

void CoilHeatingGas::resetFuelType()
{
  getImpl<detail::CoilHeatingGas_Impl>()->resetFuelType();
}

double CoilHeatingGas::gasBurnerEfficiency() const
{
  return getImpl<detail::CoilHeatingGas_Impl>()->gasBurnerEfficiency();
}

void CoilHeatingGas::setGasBurnerEfficiency(double val)
{
  getImpl<detail::CoilHeatingGas_Impl>()->setGasBurnerEfficiency(val);
}

double CoilHeatingGas::parasiticElectricLoad() const
{
  return getImpl<detail::CoilHeatingGas_Impl>()->parasiticElectricLoad();
}

void CoilHeatingGas::setParasiticElectricLoad(double val)
{
  getImpl<detail::CoilHeatingGas_Impl>()->setParasiticElectricLoad(val);
}

double CoilHeatingGas::parasiticGasLoad() const
{
  return getImpl<detail::CoilHeatingGas_Impl>()->parasiticGasLoad();
}

void CoilHeatingGas::setParasiticGasLoad(double val)
{
  getImpl<detail::CoilHeatingGas_Impl>()->setParasiticGasLoad(val);
}

boost::optional<double> CoilHeatingGas::nominalCapacity() const {
  return getImpl<detail::CoilHeatingGas_Impl>()->nominalCapacity();
}

OSOptionalQuantity CoilHeatingGas::getNominalCapacity(bool returnIP) const {
  return getImpl<detail::CoilHeatingGas_Impl>()->getNominalCapacity(returnIP);
}

bool CoilHeatingGas::isNominalCapacityAutosized() const {
  return getImpl<detail::CoilHeatingGas_Impl>()->isNominalCapacityAutosized();
}

void CoilHeatingGas::setNominalCapacity(double nominalCapacity) {
  getImpl<detail::CoilHeatingGas_Impl>()->setNominalCapacity(nominalCapacity);
}

bool CoilHeatingGas::setNominalCapacity(const Quantity& nominalCapacity) {
  return getImpl<detail::CoilHeatingGas_Impl>()->setNominalCapacity(nominalCapacity);
}

void CoilHeatingGas::resetNominalCapacity() {
  getImpl<detail::CoilHeatingGas_Impl>()->resetNominalCapacity();
}

void CoilHeatingGas::autosizeNominalCapacity() {
  getImpl<detail::CoilHeatingGas_Impl>()->autosizeNominalCapacity();
}

boost::optional<Curve> CoilHeatingGas::partLoadFractionCorrelationCurve() const
{
  return getImpl<detail::CoilHeatingGas_Impl>()->partLoadFractionCorrelationCurve();
}

bool CoilHeatingGas::setPartLoadFractionCorrelationCurve( const Curve& curve )
{
  return getImpl<detail::CoilHeatingGas_Impl>()->setPartLoadFractionCorrelationCurve(curve);
}

void CoilHeatingGas::resetPartLoadFractionCorrelationCurve()
{
  getImpl<detail::CoilHeatingGas_Impl>()->resetPartLoadFractionCorrelationCurve();
}

IddObjectType CoilHeatingGas::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Coil_Heating_Gas);
  return result;
}

} // model
} // openstudio
