/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

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

FuelSupplyConstituent::FuelSupplyConstituent(std::string constituentName, double molarFraction)
  : m_name(constituentName), m_molarFraction(molarFraction) {

  if ((m_molarFraction < 0) || (m_molarFraction > 1)) {
    LOG_AND_THROW("Unable to create constituent '" << m_name << "', molar fraction of " << m_molarFraction << " is outside the range [0, 1]");
  }
  if (!isValid(m_name)) {
    LOG_AND_THROW("ConstituentName '" << m_name << " is not valid. Check FuelSupplyConstituent::constituentNameValues() to see possible names.");
  }
}

std::string FuelSupplyConstituent::constituentName() const {
  return m_name;
}

double FuelSupplyConstituent::molarFraction() const {
  return m_molarFraction;
}

bool FuelSupplyConstituent::isValid(std::string constituentName) {
  std::vector<std::string> validConstituentNames = constituentNameValues();
  return std::find_if(validConstituentNames.begin(),
                      validConstituentNames.end(),
                      std::bind(istringEqual, constituentName, std::placeholders::_1)) != validConstituentNames.end();

}

std::vector<std::string> FuelSupplyConstituent::constituentNameValues() {
  IddObject obj = IddFactory::instance().getObject(GeneratorFuelSupply::iddObjectType()).get();
  // Return IddKeyNames in extensible portion
  return getIddKeyNames(obj,
                        obj.numFields() + OS_Generator_FuelSupplyExtensibleFields::ConstituentName);
}

std::vector<std::string> FuelSupplyConstituent::validConstituentNameValues() {
  return constituentNameValues();
}

std::ostream& operator<< (std::ostream& out, const openstudio::model::FuelSupplyConstituent& constituent) {
  out << "name=" << constituent.constituentName() << ", molar fraction=" << constituent.molarFraction();
  return out;
}

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
    return result;
  }

  IddObjectType GeneratorFuelSupply_Impl::iddObjectType() const {
    return GeneratorFuelSupply::iddObjectType();
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

    if ( (curveC = compressorPowerMultiplierFunctionofFuelRateCurve()) ) {
      result.push_back(curveC.get());
    }

    return result;
  }

  // Get the parent GeneratorFuelCell
  boost::optional<GeneratorFuelCell> GeneratorFuelSupply_Impl::fuelCell() const {

    boost::optional<GeneratorFuelCell> fc;
    // We use getModelObjectSources to check if more than one
    std::vector<GeneratorFuelCell> fcs = getObject<ModelObject>().getModelObjectSources<GeneratorFuelCell>(GeneratorFuelCell::iddObjectType());

    if( fcs.size() > 0u) {
      if( fcs.size() > 1u) {
        LOG(Error, briefDescription() << " is referenced by more than one GeneratorFuelCell, returning the first");
      }
      fc = fcs[0];
    }
    return fc;

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
    // TODO A bit unusual to set a default that doesn't exist in IDD
    bool result = setString(OS_Generator_FuelSupplyFields::FuelType, "GaseousConstituents");
    OS_ASSERT(result);
  }

  bool GeneratorFuelSupply_Impl::setLiquidGenericFuelLowerHeatingValue(double liquidGenericFuelLowerHeatingValue) {
    bool result = setDouble(OS_Generator_FuelSupplyFields::LiquidGenericFuelLowerHeatingValue, liquidGenericFuelLowerHeatingValue);
    OS_ASSERT(result);
    return result;
  }

  void GeneratorFuelSupply_Impl::resetLiquidGenericFuelLowerHeatingValue() {
    bool result = setString(OS_Generator_FuelSupplyFields::LiquidGenericFuelLowerHeatingValue, "");
    OS_ASSERT(result);
  }

  bool GeneratorFuelSupply_Impl::setLiquidGenericFuelHigherHeatingValue(double liquidGenericFuelHigherHeatingValue) {
    bool result = setDouble(OS_Generator_FuelSupplyFields::LiquidGenericFuelHigherHeatingValue, liquidGenericFuelHigherHeatingValue);
    OS_ASSERT(result);
    return result;
  }

  void GeneratorFuelSupply_Impl::resetLiquidGenericFuelHigherHeatingValue() {
    bool result = setString(OS_Generator_FuelSupplyFields::LiquidGenericFuelHigherHeatingValue, "");
    OS_ASSERT(result);
  }

  bool GeneratorFuelSupply_Impl::setLiquidGenericFuelMolecularWeight(double liquidGenericFuelMolecularWeight) {
    bool result = setDouble(OS_Generator_FuelSupplyFields::LiquidGenericFuelMolecularWeight, liquidGenericFuelMolecularWeight);
    OS_ASSERT(result);
    return result;
  }

  void GeneratorFuelSupply_Impl::resetLiquidGenericFuelMolecularWeight() {
    bool result = setString(OS_Generator_FuelSupplyFields::LiquidGenericFuelMolecularWeight, "");
    OS_ASSERT(result);
  }

  bool GeneratorFuelSupply_Impl::setLiquidGenericFuelCO2EmissionFactor(double liquidGenericFuelCO2EmissionFactor) {
    bool result = setDouble(OS_Generator_FuelSupplyFields::LiquidGenericFuelCO2EmissionFactor, liquidGenericFuelCO2EmissionFactor);
    OS_ASSERT(result);
    return result;
  }

  void GeneratorFuelSupply_Impl::resetLiquidGenericFuelCO2EmissionFactor() {
    bool result = setString(OS_Generator_FuelSupplyFields::LiquidGenericFuelCO2EmissionFactor, "");
    OS_ASSERT(result);
  }

  // TODO: this field shouldn't even exist in the IDD
  unsigned int GeneratorFuelSupply_Impl::numberofConstituentsinGaseousConstituentFuelSupply() const {
    return numExtensibleGroups();
  }

  // TODO: this field shouldn't even exist in the IDD
  bool GeneratorFuelSupply_Impl::setNumberofConstituentsinGaseousConstituentFuelSupply(unsigned int numberofConstituentsinGaseousConstituentFuelSupply) {
    bool result = setInt(OS_Generator_FuelSupplyFields::NumberofConstituentsinGaseousConstituentFuelSupply, numberofConstituentsinGaseousConstituentFuelSupply);
    return result;
  }

  // TODO: this field shouldn't even exist in the IDD
  void GeneratorFuelSupply_Impl::resetNumberofConstituentsinGaseousConstituentFuelSupply() {
    bool result = setInt(OS_Generator_FuelSupplyFields::NumberofConstituentsinGaseousConstituentFuelSupply, 0);
    OS_ASSERT(result);
  }

  double GeneratorFuelSupply_Impl::sumofConstituentsMolarFractions() const {
    double result = 0;
    if (numberofConstituentsinGaseousConstituentFuelSupply() == 0) {
      LOG(Warn, briefDescription() << " does not have any constituents");
    } else {
      for (const FuelSupplyConstituent& constituent: constituents()) {
        result += constituent.molarFraction();
      }
    }

    return result;
  }

  bool GeneratorFuelSupply_Impl::addConstituent(const FuelSupplyConstituent& constituent) {
    bool result;

    unsigned int num = numberofConstituentsinGaseousConstituentFuelSupply();
    // Max number of constituents is 12
    if (num >= 12) {
      LOG(Warn, briefDescription() << " already has 12 constituents which is the limit");
      result = false;
    } else {
      // Push an extensible group
      WorkspaceExtensibleGroup eg = getObject<ModelObject>().pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
      bool temp = eg.setString(OS_Generator_FuelSupplyExtensibleFields::ConstituentName, constituent.constituentName());
      bool ok = eg.setDouble(OS_Generator_FuelSupplyExtensibleFields::ConstituentMolarFraction, constituent.molarFraction());
      if (temp && ok) {
        setNumberofConstituentsinGaseousConstituentFuelSupply(num + 1);
        result = true;
      } else {
        // Something went wrong, probably the constituent name isn't in the list of possible choices
        // So erase the new extensible group
        getObject<ModelObject>().eraseExtensibleGroup(eg.groupIndex());
        result = false;
      }
    }
    return result;
  }

  bool GeneratorFuelSupply_Impl::addConstituent(std::string name, double molarFraction) {
   // Make a constituent (which will check for validity), and then call the above function
    FuelSupplyConstituent constituent(name, molarFraction);
    return addConstituent(constituent);
  }

  bool GeneratorFuelSupply_Impl::removeConstituent(unsigned groupIndex) {
    bool result;

    unsigned int num = numberofConstituentsinGaseousConstituentFuelSupply();
    if (groupIndex < num) {
      getObject<ModelObject>().eraseExtensibleGroup(groupIndex);
      setNumberofConstituentsinGaseousConstituentFuelSupply(num - 1);
      result = true;
    } else {
      result = false;
    }
    return result;
  }

  void GeneratorFuelSupply_Impl::removeAllConstituents() {
    getObject<ModelObject>().clearExtensibleGroups();
    resetNumberofConstituentsinGaseousConstituentFuelSupply();
  }

  std::vector<FuelSupplyConstituent> GeneratorFuelSupply_Impl::constituents() const {
    std::vector<FuelSupplyConstituent> result;

    std::vector<IdfExtensibleGroup> groups = extensibleGroups();

    for (const auto & group : groups) {
      boost::optional<std::string> name = group.cast<WorkspaceExtensibleGroup>().getString(OS_Generator_FuelSupplyExtensibleFields::ConstituentName);
      boost::optional<double> molarFraction = group.cast<WorkspaceExtensibleGroup>().getDouble(OS_Generator_FuelSupplyExtensibleFields::ConstituentMolarFraction);

      if (name && molarFraction) {
        FuelSupplyConstituent constituent(name.get(), molarFraction.get());
        result.push_back(constituent);
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

  // From E+ 9.0.0, Microcogeneration.idf
  // LiquidGeneric is broken, see https://github.com/NREL/EnergyPlus/issues/6998
  setName("NATURALGAS");
  setFuelType("GaseousConstituents");
  addConstituent("METHANE", 0.9490);
  addConstituent("CarbonDioxide", 0.0070);
  addConstituent("NITROGEN", 0.0160);
  addConstituent("ETHANE", 0.0250);
  addConstituent("PROPANE", 0.0020);
  addConstituent("BUTANE", 0.0006);
  addConstituent("PENTANE", 0.0002);
  addConstituent("OXYGEN", 0.0002);
}

GeneratorFuelSupply::GeneratorFuelSupply(const Model& model)
  : ModelObject(GeneratorFuelSupply::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::GeneratorFuelSupply_Impl>());

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

  // From E+ 9.0.0, Microcogeneration.idf
  // LiquidGeneric is broken, see https://github.com/NREL/EnergyPlus/issues/6998
  setName("NATURALGAS");
  setFuelType("GaseousConstituents");
  addConstituent("METHANE", 0.9490);
  addConstituent("CarbonDioxide", 0.0070);
  addConstituent("NITROGEN", 0.0160);
  addConstituent("ETHANE", 0.0250);
  addConstituent("PROPANE", 0.0020);
  addConstituent("BUTANE", 0.0006);
  addConstituent("PENTANE", 0.0002);
  addConstituent("OXYGEN", 0.0002);
}

IddObjectType GeneratorFuelSupply::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Generator_FuelSupply);
}

double GeneratorFuelSupply::sumofConstituentsMolarFractions() const {
  return getImpl<detail::GeneratorFuelSupply_Impl>()->sumofConstituentsMolarFractions();
}

bool GeneratorFuelSupply::addConstituent(const FuelSupplyConstituent& constituent) {
  return getImpl<detail::GeneratorFuelSupply_Impl>()->addConstituent(constituent);
}

bool GeneratorFuelSupply::addConstituent(std::string name, double molarFraction) {
  return getImpl<detail::GeneratorFuelSupply_Impl>()->addConstituent(name, molarFraction);
}

// TODO: change to bool
void GeneratorFuelSupply::removeConstituent(int groupIndex) {
  getImpl<detail::GeneratorFuelSupply_Impl>()->removeConstituent(groupIndex);
}

void GeneratorFuelSupply::removeAllConstituents() {
  return getImpl<detail::GeneratorFuelSupply_Impl>()->removeAllConstituents();
}

std::vector<FuelSupplyConstituent> GeneratorFuelSupply::constituents() const {
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

bool GeneratorFuelSupply::setLiquidGenericFuelLowerHeatingValue(double liquidGenericFuelLowerHeatingValue) {
  return getImpl<detail::GeneratorFuelSupply_Impl>()->setLiquidGenericFuelLowerHeatingValue(liquidGenericFuelLowerHeatingValue);
}

void GeneratorFuelSupply::resetLiquidGenericFuelLowerHeatingValue() {
  getImpl<detail::GeneratorFuelSupply_Impl>()->resetLiquidGenericFuelLowerHeatingValue();
}

bool GeneratorFuelSupply::setLiquidGenericFuelHigherHeatingValue(double liquidGenericFuelHigherHeatingValue) {
  return getImpl<detail::GeneratorFuelSupply_Impl>()->setLiquidGenericFuelHigherHeatingValue(liquidGenericFuelHigherHeatingValue);
}

void GeneratorFuelSupply::resetLiquidGenericFuelHigherHeatingValue() {
  getImpl<detail::GeneratorFuelSupply_Impl>()->resetLiquidGenericFuelHigherHeatingValue();
}

bool GeneratorFuelSupply::setLiquidGenericFuelMolecularWeight(double liquidGenericFuelMolecularWeight) {
  return getImpl<detail::GeneratorFuelSupply_Impl>()->setLiquidGenericFuelMolecularWeight(liquidGenericFuelMolecularWeight);
}

void GeneratorFuelSupply::resetLiquidGenericFuelMolecularWeight() {
  getImpl<detail::GeneratorFuelSupply_Impl>()->resetLiquidGenericFuelMolecularWeight();
}

bool GeneratorFuelSupply::setLiquidGenericFuelCO2EmissionFactor(double liquidGenericFuelCO2EmissionFactor) {
  return getImpl<detail::GeneratorFuelSupply_Impl>()->setLiquidGenericFuelCO2EmissionFactor(liquidGenericFuelCO2EmissionFactor);
}

void GeneratorFuelSupply::resetLiquidGenericFuelCO2EmissionFactor() {
  getImpl<detail::GeneratorFuelSupply_Impl>()->resetLiquidGenericFuelCO2EmissionFactor();
}

boost::optional<GeneratorFuelCell> GeneratorFuelSupply::fuelCell() const {
  return getImpl<detail::GeneratorFuelSupply_Impl>()->fuelCell();
}

/// @cond
GeneratorFuelSupply::GeneratorFuelSupply(std::shared_ptr<detail::GeneratorFuelSupply_Impl> impl)
  : ModelObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio
