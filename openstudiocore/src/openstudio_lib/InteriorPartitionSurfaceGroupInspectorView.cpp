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

#include "InteriorPartitionSurfaceGroupInspectorView.hpp"
#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSQuantityEdit.hpp"
#include "OSVectorController.hpp"

#include "../model/InteriorPartitionSurfaceGroup.hpp"
#include "../model/InteriorPartitionSurfaceGroup_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QColorDialog>
#include <QColor>
#include <QGridLayout>
#include <QScrollArea>
#include <QStackedWidget>

namespace openstudio {

InteriorPartitionSurfaceGroupInspectorView::InteriorPartitionSurfaceGroupInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent )
  : ModelObjectInspectorView(model, true, parent)
{
  m_isIP = isIP;

  QWidget* hiddenWidget = new QWidget();
  this->stackedWidget()->insertWidget(0, hiddenWidget);

  QWidget* visibleWidget = new QWidget();
  this->stackedWidget()->insertWidget(1, visibleWidget);

  this->stackedWidget()->setCurrentIndex(0);

  QGridLayout* mainGridLayout = new QGridLayout();
  mainGridLayout->setContentsMargins(7,7,7,7);
  mainGridLayout->setSpacing(14);
  visibleWidget->setLayout(mainGridLayout);

  // name
  QVBoxLayout* vLayout = new QVBoxLayout();

  QLabel* label = new QLabel();
  label->setText("Name: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_nameEdit = new OSLineEdit();
  vLayout->addWidget(m_nameEdit);

  mainGridLayout->addLayout(vLayout,0,0,1,2, Qt::AlignTop);

  // direction of relative north and multiplier
  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Direction of relative north: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  bool isConnected = false;

  m_directionofRelativeNorthEdit = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_directionofRelativeNorthEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  vLayout->addWidget(m_directionofRelativeNorthEdit);

  mainGridLayout->addLayout(vLayout,1,0, Qt::AlignTop|Qt::AlignLeft);

  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Multiplier: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_multiplierEdit = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_multiplierEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  vLayout->addWidget(m_multiplierEdit);

  mainGridLayout->addLayout(vLayout,1,1, Qt::AlignTop|Qt::AlignLeft);

  // x, y, and z origin
  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("X Origin: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_xOriginEdit = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_xOriginEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  vLayout->addWidget(m_xOriginEdit);

  mainGridLayout->addLayout(vLayout,2,0, Qt::AlignTop|Qt::AlignLeft);

  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Y Origin: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_yOriginEdit = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_yOriginEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  vLayout->addWidget(m_yOriginEdit);

  mainGridLayout->addLayout(vLayout,2,1, Qt::AlignTop|Qt::AlignLeft);

  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Z Origin: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_zOriginEdit = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_zOriginEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  vLayout->addWidget(m_zOriginEdit);

  mainGridLayout->addLayout(vLayout,2,2, Qt::AlignTop|Qt::AlignLeft);

  mainGridLayout->setColumnMinimumWidth(0, 100);
  mainGridLayout->setColumnMinimumWidth(1, 100);
  mainGridLayout->setColumnStretch(2,1);
  mainGridLayout->setRowMinimumHeight(0, 30);
  mainGridLayout->setRowMinimumHeight(1, 30);
  mainGridLayout->setRowMinimumHeight(2, 30);
  mainGridLayout->setRowStretch(3,1);
}

void InteriorPartitionSurfaceGroupInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  detach();
}

void InteriorPartitionSurfaceGroupInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::InteriorPartitionSurfaceGroup interiorPartitionSurfaceGroup = modelObject.cast<model::InteriorPartitionSurfaceGroup>();
  attach(interiorPartitionSurfaceGroup);
  refresh();
}

void InteriorPartitionSurfaceGroupInspectorView::onUpdate()
{
  refresh();
}

void InteriorPartitionSurfaceGroupInspectorView::attach(openstudio::model::InteriorPartitionSurfaceGroup& interiorPartitionSurfaceGroup)
{
  m_nameEdit->bind(interiorPartitionSurfaceGroup, "name");
  //m_directionofRelativeNorthEdit->bind(interiorPartitionSurfaceGroup, "directionofRelativeNorth", m_isIP, std::string("isDirectionofRelativeNorthDefaulted"));
  //m_multiplierEdit->bind(interiorPartitionSurfaceGroup, "multiplier");
  //m_xOriginEdit->bind(interiorPartitionSurfaceGroup, "xOrigin", m_isIP, std::string("isXOriginDefaulted"));
  //m_yOriginEdit->bind(interiorPartitionSurfaceGroup, "yOrigin", m_isIP, std::string("isYOriginDefaulted"));
  //m_zOriginEdit->bind(interiorPartitionSurfaceGroup, "zOrigin", m_isIP, std::string("isZOriginDefaulted"));

  this->stackedWidget()->setCurrentIndex(1);
}

void InteriorPartitionSurfaceGroupInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_nameEdit->unbind();
  m_directionofRelativeNorthEdit->unbind();
  m_multiplierEdit->unbind();
  m_xOriginEdit->unbind();
  m_yOriginEdit->unbind();
  m_zOriginEdit->unbind();
}

void InteriorPartitionSurfaceGroupInspectorView::refresh()
{
}

void InteriorPartitionSurfaceGroupInspectorView::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

} // openstudio

