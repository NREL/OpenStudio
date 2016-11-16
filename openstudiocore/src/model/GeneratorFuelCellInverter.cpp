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

#include "GeneratorFuelCellInverter.hpp"
#include "GeneratorFuelCellInverter_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "CurveCubic.hpp"
#include "CurveCubic_Impl.hpp"
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

  GeneratorFuelCellInverter_Impl::GeneratorFuelCellInverter_Impl(const IdfObject& idfObject,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == GeneratorFuelCellInverter::iddObjectType());
  }

  GeneratorFuelCellInverter_Impl::GeneratorFuelCellInverter_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == GeneratorFuelCellInverter::iddObjectType());
  }

  GeneratorFuelCellInverter_Impl::GeneratorFuelCellInverter_Impl(const GeneratorFuelCellInverter_Impl& other,
                                                                 Model_Impl* model,
                                                                 bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& GeneratorFuelCellInverter_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType GeneratorFuelCellInverter_Impl::iddObjectType() const {
    return GeneratorFuelCellInverter::iddObjectType();
  }

  boost::optional<std::string> GeneratorFuelCellInverter_Impl::inverterEfficiencyCalculationMode() const {
    return getString(OS_Generator_FuelCell_InverterFields::InverterEfficiencyCalculationMode,true);
  }

  boost::optional<double> GeneratorFuelCellInverter_Impl::inverterEfficiency() const {
    return getDouble(OS_Generator_FuelCell_InverterFields::InverterEfficiency,true);
  }

  boost::optional<CurveQuadratic> GeneratorFuelCellInverter_Impl::efficiencyFunctionofDCPowerCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveQuadratic>(OS_Generator_FuelCell_InverterFields::EfficiencyFunctionofDCPowerCurveName);
  }

  bool GeneratorFuelCellInverter_Impl::setInverterEfficiencyCalculationMode(const std::string& inverterEfficiencyCalculationMode) {
    bool result = setString(OS_Generator_FuelCell_InverterFields::InverterEfficiencyCalculationMode, inverterEfficiencyCalculationMode);
    return result;
  }

  void GeneratorFuelCellInverter_Impl::resetInverterEfficiencyCalculationMode() {
    bool result = setString(OS_Generator_FuelCell_InverterFields::InverterEfficiencyCalculationMode, "");
    OS_ASSERT(result);
  }

  bool GeneratorFuelCellInverter_Impl::setInverterEfficiency(double inverterEfficiency) {
    bool result = setDouble(OS_Generator_FuelCell_InverterFields::InverterEfficiency, inverterEfficiency);
    return result;
  }

  void GeneratorFuelCellInverter_Impl::resetInverterEfficiency() {
    bool result = setString(OS_Generator_FuelCell_InverterFields::InverterEfficiency, "");
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

} // detail

GeneratorFuelCellInverter::GeneratorFuelCellInverter(const Model& model)
  : ModelObject(GeneratorFuelCellInverter::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::GeneratorFuelCellInverter_Impl>());

  setInverterEfficiencyCalculationMode("Constant");
  setInverterEfficiency(1.0);
}

IddObjectType GeneratorFuelCellInverter::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Generator_FuelCell_Inverter);
}

std::vector<std::string> GeneratorFuelCellInverter::inverterEfficiencyCalculationModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Generator_FuelCell_InverterFields::InverterEfficiencyCalculationMode);
}

boost::optional<std::string> GeneratorFuelCellInverter::inverterEfficiencyCalculationMode() const {
  return getImpl<detail::GeneratorFuelCellInverter_Impl>()->inverterEfficiencyCalculationMode();
}

boost::optional<double> GeneratorFuelCellInverter::inverterEfficiency() const {
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

/// @cond
GeneratorFuelCellInverter::GeneratorFuelCellInverter(std::shared_ptr<detail::GeneratorFuelCellInverter_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

