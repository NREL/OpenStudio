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
  : SpaceLoad(std::move(impl))
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
