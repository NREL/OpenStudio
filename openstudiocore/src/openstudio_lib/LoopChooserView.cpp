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
  while((child = m_vLayout->takeAt(0)) != nullptr) 
  {
      delete child->widget();
      delete child;
  }
  m_loopChooserItems.clear();

  if( m_component )
  {
    auto label = new QLabel();
    label->setObjectName("IGHeader");
    label->setText(toQString(m_component->iddObject().name()));
    m_vLayout->addWidget(label);

    std::vector<model::PlantLoop> loops;
    loops = m_component->model().getConcreteModelObjects<model::PlantLoop>();

    for( auto it = loops.begin();
          it < loops.end();
          ++it )
    {
      auto loopChooserItem = new LoopChooserItem(*it,this);
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
  for( auto it = m_loopChooserItems.begin();
       it < m_loopChooserItems.end();
       ++it )
  {
    if( (*it)->loopName() == loopName )
    {
      return *it;
    }
  }
  return nullptr;
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
  auto hLayout = new QHBoxLayout();

  m_checkBox = new QCheckBox();
  
  m_checkBox->setText(toQString(loop.name().get()));

  connect(m_checkBox, &QCheckBox::clicked, this, &LoopChooserItem::sendClickedSignal);
  connect(this, &LoopChooserItem::addToLoopClicked, m_loopChooserView, &LoopChooserView::onAddToLoopClicked);
  connect(this, &LoopChooserItem::removeFromLoopClicked, m_loopChooserView, &LoopChooserView::onRemoveFromLoopClicked);
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

