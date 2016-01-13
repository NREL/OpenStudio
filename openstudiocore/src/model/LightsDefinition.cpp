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

  const std::vector<std::string>& LightsDefinition_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
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
        result = setString(OS_Lights_DefinitionFields::DesignLevelCalculationMethod, "LightingLevel");
        OS_ASSERT(result);
        result = setDouble(OS_Lights_DefinitionFields::LightingLevel, lightingLevel.get());
        OS_ASSERT(result);
        result = setString(OS_Lights_DefinitionFields::WattsperSpaceFloorArea, "");
        OS_ASSERT(result);
        result = setString(OS_Lights_DefinitionFields::WattsperPerson, "");
        OS_ASSERT(result);
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
        result = setString(OS_Lights_DefinitionFields::DesignLevelCalculationMethod, "Watts/Area");
        OS_ASSERT(result);
        result = setString(OS_Lights_DefinitionFields::LightingLevel, "");
        OS_ASSERT(result);
        result = setDouble(OS_Lights_DefinitionFields::WattsperSpaceFloorArea, wattsperSpaceFloorArea.get());
        OS_ASSERT(result);
        result = setString(OS_Lights_DefinitionFields::WattsperPerson, "");
        OS_ASSERT(result);
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
        result = setString(OS_Lights_DefinitionFields::DesignLevelCalculationMethod, "Watts/Person");
        OS_ASSERT(result);
        result = setString(OS_Lights_DefinitionFields::LightingLevel, "");
        OS_ASSERT(result);
        result = setString(OS_Lights_DefinitionFields::WattsperSpaceFloorArea, "");
        OS_ASSERT(result);
        result = setDouble(OS_Lights_DefinitionFields::WattsperPerson, wattsperPerson.get());
        OS_ASSERT(result);
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
  : SpaceLoadDefinition(impl)
{}
/// @endcond

} // model
} // openstudio

