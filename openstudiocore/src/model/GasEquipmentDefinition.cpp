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

#include "GasEquipmentDefinition.hpp"
#include "GasEquipmentDefinition_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_GasEquipment_Definition_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/math/FloatCompare.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ATTRIBUTE_IMPLEMENTATION(1,0,0,designLevel,DesignLevel,
                           GasEquipmentDefinition,0,OS_GasEquipment_Definition,DesignLevel)

  ATTRIBUTE_IMPLEMENTATION(1,0,0,wattsperSpaceFloorArea,WattsperSpaceFloorArea,
                           GasEquipmentDefinition,0,OS_GasEquipment_Definition,WattsperSpaceFloorArea)

  ATTRIBUTE_IMPLEMENTATION(1,0,0,wattsperPerson,WattsperPerson,
                           GasEquipmentDefinition,0,OS_GasEquipment_Definition,WattsperPerson)

  ATTRIBUTE_IMPLEMENTATION(0,1,0,fractionRadiant,FractionRadiant,
                           GasEquipmentDefinition,0,OS_GasEquipment_Definition,FractionRadiant)

  ATTRIBUTE_IMPLEMENTATION(0,1,0,fractionLatent,FractionLatent,
                           GasEquipmentDefinition,0,OS_GasEquipment_Definition,FractionLatent)

  ATTRIBUTE_IMPLEMENTATION(0,1,0,fractionLost,FractionLost,
                           GasEquipmentDefinition,0,OS_GasEquipment_Definition,FractionLost)

  ATTRIBUTE_IMPLEMENTATION(0,1,0,carbonDioxideGenerationRate,CarbonDioxideGenerationRate,
                           GasEquipmentDefinition,0,OS_GasEquipment_Definition,CarbonDioxideGenerationRate)

  GasEquipmentDefinition_Impl::GasEquipmentDefinition_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : SpaceLoadDefinition_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == GasEquipmentDefinition::iddObjectType());
  }

  GasEquipmentDefinition_Impl::GasEquipmentDefinition_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                           Model_Impl* model,
                                                           bool keepHandle)
    : SpaceLoadDefinition_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == GasEquipmentDefinition::iddObjectType());
  }

  GasEquipmentDefinition_Impl::GasEquipmentDefinition_Impl(const GasEquipmentDefinition_Impl& other,
                                                           Model_Impl* model,
                                                           bool keepHandle)
    : SpaceLoadDefinition_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& GasEquipmentDefinition_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType GasEquipmentDefinition_Impl::iddObjectType() const {
    return GasEquipmentDefinition::iddObjectType();
  }

  std::string GasEquipmentDefinition_Impl::designLevelCalculationMethod() const {
    boost::optional<std::string> value = getString(OS_GasEquipment_DefinitionFields::DesignLevelCalculationMethod,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> GasEquipmentDefinition_Impl::designLevel() const {
    boost::optional<double> result;
    if (istringEqual("EquipmentLevel", this->designLevelCalculationMethod())){
      result = getDouble(OS_GasEquipment_DefinitionFields::DesignLevel,true);
      //OS_ASSERT(result);
    }
    return result;
  }

  boost::optional<double> GasEquipmentDefinition_Impl::wattsperSpaceFloorArea() const {
    boost::optional<double> result;
    if (istringEqual("Watts/Area", this->designLevelCalculationMethod())){
      result = getDouble(OS_GasEquipment_DefinitionFields::WattsperSpaceFloorArea,true);
      //OS_ASSERT(result);
    }
    return result;
  }

  boost::optional<double> GasEquipmentDefinition_Impl::wattsperPerson() const {
    boost::optional<double> result;
    if (istringEqual("Watts/Person", this->designLevelCalculationMethod())){
      result = getDouble(OS_GasEquipment_DefinitionFields::WattsperPerson,true);
      //OS_ASSERT(result);
    }
    return result;
  }

  double GasEquipmentDefinition_Impl::fractionLatent() const {
    boost::optional<double> value = getDouble(OS_GasEquipment_DefinitionFields::FractionLatent,true);
    OS_ASSERT(value);
    return value.get();
  }

  //bool GasEquipmentDefinition_Impl::isFractionLatentDefaulted() const {
  //  return isEmpty(OS_GasEquipment_DefinitionFields::FractionLatent);
  //}

  double GasEquipmentDefinition_Impl::fractionRadiant() const {
    boost::optional<double> value = getDouble(OS_GasEquipment_DefinitionFields::FractionRadiant,true);
    OS_ASSERT(value);
    return value.get();
  }

  //bool GasEquipmentDefinition_Impl::isFractionRadiantDefaulted() const {
  //  return isEmpty(OS_GasEquipment_DefinitionFields::FractionRadiant);
  //}

  double GasEquipmentDefinition_Impl::fractionLost() const {
    boost::optional<double> value = getDouble(OS_GasEquipment_DefinitionFields::FractionLost,true);
    OS_ASSERT(value);
    return value.get();
  }

  //bool GasEquipmentDefinition_Impl::isFractionLostDefaulted() const {
  //  return isEmpty(OS_GasEquipment_DefinitionFields::FractionLost);
  //}

  double GasEquipmentDefinition_Impl::carbonDioxideGenerationRate() const {
    boost::optional<double> value = getDouble(OS_GasEquipment_DefinitionFields::CarbonDioxideGenerationRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  //bool GasEquipmentDefinition_Impl::isCarbonDioxideGenerationRateDefaulted() const {
  //  return isEmpty(OS_GasEquipment_DefinitionFields::CarbonDioxideGenerationRate);
  //}

  bool GasEquipmentDefinition_Impl::setDesignLevel(boost::optional<double> designLevel) {
    bool result = true;
    if (designLevel) {
      if (*designLevel < 0){
        result = false;
      }else{
        result = setString(OS_GasEquipment_DefinitionFields::DesignLevelCalculationMethod, "EquipmentLevel");
        OS_ASSERT(result);
        result = setDouble(OS_GasEquipment_DefinitionFields::DesignLevel, designLevel.get());
        OS_ASSERT(result);
        result = setString(OS_GasEquipment_DefinitionFields::WattsperSpaceFloorArea, "");
        OS_ASSERT(result);
        result = setString(OS_GasEquipment_DefinitionFields::WattsperPerson, "");
        OS_ASSERT(result);
      }
    } else {
      if (istringEqual("EquipmentLevel", this->designLevelCalculationMethod())){
        result = setDouble(OS_GasEquipment_DefinitionFields::DesignLevel, 0.0);
      }
    }
    return result;
  }

  bool GasEquipmentDefinition_Impl::setWattsperSpaceFloorArea(boost::optional<double> wattsperSpaceFloorArea) {
    bool result = true;
    if (wattsperSpaceFloorArea) {
      if (*wattsperSpaceFloorArea < 0){
        result = false;
      }else{
        result = setString(OS_GasEquipment_DefinitionFields::DesignLevelCalculationMethod, "Watts/Area");
        OS_ASSERT(result);
        result = setString(OS_GasEquipment_DefinitionFields::DesignLevel, "");
        OS_ASSERT(result);
        result = setDouble(OS_GasEquipment_DefinitionFields::WattsperSpaceFloorArea, wattsperSpaceFloorArea.get());
        OS_ASSERT(result);
        result = setString(OS_GasEquipment_DefinitionFields::WattsperPerson, "");
        OS_ASSERT(result);
      }
    } else {
      if (istringEqual("Watts/Area", this->designLevelCalculationMethod())){
        result = setDouble(OS_GasEquipment_DefinitionFields::WattsperSpaceFloorArea, 0.0);
      }
    }
    return result;
  }

  bool GasEquipmentDefinition_Impl::setWattsperPerson(boost::optional<double> wattsperPerson) {
    bool result = true;
    if (wattsperPerson) {
      if (*wattsperPerson < 0){
        result = false;
      }else{
        result = setString(OS_GasEquipment_DefinitionFields::DesignLevelCalculationMethod, "Watts/Person");
        OS_ASSERT(result);
        result = setString(OS_GasEquipment_DefinitionFields::DesignLevel, "");
        OS_ASSERT(result);
        result = setString(OS_GasEquipment_DefinitionFields::WattsperSpaceFloorArea, "");
        OS_ASSERT(result);
        result = setDouble(OS_GasEquipment_DefinitionFields::WattsperPerson, wattsperPerson.get());
        OS_ASSERT(result);
      }
    } else {
      if (istringEqual("Watts/Person", this->designLevelCalculationMethod())){
        result = setDouble(OS_GasEquipment_DefinitionFields::WattsperPerson, 0.0);
      }
    }
    return result;
  }

  bool GasEquipmentDefinition_Impl::setFractionLatent(double fractionLatent) {
    bool result = setDouble(OS_GasEquipment_DefinitionFields::FractionLatent, fractionLatent);
    return result;
  }

  //void GasEquipmentDefinition_Impl::resetFractionLatent() {
  //  bool result = setString(OS_GasEquipment_DefinitionFields::FractionLatent, "");
  //  OS_ASSERT(result);
  //}

  bool GasEquipmentDefinition_Impl::setFractionRadiant(double fractionRadiant) {
    bool result = setDouble(OS_GasEquipment_DefinitionFields::FractionRadiant, fractionRadiant);
    return result;
  }

  //void GasEquipmentDefinition_Impl::resetFractionRadiant() {
  //  bool result = setString(OS_GasEquipment_DefinitionFields::FractionRadiant, "");
  //  OS_ASSERT(result);
  //}

  bool GasEquipmentDefinition_Impl::setFractionLost(double fractionLost) {
    bool result = setDouble(OS_GasEquipment_DefinitionFields::FractionLost, fractionLost);
    return result;
  }

  //void GasEquipmentDefinition_Impl::resetFractionLost() {
  //  bool result = setString(OS_GasEquipment_DefinitionFields::FractionLost, "");
  //  OS_ASSERT(result);
  //}

  bool GasEquipmentDefinition_Impl::setCarbonDioxideGenerationRate(double carbonDioxideGenerationRate) {
    bool result = setDouble(OS_GasEquipment_DefinitionFields::CarbonDioxideGenerationRate, carbonDioxideGenerationRate);
    return result;
  }

  //void GasEquipmentDefinition_Impl::resetCarbonDioxideGenerationRate() {
  //  bool result = setString(OS_GasEquipment_DefinitionFields::CarbonDioxideGenerationRate, "");
  //  OS_ASSERT(result);
  //}

  double GasEquipmentDefinition_Impl::getDesignLevel(double floorArea, double numPeople) const
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

  double GasEquipmentDefinition_Impl::getPowerPerFloorArea(double floorArea, 
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

  double GasEquipmentDefinition_Impl::getPowerPerPerson(double floorArea, 
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
 
  bool GasEquipmentDefinition_Impl::setDesignLevelCalculationMethod(const std::string& method,
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

GasEquipmentDefinition::GasEquipmentDefinition(const Model& model)
  : SpaceLoadDefinition(GasEquipmentDefinition::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::GasEquipmentDefinition_Impl>());
  bool test = this->setDesignLevel(0.0);
  OS_ASSERT(test);
}

IddObjectType GasEquipmentDefinition::iddObjectType() {
  IddObjectType result(IddObjectType::OS_GasEquipment_Definition);
  return result;
}

std::vector<std::string> GasEquipmentDefinition::validDesignLevelCalculationMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_GasEquipment_DefinitionFields::DesignLevelCalculationMethod);
}

std::string GasEquipmentDefinition::designLevelCalculationMethod() const {
  return getImpl<detail::GasEquipmentDefinition_Impl>()->designLevelCalculationMethod();
}

boost::optional<double> GasEquipmentDefinition::designLevel() const {
  return getImpl<detail::GasEquipmentDefinition_Impl>()->designLevel();
}

boost::optional<double> GasEquipmentDefinition::wattsperSpaceFloorArea() const {
  return getImpl<detail::GasEquipmentDefinition_Impl>()->wattsperSpaceFloorArea();
}

boost::optional<double> GasEquipmentDefinition::wattsperPerson() const {
  return getImpl<detail::GasEquipmentDefinition_Impl>()->wattsperPerson();
}

double GasEquipmentDefinition::fractionLatent() const {
  return getImpl<detail::GasEquipmentDefinition_Impl>()->fractionLatent();
}

bool GasEquipmentDefinition::isFractionLatentDefaulted() const {
  return getImpl<detail::GasEquipmentDefinition_Impl>()->isFractionLatentDefaulted();
}

double GasEquipmentDefinition::fractionRadiant() const {
  return getImpl<detail::GasEquipmentDefinition_Impl>()->fractionRadiant();
}

bool GasEquipmentDefinition::isFractionRadiantDefaulted() const {
  return getImpl<detail::GasEquipmentDefinition_Impl>()->isFractionRadiantDefaulted();
}

double GasEquipmentDefinition::fractionLost() const {
  return getImpl<detail::GasEquipmentDefinition_Impl>()->fractionLost();
}

bool GasEquipmentDefinition::isFractionLostDefaulted() const {
  return getImpl<detail::GasEquipmentDefinition_Impl>()->isFractionLostDefaulted();
}

double GasEquipmentDefinition::carbonDioxideGenerationRate() const {
  return getImpl<detail::GasEquipmentDefinition_Impl>()->carbonDioxideGenerationRate();
}

bool GasEquipmentDefinition::isCarbonDioxideGenerationRateDefaulted() const {
  return getImpl<detail::GasEquipmentDefinition_Impl>()->isCarbonDioxideGenerationRateDefaulted();
}

bool GasEquipmentDefinition::setDesignLevel(double designLevel) {
  return getImpl<detail::GasEquipmentDefinition_Impl>()->setDesignLevel(designLevel);
}

bool GasEquipmentDefinition::setWattsperSpaceFloorArea(double wattsperSpaceFloorArea) {
  return getImpl<detail::GasEquipmentDefinition_Impl>()->setWattsperSpaceFloorArea(wattsperSpaceFloorArea);
}

bool GasEquipmentDefinition::setWattsperPerson(double wattsperPerson) {
  return getImpl<detail::GasEquipmentDefinition_Impl>()->setWattsperPerson(wattsperPerson);
}

bool GasEquipmentDefinition::setFractionLatent(double fractionLatent) {
  return getImpl<detail::GasEquipmentDefinition_Impl>()->setFractionLatent(fractionLatent);
}

void GasEquipmentDefinition::resetFractionLatent() {
  getImpl<detail::GasEquipmentDefinition_Impl>()->resetFractionLatent();
}

bool GasEquipmentDefinition::setFractionRadiant(double fractionRadiant) {
  return getImpl<detail::GasEquipmentDefinition_Impl>()->setFractionRadiant(fractionRadiant);
}

void GasEquipmentDefinition::resetFractionRadiant() {
  getImpl<detail::GasEquipmentDefinition_Impl>()->resetFractionRadiant();
}

bool GasEquipmentDefinition::setFractionLost(double fractionLost) {
  return getImpl<detail::GasEquipmentDefinition_Impl>()->setFractionLost(fractionLost);
}

void GasEquipmentDefinition::resetFractionLost() {
  getImpl<detail::GasEquipmentDefinition_Impl>()->resetFractionLost();
}

bool GasEquipmentDefinition::setCarbonDioxideGenerationRate(double carbonDioxideGenerationRate) {
  return getImpl<detail::GasEquipmentDefinition_Impl>()->setCarbonDioxideGenerationRate(carbonDioxideGenerationRate);
}

void GasEquipmentDefinition::resetCarbonDioxideGenerationRate() {
  getImpl<detail::GasEquipmentDefinition_Impl>()->resetCarbonDioxideGenerationRate();
}

double GasEquipmentDefinition::getDesignLevel(double floorArea, double numPeople) const {
  return getImpl<detail::GasEquipmentDefinition_Impl>()->getDesignLevel(floorArea,numPeople);
}

double GasEquipmentDefinition::getPowerPerFloorArea(double floorArea, double numPeople) const {
  return getImpl<detail::GasEquipmentDefinition_Impl>()->getPowerPerFloorArea(floorArea,numPeople);
}

double GasEquipmentDefinition::getPowerPerPerson(double floorArea, double numPeople) const {
  return getImpl<detail::GasEquipmentDefinition_Impl>()->getPowerPerPerson(floorArea,numPeople);
}

bool GasEquipmentDefinition::setDesignLevelCalculationMethod(const std::string& method, 
                                     double floorArea, 
                                     double numPeople)
{
  return getImpl<detail::GasEquipmentDefinition_Impl>()->setDesignLevelCalculationMethod(method,floorArea,numPeople);
}

/// @cond
GasEquipmentDefinition::GasEquipmentDefinition(std::shared_ptr<detail::GasEquipmentDefinition_Impl> impl)
  : SpaceLoadDefinition(impl)
{}
/// @endcond


} // model
} // openstudio

