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

#include "SpaceTypeInspectorView.hpp"

#include "ModelObjectListView.hpp"
#include "SpaceTypesGridView.hpp"

#include <QPainter>
#include <QStackedWidget>
#include <QStyleOption>
#include <QTimer>
#include <QVBoxLayout>

namespace openstudio {

  // SpaceTypeInspectorView

  SpaceTypeInspectorView::SpaceTypeInspectorView(bool isIP,
    const model::Model& model,
    QWidget * parent)
    : ModelObjectInspectorView(model, true, parent),
    m_isIP(isIP)
  {
    bool isConnected = false;

    m_gridView = new SpaceTypesGridView(this->m_isIP, this->m_model, this);
    stackedWidget()->addWidget(m_gridView);

    isConnected = connect(m_gridView, SIGNAL(dropZoneItemClicked(OSItem*)), this, SIGNAL(dropZoneItemClicked(OSItem*)));
    OS_ASSERT(isConnected);

    isConnected = connect(this, SIGNAL(selectionCleared()), m_gridView, SIGNAL(selectionCleared()));
    OS_ASSERT(isConnected);

    isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), this, SLOT(toggleUnits(bool)));
    OS_ASSERT(isConnected);

    isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_gridView, SIGNAL(toggleUnitsClicked(bool)));
    OS_ASSERT(isConnected);
  }

  std::vector<model::ModelObject> SpaceTypeInspectorView::selectedObjects() const
  {
    return m_gridView->selectedObjects();
  }

  void SpaceTypeInspectorView::onClearSelection()
  {}

  void SpaceTypeInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
  {}

  void SpaceTypeInspectorView::onUpdate()
  {}

  void SpaceTypeInspectorView::refresh()
  {}

  void SpaceTypeInspectorView::toggleUnits(bool displayIP)
  {
    m_isIP = displayIP;
  }

} // openstudio
