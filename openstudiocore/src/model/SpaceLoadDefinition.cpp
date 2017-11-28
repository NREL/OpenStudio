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

#include "SpaceLoadDefinition.hpp"
#include "SpaceLoadDefinition_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "SpaceLoadInstance.hpp"
#include "SpaceLoadInstance_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  SpaceLoadDefinition_Impl::SpaceLoadDefinition_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ResourceObject_Impl(idfObject, model, keepHandle)
  {}

  SpaceLoadDefinition_Impl::SpaceLoadDefinition_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : ResourceObject_Impl(other, model, keepHandle)
  {}

  SpaceLoadDefinition_Impl::SpaceLoadDefinition_Impl(const SpaceLoadDefinition_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : ResourceObject_Impl(other, model, keepHandle)
  {}

  std::vector<openstudio::IdfObject> SpaceLoadDefinition_Impl::remove()
  {
    std::vector<openstudio::IdfObject> temp1;
    std::vector<SpaceLoadInstance> instances = this->instances();
    for (SpaceLoadInstance instance : instances){
      std::vector<openstudio::IdfObject> temp2 = instance.remove();
      temp1.insert(temp1.end(), temp2.begin(), temp2.end());
    }

    std::vector<openstudio::IdfObject> result = ResourceObject_Impl::remove();
    result.insert(result.end(), temp1.begin(), temp1.end());

    return result;
  }

  std::vector<SpaceLoadInstance> SpaceLoadDefinition_Impl::instances() const
  {
    return getObject<ModelObject>().getModelObjectSources<SpaceLoadInstance>();
  }

  double SpaceLoadDefinition_Impl::floorArea() const
  {
    double result = 0;
    for (const SpaceLoadInstance& instance : this->instances()){
      result += instance.floorArea();
    }
    return result;
  }

  int SpaceLoadDefinition_Impl::quantity() const
  {
    int result = 0;
    for (const SpaceLoadInstance& instance : this->instances()){
      result += instance.quantity();
    }
    return result;
  }

} // detail

SpaceLoadDefinition::SpaceLoadDefinition(IddObjectType type,const Model& model)
  : ResourceObject(type,model)
{
  OS_ASSERT(getImpl<detail::SpaceLoadDefinition_Impl>());
}

SpaceLoadDefinition::SpaceLoadDefinition(std::shared_ptr<detail::SpaceLoadDefinition_Impl> impl)
  : ResourceObject(std::move(impl))
{}

std::vector<SpaceLoadInstance> SpaceLoadDefinition::instances() const
{
  return getImpl<detail::SpaceLoadDefinition_Impl>()->instances();
}

double SpaceLoadDefinition::floorArea() const
{
  return getImpl<detail::SpaceLoadDefinition_Impl>()->floorArea();
}

int SpaceLoadDefinition::quantity() const
{
  return getImpl<detail::SpaceLoadDefinition_Impl>()->quantity();
}

} // model
} // openstudio
