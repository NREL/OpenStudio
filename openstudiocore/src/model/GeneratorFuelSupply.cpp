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
#include "Model.hpp"
#include "Model_Impl.hpp"

#include "GeneratorFuelCell.hpp"
#include "GeneratorFuelCell_Impl.hpp"
#include "GeneratorFuelSupply.hpp"
#include "GeneratorFuelSupply_Impl.hpp"

#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleConstant.hpp"
#include "ScheduleConstant_Impl.hpp"
#include "CurveCubic.hpp"
#include "CurveCubic_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeLimits_Impl.hpp"
#include "ScheduleTypeRegistry.hpp"

#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Generator_FuelSupply_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  GeneratorFuelSupply_Impl::GeneratorFuelSupply_Impl(const IdfObject& idfObject,
                                                     Model_Impl* model,
                                                     bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == GeneratorFuelSupply::iddObjectType());
  }

  GeneratorFuelSupply_Impl::GeneratorFuelSupply_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                     Model_Impl* model,
                                                     bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == GeneratorFuelSupply::iddObjectType());
  }

  GeneratorFuelSupply_Impl::GeneratorFuelSupply_Impl(const GeneratorFuelSupply_Impl& other,
                                                     Model_Impl* model,
                                                     bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& GeneratorFuelSupply_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType GeneratorFuelSupply_Impl::iddObjectType() const {
    return GeneratorFuelSupply::iddObjectType();
  }

  // This will clone both the GeneratorFuelCellExhaustGasToWaterHeatExchanger and its linked GeneratorFuelCell
  // and will return a reference to the GeneratorMicroTurbineHeatRecovery
  ModelObject GeneratorFuelSupply_Impl::clone(Model model) const {

    // We call the parent generator's Clone method which will clone both the fuelCell and fuelCellHX
    GeneratorFuelCell fs = fuelCell();
    GeneratorFuelCell fsClone = fs.clone(model).cast<GeneratorFuelCell>();

    // We get the clone of the parent generator's MTHR so we can return that
    GeneratorFuelSupply hxClone = fsClone.fuelSupply();


    return hxClone;
  }

  std::vector<IddObjectType> GeneratorFuelSupply_Impl::allowableChildTypes() const {
    std::vector<IddObjectType> result;
    result.push_back(IddObjectType::OS_Curve_Cubic);
    return result;
  }

  // Returns the children object
  std::vector<ModelObject> GeneratorFuelSupply_Impl::children() const {
    std::vector<ModelObject> result;
    boost::optional<CurveCubic> curveC;

    if (curveC = compressorPowerMultiplierFunctionofFuelRateCurve()) {
      result.push_back(curveC.get());
    }

    return result;
  }

  // Get the parent GeneratorFuelCell
  GeneratorFuelCell GeneratorFuelSupply_Impl::fuelCell() const {

    boost::optional<GeneratorFuelCell> value;
    for (const GeneratorFuelCell& fc : this->model().getConcreteModelObjects<GeneratorFuelCell>()) {
      if (boost::optional<GeneratorFuelSupply> fcHX = fc.fuelSupply()) {
        if (fcHX->handle() == this->handle()) {
          value = fc;
        }
      }
    }
    OS_ASSERT(value);
    return value.get();

  }

  std::vector<ScheduleTypeKey> GeneratorFuelSupply_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Generator_FuelSupplyFields::FuelTemperatureScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("GeneratorFuelSupply","Fuel Temperature"));
    }
    return result;
  }

  std::string GeneratorFuelSupply_Impl::fuelTemperatureModelingMode() const {
    boost::optional<std::string> value = getString(OS_Generator_FuelSupplyFields::FuelTemperatureModelingMode, true);
    if (!value) {
      LOG_AND_THROW(" does not have fuelTemperatureModelingMode");
    }
    return value.get();
  }

  boost::optional<Node> GeneratorFuelSupply_Impl::fuelTemperatureReferenceNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_Generator_FuelSupplyFields::FuelTemperatureReferenceNodeName);
  }

  boost::optional<Schedule> GeneratorFuelSupply_Impl::fuelTemperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Generator_FuelSupplyFields::FuelTemperatureScheduleName);
  }

  CurveCubic GeneratorFuelSupply_Impl::compressorPowerMultiplierFunctionofFuelRateCurve() const {
    boost::optional<CurveCubic> value = getObject<ModelObject>().getModelObjectTarget<CurveCubic>(OS_Generator_FuelSupplyFields::CompressorPowerMultiplierFunctionofFuelRateCurveName);
    if (!value) {
      LOG_AND_THROW(" does not have compressorPowerMultiplierFunctionofFuelRateCurve");
    }
    return value.get();
  }

  double GeneratorFuelSupply_Impl::compressorHeatLossFactor() const {
    boost::optional<double> value = getDouble(OS_Generator_FuelSupplyFields::CompressorHeatLossFactor, true);
    if (!value) {
      LOG_AND_THROW(" does not have compressorHeatLossFactor");
    }
    return value.get();
  }

  std::string GeneratorFuelSupply_Impl::fuelType() const {
    boost::optional<std::string> value = getString(OS_Generator_FuelSupplyFields::FuelType, true);
    if (!value) {
      LOG_AND_THROW(" does not have fuelType");
    }
    return value.get();
  }

  boost::optional<double> GeneratorFuelSupply_Impl::liquidGenericFuelLowerHeatingValue() const {
    boost::optional<double> value = getDouble(OS_Generator_FuelSupplyFields::LiquidGenericFuelLowerHeatingValue, true);
    return value;
  }

  boost::optional<double> GeneratorFuelSupply_Impl::liquidGenericFuelHigherHeatingValue() const {
    boost::optional<double> value = getDouble(OS_Generator_FuelSupplyFields::LiquidGenericFuelHigherHeatingValue, true);
    return value;
  }

  boost::optional<double> GeneratorFuelSupply_Impl::liquidGenericFuelMolecularWeight() const {
    boost::optional<double> value = getDouble(OS_Generator_FuelSupplyFields::LiquidGenericFuelMolecularWeight, true);
    return value;
  }

  boost::optional<double> GeneratorFuelSupply_Impl::liquidGenericFuelCO2EmissionFactor() const {
    boost::optional<double> value = getDouble(OS_Generator_FuelSupplyFields::LiquidGenericFuelCO2EmissionFactor, true);
    return value;
  }

  boost::optional<unsigned int> GeneratorFuelSupply_Impl::numberofConstituentsinGaseousConstituentFuelSupply() const {
    boost::optional<unsigned int> value;
    boost::optional<int> temp = getInt(OS_Generator_FuelSupplyFields::NumberofConstituentsinGaseousConstituentFuelSupply, true);
    if (temp) {
      if (temp >= 0) {
        value = temp;
      }
    }
    return value;
  }

  bool GeneratorFuelSupply_Impl::setFuelTemperatureModelingMode(const std::string& fuelTemperatureModelingMode) {
    bool result = setString(OS_Generator_FuelSupplyFields::FuelTemperatureModelingMode, fuelTemperatureModelingMode);
    return result;
  }

  void GeneratorFuelSupply_Impl::resetFuelTemperatureModelingMode() {
    bool result = setString(OS_Generator_FuelSupplyFields::FuelTemperatureModelingMode, "Scheduled");
    OS_ASSERT(result);
    ScheduleConstant schedule(this->model());
    schedule.setValue(20);
    result = setFuelTemperatureSchedule(schedule);
    OS_ASSERT(result);
  }

  bool GeneratorFuelSupply_Impl::setFuelTemperatureReferenceNode(const Node& connection) {
    bool result = setPointer(OS_Generator_FuelSupplyFields::FuelTemperatureReferenceNodeName, connection.handle());
    return result;
  }

  void GeneratorFuelSupply_Impl::resetFuelTemperatureReferenceNode() {
    bool result = setString(OS_Generator_FuelSupplyFields::FuelTemperatureReferenceNodeName, "");
    OS_ASSERT(result);
  }

  bool GeneratorFuelSupply_Impl::setFuelTemperatureSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Generator_FuelSupplyFields::FuelTemperatureScheduleName,
                              "GeneratorFuelSupply",
                              "Fuel Temperature",
                              schedule);
    return result;
  }

  void GeneratorFuelSupply_Impl::resetFuelTemperatureSchedule() {
    bool result = setString(OS_Generator_FuelSupplyFields::FuelTemperatureScheduleName, "");
    OS_ASSERT(result);
  }

  bool GeneratorFuelSupply_Impl::setCompressorPowerMultiplierFunctionofFuelRateCurve(const CurveCubic& cubicCurves) {
    bool result = setPointer(OS_Generator_FuelSupplyFields::CompressorPowerMultiplierFunctionofFuelRateCurveName, cubicCurves.handle());
    return result;
  }

  bool GeneratorFuelSupply_Impl::setCompressorHeatLossFactor(double compressorHeatLossFactor) {
    bool result = setDouble(OS_Generator_FuelSupplyFields::CompressorHeatLossFactor, compressorHeatLossFactor);
    return result;
  }

  void GeneratorFuelSupply_Impl::resetCompressorHeatLossFactor() {
    bool result = setDouble(OS_Generator_FuelSupplyFields::CompressorHeatLossFactor, 0);
    OS_ASSERT(result);
  }

  bool GeneratorFuelSupply_Impl::setFuelType(const std::string& fuelType) {
    bool result = setString(OS_Generator_FuelSupplyFields::FuelType, fuelType);
    return result;
  }

  void GeneratorFuelSupply_Impl::resetFuelType() {
    bool result = setString(OS_Generator_FuelSupplyFields::FuelType, "LiquidGeneric");
    OS_ASSERT(result);
  }

  void GeneratorFuelSupply_Impl::setLiquidGenericFuelLowerHeatingValue(double liquidGenericFuelLowerHeatingValue) {
    bool result = setDouble(OS_Generator_FuelSupplyFields::LiquidGenericFuelLowerHeatingValue, liquidGenericFuelLowerHeatingValue);
    OS_ASSERT(result);
  }

  void GeneratorFuelSupply_Impl::resetLiquidGenericFuelLowerHeatingValue() {
    bool result = setString(OS_Generator_FuelSupplyFields::LiquidGenericFuelLowerHeatingValue, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelSupply_Impl::setLiquidGenericFuelHigherHeatingValue(double liquidGenericFuelHigherHeatingValue) {
    bool result = setDouble(OS_Generator_FuelSupplyFields::LiquidGenericFuelHigherHeatingValue, liquidGenericFuelHigherHeatingValue);
    OS_ASSERT(result);
  }

  void GeneratorFuelSupply_Impl::resetLiquidGenericFuelHigherHeatingValue() {
    bool result = setString(OS_Generator_FuelSupplyFields::LiquidGenericFuelHigherHeatingValue, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelSupply_Impl::setLiquidGenericFuelMolecularWeight(double liquidGenericFuelMolecularWeight) {
    bool result = setDouble(OS_Generator_FuelSupplyFields::LiquidGenericFuelMolecularWeight, liquidGenericFuelMolecularWeight);
    OS_ASSERT(result);
  }

  void GeneratorFuelSupply_Impl::resetLiquidGenericFuelMolecularWeight() {
    bool result = setString(OS_Generator_FuelSupplyFields::LiquidGenericFuelMolecularWeight, "");
    OS_ASSERT(result);
  }

  void GeneratorFuelSupply_Impl::setLiquidGenericFuelCO2EmissionFactor(double liquidGenericFuelCO2EmissionFactor) {
    bool result = setDouble(OS_Generator_FuelSupplyFields::LiquidGenericFuelCO2EmissionFactor, liquidGenericFuelCO2EmissionFactor);
    OS_ASSERT(result);
  }

  void GeneratorFuelSupply_Impl::resetLiquidGenericFuelCO2EmissionFactor() {
    bool result = setString(OS_Generator_FuelSupplyFields::LiquidGenericFuelCO2EmissionFactor, "");
    OS_ASSERT(result);
  }

  bool GeneratorFuelSupply_Impl::setNumberofConstituentsinGaseousConstituentFuelSupply(unsigned int numberofConstituentsinGaseousConstituentFuelSupply) {
    bool result = setInt(OS_Generator_FuelSupplyFields::NumberofConstituentsinGaseousConstituentFuelSupply, numberofConstituentsinGaseousConstituentFuelSupply);
    return result;
  }

  void GeneratorFuelSupply_Impl::resetNumberofConstituentsinGaseousConstituentFuelSupply() {
    bool result = setInt(OS_Generator_FuelSupplyFields::NumberofConstituentsinGaseousConstituentFuelSupply, 0);
    OS_ASSERT(result);
  }

  bool GeneratorFuelSupply_Impl::addConstituent(std::string name, double molarFraction) {
    WorkspaceExtensibleGroup eg = getObject<ModelObject>().pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
    bool temp = false;
    bool ok = false;
    unsigned int num;
    if (numberofConstituentsinGaseousConstituentFuelSupply()) {
      num = numberofConstituentsinGaseousConstituentFuelSupply().get();
    } else {
      num = 0;
    }
    //max number of constituents is 12
    if (num < 12) {
      temp = eg.setString(OS_Generator_FuelSupplyExtensibleFields::ConstituentName, name);
      ok = eg.setDouble(OS_Generator_FuelSupplyExtensibleFields::ConstituentMolarFraction, molarFraction);
    }
    if (temp && ok) {
      setNumberofConstituentsinGaseousConstituentFuelSupply(num + 1);
    } else {
      if (!eg.empty()) {
        getObject<ModelObject>().eraseExtensibleGroup(eg.groupIndex());
      }
    }
    return temp;
  }

  void GeneratorFuelSupply_Impl::removeConstituent(unsigned groupIndex) {
    unsigned numberofDataPairs = numExtensibleGroups();
    if (groupIndex < numberofDataPairs) {
      unsigned int num;
      getObject<ModelObject>().eraseExtensibleGroup(groupIndex);
      if (numberofConstituentsinGaseousConstituentFuelSupply()) {
        num = numberofConstituentsinGaseousConstituentFuelSupply().get();
        setNumberofConstituentsinGaseousConstituentFuelSupply(num - 1);
      } else {
        setNumberofConstituentsinGaseousConstituentFuelSupply(numExtensibleGroups());
      }
    }
  }

  void GeneratorFuelSupply_Impl::removeAllConstituents() {
    getObject<ModelObject>().clearExtensibleGroups();
    resetNumberofConstituentsinGaseousConstituentFuelSupply();
  }

  std::vector< std::pair<std::string, double> > GeneratorFuelSupply_Impl::constituents() {
    std::vector< std::pair<std::string, double> > result;

    std::vector<IdfExtensibleGroup> groups = extensibleGroups();

    for (const auto & group : groups) {
      boost::optional<std::string> name = group.cast<WorkspaceExtensibleGroup>().getString(OS_Generator_FuelSupplyExtensibleFields::ConstituentName);
      boost::optional<double> molarFraction = group.cast<WorkspaceExtensibleGroup>().getDouble(OS_Generator_FuelSupplyExtensibleFields::ConstituentMolarFraction);

      if (name && molarFraction) {
        result.push_back(std::make_pair(name.get(), molarFraction.get()));
      }
    }

    return result;
  }

} // detail

GeneratorFuelSupply::GeneratorFuelSupply(const Model& model, Schedule& tempSchedule, const CurveCubic& powerCurve)
  : ModelObject(GeneratorFuelSupply::iddObjectType(), model) {
  OS_ASSERT(getImpl<detail::GeneratorFuelSupply_Impl>());

  setFuelTemperatureModelingMode("Scheduled");
  bool ok = setFuelTemperatureSchedule(tempSchedule);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s temp schedule to "
      << tempSchedule.briefDescription() << ".");
  }

  ok = setCompressorPowerMultiplierFunctionofFuelRateCurve(powerCurve);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s power curve to "
      << powerCurve.briefDescription() << ".");
  }
  setCompressorHeatLossFactor(1);
  setFuelType("LiquidGeneric");
  setLiquidGenericFuelLowerHeatingValue(43100);
  setLiquidGenericFuelHigherHeatingValue(46200);
  setLiquidGenericFuelMolecularWeight(1);
  setLiquidGenericFuelCO2EmissionFactor(0);
  setNumberofConstituentsinGaseousConstituentFuelSupply(0);
}

GeneratorFuelSupply::GeneratorFuelSupply(const Model& model)
  : ModelObject(GeneratorFuelSupply::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::GeneratorFuelSupply_Impl>());

  setLiquidGenericFuelCO2EmissionFactor(0);

  setFuelTemperatureModelingMode("Scheduled");
  ScheduleConstant schedule(model);
  schedule.setValue(20);
  schedule.setName("Fuel Temperature");
  setFuelTemperatureSchedule(schedule);

  CurveCubic curveCubic(model);
  curveCubic.setCoefficient1Constant(0);
  curveCubic.setCoefficient2x(0);
  curveCubic.setCoefficient3xPOW2(0);
  curveCubic.setCoefficient4xPOW3(0);
  curveCubic.setMinimumValueofx(-1.0e10);
  curveCubic.setMaximumValueofx(1.0e10);
  curveCubic.setName("Compressor Power Multiplier Function of FuelRate Curve");
  setCompressorPowerMultiplierFunctionofFuelRateCurve(curveCubic);
  setCompressorHeatLossFactor(1);
  setFuelType("LiquidGeneric");
  setLiquidGenericFuelLowerHeatingValue(43100);
  setLiquidGenericFuelHigherHeatingValue(46200);
  setLiquidGenericFuelMolecularWeight(1);
  setNumberofConstituentsinGaseousConstituentFuelSupply(0);
}

IddObjectType GeneratorFuelSupply::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Generator_FuelSupply);
}

bool GeneratorFuelSupply::addConstituent(std::string name, double molarFraction) {
  return getImpl<detail::GeneratorFuelSupply_Impl>()->addConstituent(name, molarFraction);
}

void GeneratorFuelSupply::removeConstituent(int groupIndex) {
  return getImpl<detail::GeneratorFuelSupply_Impl>()->removeConstituent(groupIndex);
}

void GeneratorFuelSupply::removeAllConstituents() {
  return getImpl<detail::GeneratorFuelSupply_Impl>()->removeAllConstituents();
}

std::vector< std::pair<std::string, double> > GeneratorFuelSupply::constituents() {
  return getImpl<detail::GeneratorFuelSupply_Impl>()->constituents();
}

std::vector<std::string> GeneratorFuelSupply::fuelTemperatureModelingModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Generator_FuelSupplyFields::FuelTemperatureModelingMode);
}

std::vector<std::string> GeneratorFuelSupply::fuelTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Generator_FuelSupplyFields::FuelType);
}

std::string GeneratorFuelSupply::fuelTemperatureModelingMode() const {
  return getImpl<detail::GeneratorFuelSupply_Impl>()->fuelTemperatureModelingMode();
}

boost::optional<Node> GeneratorFuelSupply::fuelTemperatureReferenceNode() const {
  return getImpl<detail::GeneratorFuelSupply_Impl>()->fuelTemperatureReferenceNode();
}

boost::optional<Schedule> GeneratorFuelSupply::fuelTemperatureSchedule() const {
  return getImpl<detail::GeneratorFuelSupply_Impl>()->fuelTemperatureSchedule();
}

CurveCubic GeneratorFuelSupply::compressorPowerMultiplierFunctionofFuelRateCurve() const {
  return getImpl<detail::GeneratorFuelSupply_Impl>()->compressorPowerMultiplierFunctionofFuelRateCurve();
}

double GeneratorFuelSupply::compressorHeatLossFactor() const {
  return getImpl<detail::GeneratorFuelSupply_Impl>()->compressorHeatLossFactor();
}

std::string GeneratorFuelSupply::fuelType() const {
  return getImpl<detail::GeneratorFuelSupply_Impl>()->fuelType();
}

boost::optional<double> GeneratorFuelSupply::liquidGenericFuelLowerHeatingValue() const {
  return getImpl<detail::GeneratorFuelSupply_Impl>()->liquidGenericFuelLowerHeatingValue();
}

boost::optional<double> GeneratorFuelSupply::liquidGenericFuelHigherHeatingValue() const {
  return getImpl<detail::GeneratorFuelSupply_Impl>()->liquidGenericFuelHigherHeatingValue();
}

boost::optional<double> GeneratorFuelSupply::liquidGenericFuelMolecularWeight() const {
  return getImpl<detail::GeneratorFuelSupply_Impl>()->liquidGenericFuelMolecularWeight();
}

boost::optional<double> GeneratorFuelSupply::liquidGenericFuelCO2EmissionFactor() const {
  return getImpl<detail::GeneratorFuelSupply_Impl>()->liquidGenericFuelCO2EmissionFactor();
}

boost::optional<unsigned int> GeneratorFuelSupply::numberofConstituentsinGaseousConstituentFuelSupply() const {
  return getImpl<detail::GeneratorFuelSupply_Impl>()->numberofConstituentsinGaseousConstituentFuelSupply();
}

bool GeneratorFuelSupply::setFuelTemperatureModelingMode(const std::string& fuelTemperatureModelingMode) {
  return getImpl<detail::GeneratorFuelSupply_Impl>()->setFuelTemperatureModelingMode(fuelTemperatureModelingMode);
}

void GeneratorFuelSupply::resetFuelTemperatureModelingMode() {
  getImpl<detail::GeneratorFuelSupply_Impl>()->resetFuelTemperatureModelingMode();
}

bool GeneratorFuelSupply::setFuelTemperatureReferenceNode(const Node& connection) {
  return getImpl<detail::GeneratorFuelSupply_Impl>()->setFuelTemperatureReferenceNode(connection);
}

void GeneratorFuelSupply::resetFuelTemperatureReferenceNode() {
  getImpl<detail::GeneratorFuelSupply_Impl>()->resetFuelTemperatureReferenceNode();
}

bool GeneratorFuelSupply::setFuelTemperatureSchedule(Schedule& schedule) {
  return getImpl<detail::GeneratorFuelSupply_Impl>()->setFuelTemperatureSchedule(schedule);
}

void GeneratorFuelSupply::resetFuelTemperatureSchedule() {
  getImpl<detail::GeneratorFuelSupply_Impl>()->resetFuelTemperatureSchedule();
}

bool GeneratorFuelSupply::setCompressorPowerMultiplierFunctionofFuelRateCurve(const CurveCubic& cubicCurves) {
  return getImpl<detail::GeneratorFuelSupply_Impl>()->setCompressorPowerMultiplierFunctionofFuelRateCurve(cubicCurves);
}

bool GeneratorFuelSupply::setCompressorHeatLossFactor(double compressorHeatLossFactor) {
  return getImpl<detail::GeneratorFuelSupply_Impl>()->setCompressorHeatLossFactor(compressorHeatLossFactor);
}

void GeneratorFuelSupply::resetCompressorHeatLossFactor() {
  getImpl<detail::GeneratorFuelSupply_Impl>()->resetCompressorHeatLossFactor();
}

bool GeneratorFuelSupply::setFuelType(const std::string& fuelType) {
  return getImpl<detail::GeneratorFuelSupply_Impl>()->setFuelType(fuelType);
}

void GeneratorFuelSupply::resetFuelType() {
  getImpl<detail::GeneratorFuelSupply_Impl>()->resetFuelType();
}

void GeneratorFuelSupply::setLiquidGenericFuelLowerHeatingValue(double liquidGenericFuelLowerHeatingValue) {
  getImpl<detail::GeneratorFuelSupply_Impl>()->setLiquidGenericFuelLowerHeatingValue(liquidGenericFuelLowerHeatingValue);
}

void GeneratorFuelSupply::resetLiquidGenericFuelLowerHeatingValue() {
  getImpl<detail::GeneratorFuelSupply_Impl>()->resetLiquidGenericFuelLowerHeatingValue();
}

void GeneratorFuelSupply::setLiquidGenericFuelHigherHeatingValue(double liquidGenericFuelHigherHeatingValue) {
  getImpl<detail::GeneratorFuelSupply_Impl>()->setLiquidGenericFuelHigherHeatingValue(liquidGenericFuelHigherHeatingValue);
}

void GeneratorFuelSupply::resetLiquidGenericFuelHigherHeatingValue() {
  getImpl<detail::GeneratorFuelSupply_Impl>()->resetLiquidGenericFuelHigherHeatingValue();
}

void GeneratorFuelSupply::setLiquidGenericFuelMolecularWeight(double liquidGenericFuelMolecularWeight) {
  getImpl<detail::GeneratorFuelSupply_Impl>()->setLiquidGenericFuelMolecularWeight(liquidGenericFuelMolecularWeight);
}

void GeneratorFuelSupply::resetLiquidGenericFuelMolecularWeight() {
  getImpl<detail::GeneratorFuelSupply_Impl>()->resetLiquidGenericFuelMolecularWeight();
}

void GeneratorFuelSupply::setLiquidGenericFuelCO2EmissionFactor(double liquidGenericFuelCO2EmissionFactor) {
  getImpl<detail::GeneratorFuelSupply_Impl>()->setLiquidGenericFuelCO2EmissionFactor(liquidGenericFuelCO2EmissionFactor);
}

void GeneratorFuelSupply::resetLiquidGenericFuelCO2EmissionFactor() {
  getImpl<detail::GeneratorFuelSupply_Impl>()->resetLiquidGenericFuelCO2EmissionFactor();
}

bool GeneratorFuelSupply::setNumberofConstituentsinGaseousConstituentFuelSupply(unsigned int numberofConstituentsinGaseousConstituentFuelSupply) {
  return getImpl<detail::GeneratorFuelSupply_Impl>()->setNumberofConstituentsinGaseousConstituentFuelSupply(numberofConstituentsinGaseousConstituentFuelSupply);
}

void GeneratorFuelSupply::resetNumberofConstituentsinGaseousConstituentFuelSupply() {
  getImpl<detail::GeneratorFuelSupply_Impl>()->resetNumberofConstituentsinGaseousConstituentFuelSupply();
}

GeneratorFuelCell GeneratorFuelSupply::fuelCell() const {
  return getImpl<detail::GeneratorFuelSupply_Impl>()->fuelCell();
}

/// @cond
GeneratorFuelSupply::GeneratorFuelSupply(std::shared_ptr<detail::GeneratorFuelSupply_Impl> impl)
  : ModelObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

