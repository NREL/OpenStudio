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

#include "ThermalZonesTabController.hpp"

#include "OSItem.hpp"
#include "OSItemSelectorButtons.hpp"
#include "ThermalZonesController.hpp"
#include "ThermalZonesTabView.hpp"
#include "ThermalZonesView.hpp"
#include "ThermalZonesGridView.hpp"
#include "../model/ThermalZone.hpp"

namespace openstudio {

ThermalZonesTabController::ThermalZonesTabController(bool isIP, const model::Model& model)
  : MainTabController(new ThermalZonesTabView()),
    m_model(model)
{
  m_thermalZonesView = new ThermalZonesView(isIP,model);
  
  bool bingo;
  bingo = connect(m_thermalZonesView->selectorButtons,&OSItemSelectorButtons::addClicked,this,&ThermalZonesTabController::addObject);
  OS_ASSERT(bingo);

  bingo = connect(m_thermalZonesView->selectorButtons,&OSItemSelectorButtons::removeClicked,this,&ThermalZonesTabController::removeSelectedObjects);
  OS_ASSERT(bingo);

  mainContentWidget()->addTabWidget(m_thermalZonesView);
}

//void ThermalZonesController::onSelectItem(OSItem *item)
//{
//  subTabView()->inspectorView()->selectItem(item);
//  subTabView()->itemSelectorButtons()->enableCopyButton();
//  subTabView()->itemSelectorButtons()->enableRemoveButton();
//  subTabView()->itemSelectorButtons()->enablePurgeButton();
//}

void ThermalZonesTabController::addObject()
{
  openstudio::model::ThermalZone tz(m_model);
}

//void ThermalZonesController::onCopyObject(const openstudio::model::ModelObject& modelObject)
//{
//  //DLM: ThermalZone::clone is not implemented yet
//  //modelObject.clone(this->model());
//}

void ThermalZonesTabController::removeSelectedObjects()
{
  for( auto & selectedObject : m_thermalZonesView->gridView->selectedObjects() ) {
    selectedObject.remove();
  }
}

//void ThermalZonesController::onReplaceObject(openstudio::model::ModelObject modelObject, const OSItemId& replacementItemId)
//{
//  // not yet implemented
//}
//
//void ThermalZonesController::onPurgeObjects(const openstudio::IddObjectType& iddObjectType)
//{
//  //std::vector<Handle> toRemove;
//  for (model::ThermalZone thermalZone : this->model().getConcreteModelObjects<model::ThermalZone>()){
//    if (thermalZone.spaces().empty() && thermalZone.isRemovable()){
//      //toRemove.push_back(thermalZone.handle());
//
//      // DLM: just remove it
//      thermalZone.remove();
//    }
//  }
//  
//  // DLM: this method doesn't actually call ThermalZone::remove which is important to preserve model validity
//  //this->model().removeObjects(toRemove);
//}
//
//void ThermalZonesController::onDrop(const OSItemId& itemId)
//{
//  boost::optional<model::ModelObject> modelObject = this->getModelObject(itemId);
//  if(modelObject){
//    if(modelObject->optionalCast<model::ThermalZone>()){
//      if (this->fromComponentLibrary(itemId)){
//        //DLM: ThermalZone::clone is not implemented yet
//        //modelObject = modelObject->clone(this->model());
//      }
//    }
//  }
//}
//
//void ThermalZonesController::onInspectItem(OSItem* item)
//{
//}

} // openstudio

