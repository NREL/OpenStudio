/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#include "SpaceLoadInstance.hpp"
#include "SpaceLoadInstance_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "SpaceLoadDefinition.hpp"
#include "SpaceLoadDefinition_Impl.hpp"
#include "Space.hpp"
#include "SpaceType.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  SpaceLoadInstance_Impl::SpaceLoadInstance_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : SpaceLoad_Impl(idfObject, model, keepHandle)
  {}

  SpaceLoadInstance_Impl::SpaceLoadInstance_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : SpaceLoad_Impl(other, model, keepHandle)
  {}

  SpaceLoadInstance_Impl::SpaceLoadInstance_Impl(const SpaceLoadInstance_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : SpaceLoad_Impl(other, model, keepHandle)
  {}

  SpaceLoadDefinition SpaceLoadInstance_Impl::definition() const
  {
    OptionalSpaceLoadDefinition result = getObject<ModelObject>().getModelObjectTarget<SpaceLoadDefinition>(this->definitionIndex());
    if(!result){
      LOG_AND_THROW("SpaceLoadInstance does not have a definition." << std::endl << this->idfObject());
    }
    return result.get();
  }

  void SpaceLoadInstance_Impl::makeUnique()
  {
    SpaceLoadDefinition definition = this->definition();
    SpaceLoadInstanceVector instances = definition.instances();
    OS_ASSERT(!instances.empty());
    if (instances.size() == 1){
      OS_ASSERT(this->handle() == instances[0].handle());
    }else{
      ModelObject clone = definition.clone(this->model());
      OS_ASSERT(this->setPointer(this->definitionIndex(), clone.handle()));
    }
  }

  double SpaceLoadInstance_Impl::floorArea() const
  {
    double result = 0;

    boost::optional<Space> space = this->space();
    boost::optional<SpaceType> spaceType = this->spaceType();
    if (space){
      result = this->multiplier()*space->multiplier()*space->floorArea();
    }else if (spaceType){
      result = this->multiplier()*spaceType->floorArea();
    }

    return result;
  }

  int SpaceLoadInstance_Impl::quantity() const
  {
    int result = 0;

    boost::optional<Space> space = this->space();
    boost::optional<SpaceType> spaceType = this->spaceType();
    if (space){
      result = this->multiplier()*space->multiplier();
    }else if (spaceType){
      for (const Space& space : spaceType->spaces()){
        result += this->multiplier()*space.multiplier();
      }
    }

    return result;
  }

} // detail

SpaceLoadInstance::SpaceLoadInstance(IddObjectType type,const SpaceLoadDefinition& definition)
  : SpaceLoad(type,definition.model())
{
  OS_ASSERT(getImpl<detail::SpaceLoadInstance_Impl>());
  OS_ASSERT(this->setPointer(getImpl<detail::SpaceLoadInstance_Impl>()->definitionIndex(), definition.handle()));
}

SpaceLoadInstance::SpaceLoadInstance(std::shared_ptr<detail::SpaceLoadInstance_Impl> impl)
  : SpaceLoad(impl)
{}

SpaceLoadDefinition SpaceLoadInstance::definition() const
{
  return getImpl<detail::SpaceLoadInstance_Impl>()->definition();
}

bool SpaceLoadInstance::setDefinition(const SpaceLoadDefinition& definition)
{
  return getImpl<detail::SpaceLoadInstance_Impl>()->setDefinition(definition);
}

void SpaceLoadInstance::makeUnique()
{
  getImpl<detail::SpaceLoadInstance_Impl>()->makeUnique();
}

double SpaceLoadInstance::multiplier() const
{
  return getImpl<detail::SpaceLoadInstance_Impl>()->multiplier();
}

bool SpaceLoadInstance::isMultiplierDefaulted() const
{
  return getImpl<detail::SpaceLoadInstance_Impl>()->isMultiplierDefaulted();
}

double SpaceLoadInstance::floorArea() const
{
  return getImpl<detail::SpaceLoadInstance_Impl>()->floorArea();
}

int SpaceLoadInstance::quantity() const
{
  return getImpl<detail::SpaceLoadInstance_Impl>()->quantity();
}

} // model
} // openstudio
