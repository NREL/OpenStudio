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

#include "GlareSensorInspectorView.hpp"
#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSQuantityEdit.hpp"
#include "../shared_gui_components/OSComboBox.hpp"

#include "../model/GlareSensor.hpp"
#include "../model/GlareSensor_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGridLayout>
#include <QScrollArea>
#include <QStackedWidget>

namespace openstudio {

GlareSensorInspectorView::GlareSensorInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent )
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
  label->setText("Phi Rotation About Z: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_phiRotationEdit = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_phiRotationEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  vLayout->addWidget(m_phiRotationEdit);

  mainGridLayout->addLayout(vLayout,2,2, Qt::AlignTop|Qt::AlignLeft);

  // number of views and max glare
  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Number of Views: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_numberOfViewsEdit = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_numberOfViewsEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  vLayout->addWidget(m_numberOfViewsEdit);

  mainGridLayout->addLayout(vLayout,5,0, Qt::AlignTop|Qt::AlignLeft);

  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Max Glare: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_maxGlareEdit = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_maxGlareEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  vLayout->addWidget(m_maxGlareEdit);

  mainGridLayout->addLayout(vLayout,5,1, Qt::AlignTop|Qt::AlignLeft);

  mainGridLayout->setColumnMinimumWidth(0, 80);
  mainGridLayout->setColumnMinimumWidth(1, 80);
  mainGridLayout->setColumnMinimumWidth(1, 80);
  mainGridLayout->setColumnStretch(3,1);
  mainGridLayout->setRowMinimumHeight(0, 30);
  mainGridLayout->setRowMinimumHeight(1, 30);
  mainGridLayout->setRowMinimumHeight(2, 30);
  mainGridLayout->setRowMinimumHeight(3, 30);
  mainGridLayout->setRowMinimumHeight(4, 30);
  mainGridLayout->setRowMinimumHeight(5, 30);
  mainGridLayout->setRowStretch(6,1);
}

void GlareSensorInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  detach();
}

void GlareSensorInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::GlareSensor glareSensor = modelObject.cast<model::GlareSensor>();
  attach(glareSensor);
  refresh();
}

void GlareSensorInspectorView::onUpdate()
{
  refresh();
}

void GlareSensorInspectorView::attach(openstudio::model::GlareSensor& glareSensor)
{
  m_nameEdit->bind(glareSensor, "name");
  //m_xCoordinateEdit->bind();
  //m_yCoordinateEdit->bind();
  //m_zCoordinateEdit->bind();
  //m_psiRotationEdit->bind();
  //m_thetaRotationEdit->bind();
  //m_phiRotationEdit->bind();
  //m_illuminanceSetpointEdit->bind();
  //m_controlTypeComboBox->bind();
  //m_numberOfStepsEdit->bind();
  //m_probabilityResetEdit->bind();
  //m_numberOfViewsEdit->bind();
  //m_maxGlareEdit->bind();

  this->stackedWidget()->setCurrentIndex(1);
}

void GlareSensorInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_nameEdit->unbind();
  m_xCoordinateEdit->unbind();
  m_yCoordinateEdit->unbind();
  m_zCoordinateEdit->unbind();
  m_psiRotationEdit->unbind();
  m_thetaRotationEdit->unbind();
  m_phiRotationEdit->unbind();
  //m_controlTypeComboBox->unbind();
  m_numberOfViewsEdit->unbind();
  m_maxGlareEdit->unbind();
}

void GlareSensorInspectorView::refresh()
{
}

void GlareSensorInspectorView::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

} // openstudio

