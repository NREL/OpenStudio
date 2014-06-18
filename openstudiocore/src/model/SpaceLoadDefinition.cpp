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
  : ResourceObject(impl)
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
