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

#include "DaylightingControlInspectorView.hpp"
#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSQuantityEdit.hpp"
#include "../shared_gui_components/OSComboBox.hpp"

#include "../model/DaylightingControl.hpp"
#include "../model/DaylightingControl_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGridLayout>
#include <QScrollArea>
#include <QStackedWidget>

namespace openstudio {

DaylightingControlInspectorView::DaylightingControlInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent )
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

  // setpoint and control type
  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Illuminance Setpoint: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_illuminanceSetpointEdit = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_illuminanceSetpointEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  vLayout->addWidget(m_illuminanceSetpointEdit);

  mainGridLayout->addLayout(vLayout,3,0, Qt::AlignTop|Qt::AlignLeft);

  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Control Type: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_controlTypeComboBox = new OSComboBox();
  vLayout->addWidget(m_controlTypeComboBox);

  mainGridLayout->addLayout(vLayout,3,1, Qt::AlignTop|Qt::AlignLeft);

  // number of steps and probability reset
  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Number of Steps: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_numberOfStepsEdit = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_numberOfStepsEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  vLayout->addWidget(m_numberOfStepsEdit);

  mainGridLayout->addLayout(vLayout,4,0, Qt::AlignTop|Qt::AlignLeft);

  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Probability Reset: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_probabilityResetEdit = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_probabilityResetEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  vLayout->addWidget(m_probabilityResetEdit);

  mainGridLayout->addLayout(vLayout,4,1, Qt::AlignTop|Qt::AlignLeft);

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

void DaylightingControlInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  detach();
}

void DaylightingControlInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::DaylightingControl daylightingControl = modelObject.cast<model::DaylightingControl>();
  attach(daylightingControl);
  refresh();
}

void DaylightingControlInspectorView::onUpdate()
{
  refresh();
}

void DaylightingControlInspectorView::attach(openstudio::model::DaylightingControl& daylightingControl)
{
  m_nameEdit->bind(daylightingControl, "name");
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

void DaylightingControlInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_nameEdit->unbind();
  m_xCoordinateEdit->unbind();
  m_yCoordinateEdit->unbind();
  m_zCoordinateEdit->unbind();
  m_psiRotationEdit->unbind();
  m_thetaRotationEdit->unbind();
  m_phiRotationEdit->unbind();
  m_illuminanceSetpointEdit->unbind();
  //m_controlTypeComboBox->unbind();
  m_numberOfStepsEdit->unbind();
  m_probabilityResetEdit->unbind();
  m_numberOfViewsEdit->unbind();
  m_maxGlareEdit->unbind();
}

void DaylightingControlInspectorView::refresh()
{
}

void DaylightingControlInspectorView::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

} // openstudio

