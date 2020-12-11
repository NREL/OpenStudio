/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "GeneratorFuelCellInverter.hpp"
#include "GeneratorFuelCellInverter_Impl.hpp"

#include "CurveQuadratic.hpp"
#include "CurveQuadratic_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Generator_FuelCell_Inverter_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    GeneratorFuelCellInverter_Impl::GeneratorFuelCellInverter_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == GeneratorFuelCellInverter::iddObjectType());
    }

    GeneratorFuelCellInverter_Impl::GeneratorFuelCellInverter_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                   bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == GeneratorFuelCellInverter::iddObjectType());
    }

    GeneratorFuelCellInverter_Impl::GeneratorFuelCellInverter_Impl(const GeneratorFuelCellInverter_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& GeneratorFuelCellInverter_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType GeneratorFuelCellInverter_Impl::iddObjectType() const {
      return GeneratorFuelCellInverter::iddObjectType();
    }

    std::vector<IddObjectType> GeneratorFuelCellInverter_Impl::allowableChildTypes() const {
      std::vector<IddObjectType> result;
      result.push_back(IddObjectType::OS_Curve_Quadratic);
      return result;
    }

    // Returns the children object
    std::vector<ModelObject> GeneratorFuelCellInverter_Impl::children() const {
      std::vector<ModelObject> result;
      boost::optional<CurveQuadratic> curveQ;

      if ((curveQ = efficiencyFunctionofDCPowerCurve())) {
        result.push_back(curveQ.get());
      }

      return result;
    }

    // Get the parent GeneratorFuelCell
    boost::optional<GeneratorFuelCell> GeneratorFuelCellInverter_Impl::fuelCell() const {

      boost::optional<GeneratorFuelCell> fc;
      // We use getModelObjectSources to check if more than one
      std::vector<GeneratorFuelCell> fcs = getObject<ModelObject>().getModelObjectSources<GeneratorFuelCell>(GeneratorFuelCell::iddObjectType());

      if (fcs.size() > 0u) {
        if (fcs.size() > 1u) {
          LOG(Error, briefDescription() << " is referenced by more than one GeneratorFuelCell, returning the first");
        }
        fc = fcs[0];
      }
      return fc;
    }

    std::string GeneratorFuelCellInverter_Impl::inverterEfficiencyCalculationMode() const {
      boost::optional<std::string> value = getString(OS_Generator_FuelCell_InverterFields::InverterEfficiencyCalculationMode, true);
      if (!value) {
        LOG_AND_THROW(" does not have inverterEfficiencyCalculationMode");
      }
      return value.get();
    }

    double GeneratorFuelCellInverter_Impl::inverterEfficiency() const {
      boost::optional<double> value = getDouble(OS_Generator_FuelCell_InverterFields::InverterEfficiency, true);
      if (!value) {
        LOG_AND_THROW(" does not have inverterEfficiency");
      }
      return value.get();
    }

    boost::optional<CurveQuadratic> GeneratorFuelCellInverter_Impl::efficiencyFunctionofDCPowerCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<CurveQuadratic>(
        OS_Generator_FuelCell_InverterFields::EfficiencyFunctionofDCPowerCurveName);
    }

    bool GeneratorFuelCellInverter_Impl::setInverterEfficiencyCalculationMode(const std::string& inverterEfficiencyCalculationMode) {
      bool result = setString(OS_Generator_FuelCell_InverterFields::InverterEfficiencyCalculationMode, inverterEfficiencyCalculationMode);
      return result;
    }

    void GeneratorFuelCellInverter_Impl::resetInverterEfficiencyCalculationMode() {
      bool result = setString(OS_Generator_FuelCell_InverterFields::InverterEfficiencyCalculationMode, "Constant");
      this->setInverterEfficiency(1.0);
      OS_ASSERT(result);
    }

    bool GeneratorFuelCellInverter_Impl::setInverterEfficiency(double inverterEfficiency) {
      bool result = setDouble(OS_Generator_FuelCell_InverterFields::InverterEfficiency, inverterEfficiency);
      return result;
    }

    void GeneratorFuelCellInverter_Impl::resetInverterEfficiency() {
      bool result = setDouble(OS_Generator_FuelCell_InverterFields::InverterEfficiency, 1);
      OS_ASSERT(result);
    }

    bool GeneratorFuelCellInverter_Impl::setEfficiencyFunctionofDCPowerCurve(const CurveQuadratic& quadraticCurves) {
      bool result = setPointer(OS_Generator_FuelCell_InverterFields::EfficiencyFunctionofDCPowerCurveName, quadraticCurves.handle());
      return result;
    }

    void GeneratorFuelCellInverter_Impl::resetEfficiencyFunctionofDCPowerCurve() {
      bool result = setString(OS_Generator_FuelCell_InverterFields::EfficiencyFunctionofDCPowerCurveName, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  GeneratorFuelCellInverter::GeneratorFuelCellInverter(const Model& model) : ModelObject(GeneratorFuelCellInverter::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::GeneratorFuelCellInverter_Impl>());

    setInverterEfficiencyCalculationMode("Constant");
    setInverterEfficiency(1.0);

    CurveQuadratic curveQ(model);
    curveQ.setCoefficient1Constant(0.560717);
    curveQ.setCoefficient2x(0.00012401);
    curveQ.setCoefficient3xPOW2(-0.0000000201648);
    curveQ.setMinimumValueofx(-1.0e10);
    curveQ.setMaximumValueofx(1.0e10);
    curveQ.setName("Efficiency Function of DC Power Curve");
    setEfficiencyFunctionofDCPowerCurve(curveQ);
  }

  GeneratorFuelCellInverter::GeneratorFuelCellInverter(const Model& model, const CurveQuadratic& powerCurve)
    : ModelObject(GeneratorFuelCellInverter::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::GeneratorFuelCellInverter_Impl>());

    setInverterEfficiencyCalculationMode("Quadratic");
    setEfficiencyFunctionofDCPowerCurve(powerCurve);
    setInverterEfficiency(1.0);
  }

  IddObjectType GeneratorFuelCellInverter::iddObjectType() {
    return IddObjectType(IddObjectType::OS_Generator_FuelCell_Inverter);
  }

  std::vector<std::string> GeneratorFuelCellInverter::inverterEfficiencyCalculationModeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_Generator_FuelCell_InverterFields::InverterEfficiencyCalculationMode);
  }

  std::string GeneratorFuelCellInverter::inverterEfficiencyCalculationMode() const {
    return getImpl<detail::GeneratorFuelCellInverter_Impl>()->inverterEfficiencyCalculationMode();
  }

  double GeneratorFuelCellInverter::inverterEfficiency() const {
    return getImpl<detail::GeneratorFuelCellInverter_Impl>()->inverterEfficiency();
  }

  boost::optional<CurveQuadratic> GeneratorFuelCellInverter::efficiencyFunctionofDCPowerCurve() const {
    return getImpl<detail::GeneratorFuelCellInverter_Impl>()->efficiencyFunctionofDCPowerCurve();
  }

  bool GeneratorFuelCellInverter::setInverterEfficiencyCalculationMode(const std::string& inverterEfficiencyCalculationMode) {
    return getImpl<detail::GeneratorFuelCellInverter_Impl>()->setInverterEfficiencyCalculationMode(inverterEfficiencyCalculationMode);
  }

  void GeneratorFuelCellInverter::resetInverterEfficiencyCalculationMode() {
    getImpl<detail::GeneratorFuelCellInverter_Impl>()->resetInverterEfficiencyCalculationMode();
  }

  bool GeneratorFuelCellInverter::setInverterEfficiency(double inverterEfficiency) {
    return getImpl<detail::GeneratorFuelCellInverter_Impl>()->setInverterEfficiency(inverterEfficiency);
  }

  void GeneratorFuelCellInverter::resetInverterEfficiency() {
    getImpl<detail::GeneratorFuelCellInverter_Impl>()->resetInverterEfficiency();
  }

  bool GeneratorFuelCellInverter::setEfficiencyFunctionofDCPowerCurve(const CurveQuadratic& quadraticCurves) {
    return getImpl<detail::GeneratorFuelCellInverter_Impl>()->setEfficiencyFunctionofDCPowerCurve(quadraticCurves);
  }

  void GeneratorFuelCellInverter::resetEfficiencyFunctionofDCPowerCurve() {
    getImpl<detail::GeneratorFuelCellInverter_Impl>()->resetEfficiencyFunctionofDCPowerCurve();
  }

  boost::optional<GeneratorFuelCell> GeneratorFuelCellInverter::fuelCell() const {
    return getImpl<detail::GeneratorFuelCellInverter_Impl>()->fuelCell();
  }

  /// @cond
  GeneratorFuelCellInverter::GeneratorFuelCellInverter(std::shared_ptr<detail::GeneratorFuelCellInverter_Impl> impl) : ModelObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
