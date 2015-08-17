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

#include "ThermalZonesView.hpp"

#include "ModelObjectListView.hpp"
#include "ThermalZonesGridView.hpp"

#include <QPainter>
#include <QStackedWidget>
#include <QStyleOption>
#include <QTimer>
#include <QVBoxLayout>

#include <utilities/idd/IddEnums.hxx>

namespace openstudio {

ThermalZonesView::ThermalZonesView(bool isIP,
  const model::Model & model,
  QWidget * parent)
  : ModelSubTabView(new ModelObjectListView(IddObjectType::OS_ThermalZone,model,true,parent),
                    new ThermalZoneView(isIP,model,parent),
                    true,
                    parent)
{
  bool isConnected = false;

  isConnected = connect(itemSelector(), SIGNAL(selectionCleared()), inspectorView(), SIGNAL(selectionCleared()));
  OS_ASSERT(isConnected);

  isConnected = connect(inspectorView(), SIGNAL(dropZoneItemClicked(OSItem*)), this, SIGNAL(dropZoneItemClicked(OSItem*)));
  OS_ASSERT(isConnected);
}

ThermalZoneView::ThermalZoneView(bool isIP,
  const model::Model & model,
  QWidget * parent )
  : ModelObjectInspectorView(model,true,parent),
    m_isIP(isIP)
{
  bool isConnected = false;

  m_thermalZonesGridView = new ThermalZonesGridView(this->m_isIP,this->m_model,this);
  this->stackedWidget()->addWidget(m_thermalZonesGridView);

  isConnected = connect(m_thermalZonesGridView, SIGNAL(dropZoneItemClicked(OSItem*)), this, SIGNAL(dropZoneItemClicked(OSItem*)));
  OS_ASSERT(isConnected);

  isConnected = connect(this, SIGNAL(selectionCleared()), m_thermalZonesGridView, SIGNAL(selectionCleared()));
  OS_ASSERT(isConnected);

  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), this, SLOT(toggleUnits(bool)));
  OS_ASSERT(isConnected);

  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_thermalZonesGridView, SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  refresh();
}

std::vector<model::ModelObject> ThermalZoneView::selectedObjects() const
{
  return m_thermalZonesGridView->selectedObjects();
}

void ThermalZoneView::onClearSelection()
{}

void ThermalZoneView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{}

void ThermalZoneView::onUpdate()
{}

void ThermalZoneView::refresh()
{}

void ThermalZoneView::toggleUnits(bool isIP)
{
  m_isIP = isIP;
}

} // openstudio
