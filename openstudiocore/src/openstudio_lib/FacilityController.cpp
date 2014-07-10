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

#include "FacilityController.hpp"
#include "FacilityView.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {

FacilityController::FacilityController(bool isIP, const model::Model& model)
  : ModelSubTabController(new FacilityView(isIP, model), model)
{
  bool isConnected = connect(this,SIGNAL(toggleUnitsClicked(bool)),
                             subTabView(),SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);
}

FacilityController::~FacilityController()
{
}

void FacilityController::onAddObject(const openstudio::IddObjectType& iddObjectType)
{
}

void FacilityController::onCopyObject(const openstudio::model::ModelObject& modelObject)
{
}

void FacilityController::onRemoveObject(openstudio::model::ModelObject modelObject)
{
}

void FacilityController::onReplaceObject(openstudio::model::ModelObject modelObject, const OSItemId& replacementItemId)
{
}

void FacilityController::onPurgeObjects(const openstudio::IddObjectType& iddObjectType)
{
}

void FacilityController::onDrop(const OSItemId& itemId)
{
}

void FacilityController::onInspectItem(OSItem* item)
{
}

void FacilityController::toggleUnits(bool displayIP)
{
}

} // openstudio

