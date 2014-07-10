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

#include "StandardOpaqueMaterialInspectorView.hpp"

#include "../model/Model_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <QStyleOption>
#include <QPainter>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QScrollArea>
#include <sstream>

namespace openstudio {

StandardOpaqueMaterialInspectorView::StandardOpaqueMaterialInspectorView(const model::Model& model,
                                                                         QWidget * parent )
  : ModelObjectInspectorView(model, true, parent)
{
  QWidget* hiddenWidget = new QWidget();
  this->stackedWidget()->insertWidget(0, hiddenWidget);

  QWidget* visibleWidget = new QWidget();
  this->stackedWidget()->insertWidget(1, visibleWidget);

  this->stackedWidget()->setCurrentIndex(0);

  QVBoxLayout * mainVLayout = new QVBoxLayout();
  mainVLayout->setContentsMargins(0,0,0,1);
  mainVLayout->setSpacing(0);
  visibleWidget->setLayout(mainVLayout);

  m_mainLabel = new QLabel(this);
  m_mainLabel->setWordWrap(true);
  m_mainLabel->setText("StandardOpaqueMaterialInspectorView: Initial");
  m_mainLabel->setAlignment(Qt::AlignHCenter);
  m_mainLabel->setAlignment(Qt::AlignVCenter);
  mainVLayout->addWidget(m_mainLabel);
}

void StandardOpaqueMaterialInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  this->stackedWidget()->setCurrentIndex(0);
}

void StandardOpaqueMaterialInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  std::stringstream ss;
  ss << "StandardOpaqueMaterialInspectorView:" << std::endl;
  ss << modelObject;
  m_mainLabel->setText(toQString(ss.str()));

  this->stackedWidget()->setCurrentIndex(1);
}

void StandardOpaqueMaterialInspectorView::onUpdate()
{
  boost::optional<openstudio::model::ModelObject> modelObject = this->modelObject();
  OS_ASSERT(modelObject);

  std::stringstream ss;
  ss << "StandardOpaqueMaterialInspectorView:" << std::endl;
  ss << *modelObject;
  m_mainLabel->setText(toQString(ss.str()));
}



} // openstudio
