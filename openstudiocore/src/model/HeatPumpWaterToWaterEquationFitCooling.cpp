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

#include "HeatPumpWaterToWaterEquationFitCooling.hpp"
#include "HeatPumpWaterToWaterEquationFitCooling_Impl.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_HeatPump_WaterToWater_EquationFit_Cooling_FieldEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  HeatPumpWaterToWaterEquationFitCooling_Impl::HeatPumpWaterToWaterEquationFitCooling_Impl(const IdfObject& idfObject,
                                                                                           Model_Impl* model,
                                                                                           bool keepHandle)
    : WaterToWaterComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == HeatPumpWaterToWaterEquationFitCooling::iddObjectType());
  }

  HeatPumpWaterToWaterEquationFitCooling_Impl::HeatPumpWaterToWaterEquationFitCooling_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                           Model_Impl* model,
                                                                                           bool keepHandle)
    : WaterToWaterComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == HeatPumpWaterToWaterEquationFitCooling::iddObjectType());
  }

  HeatPumpWaterToWaterEquationFitCooling_Impl::HeatPumpWaterToWaterEquationFitCooling_Impl(const HeatPumpWaterToWaterEquationFitCooling_Impl& other,
                                                                                           Model_Impl* model,
                                                                                           bool keepHandle)
    : WaterToWaterComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& HeatPumpWaterToWaterEquationFitCooling_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType HeatPumpWaterToWaterEquationFitCooling_Impl::iddObjectType() const {
    return HeatPumpWaterToWaterEquationFitCooling::iddObjectType();
  }

  double HeatPumpWaterToWaterEquationFitCooling_Impl::ratedLoadSideFlowRate() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::RatedLoadSideFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpWaterToWaterEquationFitCooling_Impl::ratedSourceSideFlowRate() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::RatedSourceSideFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpWaterToWaterEquationFitCooling_Impl::ratedCoolingCapacity() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::RatedCoolingCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpWaterToWaterEquationFitCooling_Impl::ratedCoolingPowerConsumption() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::RatedCoolingPowerConsumption,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpWaterToWaterEquationFitCooling_Impl::coolingCapacityCoefficient1() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCapacityCoefficient1,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpWaterToWaterEquationFitCooling_Impl::coolingCapacityCoefficient2() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCapacityCoefficient2,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpWaterToWaterEquationFitCooling_Impl::coolingCapacityCoefficient3() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCapacityCoefficient3,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpWaterToWaterEquationFitCooling_Impl::coolingCapacityCoefficient4() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCapacityCoefficient4,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpWaterToWaterEquationFitCooling_Impl::coolingCapacityCoefficient5() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCapacityCoefficient5,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpWaterToWaterEquationFitCooling_Impl::coolingCompressorPowerCoefficient1() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCompressorPowerCoefficient1,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpWaterToWaterEquationFitCooling_Impl::coolingCompressorPowerCoefficient2() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCompressorPowerCoefficient2,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpWaterToWaterEquationFitCooling_Impl::coolingCompressorPowerCoefficient3() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCompressorPowerCoefficient3,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpWaterToWaterEquationFitCooling_Impl::coolingCompressorPowerCoefficient4() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCompressorPowerCoefficient4,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatPumpWaterToWaterEquationFitCooling_Impl::coolingCompressorPowerCoefficient5() const {
    boost::optional<double> value = getDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCompressorPowerCoefficient5,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool HeatPumpWaterToWaterEquationFitCooling_Impl::setRatedLoadSideFlowRate(double ratedLoadSideFlowRate) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::RatedLoadSideFlowRate, ratedLoadSideFlowRate);
    return result;
  }

  bool HeatPumpWaterToWaterEquationFitCooling_Impl::setRatedSourceSideFlowRate(double ratedSourceSideFlowRate) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::RatedSourceSideFlowRate, ratedSourceSideFlowRate);
    return result;
  }

  bool HeatPumpWaterToWaterEquationFitCooling_Impl::setRatedCoolingCapacity(double ratedCoolingCapacity) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::RatedCoolingCapacity, ratedCoolingCapacity);
    return result;
  }

  bool HeatPumpWaterToWaterEquationFitCooling_Impl::setRatedCoolingPowerConsumption(double ratedCoolingPowerConsumption) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::RatedCoolingPowerConsumption, ratedCoolingPowerConsumption);
    return result;
  }

  void HeatPumpWaterToWaterEquationFitCooling_Impl::setCoolingCapacityCoefficient1(double coolingCapacityCoefficient1) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCapacityCoefficient1, coolingCapacityCoefficient1);
    OS_ASSERT(result);
  }

  void HeatPumpWaterToWaterEquationFitCooling_Impl::setCoolingCapacityCoefficient2(double coolingCapacityCoefficient2) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCapacityCoefficient2, coolingCapacityCoefficient2);
    OS_ASSERT(result);
  }

  void HeatPumpWaterToWaterEquationFitCooling_Impl::setCoolingCapacityCoefficient3(double coolingCapacityCoefficient3) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCapacityCoefficient3, coolingCapacityCoefficient3);
    OS_ASSERT(result);
  }

  void HeatPumpWaterToWaterEquationFitCooling_Impl::setCoolingCapacityCoefficient4(double coolingCapacityCoefficient4) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCapacityCoefficient4, coolingCapacityCoefficient4);
    OS_ASSERT(result);
  }

  void HeatPumpWaterToWaterEquationFitCooling_Impl::setCoolingCapacityCoefficient5(double coolingCapacityCoefficient5) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCapacityCoefficient5, coolingCapacityCoefficient5);
    OS_ASSERT(result);
  }

  void HeatPumpWaterToWaterEquationFitCooling_Impl::setCoolingCompressorPowerCoefficient1(double coolingCompressorPowerCoefficient1) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCompressorPowerCoefficient1, coolingCompressorPowerCoefficient1);
    OS_ASSERT(result);
  }

  void HeatPumpWaterToWaterEquationFitCooling_Impl::setCoolingCompressorPowerCoefficient2(double coolingCompressorPowerCoefficient2) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCompressorPowerCoefficient2, coolingCompressorPowerCoefficient2);
    OS_ASSERT(result);
  }

  void HeatPumpWaterToWaterEquationFitCooling_Impl::setCoolingCompressorPowerCoefficient3(double coolingCompressorPowerCoefficient3) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCompressorPowerCoefficient3, coolingCompressorPowerCoefficient3);
    OS_ASSERT(result);
  }

  void HeatPumpWaterToWaterEquationFitCooling_Impl::setCoolingCompressorPowerCoefficient4(double coolingCompressorPowerCoefficient4) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCompressorPowerCoefficient4, coolingCompressorPowerCoefficient4);
    OS_ASSERT(result);
  }

  void HeatPumpWaterToWaterEquationFitCooling_Impl::setCoolingCompressorPowerCoefficient5(double coolingCompressorPowerCoefficient5) {
    bool result = setDouble(OS_HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCompressorPowerCoefficient5, coolingCompressorPowerCoefficient5);
    OS_ASSERT(result);
  }

  unsigned HeatPumpWaterToWaterEquationFitCooling_Impl::supplyInletPort()
  {
    return OS_HeatPump_WaterToWater_EquationFit_CoolingFields::SourceSideInletNodeName;
  }

  unsigned HeatPumpWaterToWaterEquationFitCooling_Impl::supplyOutletPort()
  {
    return OS_HeatPump_WaterToWater_EquationFit_CoolingFields::SourceSideOutletNodeName;
  }

  unsigned HeatPumpWaterToWaterEquationFitCooling_Impl::demandInletPort()
  {
    return OS_HeatPump_WaterToWater_EquationFit_CoolingFields::LoadSideInletNodeName;
  }

  unsigned HeatPumpWaterToWaterEquationFitCooling_Impl::demandOutletPort()
  {
    return OS_HeatPump_WaterToWater_EquationFit_CoolingFields::LoadSideOutletNodeName;
  }

} // detail

HeatPumpWaterToWaterEquationFitCooling::HeatPumpWaterToWaterEquationFitCooling(const Model& model)
  : WaterToWaterComponent(HeatPumpWaterToWaterEquationFitCooling::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>());

  setRatedLoadSideFlowRate(1.89E-03);
  setRatedSourceSideFlowRate(1.89E-03);
  setRatedCoolingCapacity(39890.91);
  setRatedCoolingPowerConsumption(4790.00);
  setCoolingCapacityCoefficient1(-1.52030596);
  setCoolingCapacityCoefficient2(3.46625667);
  setCoolingCapacityCoefficient3(-1.32267797);
  setCoolingCapacityCoefficient4(0.09395678);
  setCoolingCapacityCoefficient5(0.038975504);
  setCoolingCompressorPowerCoefficient1(-8.59564386);
  setCoolingCompressorPowerCoefficient2(0.96265085);
  setCoolingCompressorPowerCoefficient3(8.69489229);
  setCoolingCompressorPowerCoefficient4(0.02501669);
  setCoolingCompressorPowerCoefficient5(-0.20132665);
}

IddObjectType HeatPumpWaterToWaterEquationFitCooling::iddObjectType() {
  return IddObjectType(IddObjectType::OS_HeatPump_WaterToWater_EquationFit_Cooling);
}

double HeatPumpWaterToWaterEquationFitCooling::ratedLoadSideFlowRate() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->ratedLoadSideFlowRate();
}

double HeatPumpWaterToWaterEquationFitCooling::ratedSourceSideFlowRate() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->ratedSourceSideFlowRate();
}

double HeatPumpWaterToWaterEquationFitCooling::ratedCoolingCapacity() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->ratedCoolingCapacity();
}

double HeatPumpWaterToWaterEquationFitCooling::ratedCoolingPowerConsumption() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->ratedCoolingPowerConsumption();
}

double HeatPumpWaterToWaterEquationFitCooling::coolingCapacityCoefficient1() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->coolingCapacityCoefficient1();
}

double HeatPumpWaterToWaterEquationFitCooling::coolingCapacityCoefficient2() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->coolingCapacityCoefficient2();
}

double HeatPumpWaterToWaterEquationFitCooling::coolingCapacityCoefficient3() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->coolingCapacityCoefficient3();
}

double HeatPumpWaterToWaterEquationFitCooling::coolingCapacityCoefficient4() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->coolingCapacityCoefficient4();
}

double HeatPumpWaterToWaterEquationFitCooling::coolingCapacityCoefficient5() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->coolingCapacityCoefficient5();
}

double HeatPumpWaterToWaterEquationFitCooling::coolingCompressorPowerCoefficient1() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->coolingCompressorPowerCoefficient1();
}

double HeatPumpWaterToWaterEquationFitCooling::coolingCompressorPowerCoefficient2() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->coolingCompressorPowerCoefficient2();
}

double HeatPumpWaterToWaterEquationFitCooling::coolingCompressorPowerCoefficient3() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->coolingCompressorPowerCoefficient3();
}

double HeatPumpWaterToWaterEquationFitCooling::coolingCompressorPowerCoefficient4() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->coolingCompressorPowerCoefficient4();
}

double HeatPumpWaterToWaterEquationFitCooling::coolingCompressorPowerCoefficient5() const {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->coolingCompressorPowerCoefficient5();
}

bool HeatPumpWaterToWaterEquationFitCooling::setRatedLoadSideFlowRate(double ratedLoadSideFlowRate) {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->setRatedLoadSideFlowRate(ratedLoadSideFlowRate);
}

bool HeatPumpWaterToWaterEquationFitCooling::setRatedSourceSideFlowRate(double ratedSourceSideFlowRate) {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->setRatedSourceSideFlowRate(ratedSourceSideFlowRate);
}

bool HeatPumpWaterToWaterEquationFitCooling::setRatedCoolingCapacity(double ratedCoolingCapacity) {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->setRatedCoolingCapacity(ratedCoolingCapacity);
}

bool HeatPumpWaterToWaterEquationFitCooling::setRatedCoolingPowerConsumption(double ratedCoolingPowerConsumption) {
  return getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->setRatedCoolingPowerConsumption(ratedCoolingPowerConsumption);
}

void HeatPumpWaterToWaterEquationFitCooling::setCoolingCapacityCoefficient1(double coolingCapacityCoefficient1) {
  getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->setCoolingCapacityCoefficient1(coolingCapacityCoefficient1);
}

void HeatPumpWaterToWaterEquationFitCooling::setCoolingCapacityCoefficient2(double coolingCapacityCoefficient2) {
  getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->setCoolingCapacityCoefficient2(coolingCapacityCoefficient2);
}

void HeatPumpWaterToWaterEquationFitCooling::setCoolingCapacityCoefficient3(double coolingCapacityCoefficient3) {
  getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->setCoolingCapacityCoefficient3(coolingCapacityCoefficient3);
}

void HeatPumpWaterToWaterEquationFitCooling::setCoolingCapacityCoefficient4(double coolingCapacityCoefficient4) {
  getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->setCoolingCapacityCoefficient4(coolingCapacityCoefficient4);
}

void HeatPumpWaterToWaterEquationFitCooling::setCoolingCapacityCoefficient5(double coolingCapacityCoefficient5) {
  getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->setCoolingCapacityCoefficient5(coolingCapacityCoefficient5);
}

void HeatPumpWaterToWaterEquationFitCooling::setCoolingCompressorPowerCoefficient1(double coolingCompressorPowerCoefficient1) {
  getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->setCoolingCompressorPowerCoefficient1(coolingCompressorPowerCoefficient1);
}

void HeatPumpWaterToWaterEquationFitCooling::setCoolingCompressorPowerCoefficient2(double coolingCompressorPowerCoefficient2) {
  getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->setCoolingCompressorPowerCoefficient2(coolingCompressorPowerCoefficient2);
}

void HeatPumpWaterToWaterEquationFitCooling::setCoolingCompressorPowerCoefficient3(double coolingCompressorPowerCoefficient3) {
  getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->setCoolingCompressorPowerCoefficient3(coolingCompressorPowerCoefficient3);
}

void HeatPumpWaterToWaterEquationFitCooling::setCoolingCompressorPowerCoefficient4(double coolingCompressorPowerCoefficient4) {
  getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->setCoolingCompressorPowerCoefficient4(coolingCompressorPowerCoefficient4);
}

void HeatPumpWaterToWaterEquationFitCooling::setCoolingCompressorPowerCoefficient5(double coolingCompressorPowerCoefficient5) {
  getImpl<detail::HeatPumpWaterToWaterEquationFitCooling_Impl>()->setCoolingCompressorPowerCoefficient5(coolingCompressorPowerCoefficient5);
}

/// @cond
HeatPumpWaterToWaterEquationFitCooling::HeatPumpWaterToWaterEquationFitCooling(std::shared_ptr<detail::HeatPumpWaterToWaterEquationFitCooling_Impl> impl)
  : WaterToWaterComponent(impl)
{}
/// @endcond

} // model
} // openstudio

