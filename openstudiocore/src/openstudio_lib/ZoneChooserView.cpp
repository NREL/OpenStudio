/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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
  auto mainLayout = new QVBoxLayout();

  mainLayout->setContentsMargins(0,0,0,0);

  this->setLayout(mainLayout);

  auto scrollArea = new QScrollArea();
  
  scrollArea->setWidgetResizable(true);

  scrollArea->setFrameStyle(QFrame::NoFrame);

  mainLayout->addWidget(scrollArea);

  auto scrollWidget = new QWidget();

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
  while((child = m_vLayout->takeAt(0)) != nullptr) 
  {
      delete child->widget();
      delete child;
  }
  m_zoneChooserItems.clear();

  if( m_mixer )
  {
    auto label = new QLabel();
    label->setObjectName("IGHeader");
    label->setText(toQString(m_mixer->iddObject().name()));
    m_vLayout->addWidget(label);
  }
  else if( m_splitter )
  {
    auto label = new QLabel();
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

  for( auto it = zones.begin();
        it < zones.end();
        ++it )
  {
    if( ! it->isPlenum() )
    {
      auto zoneChooserItem = new ZoneChooserItem(*it,this);
      m_zoneChooserItems.push_back(zoneChooserItem);
      m_vLayout->addWidget(zoneChooserItem);
      zoneChooserItem->setChecked(false);
    }
  }
  m_vLayout->addStretch();

  std::vector<model::ThermalZone> thermalZones;
  thermalZones = m_splitter->thermalZones();
  for( auto it = thermalZones.begin();
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
  for( auto it = m_zoneChooserItems.begin();
       it < m_zoneChooserItems.end();
       ++it )
  {
    if( (*it)->zoneName() == zoneName )
    {
      return *it;
    }
  }
  return nullptr;
}

ZoneChooserItem::ZoneChooserItem(model::ThermalZone & zone, ZoneChooserView * parent)
  : QWidget(parent),
    m_zoneChooserView(parent),
    m_thermalZone(zone)
{
  auto hLayout = new QHBoxLayout();

  m_checkBox = new QCheckBox();
  
  m_checkBox->setText(toQString(zone.name().get()));

  connect(m_checkBox, &QCheckBox::clicked, this, &ZoneChooserItem::sendClickedSignal);
  connect(this, &ZoneChooserItem::addZoneClicked, m_zoneChooserView, &ZoneChooserView::addZoneClicked);
  connect(this, &ZoneChooserItem::removeZoneClicked, m_zoneChooserView, &ZoneChooserView::removeZoneClicked);
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

