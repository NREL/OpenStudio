/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#include <model/CoilHeatingWaterToAirHeatPumpEquationFit.hpp>
#include <model/CoilHeatingWaterToAirHeatPumpEquationFit_Impl.hpp>
#include <model/ZoneHVACComponent.hpp>
#include <model/ZoneHVACComponent_Impl.hpp>
#include <model/ZoneHVACWaterToAirHeatPump.hpp>
#include <model/ZoneHVACWaterToAirHeatPump_Impl.hpp>
#include <model/AirLoopHVACUnitarySystem.hpp>
#include <model/AirLoopHVACUnitarySystem_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/Model.hpp>
#include <utilities/idd/OS_Coil_Heating_WaterToAirHeatPump_EquationFit_FieldEnums.hxx>
#include <utilities/units/Unit.hpp>
#include <utilities/core/Compare.hpp>
#include <utilities/core/Assert.hpp>
#include <boost/foreach.hpp>

namespace openstudio {
namespace model {

namespace detail {

  //constructor
  CoilHeatingWaterToAirHeatPumpEquationFit_Impl::CoilHeatingWaterToAirHeatPumpEquationFit_Impl(const IdfObject& idfObject,
                                                                                               Model_Impl* model,
                                                                                               bool keepHandle)
    : WaterToAirComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilHeatingWaterToAirHeatPumpEquationFit::iddObjectType());
  }

  CoilHeatingWaterToAirHeatPumpEquationFit_Impl::CoilHeatingWaterToAirHeatPumpEquationFit_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                               Model_Impl* model,
                                                                                               bool keepHandle)
    : WaterToAirComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilHeatingWaterToAirHeatPumpEquationFit::iddObjectType());
  }

  CoilHeatingWaterToAirHeatPumpEquationFit_Impl::CoilHeatingWaterToAirHeatPumpEquationFit_Impl(const CoilHeatingWaterToAirHeatPumpEquationFit_Impl& other,
                                                                                               Model_Impl* model,
                                                                                               bool keepHandle)
    : WaterToAirComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoilHeatingWaterToAirHeatPumpEquationFit_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CoilHeatingWaterToAirHeatPumpEquationFit_Impl::iddObjectType() const {
    return CoilHeatingWaterToAirHeatPumpEquationFit::iddObjectType();
  }
  
  unsigned CoilHeatingWaterToAirHeatPumpEquationFit_Impl::airInletPort()
  {
    return OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::AirInletNodeName;
  }

  unsigned CoilHeatingWaterToAirHeatPumpEquationFit_Impl::airOutletPort()
  {
    return OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::AirOutletNodeName;
  }
  
  unsigned CoilHeatingWaterToAirHeatPumpEquationFit_Impl::waterInletPort()
  {
    return OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::WaterInletNodeName;
  }

  unsigned CoilHeatingWaterToAirHeatPumpEquationFit_Impl::waterOutletPort()
  {
    return OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::WaterOutletNodeName;
  }

 
  boost::optional<double> CoilHeatingWaterToAirHeatPumpEquationFit_Impl::ratedAirFlowRate() const {
    return getDouble(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedAirFlowRate,true);
  }

  bool CoilHeatingWaterToAirHeatPumpEquationFit_Impl::isRatedAirFlowRateDefaulted() const {
    return isEmpty(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedAirFlowRate);
  }

  bool CoilHeatingWaterToAirHeatPumpEquationFit_Impl::isRatedAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> CoilHeatingWaterToAirHeatPumpEquationFit_Impl::ratedWaterFlowRate() const {
    return getDouble(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedWaterFlowRate,true);
  }

  bool CoilHeatingWaterToAirHeatPumpEquationFit_Impl::isRatedWaterFlowRateDefaulted() const {
    return isEmpty(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedWaterFlowRate);
  }

  bool CoilHeatingWaterToAirHeatPumpEquationFit_Impl::isRatedWaterFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedWaterFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> CoilHeatingWaterToAirHeatPumpEquationFit_Impl::ratedHeatingCapacity() const {
    return getDouble(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedHeatingCapacity,true);
  }

  bool CoilHeatingWaterToAirHeatPumpEquationFit_Impl::isRatedHeatingCapacityDefaulted() const {
    return isEmpty(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedHeatingCapacity);
  }

  bool CoilHeatingWaterToAirHeatPumpEquationFit_Impl::isRatedHeatingCapacityAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedHeatingCapacity, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double CoilHeatingWaterToAirHeatPumpEquationFit_Impl::ratedHeatingCoefficientofPerformance() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedHeatingCoefficientofPerformance,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilHeatingWaterToAirHeatPumpEquationFit_Impl::isRatedHeatingCoefficientofPerformanceDefaulted() const {
    return isEmpty(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedHeatingCoefficientofPerformance);
  }

  double CoilHeatingWaterToAirHeatPumpEquationFit_Impl::heatingCapacityCoefficient1() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::HeatingCapacityCoefficient1,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilHeatingWaterToAirHeatPumpEquationFit_Impl::heatingCapacityCoefficient2() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::HeatingCapacityCoefficient2,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilHeatingWaterToAirHeatPumpEquationFit_Impl::heatingCapacityCoefficient3() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::HeatingCapacityCoefficient3,true);
    OS_ASSERT(value);
    return value.get();
  }

 double CoilHeatingWaterToAirHeatPumpEquationFit_Impl::heatingCapacityCoefficient4() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::HeatingCapacityCoefficient4,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilHeatingWaterToAirHeatPumpEquationFit_Impl::heatingCapacityCoefficient5() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::HeatingCapacityCoefficient5,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilHeatingWaterToAirHeatPumpEquationFit_Impl::heatingPowerConsumptionCoefficient1() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::HeatingPowerConsumptionCoefficient1,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilHeatingWaterToAirHeatPumpEquationFit_Impl::heatingPowerConsumptionCoefficient2() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::HeatingPowerConsumptionCoefficient2,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilHeatingWaterToAirHeatPumpEquationFit_Impl::heatingPowerConsumptionCoefficient3() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::HeatingPowerConsumptionCoefficient3,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilHeatingWaterToAirHeatPumpEquationFit_Impl::heatingPowerConsumptionCoefficient4() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::HeatingPowerConsumptionCoefficient4,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilHeatingWaterToAirHeatPumpEquationFit_Impl::heatingPowerConsumptionCoefficient5() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::HeatingPowerConsumptionCoefficient5,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilHeatingWaterToAirHeatPumpEquationFit_Impl::setRatedAirFlowRate(boost::optional<double> ratedAirFlowRate) {
    bool result(false);
    if (ratedAirFlowRate) {
      result = setDouble(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedAirFlowRate, ratedAirFlowRate.get());
    }
    else {
      resetRatedAirFlowRate();
      result = true;
    }
    return result;
  }

  void CoilHeatingWaterToAirHeatPumpEquationFit_Impl::resetRatedAirFlowRate() {
    bool result = setString(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedAirFlowRate, "");
    OS_ASSERT(result);
  }

  void CoilHeatingWaterToAirHeatPumpEquationFit_Impl::autosizeRatedAirFlowRate() {
    bool result = setString(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedAirFlowRate, "Autosize");
    OS_ASSERT(result);
  }

  bool CoilHeatingWaterToAirHeatPumpEquationFit_Impl::setRatedWaterFlowRate(boost::optional<double> ratedWaterFlowRate) {
    bool result(false);
    if (ratedWaterFlowRate) {
      result = setDouble(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedWaterFlowRate, ratedWaterFlowRate.get());
    }
    else {
      resetRatedWaterFlowRate();
      result = true;
    }
    return result;
  }

  void CoilHeatingWaterToAirHeatPumpEquationFit_Impl::resetRatedWaterFlowRate() {
    bool result = setString(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedWaterFlowRate, "");
    OS_ASSERT(result);
  }

  void CoilHeatingWaterToAirHeatPumpEquationFit_Impl::autosizeRatedWaterFlowRate() {
    bool result = setString(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedWaterFlowRate, "Autosize");
    OS_ASSERT(result);
  }

  bool CoilHeatingWaterToAirHeatPumpEquationFit_Impl::setRatedHeatingCapacity(boost::optional<double> ratedHeatingCapacity) {
    bool result(false);
    if (ratedHeatingCapacity) {
      result = setDouble(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedHeatingCapacity, ratedHeatingCapacity.get());
    }
    else {
      resetRatedHeatingCapacity();
      result = true;
    }
    return result;
  }

 void CoilHeatingWaterToAirHeatPumpEquationFit_Impl::resetRatedHeatingCapacity() {
    bool result = setString(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedHeatingCapacity, "");
    OS_ASSERT(result);
  }

  void CoilHeatingWaterToAirHeatPumpEquationFit_Impl::autosizeRatedHeatingCapacity() {
    bool result = setString(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedHeatingCapacity, "Autosize");
    OS_ASSERT(result);
  }

  bool CoilHeatingWaterToAirHeatPumpEquationFit_Impl::setRatedHeatingCoefficientofPerformance(double ratedHeatingCoefficientofPerformance) {
    bool result = setDouble(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedHeatingCoefficientofPerformance, ratedHeatingCoefficientofPerformance);
    return result;
  }

 void CoilHeatingWaterToAirHeatPumpEquationFit_Impl::resetRatedHeatingCoefficientofPerformance() {
    bool result = setString(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedHeatingCoefficientofPerformance, "");
    OS_ASSERT(result);
  }

  void CoilHeatingWaterToAirHeatPumpEquationFit_Impl::setHeatingCapacityCoefficient1(double heatingCapacityCoefficient1) {
    bool result = setDouble(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::HeatingCapacityCoefficient1, heatingCapacityCoefficient1);
    OS_ASSERT(result);
  }

  void CoilHeatingWaterToAirHeatPumpEquationFit_Impl::setHeatingCapacityCoefficient2(double heatingCapacityCoefficient2) {
    bool result = setDouble(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::HeatingCapacityCoefficient2, heatingCapacityCoefficient2);
    OS_ASSERT(result);
  }

 
  void CoilHeatingWaterToAirHeatPumpEquationFit_Impl::setHeatingCapacityCoefficient3(double heatingCapacityCoefficient3) {
    bool result = setDouble(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::HeatingCapacityCoefficient3, heatingCapacityCoefficient3);
    OS_ASSERT(result);
  }


  void CoilHeatingWaterToAirHeatPumpEquationFit_Impl::setHeatingCapacityCoefficient4(double heatingCapacityCoefficient4) {
    bool result = setDouble(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::HeatingCapacityCoefficient4, heatingCapacityCoefficient4);
    OS_ASSERT(result);
  }

  void CoilHeatingWaterToAirHeatPumpEquationFit_Impl::setHeatingCapacityCoefficient5(double heatingCapacityCoefficient5) {
    bool result = setDouble(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::HeatingCapacityCoefficient5, heatingCapacityCoefficient5);
    OS_ASSERT(result);
  }


  void CoilHeatingWaterToAirHeatPumpEquationFit_Impl::setHeatingPowerConsumptionCoefficient1(double heatingPowerConsumptionCoefficient1) {
    bool result = setDouble(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::HeatingPowerConsumptionCoefficient1, heatingPowerConsumptionCoefficient1);
    OS_ASSERT(result);
  }


  void CoilHeatingWaterToAirHeatPumpEquationFit_Impl::setHeatingPowerConsumptionCoefficient2(double heatingPowerConsumptionCoefficient2) {
    bool result = setDouble(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::HeatingPowerConsumptionCoefficient2, heatingPowerConsumptionCoefficient2);
    OS_ASSERT(result);
  }

  void CoilHeatingWaterToAirHeatPumpEquationFit_Impl::setHeatingPowerConsumptionCoefficient3(double heatingPowerConsumptionCoefficient3) {
    bool result = setDouble(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::HeatingPowerConsumptionCoefficient3, heatingPowerConsumptionCoefficient3);
    OS_ASSERT(result);
  }

  void CoilHeatingWaterToAirHeatPumpEquationFit_Impl::setHeatingPowerConsumptionCoefficient4(double heatingPowerConsumptionCoefficient4) {
    bool result = setDouble(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::HeatingPowerConsumptionCoefficient4, heatingPowerConsumptionCoefficient4);
    OS_ASSERT(result);
  }

  void CoilHeatingWaterToAirHeatPumpEquationFit_Impl::setHeatingPowerConsumptionCoefficient5(double heatingPowerConsumptionCoefficient5) {
    bool result = setDouble(OS_Coil_Heating_WaterToAirHeatPump_EquationFitFields::HeatingPowerConsumptionCoefficient5, heatingPowerConsumptionCoefficient5);
    OS_ASSERT(result);
  }

  boost::optional<HVACComponent> CoilHeatingWaterToAirHeatPumpEquationFit_Impl::containingHVACComponent() const
  {
    // AirLoopHVACUnitarySystem
    std::vector<AirLoopHVACUnitarySystem> airLoopHVACUnitarySystems = this->model().getConcreteModelObjects<AirLoopHVACUnitarySystem>();

    for( std::vector<AirLoopHVACUnitarySystem>::iterator it = airLoopHVACUnitarySystems.begin();
    it < airLoopHVACUnitarySystems.end();
    ++it )
    {
      if( boost::optional<HVACComponent> heatingCoil = it->heatingCoil() )
      {
        if( heatingCoil->handle() == this->handle() )
        {
          return *it;
        }
      }
    }
    return boost::none;
  }

  boost::optional<ZoneHVACComponent> CoilHeatingWaterToAirHeatPumpEquationFit_Impl::containingZoneHVACComponent() const
  {
    // ZoneHVACWaterToAirHeatPump

    std::vector<ZoneHVACWaterToAirHeatPump> zoneHVACWaterToAirHeatPumps;

    zoneHVACWaterToAirHeatPumps = this->model().getConcreteModelObjects<ZoneHVACWaterToAirHeatPump>();

    for( std::vector<ZoneHVACWaterToAirHeatPump>::iterator it = zoneHVACWaterToAirHeatPumps.begin();
    it < zoneHVACWaterToAirHeatPumps.end();
    ++it )
    {
      if( boost::optional<HVACComponent> coil = it->heatingCoil() )
      {
        if( coil->handle() == this->handle() )
        {
          return *it;
        }
      }
    }

    return boost::none;
  }

} // detail

// create a new CoilHeatingWaterToAirHeatPumpEquationFit object in the model's workspace
CoilHeatingWaterToAirHeatPumpEquationFit::CoilHeatingWaterToAirHeatPumpEquationFit(const Model& model)
  : WaterToAirComponent(CoilHeatingWaterToAirHeatPumpEquationFit::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>());

   setHeatingCapacityCoefficient1(-5.50102734);
   setHeatingCapacityCoefficient2(-0.96688754);
   setHeatingCapacityCoefficient3(7.70755007);
   setHeatingCapacityCoefficient4(0.031928881);
   setHeatingCapacityCoefficient5(0.028112522);
   setHeatingPowerConsumptionCoefficient1(-7.47517858);
   setHeatingPowerConsumptionCoefficient2(6.40876653);
   setHeatingPowerConsumptionCoefficient3(1.99711665);
   setHeatingPowerConsumptionCoefficient4(-0.050682973);
   setHeatingPowerConsumptionCoefficient5(0.011385145);
}

IddObjectType CoilHeatingWaterToAirHeatPumpEquationFit::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Coil_Heating_WaterToAirHeatPump_EquationFit);
}

boost::optional<double> CoilHeatingWaterToAirHeatPumpEquationFit::ratedAirFlowRate() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->ratedAirFlowRate();
}

bool CoilHeatingWaterToAirHeatPumpEquationFit::isRatedAirFlowRateDefaulted() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->isRatedAirFlowRateDefaulted();
}

bool CoilHeatingWaterToAirHeatPumpEquationFit::isRatedAirFlowRateAutosized() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->isRatedAirFlowRateAutosized();
}

boost::optional<double> CoilHeatingWaterToAirHeatPumpEquationFit::ratedWaterFlowRate() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->ratedWaterFlowRate();
}

bool CoilHeatingWaterToAirHeatPumpEquationFit::isRatedWaterFlowRateDefaulted() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->isRatedWaterFlowRateDefaulted();
}

bool CoilHeatingWaterToAirHeatPumpEquationFit::isRatedWaterFlowRateAutosized() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->isRatedWaterFlowRateAutosized();
}

boost::optional<double> CoilHeatingWaterToAirHeatPumpEquationFit::ratedHeatingCapacity() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->ratedHeatingCapacity();
}

bool CoilHeatingWaterToAirHeatPumpEquationFit::isRatedHeatingCapacityDefaulted() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->isRatedHeatingCapacityDefaulted();
}

bool CoilHeatingWaterToAirHeatPumpEquationFit::isRatedHeatingCapacityAutosized() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->isRatedHeatingCapacityAutosized();
}

double CoilHeatingWaterToAirHeatPumpEquationFit::ratedHeatingCoefficientofPerformance() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->ratedHeatingCoefficientofPerformance();
}

bool CoilHeatingWaterToAirHeatPumpEquationFit::isRatedHeatingCoefficientofPerformanceDefaulted() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->isRatedHeatingCoefficientofPerformanceDefaulted();
}

double CoilHeatingWaterToAirHeatPumpEquationFit::heatingCapacityCoefficient1() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->heatingCapacityCoefficient1();
}

double CoilHeatingWaterToAirHeatPumpEquationFit::heatingCapacityCoefficient2() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->heatingCapacityCoefficient2();
}

double CoilHeatingWaterToAirHeatPumpEquationFit::heatingCapacityCoefficient3() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->heatingCapacityCoefficient3();
}

double CoilHeatingWaterToAirHeatPumpEquationFit::heatingCapacityCoefficient4() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->heatingCapacityCoefficient4();
}

double CoilHeatingWaterToAirHeatPumpEquationFit::heatingCapacityCoefficient5() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->heatingCapacityCoefficient5();
}

double CoilHeatingWaterToAirHeatPumpEquationFit::heatingPowerConsumptionCoefficient1() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->heatingPowerConsumptionCoefficient1();
}

double CoilHeatingWaterToAirHeatPumpEquationFit::heatingPowerConsumptionCoefficient2() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->heatingPowerConsumptionCoefficient2();
}

double CoilHeatingWaterToAirHeatPumpEquationFit::heatingPowerConsumptionCoefficient3() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->heatingPowerConsumptionCoefficient3();
}

double CoilHeatingWaterToAirHeatPumpEquationFit::heatingPowerConsumptionCoefficient4() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->heatingPowerConsumptionCoefficient4();
}

double CoilHeatingWaterToAirHeatPumpEquationFit::heatingPowerConsumptionCoefficient5() const {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->heatingPowerConsumptionCoefficient5();
}

bool CoilHeatingWaterToAirHeatPumpEquationFit::setRatedAirFlowRate(OptionalDouble ratedAirFlowRate) {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->setRatedAirFlowRate(ratedAirFlowRate);
}

void CoilHeatingWaterToAirHeatPumpEquationFit::resetRatedAirFlowRate() {
  getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->resetRatedAirFlowRate();
}

void CoilHeatingWaterToAirHeatPumpEquationFit::autosizeRatedAirFlowRate() {
  getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->autosizeRatedAirFlowRate();
}

bool CoilHeatingWaterToAirHeatPumpEquationFit::setRatedWaterFlowRate(OptionalDouble ratedWaterFlowRate) {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->setRatedWaterFlowRate(ratedWaterFlowRate);
}

void CoilHeatingWaterToAirHeatPumpEquationFit::resetRatedWaterFlowRate() {
  getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->resetRatedWaterFlowRate();
}

void CoilHeatingWaterToAirHeatPumpEquationFit::autosizeRatedWaterFlowRate() {
  getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->autosizeRatedWaterFlowRate();
}

bool CoilHeatingWaterToAirHeatPumpEquationFit::setRatedHeatingCapacity(OptionalDouble ratedHeatingCapacity) {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->setRatedHeatingCapacity(ratedHeatingCapacity);
}

void CoilHeatingWaterToAirHeatPumpEquationFit::resetRatedHeatingCapacity() {
  getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->resetRatedHeatingCapacity();
}

void CoilHeatingWaterToAirHeatPumpEquationFit::autosizeRatedHeatingCapacity() {
  getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->autosizeRatedHeatingCapacity();
}

bool CoilHeatingWaterToAirHeatPumpEquationFit::setRatedHeatingCoefficientofPerformance(double ratedHeatingCoefficientofPerformance) {
  return getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->setRatedHeatingCoefficientofPerformance(ratedHeatingCoefficientofPerformance);
}

void CoilHeatingWaterToAirHeatPumpEquationFit::resetRatedHeatingCoefficientofPerformance() {
  getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->resetRatedHeatingCoefficientofPerformance();
}

void CoilHeatingWaterToAirHeatPumpEquationFit::setHeatingCapacityCoefficient1(double heatingCapacityCoefficient1) {
  getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->setHeatingCapacityCoefficient1(heatingCapacityCoefficient1);
}

void CoilHeatingWaterToAirHeatPumpEquationFit::setHeatingCapacityCoefficient2(double heatingCapacityCoefficient2) {
  getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->setHeatingCapacityCoefficient2(heatingCapacityCoefficient2);
}

void CoilHeatingWaterToAirHeatPumpEquationFit::setHeatingCapacityCoefficient3(double heatingCapacityCoefficient3) {
  getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->setHeatingCapacityCoefficient3(heatingCapacityCoefficient3);
}

void CoilHeatingWaterToAirHeatPumpEquationFit::setHeatingCapacityCoefficient4(double heatingCapacityCoefficient4) {
  getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->setHeatingCapacityCoefficient4(heatingCapacityCoefficient4);
}

void CoilHeatingWaterToAirHeatPumpEquationFit::setHeatingCapacityCoefficient5(double heatingCapacityCoefficient5) {
  getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->setHeatingCapacityCoefficient5(heatingCapacityCoefficient5);
}

void CoilHeatingWaterToAirHeatPumpEquationFit::setHeatingPowerConsumptionCoefficient1(double heatingPowerConsumptionCoefficient1) {
  getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->setHeatingPowerConsumptionCoefficient1(heatingPowerConsumptionCoefficient1);
}

void CoilHeatingWaterToAirHeatPumpEquationFit::setHeatingPowerConsumptionCoefficient2(double heatingPowerConsumptionCoefficient2) {
  getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->setHeatingPowerConsumptionCoefficient2(heatingPowerConsumptionCoefficient2);
}

void CoilHeatingWaterToAirHeatPumpEquationFit::setHeatingPowerConsumptionCoefficient3(double heatingPowerConsumptionCoefficient3) {
  getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->setHeatingPowerConsumptionCoefficient3(heatingPowerConsumptionCoefficient3);
}

void CoilHeatingWaterToAirHeatPumpEquationFit::setHeatingPowerConsumptionCoefficient4(double heatingPowerConsumptionCoefficient4) {
  getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->setHeatingPowerConsumptionCoefficient4(heatingPowerConsumptionCoefficient4);
}

void CoilHeatingWaterToAirHeatPumpEquationFit::setHeatingPowerConsumptionCoefficient5(double heatingPowerConsumptionCoefficient5) {
  getImpl<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl>()->setHeatingPowerConsumptionCoefficient5(heatingPowerConsumptionCoefficient5);
}

/// @cond
CoilHeatingWaterToAirHeatPumpEquationFit::CoilHeatingWaterToAirHeatPumpEquationFit(boost::shared_ptr<detail::CoilHeatingWaterToAirHeatPumpEquationFit_Impl> impl)
  : WaterToAirComponent(impl)
{}
/// @endcond

} // model
} // openstudio

