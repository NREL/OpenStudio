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

#include "InternalMassDefinition.hpp"
#include "InternalMassDefinition_Impl.hpp"

#include "ConstructionBase.hpp"
#include "ConstructionBase_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_InternalMass_Definition_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/math/FloatCompare.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ATTRIBUTE_IMPLEMENTATION(1,0,0,surfaceArea,SurfaceArea,InternalMassDefinition,0,OS_InternalMass_Definition,SurfaceArea)

  ATTRIBUTE_IMPLEMENTATION(1,0,0,surfaceAreaperSpaceFloorArea,SurfaceAreaperSpaceFloorArea,
                           InternalMassDefinition,0,OS_InternalMass_Definition,SurfaceAreaperSpaceFloorArea)

  ATTRIBUTE_IMPLEMENTATION(1,0,0,surfaceAreaperPerson,SurfaceAreaperPerson,
                           InternalMassDefinition,0,OS_InternalMass_Definition,SurfaceAreaperPerson)

  InternalMassDefinition_Impl::InternalMassDefinition_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : SpaceLoadDefinition_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == InternalMassDefinition::iddObjectType());
  }

  InternalMassDefinition_Impl::InternalMassDefinition_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                           Model_Impl* model,
                                                           bool keepHandle)
    : SpaceLoadDefinition_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == InternalMassDefinition::iddObjectType());
  }

  InternalMassDefinition_Impl::InternalMassDefinition_Impl(const InternalMassDefinition_Impl& other,
                                                           Model_Impl* model,
                                                           bool keepHandle)
    : SpaceLoadDefinition_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& InternalMassDefinition_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType InternalMassDefinition_Impl::iddObjectType() const {
    return InternalMassDefinition::iddObjectType();
  }

  std::string InternalMassDefinition_Impl::designLevelCalculationMethod() const {
    boost::optional<std::string> value = getString(OS_InternalMass_DefinitionFields::DesignLevelCalculationMethod,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> InternalMassDefinition_Impl::surfaceArea() const {
    boost::optional<double> result;
    if (istringEqual("SurfaceArea", this->designLevelCalculationMethod())){
      result = getDouble(OS_InternalMass_DefinitionFields::SurfaceArea, true);
      //OS_ASSERT(result);
    }
    return result;
  }

  boost::optional<double> InternalMassDefinition_Impl::surfaceAreaperSpaceFloorArea() const {
    boost::optional<double> result;
    if (istringEqual("SurfaceArea/Area", this->designLevelCalculationMethod())){
      result = getDouble(OS_InternalMass_DefinitionFields::SurfaceAreaperSpaceFloorArea, true);
      //OS_ASSERT(result);
    }
    return result;
  }

  boost::optional<double> InternalMassDefinition_Impl::surfaceAreaperPerson() const {
    boost::optional<double> result;
    if (istringEqual("SurfaceArea/Person", this->designLevelCalculationMethod())){
      result = getDouble(OS_InternalMass_DefinitionFields::SurfaceAreaperPerson, true);
      //OS_ASSERT(result);
    }
    return result;
  }

  bool InternalMassDefinition_Impl::setSurfaceArea(boost::optional<double> surfaceArea) {
    bool result = true;
    if (surfaceArea) {
      if (*surfaceArea < 0){
        result = false;
      }else{
        result = setString(OS_InternalMass_DefinitionFields::DesignLevelCalculationMethod, "SurfaceArea");
        OS_ASSERT(result);
        result = setDouble(OS_InternalMass_DefinitionFields::SurfaceArea, surfaceArea.get());
        OS_ASSERT(result);
        result = setString(OS_InternalMass_DefinitionFields::SurfaceAreaperSpaceFloorArea, "");
        OS_ASSERT(result);
        result = setString(OS_InternalMass_DefinitionFields::SurfaceAreaperPerson, "");
        OS_ASSERT(result);
      }
    } else {
      if (istringEqual("SurfaceArea", this->designLevelCalculationMethod())){
        result = setDouble(OS_InternalMass_DefinitionFields::SurfaceArea, 0.0);
      }
    }
    return result;
  }

  bool InternalMassDefinition_Impl::setSurfaceAreaperSpaceFloorArea(boost::optional<double> surfaceAreaperSpaceFloorArea) {
    bool result = true;
    if (surfaceAreaperSpaceFloorArea) {
      if (*surfaceAreaperSpaceFloorArea < 0){
        result = false;
      }else{
        result = setString(OS_InternalMass_DefinitionFields::DesignLevelCalculationMethod, "SurfaceArea/Area");
        OS_ASSERT(result);
        result = setString(OS_InternalMass_DefinitionFields::SurfaceArea, "");
        OS_ASSERT(result);
        result = setDouble(OS_InternalMass_DefinitionFields::SurfaceAreaperSpaceFloorArea, surfaceAreaperSpaceFloorArea.get());
        OS_ASSERT(result);
        result = setString(OS_InternalMass_DefinitionFields::SurfaceAreaperPerson, "");
        OS_ASSERT(result);
      }
    } else {
      if (istringEqual("SurfaceArea/Area", this->designLevelCalculationMethod())){
        result = setDouble(OS_InternalMass_DefinitionFields::SurfaceAreaperSpaceFloorArea, 0.0);
      }
    }
    return result;
  }

  bool InternalMassDefinition_Impl::setSurfaceAreaperPerson(boost::optional<double> surfaceAreaperPerson) {
    bool result = true;
    if (surfaceAreaperPerson) {
      if (*surfaceAreaperPerson < 0){
        result = false;
      }else{
        result = setString(OS_InternalMass_DefinitionFields::DesignLevelCalculationMethod, "SurfaceArea/Person");
        OS_ASSERT(result);
        result = setString(OS_InternalMass_DefinitionFields::SurfaceArea, "");
        OS_ASSERT(result);
        result = setString(OS_InternalMass_DefinitionFields::SurfaceAreaperSpaceFloorArea, "");
        OS_ASSERT(result);
        result = setDouble(OS_InternalMass_DefinitionFields::SurfaceAreaperPerson, surfaceAreaperPerson.get());
        OS_ASSERT(result);
      }
    } else {
      if (istringEqual("SurfaceArea/Person", this->designLevelCalculationMethod())){
        result = setDouble(OS_InternalMass_DefinitionFields::SurfaceAreaperPerson, 0.0);
      }
    }
    return result;
  }

  boost::optional<ConstructionBase> InternalMassDefinition_Impl::construction() const
  {
    return getObject<ModelObject>().getModelObjectTarget<ConstructionBase>(OS_InternalMass_DefinitionFields::ConstructionName);
  }

  bool InternalMassDefinition_Impl::isConstructionDefaulted() const
  {
    return isEmpty(OS_InternalMass_DefinitionFields::ConstructionName);
  }

  bool InternalMassDefinition_Impl::setConstruction(const ConstructionBase& construction)
  {
    return setPointer(OS_InternalMass_DefinitionFields::ConstructionName, construction.handle());
  }

  void InternalMassDefinition_Impl::resetConstruction()
  {
    setString(OS_InternalMass_DefinitionFields::ConstructionName, "");
  }

  double InternalMassDefinition_Impl::getSurfaceArea(double floorArea, double numPeople) const {
    std::string method = designLevelCalculationMethod();

    if (method == "SurfaceArea") {
      return surfaceArea().get();
    }
    else if (method == "SurfaceArea/Area") {
      return surfaceAreaperSpaceFloorArea().get() * floorArea;
    }
    else if (method == "SurfaceArea/Person") {
      return surfaceAreaperPerson().get() * numPeople;
    }

    OS_ASSERT(false);
    return 0.0;
  }

  double InternalMassDefinition_Impl::getSurfaceAreaPerFloorArea(double floorArea,
                                                                      double numPeople) const
  {
    std::string method = designLevelCalculationMethod();

    if (method == "SurfaceArea") {
      if (equal(floorArea,0.0)) {
        LOG_AND_THROW("Calculation would require division by zero.");
      }
      return surfaceArea().get() / floorArea;
    }
    else if (method == "SurfaceArea/Area") {
      return surfaceAreaperSpaceFloorArea().get();
    }
    else if (method == "SurfaceArea/Person") {
      if (equal(floorArea,0.0)) {
        LOG_AND_THROW("Calculation would require division by zero.");
      }
      return surfaceAreaperPerson().get() * numPeople / floorArea;
    }

    OS_ASSERT(false);
    return 0.0;
  }

  double InternalMassDefinition_Impl::getSurfaceAreaPerPerson(double floorArea,
                                                              double numPeople) const
  {
    std::string method = designLevelCalculationMethod();

    if (method == "SurfaceArea") {
      if (equal(numPeople,0.0)) {
        LOG_AND_THROW("Calculation would require division by zero.");
      }
      return surfaceArea().get() / numPeople;
    }
    else if (method == "SurfaceArea/Area") {
      if (equal(numPeople,0.0)) {
        LOG_AND_THROW("Calculation would require division by zero.");
      }
      return surfaceAreaperSpaceFloorArea().get() * floorArea / numPeople;
    }
    else if (method == "SurfaceArea/Person") {
      return surfaceAreaperPerson().get();
    }

    OS_ASSERT(false);
    return 0.0;
  }

  bool InternalMassDefinition_Impl::setDesignLevelCalculationMethod(const std::string& method,
                                                                    double floorArea,
                                                                    double numPeople)
  {
    std::string wmethod(method);
    boost::to_lower(wmethod);

    if (wmethod == "surfacearea") {
      return setSurfaceArea(getSurfaceArea(floorArea,numPeople));
    }
    else if (wmethod == "surfacearea/area") {
      return setSurfaceAreaperSpaceFloorArea(getSurfaceAreaPerFloorArea(floorArea,numPeople));
    }
    else if (wmethod == "surfacearea/person") {
      return setSurfaceAreaperPerson(getSurfaceAreaPerPerson(floorArea,numPeople));
    }

    return false;
  }

  boost::optional<ModelObject> InternalMassDefinition_Impl::constructionAsModelObject() const {
    OptionalModelObject result;
    OptionalConstructionBase intermediate = construction();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  bool InternalMassDefinition_Impl::setConstructionAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalConstructionBase intermediate = modelObject->optionalCast<ConstructionBase>();
      if (intermediate) {
        return setConstruction(*intermediate);
      }
      else {
        return false;
      }
    }
    else {
      resetConstruction();
    }
    return true;
  }

} // detail

InternalMassDefinition::InternalMassDefinition(const Model& model)
  : SpaceLoadDefinition(InternalMassDefinition::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::InternalMassDefinition_Impl>());
  bool test = this->setSurfaceArea(0.0);
  OS_ASSERT(test);
}

IddObjectType InternalMassDefinition::iddObjectType() {
  IddObjectType result(IddObjectType::OS_InternalMass_Definition);
  return result;
}

std::vector<std::string> InternalMassDefinition::validDesignLevelCalculationMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_InternalMass_DefinitionFields::DesignLevelCalculationMethod);
}

std::string InternalMassDefinition::designLevelCalculationMethod() const {
  return getImpl<detail::InternalMassDefinition_Impl>()->designLevelCalculationMethod();
}

boost::optional<double> InternalMassDefinition::surfaceArea() const {
  return getImpl<detail::InternalMassDefinition_Impl>()->surfaceArea();
}

boost::optional<double> InternalMassDefinition::surfaceAreaperSpaceFloorArea() const {
  return getImpl<detail::InternalMassDefinition_Impl>()->surfaceAreaperSpaceFloorArea();
}

boost::optional<double> InternalMassDefinition::surfaceAreaperPerson() const {
  return getImpl<detail::InternalMassDefinition_Impl>()->surfaceAreaperPerson();
}

bool InternalMassDefinition::setSurfaceArea(double surfaceArea) {
  return getImpl<detail::InternalMassDefinition_Impl>()->setSurfaceArea(surfaceArea);
}

bool InternalMassDefinition::setSurfaceAreaperSpaceFloorArea(double surfaceAreaperSpaceFloorArea) {
  return getImpl<detail::InternalMassDefinition_Impl>()->setSurfaceAreaperSpaceFloorArea(surfaceAreaperSpaceFloorArea);
}

bool InternalMassDefinition::setSurfaceAreaperPerson(double surfaceAreaperPerson) {
  return getImpl<detail::InternalMassDefinition_Impl>()->setSurfaceAreaperPerson(surfaceAreaperPerson);
}

boost::optional<ConstructionBase> InternalMassDefinition::construction() const {
  return getImpl<detail::InternalMassDefinition_Impl>()->construction();
}

bool InternalMassDefinition::isConstructionDefaulted() const {
  return getImpl<detail::InternalMassDefinition_Impl>()->isConstructionDefaulted();
}

bool InternalMassDefinition::setConstruction(const ConstructionBase& construction) {
  return getImpl<detail::InternalMassDefinition_Impl>()->setConstruction(construction);
}

void InternalMassDefinition::resetConstruction() {
  getImpl<detail::InternalMassDefinition_Impl>()->resetConstruction();
}

double InternalMassDefinition::getSurfaceArea(double floorArea, double numPeople) const {
  return getImpl<detail::InternalMassDefinition_Impl>()->getSurfaceArea(floorArea,numPeople);
}

double InternalMassDefinition::getSurfaceAreaPerFloorArea(double floorArea, double numPeople) const {
  return getImpl<detail::InternalMassDefinition_Impl>()->getSurfaceAreaPerFloorArea(floorArea,numPeople);
}

double InternalMassDefinition::getSurfaceAreaPerPerson(double floorArea, double numPeople) const {
  return getImpl<detail::InternalMassDefinition_Impl>()->getSurfaceAreaPerPerson(floorArea,numPeople);
}

bool InternalMassDefinition::setDesignLevelCalculationMethod(const std::string& method,
                                                             double floorArea,
                                                             double numPeople)
{
  return getImpl<detail::InternalMassDefinition_Impl>()->setDesignLevelCalculationMethod(method,floorArea,numPeople);
}

/// @cond
InternalMassDefinition::InternalMassDefinition(std::shared_ptr<detail::InternalMassDefinition_Impl> impl)
  : SpaceLoadDefinition(std::move(impl))
{}
/// @endcond


} // model
} // openstudio

