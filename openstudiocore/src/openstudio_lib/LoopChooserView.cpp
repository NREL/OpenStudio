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

#include "LoopChooserView.hpp"
#include "../model/Model.hpp"
#include "../model/WaterToAirComponent.hpp"
#include "../model/WaterToAirComponent_Impl.hpp"
#include "../model/HVACComponent.hpp"
#include "../model/HVACComponent_Impl.hpp"
#include "../model/Loop.hpp"
#include "../model/Loop_Impl.hpp"
#include "../model/PlantLoop.hpp"
#include "../model/PlantLoop_Impl.hpp"
#include <QCheckBox>
#include <QHBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include <QLabel>
#include <QScrollArea>

namespace openstudio {

LoopChooserView::LoopChooserView(QWidget* parent)
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

void LoopChooserView::layoutModelObject(boost::optional<model::ModelObject> & modelObject)
{
  m_component = boost::none;

  if( modelObject )
  {
    if( boost::optional<model::HVACComponent> comp = modelObject->optionalCast<model::HVACComponent>() )
    {
      m_component = comp;
    }
  }

  layoutView();
}

void LoopChooserView::layoutView()
{
  this->setUpdatesEnabled(false);

  QLayoutItem * child;
  while((child = m_vLayout->takeAt(0)) != 0) 
  {
      delete child->widget();
      delete child;
  }
  m_loopChooserItems.clear();

  if( m_component )
  {
    QLabel * label = new QLabel();
    label->setObjectName("IGHeader");
    label->setText(toQString(m_component->iddObject().name()));
    m_vLayout->addWidget(label);

    std::vector<model::PlantLoop> loops;
    loops = m_component->model().getConcreteModelObjects<model::PlantLoop>();

    for( std::vector<model::PlantLoop>::iterator it = loops.begin();
          it < loops.end();
          ++it )
    {
      LoopChooserItem * loopChooserItem = new LoopChooserItem(*it,this);
      m_loopChooserItems.push_back(loopChooserItem);
      m_vLayout->addWidget(loopChooserItem);
      loopChooserItem->setChecked(false);
    }
    m_vLayout->addStretch();

    boost::optional<model::PlantLoop> plantLoop;

    plantLoop = m_component->plantLoop();

    if( plantLoop )
    {
      loopChooserItemForLoop(plantLoop->name().get())->setChecked(true);
    }
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

void LoopChooserView::paintEvent ( QPaintEvent * event )
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

LoopChooserItem * LoopChooserView::loopChooserItemForLoop(std::string loopName)
{
  for( std::vector<LoopChooserItem *>::iterator it = m_loopChooserItems.begin();
       it < m_loopChooserItems.end();
       ++it )
  {
    if( (*it)->loopName() == loopName )
    {
      return *it;
    }
  }
  return NULL;
}

void LoopChooserView::onAddToLoopClicked(model::Loop & loop)
{
  emit addToLoopClicked(loop,m_component);
}

void LoopChooserView::onRemoveFromLoopClicked(model::Loop & loop)
{
  emit removeFromLoopClicked(loop,m_component);
}

LoopChooserItem::LoopChooserItem(model::Loop & loop, LoopChooserView * parent)
  : QWidget(parent),
    m_loopChooserView(parent),
    m_loop(loop)
{
  QHBoxLayout * hLayout = new QHBoxLayout();

  m_checkBox = new QCheckBox();
  
  m_checkBox->setText(toQString(loop.name().get()));

  connect( m_checkBox, SIGNAL(clicked(bool)), this, SLOT(sendClickedSignal(bool)) );
  connect( this, SIGNAL(addToLoopClicked(model::Loop &)), m_loopChooserView,SLOT(onAddToLoopClicked(model::Loop &)) );
  connect( this, SIGNAL(removeFromLoopClicked(model::Loop &)), m_loopChooserView,SLOT(onRemoveFromLoopClicked(model::Loop &)) );

  hLayout->addWidget(m_checkBox);

  setLayout(hLayout);
}

void LoopChooserItem::sendClickedSignal( bool checked )
{
  if( checked )
  {
    emit addToLoopClicked(m_loop);

    m_loopChooserView->layoutView();
  }
  else
  {
    emit removeFromLoopClicked(m_loop);

    m_loopChooserView->layoutView();
  }
}

std::string LoopChooserItem::loopName()
{
  return toString(m_checkBox->text());
}

void LoopChooserItem::setChecked( bool checked )
{
  m_checkBox->setChecked( checked );
}

} // openstudio

