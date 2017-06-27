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

#include "LuminaireDefinition.hpp"
#include "LuminaireDefinition_Impl.hpp"

#include <utilities/idd/OS_Luminaire_Definition_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/math/FloatCompare.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ATTRIBUTE_IMPLEMENTATION(0,1,0,lightingPower,LightingPower,
                           LuminaireDefinition,0,OS_Luminaire_Definition,LightingPower)

  ATTRIBUTE_IMPLEMENTATION(0,1,0,fractionRadiant,FractionRadiant,
                           LuminaireDefinition,0,OS_Luminaire_Definition,FractionRadiant)

  ATTRIBUTE_IMPLEMENTATION(0,1,0,fractionVisible,FractionVisible,
                           LuminaireDefinition,0,OS_Luminaire_Definition,FractionVisible)

  ATTRIBUTE_IMPLEMENTATION(0,1,0,returnAirFraction,ReturnAirFraction,
                           LuminaireDefinition,0,OS_Luminaire_Definition,ReturnAirFraction)

  LuminaireDefinition_Impl::LuminaireDefinition_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : SpaceLoadDefinition_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == LuminaireDefinition::iddObjectType());
  }

  LuminaireDefinition_Impl::LuminaireDefinition_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                     Model_Impl* model,
                                                     bool keepHandle)
    : SpaceLoadDefinition_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == LuminaireDefinition::iddObjectType());
  }

  LuminaireDefinition_Impl::LuminaireDefinition_Impl(const LuminaireDefinition_Impl& other,
                                                     Model_Impl* model,
                                                     bool keepHandle)
    : SpaceLoadDefinition_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& LuminaireDefinition_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType LuminaireDefinition_Impl::iddObjectType() const {
    return LuminaireDefinition::iddObjectType();
  }

  double LuminaireDefinition_Impl::lightingPower() const {
    boost::optional<double> value = getDouble(OS_Luminaire_DefinitionFields::LightingPower,true);
    OS_ASSERT(value);
    return value.get();
  }

  //bool LuminaireDefinition_Impl::isLightingPowerDefaulted() const {
  //  return isEmpty(OS_Luminaire_DefinitionFields::LightingPower);
  //}

  double LuminaireDefinition_Impl::fractionRadiant() const {
    boost::optional<double> value = getDouble(OS_Luminaire_DefinitionFields::FractionRadiant,true);
    OS_ASSERT(value);
    return value.get();
  }

  //bool LuminaireDefinition_Impl::isFractionRadiantDefaulted() const {
  //  return isEmpty(OS_Luminaire_DefinitionFields::FractionRadiant);
  //}

  double LuminaireDefinition_Impl::fractionVisible() const {
    boost::optional<double> value = getDouble(OS_Luminaire_DefinitionFields::FractionVisible,true);
    OS_ASSERT(value);
    return value.get();
  }

  //bool LuminaireDefinition_Impl::isFractionVisibleDefaulted() const {
  //  return isEmpty(OS_Luminaire_DefinitionFields::FractionVisible);
  //}

  double LuminaireDefinition_Impl::returnAirFraction() const {
    boost::optional<double> value = getDouble(OS_Luminaire_DefinitionFields::ReturnAirFraction,true);
    OS_ASSERT(value);
    return value.get();
  }

  //bool LuminaireDefinition_Impl::isReturnAirFractionDefaulted() const {
  //  return isEmpty(OS_Luminaire_DefinitionFields::ReturnAirFraction);
  //}

  bool LuminaireDefinition_Impl::returnAirFractionCalculatedfromPlenumTemperature() const {
    boost::optional<std::string> value = getString(OS_Luminaire_DefinitionFields::ReturnAirFractionCalculatedfromPlenumTemperature,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }

  bool LuminaireDefinition_Impl::isReturnAirFractionCalculatedfromPlenumTemperatureDefaulted() const {
    return isEmpty(OS_Luminaire_DefinitionFields::ReturnAirFractionCalculatedfromPlenumTemperature);
  }

  double LuminaireDefinition_Impl::returnAirFractionFunctionofPlenumTemperatureCoefficient1() const {
    boost::optional<double> value = getDouble(OS_Luminaire_DefinitionFields::ReturnAirFractionFunctionofPlenumTemperatureCoefficient1,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool LuminaireDefinition_Impl::isReturnAirFractionFunctionofPlenumTemperatureCoefficient1Defaulted() const {
    return isEmpty(OS_Luminaire_DefinitionFields::ReturnAirFractionFunctionofPlenumTemperatureCoefficient1);
  }

  double LuminaireDefinition_Impl::returnAirFractionFunctionofPlenumTemperatureCoefficient2() const {
    boost::optional<double> value = getDouble(OS_Luminaire_DefinitionFields::ReturnAirFractionFunctionofPlenumTemperatureCoefficient2,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool LuminaireDefinition_Impl::isReturnAirFractionFunctionofPlenumTemperatureCoefficient2Defaulted() const {
    return isEmpty(OS_Luminaire_DefinitionFields::ReturnAirFractionFunctionofPlenumTemperatureCoefficient2);
  }

  bool LuminaireDefinition_Impl::setLightingPower(double lightingPower) {
    bool result = setDouble(OS_Luminaire_DefinitionFields::LightingPower, lightingPower);
    return result;
  }

  //void LuminaireDefinition_Impl::resetLightingPower() {
  //  bool result = setString(OS_Luminaire_DefinitionFields::LightingPower, "");
  //  OS_ASSERT(result);
  //}

  bool LuminaireDefinition_Impl::setFractionRadiant(double fractionRadiant) {
    bool result = setDouble(OS_Luminaire_DefinitionFields::FractionRadiant, fractionRadiant);
    return result;
  }

  //void LuminaireDefinition_Impl::resetFractionRadiant() {
  //  bool result = setString(OS_Luminaire_DefinitionFields::FractionRadiant, "");
  //  OS_ASSERT(result);
  //}

  bool LuminaireDefinition_Impl::setFractionVisible(double fractionVisible) {
    bool result = setDouble(OS_Luminaire_DefinitionFields::FractionVisible, fractionVisible);
    return result;
  }

  //void LuminaireDefinition_Impl::resetFractionVisible() {
  //  bool result = setString(OS_Luminaire_DefinitionFields::FractionVisible, "");
  //  OS_ASSERT(result);
  //}

  bool LuminaireDefinition_Impl::setReturnAirFraction(double returnAirFraction) {
    bool result = setDouble(OS_Luminaire_DefinitionFields::ReturnAirFraction, returnAirFraction);
    return result;
  }

  //void LuminaireDefinition_Impl::resetReturnAirFraction() {
  //  bool result = setString(OS_Luminaire_DefinitionFields::ReturnAirFraction, "");
  //  OS_ASSERT(result);
  //}

  void LuminaireDefinition_Impl::setReturnAirFractionCalculatedfromPlenumTemperature(bool returnAirFractionCalculatedfromPlenumTemperature) {
    bool result = false;
    if (returnAirFractionCalculatedfromPlenumTemperature) {
      result = setString(OS_Luminaire_DefinitionFields::ReturnAirFractionCalculatedfromPlenumTemperature, "Yes");
    } else {
      result = setString(OS_Luminaire_DefinitionFields::ReturnAirFractionCalculatedfromPlenumTemperature, "No");
    }
    OS_ASSERT(result);
  }

  void LuminaireDefinition_Impl::resetReturnAirFractionCalculatedfromPlenumTemperature() {
    bool result = setString(OS_Luminaire_DefinitionFields::ReturnAirFractionCalculatedfromPlenumTemperature, "");
    OS_ASSERT(result);
  }

  bool LuminaireDefinition_Impl::setReturnAirFractionFunctionofPlenumTemperatureCoefficient1(double returnAirFractionFunctionofPlenumTemperatureCoefficient1) {
    bool result = setDouble(OS_Luminaire_DefinitionFields::ReturnAirFractionFunctionofPlenumTemperatureCoefficient1, returnAirFractionFunctionofPlenumTemperatureCoefficient1);
    return result;
  }

  void LuminaireDefinition_Impl::resetReturnAirFractionFunctionofPlenumTemperatureCoefficient1() {
    bool result = setString(OS_Luminaire_DefinitionFields::ReturnAirFractionFunctionofPlenumTemperatureCoefficient1, "");
    OS_ASSERT(result);
  }

  bool LuminaireDefinition_Impl::setReturnAirFractionFunctionofPlenumTemperatureCoefficient2(double returnAirFractionFunctionofPlenumTemperatureCoefficient2) {
    bool result = setDouble(OS_Luminaire_DefinitionFields::ReturnAirFractionFunctionofPlenumTemperatureCoefficient2, returnAirFractionFunctionofPlenumTemperatureCoefficient2);
    return result;
  }

  void LuminaireDefinition_Impl::resetReturnAirFractionFunctionofPlenumTemperatureCoefficient2() {
    bool result = setString(OS_Luminaire_DefinitionFields::ReturnAirFractionFunctionofPlenumTemperatureCoefficient2, "");
    OS_ASSERT(result);
  }

} // detail

LuminaireDefinition::LuminaireDefinition(const Model& model)
  : SpaceLoadDefinition(LuminaireDefinition::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::LuminaireDefinition_Impl>());
}

IddObjectType LuminaireDefinition::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Luminaire_Definition);
  return result;
}

double LuminaireDefinition::lightingPower() const {
  return getImpl<detail::LuminaireDefinition_Impl>()->lightingPower();
}

bool LuminaireDefinition::isLightingPowerDefaulted() const {
  return getImpl<detail::LuminaireDefinition_Impl>()->isLightingPowerDefaulted();
}

double LuminaireDefinition::fractionRadiant() const {
  return getImpl<detail::LuminaireDefinition_Impl>()->fractionRadiant();
}

bool LuminaireDefinition::isFractionRadiantDefaulted() const {
  return getImpl<detail::LuminaireDefinition_Impl>()->isFractionRadiantDefaulted();
}

double LuminaireDefinition::fractionVisible() const {
  return getImpl<detail::LuminaireDefinition_Impl>()->fractionVisible();
}

bool LuminaireDefinition::isFractionVisibleDefaulted() const {
  return getImpl<detail::LuminaireDefinition_Impl>()->isFractionVisibleDefaulted();
}

double LuminaireDefinition::returnAirFraction() const {
  return getImpl<detail::LuminaireDefinition_Impl>()->returnAirFraction();
}

bool LuminaireDefinition::isReturnAirFractionDefaulted() const {
  return getImpl<detail::LuminaireDefinition_Impl>()->isReturnAirFractionDefaulted();
}

bool LuminaireDefinition::returnAirFractionCalculatedfromPlenumTemperature() const {
  return getImpl<detail::LuminaireDefinition_Impl>()->returnAirFractionCalculatedfromPlenumTemperature();
}

bool LuminaireDefinition::isReturnAirFractionCalculatedfromPlenumTemperatureDefaulted() const {
  return getImpl<detail::LuminaireDefinition_Impl>()->isReturnAirFractionCalculatedfromPlenumTemperatureDefaulted();
}

double LuminaireDefinition::returnAirFractionFunctionofPlenumTemperatureCoefficient1() const {
  return getImpl<detail::LuminaireDefinition_Impl>()->returnAirFractionFunctionofPlenumTemperatureCoefficient1();
}

bool LuminaireDefinition::isReturnAirFractionFunctionofPlenumTemperatureCoefficient1Defaulted() const {
  return getImpl<detail::LuminaireDefinition_Impl>()->isReturnAirFractionFunctionofPlenumTemperatureCoefficient1Defaulted();
}

double LuminaireDefinition::returnAirFractionFunctionofPlenumTemperatureCoefficient2() const {
  return getImpl<detail::LuminaireDefinition_Impl>()->returnAirFractionFunctionofPlenumTemperatureCoefficient2();
}

bool LuminaireDefinition::isReturnAirFractionFunctionofPlenumTemperatureCoefficient2Defaulted() const {
  return getImpl<detail::LuminaireDefinition_Impl>()->isReturnAirFractionFunctionofPlenumTemperatureCoefficient2Defaulted();
}

bool LuminaireDefinition::setLightingPower(double lightingPower) {
  return getImpl<detail::LuminaireDefinition_Impl>()->setLightingPower(lightingPower);
}

void LuminaireDefinition::resetLightingPower() {
  getImpl<detail::LuminaireDefinition_Impl>()->resetLightingPower();
}

bool LuminaireDefinition::setFractionRadiant(double fractionRadiant) {
  return getImpl<detail::LuminaireDefinition_Impl>()->setFractionRadiant(fractionRadiant);
}

void LuminaireDefinition::resetFractionRadiant() {
  getImpl<detail::LuminaireDefinition_Impl>()->resetFractionRadiant();
}

bool LuminaireDefinition::setFractionVisible(double fractionVisible) {
  return getImpl<detail::LuminaireDefinition_Impl>()->setFractionVisible(fractionVisible);
}

void LuminaireDefinition::resetFractionVisible() {
  getImpl<detail::LuminaireDefinition_Impl>()->resetFractionVisible();
}

bool LuminaireDefinition::setReturnAirFraction(double returnAirFraction) {
  return getImpl<detail::LuminaireDefinition_Impl>()->setReturnAirFraction(returnAirFraction);
}

void LuminaireDefinition::resetReturnAirFraction() {
  getImpl<detail::LuminaireDefinition_Impl>()->resetReturnAirFraction();
}

void LuminaireDefinition::setReturnAirFractionCalculatedfromPlenumTemperature(bool returnAirFractionCalculatedfromPlenumTemperature) {
  getImpl<detail::LuminaireDefinition_Impl>()->setReturnAirFractionCalculatedfromPlenumTemperature(returnAirFractionCalculatedfromPlenumTemperature);
}

void LuminaireDefinition::resetReturnAirFractionCalculatedfromPlenumTemperature() {
  getImpl<detail::LuminaireDefinition_Impl>()->resetReturnAirFractionCalculatedfromPlenumTemperature();
}

bool LuminaireDefinition::setReturnAirFractionFunctionofPlenumTemperatureCoefficient1(double returnAirFractionFunctionofPlenumTemperatureCoefficient1) {
  return getImpl<detail::LuminaireDefinition_Impl>()->setReturnAirFractionFunctionofPlenumTemperatureCoefficient1(returnAirFractionFunctionofPlenumTemperatureCoefficient1);
}

void LuminaireDefinition::resetReturnAirFractionFunctionofPlenumTemperatureCoefficient1() {
  getImpl<detail::LuminaireDefinition_Impl>()->resetReturnAirFractionFunctionofPlenumTemperatureCoefficient1();
}

bool LuminaireDefinition::setReturnAirFractionFunctionofPlenumTemperatureCoefficient2(double returnAirFractionFunctionofPlenumTemperatureCoefficient2) {
  return getImpl<detail::LuminaireDefinition_Impl>()->setReturnAirFractionFunctionofPlenumTemperatureCoefficient2(returnAirFractionFunctionofPlenumTemperatureCoefficient2);
}

void LuminaireDefinition::resetReturnAirFractionFunctionofPlenumTemperatureCoefficient2() {
  getImpl<detail::LuminaireDefinition_Impl>()->resetReturnAirFractionFunctionofPlenumTemperatureCoefficient2();
}

/// @cond
LuminaireDefinition::LuminaireDefinition(std::shared_ptr<detail::LuminaireDefinition_Impl> impl)
  : SpaceLoadDefinition(std::move(impl))
{}
/// @endcond


} // model
} // openstudio

