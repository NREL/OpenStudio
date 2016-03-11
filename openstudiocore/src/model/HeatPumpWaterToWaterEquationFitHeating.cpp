/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#include "HeatPumpWaterToWaterEquationFitHeating.hpp"
#include "HeatPumpWaterToWaterEquationFitHeating_Impl.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_HeatPump_WaterToWater_EquationFit_Heating_FieldEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  HeatPumpWaterToWaterEquationFitHeating_Impl::HeatPumpWaterToWaterEquationFitHeating_Impl(const IdfObject& idfObject,
                                                                                           Model_Impl* model,
                                                                                           bool keepHandle)
    : WaterToWaterComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == HeatPumpWaterToWaterEquationFitHeating::iddObjectType());
  }

  HeatPumpWaterToWaterEquationFitHeating_Impl::HeatPumpWaterToWaterEquationFitHeating_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                           Model_Impl* model,
                                                                                           bool keepHandle)
    : WaterToWaterComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == HeatPumpWaterToWaterEquationFitHeating::iddObjectType());
  }

  HeatPumpWaterToWaterEquationFitHeating_Impl::HeatPumpWaterToWaterEquationFitHeating_Impl(const HeatPumpWaterToWaterEquationFitHeating_Impl& other,
                                                                                           Model_Impl* model,
                                                                                           bool keepHandle)
    : WaterToWaterComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& HeatPumpWaterToWaterEquationFitHeating_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType HeatPumpWaterToWaterEquationFitHeating_Impl::iddObjectType() const {
    return HeatPumpWaterToWaterEquationFitHeating::iddObjectType();
  }

  double HeatPumpWaterToWaterEquationFitHeating_Impl::ratedLoadSideFlowRate() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::RatedLoadSideFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpWaterToWaterEquationFitHeating_Impl::ratedSourceSideFlowRate() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::RatedSourceSideFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpWaterToWaterEquationFitHeating_Impl::ratedHeatingCapacity() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::RatedHeatingCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpWaterToWaterEquationFitHeating_Impl::ratedHeatingPowerConsumption() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::RatedHeatingPowerConsumption,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpWaterToWaterEquationFitHeating_Impl::heatingCapacityCoefficient1() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCapacityCoefficient1,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpWaterToWaterEquationFitHeating_Impl::heatingCapacityCoefficient2() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCapacityCoefficient2,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpWaterToWaterEquationFitHeating_Impl::heatingCapacityCoefficient3() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCapacityCoefficient3,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpWaterToWaterEquationFitHeating_Impl::heatingCapacityCoefficient4() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCapacityCoefficient4,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpWaterToWaterEquationFitHeating_Impl::heatingCapacityCoefficient5() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCapacityCoefficient5,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpWaterToWaterEquationFitHeating_Impl::heatingCompressorPowerCoefficient1() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCompressorPowerCoefficient1,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpWaterToWaterEquationFitHeating_Impl::heatingCompressorPowerCoefficient2() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCompressorPowerCoefficient2,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpWaterToWaterEquationFitHeating_Impl::heatingCompressorPowerCoefficient3() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCompressorPowerCoefficient3,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpWaterToWaterEquationFitHeating_Impl::heatingCompressorPowerCoefficient4() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCompressorPowerCoefficient4,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpWaterToWaterEquationFitHeating_Impl::heatingCompressorPowerCoefficient5() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCompressorPowerCoefficient5,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool HeatPumpWaterToWaterEquationFitHeating_Impl::setRatedLoadSideFlowRate(double ratedLoadSideFlowRate) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::RatedLoadSideFlowRate, ratedLoadSideFlowRate);
    return result;
  }

  bool HeatPumpWaterToWaterEquationFitHeating_Impl::setRatedSourceSideFlowRate(double ratedSourceSideFlowRate) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::RatedSourceSideFlowRate, ratedSourceSideFlowRate);
    return result;
  }

  bool HeatPumpWaterToWaterEquationFitHeating_Impl::setRatedHeatingCapacity(double ratedHeatingCapacity) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::RatedHeatingCapacity, ratedHeatingCapacity);
    return result;
  }

  bool HeatPumpWaterToWaterEquationFitHeating_Impl::setRatedHeatingPowerConsumption(double ratedHeatingPowerConsumption) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::RatedHeatingPowerConsumption, ratedHeatingPowerConsumption);
    return result;
  }

  void HeatPumpWaterToWaterEquationFitHeating_Impl::setHeatingCapacityCoefficient1(double heatingCapacityCoefficient1) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCapacityCoefficient1, heatingCapacityCoefficient1);
    OS_ASSERT(result);
  }

  void HeatPumpWaterToWaterEquationFitHeating_Impl::setHeatingCapacityCoefficient2(double heatingCapacityCoefficient2) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCapacityCoefficient2, heatingCapacityCoefficient2);
    OS_ASSERT(result);
  }

  void HeatPumpWaterToWaterEquationFitHeating_Impl::setHeatingCapacityCoefficient3(double heatingCapacityCoefficient3) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCapacityCoefficient3, heatingCapacityCoefficient3);
    OS_ASSERT(result);
  }

  void HeatPumpWaterToWaterEquationFitHeating_Impl::setHeatingCapacityCoefficient4(double heatingCapacityCoefficient4) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCapacityCoefficient4, heatingCapacityCoefficient4);
    OS_ASSERT(result);
  }

  void HeatPumpWaterToWaterEquationFitHeating_Impl::setHeatingCapacityCoefficient5(double heatingCapacityCoefficient5) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCapacityCoefficient5, heatingCapacityCoefficient5);
    OS_ASSERT(result);
  }

  void HeatPumpWaterToWaterEquationFitHeating_Impl::setHeatingCompressorPowerCoefficient1(double heatingCompressorPowerCoefficient1) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCompressorPowerCoefficient1, heatingCompressorPowerCoefficient1);
    OS_ASSERT(result);
  }

  void HeatPumpWaterToWaterEquationFitHeating_Impl::setHeatingCompressorPowerCoefficient2(double heatingCompressorPowerCoefficient2) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCompressorPowerCoefficient2, heatingCompressorPowerCoefficient2);
    OS_ASSERT(result);
  }

  void HeatPumpWaterToWaterEquationFitHeating_Impl::setHeatingCompressorPowerCoefficient3(double heatingCompressorPowerCoefficient3) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCompressorPowerCoefficient3, heatingCompressorPowerCoefficient3);
    OS_ASSERT(result);
  }

  void HeatPumpWaterToWaterEquationFitHeating_Impl::setHeatingCompressorPowerCoefficient4(double heatingCompressorPowerCoefficient4) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCompressorPowerCoefficient4, heatingCompressorPowerCoefficient4);
    OS_ASSERT(result);
  }

  void HeatPumpWaterToWaterEquationFitHeating_Impl::setHeatingCompressorPowerCoefficient5(double heatingCompressorPowerCoefficient5) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCompressorPowerCoefficient5, heatingCompressorPowerCoefficient5);
    OS_ASSERT(result);
  }

  unsigned HeatPumpWaterToWaterEquationFitHeating_Impl::supplyInletPort()
  {
    return OS_HeatPump_WaterToWater_EquationFit_HeatingFields::SourceSideInletNodeName;
  }

  unsigned HeatPumpWaterToWaterEquationFitHeating_Impl::supplyOutletPort()
  {
    return OS_HeatPump_WaterToWater_EquationFit_HeatingFields::SourceSideOutletNodeName;
  }

  unsigned HeatPumpWaterToWaterEquationFitHeating_Impl::demandInletPort()
  {
    return OS_HeatPump_WaterToWater_EquationFit_HeatingFields::LoadSideInletNodeName;
  }

  unsigned HeatPumpWaterToWaterEquationFitHeating_Impl::demandOutletPort()
  {
    return OS_HeatPump_WaterToWater_EquationFit_HeatingFields::LoadSideOutletNodeName;
  }

} // detail

HeatPumpWaterToWaterEquationFitHeating::HeatPumpWaterToWaterEquationFitHeating(const Model& model)
  : WaterToWaterComponent(HeatPumpWaterToWaterEquationFitHeating::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>());

  setRatedLoadSideFlowRate(1.89E-03);
  setRatedSourceSideFlowRate(1.89E-03);
  setRatedHeatingCapacity(39040.92);
  setRatedHeatingPowerConsumption(5130.00);
  setHeatingCapacityCoefficient1(-3.33491153);
  setHeatingCapacityCoefficient2(-0.51451946);
  setHeatingCapacityCoefficient3(4.51592706);
  setHeatingCapacityCoefficient4(0.01797107);
  setHeatingCapacityCoefficient5(0.155797661);
  setHeatingCompressorPowerCoefficient1(-8.93121751);
  setHeatingCompressorPowerCoefficient2(8.57035762);
  setHeatingCompressorPowerCoefficient3(1.29660976);
  setHeatingCompressorPowerCoefficient4(-0.21629222);
  setHeatingCompressorPowerCoefficient5(0.033862378);
}

IddObjectType HeatPumpWaterToWaterEquationFitHeating::iddObjectType() {
  return IddObjectType(IddObjectType::OS_HeatPump_WaterToWater_EquationFit_Heating);
}

double HeatPumpWaterToWaterEquationFitHeating::ratedLoadSideFlowRate() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->ratedLoadSideFlowRate();
}

double HeatPumpWaterToWaterEquationFitHeating::ratedSourceSideFlowRate() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->ratedSourceSideFlowRate();
}

double HeatPumpWaterToWaterEquationFitHeating::ratedHeatingCapacity() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->ratedHeatingCapacity();
}

double HeatPumpWaterToWaterEquationFitHeating::ratedHeatingPowerConsumption() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->ratedHeatingPowerConsumption();
}

double HeatPumpWaterToWaterEquationFitHeating::heatingCapacityCoefficient1() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->heatingCapacityCoefficient1();
}

double HeatPumpWaterToWaterEquationFitHeating::heatingCapacityCoefficient2() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->heatingCapacityCoefficient2();
}

double HeatPumpWaterToWaterEquationFitHeating::heatingCapacityCoefficient3() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->heatingCapacityCoefficient3();
}

double HeatPumpWaterToWaterEquationFitHeating::heatingCapacityCoefficient4() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->heatingCapacityCoefficient4();
}

double HeatPumpWaterToWaterEquationFitHeating::heatingCapacityCoefficient5() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->heatingCapacityCoefficient5();
}

double HeatPumpWaterToWaterEquationFitHeating::heatingCompressorPowerCoefficient1() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->heatingCompressorPowerCoefficient1();
}

double HeatPumpWaterToWaterEquationFitHeating::heatingCompressorPowerCoefficient2() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->heatingCompressorPowerCoefficient2();
}

double HeatPumpWaterToWaterEquationFitHeating::heatingCompressorPowerCoefficient3() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->heatingCompressorPowerCoefficient3();
}

double HeatPumpWaterToWaterEquationFitHeating::heatingCompressorPowerCoefficient4() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->heatingCompressorPowerCoefficient4();
}

double HeatPumpWaterToWaterEquationFitHeating::heatingCompressorPowerCoefficient5() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->heatingCompressorPowerCoefficient5();
}

bool HeatPumpWaterToWaterEquationFitHeating::setRatedLoadSideFlowRate(double ratedLoadSideFlowRate) {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->setRatedLoadSideFlowRate(ratedLoadSideFlowRate);
}

bool HeatPumpWaterToWaterEquationFitHeating::setRatedSourceSideFlowRate(double ratedSourceSideFlowRate) {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->setRatedSourceSideFlowRate(ratedSourceSideFlowRate);
}

bool HeatPumpWaterToWaterEquationFitHeating::setRatedHeatingCapacity(double ratedHeatingCapacity) {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->setRatedHeatingCapacity(ratedHeatingCapacity);
}

bool HeatPumpWaterToWaterEquationFitHeating::setRatedHeatingPowerConsumption(double ratedHeatingPowerConsumption) {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->setRatedHeatingPowerConsumption(ratedHeatingPowerConsumption);
}

void HeatPumpWaterToWaterEquationFitHeating::setHeatingCapacityCoefficient1(double heatingCapacityCoefficient1) {
  getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->setHeatingCapacityCoefficient1(heatingCapacityCoefficient1);
}

void HeatPumpWaterToWaterEquationFitHeating::setHeatingCapacityCoefficient2(double heatingCapacityCoefficient2) {
  getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->setHeatingCapacityCoefficient2(heatingCapacityCoefficient2);
}

void HeatPumpWaterToWaterEquationFitHeating::setHeatingCapacityCoefficient3(double heatingCapacityCoefficient3) {
  getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->setHeatingCapacityCoefficient3(heatingCapacityCoefficient3);
}

void HeatPumpWaterToWaterEquationFitHeating::setHeatingCapacityCoefficient4(double heatingCapacityCoefficient4) {
  getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->setHeatingCapacityCoefficient4(heatingCapacityCoefficient4);
}

void HeatPumpWaterToWaterEquationFitHeating::setHeatingCapacityCoefficient5(double heatingCapacityCoefficient5) {
  getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->setHeatingCapacityCoefficient5(heatingCapacityCoefficient5);
}

void HeatPumpWaterToWaterEquationFitHeating::setHeatingCompressorPowerCoefficient1(double heatingCompressorPowerCoefficient1) {
  getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->setHeatingCompressorPowerCoefficient1(heatingCompressorPowerCoefficient1);
}

void HeatPumpWaterToWaterEquationFitHeating::setHeatingCompressorPowerCoefficient2(double heatingCompressorPowerCoefficient2) {
  getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->setHeatingCompressorPowerCoefficient2(heatingCompressorPowerCoefficient2);
}

void HeatPumpWaterToWaterEquationFitHeating::setHeatingCompressorPowerCoefficient3(double heatingCompressorPowerCoefficient3) {
  getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->setHeatingCompressorPowerCoefficient3(heatingCompressorPowerCoefficient3);
}

void HeatPumpWaterToWaterEquationFitHeating::setHeatingCompressorPowerCoefficient4(double heatingCompressorPowerCoefficient4) {
  getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->setHeatingCompressorPowerCoefficient4(heatingCompressorPowerCoefficient4);
}

void HeatPumpWaterToWaterEquationFitHeating::setHeatingCompressorPowerCoefficient5(double heatingCompressorPowerCoefficient5) {
  getImpl<detail::HeatPumpWaterToWaterEquationFitHeating_Impl>()->setHeatingCompressorPowerCoefficient5(heatingCompressorPowerCoefficient5);
}

/// @cond
HeatPumpWaterToWaterEquationFitHeating::HeatPumpWaterToWaterEquationFitHeating(std::shared_ptr<detail::HeatPumpWaterToWaterEquationFitHeating_Impl> impl)
  : WaterToWaterComponent(impl)
{}
/// @endcond

} // model
} // openstudio

