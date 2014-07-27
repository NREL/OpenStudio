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

#include "ThermalZonesView.hpp"

#include "ModelObjectListView.hpp"
#include "ThermalZonesGridView.hpp"

#include <QPainter>
#include <QStackedWidget>
#include <QStyleOption>
#include <QTimer>
#include <QVBoxLayout>

namespace openstudio {

ThermalZonesView::ThermalZonesView(bool isIP, const model::Model & model,
                                   QWidget * parent)
  : ModelSubTabView(new ModelObjectListView(IddObjectType::OS_ThermalZone,model,true,parent),
                    new ThermalZoneView(isIP,model,parent),
                    true,
                    parent)
{
}

ThermalZoneView::ThermalZoneView(bool isIP, const model::Model & model,
                                 QWidget * parent )
  : ModelObjectInspectorView(model,true,parent),
    m_isIP(isIP),
    m_dirty(false)
{
  bool isConnected = false;

  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), this, SLOT(onToggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  ThermalZonesGridView * thermalZonesGridView = new ThermalZonesGridView(this->m_isIP,this->m_model,this);
  this->stackedWidget()->addWidget(thermalZonesGridView);

  isConnected = connect(thermalZonesGridView, SIGNAL(dropZoneItemClicked(OSItem*)), this, SIGNAL(dropZoneItemClicked(OSItem*)));
  OS_ASSERT(isConnected);

  refresh();
}

void ThermalZoneView::onClearSelection()
{
}

void ThermalZoneView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  refresh();
}

void ThermalZoneView::onUpdate()
{
  refresh();
}

void ThermalZoneView::paintEvent ( QPaintEvent * event )
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void ThermalZoneView::refresh()
{
  m_dirty = true;

  QTimer::singleShot(0,this,SLOT(refreshNow()));
}

void ThermalZoneView::refreshNow()
{
}

void ThermalZoneView::onToggleUnitsClicked(bool isIP)
{
  m_isIP = isIP;
}

} // openstudio
