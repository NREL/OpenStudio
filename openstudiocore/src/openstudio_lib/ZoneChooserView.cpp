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

#include "ZoneChooserView.hpp"
#include "../model/AirLoopHVAC.hpp"
#include "../model/Model.hpp"
#include "../model/ThermalZone.hpp"
#include "../model/ThermalZone_Impl.hpp"
#include "../model/AirLoopHVACZoneSplitter.hpp"
#include "../model/AirLoopHVACZoneSplitter_Impl.hpp"
#include "../model/AirLoopHVACZoneMixer.hpp"
#include "../model/AirLoopHVACZoneMixer_Impl.hpp"
#include <QCheckBox>
#include <QHBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include <QLabel>
#include <QScrollArea>

namespace openstudio {

ZoneChooserView::ZoneChooserView(QWidget* parent)
  : QWidget(parent)
{
  QVBoxLayout * mainLayout = new QVBoxLayout();

  mainLayout->setContentsMargins(0,0,0,0);

  this->setLayout(mainLayout);

  QScrollArea * scrollArea = new QScrollArea();
  
  scrollArea->setWidgetResizable(true);

  scrollArea->setFrameStyle(QFrame::NoFrame);

  mainLayout->addWidget(scrollArea);

  QWidget * scrollWidget = new QWidget();

  scrollArea->setWidget(scrollWidget);

  m_vLayout = new QVBoxLayout();

  m_vLayout->setContentsMargins(0,0,0,0);

  scrollWidget->setLayout(m_vLayout);
}

void ZoneChooserView::layoutModelObject(openstudio::model::ModelObject & modelObject)
{
  if( model::OptionalAirLoopHVACZoneSplitter splitter = modelObject.optionalCast<model::AirLoopHVACZoneSplitter>() )
  {
    m_splitter = splitter;
  }
  else if( model::OptionalAirLoopHVACZoneMixer mixer = modelObject.optionalCast<model::AirLoopHVACZoneMixer>() )
  {
    m_mixer = mixer;
  }
  else
  {
    m_splitter = model::OptionalAirLoopHVACZoneSplitter();
    m_mixer = model::OptionalAirLoopHVACZoneMixer();
  }

  layoutView();
}

void ZoneChooserView::layoutView()
{
  this->setUpdatesEnabled(false);

  QLayoutItem * child;
  while((child = m_vLayout->takeAt(0)) != 0) 
  {
      delete child->widget();
      delete child;
  }
  m_zoneChooserItems.clear();

  if( m_mixer )
  {
    QLabel * label = new QLabel();
    label->setObjectName("IGHeader");
    label->setText(toQString(m_mixer->iddObject().name()));
    m_vLayout->addWidget(label);
  }
  else if( m_splitter )
  {
    QLabel * label = new QLabel();
    label->setObjectName("IGHeader");
    label->setText(toQString(m_splitter->iddObject().name()));
    m_vLayout->addWidget(label);
  }

  if( m_mixer )
  {
    if( model::OptionalAirLoopHVAC airLoop = m_mixer->airLoopHVAC() )
    {
      m_splitter = airLoop->zoneSplitter();
    }
    else
    {
      return;
    }
  }
  else if( ! m_splitter )
  {
    return;
  }

  std::vector<model::ThermalZone> zones;
  zones = m_splitter->model().getConcreteModelObjects<model::ThermalZone>();
  std::sort(zones.begin(),zones.end(),WorkspaceObjectNameLess());

  for( std::vector<model::ThermalZone>::iterator it = zones.begin();
        it < zones.end();
        ++it )
  {
    if( ! it->isPlenum() )
    {
      ZoneChooserItem * zoneChooserItem = new ZoneChooserItem(*it,this);
      m_zoneChooserItems.push_back(zoneChooserItem);
      m_vLayout->addWidget(zoneChooserItem);
      zoneChooserItem->setChecked(false);
    }
  }
  m_vLayout->addStretch();

  std::vector<model::ThermalZone> thermalZones;
  thermalZones = m_splitter->thermalZones();
  for( std::vector<model::ThermalZone>::iterator it = thermalZones.begin();
        it < thermalZones.end();
        ++it )
  {
    zoneChooserItemForZone(it->name().get())->setChecked(true);
  }
  
  for(int i = 0; i < m_vLayout->count(); i++) 
  {
    if( QWidget * widget = m_vLayout->itemAt(i)->widget() )
    {
      widget->show();
    }
  }

  this->setUpdatesEnabled(true);

}

void ZoneChooserView::paintEvent ( QPaintEvent * event )
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

ZoneChooserItem * ZoneChooserView::zoneChooserItemForZone(std::string zoneName)
{
  for( std::vector<ZoneChooserItem *>::iterator it = m_zoneChooserItems.begin();
       it < m_zoneChooserItems.end();
       ++it )
  {
    if( (*it)->zoneName() == zoneName )
    {
      return *it;
    }
  }
  return NULL;
}

ZoneChooserItem::ZoneChooserItem(model::ThermalZone & zone, ZoneChooserView * parent)
  : QWidget(parent),
    m_zoneChooserView(parent),
    m_thermalZone(zone)
{
  QHBoxLayout * hLayout = new QHBoxLayout();

  m_checkBox = new QCheckBox();
  
  m_checkBox->setText(toQString(zone.name().get()));

  connect( m_checkBox, SIGNAL(clicked(bool)), this, SLOT(sendClickedSignal(bool)) );
  connect( this, SIGNAL(addZoneClicked(model::ThermalZone &)), m_zoneChooserView,SIGNAL(addZoneClicked(model::ThermalZone &)) );
  connect( this, SIGNAL(removeZoneClicked(model::ThermalZone &)), m_zoneChooserView,SIGNAL(removeZoneClicked(model::ThermalZone &)) );

  hLayout->addWidget(m_checkBox);

  setLayout(hLayout);

}

void ZoneChooserItem::sendClickedSignal( bool checked )
{
  if( checked )
  {
    emit addZoneClicked(m_thermalZone);
    m_zoneChooserView->layoutView();
  }
  else
  {
    emit removeZoneClicked(m_thermalZone);
    m_zoneChooserView->layoutView();
  }
}

std::string ZoneChooserItem::zoneName()
{
  return toString(m_checkBox->text());
}

void ZoneChooserItem::setChecked( bool checked )
{
  m_checkBox->setChecked( checked );
}

} // openstudio

