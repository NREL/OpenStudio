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

#include "PeopleDefinition.hpp"
#include "PeopleDefinition_Impl.hpp"

#include "../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_People_Definition_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/math/FloatCompare.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  PeopleDefinition_Impl::PeopleDefinition_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : SpaceLoadDefinition_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == PeopleDefinition::iddObjectType());
  }

  PeopleDefinition_Impl::PeopleDefinition_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                               Model_Impl* model,
                                               bool keepHandle)
    : SpaceLoadDefinition_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == PeopleDefinition::iddObjectType());
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
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> PeopleDefinition_Impl::numberofPeople() const {
    OptionalDouble value = getDouble(OS_People_DefinitionFields::NumberofPeople,true);
    if (istringEqual("People", this->numberofPeopleCalculationMethod())) {
      OS_ASSERT(value);
    }
    return value;
  }

  boost::optional<double> PeopleDefinition_Impl::peopleperSpaceFloorArea() const {
    OptionalDouble value = getDouble(OS_People_DefinitionFields::PeopleperSpaceFloorArea,true);
    if (istringEqual("People/Area", this->numberofPeopleCalculationMethod())) {
      OS_ASSERT(value);
    }
    return value;
  }

  boost::optional<double> PeopleDefinition_Impl::spaceFloorAreaperPerson() const {
    OptionalDouble value = getDouble(OS_People_DefinitionFields::SpaceFloorAreaperPerson,true);
    if (istringEqual("Area/Person", this->numberofPeopleCalculationMethod())) {
      OS_ASSERT(value);
    }
    return value;
  }

  double PeopleDefinition_Impl::fractionRadiant() const {
    OptionalDouble value = getDouble(OS_People_DefinitionFields::FractionRadiant,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> PeopleDefinition_Impl::sensibleHeatFraction() const {
    OptionalDouble value = getDouble(OS_People_DefinitionFields::SensibleHeatFraction,true);
    return value;
  }

  bool PeopleDefinition_Impl::isSensibleHeatFractionDefaulted() const {
    return isEmpty(OS_People_DefinitionFields::SensibleHeatFraction);
  }

  bool PeopleDefinition_Impl::isSensibleHeatFractionAutocalculated() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_People_DefinitionFields::SensibleHeatFraction, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autocalculate");
    }
    return result;
  }

  double PeopleDefinition_Impl::carbonDioxideGenerationRate() const {
    OptionalDouble value = getDouble(OS_People_DefinitionFields::CarbonDioxideGenerationRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool PeopleDefinition_Impl::isCarbonDioxideGenerationRateDefaulted() const {
    return isEmpty(OS_People_DefinitionFields::CarbonDioxideGenerationRate);
  }

  bool PeopleDefinition_Impl::enableASHRAE55ComfortWarnings() const {
    boost::optional<std::string> value = getString(OS_People_DefinitionFields::EnableASHRAE55ComfortWarnings,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }

  bool PeopleDefinition_Impl::isEnableASHRAE55ComfortWarningsDefaulted() const {
    return isEmpty(OS_People_DefinitionFields::EnableASHRAE55ComfortWarnings);
  }

  std::string PeopleDefinition_Impl::meanRadiantTemperatureCalculationType() const {
    boost::optional<std::string> value = getString(OS_People_DefinitionFields::MeanRadiantTemperatureCalculationType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool PeopleDefinition_Impl::isMeanRadiantTemperatureCalculationTypeDefaulted() const {
    return isEmpty(OS_People_DefinitionFields::MeanRadiantTemperatureCalculationType);
  }

  boost::optional<std::string> PeopleDefinition_Impl::getThermalComfortModelType(int i) const {
    OptionalString result;
    if (i < numThermalComfortModelTypes()) {
      IdfExtensibleGroup eg = getExtensibleGroup(i);
      OS_ASSERT(!eg.empty());
      result = eg.getString(OS_People_DefinitionExtensibleFields::ThermalComfortModelType,true);
    }
    return result;
  }

  int PeopleDefinition_Impl::numThermalComfortModelTypes() const {
    return numExtensibleGroups();
  }

  bool PeopleDefinition_Impl::setNumberofPeople(boost::optional<double> numberofPeople) {
    bool result = false;
    if (numberofPeople) {
      result = setDouble(OS_People_DefinitionFields::NumberofPeople,numberofPeople.get());
      if (result) {
        result = setString(OS_People_DefinitionFields::NumberofPeopleCalculationMethod, "People");
        OS_ASSERT(result);
        result = setString(OS_People_DefinitionFields::PeopleperSpaceFloorArea, "");
        OS_ASSERT(result);
        result = setString(OS_People_DefinitionFields::SpaceFloorAreaperPerson, "");
        OS_ASSERT(result);
      }
    } else {
      if (istringEqual("People", this->numberofPeopleCalculationMethod())){
        result = setDouble(OS_People_DefinitionFields::NumberofPeople, 0.0);
      }
    }
    return result;
  }

  bool PeopleDefinition_Impl::setPeopleperSpaceFloorArea(boost::optional<double> peopleperSpaceFloorArea) {
    bool result = false;
    if (peopleperSpaceFloorArea) {
      result = setDouble(OS_People_DefinitionFields::PeopleperSpaceFloorArea,peopleperSpaceFloorArea.get());
      if (result) {
        result = setString(OS_People_DefinitionFields::NumberofPeopleCalculationMethod, "People/Area");
        OS_ASSERT(result);
        result = setString(OS_People_DefinitionFields::NumberofPeople, "");
        OS_ASSERT(result);
        result = setString(OS_People_DefinitionFields::SpaceFloorAreaperPerson, "");
        OS_ASSERT(result);
      }
    } else {
      if (istringEqual("People/Area", this->numberofPeopleCalculationMethod())){
        result = setDouble(OS_People_DefinitionFields::PeopleperSpaceFloorArea, 0.0);
      }
    }
    return result;
  }

  bool PeopleDefinition_Impl::setSpaceFloorAreaperPerson(boost::optional<double> spaceFloorAreaperPerson) {
    bool result(false);
    if (spaceFloorAreaperPerson) {
      result = setDouble(OS_People_DefinitionFields::SpaceFloorAreaperPerson,spaceFloorAreaperPerson.get());
      if (result) {
        result = setString(OS_People_DefinitionFields::NumberofPeopleCalculationMethod, "Area/Person");
        OS_ASSERT(result);
        result = setString(OS_People_DefinitionFields::NumberofPeople, "");
        OS_ASSERT(result);
        result = setString(OS_People_DefinitionFields::PeopleperSpaceFloorArea, "");
        OS_ASSERT(result);
      }
    } else {
      if (istringEqual("Area/Person", this->numberofPeopleCalculationMethod())){
        result = setDouble(OS_People_DefinitionFields::SpaceFloorAreaperPerson, 0.0);
      }
    }
    return result;
  }

  bool PeopleDefinition_Impl::setFractionRadiant(double fractionRadiant) {
    return setDouble(OS_People_DefinitionFields::FractionRadiant,fractionRadiant);
  }

  bool PeopleDefinition_Impl::setSensibleHeatFraction(double sensibleHeatFraction) {
    return setDouble(OS_People_DefinitionFields::SensibleHeatFraction,sensibleHeatFraction);
  }

  void PeopleDefinition_Impl::resetSensibleHeatFraction() {
    bool result = setString(OS_People_DefinitionFields::SensibleHeatFraction, "");
    OS_ASSERT(result);
  }

  void PeopleDefinition_Impl::autocalculateSensibleHeatFraction() {
    bool result = setString(OS_People_DefinitionFields::SensibleHeatFraction, "Autocalculate");
    OS_ASSERT(result);
  }

  bool PeopleDefinition_Impl::setCarbonDioxideGenerationRate(double carbonDioxideGenerationRate) {
    return setDouble(OS_People_DefinitionFields::CarbonDioxideGenerationRate,carbonDioxideGenerationRate);
  }

  void PeopleDefinition_Impl::resetCarbonDioxideGenerationRate() {
    bool result = setString(OS_People_DefinitionFields::CarbonDioxideGenerationRate, "");
    OS_ASSERT(result);
  }

  void PeopleDefinition_Impl::setEnableASHRAE55ComfortWarnings(bool enableASHRAE55ComfortWarnings) {
    bool result = false;
    if (enableASHRAE55ComfortWarnings) {
      result = setString(OS_People_DefinitionFields::EnableASHRAE55ComfortWarnings, "Yes");
    } else {
      result = setString(OS_People_DefinitionFields::EnableASHRAE55ComfortWarnings, "No");
    }
    OS_ASSERT(result);
  }

  void PeopleDefinition_Impl::resetEnableASHRAE55ComfortWarnings() {
    bool result = setString(OS_People_DefinitionFields::EnableASHRAE55ComfortWarnings, "");
    OS_ASSERT(result);
  }

  bool PeopleDefinition_Impl::setMeanRadiantTemperatureCalculationType(std::string meanRadiantTemperatureCalculationType) {
    bool result = setString(OS_People_DefinitionFields::MeanRadiantTemperatureCalculationType, meanRadiantTemperatureCalculationType);
    return result;
  }

  void PeopleDefinition_Impl::resetMeanRadiantTemperatureCalculationType() {
    bool result = setString(OS_People_DefinitionFields::MeanRadiantTemperatureCalculationType, "");
    OS_ASSERT(result);
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
      OS_ASSERT(!eg.empty());
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

    OS_ASSERT(false);
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

    OS_ASSERT(false);
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

    OS_ASSERT(false);
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
  OS_ASSERT(getImpl<detail::PeopleDefinition_Impl>());
  bool ok = getImpl<detail::PeopleDefinition_Impl>()->setNumberofPeople(0.0);
  OS_ASSERT(ok);
  ok = getImpl<detail::PeopleDefinition_Impl>()->setFractionRadiant(0.3);
  OS_ASSERT(ok);
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

boost::optional<double> PeopleDefinition::peopleperSpaceFloorArea() const {
  return getImpl<detail::PeopleDefinition_Impl>()->peopleperSpaceFloorArea();
}

boost::optional<double> PeopleDefinition::spaceFloorAreaperPerson() const {
  return getImpl<detail::PeopleDefinition_Impl>()->spaceFloorAreaperPerson();
}

double PeopleDefinition::fractionRadiant() const {
  return getImpl<detail::PeopleDefinition_Impl>()->fractionRadiant();
}

boost::optional<double> PeopleDefinition::sensibleHeatFraction() const {
  return getImpl<detail::PeopleDefinition_Impl>()->sensibleHeatFraction();
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

bool PeopleDefinition::setPeopleperSpaceFloorArea(double peopleperSpaceFloorArea) {
  return getImpl<detail::PeopleDefinition_Impl>()->setPeopleperSpaceFloorArea(peopleperSpaceFloorArea);
}

bool PeopleDefinition::setSpaceFloorAreaperPerson(double spaceFloorAreaperPerson) {
  return getImpl<detail::PeopleDefinition_Impl>()->setSpaceFloorAreaperPerson(spaceFloorAreaperPerson);
}

bool PeopleDefinition::setFractionRadiant(double fractionRadiant) {
  return getImpl<detail::PeopleDefinition_Impl>()->setFractionRadiant(fractionRadiant);
}

bool PeopleDefinition::setSensibleHeatFraction(double sensibleHeatFraction) {
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
PeopleDefinition::PeopleDefinition(std::shared_ptr<detail::PeopleDefinition_Impl> impl)
  : SpaceLoadDefinition(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

