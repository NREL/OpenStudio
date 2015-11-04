/**********************************************************************
*  Copyright (c) 2008-2015, Alliance for Sustainable Energy.  
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

#include "ScheduleSetsController.hpp"
#include "ScheduleSetsView.hpp"

#include "../model/DefaultScheduleSet.hpp"
#include "../model/DefaultScheduleSet_Impl.hpp"

#include "../utilities/core/Logger.hpp"

#include <utilities/idd/IddEnums.hxx>

namespace openstudio {

ScheduleSetsController::ScheduleSetsController(const model::Model& model)
  : ModelSubTabController(new ScheduleSetsView(model), model)
{
}

void ScheduleSetsController::onAddObject(const openstudio::IddObjectType& iddObjectType)
{
  switch(iddObjectType.value()){
    case IddObjectType::OS_DefaultScheduleSet:
      openstudio::model::DefaultScheduleSet(this->model());
      break;
    default:
      LOG_FREE_AND_THROW("ScheduleSetsController", "Unknown IddObjectType '" << iddObjectType.valueName() << "'");
  }
}

void ScheduleSetsController::onCopyObject(const openstudio::model::ModelObject& modelObject)
{
  modelObject.clone(this->model());
}

void ScheduleSetsController::onRemoveObject(openstudio::model::ModelObject modelObject)
{
  modelObject.remove();
}

void ScheduleSetsController::onReplaceObject(openstudio::model::ModelObject modelObject, const OSItemId& replacementItemId)
{
  // not yet implemented
}

void ScheduleSetsController::onPurgeObjects(const openstudio::IddObjectType& iddObjectType)
{
  this->model().purgeUnusedResourceObjects(iddObjectType);
}

void ScheduleSetsController::onDrop(const OSItemId& itemId)
{
  boost::optional<model::ModelObject> modelObject = this->getModelObject(itemId);
  if(modelObject){
    if(modelObject->optionalCast<model::DefaultScheduleSet>()){
      if (this->fromComponentLibrary(itemId)){
        modelObject = modelObject->clone(this->model());
      }
    }
  }
}

void ScheduleSetsController::onInspectItem(OSItem* item)
{
}

} // openstudio

