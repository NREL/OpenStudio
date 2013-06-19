/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <model/PeopleDefinition.hpp>
#include <model/PeopleDefinition_Impl.hpp>

#include <utilities/idf/IdfExtensibleGroup.hpp>

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_People_Definition_FieldEnums.hxx>

#include <utilities/math/FloatCompare.hpp>
#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model {

namespace detail {

  PeopleDefinition_Impl::PeopleDefinition_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : SpaceLoadDefinition_Impl(idfObject,model,keepHandle)
  {
    BOOST_ASSERT(idfObject.iddObject().type() == PeopleDefinition::iddObjectType());
  }

  PeopleDefinition_Impl::PeopleDefinition_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                               Model_Impl* model,
                                               bool keepHandle)
    : SpaceLoadDefinition_Impl(other,model,keepHandle)
  {
    BOOST_ASSERT(other.iddObject().type() == PeopleDefinition::iddObjectType());
  }

  PeopleDefinition_Impl::PeopleDefinition_Impl(const PeopleDefinition_Impl& other,
                                               Model_Impl* model,
                                               bool keepHandle)
    : SpaceLoadDefinition_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& PeopleDefinition_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType PeopleDefinition_Impl::iddObjectType() const {
    return PeopleDefinition::iddObjectType();
  }

  std::string PeopleDefinition_Impl::numberofPeopleCalculationMethod() const {
    boost::optional<std::string> value = getString(OS_People_DefinitionFields::NumberofPeopleCalculationMethod,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  boost::optional<double> PeopleDefinition_Impl::numberofPeople() const {
    OSOptionalQuantity q = getNumberofPeople(false);
    OptionalDouble result;
    if (q.isSet()) {
      result = q.get().value();
    }
    return result;
  }
  
  OSOptionalQuantity PeopleDefinition_Impl::getNumberofPeople(bool returnIP) const {
    OSOptionalQuantity value = getQuantity(OS_People_DefinitionFields::NumberofPeople,true,returnIP);
    if (istringEqual("People", this->numberofPeopleCalculationMethod())) {
      BOOST_ASSERT(value.isSet());
    }
    return value;
  }  

  boost::optional<double> PeopleDefinition_Impl::peopleperSpaceFloorArea() const {
    OSOptionalQuantity q = getPeopleperSpaceFloorArea(false);
    OptionalDouble result;
    if (q.isSet()) {
      result = q.get().value();
    }
    return result;
  }
  
  OSOptionalQuantity PeopleDefinition_Impl::getPeopleperSpaceFloorArea(bool returnIP) const {
    OSOptionalQuantity value = getQuantity(OS_People_DefinitionFields::PeopleperSpaceFloorArea,true,returnIP);
    if (istringEqual("People/Area", this->numberofPeopleCalculationMethod())) {    
      BOOST_ASSERT(value.isSet());
    }
    return value;
  }  

  boost::optional<double> PeopleDefinition_Impl::spaceFloorAreaperPerson() const {
    OSOptionalQuantity q = getSpaceFloorAreaperPerson(false);
    OptionalDouble result;
    if (q.isSet()) {
      result = q.get().value();
    }
    return result;
  }
  
  OSOptionalQuantity PeopleDefinition_Impl::getSpaceFloorAreaperPerson(bool returnIP) const {
    OSOptionalQuantity value = getQuantity(OS_People_DefinitionFields::SpaceFloorAreaperPerson,true,returnIP);
    if (istringEqual("Area/Person", this->numberofPeopleCalculationMethod())) {
      BOOST_ASSERT(value.isSet());
    }
    return value;
  }  

  double PeopleDefinition_Impl::fractionRadiant() const {
    Quantity q = getFractionRadiant(false);
    return q.value();
  }

  Quantity PeopleDefinition_Impl::getFractionRadiant(bool returnIP) const {
    OSOptionalQuantity value = getQuantity(OS_People_DefinitionFields::FractionRadiant,true,returnIP);
    BOOST_ASSERT(value.isSet());
    return value.get();
  }

  boost::optional<double> PeopleDefinition_Impl::sensibleHeatFraction() const {
    OSOptionalQuantity q = getSensibleHeatFraction(false);
    OptionalDouble result;
    if (q.isSet()) {
      result = q.get().value();
    }
    return result;
  }

  OSOptionalQuantity PeopleDefinition_Impl::getSensibleHeatFraction(bool returnIP) const {
    OSOptionalQuantity value = getQuantity(OS_People_DefinitionFields::SensibleHeatFraction,true,returnIP);
    return value;
  }

  bool PeopleDefinition_Impl::isSensibleHeatFractionDefaulted() const {
    return isEmpty(OS_People_DefinitionFields::SensibleHeatFraction);
  }

  bool PeopleDefinition_Impl::isSensibleHeatFractionAutocalculated() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_People_DefinitionFields::SensibleHeatFraction, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "canAutocalculate");
    }
    return result;
  }

  double PeopleDefinition_Impl::carbonDioxideGenerationRate() const {
    Quantity q = getCarbonDioxideGenerationRate(false);
    return q.value();
  }

  Quantity PeopleDefinition_Impl::getCarbonDioxideGenerationRate(bool returnIP) const {
    OSOptionalQuantity value = getQuantity(OS_People_DefinitionFields::CarbonDioxideGenerationRate,true,returnIP);
    BOOST_ASSERT(value.isSet());
    return value.get();
  }

  bool PeopleDefinition_Impl::isCarbonDioxideGenerationRateDefaulted() const {
    return isEmpty(OS_People_DefinitionFields::CarbonDioxideGenerationRate);
  }

  bool PeopleDefinition_Impl::enableASHRAE55ComfortWarnings() const {
    boost::optional<std::string> value = getString(OS_People_DefinitionFields::EnableASHRAE55ComfortWarnings,true);
    BOOST_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }

  bool PeopleDefinition_Impl::isEnableASHRAE55ComfortWarningsDefaulted() const {
    return isEmpty(OS_People_DefinitionFields::EnableASHRAE55ComfortWarnings);
  }

  std::string PeopleDefinition_Impl::meanRadiantTemperatureCalculationType() const {
    boost::optional<std::string> value = getString(OS_People_DefinitionFields::MeanRadiantTemperatureCalculationType,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool PeopleDefinition_Impl::isMeanRadiantTemperatureCalculationTypeDefaulted() const {
    return isEmpty(OS_People_DefinitionFields::MeanRadiantTemperatureCalculationType);
  }

  boost::optional<std::string> PeopleDefinition_Impl::getThermalComfortModelType(int i) const {
    OptionalString result;
    if (i < numThermalComfortModelTypes()) {
      IdfExtensibleGroup eg = getExtensibleGroup(i);
      BOOST_ASSERT(!eg.empty());
      result = eg.getString(OS_People_DefinitionExtensibleFields::ThermalComfortModelType,true);
    }
    return result;
  }

  int PeopleDefinition_Impl::numThermalComfortModelTypes() const {
    return numExtensibleGroups();
  }

  bool PeopleDefinition_Impl::setNumberofPeople(boost::optional<double> numberofPeople) {
    OSOptionalQuantity temp = getNumberofPeople();
    if (numberofPeople) {
      temp = OSOptionalQuantity(Quantity(numberofPeople.get(),temp.units()));
    }
    else {
      temp = OSOptionalQuantity(temp.units());
    }
    bool result = setNumberofPeople(temp);
    return result;
  }
  
  bool PeopleDefinition_Impl::setNumberofPeople(const OSOptionalQuantity& numberofPeople) {
    bool result = false;
    if (numberofPeople.isSet()) {
      result = setQuantity(OS_People_DefinitionFields::NumberofPeople,numberofPeople.get());
      if (result) {
        result = setString(OS_People_DefinitionFields::NumberofPeopleCalculationMethod, "People");
        BOOST_ASSERT(result);
        result = setString(OS_People_DefinitionFields::PeopleperSpaceFloorArea, "");
        BOOST_ASSERT(result);
        result = setString(OS_People_DefinitionFields::SpaceFloorAreaperPerson, "");
        BOOST_ASSERT(result);                
      }
    } else {
      if (istringEqual("People", this->numberofPeopleCalculationMethod())){
        result = setDouble(OS_People_DefinitionFields::NumberofPeople, 0.0);
      }      
    }
    return result;
  }  

  bool PeopleDefinition_Impl::setPeopleperSpaceFloorArea(boost::optional<double> peopleperSpaceFloorArea) {
    OSOptionalQuantity temp = getPeopleperSpaceFloorArea();
    if (peopleperSpaceFloorArea) {
      temp = OSOptionalQuantity(Quantity(peopleperSpaceFloorArea.get(),temp.units()));
    }
    else {
      temp = OSOptionalQuantity(temp.units());
    }
    bool result = setPeopleperSpaceFloorArea(temp);
    return result;
  }
  
  bool PeopleDefinition_Impl::setPeopleperSpaceFloorArea(const OSOptionalQuantity& peopleperSpaceFloorArea) {
    bool result = false;
    if (peopleperSpaceFloorArea.isSet()) {
      result = setQuantity(OS_People_DefinitionFields::PeopleperSpaceFloorArea,peopleperSpaceFloorArea.get());
      if (result) {
        result = setString(OS_People_DefinitionFields::NumberofPeopleCalculationMethod, "People/Area");
        BOOST_ASSERT(result);
        result = setString(OS_People_DefinitionFields::NumberofPeople, "");
        BOOST_ASSERT(result);
        result = setString(OS_People_DefinitionFields::SpaceFloorAreaperPerson, "");
        BOOST_ASSERT(result);                
      }      
    } else {
      if (istringEqual("People/Area", this->numberofPeopleCalculationMethod())){
        result = setDouble(OS_People_DefinitionFields::PeopleperSpaceFloorArea, 0.0);
      }
    }
    return result;
  }  

  bool PeopleDefinition_Impl::setSpaceFloorAreaperPerson(boost::optional<double> spaceFloorAreaperPerson) {
    OSOptionalQuantity temp = getSpaceFloorAreaperPerson();
    if (spaceFloorAreaperPerson) {
      temp = OSOptionalQuantity(Quantity(spaceFloorAreaperPerson.get(),temp.units()));
    }
    else {
      temp = OSOptionalQuantity(temp.units());
    }
    bool result = setSpaceFloorAreaperPerson(temp);
    return result;
  }
  
  bool PeopleDefinition_Impl::setSpaceFloorAreaperPerson(const OSOptionalQuantity& spaceFloorAreaperPerson) {
    bool result(false);
    if (spaceFloorAreaperPerson.isSet()) {
      result = setQuantity(OS_People_DefinitionFields::SpaceFloorAreaperPerson,spaceFloorAreaperPerson.get());
      if (result) {
        result = setString(OS_People_DefinitionFields::NumberofPeopleCalculationMethod, "Area/Person");
        BOOST_ASSERT(result);
        result = setString(OS_People_DefinitionFields::NumberofPeople, "");
        BOOST_ASSERT(result);
        result = setString(OS_People_DefinitionFields::PeopleperSpaceFloorArea, "");
        BOOST_ASSERT(result);      
      }
    } else {
      if (istringEqual("Area/Person", this->numberofPeopleCalculationMethod())){
        result = setDouble(OS_People_DefinitionFields::SpaceFloorAreaperPerson, 0.0);
      }
    }
    return result;
  }  

  bool PeopleDefinition_Impl::setFractionRadiant(double fractionRadiant) {
    OSOptionalQuantity temp = getQuantity(OS_People_DefinitionFields::FractionRadiant,true,false);
    bool result = setFractionRadiant(Quantity(fractionRadiant,temp.units()));
    return result;
  }

  bool PeopleDefinition_Impl::setFractionRadiant(const Quantity& fractionRadiant) {
    return setQuantity(OS_People_DefinitionFields::FractionRadiant,fractionRadiant);
  }

  bool PeopleDefinition_Impl::setSensibleHeatFraction(boost::optional<double> sensibleHeatFraction) {
    OSOptionalQuantity temp = getSensibleHeatFraction();
    if (sensibleHeatFraction) {
      temp = OSOptionalQuantity(Quantity(sensibleHeatFraction.get(),temp.units()));
    }
    else {
      temp = OSOptionalQuantity(temp.units());
    }
    bool result = setSensibleHeatFraction(temp);
    return result;
  }

  bool PeopleDefinition_Impl::setSensibleHeatFraction(const OSOptionalQuantity& sensibleHeatFraction) {
    bool result;
    if (sensibleHeatFraction.isSet()) {
      result = setQuantity(OS_People_DefinitionFields::SensibleHeatFraction,sensibleHeatFraction.get());
    } else {
      result = setString(OS_People_DefinitionFields::SensibleHeatFraction, "");
    }
    return result;
  }

  bool PeopleDefinition_Impl::setSensibleHeatFraction(double sensibleHeatFraction) {
    return setSensibleHeatFraction(OptionalDouble(sensibleHeatFraction));
  }

  void PeopleDefinition_Impl::resetSensibleHeatFraction() {
    bool result = setString(OS_People_DefinitionFields::SensibleHeatFraction, "");
    BOOST_ASSERT(result);
  }

  void PeopleDefinition_Impl::autocalculateSensibleHeatFraction() {
    bool result = setString(OS_People_DefinitionFields::SensibleHeatFraction, "Autocalculate");
    BOOST_ASSERT(result);
  }

  bool PeopleDefinition_Impl::setCarbonDioxideGenerationRate(double carbonDioxideGenerationRate) {
    Quantity temp = getCarbonDioxideGenerationRate();
    bool result = setCarbonDioxideGenerationRate(Quantity(carbonDioxideGenerationRate,temp.units()));
    return result;
  }

  bool PeopleDefinition_Impl::setCarbonDioxideGenerationRate(const Quantity& carbonDioxideGenerationRate) {
    return setQuantity(OS_People_DefinitionFields::CarbonDioxideGenerationRate,carbonDioxideGenerationRate);
  }

  void PeopleDefinition_Impl::resetCarbonDioxideGenerationRate() {
    bool result = setString(OS_People_DefinitionFields::CarbonDioxideGenerationRate, "");
    BOOST_ASSERT(result);
  }

  void PeopleDefinition_Impl::setEnableASHRAE55ComfortWarnings(bool enableASHRAE55ComfortWarnings) {
    bool result = false;
    if (enableASHRAE55ComfortWarnings) {
      result = setString(OS_People_DefinitionFields::EnableASHRAE55ComfortWarnings, "Yes");
    } else {
      result = setString(OS_People_DefinitionFields::EnableASHRAE55ComfortWarnings, "No");
    }
    BOOST_ASSERT(result);
  }

  void PeopleDefinition_Impl::resetEnableASHRAE55ComfortWarnings() {
    bool result = setString(OS_People_DefinitionFields::EnableASHRAE55ComfortWarnings, "");
    BOOST_ASSERT(result);
  }

  bool PeopleDefinition_Impl::setMeanRadiantTemperatureCalculationType(std::string meanRadiantTemperatureCalculationType) {
    bool result = false;
    result = setString(OS_People_DefinitionFields::MeanRadiantTemperatureCalculationType, meanRadiantTemperatureCalculationType);
    return result;
  }

  void PeopleDefinition_Impl::resetMeanRadiantTemperatureCalculationType() {
    bool result = setString(OS_People_DefinitionFields::MeanRadiantTemperatureCalculationType, "");
    BOOST_ASSERT(result);
  }

  bool PeopleDefinition_Impl::pushThermalComfortModelType(
      const std::string& thermalComfortModelType)
  {
    return !pushExtensibleGroup(StringVector(1u,thermalComfortModelType)).empty();
  }

  bool PeopleDefinition_Impl::setThermalComfortModelType(
      int i, const std::string& thermalComfortModelType)
  {
    int n = numThermalComfortModelTypes();
    if (i == n) {
      return pushThermalComfortModelType(thermalComfortModelType);
    }
    if (i < n) {
      IdfExtensibleGroup eg = getExtensibleGroup(i);
      BOOST_ASSERT(!eg.empty());
      return eg.setString(OS_People_DefinitionExtensibleFields::ThermalComfortModelType,
                          thermalComfortModelType);
    }
    return false;
  }

  bool PeopleDefinition_Impl::eraseThermalComfortModelType(int i) {
    return !eraseExtensibleGroup(i).empty();
  }

  double PeopleDefinition_Impl::getNumberOfPeople(double floorArea) const {
    std::string method = numberofPeopleCalculationMethod();

    if (method == "People") {
      return numberofPeople().get();
    }
    else if (method == "People/Area") {
      return peopleperSpaceFloorArea().get() * floorArea;
    }
    else if (method == "Area/Person") {
      double areaPerPerson = spaceFloorAreaperPerson().get();
      if (equal(areaPerPerson,0.0)) {
        LOG_AND_THROW("Calculation would require division by zero.");
      }
      return floorArea / areaPerPerson;
    }

    BOOST_ASSERT(false);
    return 0.0;
  }

  double PeopleDefinition_Impl::getPeoplePerFloorArea(double floorArea) const {
    std::string method = numberofPeopleCalculationMethod();

    if (method == "People") {
      if (equal(floorArea,0.0)) {
        LOG_AND_THROW("Calculation would require division by zero.");
      }
      return numberofPeople().get() / floorArea;
    }
    else if (method == "People/Area") {
      return peopleperSpaceFloorArea().get();
    }
    else if (method == "Area/Person") {
      double areaPerPerson = spaceFloorAreaperPerson().get();
      if (equal(areaPerPerson,0.0)) {
        LOG_AND_THROW("Calculation would require division by zero.");
      }
      return 1.0 / areaPerPerson;
    }

    BOOST_ASSERT(false);
    return 0.0;
  }

  double PeopleDefinition_Impl::getFloorAreaPerPerson(double floorArea) const {
    std::string method = numberofPeopleCalculationMethod();

    if (method == "People") {
      double numPeople = numberofPeople().get();
      if (equal(numPeople,0.0)) {
        LOG_AND_THROW("Calculation would require division by zero.");
      }
      return floorArea / numPeople;
    }
    else if (method == "People/Area") {
      double peoplePerArea = peopleperSpaceFloorArea().get();
      if (equal(peoplePerArea,0.0)) {
        LOG_AND_THROW("Calculation would require division by zero.");
      }
      return 1.0 / peoplePerArea;
    }
    else if (method == "Area/Person") {
      return spaceFloorAreaperPerson().get();
    }

    BOOST_ASSERT(false);
    return 0.0;
  }

  bool PeopleDefinition_Impl::setNumberOfPeopleCalculationMethod(const std::string& method,
                                                                 double floorArea)
  {
    std::string wmethod(method);
    boost::to_lower(wmethod);

    if (wmethod == "people") {
      return setNumberofPeople(getNumberOfPeople(floorArea));
    }
    else if (wmethod == "people/area") {
      return setPeopleperSpaceFloorArea(getPeoplePerFloorArea(floorArea));
    }
    else if (wmethod == "area/person") {
      return setSpaceFloorAreaperPerson(getFloorAreaPerPerson(floorArea));
    }

    return false;
  }
  
  std::vector<std::string> PeopleDefinition_Impl::numberofPeopleCalculationMethodValues() const {
    return PeopleDefinition::numberofPeopleCalculationMethodValues();
  }

  openstudio::OSOptionalQuantity PeopleDefinition_Impl::numberofPeople_SI() const {
    return getNumberofPeople(false);
  }

  openstudio::OSOptionalQuantity PeopleDefinition_Impl::numberofPeople_IP() const {
    return getNumberofPeople(true);
  }

  openstudio::OSOptionalQuantity PeopleDefinition_Impl::peopleperSpaceFloorArea_SI() const {
    return getPeopleperSpaceFloorArea(false);
  }

  openstudio::OSOptionalQuantity PeopleDefinition_Impl::peopleperSpaceFloorArea_IP() const {
    return getPeopleperSpaceFloorArea(true);
  }

  openstudio::OSOptionalQuantity PeopleDefinition_Impl::spaceFloorAreaperPerson_SI() const {
    return getSpaceFloorAreaperPerson(false);
  }

  openstudio::OSOptionalQuantity PeopleDefinition_Impl::spaceFloorAreaperPerson_IP() const {
    return getSpaceFloorAreaperPerson(true);
  }

  openstudio::Quantity PeopleDefinition_Impl::fractionRadiant_SI() const {
    return getFractionRadiant(false);
  }

  openstudio::Quantity PeopleDefinition_Impl::fractionRadiant_IP() const {
    return getFractionRadiant(true);
  }

  openstudio::OSOptionalQuantity PeopleDefinition_Impl::sensibleHeatFraction_SI() const {
    return getSensibleHeatFraction(false);
  }

  openstudio::OSOptionalQuantity PeopleDefinition_Impl::sensibleHeatFraction_IP() const {
    return getSensibleHeatFraction(true);
  }

  openstudio::Quantity PeopleDefinition_Impl::carbonDioxideGenerationRate_SI() const {
    return getCarbonDioxideGenerationRate(false);
  }

  openstudio::Quantity PeopleDefinition_Impl::carbonDioxideGenerationRate_IP() const {
    return getCarbonDioxideGenerationRate(true);
  }

  std::vector<std::string> PeopleDefinition_Impl::meanRadiantTemperatureCalculationTypeValues() const {
    return PeopleDefinition::meanRadiantTemperatureCalculationTypeValues();
  }

  std::vector<std::string> PeopleDefinition_Impl::thermalComfortModelTypeValues() const {
    return PeopleDefinition::thermalComfortModelTypeValues();
  }

} // detail

PeopleDefinition::PeopleDefinition(const Model& model)
  : SpaceLoadDefinition(PeopleDefinition::iddObjectType(),model)
{
  BOOST_ASSERT(getImpl<detail::PeopleDefinition_Impl>());
  bool ok = getImpl<detail::PeopleDefinition_Impl>()->setNumberofPeople(0.0);
  BOOST_ASSERT(ok);
  ok = getImpl<detail::PeopleDefinition_Impl>()->setFractionRadiant(0.3);
  BOOST_ASSERT(ok);
}

IddObjectType PeopleDefinition::iddObjectType() {
  IddObjectType result(IddObjectType::OS_People_Definition);
  return result;
}

std::vector<std::string> PeopleDefinition::numberofPeopleCalculationMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_People_DefinitionFields::NumberofPeopleCalculationMethod);
}

std::vector<std::string> PeopleDefinition::meanRadiantTemperatureCalculationTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_People_DefinitionFields::MeanRadiantTemperatureCalculationType);
}

std::vector<std::string> PeopleDefinition::thermalComfortModelTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_People_DefinitionExtensibleFields::ThermalComfortModelType);
}

std::string PeopleDefinition::numberofPeopleCalculationMethod() const {
  return getImpl<detail::PeopleDefinition_Impl>()->numberofPeopleCalculationMethod();
}

boost::optional<double> PeopleDefinition::numberofPeople() const {
  return getImpl<detail::PeopleDefinition_Impl>()->numberofPeople();
}

OSOptionalQuantity PeopleDefinition::getNumberofPeople(bool returnIP) const {
  return getImpl<detail::PeopleDefinition_Impl>()->getNumberofPeople(returnIP);
}

boost::optional<double> PeopleDefinition::peopleperSpaceFloorArea() const {
  return getImpl<detail::PeopleDefinition_Impl>()->peopleperSpaceFloorArea();
}

OSOptionalQuantity PeopleDefinition::getPeopleperSpaceFloorArea(bool returnIP) const {
  return getImpl<detail::PeopleDefinition_Impl>()->getPeopleperSpaceFloorArea(returnIP);
}

boost::optional<double> PeopleDefinition::spaceFloorAreaperPerson() const {
  return getImpl<detail::PeopleDefinition_Impl>()->spaceFloorAreaperPerson();
}

OSOptionalQuantity PeopleDefinition::getSpaceFloorAreaperPerson(bool returnIP) const {
  return getImpl<detail::PeopleDefinition_Impl>()->getSpaceFloorAreaperPerson(returnIP);
}

double PeopleDefinition::fractionRadiant() const {
  return getImpl<detail::PeopleDefinition_Impl>()->fractionRadiant();
}

Quantity PeopleDefinition::getFractionRadiant(bool returnIP) const {
  return getImpl<detail::PeopleDefinition_Impl>()->getFractionRadiant(returnIP);
}

boost::optional<double> PeopleDefinition::sensibleHeatFraction() const {
  return getImpl<detail::PeopleDefinition_Impl>()->sensibleHeatFraction();
}

OSOptionalQuantity PeopleDefinition::getSensibleHeatFraction(bool returnIP) const {
  return getImpl<detail::PeopleDefinition_Impl>()->getSensibleHeatFraction(returnIP);
}

bool PeopleDefinition::isSensibleHeatFractionDefaulted() const {
  return getImpl<detail::PeopleDefinition_Impl>()->isSensibleHeatFractionDefaulted();
}

bool PeopleDefinition::isSensibleHeatFractionAutocalculated() const {
  return getImpl<detail::PeopleDefinition_Impl>()->isSensibleHeatFractionAutocalculated();
}

double PeopleDefinition::carbonDioxideGenerationRate() const {
  return getImpl<detail::PeopleDefinition_Impl>()->carbonDioxideGenerationRate();
}

Quantity PeopleDefinition::getCarbonDioxideGenerationRate(bool returnIP) const {
  return getImpl<detail::PeopleDefinition_Impl>()->getCarbonDioxideGenerationRate(returnIP);
}

bool PeopleDefinition::isCarbonDioxideGenerationRateDefaulted() const {
  return getImpl<detail::PeopleDefinition_Impl>()->isCarbonDioxideGenerationRateDefaulted();
}

bool PeopleDefinition::enableASHRAE55ComfortWarnings() const {
  return getImpl<detail::PeopleDefinition_Impl>()->enableASHRAE55ComfortWarnings();
}

bool PeopleDefinition::isEnableASHRAE55ComfortWarningsDefaulted() const {
  return getImpl<detail::PeopleDefinition_Impl>()->isEnableASHRAE55ComfortWarningsDefaulted();
}

std::string PeopleDefinition::meanRadiantTemperatureCalculationType() const {
  return getImpl<detail::PeopleDefinition_Impl>()->meanRadiantTemperatureCalculationType();
}

bool PeopleDefinition::isMeanRadiantTemperatureCalculationTypeDefaulted() const {
  return getImpl<detail::PeopleDefinition_Impl>()->isMeanRadiantTemperatureCalculationTypeDefaulted();
}

boost::optional<std::string> PeopleDefinition::getThermalComfortModelType(int i) const {
  return getImpl<detail::PeopleDefinition_Impl>()->getThermalComfortModelType(i);
}

int PeopleDefinition::numThermalComfortModelTypes() const {
  return getImpl<detail::PeopleDefinition_Impl>()->numThermalComfortModelTypes();
}

bool PeopleDefinition::setNumberofPeople(double numberofPeople) {
  return getImpl<detail::PeopleDefinition_Impl>()->setNumberofPeople(numberofPeople);
}

bool PeopleDefinition::setNumberofPeople(const Quantity& numberofPeople) {
  return getImpl<detail::PeopleDefinition_Impl>()->setNumberofPeople(numberofPeople);
}

bool PeopleDefinition::setPeopleperSpaceFloorArea(double peopleperSpaceFloorArea) {
  return getImpl<detail::PeopleDefinition_Impl>()->setPeopleperSpaceFloorArea(peopleperSpaceFloorArea);
}

bool PeopleDefinition::setPeopleperSpaceFloorArea(const Quantity& peopleperSpaceFloorArea) {
  return getImpl<detail::PeopleDefinition_Impl>()->setPeopleperSpaceFloorArea(peopleperSpaceFloorArea);
}

bool PeopleDefinition::setSpaceFloorAreaperPerson(double spaceFloorAreaperPerson) {
  return getImpl<detail::PeopleDefinition_Impl>()->setSpaceFloorAreaperPerson(spaceFloorAreaperPerson);
}

bool PeopleDefinition::setSpaceFloorAreaperPerson(const Quantity& spaceFloorAreaperPerson) {
  return getImpl<detail::PeopleDefinition_Impl>()->setSpaceFloorAreaperPerson(spaceFloorAreaperPerson);
}

bool PeopleDefinition::setFractionRadiant(double fractionRadiant) {
  return getImpl<detail::PeopleDefinition_Impl>()->setFractionRadiant(fractionRadiant);
}

bool PeopleDefinition::setFractionRadiant(const Quantity& fractionRadiant) {
  return getImpl<detail::PeopleDefinition_Impl>()->setFractionRadiant(fractionRadiant);
}

bool PeopleDefinition::setSensibleHeatFraction(boost::optional<double> sensibleHeatFraction) {
  return getImpl<detail::PeopleDefinition_Impl>()->setSensibleHeatFraction(sensibleHeatFraction);
}

bool PeopleDefinition::setSensibleHeatFraction(double sensibleHeatFraction) {
  return getImpl<detail::PeopleDefinition_Impl>()->setSensibleHeatFraction(sensibleHeatFraction);
}

bool PeopleDefinition::setSensibleHeatFraction(const Quantity& sensibleHeatFraction) {
  return getImpl<detail::PeopleDefinition_Impl>()->setSensibleHeatFraction(sensibleHeatFraction);
}

void PeopleDefinition::resetSensibleHeatFraction() {
  getImpl<detail::PeopleDefinition_Impl>()->resetSensibleHeatFraction();
}

void PeopleDefinition::autocalculateSensibleHeatFraction() {
  getImpl<detail::PeopleDefinition_Impl>()->autocalculateSensibleHeatFraction();
}

bool PeopleDefinition::setCarbonDioxideGenerationRate(double carbonDioxideGenerationRate) {
  return getImpl<detail::PeopleDefinition_Impl>()->setCarbonDioxideGenerationRate(carbonDioxideGenerationRate);
}

bool PeopleDefinition::setCarbonDioxideGenerationRate(const Quantity& carbonDioxideGenerationRate) {
  return getImpl<detail::PeopleDefinition_Impl>()->setCarbonDioxideGenerationRate(carbonDioxideGenerationRate);
}

void PeopleDefinition::resetCarbonDioxideGenerationRate() {
  getImpl<detail::PeopleDefinition_Impl>()->resetCarbonDioxideGenerationRate();
}

void PeopleDefinition::setEnableASHRAE55ComfortWarnings(bool enableASHRAE55ComfortWarnings) {
  getImpl<detail::PeopleDefinition_Impl>()->setEnableASHRAE55ComfortWarnings(enableASHRAE55ComfortWarnings);
}

void PeopleDefinition::resetEnableASHRAE55ComfortWarnings() {
  getImpl<detail::PeopleDefinition_Impl>()->resetEnableASHRAE55ComfortWarnings();
}

bool PeopleDefinition::setMeanRadiantTemperatureCalculationType(std::string meanRadiantTemperatureCalculationType) {
  return getImpl<detail::PeopleDefinition_Impl>()->setMeanRadiantTemperatureCalculationType(meanRadiantTemperatureCalculationType);
}

void PeopleDefinition::resetMeanRadiantTemperatureCalculationType() {
  getImpl<detail::PeopleDefinition_Impl>()->resetMeanRadiantTemperatureCalculationType();
}

bool PeopleDefinition::pushThermalComfortModelType(const std::string& thermalComfortModelType) {
  return getImpl<detail::PeopleDefinition_Impl>()->pushThermalComfortModelType(thermalComfortModelType);
}

bool PeopleDefinition::setThermalComfortModelType(int i, const std::string& thermalComfortModelType) {
  return getImpl<detail::PeopleDefinition_Impl>()->setThermalComfortModelType(i,thermalComfortModelType);
}

bool PeopleDefinition::eraseThermalComfortModelType(int i) {
  return getImpl<detail::PeopleDefinition_Impl>()->eraseThermalComfortModelType(i);
}

double PeopleDefinition::getNumberOfPeople(double floorArea) const {
  return getImpl<detail::PeopleDefinition_Impl>()->getNumberOfPeople(floorArea);
}

double PeopleDefinition::getPeoplePerFloorArea(double floorArea) const {
  return getImpl<detail::PeopleDefinition_Impl>()->getPeoplePerFloorArea(floorArea);
}

double PeopleDefinition::getFloorAreaPerPerson(double floorArea) const {
  return getImpl<detail::PeopleDefinition_Impl>()->getFloorAreaPerPerson(floorArea);
}

bool PeopleDefinition::setNumberOfPeopleCalculationMethod(const std::string& method,double floorArea)
{
  return getImpl<detail::PeopleDefinition_Impl>()->setNumberOfPeopleCalculationMethod(method,floorArea);
}

/// @cond
PeopleDefinition::PeopleDefinition(boost::shared_ptr<detail::PeopleDefinition_Impl> impl)
  : SpaceLoadDefinition(impl)
{}
/// @endcond

} // model
} // openstudio

