/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "GeneratorFuelCellWaterSupply.hpp"
#include "GeneratorFuelCellWaterSupply_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "CurveCubic.hpp"
#include "CurveCubic_Impl.hpp"
#include "CurveQuadratic.hpp"
#include "CurveQuadratic_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "../../model/ScheduleTypeLimits.hpp"
#include "../../model/ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Generator_FuelCell_WaterSupply_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  GeneratorFuelCellWaterSupply_Impl::GeneratorFuelCellWaterSupply_Impl(const IdfObject& idfObject,
                                                                       Model_Impl* model,
                                                                       bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == GeneratorFuelCellWaterSupply::iddObjectType());
  }

  GeneratorFuelCellWaterSupply_Impl::GeneratorFuelCellWaterSupply_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                       Model_Impl* model,
                                                                       bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == GeneratorFuelCellWaterSupply::iddObjectType());
  }

  GeneratorFuelCellWaterSupply_Impl::GeneratorFuelCellWaterSupply_Impl(const GeneratorFuelCellWaterSupply_Impl& other,
                                                                       Model_Impl* model,
                                                                       bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& GeneratorFuelCellWaterSupply_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType GeneratorFuelCellWaterSupply_Impl::iddObjectType() const {
    return GeneratorFuelCellWaterSupply::iddObjectType();
  }

  std::vector<ScheduleTypeKey> GeneratorFuelCellWaterSupply_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    // TODO: Check schedule display names.
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Generator_FuelCell_WaterSupplyFields::WaterTemperatureScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("GeneratorFuelCellWaterSupply","Water Temperature"));
    }
    return result;
  }

  boost::optional<QuadraticCurves> GeneratorFuelCellWaterSupply_Impl::reformerWaterFlowRateFunctionofFuelRateCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<QuadraticCurves>(OS_Generator_FuelCell_WaterSupplyFields::ReformerWaterFlowRateFunctionofFuelRateCurveName);
  }

  boost::optional<CubicCurves> GeneratorFuelCellWaterSupply_Impl::reformerWaterPumpPowerFunctionofFuelRateCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CubicCurves>(OS_Generator_FuelCell_WaterSupplyFields::ReformerWaterPumpPowerFunctionofFuelRateCurveName);
  }

  boost::optional<double> GeneratorFuelCellWaterSupply_Impl::pumpHeatLossFactor() const {
    return getDouble(OS_Generator_FuelCell_WaterSupplyFields::PumpHeatLossFactor,true);
  }

  boost::optional<std::string> GeneratorFuelCellWaterSupply_Impl::waterTemperatureModelingMode() const {
    return getString(OS_Generator_FuelCell_WaterSupplyFields::WaterTemperatureModelingMode,true);
  }

  boost::optional<Connection> GeneratorFuelCellWaterSupply_Impl::waterTemperatureReferenceNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_Generator_FuelCell_WaterSupplyFields::WaterTemperatureReferenceNodeName);
  }

  boost::optional<Schedule> GeneratorFuelCellWaterSupply_Impl::waterTemperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Generator_FuelCell_WaterSupplyFields::WaterTemperatureScheduleName);
  }

  bool GeneratorFuelCellWaterSupply_Impl::setReformerWaterFlowRateFunctionofFuelRateCurve(const CurveQuadratic& quadraticCurves) {
    bool result = setPointer(OS_Generator_FuelCell_WaterSupplyFields::ReformerWaterFlowRateFunctionofFuelRateCurveName, quadraticCurves.handle());
    return result;
  }

  void GeneratorFuelCellWaterSupply_Impl::resetReformerWaterFlowRateFunctionofFuelRateCurve() {
    bool result = setString(OS_Generator_FuelCell_WaterSupplyFields::ReformerWaterFlowRateFunctionofFuelRateCurveName, "");
    OS_ASSERT(result);
  }

  bool GeneratorFuelCellWaterSupply_Impl::setReformerWaterPumpPowerFunctionofFuelRateCurve(const CurveCubic& cubicCurves) {
    bool result = setPointer(OS_Generator_FuelCell_WaterSupplyFields::ReformerWaterPumpPowerFunctionofFuelRateCurveName, cubicCurves.handle());
    return result;
  }

  void GeneratorFuelCellWaterSupply_Impl::resetReformerWaterPumpPowerFunctionofFuelRateCurve() {
    bool result = setString(OS_Generator_FuelCell_WaterSupplyFields::ReformerWaterPumpPowerFunctionofFuelRateCurveName, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelCellWaterSupply_Impl::setPumpHeatLossFactor(double pumpHeatLossFactor) {
    bool result = setDouble(OS_Generator_FuelCell_WaterSupplyFields::PumpHeatLossFactor, pumpHeatLossFactor);
    OS_ASSERT(result);
  }

  void GeneratorFuelCellWaterSupply_Impl::resetPumpHeatLossFactor() {
    bool result = setString(OS_Generator_FuelCell_WaterSupplyFields::PumpHeatLossFactor, "");
    OS_ASSERT(result);
  }

  bool GeneratorFuelCellWaterSupply_Impl::setWaterTemperatureModelingMode(const std::string& waterTemperatureModelingMode) {
    bool result = setString(OS_Generator_FuelCell_WaterSupplyFields::WaterTemperatureModelingMode, waterTemperatureModelingMode);
    return result;
  }

  void GeneratorFuelCellWaterSupply_Impl::resetWaterTemperatureModelingMode() {
    bool result = setString(OS_Generator_FuelCell_WaterSupplyFields::WaterTemperatureModelingMode, "");
    OS_ASSERT(result);
  }

  bool GeneratorFuelCellWaterSupply_Impl::setWaterTemperatureReferenceNode(const Connection& connection) {
    bool result = setPointer(OS_Generator_FuelCell_WaterSupplyFields::WaterTemperatureReferenceNodeName, connection.handle());
    return result;
  }

  void GeneratorFuelCellWaterSupply_Impl::resetWaterTemperatureReferenceNode() {
    bool result = setString(OS_Generator_FuelCell_WaterSupplyFields::WaterTemperatureReferenceNodeName, "");
    OS_ASSERT(result);
  }

  bool GeneratorFuelCellWaterSupply_Impl::setWaterTemperatureSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Generator_FuelCell_WaterSupplyFields::WaterTemperatureScheduleName,
                              "GeneratorFuelCellWaterSupply",
                              "Water Temperature",
                              schedule);
    return result;
  }

  void GeneratorFuelCellWaterSupply_Impl::resetWaterTemperatureSchedule() {
    bool result = setString(OS_Generator_FuelCell_WaterSupplyFields::WaterTemperatureScheduleName, "");
    OS_ASSERT(result);
  }

} // detail

GeneratorFuelCellWaterSupply::GeneratorFuelCellWaterSupply(const Model& model)
  : ModelObject(GeneratorFuelCellWaterSupply::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::GeneratorFuelCellWaterSupply_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
}

IddObjectType GeneratorFuelCellWaterSupply::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Generator_FuelCell_WaterSupply);
}

std::vector<std::string> GeneratorFuelCellWaterSupply::waterTemperatureModelingModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Generator_FuelCell_WaterSupplyFields::WaterTemperatureModelingMode);
}

boost::optional<QuadraticCurves> GeneratorFuelCellWaterSupply::reformerWaterFlowRateFunctionofFuelRateCurve() const {
  return getImpl<detail::GeneratorFuelCellWaterSupply_Impl>()->reformerWaterFlowRateFunctionofFuelRateCurve();
}

boost::optional<CubicCurves> GeneratorFuelCellWaterSupply::reformerWaterPumpPowerFunctionofFuelRateCurve() const {
  return getImpl<detail::GeneratorFuelCellWaterSupply_Impl>()->reformerWaterPumpPowerFunctionofFuelRateCurve();
}

boost::optional<double> GeneratorFuelCellWaterSupply::pumpHeatLossFactor() const {
  return getImpl<detail::GeneratorFuelCellWaterSupply_Impl>()->pumpHeatLossFactor();
}

boost::optional<std::string> GeneratorFuelCellWaterSupply::waterTemperatureModelingMode() const {
  return getImpl<detail::GeneratorFuelCellWaterSupply_Impl>()->waterTemperatureModelingMode();
}

boost::optional<Connection> GeneratorFuelCellWaterSupply::waterTemperatureReferenceNode() const {
  return getImpl<detail::GeneratorFuelCellWaterSupply_Impl>()->waterTemperatureReferenceNode();
}

boost::optional<Schedule> GeneratorFuelCellWaterSupply::waterTemperatureSchedule() const {
  return getImpl<detail::GeneratorFuelCellWaterSupply_Impl>()->waterTemperatureSchedule();
}

bool GeneratorFuelCellWaterSupply::setReformerWaterFlowRateFunctionofFuelRateCurve(const QuadraticCurves& quadraticCurves) {
  return getImpl<detail::GeneratorFuelCellWaterSupply_Impl>()->setReformerWaterFlowRateFunctionofFuelRateCurve(quadraticCurves);
}

void GeneratorFuelCellWaterSupply::resetReformerWaterFlowRateFunctionofFuelRateCurve() {
  getImpl<detail::GeneratorFuelCellWaterSupply_Impl>()->resetReformerWaterFlowRateFunctionofFuelRateCurve();
}

bool GeneratorFuelCellWaterSupply::setReformerWaterPumpPowerFunctionofFuelRateCurve(const CubicCurves& cubicCurves) {
  return getImpl<detail::GeneratorFuelCellWaterSupply_Impl>()->setReformerWaterPumpPowerFunctionofFuelRateCurve(cubicCurves);
}

void GeneratorFuelCellWaterSupply::resetReformerWaterPumpPowerFunctionofFuelRateCurve() {
  getImpl<detail::GeneratorFuelCellWaterSupply_Impl>()->resetReformerWaterPumpPowerFunctionofFuelRateCurve();
}

void GeneratorFuelCellWaterSupply::setPumpHeatLossFactor(double pumpHeatLossFactor) {
  getImpl<detail::GeneratorFuelCellWaterSupply_Impl>()->setPumpHeatLossFactor(pumpHeatLossFactor);
}

void GeneratorFuelCellWaterSupply::resetPumpHeatLossFactor() {
  getImpl<detail::GeneratorFuelCellWaterSupply_Impl>()->resetPumpHeatLossFactor();
}

bool GeneratorFuelCellWaterSupply::setWaterTemperatureModelingMode(const std::string& waterTemperatureModelingMode) {
  return getImpl<detail::GeneratorFuelCellWaterSupply_Impl>()->setWaterTemperatureModelingMode(waterTemperatureModelingMode);
}

void GeneratorFuelCellWaterSupply::resetWaterTemperatureModelingMode() {
  getImpl<detail::GeneratorFuelCellWaterSupply_Impl>()->resetWaterTemperatureModelingMode();
}

bool GeneratorFuelCellWaterSupply::setWaterTemperatureReferenceNode(const Connection& connection) {
  return getImpl<detail::GeneratorFuelCellWaterSupply_Impl>()->setWaterTemperatureReferenceNode(connection);
}

void GeneratorFuelCellWaterSupply::resetWaterTemperatureReferenceNode() {
  getImpl<detail::GeneratorFuelCellWaterSupply_Impl>()->resetWaterTemperatureReferenceNode();
}

bool GeneratorFuelCellWaterSupply::setWaterTemperatureSchedule(Schedule& schedule) {
  return getImpl<detail::GeneratorFuelCellWaterSupply_Impl>()->setWaterTemperatureSchedule(schedule);
}

void GeneratorFuelCellWaterSupply::resetWaterTemperatureSchedule() {
  getImpl<detail::GeneratorFuelCellWaterSupply_Impl>()->resetWaterTemperatureSchedule();
}

/// @cond
GeneratorFuelCellWaterSupply::GeneratorFuelCellWaterSupply(std::shared_ptr<detail::GeneratorFuelCellWaterSupply_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

