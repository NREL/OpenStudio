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

#include "IlluminanceMapInspectorView.hpp"
#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSQuantityEdit.hpp"
#include "../shared_gui_components/OSComboBox.hpp"

#include "../model/IlluminanceMap.hpp"
#include "../model/IlluminanceMap_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGridLayout>
#include <QScrollArea>
#include <QStackedWidget>

namespace openstudio {

IlluminanceMapInspectorView::IlluminanceMapInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent )
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

  // x, y, and z
  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("X Coordinate: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  bool isConnected = false;

  m_xCoordinateEdit = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_xCoordinateEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  vLayout->addWidget(m_xCoordinateEdit);

  mainGridLayout->addLayout(vLayout,1,0, Qt::AlignTop|Qt::AlignLeft);

  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Y Coordinate: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_yCoordinateEdit = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_yCoordinateEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  vLayout->addWidget(m_yCoordinateEdit);

  mainGridLayout->addLayout(vLayout,1,1, Qt::AlignTop|Qt::AlignLeft);

  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Z Coordinate: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_zCoordinateEdit = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_zCoordinateEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  vLayout->addWidget(m_zCoordinateEdit);

  mainGridLayout->addLayout(vLayout,1,2, Qt::AlignTop|Qt::AlignLeft);

  // psi, theta, and phi
  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Psi Rotation About X: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_psiRotationEdit = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_psiRotationEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  vLayout->addWidget(m_psiRotationEdit);

  mainGridLayout->addLayout(vLayout,2,0, Qt::AlignTop|Qt::AlignLeft);

  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Theta Rotation About Y: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_thetaRotationEdit = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_thetaRotationEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  vLayout->addWidget(m_thetaRotationEdit);

  mainGridLayout->addLayout(vLayout,2,1, Qt::AlignTop|Qt::AlignLeft);

  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Phi Rotation About Y: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_phiRotationEdit = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_phiRotationEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  vLayout->addWidget(m_phiRotationEdit);

  mainGridLayout->addLayout(vLayout,2,2, Qt::AlignTop|Qt::AlignLeft);

  // x length and number of x points
  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("X Length: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_xLength = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_xLength, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  vLayout->addWidget(m_xLength);

  mainGridLayout->addLayout(vLayout,3,0, Qt::AlignTop|Qt::AlignLeft);

  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Number of X Points: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_numberOfXPoints = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_numberOfXPoints, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  vLayout->addWidget(m_numberOfXPoints);

  mainGridLayout->addLayout(vLayout,3,1, Qt::AlignTop|Qt::AlignLeft);

  // y length and number of y points
  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Y Length: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_yLength = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_yLength, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  vLayout->addWidget(m_yLength);

  mainGridLayout->addLayout(vLayout,4,0, Qt::AlignTop|Qt::AlignLeft);

  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Number of Y Points: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_numberOfYPoints = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_numberOfYPoints, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  vLayout->addWidget(m_numberOfYPoints);

  mainGridLayout->addLayout(vLayout,4,1, Qt::AlignTop|Qt::AlignLeft);
  mainGridLayout->setColumnMinimumWidth(0, 80);
  mainGridLayout->setColumnMinimumWidth(1, 80);
  mainGridLayout->setColumnMinimumWidth(2, 80);
  mainGridLayout->setColumnStretch(3,1);
  mainGridLayout->setRowMinimumHeight(0, 30);
  mainGridLayout->setRowMinimumHeight(1, 30);
  mainGridLayout->setRowMinimumHeight(2, 30);
  mainGridLayout->setRowMinimumHeight(3, 30);
  mainGridLayout->setRowMinimumHeight(4, 30);
  mainGridLayout->setRowStretch(5,1);
}

void IlluminanceMapInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  detach();
}

void IlluminanceMapInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::IlluminanceMap illuminanceMap = modelObject.cast<model::IlluminanceMap>();
  attach(illuminanceMap);
  refresh();
}

void IlluminanceMapInspectorView::onUpdate()
{
  refresh();
}

void IlluminanceMapInspectorView::attach(openstudio::model::IlluminanceMap& illuminanceMap)
{
  m_nameEdit->bind(illuminanceMap, "name");
  //m_xCoordinateEdit->bind();
  //m_yCoordinateEdit->bind();
  //m_zCoordinateEdit->bind();
  //m_psiRotationEdit->bind();
  //m_thetaRotationEdit->bind();
  //m_phiRotationEdit->bind();
  //m_xLength->bind();
  //m_numberOfXPoints->bind();
  //m_yLength->bind();
  //m_numberOfYPoints->bind();

  this->stackedWidget()->setCurrentIndex(1);
}

void IlluminanceMapInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_nameEdit->unbind();
  m_xCoordinateEdit->unbind();
  m_yCoordinateEdit->unbind();
  m_zCoordinateEdit->unbind();
  m_psiRotationEdit->unbind();
  m_thetaRotationEdit->unbind();
  m_phiRotationEdit->unbind();
  m_xLength->unbind();
  m_numberOfXPoints->unbind();
  m_yLength->unbind();
  m_numberOfYPoints->unbind();
}

void IlluminanceMapInspectorView::refresh()
{
}

void IlluminanceMapInspectorView::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

} // openstudio

