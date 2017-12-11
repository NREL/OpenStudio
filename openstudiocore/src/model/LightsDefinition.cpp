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

#include "LightsDefinition.hpp"
#include "LightsDefinition_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Lights_Definition_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/math/FloatCompare.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ATTRIBUTE_IMPLEMENTATION(1,0,0,lightingLevel,LightingLevel,
                           LightsDefinition,0,OS_Lights_Definition,LightingLevel)

  ATTRIBUTE_IMPLEMENTATION(1,0,0,wattsperSpaceFloorArea,WattsperSpaceFloorArea,
                           LightsDefinition,0,OS_Lights_Definition,WattsperSpaceFloorArea)

  ATTRIBUTE_IMPLEMENTATION(1,0,0,wattsperPerson,WattsperPerson,
                           LightsDefinition,0,OS_Lights_Definition,WattsperPerson)

  ATTRIBUTE_IMPLEMENTATION(0,1,0,fractionRadiant,FractionRadiant,
                           LightsDefinition,0,OS_Lights_Definition,FractionRadiant)

  ATTRIBUTE_IMPLEMENTATION(0,1,0,fractionVisible,FractionVisible,
                           LightsDefinition,0,OS_Lights_Definition,FractionVisible)

  ATTRIBUTE_IMPLEMENTATION(0,1,0,returnAirFraction,ReturnAirFraction,
                           LightsDefinition,0,OS_Lights_Definition,ReturnAirFraction)

  LightsDefinition_Impl::LightsDefinition_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : SpaceLoadDefinition_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == LightsDefinition::iddObjectType());
  }

  LightsDefinition_Impl::LightsDefinition_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                               Model_Impl* model,
                                               bool keepHandle)
    : SpaceLoadDefinition_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == LightsDefinition::iddObjectType());
  }

  LightsDefinition_Impl::LightsDefinition_Impl(const LightsDefinition_Impl& other,
                                               Model_Impl* model,
                                               bool keepHandle)
    : SpaceLoadDefinition_Impl(other,model,keepHandle)
  {}

  // TODO: remove
  const std::vector<std::string>& LightsDefinition_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty())
    {
      // Not appropriate: output is listed in Lights instead
    }
    return result;
  }

  IddObjectType LightsDefinition_Impl::iddObjectType() const {
    return LightsDefinition::iddObjectType();
  }

  std::string LightsDefinition_Impl::designLevelCalculationMethod() const {
    boost::optional<std::string> value = getString(OS_Lights_DefinitionFields::DesignLevelCalculationMethod,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> LightsDefinition_Impl::lightingLevel() const {
    boost::optional<double> result;
    if (istringEqual("LightingLevel", this->designLevelCalculationMethod())){
      result = getDouble(OS_Lights_DefinitionFields::LightingLevel,true);
      //OS_ASSERT(result);
    }
    return result;
  }

  boost::optional<double> LightsDefinition_Impl::wattsperSpaceFloorArea() const {
    boost::optional<double> result;
    if (istringEqual("Watts/Area", this->designLevelCalculationMethod())){
      result = getDouble(OS_Lights_DefinitionFields::WattsperSpaceFloorArea,true);
      //OS_ASSERT(result);
    }
    return result;
  }

  boost::optional<double> LightsDefinition_Impl::wattsperPerson() const {
    boost::optional<double> result;
    if (istringEqual("Watts/Person", this->designLevelCalculationMethod())){
      result = getDouble(OS_Lights_DefinitionFields::WattsperPerson,true);
      //OS_ASSERT(result);
    }
    return result;
  }

  double LightsDefinition_Impl::fractionRadiant() const {
    boost::optional<double> value = getDouble(OS_Lights_DefinitionFields::FractionRadiant,true);
    OS_ASSERT(value);
    return value.get();
  }

  //bool LightsDefinition_Impl::isFractionRadiantDefaulted() const {
  //  return isEmpty(OS_Lights_DefinitionFields::FractionRadiant);
  //}

  double LightsDefinition_Impl::fractionVisible() const {
    boost::optional<double> value = getDouble(OS_Lights_DefinitionFields::FractionVisible,true);
    OS_ASSERT(value);
    return value.get();
  }

  //bool LightsDefinition_Impl::isFractionVisibleDefaulted() const {
  //  return isEmpty(OS_Lights_DefinitionFields::FractionVisible);
  //}

  double LightsDefinition_Impl::returnAirFraction() const {
    boost::optional<double> value = getDouble(OS_Lights_DefinitionFields::ReturnAirFraction,true);
    OS_ASSERT(value);
    return value.get();
  }

  //bool LightsDefinition_Impl::isReturnAirFractionDefaulted() const {
  //  return isEmpty(OS_Lights_DefinitionFields::ReturnAirFraction);
  //}

  bool LightsDefinition_Impl::returnAirFractionCalculatedfromPlenumTemperature() const {
    boost::optional<std::string> value = getString(OS_Lights_DefinitionFields::ReturnAirFractionCalculatedfromPlenumTemperature,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }

  bool LightsDefinition_Impl::isReturnAirFractionCalculatedfromPlenumTemperatureDefaulted() const {
    return isEmpty(OS_Lights_DefinitionFields::ReturnAirFractionCalculatedfromPlenumTemperature);
  }

  double LightsDefinition_Impl::returnAirFractionFunctionofPlenumTemperatureCoefficient1() const {
    boost::optional<double> value = getDouble(OS_Lights_DefinitionFields::ReturnAirFractionFunctionofPlenumTemperatureCoefficient1,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool LightsDefinition_Impl::isReturnAirFractionFunctionofPlenumTemperatureCoefficient1Defaulted() const {
    return isEmpty(OS_Lights_DefinitionFields::ReturnAirFractionFunctionofPlenumTemperatureCoefficient1);
  }

  double LightsDefinition_Impl::returnAirFractionFunctionofPlenumTemperatureCoefficient2() const {
    boost::optional<double> value = getDouble(OS_Lights_DefinitionFields::ReturnAirFractionFunctionofPlenumTemperatureCoefficient2,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool LightsDefinition_Impl::isReturnAirFractionFunctionofPlenumTemperatureCoefficient2Defaulted() const {
    return isEmpty(OS_Lights_DefinitionFields::ReturnAirFractionFunctionofPlenumTemperatureCoefficient2);
  }

  bool LightsDefinition_Impl::setLightingLevel(boost::optional<double> lightingLevel) {
    bool result = true;
    if (lightingLevel) {
      if (*lightingLevel < 0){
        result = false;
      }else{
        // This one could return false for good reasons, if NaN of Infinity was passed
        result = setDouble(OS_Lights_DefinitionFields::LightingLevel, lightingLevel.get());
        if (result) {
          result = setString(OS_Lights_DefinitionFields::DesignLevelCalculationMethod, "LightingLevel");
          OS_ASSERT(result);
          result = setString(OS_Lights_DefinitionFields::WattsperSpaceFloorArea, "");
          OS_ASSERT(result);
          result = setString(OS_Lights_DefinitionFields::WattsperPerson, "");
          OS_ASSERT(result);
        }
      }
    } else {
      if (istringEqual("LightingLevel", this->designLevelCalculationMethod())){
        result = setDouble(OS_Lights_DefinitionFields::LightingLevel, 0.0);
      }
    }
    return result;
  }

  bool LightsDefinition_Impl::setWattsperSpaceFloorArea(boost::optional<double> wattsperSpaceFloorArea) {
    bool result = true;
    if (wattsperSpaceFloorArea) {
      if (*wattsperSpaceFloorArea < 0){
        result = false;
      }else{
        // This one could return false for good reasons, if NaN of Infinity was passed
        result = setDouble(OS_Lights_DefinitionFields::WattsperSpaceFloorArea, wattsperSpaceFloorArea.get());
        if (result) {
          result = setString(OS_Lights_DefinitionFields::DesignLevelCalculationMethod, "Watts/Area");
          OS_ASSERT(result);
          result = setString(OS_Lights_DefinitionFields::LightingLevel, "");
          OS_ASSERT(result);
          result = setString(OS_Lights_DefinitionFields::WattsperPerson, "");
          OS_ASSERT(result);
        }
      }
    } else {
      if (istringEqual("Watts/Area", this->designLevelCalculationMethod())){
        result = setDouble(OS_Lights_DefinitionFields::WattsperSpaceFloorArea, 0.0);
      }
    }
    return result;
  }

  bool LightsDefinition_Impl::setWattsperPerson(boost::optional<double> wattsperPerson) {
    bool result = true;
    if (wattsperPerson) {
      if (*wattsperPerson < 0){
        result = false;
      }else{
        // This one could return false for good reasons, if NaN of Infinity was passed
        result = setDouble(OS_Lights_DefinitionFields::WattsperPerson, wattsperPerson.get());
        if (result) {
          result = setString(OS_Lights_DefinitionFields::DesignLevelCalculationMethod, "Watts/Person");
          OS_ASSERT(result);
          result = setString(OS_Lights_DefinitionFields::LightingLevel, "");
          OS_ASSERT(result);
          result = setString(OS_Lights_DefinitionFields::WattsperSpaceFloorArea, "");
          OS_ASSERT(result);
        }
      }
    } else {
      if (istringEqual("Watts/Person", this->designLevelCalculationMethod())){
        result = setDouble(OS_Lights_DefinitionFields::WattsperPerson, 0.0);
      }
    }
    return result;
  }

  bool LightsDefinition_Impl::setFractionRadiant(double fractionRadiant) {
    bool result = setDouble(OS_Lights_DefinitionFields::FractionRadiant, fractionRadiant);
    return result;
  }

  //void LightsDefinition_Impl::resetFractionRadiant() {
  //  bool result = setString(OS_Lights_DefinitionFields::FractionRadiant, "");
  //  OS_ASSERT(result);
  //}

  bool LightsDefinition_Impl::setFractionVisible(double fractionVisible) {
    bool result = setDouble(OS_Lights_DefinitionFields::FractionVisible, fractionVisible);
    return result;
  }

  //void LightsDefinition_Impl::resetFractionVisible() {
  //  bool result = setString(OS_Lights_DefinitionFields::FractionVisible, "");
  //  OS_ASSERT(result);
  //}

  bool LightsDefinition_Impl::setReturnAirFraction(double returnAirFraction) {
    bool result = setDouble(OS_Lights_DefinitionFields::ReturnAirFraction, returnAirFraction);
    return result;
  }

  //void LightsDefinition_Impl::resetReturnAirFraction() {
  //  bool result = setString(OS_Lights_DefinitionFields::ReturnAirFraction, "");
  //  OS_ASSERT(result);
  //}

  void LightsDefinition_Impl::setReturnAirFractionCalculatedfromPlenumTemperature(bool returnAirFractionCalculatedfromPlenumTemperature) {
    bool result = false;
    if (returnAirFractionCalculatedfromPlenumTemperature) {
      result = setString(OS_Lights_DefinitionFields::ReturnAirFractionCalculatedfromPlenumTemperature, "Yes");
    } else {
      result = setString(OS_Lights_DefinitionFields::ReturnAirFractionCalculatedfromPlenumTemperature, "No");
    }
    OS_ASSERT(result);
  }

  void LightsDefinition_Impl::resetReturnAirFractionCalculatedfromPlenumTemperature() {
    bool result = setString(OS_Lights_DefinitionFields::ReturnAirFractionCalculatedfromPlenumTemperature, "");
    OS_ASSERT(result);
  }

  bool LightsDefinition_Impl::setReturnAirFractionFunctionofPlenumTemperatureCoefficient1(double returnAirFractionFunctionofPlenumTemperatureCoefficient1) {
    bool result = setDouble(OS_Lights_DefinitionFields::ReturnAirFractionFunctionofPlenumTemperatureCoefficient1, returnAirFractionFunctionofPlenumTemperatureCoefficient1);
    return result;
  }

  void LightsDefinition_Impl::resetReturnAirFractionFunctionofPlenumTemperatureCoefficient1() {
    bool result = setString(OS_Lights_DefinitionFields::ReturnAirFractionFunctionofPlenumTemperatureCoefficient1, "");
    OS_ASSERT(result);
  }

  bool LightsDefinition_Impl::setReturnAirFractionFunctionofPlenumTemperatureCoefficient2(double returnAirFractionFunctionofPlenumTemperatureCoefficient2) {
    bool result = setDouble(OS_Lights_DefinitionFields::ReturnAirFractionFunctionofPlenumTemperatureCoefficient2, returnAirFractionFunctionofPlenumTemperatureCoefficient2);
    return result;
  }

  void LightsDefinition_Impl::resetReturnAirFractionFunctionofPlenumTemperatureCoefficient2() {
    bool result = setString(OS_Lights_DefinitionFields::ReturnAirFractionFunctionofPlenumTemperatureCoefficient2, "");
    OS_ASSERT(result);
  }

  double LightsDefinition_Impl::getLightingPower(double floorArea, double numPeople) const {
    std::string method = designLevelCalculationMethod();

    if (method == "LightingLevel") {
      return lightingLevel().get();
    }
    else if (method == "Watts/Area") {
      return wattsperSpaceFloorArea().get() * floorArea;
    }
    else if (method == "Watts/Person") {
      return wattsperPerson().get() * numPeople;
    }

    OS_ASSERT(false);
    return 0.0;
  }

  double LightsDefinition_Impl::getPowerPerFloorArea(double floorArea, double numPeople) const
  {
    std::string method = designLevelCalculationMethod();

    if (method == "LightingLevel") {
      if (equal(floorArea,0.0)) {
        LOG_AND_THROW("Calculation would require division by zero.");
      }
      return lightingLevel().get() / floorArea;
    }
    else if (method == "Watts/Area") {
      return wattsperSpaceFloorArea().get();
    }
    else if (method == "Watts/Person") {
      if (equal(floorArea,0.0)) {
        LOG_AND_THROW("Calculation would require division by zero.");
      }
      return wattsperPerson().get() * numPeople / floorArea;
    }

    OS_ASSERT(false);
    return 0.0;
  }

  double LightsDefinition_Impl::getPowerPerPerson(double floorArea, double numPeople) const {
    std::string method = designLevelCalculationMethod();

    if (method == "LightingLevel") {
      if (equal(numPeople,0.0)) {
        LOG_AND_THROW("Calculation would require division by zero.");
      }
      return lightingLevel().get() / numPeople;
    }
    else if (method == "Watts/Area") {
      if (equal(numPeople,0.0)) {
        LOG_AND_THROW("Calculation would require division by zero.");
      }
      return wattsperSpaceFloorArea().get() * floorArea / numPeople;
    }
    else if (method == "Watts/Person") {
      return wattsperPerson().get();
    }

    OS_ASSERT(false);
    return 0.0;
  }

  bool LightsDefinition_Impl::setDesignLevelCalculationMethod(const std::string& method,
                                                              double floorArea,
                                                              double numPeople)
  {
    std::string wmethod(method);
    boost::to_lower(wmethod);

    if (wmethod == "lightinglevel") {
      return setLightingLevel(getLightingPower(floorArea,numPeople));
    }
    else if (wmethod == "watts/area") {
      return setWattsperSpaceFloorArea(getPowerPerFloorArea(floorArea,numPeople));
    }
    else if (wmethod == "watts/person") {
      return setWattsperPerson(getPowerPerPerson(floorArea,numPeople));
    }

    return false;
  }

} // detail

LightsDefinition::LightsDefinition(const Model& model)
  : SpaceLoadDefinition(LightsDefinition::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::LightsDefinition_Impl>());
  OS_ASSERT(this->setLightingLevel(0.0));
}

IddObjectType LightsDefinition::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Lights_Definition);
  return result;
}

std::vector<std::string> LightsDefinition::validDesignLevelCalculationMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Lights_DefinitionFields::DesignLevelCalculationMethod);
}

std::string LightsDefinition::designLevelCalculationMethod() const {
  return getImpl<detail::LightsDefinition_Impl>()->designLevelCalculationMethod();
}

boost::optional<double> LightsDefinition::lightingLevel() const {
  return getImpl<detail::LightsDefinition_Impl>()->lightingLevel();
}

boost::optional<double> LightsDefinition::wattsperSpaceFloorArea() const {
  return getImpl<detail::LightsDefinition_Impl>()->wattsperSpaceFloorArea();
}

boost::optional<double> LightsDefinition::wattsperPerson() const {
  return getImpl<detail::LightsDefinition_Impl>()->wattsperPerson();
}

double LightsDefinition::fractionRadiant() const {
  return getImpl<detail::LightsDefinition_Impl>()->fractionRadiant();
}

bool LightsDefinition::isFractionRadiantDefaulted() const {
  return getImpl<detail::LightsDefinition_Impl>()->isFractionRadiantDefaulted();
}

double LightsDefinition::fractionVisible() const {
  return getImpl<detail::LightsDefinition_Impl>()->fractionVisible();
}

bool LightsDefinition::isFractionVisibleDefaulted() const {
  return getImpl<detail::LightsDefinition_Impl>()->isFractionVisibleDefaulted();
}

double LightsDefinition::returnAirFraction() const {
  return getImpl<detail::LightsDefinition_Impl>()->returnAirFraction();
}

bool LightsDefinition::isReturnAirFractionDefaulted() const {
  return getImpl<detail::LightsDefinition_Impl>()->isReturnAirFractionDefaulted();
}

bool LightsDefinition::returnAirFractionCalculatedfromPlenumTemperature() const {
  return getImpl<detail::LightsDefinition_Impl>()->returnAirFractionCalculatedfromPlenumTemperature();
}

bool LightsDefinition::isReturnAirFractionCalculatedfromPlenumTemperatureDefaulted() const {
  return getImpl<detail::LightsDefinition_Impl>()->isReturnAirFractionCalculatedfromPlenumTemperatureDefaulted();
}

double LightsDefinition::returnAirFractionFunctionofPlenumTemperatureCoefficient1() const {
  return getImpl<detail::LightsDefinition_Impl>()->returnAirFractionFunctionofPlenumTemperatureCoefficient1();
}

bool LightsDefinition::isReturnAirFractionFunctionofPlenumTemperatureCoefficient1Defaulted() const {
  return getImpl<detail::LightsDefinition_Impl>()->isReturnAirFractionFunctionofPlenumTemperatureCoefficient1Defaulted();
}

double LightsDefinition::returnAirFractionFunctionofPlenumTemperatureCoefficient2() const {
  return getImpl<detail::LightsDefinition_Impl>()->returnAirFractionFunctionofPlenumTemperatureCoefficient2();
}

bool LightsDefinition::isReturnAirFractionFunctionofPlenumTemperatureCoefficient2Defaulted() const {
  return getImpl<detail::LightsDefinition_Impl>()->isReturnAirFractionFunctionofPlenumTemperatureCoefficient2Defaulted();
}

bool LightsDefinition::setLightingLevel(double lightingLevel) {
  return getImpl<detail::LightsDefinition_Impl>()->setLightingLevel(lightingLevel);
}

bool LightsDefinition::setWattsperSpaceFloorArea(double wattsperSpaceFloorArea) {
  return getImpl<detail::LightsDefinition_Impl>()->setWattsperSpaceFloorArea(wattsperSpaceFloorArea);
}

bool LightsDefinition::setWattsperPerson(double wattsperPerson) {
  return getImpl<detail::LightsDefinition_Impl>()->setWattsperPerson(wattsperPerson);
}

bool LightsDefinition::setFractionRadiant(double fractionRadiant) {
  return getImpl<detail::LightsDefinition_Impl>()->setFractionRadiant(fractionRadiant);
}

void LightsDefinition::resetFractionRadiant() {
  getImpl<detail::LightsDefinition_Impl>()->resetFractionRadiant();
}

bool LightsDefinition::setFractionVisible(double fractionVisible) {
  return getImpl<detail::LightsDefinition_Impl>()->setFractionVisible(fractionVisible);
}

void LightsDefinition::resetFractionVisible() {
  getImpl<detail::LightsDefinition_Impl>()->resetFractionVisible();
}

bool LightsDefinition::setReturnAirFraction(double returnAirFraction) {
  return getImpl<detail::LightsDefinition_Impl>()->setReturnAirFraction(returnAirFraction);
}

void LightsDefinition::resetReturnAirFraction() {
  getImpl<detail::LightsDefinition_Impl>()->resetReturnAirFraction();
}

void LightsDefinition::setReturnAirFractionCalculatedfromPlenumTemperature(bool returnAirFractionCalculatedfromPlenumTemperature) {
  getImpl<detail::LightsDefinition_Impl>()->setReturnAirFractionCalculatedfromPlenumTemperature(returnAirFractionCalculatedfromPlenumTemperature);
}

void LightsDefinition::resetReturnAirFractionCalculatedfromPlenumTemperature() {
  getImpl<detail::LightsDefinition_Impl>()->resetReturnAirFractionCalculatedfromPlenumTemperature();
}

bool LightsDefinition::setReturnAirFractionFunctionofPlenumTemperatureCoefficient1(double returnAirFractionFunctionofPlenumTemperatureCoefficient1) {
  return getImpl<detail::LightsDefinition_Impl>()->setReturnAirFractionFunctionofPlenumTemperatureCoefficient1(returnAirFractionFunctionofPlenumTemperatureCoefficient1);
}

void LightsDefinition::resetReturnAirFractionFunctionofPlenumTemperatureCoefficient1() {
  getImpl<detail::LightsDefinition_Impl>()->resetReturnAirFractionFunctionofPlenumTemperatureCoefficient1();
}

bool LightsDefinition::setReturnAirFractionFunctionofPlenumTemperatureCoefficient2(double returnAirFractionFunctionofPlenumTemperatureCoefficient2) {
  return getImpl<detail::LightsDefinition_Impl>()->setReturnAirFractionFunctionofPlenumTemperatureCoefficient2(returnAirFractionFunctionofPlenumTemperatureCoefficient2);
}

void LightsDefinition::resetReturnAirFractionFunctionofPlenumTemperatureCoefficient2() {
  getImpl<detail::LightsDefinition_Impl>()->resetReturnAirFractionFunctionofPlenumTemperatureCoefficient2();
}

double LightsDefinition::getLightingPower(double floorArea, double numPeople) const {
  return getImpl<detail::LightsDefinition_Impl>()->getLightingPower(floorArea,numPeople);
}

double LightsDefinition::getPowerPerFloorArea(double floorArea, double numPeople) const {
  return getImpl<detail::LightsDefinition_Impl>()->getPowerPerFloorArea(floorArea,numPeople);
}

double LightsDefinition::getPowerPerPerson(double floorArea, double numPeople) const {
  return getImpl<detail::LightsDefinition_Impl>()->getPowerPerPerson(floorArea,numPeople);
}

bool LightsDefinition::setDesignLevelCalculationMethod(const std::string& method,
                                     double floorArea,
                                     double numPeople)
{
  return getImpl<detail::LightsDefinition_Impl>()->setDesignLevelCalculationMethod(method,floorArea,numPeople);
}

/// @cond
LightsDefinition::LightsDefinition(std::shared_ptr<detail::LightsDefinition_Impl> impl)
  : SpaceLoadDefinition(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

