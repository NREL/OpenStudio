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

#include <openstudio_lib/UtilityBillsController.hpp>

#include <openstudio_lib/UtilityBillsView.hpp>

#include <model/Model.hpp>
//
//#include <model/BuildingStory.hpp>
//#include <model/BuildingStory_Impl.hpp>
//#include <model/Space.hpp>

namespace openstudio {

UtilityBillsController::UtilityBillsController(const model::Model& model)
  : ModelSubTabController(new UtilityBillsView(model), model)
{
}

void UtilityBillsController::onAddObject(const openstudio::IddObjectType& iddObjectType)
{
  //BOOST_ASSERT(IddObjectType::OS_BuildingStory == iddObjectType.value());
  //openstudio::model::BuildingStory(this->model());
}

void UtilityBillsController::onCopyObject(const openstudio::model::ModelObject& modelObject)
{
  modelObject.clone(this->model());
}

void UtilityBillsController::onRemoveObject(openstudio::model::ModelObject modelObject)
{
  modelObject.remove();
}

void UtilityBillsController::onReplaceObject(openstudio::model::ModelObject modelObject, const OSItemId& replacementItemId)
{
  // not yet implemented
}

void UtilityBillsController::onPurgeObjects(const openstudio::IddObjectType& iddObjectType)
{
  //BOOST_FOREACH(model::BuildingStory buildingStory, this->model().getModelObjects<model::BuildingStory>()){
  //  if (buildingStory.spaces().empty()){
  //    buildingStory.remove();
  //  }
  //}
}

void UtilityBillsController::onDrop(const OSItemId& itemId)
{
  //boost::optional<model::ModelObject> modelObject = this->getModelObject(itemId);
  //if (modelObject){
  //  if (modelObject->optionalCast<model::BuildingStory>()){
  //    if (this->fromComponentLibrary(itemId)){
  //      modelObject = modelObject->clone(this->model());
  //    }
  //  }
  //}
}

void UtilityBillsController::onInspectItem(OSItem* item)
{
}

} // openstudio

