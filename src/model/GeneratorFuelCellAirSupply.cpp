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

#include "GeneratorFuelCellAirSupply.hpp"
#include "GeneratorFuelCellAirSupply_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"

#include "GeneratorFuelCell.hpp"
#include "GeneratorFuelCell_Impl.hpp"

#include "Node.hpp"
#include "Node_Impl.hpp"
#include "CurveCubic.hpp"
#include "CurveCubic_Impl.hpp"
#include "CurveQuadratic.hpp"
#include "CurveQuadratic_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Generator_FuelCell_AirSupply_FieldEnums.hxx>

#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

AirSupplyConstituent::AirSupplyConstituent(std::string constituentName, double molarFraction)
  : m_name(constituentName), m_molarFraction(molarFraction) {

  if ((m_molarFraction < 0) || (m_molarFraction > 1)) {
    LOG_AND_THROW("Unable to create constituent '" << m_name << "', molar fraction of " << m_molarFraction << " is outside the range [0, 1]");
  }
  if (!isValid(m_name)) {
    LOG_AND_THROW("ConstituentName '" << m_name << " is not valid. Check AirSupplyConstituent::constituentNameValues() to see possible names.");
  }
}

std::string AirSupplyConstituent::constituentName() const {
  return m_name;
}

double AirSupplyConstituent::molarFraction() const {
  return m_molarFraction;
}

bool AirSupplyConstituent::isValid(std::string constituentName) {
  std::vector<std::string> validConstituentNames = constituentNameValues();
  return std::find_if(validConstituentNames.begin(),
                      validConstituentNames.end(),
                      std::bind(istringEqual, constituentName, std::placeholders::_1)) != validConstituentNames.end();

}

std::vector<std::string> AirSupplyConstituent::constituentNameValues() {
  IddObject obj = IddFactory::instance().getObject(GeneratorFuelCellAirSupply::iddObjectType()).get();
  // Return IddKeyNames in extensible portion
  return getIddKeyNames(obj,
                        obj.numFields() + OS_Generator_FuelCell_AirSupplyExtensibleFields::ConstituentName);
}

std::vector<std::string> AirSupplyConstituent::validConstituentNameValues() {
  return constituentNameValues();
}

std::ostream& operator<< (std::ostream& out, const openstudio::model::AirSupplyConstituent& constituent) {
  out << "name=" << constituent.constituentName() << ", molar fraction=" << constituent.molarFraction();
  return out;
}

namespace detail {

  GeneratorFuelCellAirSupply_Impl::GeneratorFuelCellAirSupply_Impl(const IdfObject& idfObject,
                                                                   Model_Impl* model,
                                                                   bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == GeneratorFuelCellAirSupply::iddObjectType());
  }

  GeneratorFuelCellAirSupply_Impl::GeneratorFuelCellAirSupply_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                   Model_Impl* model,
                                                                   bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == GeneratorFuelCellAirSupply::iddObjectType());
  }

  GeneratorFuelCellAirSupply_Impl::GeneratorFuelCellAirSupply_Impl(const GeneratorFuelCellAirSupply_Impl& other,
                                                                   Model_Impl* model,
                                                                   bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& GeneratorFuelCellAirSupply_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    return result;
  }

  IddObjectType GeneratorFuelCellAirSupply_Impl::iddObjectType() const {
    return GeneratorFuelCellAirSupply::iddObjectType();
  }

  std::vector<IddObjectType> GeneratorFuelCellAirSupply_Impl::allowableChildTypes() const {
    std::vector<IddObjectType> result;
    result.push_back(IddObjectType::OS_Curve_Cubic);
    result.push_back(IddObjectType::OS_Curve_Quadratic);
    return result;
  }

  // Returns the children object
  std::vector<ModelObject> GeneratorFuelCellAirSupply_Impl::children() const {
    std::vector<ModelObject> result;
    boost::optional<CurveQuadratic> curveQ;
    boost::optional<CurveCubic> curveC;

    if ( (curveC = blowerPowerCurve()) ) {
      result.push_back(curveC.get());
    }
    if ( (curveQ = airRateFunctionofElectricPowerCurve()) ) {
      result.push_back(curveQ.get());
    }
    if ( (curveQ = airRateFunctionofFuelRateCurve()) ) {
      result.push_back(curveQ.get());
    }

    return result;
  }

  // Get the parent GeneratorFuelCell
  boost::optional<GeneratorFuelCell> GeneratorFuelCellAirSupply_Impl::fuelCell() const {

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

  boost::optional<Node> GeneratorFuelCellAirSupply_Impl::airInletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_Generator_FuelCell_AirSupplyFields::AirInletNodeName);
  }

  boost::optional<CurveCubic> GeneratorFuelCellAirSupply_Impl::blowerPowerCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveCubic>(OS_Generator_FuelCell_AirSupplyFields::BlowerPowerCurveName);
  }

  double GeneratorFuelCellAirSupply_Impl::blowerHeatLossFactor() const {
    boost::optional<double> value = getDouble(OS_Generator_FuelCell_AirSupplyFields::BlowerHeatLossFactor, true);
    if (!value) {
      LOG_AND_THROW(" does not have blowerHeatLossFactor.");
    }
    return value.get();
  }

  std::string GeneratorFuelCellAirSupply_Impl::airSupplyRateCalculationMode() const {
    boost::optional<std::string> value = getString(OS_Generator_FuelCell_AirSupplyFields::AirSupplyRateCalculationMode,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> GeneratorFuelCellAirSupply_Impl::stoichiometricRatio() const {
    boost::optional<double> value = getDouble(OS_Generator_FuelCell_AirSupplyFields::StoichiometricRatio, true);
    return value;
  }

  boost::optional<CurveQuadratic> GeneratorFuelCellAirSupply_Impl::airRateFunctionofElectricPowerCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveQuadratic>(OS_Generator_FuelCell_AirSupplyFields::AirRateFunctionofElectricPowerCurveName);
  }

  boost::optional<double> GeneratorFuelCellAirSupply_Impl::airRateAirTemperatureCoefficient() const {
    boost::optional<double> value = getDouble(OS_Generator_FuelCell_AirSupplyFields::AirRateAirTemperatureCoefficient, true);
    return value;
  }

  boost::optional<CurveQuadratic> GeneratorFuelCellAirSupply_Impl::airRateFunctionofFuelRateCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveQuadratic>(OS_Generator_FuelCell_AirSupplyFields::AirRateFunctionofFuelRateCurveName);
  }

  std::string GeneratorFuelCellAirSupply_Impl::airIntakeHeatRecoveryMode() const {
    boost::optional<std::string> value = getString(OS_Generator_FuelCell_AirSupplyFields::AirIntakeHeatRecoveryMode,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string GeneratorFuelCellAirSupply_Impl::airSupplyConstituentMode() const {
    boost::optional<std::string> value = getString(OS_Generator_FuelCell_AirSupplyFields::AirSupplyConstituentMode,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool GeneratorFuelCellAirSupply_Impl::setAirInletNode(const Node& connection) {
    bool result = setPointer(OS_Generator_FuelCell_AirSupplyFields::AirInletNodeName, connection.handle());
    return result;
  }

  void GeneratorFuelCellAirSupply_Impl::resetAirInletNode() {
    bool result = setString(OS_Generator_FuelCell_AirSupplyFields::AirInletNodeName, "");
    OS_ASSERT(result);
  }

  bool GeneratorFuelCellAirSupply_Impl::setBlowerPowerCurve(const CurveCubic& cubicCurves) {
    bool result = setPointer(OS_Generator_FuelCell_AirSupplyFields::BlowerPowerCurveName, cubicCurves.handle());
    return result;
  }

  void GeneratorFuelCellAirSupply_Impl::resetBlowerPowerCurve() {
    bool result = setString(OS_Generator_FuelCell_AirSupplyFields::BlowerPowerCurveName, "");
    OS_ASSERT(result);
  }

  bool GeneratorFuelCellAirSupply_Impl::setBlowerHeatLossFactor(double blowerHeatLossFactor) {
    bool result = setDouble(OS_Generator_FuelCell_AirSupplyFields::BlowerHeatLossFactor, blowerHeatLossFactor);
    return result;
  }

  void GeneratorFuelCellAirSupply_Impl::resetBlowerHeatLossFactor() {
    bool result = setDouble(OS_Generator_FuelCell_AirSupplyFields::BlowerHeatLossFactor, 0);
    OS_ASSERT(result);
  }

  bool GeneratorFuelCellAirSupply_Impl::setAirSupplyRateCalculationMode(const std::string& airSupplyRateCalculationMode) {
    bool result = setString(OS_Generator_FuelCell_AirSupplyFields::AirSupplyRateCalculationMode, airSupplyRateCalculationMode);
    return result;
  }

  bool GeneratorFuelCellAirSupply_Impl::setStoichiometricRatio(double stoichiometricRatio) {
    bool result = setDouble(OS_Generator_FuelCell_AirSupplyFields::StoichiometricRatio, stoichiometricRatio);
    OS_ASSERT(result);
    return result;
  }

  void GeneratorFuelCellAirSupply_Impl::resetStoichiometricRatio() {
    bool result = setString(OS_Generator_FuelCell_AirSupplyFields::StoichiometricRatio, "");
    OS_ASSERT(result);
  }

  bool GeneratorFuelCellAirSupply_Impl::setAirRateFunctionofElectricPowerCurve(const CurveQuadratic& quadraticCurves) {
    bool result = setPointer(OS_Generator_FuelCell_AirSupplyFields::AirRateFunctionofElectricPowerCurveName, quadraticCurves.handle());
    return result;
  }

  void GeneratorFuelCellAirSupply_Impl::resetAirRateFunctionofElectricPowerCurve() {
    bool result = setString(OS_Generator_FuelCell_AirSupplyFields::AirRateFunctionofElectricPowerCurveName, "");
    OS_ASSERT(result);
  }

  bool GeneratorFuelCellAirSupply_Impl::setAirRateAirTemperatureCoefficient(double airRateAirTemperatureCoefficient) {
    bool result = setDouble(OS_Generator_FuelCell_AirSupplyFields::AirRateAirTemperatureCoefficient, airRateAirTemperatureCoefficient);
    OS_ASSERT(result);
    return result;
  }

  void GeneratorFuelCellAirSupply_Impl::resetAirRateAirTemperatureCoefficient() {
    bool result = setDouble(OS_Generator_FuelCell_AirSupplyFields::AirRateAirTemperatureCoefficient, 0);
    OS_ASSERT(result);
  }

  bool GeneratorFuelCellAirSupply_Impl::setAirRateFunctionofFuelRateCurve(const CurveQuadratic& quadraticCurves) {
    bool result = setPointer(OS_Generator_FuelCell_AirSupplyFields::AirRateFunctionofFuelRateCurveName, quadraticCurves.handle());
    return result;
  }

  void GeneratorFuelCellAirSupply_Impl::resetAirRateFunctionofFuelRateCurve() {
    bool result = setString(OS_Generator_FuelCell_AirSupplyFields::AirRateFunctionofFuelRateCurveName, "");
    OS_ASSERT(result);
  }

  bool GeneratorFuelCellAirSupply_Impl::setAirIntakeHeatRecoveryMode(const std::string& airIntakeHeatRecoveryMode) {
    bool result = setString(OS_Generator_FuelCell_AirSupplyFields::AirIntakeHeatRecoveryMode, airIntakeHeatRecoveryMode);
    return result;
  }

  bool GeneratorFuelCellAirSupply_Impl::setAirSupplyConstituentMode(const std::string& airSupplyConstituentMode) {
    bool result = setString(OS_Generator_FuelCell_AirSupplyFields::AirSupplyConstituentMode, airSupplyConstituentMode);
    return result;
  }

  // TODO: this field shouldn't even exist in the IDD
  unsigned int GeneratorFuelCellAirSupply_Impl::numberofUserDefinedConstituents() const {
    return numExtensibleGroups();
  }

  // TODO: this shouldn't even exist in IDD
  bool GeneratorFuelCellAirSupply_Impl::setNumberofUserDefinedConstituents(unsigned int numberofUserDefinedConstituents) {
    bool result = setInt(OS_Generator_FuelCell_AirSupplyFields::NumberofUserDefinedConstituents, numberofUserDefinedConstituents);
    return result;
  }

  // TODO: this shouldn't even exist in IDD
  void GeneratorFuelCellAirSupply_Impl::resetNumberofUserDefinedConstituents() {
    bool result = setInt(OS_Generator_FuelCell_AirSupplyFields::NumberofUserDefinedConstituents, 0);
    OS_ASSERT(result);
  }

  double GeneratorFuelCellAirSupply_Impl::sumofConstituentsMolarFractions() const {
    double result = 0;
    if (numberofUserDefinedConstituents() == 0) {
      LOG(Warn, briefDescription() << " does not have any constituents");
    } else {
      for (const AirSupplyConstituent& constituent: constituents()) {
        result += constituent.molarFraction();
      }
    }

    return result;
  }

  bool GeneratorFuelCellAirSupply_Impl::addConstituent(const AirSupplyConstituent& constituent) {

    bool result;

    unsigned int num = numberofUserDefinedConstituents();
    // Max number of constituents is 5
    if (num >= 5) {
      LOG(Warn, briefDescription() << " already has 5 constituents which is the limit");
      result = false;
    } else {
      // Push an extensible group
      WorkspaceExtensibleGroup eg = getObject<ModelObject>().pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
      bool temp = eg.setString(OS_Generator_FuelCell_AirSupplyExtensibleFields::ConstituentName, constituent.constituentName());
      bool ok = eg.setDouble(OS_Generator_FuelCell_AirSupplyExtensibleFields::MolarFraction, constituent.molarFraction());
      if (temp && ok) {
        setNumberofUserDefinedConstituents(num + 1);
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

  bool GeneratorFuelCellAirSupply_Impl::addConstituent(std::string name, double molarFraction) {
    // Make a constituent (which will check for validity), and then call the above function
    AirSupplyConstituent constituent(name, molarFraction);
    return addConstituent(constituent);
  }

  bool GeneratorFuelCellAirSupply_Impl::removeConstituent(unsigned groupIndex) {
    bool result;

    unsigned int num = numberofUserDefinedConstituents();
    if (groupIndex < num) {
      getObject<ModelObject>().eraseExtensibleGroup(groupIndex);
      setNumberofUserDefinedConstituents(num - 1);
      result = true;
    } else {
      result = false;
    }
    return result;
  }

  void GeneratorFuelCellAirSupply_Impl::removeAllConstituents() {
    getObject<ModelObject>().clearExtensibleGroups();
    resetNumberofUserDefinedConstituents();
  }

  std::vector<AirSupplyConstituent> GeneratorFuelCellAirSupply_Impl::constituents() const {
    std::vector<AirSupplyConstituent> result;

    std::vector<IdfExtensibleGroup> groups = extensibleGroups();

    for (const auto & group : groups) {
      boost::optional<std::string> name = group.cast<WorkspaceExtensibleGroup>().getString(OS_Generator_FuelCell_AirSupplyExtensibleFields::ConstituentName);
      boost::optional<double> molarFraction = group.cast<WorkspaceExtensibleGroup>().getDouble(OS_Generator_FuelCell_AirSupplyExtensibleFields::MolarFraction);

      if (name && molarFraction) {
        AirSupplyConstituent constituent(name.get(), molarFraction.get());
        result.push_back(constituent);
      }
    }

    return result;
  }

} // detail

GeneratorFuelCellAirSupply::GeneratorFuelCellAirSupply(const Model& model,
                                                       const Node& airInletNode,
                                                       const CurveQuadratic& electricPowerCurve,
                                                       const CurveQuadratic& fuelRateCurve,
                                                       const CurveCubic& blowerPowerCurve)
  : ModelObject(GeneratorFuelCellAirSupply::iddObjectType(), model) {
  OS_ASSERT(getImpl<detail::GeneratorFuelCellAirSupply_Impl>());

  bool ok = setAirInletNode(airInletNode);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s airInletNode to "
      << airInletNode.briefDescription() << ".");
  }

  ok = setBlowerPowerCurve(blowerPowerCurve);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s blowerPowerCurve to "
      << blowerPowerCurve.briefDescription() << ".");
  }

  setBlowerHeatLossFactor(1.0);
  setAirSupplyRateCalculationMode("AirRatiobyStoics");
  setStoichiometricRatio(1.0);

  setAirRateFunctionofElectricPowerCurve(electricPowerCurve);
  setAirRateAirTemperatureCoefficient(0.00283);

  setAirRateFunctionofFuelRateCurve(fuelRateCurve);
  setAirIntakeHeatRecoveryMode("NoRecovery");
  setAirSupplyConstituentMode("AmbientAir");
}

GeneratorFuelCellAirSupply::GeneratorFuelCellAirSupply(const Model& model,
                                                       const Node& airInletNode,
                                                       const CurveQuadratic& electricPowerCurve,
                                                       const CurveQuadratic& fuelRateCurve)
  : ModelObject(GeneratorFuelCellAirSupply::iddObjectType(), model) {
  OS_ASSERT(getImpl<detail::GeneratorFuelCellAirSupply_Impl>());

  bool ok = setAirInletNode(airInletNode);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s airInletNode to "
      << airInletNode.briefDescription() << ".");
  }

  CurveCubic curveCubic(model);
  curveCubic.setCoefficient1Constant(0);
  curveCubic.setCoefficient2x(0);
  curveCubic.setCoefficient3xPOW2(0);
  curveCubic.setCoefficient4xPOW3(0);
  curveCubic.setMinimumValueofx(-1.0e10);
  curveCubic.setMaximumValueofx(1.0e10);
  ok = setBlowerPowerCurve(curveCubic);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s setBlowerPowerCurve to "
      << curveCubic.briefDescription() << ".");
  }

  setBlowerHeatLossFactor(1.0);
  setAirSupplyRateCalculationMode("AirRatiobyStoics");
  setStoichiometricRatio(1.0);

  ok = setAirRateFunctionofElectricPowerCurve(electricPowerCurve);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s electricPowerCurve to "
      << electricPowerCurve.briefDescription() << ".");
  }
  setAirRateAirTemperatureCoefficient(0.00283);

  ok = setAirRateFunctionofFuelRateCurve(fuelRateCurve);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s fuelRateCurve to "
      << fuelRateCurve.briefDescription() << ".");
  }
  setAirIntakeHeatRecoveryMode("NoRecovery");
  setAirSupplyConstituentMode("AmbientAir");
}

GeneratorFuelCellAirSupply::GeneratorFuelCellAirSupply(const Model& model,
  const Node& airInletNode)
  : ModelObject(GeneratorFuelCellAirSupply::iddObjectType(), model) {
  OS_ASSERT(getImpl<detail::GeneratorFuelCellAirSupply_Impl>());

  bool ok = setAirInletNode(airInletNode);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s airInletNode to "
      << airInletNode.briefDescription() << ".");
  }

  CurveCubic curveCubic(model);
  curveCubic.setCoefficient1Constant(0);
  curveCubic.setCoefficient2x(0);
  curveCubic.setCoefficient3xPOW2(0);
  curveCubic.setCoefficient4xPOW3(0);
  curveCubic.setMinimumValueofx(-1.0e10);
  curveCubic.setMaximumValueofx(1.0e10);
  ok = setBlowerPowerCurve(curveCubic);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s setBlowerPowerCurve to "
      << curveCubic.briefDescription() << ".");
  }

  setBlowerHeatLossFactor(1.0);
  setAirSupplyRateCalculationMode("AirRatiobyStoics");
  setStoichiometricRatio(1.0);

  setAirRateAirTemperatureCoefficient(0.00283);

  setAirIntakeHeatRecoveryMode("NoRecovery");
  setAirSupplyConstituentMode("AmbientAir");
}

GeneratorFuelCellAirSupply::GeneratorFuelCellAirSupply(const Model& model)
  : ModelObject(GeneratorFuelCellAirSupply::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::GeneratorFuelCellAirSupply_Impl>());
  //setAirInletNode();  //A new OA node is created on Forward Translation if one is not set, so this method can be left blank
  CurveCubic curveCubic(model);
  curveCubic.setCoefficient1Constant(0);
  curveCubic.setCoefficient2x(0);
  curveCubic.setCoefficient3xPOW2(0);
  curveCubic.setCoefficient4xPOW3(0);
  curveCubic.setMinimumValueofx(-1.0e10);
  curveCubic.setMaximumValueofx(1.0e10);
  curveCubic.setName("Blower Power Curve");
  bool ok = setBlowerPowerCurve(curveCubic);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s setBlowerPowerCurve to "
      << curveCubic.briefDescription() << ".");
  }

  setBlowerHeatLossFactor(1.0);
  setAirSupplyRateCalculationMode("AirRatiobyStoics");
  setStoichiometricRatio(1.0);
  setAirRateAirTemperatureCoefficient(0.00283);
  setAirIntakeHeatRecoveryMode("NoRecovery");
  setAirSupplyConstituentMode("AmbientAir");

  CurveQuadratic curveQ(model);
  curveQ.setCoefficient1Constant(1.50976E-3);
  curveQ.setCoefficient2x(-7.76656E-7);
  curveQ.setCoefficient3xPOW2(1.30317E-10);
  curveQ.setMinimumValueofx(-1.0e10);
  curveQ.setMaximumValueofx(1.0e10);
  curveQ.setName("Air Rate Function of Electric Power Curve");
  ok = setAirRateFunctionofElectricPowerCurve(curveQ);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s setBlowerPowerCurve to "
      << curveQ.briefDescription() << ".");
  }

  CurveQuadratic curveQ2(model);
  curveQ2.setMinimumValueofx(-1.0e10);
  curveQ2.setMaximumValueofx(1.0e10);
  curveQ2.setCoefficient1Constant(0);
  curveQ2.setCoefficient2x(0);
  curveQ2.setCoefficient3xPOW2(0);
  curveQ2.setName("Air Rate Function of Fuel Rate Curve");
  ok = setAirRateFunctionofFuelRateCurve(curveQ2);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s setBlowerPowerCurve to "
      << curveQ2.briefDescription() << ".");
  }

}

IddObjectType GeneratorFuelCellAirSupply::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Generator_FuelCell_AirSupply);
}

double GeneratorFuelCellAirSupply::sumofConstituentsMolarFractions() const {
  return getImpl<detail::GeneratorFuelCellAirSupply_Impl>()->sumofConstituentsMolarFractions();
}

bool GeneratorFuelCellAirSupply::addConstituent(const AirSupplyConstituent& constituent) {
  return getImpl<detail::GeneratorFuelCellAirSupply_Impl>()->addConstituent(constituent);
}

bool GeneratorFuelCellAirSupply::addConstituent(std::string name, double molarFraction) {
  return getImpl<detail::GeneratorFuelCellAirSupply_Impl>()->addConstituent(name, molarFraction);
}

// TODO: change to bool
void GeneratorFuelCellAirSupply::removeConstituent(int groupIndex) {
  getImpl<detail::GeneratorFuelCellAirSupply_Impl>()->removeConstituent(groupIndex);
}

void GeneratorFuelCellAirSupply::removeAllConstituents() {
  return getImpl<detail::GeneratorFuelCellAirSupply_Impl>()->removeAllConstituents();
}

std::vector<AirSupplyConstituent> GeneratorFuelCellAirSupply::constituents() const {
  return getImpl<detail::GeneratorFuelCellAirSupply_Impl>()->constituents();
}

std::vector<std::string> GeneratorFuelCellAirSupply::airSupplyRateCalculationModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Generator_FuelCell_AirSupplyFields::AirSupplyRateCalculationMode);
}

std::vector<std::string> GeneratorFuelCellAirSupply::airIntakeHeatRecoveryModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Generator_FuelCell_AirSupplyFields::AirIntakeHeatRecoveryMode);
}

std::vector<std::string> GeneratorFuelCellAirSupply::airSupplyConstituentModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Generator_FuelCell_AirSupplyFields::AirSupplyConstituentMode);
}

boost::optional<Node> GeneratorFuelCellAirSupply::airInletNode() const {
  return getImpl<detail::GeneratorFuelCellAirSupply_Impl>()->airInletNode();
}

boost::optional<CurveCubic> GeneratorFuelCellAirSupply::blowerPowerCurve() const {
  return getImpl<detail::GeneratorFuelCellAirSupply_Impl>()->blowerPowerCurve();
}

double GeneratorFuelCellAirSupply::blowerHeatLossFactor() const {
  return getImpl<detail::GeneratorFuelCellAirSupply_Impl>()->blowerHeatLossFactor();
}

std::string GeneratorFuelCellAirSupply::airSupplyRateCalculationMode() const {
  return getImpl<detail::GeneratorFuelCellAirSupply_Impl>()->airSupplyRateCalculationMode();
}

boost::optional<double> GeneratorFuelCellAirSupply::stoichiometricRatio() const {
  return getImpl<detail::GeneratorFuelCellAirSupply_Impl>()->stoichiometricRatio();
}

boost::optional<CurveQuadratic> GeneratorFuelCellAirSupply::airRateFunctionofElectricPowerCurve() const {
  return getImpl<detail::GeneratorFuelCellAirSupply_Impl>()->airRateFunctionofElectricPowerCurve();
}

boost::optional<double> GeneratorFuelCellAirSupply::airRateAirTemperatureCoefficient() const {
  return getImpl<detail::GeneratorFuelCellAirSupply_Impl>()->airRateAirTemperatureCoefficient();
}

boost::optional<CurveQuadratic> GeneratorFuelCellAirSupply::airRateFunctionofFuelRateCurve() const {
  return getImpl<detail::GeneratorFuelCellAirSupply_Impl>()->airRateFunctionofFuelRateCurve();
}

std::string GeneratorFuelCellAirSupply::airIntakeHeatRecoveryMode() const {
  return getImpl<detail::GeneratorFuelCellAirSupply_Impl>()->airIntakeHeatRecoveryMode();
}

std::string GeneratorFuelCellAirSupply::airSupplyConstituentMode() const {
  return getImpl<detail::GeneratorFuelCellAirSupply_Impl>()->airSupplyConstituentMode();
}

boost::optional<unsigned int> GeneratorFuelCellAirSupply::numberofUserDefinedConstituents() const {
  return getImpl<detail::GeneratorFuelCellAirSupply_Impl>()->numberofUserDefinedConstituents();
}

bool GeneratorFuelCellAirSupply::setAirInletNode(const Node& connection) {
  return getImpl<detail::GeneratorFuelCellAirSupply_Impl>()->setAirInletNode(connection);
}

void GeneratorFuelCellAirSupply::resetAirInletNode() {
  getImpl<detail::GeneratorFuelCellAirSupply_Impl>()->resetAirInletNode();
}

bool GeneratorFuelCellAirSupply::setBlowerPowerCurve(const CurveCubic& cubicCurves) {
  return getImpl<detail::GeneratorFuelCellAirSupply_Impl>()->setBlowerPowerCurve(cubicCurves);
}

void GeneratorFuelCellAirSupply::resetBlowerPowerCurve() {
  getImpl<detail::GeneratorFuelCellAirSupply_Impl>()->resetBlowerPowerCurve();
}

bool GeneratorFuelCellAirSupply::setBlowerHeatLossFactor(double blowerHeatLossFactor) {
  return getImpl<detail::GeneratorFuelCellAirSupply_Impl>()->setBlowerHeatLossFactor(blowerHeatLossFactor);
}

void GeneratorFuelCellAirSupply::resetBlowerHeatLossFactor() {
  getImpl<detail::GeneratorFuelCellAirSupply_Impl>()->resetBlowerHeatLossFactor();
}

bool GeneratorFuelCellAirSupply::setAirSupplyRateCalculationMode(const std::string& airSupplyRateCalculationMode) {
  return getImpl<detail::GeneratorFuelCellAirSupply_Impl>()->setAirSupplyRateCalculationMode(airSupplyRateCalculationMode);
}

bool GeneratorFuelCellAirSupply::setStoichiometricRatio(double stoichiometricRatio) {
  return getImpl<detail::GeneratorFuelCellAirSupply_Impl>()->setStoichiometricRatio(stoichiometricRatio);
}

void GeneratorFuelCellAirSupply::resetStoichiometricRatio() {
  getImpl<detail::GeneratorFuelCellAirSupply_Impl>()->resetStoichiometricRatio();
}

bool GeneratorFuelCellAirSupply::setAirRateFunctionofElectricPowerCurve(const CurveQuadratic& quadraticCurves) {
  return getImpl<detail::GeneratorFuelCellAirSupply_Impl>()->setAirRateFunctionofElectricPowerCurve(quadraticCurves);
}

void GeneratorFuelCellAirSupply::resetAirRateFunctionofElectricPowerCurve() {
  getImpl<detail::GeneratorFuelCellAirSupply_Impl>()->resetAirRateFunctionofElectricPowerCurve();
}

bool GeneratorFuelCellAirSupply::setAirRateAirTemperatureCoefficient(double airRateAirTemperatureCoefficient) {
  return getImpl<detail::GeneratorFuelCellAirSupply_Impl>()->setAirRateAirTemperatureCoefficient(airRateAirTemperatureCoefficient);
}

void GeneratorFuelCellAirSupply::resetAirRateAirTemperatureCoefficient() {
  getImpl<detail::GeneratorFuelCellAirSupply_Impl>()->resetAirRateAirTemperatureCoefficient();
}

bool GeneratorFuelCellAirSupply::setAirRateFunctionofFuelRateCurve(const CurveQuadratic& quadraticCurves) {
  return getImpl<detail::GeneratorFuelCellAirSupply_Impl>()->setAirRateFunctionofFuelRateCurve(quadraticCurves);
}

void GeneratorFuelCellAirSupply::resetAirRateFunctionofFuelRateCurve() {
  getImpl<detail::GeneratorFuelCellAirSupply_Impl>()->resetAirRateFunctionofFuelRateCurve();
}

bool GeneratorFuelCellAirSupply::setAirIntakeHeatRecoveryMode(const std::string& airIntakeHeatRecoveryMode) {
  return getImpl<detail::GeneratorFuelCellAirSupply_Impl>()->setAirIntakeHeatRecoveryMode(airIntakeHeatRecoveryMode);
}

bool GeneratorFuelCellAirSupply::setAirSupplyConstituentMode(const std::string& airSupplyConstituentMode) {
  return getImpl<detail::GeneratorFuelCellAirSupply_Impl>()->setAirSupplyConstituentMode(airSupplyConstituentMode);
}

boost::optional<GeneratorFuelCell> GeneratorFuelCellAirSupply::fuelCell() const {
  return getImpl<detail::GeneratorFuelCellAirSupply_Impl>()->fuelCell();
}

/// @cond
GeneratorFuelCellAirSupply::GeneratorFuelCellAirSupply(std::shared_ptr<detail::GeneratorFuelCellAirSupply_Impl> impl)
  : ModelObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio
