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

#include "InternalMass.hpp"
#include "InternalMass_Impl.hpp"

#include "InternalMassDefinition.hpp"
#include "InternalMassDefinition_Impl.hpp"
#include "Space.hpp"
#include "Space_Impl.hpp"
#include "LifeCycleCost.hpp"

#include <utilities/idd/OS_InternalMass_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  InternalMass_Impl::InternalMass_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : SpaceLoadInstance_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == InternalMass::iddObjectType());
  }

  InternalMass_Impl::InternalMass_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                       Model_Impl* model,
                                       bool keepHandle)
    : SpaceLoadInstance_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == InternalMass::iddObjectType());
  }

  InternalMass_Impl::InternalMass_Impl(const InternalMass_Impl& other,
                                       Model_Impl* model,
                                       bool keepHandle)
    : SpaceLoadInstance_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& InternalMass_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType InternalMass_Impl::iddObjectType() const {
    return InternalMass::iddObjectType();
  }

  bool InternalMass_Impl::hardSize()
  {
    boost::optional<Space> space = this->space();
    if (!space){
      return false;
    }

    this->makeUnique();

    InternalMassDefinition internalMassDefinition = this->internalMassDefinition();
    for (LifeCycleCost cost : internalMassDefinition.lifeCycleCosts()){
      cost.convertToCostPerEach();
    }

    boost::optional<double> surfaceArea = internalMassDefinition.surfaceArea();
    if (surfaceArea){
      return true;
    }

    boost::optional<double> surfaceAreaperSpaceFloorArea = internalMassDefinition.surfaceAreaperSpaceFloorArea();
    if (surfaceAreaperSpaceFloorArea){
      return internalMassDefinition.setSurfaceArea(*surfaceAreaperSpaceFloorArea * space->floorArea());
    }

    boost::optional<double> surfaceAreaperPerson = internalMassDefinition.surfaceAreaperPerson();
    if (surfaceAreaperPerson){
      return internalMassDefinition.setSurfaceArea(*surfaceAreaperPerson * space->numberOfPeople());
    }
  
    return false;
  }

  bool InternalMass_Impl::hardApplySchedules()
  {
    return true;
  }

  double InternalMass_Impl::multiplier() const {
    boost::optional<double> value = getDouble(OS_InternalMassFields::Multiplier,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool InternalMass_Impl::isMultiplierDefaulted() const {
    return isEmpty(OS_InternalMassFields::Multiplier);
  }

  bool InternalMass_Impl::isAbsolute() const {
    InternalMassDefinition definition = internalMassDefinition();
    if (definition.surfaceArea()) {
      return true;
    }
    return false;
  }

  bool InternalMass_Impl::setMultiplier(double multiplier) {
    bool result = setDouble(OS_InternalMassFields::Multiplier, multiplier);
    return result;
  }

  void InternalMass_Impl::resetMultiplier() {
    bool result = setString(OS_InternalMassFields::Multiplier, "");
    OS_ASSERT(result);
  }

  int InternalMass_Impl::spaceIndex() const {
    return OS_InternalMassFields::SpaceorSpaceTypeName;
  }

  int InternalMass_Impl::definitionIndex() const {
    return OS_InternalMassFields::InternalMassDefinitionName;
  }

  InternalMassDefinition InternalMass_Impl::internalMassDefinition() const
  {
    return this->definition().cast<InternalMassDefinition>();
  }

  bool InternalMass_Impl::setInternalMassDefinition(const InternalMassDefinition& definition)
  {
    return this->setPointer(this->definitionIndex(), definition.handle());
  }

  bool InternalMass_Impl::setDefinition(const SpaceLoadDefinition& definition)
  {
    bool result = false;
    boost::optional<InternalMassDefinition> internalMassDefinition = definition.optionalCast<InternalMassDefinition>();
    if (internalMassDefinition){
      result = setInternalMassDefinition(*internalMassDefinition);
    }
    return result;
  }

  boost::optional<double> InternalMass_Impl::surfaceArea() const {
    OptionalDouble result = internalMassDefinition().surfaceArea();
    if (result) {
      return result.get() * multiplier();
    }
    return result;
  }

  boost::optional<double> InternalMass_Impl::surfaceAreaPerFloorArea() const {
    OptionalDouble result = internalMassDefinition().surfaceAreaperSpaceFloorArea();
    if (result) {
      return result.get() * multiplier();
    }
    return result;
  }

  boost::optional<double> InternalMass_Impl::surfaceAreaPerPerson() const {
    OptionalDouble result = internalMassDefinition().surfaceAreaperPerson();
    if (result) {
      return result.get() * multiplier();
    }
    return result;
  }

  double InternalMass_Impl::getSurfaceArea(double floorArea, double numPeople) const {
    return internalMassDefinition().getSurfaceArea(floorArea,numPeople) * multiplier();
  }

  double InternalMass_Impl::getSurfaceAreaPerFloorArea(double floorArea, double numPeople) const {
    return internalMassDefinition().getSurfaceAreaPerFloorArea(floorArea,numPeople) * multiplier();
  }

  double InternalMass_Impl::getSurfaceAreaPerPerson(double floorArea, double numPeople) const {
    return internalMassDefinition().getSurfaceAreaPerPerson(floorArea,numPeople) * multiplier();
  }

  boost::optional<ModelObject> InternalMass_Impl::internalMassDefinitionAsModelObject() const {
    OptionalModelObject result = internalMassDefinition();
    return result;
  }

  bool InternalMass_Impl::setInternalMassDefinitionAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalInternalMassDefinition intermediate = modelObject->optionalCast<InternalMassDefinition>();
      if (intermediate) {
        return setInternalMassDefinition(*intermediate);
      }
    }
    return false;
  }

} // detail

InternalMass::InternalMass(const InternalMassDefinition& internalMassDefinition)
  : SpaceLoadInstance(InternalMass::iddObjectType(),internalMassDefinition)
{
  OS_ASSERT(getImpl<detail::InternalMass_Impl>());
}

IddObjectType InternalMass::iddObjectType() {
  IddObjectType result(IddObjectType::OS_InternalMass);
  return result;
}

bool InternalMass::setMultiplier(double multiplier) {
  return getImpl<detail::InternalMass_Impl>()->setMultiplier(multiplier);
}

void InternalMass::resetMultiplier() {
  getImpl<detail::InternalMass_Impl>()->resetMultiplier();
}

InternalMassDefinition InternalMass::internalMassDefinition() const
{
  return getImpl<detail::InternalMass_Impl>()->internalMassDefinition();
}

bool InternalMass::setInternalMassDefinition(const InternalMassDefinition& internalMassDefinition)
{
  return getImpl<detail::InternalMass_Impl>()->setInternalMassDefinition(internalMassDefinition);
}

boost::optional<double> InternalMass::surfaceArea() const {
  return getImpl<detail::InternalMass_Impl>()->surfaceArea();
}

boost::optional<double> InternalMass::surfaceAreaPerFloorArea() const {
  return getImpl<detail::InternalMass_Impl>()->surfaceAreaPerFloorArea();
}

boost::optional<double> InternalMass::surfaceAreaPerPerson() const {
  return getImpl<detail::InternalMass_Impl>()->surfaceAreaPerPerson();
}

double InternalMass::getSurfaceArea(double floorArea, double numPeople) const {
  return getImpl<detail::InternalMass_Impl>()->getSurfaceArea(floorArea,numPeople);
}

double InternalMass::getSurfaceAreaPerFloorArea(double floorArea, double numPeople) const {
  return getImpl<detail::InternalMass_Impl>()->getSurfaceAreaPerFloorArea(floorArea,numPeople);
}

double InternalMass::getSurfaceAreaPerPerson(double floorArea, double numPeople) const {
  return getImpl<detail::InternalMass_Impl>()->getSurfaceAreaPerPerson(floorArea,numPeople);
}

/// @cond
InternalMass::InternalMass(std::shared_ptr<detail::InternalMass_Impl> impl)
  : SpaceLoadInstance(impl)
{}
/// @endcond


} // model
} // openstudio

