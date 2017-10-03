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

#include "HotWaterEquipmentDefinition.hpp"
#include "HotWaterEquipmentDefinition_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_HotWaterEquipment_Definition_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/math/FloatCompare.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  HotWaterEquipmentDefinition_Impl::HotWaterEquipmentDefinition_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : SpaceLoadDefinition_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == HotWaterEquipmentDefinition::iddObjectType());
  }

  HotWaterEquipmentDefinition_Impl::HotWaterEquipmentDefinition_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                     Model_Impl* model,
                                                                     bool keepHandle)
    : SpaceLoadDefinition_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == HotWaterEquipmentDefinition::iddObjectType());
  }

  HotWaterEquipmentDefinition_Impl::HotWaterEquipmentDefinition_Impl(const HotWaterEquipmentDefinition_Impl& other,
                                                                     Model_Impl* model,
                                                                     bool keepHandle)
    : SpaceLoadDefinition_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& HotWaterEquipmentDefinition_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType HotWaterEquipmentDefinition_Impl::iddObjectType() const {
    return HotWaterEquipmentDefinition::iddObjectType();
  }

  std::string HotWaterEquipmentDefinition_Impl::designLevelCalculationMethod() const {
    boost::optional<std::string> value = getString(OS_HotWaterEquipment_DefinitionFields::DesignLevelCalculationMethod,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> HotWaterEquipmentDefinition_Impl::designLevel() const {
    boost::optional<double> result;
    if (istringEqual("EquipmentLevel", this->designLevelCalculationMethod())){
      result = getDouble(OS_HotWaterEquipment_DefinitionFields::DesignLevel,true);
      OS_ASSERT(result);
    }
    return result;
  }

  boost::optional<double> HotWaterEquipmentDefinition_Impl::wattsperSpaceFloorArea() const {
    boost::optional<double> result;
    if (istringEqual("Watts/Area", this->designLevelCalculationMethod())){
      result = getDouble(OS_HotWaterEquipment_DefinitionFields::WattsperSpaceFloorArea,true);
      OS_ASSERT(result);
    }
    return result;
  }

  boost::optional<double> HotWaterEquipmentDefinition_Impl::wattsperPerson() const {
    boost::optional<double> result;
    if (istringEqual("Watts/Person", this->designLevelCalculationMethod())){
      result = getDouble(OS_HotWaterEquipment_DefinitionFields::WattsperPerson,true);
      OS_ASSERT(result);
    }
    return result;
  }

  double HotWaterEquipmentDefinition_Impl::fractionLatent() const {
    boost::optional<double> value = getDouble(OS_HotWaterEquipment_DefinitionFields::FractionLatent,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool HotWaterEquipmentDefinition_Impl::isFractionLatentDefaulted() const {
    return isEmpty(OS_HotWaterEquipment_DefinitionFields::FractionLatent);
  }

  double HotWaterEquipmentDefinition_Impl::fractionRadiant() const {
    boost::optional<double> value = getDouble(OS_HotWaterEquipment_DefinitionFields::FractionRadiant,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool HotWaterEquipmentDefinition_Impl::isFractionRadiantDefaulted() const {
    return isEmpty(OS_HotWaterEquipment_DefinitionFields::FractionRadiant);
  }

  double HotWaterEquipmentDefinition_Impl::fractionLost() const {
    boost::optional<double> value = getDouble(OS_HotWaterEquipment_DefinitionFields::FractionLost,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool HotWaterEquipmentDefinition_Impl::isFractionLostDefaulted() const {
    return isEmpty(OS_HotWaterEquipment_DefinitionFields::FractionLost);
  }

  bool HotWaterEquipmentDefinition_Impl::setDesignLevel(boost::optional<double> designLevel) {
    bool result = true;
    if (designLevel) {
      if (*designLevel < 0){
        result = false;
      }else{
        result = setString(OS_HotWaterEquipment_DefinitionFields::DesignLevelCalculationMethod, "EquipmentLevel");
        OS_ASSERT(result);
        result = setDouble(OS_HotWaterEquipment_DefinitionFields::DesignLevel, designLevel.get());
        OS_ASSERT(result);
        result = setString(OS_HotWaterEquipment_DefinitionFields::WattsperSpaceFloorArea, "");
        OS_ASSERT(result);
        result = setString(OS_HotWaterEquipment_DefinitionFields::WattsperPerson, "");
        OS_ASSERT(result);
      }
    } else {
      if (istringEqual("EquipmentLevel", this->designLevelCalculationMethod())){
        result = setDouble(OS_HotWaterEquipment_DefinitionFields::DesignLevel, 0.0);
      }
    }
    return result;
  }

  bool HotWaterEquipmentDefinition_Impl::setWattsperSpaceFloorArea(boost::optional<double> wattsperSpaceFloorArea) {
    bool result = true;
    if (wattsperSpaceFloorArea) {
      if (*wattsperSpaceFloorArea < 0){
        result = false;
      }else{
        result = setString(OS_HotWaterEquipment_DefinitionFields::DesignLevelCalculationMethod, "Watts/Area");
        OS_ASSERT(result);
        result = setString(OS_HotWaterEquipment_DefinitionFields::DesignLevel, "");
        OS_ASSERT(result);
        result = setDouble(OS_HotWaterEquipment_DefinitionFields::WattsperSpaceFloorArea, wattsperSpaceFloorArea.get());
        OS_ASSERT(result);
        result = setString(OS_HotWaterEquipment_DefinitionFields::WattsperPerson, "");
        OS_ASSERT(result);
      }
    } else {
      if (istringEqual("Watts/Area", this->designLevelCalculationMethod())){
        result = setDouble(OS_HotWaterEquipment_DefinitionFields::WattsperSpaceFloorArea, 0.0);
      }
    }
    return result;
  }

  bool HotWaterEquipmentDefinition_Impl::setWattsperPerson(boost::optional<double> wattsperPerson) {
    bool result = true;
    if (wattsperPerson) {
      if (*wattsperPerson < 0){
        result = false;
      }else{
        result = setString(OS_HotWaterEquipment_DefinitionFields::DesignLevelCalculationMethod, "Watts/Person");
        OS_ASSERT(result);
        result = setString(OS_HotWaterEquipment_DefinitionFields::DesignLevel, "");
        OS_ASSERT(result);
        result = setString(OS_HotWaterEquipment_DefinitionFields::WattsperSpaceFloorArea, "");
        OS_ASSERT(result);
        result = setDouble(OS_HotWaterEquipment_DefinitionFields::WattsperPerson, wattsperPerson.get());
        OS_ASSERT(result);
      }
    } else {
      if (istringEqual("Watts/Person", this->designLevelCalculationMethod())){
        result = setDouble(OS_HotWaterEquipment_DefinitionFields::WattsperPerson, 0.0);
      }
    }
    return result;
  }

  bool HotWaterEquipmentDefinition_Impl::setFractionLatent(double fractionLatent) {
    bool result = setDouble(OS_HotWaterEquipment_DefinitionFields::FractionLatent, fractionLatent);
    return result;
  }

  void HotWaterEquipmentDefinition_Impl::resetFractionLatent() {
    bool result = setString(OS_HotWaterEquipment_DefinitionFields::FractionLatent, "");
    OS_ASSERT(result);
  }

  bool HotWaterEquipmentDefinition_Impl::setFractionRadiant(double fractionRadiant) {
    bool result = setDouble(OS_HotWaterEquipment_DefinitionFields::FractionRadiant, fractionRadiant);
    return result;
  }

  void HotWaterEquipmentDefinition_Impl::resetFractionRadiant() {
    bool result = setString(OS_HotWaterEquipment_DefinitionFields::FractionRadiant, "");
    OS_ASSERT(result);
  }

  bool HotWaterEquipmentDefinition_Impl::setFractionLost(double fractionLost) {
    bool result = setDouble(OS_HotWaterEquipment_DefinitionFields::FractionLost, fractionLost);
    return result;
  }

  void HotWaterEquipmentDefinition_Impl::resetFractionLost() {
    bool result = setString(OS_HotWaterEquipment_DefinitionFields::FractionLost, "");
    OS_ASSERT(result);
  }

  double HotWaterEquipmentDefinition_Impl::getDesignLevel(double floorArea, double numPeople) const
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

  double HotWaterEquipmentDefinition_Impl::getPowerPerFloorArea(double floorArea,
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

  double HotWaterEquipmentDefinition_Impl::getPowerPerPerson(double floorArea,
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

  bool HotWaterEquipmentDefinition_Impl::setDesignLevelCalculationMethod(const std::string& method,
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

HotWaterEquipmentDefinition::HotWaterEquipmentDefinition(const Model& model)
  : SpaceLoadDefinition(HotWaterEquipmentDefinition::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::HotWaterEquipmentDefinition_Impl>());
  bool test = this->setDesignLevel(0.0);
  OS_ASSERT(test);
}

IddObjectType HotWaterEquipmentDefinition::iddObjectType() {
  IddObjectType result(IddObjectType::OS_HotWaterEquipment_Definition);
  return result;
}

std::vector<std::string> HotWaterEquipmentDefinition::validDesignLevelCalculationMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_HotWaterEquipment_DefinitionFields::DesignLevelCalculationMethod);
}

std::string HotWaterEquipmentDefinition::designLevelCalculationMethod() const {
  return getImpl<detail::HotWaterEquipmentDefinition_Impl>()->designLevelCalculationMethod();
}

boost::optional<double> HotWaterEquipmentDefinition::designLevel() const {
  return getImpl<detail::HotWaterEquipmentDefinition_Impl>()->designLevel();
}

boost::optional<double> HotWaterEquipmentDefinition::wattsperSpaceFloorArea() const {
  return getImpl<detail::HotWaterEquipmentDefinition_Impl>()->wattsperSpaceFloorArea();
}

boost::optional<double> HotWaterEquipmentDefinition::wattsperPerson() const {
  return getImpl<detail::HotWaterEquipmentDefinition_Impl>()->wattsperPerson();
}

double HotWaterEquipmentDefinition::fractionLatent() const {
  return getImpl<detail::HotWaterEquipmentDefinition_Impl>()->fractionLatent();
}

bool HotWaterEquipmentDefinition::isFractionLatentDefaulted() const {
  return getImpl<detail::HotWaterEquipmentDefinition_Impl>()->isFractionLatentDefaulted();
}

double HotWaterEquipmentDefinition::fractionRadiant() const {
  return getImpl<detail::HotWaterEquipmentDefinition_Impl>()->fractionRadiant();
}

bool HotWaterEquipmentDefinition::isFractionRadiantDefaulted() const {
  return getImpl<detail::HotWaterEquipmentDefinition_Impl>()->isFractionRadiantDefaulted();
}

double HotWaterEquipmentDefinition::fractionLost() const {
  return getImpl<detail::HotWaterEquipmentDefinition_Impl>()->fractionLost();
}

bool HotWaterEquipmentDefinition::isFractionLostDefaulted() const {
  return getImpl<detail::HotWaterEquipmentDefinition_Impl>()->isFractionLostDefaulted();
}

bool HotWaterEquipmentDefinition::setDesignLevel(double designLevel) {
  return getImpl<detail::HotWaterEquipmentDefinition_Impl>()->setDesignLevel(designLevel);
}

bool HotWaterEquipmentDefinition::setWattsperSpaceFloorArea(double wattsperSpaceFloorArea) {
  return getImpl<detail::HotWaterEquipmentDefinition_Impl>()->setWattsperSpaceFloorArea(wattsperSpaceFloorArea);
}

bool HotWaterEquipmentDefinition::setWattsperPerson(double wattsperPerson) {
  return getImpl<detail::HotWaterEquipmentDefinition_Impl>()->setWattsperPerson(wattsperPerson);
}

bool HotWaterEquipmentDefinition::setFractionLatent(double fractionLatent) {
  return getImpl<detail::HotWaterEquipmentDefinition_Impl>()->setFractionLatent(fractionLatent);
}

void HotWaterEquipmentDefinition::resetFractionLatent() {
  getImpl<detail::HotWaterEquipmentDefinition_Impl>()->resetFractionLatent();
}

bool HotWaterEquipmentDefinition::setFractionRadiant(double fractionRadiant) {
  return getImpl<detail::HotWaterEquipmentDefinition_Impl>()->setFractionRadiant(fractionRadiant);
}

void HotWaterEquipmentDefinition::resetFractionRadiant() {
  getImpl<detail::HotWaterEquipmentDefinition_Impl>()->resetFractionRadiant();
}

bool HotWaterEquipmentDefinition::setFractionLost(double fractionLost) {
  return getImpl<detail::HotWaterEquipmentDefinition_Impl>()->setFractionLost(fractionLost);
}

void HotWaterEquipmentDefinition::resetFractionLost() {
  getImpl<detail::HotWaterEquipmentDefinition_Impl>()->resetFractionLost();
}

double HotWaterEquipmentDefinition::getDesignLevel(double floorArea, double numPeople) const {
  return getImpl<detail::HotWaterEquipmentDefinition_Impl>()->getDesignLevel(floorArea,numPeople);
}

double HotWaterEquipmentDefinition::getPowerPerFloorArea(double floorArea, double numPeople) const {
  return getImpl<detail::HotWaterEquipmentDefinition_Impl>()->getPowerPerFloorArea(floorArea,numPeople);
}

double HotWaterEquipmentDefinition::getPowerPerPerson(double floorArea, double numPeople) const {
  return getImpl<detail::HotWaterEquipmentDefinition_Impl>()->getPowerPerPerson(floorArea,numPeople);
}

bool HotWaterEquipmentDefinition::setDesignLevelCalculationMethod(const std::string& method,
                                     double floorArea,
                                     double numPeople)
{
  return getImpl<detail::HotWaterEquipmentDefinition_Impl>()->setDesignLevelCalculationMethod(method,floorArea,numPeople);
}

/// @cond
HotWaterEquipmentDefinition::HotWaterEquipmentDefinition(std::shared_ptr<detail::HotWaterEquipmentDefinition_Impl> impl)
  : SpaceLoadDefinition(std::move(impl))
{}
/// @endcond


} // model
} // openstudio

