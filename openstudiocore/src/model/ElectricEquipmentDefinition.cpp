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

#include "ElectricEquipmentDefinition.hpp"
#include "ElectricEquipmentDefinition_Impl.hpp"
#include "attributes.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_ElectricEquipment_Definition_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/math/FloatCompare.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ATTRIBUTE_IMPLEMENTATION(1,0,0,designLevel,DesignLevel,
                           ElectricEquipmentDefinition,0,OS_ElectricEquipment_Definition,DesignLevel)

  ATTRIBUTE_IMPLEMENTATION(1,0,0,wattsperSpaceFloorArea,WattsperSpaceFloorArea,
                           ElectricEquipmentDefinition,0,OS_ElectricEquipment_Definition,WattsperSpaceFloorArea)

  ATTRIBUTE_IMPLEMENTATION(1,0,0,wattsperPerson,WattsperPerson,
                           ElectricEquipmentDefinition,0,OS_ElectricEquipment_Definition,WattsperPerson)

  ATTRIBUTE_IMPLEMENTATION(0,1,0,fractionRadiant,FractionRadiant,
                           ElectricEquipmentDefinition,0,OS_ElectricEquipment_Definition,FractionRadiant)

  ATTRIBUTE_IMPLEMENTATION(0,1,0,fractionLatent,FractionLatent,
                           ElectricEquipmentDefinition,0,OS_ElectricEquipment_Definition,FractionLatent)

  ATTRIBUTE_IMPLEMENTATION(0,1,0,fractionLost,FractionLost,
                           ElectricEquipmentDefinition,0,OS_ElectricEquipment_Definition,FractionLost)

  ElectricEquipmentDefinition_Impl::ElectricEquipmentDefinition_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : SpaceLoadDefinition_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ElectricEquipmentDefinition::iddObjectType());
  }

  ElectricEquipmentDefinition_Impl::ElectricEquipmentDefinition_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                     Model_Impl* model,
                                                                     bool keepHandle)
    : SpaceLoadDefinition_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ElectricEquipmentDefinition::iddObjectType());
  }

  ElectricEquipmentDefinition_Impl::ElectricEquipmentDefinition_Impl(const ElectricEquipmentDefinition_Impl& other,
                                                                     Model_Impl* model,
                                                                     bool keepHandle)
    : SpaceLoadDefinition_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ElectricEquipmentDefinition_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ElectricEquipmentDefinition_Impl::iddObjectType() const {
    return ElectricEquipmentDefinition::iddObjectType();
  }

  std::string ElectricEquipmentDefinition_Impl::designLevelCalculationMethod() const {
    boost::optional<std::string> value = getString(OS_ElectricEquipment_DefinitionFields::DesignLevelCalculationMethod,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> ElectricEquipmentDefinition_Impl::designLevel() const {
    boost::optional<double> result;
    if (istringEqual("EquipmentLevel", this->designLevelCalculationMethod())){
      result = getDouble(OS_ElectricEquipment_DefinitionFields::DesignLevel,true);
      //OS_ASSERT(result);
    }
    return result;
  }

  boost::optional<double> ElectricEquipmentDefinition_Impl::wattsperSpaceFloorArea() const {
    boost::optional<double> result;
    if (istringEqual("Watts/Area", this->designLevelCalculationMethod())){
      result = getDouble(OS_ElectricEquipment_DefinitionFields::WattsperSpaceFloorArea,true);
      //OS_ASSERT(result);
    }
    return result;
  }

  boost::optional<double> ElectricEquipmentDefinition_Impl::wattsperPerson() const {
    boost::optional<double> result;
    if (istringEqual("Watts/Person", this->designLevelCalculationMethod())){
      result = getDouble(OS_ElectricEquipment_DefinitionFields::WattsperPerson,true);
      //OS_ASSERT(result);
    }
    return result;
  }

  double ElectricEquipmentDefinition_Impl::fractionLatent() const {
    boost::optional<double> value = getDouble(OS_ElectricEquipment_DefinitionFields::FractionLatent,true);
    OS_ASSERT(value);
    return value.get();
  }

  //bool ElectricEquipmentDefinition_Impl::isFractionLatentDefaulted() const {
  //  return isEmpty(OS_ElectricEquipment_DefinitionFields::FractionLatent);
  //}

  double ElectricEquipmentDefinition_Impl::fractionRadiant() const {
    boost::optional<double> value = getDouble(OS_ElectricEquipment_DefinitionFields::FractionRadiant,true);
    OS_ASSERT(value);
    return value.get();
  }

  //bool ElectricEquipmentDefinition_Impl::isFractionRadiantDefaulted() const {
  //  return isEmpty(OS_ElectricEquipment_DefinitionFields::FractionRadiant);
  //}

  double ElectricEquipmentDefinition_Impl::fractionLost() const {
    boost::optional<double> value = getDouble(OS_ElectricEquipment_DefinitionFields::FractionLost,true);
    OS_ASSERT(value);
    return value.get();
  }

  //bool ElectricEquipmentDefinition_Impl::isFractionLostDefaulted() const {
  //  return isEmpty(OS_ElectricEquipment_DefinitionFields::FractionLost);
  //}

  bool ElectricEquipmentDefinition_Impl::setDesignLevel(boost::optional<double> designLevel) {
    bool result = true;
    if (designLevel) {
      if (*designLevel < 0){
        result = false;
      }else{
        result = setString(OS_ElectricEquipment_DefinitionFields::DesignLevelCalculationMethod, "EquipmentLevel");
        OS_ASSERT(result);
        result = setDouble(OS_ElectricEquipment_DefinitionFields::DesignLevel, designLevel.get());
        OS_ASSERT(result);
        result = setString(OS_ElectricEquipment_DefinitionFields::WattsperSpaceFloorArea, "");
        OS_ASSERT(result);
        result = setString(OS_ElectricEquipment_DefinitionFields::WattsperPerson, "");
        OS_ASSERT(result);
      }
    } else {
      if (istringEqual("EquipmentLevel", this->designLevelCalculationMethod())){
        result = setDouble(OS_ElectricEquipment_DefinitionFields::DesignLevel, 0.0);
      }
    }
    return result;
  }

  bool ElectricEquipmentDefinition_Impl::setWattsperSpaceFloorArea(boost::optional<double> wattsperSpaceFloorArea) {
    bool result = true;
    if (wattsperSpaceFloorArea) {
      if (*wattsperSpaceFloorArea < 0){
        result = false;
      }else{
        result = setString(OS_ElectricEquipment_DefinitionFields::DesignLevelCalculationMethod, "Watts/Area");
        OS_ASSERT(result);
        result = setString(OS_ElectricEquipment_DefinitionFields::DesignLevel, "");
        OS_ASSERT(result);
        result = setDouble(OS_ElectricEquipment_DefinitionFields::WattsperSpaceFloorArea, wattsperSpaceFloorArea.get());
        OS_ASSERT(result);
        result = setString(OS_ElectricEquipment_DefinitionFields::WattsperPerson, "");
        OS_ASSERT(result);
      }
    } else {
      if (istringEqual("Watts/Area", this->designLevelCalculationMethod())){
        result = setDouble(OS_ElectricEquipment_DefinitionFields::WattsperSpaceFloorArea, 0.0);
      }
    }
    return result;
  }

  bool ElectricEquipmentDefinition_Impl::setWattsperPerson(boost::optional<double> wattsperPerson) {
    bool result = true;
    if (wattsperPerson) {
      if (*wattsperPerson < 0){
        result = false;
      }else{
        result = setString(OS_ElectricEquipment_DefinitionFields::DesignLevelCalculationMethod, "Watts/Person");
        OS_ASSERT(result);
        result = setString(OS_ElectricEquipment_DefinitionFields::DesignLevel, "");
        OS_ASSERT(result);
        result = setString(OS_ElectricEquipment_DefinitionFields::WattsperSpaceFloorArea, "");
        OS_ASSERT(result);
        result = setDouble(OS_ElectricEquipment_DefinitionFields::WattsperPerson, wattsperPerson.get());
        OS_ASSERT(result);
      }
    } else {
      if (istringEqual("Watts/Person", this->designLevelCalculationMethod())){
        result = setDouble(OS_ElectricEquipment_DefinitionFields::WattsperPerson, 0.0);
      }
    }
    return result;
  }

  bool ElectricEquipmentDefinition_Impl::setFractionLatent(double fractionLatent) {
    bool result = setDouble(OS_ElectricEquipment_DefinitionFields::FractionLatent, fractionLatent);
    return result;
  }

  //void ElectricEquipmentDefinition_Impl::resetFractionLatent() {
  //  bool result = setString(OS_ElectricEquipment_DefinitionFields::FractionLatent, "");
  //  OS_ASSERT(result);
  //}

  bool ElectricEquipmentDefinition_Impl::setFractionRadiant(double fractionRadiant) {
    bool result = setDouble(OS_ElectricEquipment_DefinitionFields::FractionRadiant, fractionRadiant);
    return result;
  }

  //void ElectricEquipmentDefinition_Impl::resetFractionRadiant() {
  //  bool result = setString(OS_ElectricEquipment_DefinitionFields::FractionRadiant, "");
  //  OS_ASSERT(result);
  //}

  bool ElectricEquipmentDefinition_Impl::setFractionLost(double fractionLost) {
    bool result = setDouble(OS_ElectricEquipment_DefinitionFields::FractionLost, fractionLost);
    return result;
  }

  //void ElectricEquipmentDefinition_Impl::resetFractionLost() {
  //  bool result = setString(OS_ElectricEquipment_DefinitionFields::FractionLost, "");
  //  OS_ASSERT(result);
  //}

  double ElectricEquipmentDefinition_Impl::getDesignLevel(double floorArea, double numPeople) const
  {
    std::string method = designLevelCalculationMethod();

    if (method == "EquipmentLevel") {
      return designLevel().get();
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

  double ElectricEquipmentDefinition_Impl::getPowerPerFloorArea(double floorArea,
                                                                     double numPeople) const
  {
    std::string method = designLevelCalculationMethod();

    if (method == "EquipmentLevel") {
      if (equal(floorArea,0.0)) {
        LOG_AND_THROW("Calculation would require division by zero.");
      }
      return designLevel().get() / floorArea;
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

  double ElectricEquipmentDefinition_Impl::getPowerPerPerson(double floorArea,
                                                             double numPeople) const
  {
    std::string method = designLevelCalculationMethod();

    if (method == "EquipmentLevel") {
      if (equal(numPeople,0.0)) {
        LOG_AND_THROW("Calculation would require division by zero.");
      }
      return designLevel().get() / numPeople;
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

  bool ElectricEquipmentDefinition_Impl::setDesignLevelCalculationMethod(const std::string& method,
                                                                         double floorArea,
                                                                         double numPeople)
  {
    std::string wmethod(method);
    boost::to_lower(wmethod);

    if (wmethod == "equipmentlevel") {
      return setDesignLevel(getDesignLevel(floorArea,numPeople));
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

ElectricEquipmentDefinition::ElectricEquipmentDefinition(const Model& model)
  : SpaceLoadDefinition(ElectricEquipmentDefinition::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ElectricEquipmentDefinition_Impl>());
  bool test = this->setDesignLevel(0.0);
  OS_ASSERT(test);
}

IddObjectType ElectricEquipmentDefinition::iddObjectType() {
  IddObjectType result(IddObjectType::OS_ElectricEquipment_Definition);
  return result;
}

std::vector<std::string> ElectricEquipmentDefinition::validDesignLevelCalculationMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ElectricEquipment_DefinitionFields::DesignLevelCalculationMethod);
}

std::string ElectricEquipmentDefinition::designLevelCalculationMethod() const {
  return getImpl<detail::ElectricEquipmentDefinition_Impl>()->designLevelCalculationMethod();
}

boost::optional<double> ElectricEquipmentDefinition::designLevel() const {
  return getImpl<detail::ElectricEquipmentDefinition_Impl>()->designLevel();
}

boost::optional<double> ElectricEquipmentDefinition::wattsperSpaceFloorArea() const {
  return getImpl<detail::ElectricEquipmentDefinition_Impl>()->wattsperSpaceFloorArea();
}

boost::optional<double> ElectricEquipmentDefinition::wattsperPerson() const {
  return getImpl<detail::ElectricEquipmentDefinition_Impl>()->wattsperPerson();
}

double ElectricEquipmentDefinition::fractionLatent() const {
  return getImpl<detail::ElectricEquipmentDefinition_Impl>()->fractionLatent();
}

bool ElectricEquipmentDefinition::isFractionLatentDefaulted() const {
  return getImpl<detail::ElectricEquipmentDefinition_Impl>()->isFractionLatentDefaulted();
}

double ElectricEquipmentDefinition::fractionRadiant() const {
  return getImpl<detail::ElectricEquipmentDefinition_Impl>()->fractionRadiant();
}

bool ElectricEquipmentDefinition::isFractionRadiantDefaulted() const {
  return getImpl<detail::ElectricEquipmentDefinition_Impl>()->isFractionRadiantDefaulted();
}

double ElectricEquipmentDefinition::fractionLost() const {
  return getImpl<detail::ElectricEquipmentDefinition_Impl>()->fractionLost();
}

bool ElectricEquipmentDefinition::isFractionLostDefaulted() const {
  return getImpl<detail::ElectricEquipmentDefinition_Impl>()->isFractionLostDefaulted();
}

bool ElectricEquipmentDefinition::setDesignLevel(double designLevel) {
  return getImpl<detail::ElectricEquipmentDefinition_Impl>()->setDesignLevel(designLevel);
}

bool ElectricEquipmentDefinition::setWattsperSpaceFloorArea(double wattsperSpaceFloorArea) {
  return getImpl<detail::ElectricEquipmentDefinition_Impl>()->setWattsperSpaceFloorArea(wattsperSpaceFloorArea);
}

bool ElectricEquipmentDefinition::setWattsperPerson(double wattsperPerson) {
  return getImpl<detail::ElectricEquipmentDefinition_Impl>()->setWattsperPerson(wattsperPerson);
}

bool ElectricEquipmentDefinition::setFractionLatent(double fractionLatent) {
  return getImpl<detail::ElectricEquipmentDefinition_Impl>()->setFractionLatent(fractionLatent);
}

void ElectricEquipmentDefinition::resetFractionLatent() {
  getImpl<detail::ElectricEquipmentDefinition_Impl>()->resetFractionLatent();
}

bool ElectricEquipmentDefinition::setFractionRadiant(double fractionRadiant) {
  return getImpl<detail::ElectricEquipmentDefinition_Impl>()->setFractionRadiant(fractionRadiant);
}

void ElectricEquipmentDefinition::resetFractionRadiant() {
  getImpl<detail::ElectricEquipmentDefinition_Impl>()->resetFractionRadiant();
}

bool ElectricEquipmentDefinition::setFractionLost(double fractionLost) {
  return getImpl<detail::ElectricEquipmentDefinition_Impl>()->setFractionLost(fractionLost);
}

void ElectricEquipmentDefinition::resetFractionLost() {
  getImpl<detail::ElectricEquipmentDefinition_Impl>()->resetFractionLost();
}

double ElectricEquipmentDefinition::getDesignLevel(double floorArea, double numPeople) const {
  return getImpl<detail::ElectricEquipmentDefinition_Impl>()->getDesignLevel(floorArea,numPeople);
}

double ElectricEquipmentDefinition::getPowerPerFloorArea(double floorArea, double numPeople) const {
  return getImpl<detail::ElectricEquipmentDefinition_Impl>()->getPowerPerFloorArea(floorArea,numPeople);
}

double ElectricEquipmentDefinition::getPowerPerPerson(double floorArea, double numPeople) const {
  return getImpl<detail::ElectricEquipmentDefinition_Impl>()->getPowerPerPerson(floorArea,numPeople);
}

bool ElectricEquipmentDefinition::setDesignLevelCalculationMethod(const std::string& method,
                                     double floorArea,
                                     double numPeople)
{
  return getImpl<detail::ElectricEquipmentDefinition_Impl>()->setDesignLevelCalculationMethod(method,floorArea,numPeople);
}

/// @cond
ElectricEquipmentDefinition::ElectricEquipmentDefinition(std::shared_ptr<detail::ElectricEquipmentDefinition_Impl> impl)
  : SpaceLoadDefinition(std::move(impl))
{}
/// @endcond


} // model
} // openstudio

