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

#include "CoilCoolingWaterToAirHeatPumpEquationFit.hpp"
#include "CoilCoolingWaterToAirHeatPumpEquationFit_Impl.hpp"
#include "ZoneHVACComponent.hpp"
#include "ZoneHVACComponent_Impl.hpp"

#include "ZoneHVACWaterToAirHeatPump.hpp"
#include "ZoneHVACWaterToAirHeatPump_Impl.hpp"
#include "AirLoopHVACUnitarySystem.hpp"
#include "AirLoopHVACUnitarySystem_Impl.hpp"
#include "Model.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"

#include <utilities/idd/OS_Coil_Cooling_WaterToAirHeatPump_EquationFit_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Compare.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoilCoolingWaterToAirHeatPumpEquationFit_Impl::CoilCoolingWaterToAirHeatPumpEquationFit_Impl (const IdfObject& idfObject,
                                                                                                Model_Impl* model,
                                                                                                bool keepHandle)
    : WaterToAirComponent_Impl(idfObject,model,keepHandle)
    {
      OS_ASSERT(idfObject.iddObject().type() == CoilCoolingWaterToAirHeatPumpEquationFit::iddObjectType());
    }

  CoilCoolingWaterToAirHeatPumpEquationFit_Impl::CoilCoolingWaterToAirHeatPumpEquationFit_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                               Model_Impl* model,
                                                                                               bool keepHandle)
    : WaterToAirComponent_Impl(other,model,keepHandle)
    {
      OS_ASSERT(other.iddObject().type() == CoilCoolingWaterToAirHeatPumpEquationFit::iddObjectType());
    }

  CoilCoolingWaterToAirHeatPumpEquationFit_Impl::CoilCoolingWaterToAirHeatPumpEquationFit_Impl(const CoilCoolingWaterToAirHeatPumpEquationFit_Impl& other,
                                                                                               Model_Impl* model,
                                                                                               bool keepHandle)
    : WaterToAirComponent_Impl(other,model,keepHandle)
    {}

  ModelObject CoilCoolingWaterToAirHeatPumpEquationFit_Impl::clone(Model model) const
  {
    CoilCoolingWaterToAirHeatPumpEquationFit newCoil = WaterToAirComponent_Impl::clone(model).optionalCast<CoilCoolingWaterToAirHeatPumpEquationFit>().get();

    return newCoil;
  }

  const std::vector<std::string>& CoilCoolingWaterToAirHeatPumpEquationFit_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty())
    {
      result.push_back("Cooling Coil Electric Power");
      result.push_back("Cooling Coil Total Cooling Rate");
      result.push_back("Cooling Coil Sensible Cooling Rate");
      result.push_back("Cooling Coil Source Side Heat Transfer Rate");
      result.push_back("Cooling Coil Part Load Ratio");
      result.push_back("Cooling Coil Air Mass Flow Rate");
      result.push_back("Cooling Coil Air Inlet Temperature");
      result.push_back("Cooling Coil Air Inlet Humidity Ratio");
      result.push_back("Cooling Coil Air Outlet Temperature");
      result.push_back("Cooling Coil Air Outlet Humidity Ratio");
      result.push_back("Cooling Coil Source Side Mass Flow Rate");
      result.push_back("Cooling Coil Source Side Inlet Temperature");
      result.push_back("Cooling Coil Source Side Outlet Temperature");
      result.push_back("Cooling Coil Electric Energy");
      result.push_back("Cooling Coil Total Cooling Energy");
      result.push_back("Cooling Coil Sensible Cooling Energy");
      result.push_back("Cooling Coil Latent Cooling Energy");
      result.push_back("Cooling Coil Source Side Heat Transfer Energy");
    }
    return result;
  }

  IddObjectType CoilCoolingWaterToAirHeatPumpEquationFit_Impl::iddObjectType() const
  {
    return CoilCoolingWaterToAirHeatPumpEquationFit::iddObjectType();
  }

  std::vector<openstudio::IdfObject> CoilCoolingWaterToAirHeatPumpEquationFit_Impl::remove()
  {
    if( isRemovable() )
    {
      return WaterToAirComponent_Impl::remove();
    }

    return std::vector<IdfObject>();
  }

  boost::optional<double> CoilCoolingWaterToAirHeatPumpEquationFit_Impl::ratedAirFlowRate() const
  {
    return getDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedAirFlowRate,true);
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit_Impl::isRatedAirFlowRateDefaulted() const
  {
    return isEmpty(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedAirFlowRate);
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit_Impl::isRatedAirFlowRateAutosized() const
  {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedAirFlowRate, true);
    if (value)
    {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> CoilCoolingWaterToAirHeatPumpEquationFit_Impl::ratedWaterFlowRate() const
  {
    return getDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedWaterFlowRate,true);
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit_Impl::isRatedWaterFlowRateDefaulted() const
  {
    return isEmpty(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedWaterFlowRate);
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit_Impl::isRatedWaterFlowRateAutosized() const
  {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedWaterFlowRate, true);
    if (value)
    {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> CoilCoolingWaterToAirHeatPumpEquationFit_Impl::ratedTotalCoolingCapacity() const
  {
    return getDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedTotalCoolingCapacity,true);
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit_Impl::isRatedTotalCoolingCapacityDefaulted() const
  {
    return isEmpty(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedTotalCoolingCapacity);
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit_Impl::isRatedTotalCoolingCapacityAutosized() const
  {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedTotalCoolingCapacity, true);
    if (value)
    {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> CoilCoolingWaterToAirHeatPumpEquationFit_Impl::ratedSensibleCoolingCapacity() const
  {
    return getDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedSensibleCoolingCapacity,true);
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit_Impl::isRatedSensibleCoolingCapacityDefaulted() const
  {
    return isEmpty(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedSensibleCoolingCapacity);
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit_Impl::isRatedSensibleCoolingCapacityAutosized() const
  {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedSensibleCoolingCapacity, true);
    if (value)
    {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit_Impl::ratedCoolingCoefficientofPerformance() const
  {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedCoolingCoefficientofPerformance,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit_Impl::isRatedCoolingCoefficientofPerformanceDefaulted() const {
    return isEmpty(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedCoolingCoefficientofPerformance);
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit_Impl::totalCoolingCapacityCoefficient1() const
  {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::TotalCoolingCapacityCoefficient1,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit_Impl::totalCoolingCapacityCoefficient2() const
  {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::TotalCoolingCapacityCoefficient2,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit_Impl::totalCoolingCapacityCoefficient3() const
  {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::TotalCoolingCapacityCoefficient3,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit_Impl::totalCoolingCapacityCoefficient4() const
  {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::TotalCoolingCapacityCoefficient4,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit_Impl::totalCoolingCapacityCoefficient5() const
  {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::TotalCoolingCapacityCoefficient5,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit_Impl::sensibleCoolingCapacityCoefficient1() const
  {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::SensibleCoolingCapacityCoefficient1,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit_Impl::sensibleCoolingCapacityCoefficient2() const
  {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::SensibleCoolingCapacityCoefficient2,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit_Impl::sensibleCoolingCapacityCoefficient3() const
  {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::SensibleCoolingCapacityCoefficient3,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit_Impl::sensibleCoolingCapacityCoefficient4() const
  {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::SensibleCoolingCapacityCoefficient4,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit_Impl::sensibleCoolingCapacityCoefficient5() const
  {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::SensibleCoolingCapacityCoefficient5,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit_Impl::sensibleCoolingCapacityCoefficient6() const
  {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::SensibleCoolingCapacityCoefficient6,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit_Impl::coolingPowerConsumptionCoefficient1() const
  {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::CoolingPowerConsumptionCoefficient1,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit_Impl::coolingPowerConsumptionCoefficient2() const
  {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::CoolingPowerConsumptionCoefficient2,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit_Impl::coolingPowerConsumptionCoefficient3() const
  {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::CoolingPowerConsumptionCoefficient3,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit_Impl::coolingPowerConsumptionCoefficient4() const
  {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::CoolingPowerConsumptionCoefficient4,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit_Impl::coolingPowerConsumptionCoefficient5() const
  {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::CoolingPowerConsumptionCoefficient5,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit_Impl::nominalTimeforCondensateRemovaltoBegin() const
  {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::NominalTimeforCondensateRemovaltoBegin,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit_Impl::isNominalTimeforCondensateRemovaltoBeginDefaulted() const
  {
    return isEmpty(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::NominalTimeforCondensateRemovaltoBegin);
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit_Impl::ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity() const
  {
    boost::optional<double> value = getDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit_Impl::isRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacityDefaulted() const
  {
    return isEmpty(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity);
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit_Impl::setRatedAirFlowRate(boost::optional<double> ratedAirFlowRate) {
    bool result(false);
    if (ratedAirFlowRate)
    {
      result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedAirFlowRate, ratedAirFlowRate.get());
    }
    else
    {
      resetRatedAirFlowRate();
      result = true;
    }
    return result;
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit_Impl::resetRatedAirFlowRate()
  {
    bool result = setString(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedAirFlowRate, "");
    OS_ASSERT(result);
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit_Impl::autosizeRatedAirFlowRate()
  {
    bool result = setString(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit_Impl::setRatedWaterFlowRate(boost::optional<double> ratedWaterFlowRate)
  {
    bool result(false);
    if (ratedWaterFlowRate)
    {
      result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedWaterFlowRate, ratedWaterFlowRate.get());
    }
    else
    {
      resetRatedWaterFlowRate();
      result = true;
    }
    return result;
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit_Impl::resetRatedWaterFlowRate()
  {
    bool result = setString(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedWaterFlowRate, "");
    OS_ASSERT(result);
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit_Impl::autosizeRatedWaterFlowRate()
  {
    bool result = setString(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedWaterFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit_Impl::setRatedTotalCoolingCapacity(boost::optional<double> ratedTotalCoolingCapacity)
  {
    bool result(false);
    if (ratedTotalCoolingCapacity)
    {
      result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedTotalCoolingCapacity, ratedTotalCoolingCapacity.get());
    }
    else
    {
      resetRatedTotalCoolingCapacity();
      result = true;
    }
    return result;
  }

  unsigned CoilCoolingWaterToAirHeatPumpEquationFit_Impl::airInletPort()
  {
    return OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::AirInletNodeName;
  }

  unsigned CoilCoolingWaterToAirHeatPumpEquationFit_Impl::airOutletPort()
  {
    return OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::AirOutletNodeName;
  }

  unsigned CoilCoolingWaterToAirHeatPumpEquationFit_Impl::waterInletPort()
  {
    return OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::WaterInletNodeName;
  }

  unsigned CoilCoolingWaterToAirHeatPumpEquationFit_Impl::waterOutletPort()
  {
    return OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::WaterOutletNodeName;
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit_Impl::resetRatedTotalCoolingCapacity()
  {
    bool result = setString(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedTotalCoolingCapacity, "");
    OS_ASSERT(result);
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit_Impl::autosizeRatedTotalCoolingCapacity()
  {
    bool result = setString(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedTotalCoolingCapacity, "autosize");
    OS_ASSERT(result);
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit_Impl::setRatedSensibleCoolingCapacity(boost::optional<double> ratedSensibleCoolingCapacity)
  {
    bool result(false);
    if (ratedSensibleCoolingCapacity)
    {
      result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedSensibleCoolingCapacity, ratedSensibleCoolingCapacity.get());
    }
    else
    {
      resetRatedSensibleCoolingCapacity();
      result = true;
    }
    return result;
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit_Impl::resetRatedSensibleCoolingCapacity()
  {
    bool result = setString(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedSensibleCoolingCapacity, "");
    OS_ASSERT(result);
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit_Impl::autosizeRatedSensibleCoolingCapacity()
  {
    bool result = setString(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedSensibleCoolingCapacity, "autosize");
    OS_ASSERT(result);
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit_Impl::setRatedCoolingCoefficientofPerformance(double ratedCoolingCoefficientofPerformance)
  {
    bool result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedCoolingCoefficientofPerformance, ratedCoolingCoefficientofPerformance);
    return result;
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit_Impl::setTotalCoolingCapacityCoefficient1(double totalCoolingCapacityCoefficient1)
  {
    bool result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::TotalCoolingCapacityCoefficient1, totalCoolingCapacityCoefficient1);
    OS_ASSERT(result);
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit_Impl::setTotalCoolingCapacityCoefficient2(double totalCoolingCapacityCoefficient2)
  {
    bool result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::TotalCoolingCapacityCoefficient2, totalCoolingCapacityCoefficient2);
    OS_ASSERT(result);
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit_Impl::setTotalCoolingCapacityCoefficient3(double totalCoolingCapacityCoefficient3)
  {
    bool result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::TotalCoolingCapacityCoefficient3, totalCoolingCapacityCoefficient3);
    OS_ASSERT(result);
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit_Impl::setTotalCoolingCapacityCoefficient4(double totalCoolingCapacityCoefficient4)
  {
    bool result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::TotalCoolingCapacityCoefficient4, totalCoolingCapacityCoefficient4);
    OS_ASSERT(result);
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit_Impl::setTotalCoolingCapacityCoefficient5(double totalCoolingCapacityCoefficient5)
  {
    bool result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::TotalCoolingCapacityCoefficient5, totalCoolingCapacityCoefficient5);
    OS_ASSERT(result);
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit_Impl::setSensibleCoolingCapacityCoefficient1(double sensibleCoolingCapacityCoefficient1)
  {
    bool result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::SensibleCoolingCapacityCoefficient1, sensibleCoolingCapacityCoefficient1);
    OS_ASSERT(result);
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit_Impl::setSensibleCoolingCapacityCoefficient2(double sensibleCoolingCapacityCoefficient2)
  {
    bool result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::SensibleCoolingCapacityCoefficient2, sensibleCoolingCapacityCoefficient2);
    OS_ASSERT(result);
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit_Impl::setSensibleCoolingCapacityCoefficient3(double sensibleCoolingCapacityCoefficient3)
  {
    bool result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::SensibleCoolingCapacityCoefficient3, sensibleCoolingCapacityCoefficient3);
    OS_ASSERT(result);
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit_Impl::setSensibleCoolingCapacityCoefficient4(double sensibleCoolingCapacityCoefficient4)
  {
    bool result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::SensibleCoolingCapacityCoefficient4, sensibleCoolingCapacityCoefficient4);
    OS_ASSERT(result);
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit_Impl::setSensibleCoolingCapacityCoefficient5(double sensibleCoolingCapacityCoefficient5)
  {
    bool result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::SensibleCoolingCapacityCoefficient5, sensibleCoolingCapacityCoefficient5);
    OS_ASSERT(result);
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit_Impl::setSensibleCoolingCapacityCoefficient6(double sensibleCoolingCapacityCoefficient6)
  {
    bool result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::SensibleCoolingCapacityCoefficient6, sensibleCoolingCapacityCoefficient6);
    OS_ASSERT(result);
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit_Impl::setCoolingPowerConsumptionCoefficient1(double coolingPowerConsumptionCoefficient1)
  {
    bool result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::CoolingPowerConsumptionCoefficient1, coolingPowerConsumptionCoefficient1);
    OS_ASSERT(result);
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit_Impl::setCoolingPowerConsumptionCoefficient2(double coolingPowerConsumptionCoefficient2)
  {
    bool result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::CoolingPowerConsumptionCoefficient2, coolingPowerConsumptionCoefficient2);
    OS_ASSERT(result);
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit_Impl::setCoolingPowerConsumptionCoefficient3(double coolingPowerConsumptionCoefficient3)
  {
    bool result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::CoolingPowerConsumptionCoefficient3, coolingPowerConsumptionCoefficient3);
    OS_ASSERT(result);
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit_Impl::setCoolingPowerConsumptionCoefficient4(double coolingPowerConsumptionCoefficient4)
  {
    bool result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::CoolingPowerConsumptionCoefficient4, coolingPowerConsumptionCoefficient4);
    OS_ASSERT(result);
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit_Impl::setCoolingPowerConsumptionCoefficient5(double coolingPowerConsumptionCoefficient5)
  {
    bool result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::CoolingPowerConsumptionCoefficient5, coolingPowerConsumptionCoefficient5);
    OS_ASSERT(result);
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit_Impl::setNominalTimeforCondensateRemovaltoBegin(double nominalTimeforCondensateRemovaltoBegin)
  {
    bool result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::NominalTimeforCondensateRemovaltoBegin, nominalTimeforCondensateRemovaltoBegin);
    return result;
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit_Impl::resetNominalTimeforCondensateRemovaltoBegin()
  {
    bool result = setString(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::NominalTimeforCondensateRemovaltoBegin, "");
    OS_ASSERT(result);
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit_Impl::setRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(double ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity)
  {
    bool result = setDouble(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity, ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity);
    return result;
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit_Impl::resetRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity()
  {
    bool result = setString(OS_Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity, "");
    OS_ASSERT(result);
  }

  boost::optional<HVACComponent> CoilCoolingWaterToAirHeatPumpEquationFit_Impl::containingHVACComponent() const
  {
    // AirLoopHVACUnitarySystem
    std::vector<AirLoopHVACUnitarySystem> airLoopHVACUnitarySystems = this->model().getConcreteModelObjects<AirLoopHVACUnitarySystem>();

    for( const auto & airLoopHVACUnitarySystem : airLoopHVACUnitarySystems )
    {
      if( boost::optional<HVACComponent> coolingCoil = airLoopHVACUnitarySystem.coolingCoil() )
      {
        if( coolingCoil->handle() == this->handle() )
        {
          return airLoopHVACUnitarySystem;
        }
      }
    }
    return boost::none;
  }

  boost::optional<ZoneHVACComponent> CoilCoolingWaterToAirHeatPumpEquationFit_Impl::containingZoneHVACComponent() const
  {
    // ZoneHVACWaterToAirHeatPump

    std::vector<ZoneHVACWaterToAirHeatPump> zoneHVACWaterToAirHeatPumps;

    zoneHVACWaterToAirHeatPumps = this->model().getConcreteModelObjects<ZoneHVACWaterToAirHeatPump>();

    for( const auto & zoneHVACWaterToAirHeatPump : zoneHVACWaterToAirHeatPumps )
    {
      if( boost::optional<HVACComponent> coil = zoneHVACWaterToAirHeatPump.coolingCoil() )
      {
        if( coil->handle() == this->handle() )
        {
          return zoneHVACWaterToAirHeatPump;
        }
      }
    }

    return boost::none;
  }
} // detail

  CoilCoolingWaterToAirHeatPumpEquationFit::CoilCoolingWaterToAirHeatPumpEquationFit(const Model& model)
  : WaterToAirComponent(CoilCoolingWaterToAirHeatPumpEquationFit::iddObjectType(),model)
  {
    OS_ASSERT(getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>());

    setTotalCoolingCapacityCoefficient1(-0.68126221);
    setTotalCoolingCapacityCoefficient2(1.99529297);
    setTotalCoolingCapacityCoefficient3(-0.93611888);
    setTotalCoolingCapacityCoefficient4(0.02081177);
    setTotalCoolingCapacityCoefficient5(0.008438868);
    setSensibleCoolingCapacityCoefficient1(2.24209455);
    setSensibleCoolingCapacityCoefficient2(7.28913391);
    setSensibleCoolingCapacityCoefficient3(-9.06079896);
    setSensibleCoolingCapacityCoefficient4(-0.36729404);
    setSensibleCoolingCapacityCoefficient5(0.218826161);
    setSensibleCoolingCapacityCoefficient6(0.00901534);
    setCoolingPowerConsumptionCoefficient1(-3.20456384);
    setCoolingPowerConsumptionCoefficient2(0.47656454);
    setCoolingPowerConsumptionCoefficient3(3.16734236);
    setCoolingPowerConsumptionCoefficient4(0.10244637);
    setCoolingPowerConsumptionCoefficient5(-0.038132556);
  }

  IddObjectType CoilCoolingWaterToAirHeatPumpEquationFit::iddObjectType()
  {
    return IddObjectType(IddObjectType::OS_Coil_Cooling_WaterToAirHeatPump_EquationFit);
  }

  boost::optional<double> CoilCoolingWaterToAirHeatPumpEquationFit::ratedAirFlowRate() const
  {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->ratedAirFlowRate();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::isRatedAirFlowRateDefaulted() const
  {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->isRatedAirFlowRateDefaulted();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::isRatedAirFlowRateAutosized() const
  {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->isRatedAirFlowRateAutosized();
  }

  boost::optional<double> CoilCoolingWaterToAirHeatPumpEquationFit::ratedWaterFlowRate() const
  {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->ratedWaterFlowRate();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::isRatedWaterFlowRateDefaulted() const
  {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->isRatedWaterFlowRateDefaulted();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::isRatedWaterFlowRateAutosized() const
  {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->isRatedWaterFlowRateAutosized();
  }

  boost::optional<double> CoilCoolingWaterToAirHeatPumpEquationFit::ratedTotalCoolingCapacity() const
  {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->ratedTotalCoolingCapacity();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::isRatedTotalCoolingCapacityDefaulted() const
  {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->isRatedTotalCoolingCapacityDefaulted();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::isRatedTotalCoolingCapacityAutosized() const
  {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->isRatedTotalCoolingCapacityAutosized();
  }

  boost::optional<double> CoilCoolingWaterToAirHeatPumpEquationFit::ratedSensibleCoolingCapacity() const
  {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->ratedSensibleCoolingCapacity();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::isRatedSensibleCoolingCapacityDefaulted() const
  {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->isRatedSensibleCoolingCapacityDefaulted();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::isRatedSensibleCoolingCapacityAutosized() const
  {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->isRatedSensibleCoolingCapacityAutosized();
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit::ratedCoolingCoefficientofPerformance() const
  {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->ratedCoolingCoefficientofPerformance();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::isRatedCoolingCoefficientofPerformanceDefaulted() const {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->isRatedCoolingCoefficientofPerformanceDefaulted();
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit::totalCoolingCapacityCoefficient1() const
  {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->totalCoolingCapacityCoefficient1();
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit::totalCoolingCapacityCoefficient2() const
  {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->totalCoolingCapacityCoefficient2();
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit::totalCoolingCapacityCoefficient3() const
  {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->totalCoolingCapacityCoefficient3();
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit::totalCoolingCapacityCoefficient4() const
  {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->totalCoolingCapacityCoefficient4();
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit::totalCoolingCapacityCoefficient5() const
  {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->totalCoolingCapacityCoefficient5();
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit::sensibleCoolingCapacityCoefficient1() const
  {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->sensibleCoolingCapacityCoefficient1();
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit::sensibleCoolingCapacityCoefficient2() const
  {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->sensibleCoolingCapacityCoefficient2();
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit::sensibleCoolingCapacityCoefficient3() const
  {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->sensibleCoolingCapacityCoefficient3();
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit::sensibleCoolingCapacityCoefficient4() const
  {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->sensibleCoolingCapacityCoefficient4();
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit::sensibleCoolingCapacityCoefficient5() const
  {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->sensibleCoolingCapacityCoefficient5();
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit::sensibleCoolingCapacityCoefficient6() const
  {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->sensibleCoolingCapacityCoefficient6();
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit::coolingPowerConsumptionCoefficient1() const
  {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->coolingPowerConsumptionCoefficient1();
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit::coolingPowerConsumptionCoefficient2() const
  {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->coolingPowerConsumptionCoefficient2();
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit::coolingPowerConsumptionCoefficient3() const
  {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->coolingPowerConsumptionCoefficient3();
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit::coolingPowerConsumptionCoefficient4() const
  {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->coolingPowerConsumptionCoefficient4();
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit::coolingPowerConsumptionCoefficient5() const
  {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->coolingPowerConsumptionCoefficient5();
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit::nominalTimeforCondensateRemovaltoBegin() const
  {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->nominalTimeforCondensateRemovaltoBegin();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::isNominalTimeforCondensateRemovaltoBeginDefaulted() const
  {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->isNominalTimeforCondensateRemovaltoBeginDefaulted();
  }

  double CoilCoolingWaterToAirHeatPumpEquationFit::ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity() const
  {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::isRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacityDefaulted() const
  {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->isRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacityDefaulted();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::setRatedAirFlowRate(double ratedAirFlowRate)
  {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->setRatedAirFlowRate(ratedAirFlowRate);
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit::resetRatedAirFlowRate()
  {
    getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->resetRatedAirFlowRate();
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit::autosizeRatedAirFlowRate()
  {
    getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->autosizeRatedAirFlowRate();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::setRatedWaterFlowRate(double ratedWaterFlowRate)
  {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->setRatedWaterFlowRate(ratedWaterFlowRate);
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit::resetRatedWaterFlowRate()
  {
    getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->resetRatedWaterFlowRate();
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit::autosizeRatedWaterFlowRate()
  {
    getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->autosizeRatedWaterFlowRate();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::setRatedTotalCoolingCapacity(double ratedTotalCoolingCapacity)
  {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->setRatedTotalCoolingCapacity(ratedTotalCoolingCapacity);
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit::resetRatedTotalCoolingCapacity()
  {
    getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->resetRatedTotalCoolingCapacity();
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit::autosizeRatedTotalCoolingCapacity()
  {
    getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->autosizeRatedTotalCoolingCapacity();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::setRatedSensibleCoolingCapacity(double ratedSensibleCoolingCapacity)
  {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->setRatedSensibleCoolingCapacity(ratedSensibleCoolingCapacity);
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit::resetRatedSensibleCoolingCapacity()
  {
    getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->resetRatedSensibleCoolingCapacity();
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit::autosizeRatedSensibleCoolingCapacity()
  {
    getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->autosizeRatedSensibleCoolingCapacity();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::setRatedCoolingCoefficientofPerformance(double ratedCoolingCoefficientofPerformance)
  {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->setRatedCoolingCoefficientofPerformance(ratedCoolingCoefficientofPerformance);
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit::setTotalCoolingCapacityCoefficient1(double totalCoolingCapacityCoefficient1)
  {
    getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->setTotalCoolingCapacityCoefficient1(totalCoolingCapacityCoefficient1);
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit::setTotalCoolingCapacityCoefficient2(double totalCoolingCapacityCoefficient2)
  {
    getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->setTotalCoolingCapacityCoefficient2(totalCoolingCapacityCoefficient2);
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit::setTotalCoolingCapacityCoefficient3(double totalCoolingCapacityCoefficient3)
  {
    getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->setTotalCoolingCapacityCoefficient3(totalCoolingCapacityCoefficient3);
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit::setTotalCoolingCapacityCoefficient4(double totalCoolingCapacityCoefficient4)
  {
    getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->setTotalCoolingCapacityCoefficient4(totalCoolingCapacityCoefficient4);
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit::setTotalCoolingCapacityCoefficient5(double totalCoolingCapacityCoefficient5)
  {
    getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->setTotalCoolingCapacityCoefficient5(totalCoolingCapacityCoefficient5);
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit::setSensibleCoolingCapacityCoefficient1(double sensibleCoolingCapacityCoefficient1)
  {
    getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->setSensibleCoolingCapacityCoefficient1(sensibleCoolingCapacityCoefficient1);
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit::setSensibleCoolingCapacityCoefficient2(double sensibleCoolingCapacityCoefficient2)
  {
    getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->setSensibleCoolingCapacityCoefficient2(sensibleCoolingCapacityCoefficient2);
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit::setSensibleCoolingCapacityCoefficient3(double sensibleCoolingCapacityCoefficient3)
  {
    getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->setSensibleCoolingCapacityCoefficient3(sensibleCoolingCapacityCoefficient3);
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit::setSensibleCoolingCapacityCoefficient4(double sensibleCoolingCapacityCoefficient4)
  {
    getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->setSensibleCoolingCapacityCoefficient4(sensibleCoolingCapacityCoefficient4);
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit::setSensibleCoolingCapacityCoefficient5(double sensibleCoolingCapacityCoefficient5)
  {
    getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->setSensibleCoolingCapacityCoefficient5(sensibleCoolingCapacityCoefficient5);
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit::setSensibleCoolingCapacityCoefficient6(double sensibleCoolingCapacityCoefficient6)
  {
    getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->setSensibleCoolingCapacityCoefficient6(sensibleCoolingCapacityCoefficient6);
  }


  void CoilCoolingWaterToAirHeatPumpEquationFit::setCoolingPowerConsumptionCoefficient1(double coolingPowerConsumptionCoefficient1)
  {
    getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->setCoolingPowerConsumptionCoefficient1(coolingPowerConsumptionCoefficient1);
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit::setCoolingPowerConsumptionCoefficient2(double coolingPowerConsumptionCoefficient2)
  {
    getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->setCoolingPowerConsumptionCoefficient2(coolingPowerConsumptionCoefficient2);
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit::setCoolingPowerConsumptionCoefficient3(double coolingPowerConsumptionCoefficient3)
  {
    getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->setCoolingPowerConsumptionCoefficient3(coolingPowerConsumptionCoefficient3);
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit::setCoolingPowerConsumptionCoefficient4(double coolingPowerConsumptionCoefficient4)
  {
    getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->setCoolingPowerConsumptionCoefficient4(coolingPowerConsumptionCoefficient4);
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit::setCoolingPowerConsumptionCoefficient5(double coolingPowerConsumptionCoefficient5)
  {
    getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->setCoolingPowerConsumptionCoefficient5(coolingPowerConsumptionCoefficient5);
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::setNominalTimeforCondensateRemovaltoBegin(double nominalTimeforCondensateRemovaltoBegin)
  {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->setNominalTimeforCondensateRemovaltoBegin(nominalTimeforCondensateRemovaltoBegin);
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit::resetNominalTimeforCondensateRemovaltoBegin()
  {
    getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->resetNominalTimeforCondensateRemovaltoBegin();
  }

  bool CoilCoolingWaterToAirHeatPumpEquationFit::setRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(double ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity)
  {
    return getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->setRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity);
  }

  void CoilCoolingWaterToAirHeatPumpEquationFit::resetRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity()
  {
    getImpl<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl>()->resetRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity();
  }

/// @cond
CoilCoolingWaterToAirHeatPumpEquationFit::CoilCoolingWaterToAirHeatPumpEquationFit(std::shared_ptr<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl> impl)
  : WaterToAirComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

