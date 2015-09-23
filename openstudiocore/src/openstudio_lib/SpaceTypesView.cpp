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

#include "SpaceTypesView.hpp"

#include "ModelObjectListView.hpp"
#include "OSItem.hpp"
#include "SpaceTypeInspectorView.hpp"

#include "../openstudio_lib/OSItem.hpp"

#include "../model/Model_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <QLabel>
#include <QPainter>
#include <QPushButton>
#include <QScrollArea>
#include <QStackedWidget>
#include <QStyleOption>
#include <QVBoxLayout>

#include <sstream>

#include <utilities/idd/IddEnums.hxx>


namespace openstudio {


  SpaceTypesView::SpaceTypesView(bool isIP,
    const openstudio::model::Model& model,
    QWidget * parent)
  : ModelSubTabView(new ModelObjectListView(IddObjectType::OS_SpaceType, model, true, parent),
                    new SpaceTypeInspectorView(isIP, model, parent),
                    true, // Note: "true" creates a GridView SubTabView
                    parent)
{
  ModelObjectListView* modelObjectListView = qobject_cast<ModelObjectListView*>(this->itemSelector());
  OS_ASSERT(modelObjectListView);
  modelObjectListView->setItemsDraggable(false);

  bool isConnected = false;

  isConnected = connect(itemSelector(), SIGNAL(selectionCleared()), inspectorView(), SIGNAL(selectionCleared()));
  OS_ASSERT(isConnected);

  isConnected = connect(inspectorView(), SIGNAL(dropZoneItemClicked(OSItem*)), this, SIGNAL(dropZoneItemClicked(OSItem*)));
  OS_ASSERT(isConnected);
}

} // openstudio

