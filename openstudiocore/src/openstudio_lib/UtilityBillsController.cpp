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
#include <model/Model_Impl.hpp>

#include <model/UtilityBill.hpp>
#include <model/UtilityBill_Impl.hpp>

namespace openstudio {

UtilityBillsController::UtilityBillsController(const model::Model& model)
  : ModelSubTabController(new UtilityBillsView(model), model)
{
}

void UtilityBillsController::onAddObject(const openstudio::IddObjectType& iddObjectType)
{
  //BOOST_ASSERT(IddObjectType::OS_BuildingStory == iddObjectType.value());
  //openstudio::model::BuildingStory(this->model());

   Q_ASSERT(IddObjectType::OS_UtilityBill == iddObjectType.value());
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
}

void UtilityBillsController::onPurgeObjects(const openstudio::IddObjectType& iddObjectType)
{
  Q_FOREACH(model::UtilityBill utilityBill, this->model().getModelObjects<model::UtilityBill>()){
    utilityBill.remove();
  }

}

void UtilityBillsController::onDrop(const OSItemId& itemId)
{
}

void UtilityBillsController::onInspectItem(OSItem* item)
{
}

} // openstudio

