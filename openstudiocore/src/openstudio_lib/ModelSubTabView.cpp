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

#include "ModelSubTabView.hpp"

#include "ModelObjectInspectorView.hpp"
#include "ModelObjectItem.hpp"
#include "ModelObjectListView.hpp"
#include "ModelObjectTypeListView.hpp"
#include "OSAppBase.hpp"
#include "OSDocument.hpp"
#include "OSItemSelector.hpp"
#include "OSItemSelectorButtons.hpp"

#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <iostream>

#include <QButtonGroup>
#include <QCalendarWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGridLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>
#include <QRadioButton>
#include <QResizeEvent>
#include <QStackedWidget>
#include <QStyleOption>
#include <QTimer>
#include <QVBoxLayout>

namespace openstudio {

ModelSubTabView::ModelSubTabView(OSItemSelector* itemSelector,
  ModelObjectInspectorView* modelObjectInspectorView,
  bool showGridViewLayout,
  QWidget * parent)
  : SubTabView(itemSelector, modelObjectInspectorView, showGridViewLayout, parent),
    m_modelObjectInspectorView(modelObjectInspectorView)
{
  connect(this, &ModelSubTabView::dropZoneItemClicked, this, &ModelSubTabView::onDropZoneItemClicked);
  connect(modelObjectInspectorView, &ModelObjectInspectorView::dropZoneItemClicked, this, &ModelSubTabView::dropZoneItemClicked);
}

ModelObjectInspectorView* ModelSubTabView::modelObjectInspectorView()
{
  return m_modelObjectInspectorView;
}

void ModelSubTabView::onDropZoneItemClicked(OSItem* item)
{ 
  std::shared_ptr<OSDocument> currentDocument = OSAppBase::instance()->currentDocument();
  if (currentDocument){
    if (!item) {
      emit dropZoneItemSelected(item, false);
    }
    else {
      // Note: perhaps passing this here offers more flexibility in the future when determining readOnly
      emit dropZoneItemSelected(item, item->itemId().isDefaulted());
    }
  }
}

} // openstudio
