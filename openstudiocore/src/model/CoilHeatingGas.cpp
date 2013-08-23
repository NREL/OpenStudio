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

#include <model/CoilHeatingGas.hpp>
#include <model/CoilHeatingGas_Impl.hpp>
#include <model/Model.hpp>
#include <model/AirTerminalSingleDuctParallelPIUReheat.hpp>
#include <model/AirTerminalSingleDuctParallelPIUReheat_Impl.hpp>
#include <model/AirTerminalSingleDuctVAVReheat.hpp>
#include <model/AirTerminalSingleDuctVAVReheat_Impl.hpp>
#include <model/AirLoopHVACUnitaryHeatPumpAirToAir.hpp>
#include <model/AirLoopHVACUnitaryHeatPumpAirToAir_Impl.hpp>
#include <model/ZoneHVACWaterToAirHeatPump.hpp>
#include <model/ZoneHVACWaterToAirHeatPump_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/CurveCubic.hpp>
#include <model/CurveCubic_Impl.hpp>
#include <model/CurveQuadratic.hpp>
#include <model/CurveQuadratic_Impl.hpp>
#include <utilities/idd/OS_Coil_Heating_Gas_FieldEnums.hxx>
#include <utilities/core/Compare.hpp>
#include <utilities/core/Assert.hpp>
#include <boost/foreach.hpp>
#include <utilities/units/Quantity.hpp>
#include <utilities/units/OSOptionalQuantity.hpp>

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

  double CoilHeatingGas_Impl::gasBurnerEfficiency()
  {
    return this->getDouble(OS_Coil_Heating_GasFields::GasBurnerEfficiency).get();
  }

  void CoilHeatingGas_Impl::setGasBurnerEfficiency(double val)
  {
    this->setDouble(OS_Coil_Heating_GasFields::GasBurnerEfficiency,val);
  }

  double CoilHeatingGas_Impl::parasiticElectricLoad()
  {
    return this->getDouble(OS_Coil_Heating_GasFields::ParasiticElectricLoad).get();
  }

  void CoilHeatingGas_Impl::setParasiticElectricLoad(double val)
  {
    this->setDouble(OS_Coil_Heating_GasFields::ParasiticElectricLoad,val);
  }

  double CoilHeatingGas_Impl::parasiticGasLoad()
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

    // AirTerminalSingleDuctVAVReheat

    std::vector<AirTerminalSingleDuctVAVReheat> airTerminalSingleDuctVAVReheatObjects;

    airTerminalSingleDuctVAVReheatObjects = this->model().getModelObjects<AirTerminalSingleDuctVAVReheat>();

    for( std::vector<AirTerminalSingleDuctVAVReheat>::iterator it = airTerminalSingleDuctVAVReheatObjects.begin();
    it < airTerminalSingleDuctVAVReheatObjects.end();
    it++ )
    {
      if( boost::optional<HVACComponent> coil = it->reheatCoil() )
      {
        if( coil->handle() == this->handle() )
        {
          return *it;
        }
      }
    }

    // AirTerminalSingleDuctParallelPIUReheat

    std::vector<AirTerminalSingleDuctParallelPIUReheat> airTerminalSingleDuctParallelPIUReheatObjects;

    airTerminalSingleDuctParallelPIUReheatObjects = this->model().getModelObjects<AirTerminalSingleDuctParallelPIUReheat>();

    for( std::vector<AirTerminalSingleDuctParallelPIUReheat>::iterator it = airTerminalSingleDuctParallelPIUReheatObjects.begin();
    it < airTerminalSingleDuctParallelPIUReheatObjects.end();
    it++ )
    {
      if( boost::optional<HVACComponent> coil = it->reheatCoil() )
      {
        if( coil->handle() == this->handle() )
        {
          return *it;
        }
      }
    }

    // AirLoopHVACUnitaryHeatPumpAirToAir

    std::vector<AirLoopHVACUnitaryHeatPumpAirToAir> airLoopHVACUnitaryHeatPumpAirToAirs;

    airLoopHVACUnitaryHeatPumpAirToAirs = this->model().getModelObjects<AirLoopHVACUnitaryHeatPumpAirToAir>();

    for( std::vector<AirLoopHVACUnitaryHeatPumpAirToAir>::iterator it = airLoopHVACUnitaryHeatPumpAirToAirs.begin();
    it < airLoopHVACUnitaryHeatPumpAirToAirs.end();
    it++ )
    {
      if( boost::optional<HVACComponent> supplementalHeatingCoil = it->supplementalHeatingCoil() )
      {
        if( supplementalHeatingCoil->handle() == this->handle() )
        {
          return *it;
        }
      }
    }

    return boost::none;
  }


  boost::optional<ZoneHVACComponent> CoilHeatingGas_Impl::containingZoneHVACComponent() const
  {
    // ZoneHVACWaterToAirHeatPump

    std::vector<ZoneHVACWaterToAirHeatPump> zoneHVACWaterToAirHeatPumps;

    zoneHVACWaterToAirHeatPumps = this->model().getModelObjects<ZoneHVACWaterToAirHeatPump>();

    for( std::vector<ZoneHVACWaterToAirHeatPump>::iterator it = zoneHVACWaterToAirHeatPumps.begin();
    it < zoneHVACWaterToAirHeatPumps.end();
    it++ )
    {
      if( boost::optional<HVACComponent> coil = it->supplementalHeatingCoil() )
      {
        if( coil->handle() == this->handle() )
        {
          return *it;
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
    CoilHeatingGas newCoil = ModelObject_Impl::clone(model).cast<CoilHeatingGas>();

    if( boost::optional<Curve> curve1 = partLoadFractionCorrelationCurve() )
    {
      newCoil.setPartLoadFractionCorrelationCurve(curve1->clone(model).cast<Curve>());
    }

    return newCoil;
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

CoilHeatingGas::CoilHeatingGas(boost::shared_ptr<detail::CoilHeatingGas_Impl> p)
  : StraightComponent(p)
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

double CoilHeatingGas::gasBurnerEfficiency()
{
  return getImpl<detail::CoilHeatingGas_Impl>()->gasBurnerEfficiency();
}

void CoilHeatingGas::setGasBurnerEfficiency(double val)
{
  getImpl<detail::CoilHeatingGas_Impl>()->setGasBurnerEfficiency(val);
}

double CoilHeatingGas::parasiticElectricLoad()
{
  return getImpl<detail::CoilHeatingGas_Impl>()->parasiticElectricLoad();
}

void CoilHeatingGas::setParasiticElectricLoad(double val)
{
  getImpl<detail::CoilHeatingGas_Impl>()->setParasiticElectricLoad(val);
}

double CoilHeatingGas::parasiticGasLoad()
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
