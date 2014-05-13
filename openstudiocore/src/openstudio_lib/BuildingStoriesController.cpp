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

#include <openstudio_lib/BuildingStoriesController.hpp>
#include <openstudio_lib/BuildingStoriesView.hpp>

#include <model/Model.hpp>
#include <model/BuildingStory.hpp>
#include <model/BuildingStory_Impl.hpp>
#include <model/Space.hpp>

#include <utilities/core/Assert.hpp>

namespace openstudio {

BuildingStoriesController::BuildingStoriesController(const model::Model& model)
  : ModelSubTabController(new BuildingStoriesView(model), model)
{
}

void BuildingStoriesController::onAddObject(const openstudio::IddObjectType& iddObjectType)
{
  OS_ASSERT(IddObjectType::OS_BuildingStory == iddObjectType.value());
  openstudio::model::BuildingStory(this->model());
}

void BuildingStoriesController::onCopyObject(const openstudio::model::ModelObject& modelObject)
{
  modelObject.clone(this->model());
}

void BuildingStoriesController::onRemoveObject(openstudio::model::ModelObject modelObject)
{
  modelObject.remove();
}

void BuildingStoriesController::onReplaceObject(openstudio::model::ModelObject modelObject, const OSItemId& replacementItemId)
{
  // not yet implemented
}

void BuildingStoriesController::onPurgeObjects(const openstudio::IddObjectType& iddObjectType)
{
  for (model::BuildingStory buildingStory : this->model().getModelObjects<model::BuildingStory>()){
    if (buildingStory.spaces().empty()){
      buildingStory.remove();
    }
  }
}

void BuildingStoriesController::onDrop(const OSItemId& itemId)
{
  boost::optional<model::ModelObject> modelObject = this->getModelObject(itemId);
  if (modelObject){
    if (modelObject->optionalCast<model::BuildingStory>()){
      if (this->fromComponentLibrary(itemId)){
        modelObject = modelObject->clone(this->model());
      }
    }
  }
}

void BuildingStoriesController::onInspectItem(OSItem* item)
{
}

} // openstudio

