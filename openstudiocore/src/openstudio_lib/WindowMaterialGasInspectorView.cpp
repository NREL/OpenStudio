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

#include "WindowMaterialGasInspectorView.hpp"

#include "../shared_gui_components/OSComboBox.hpp"
#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSQuantityEdit.hpp"

#include "../model/Gas.hpp"
#include "../model/Gas_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <QGridLayout>
#include <QLabel>
#include <QStackedWidget>

namespace openstudio {

// WindowMaterialGasInspectorView

WindowMaterialGasInspectorView::WindowMaterialGasInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent)
  : MaterialBaseInspectorView(isIP, model, parent),
    m_gasType(nullptr),
    m_thickness(nullptr),
    m_conductivityCoefficientA(nullptr),
    m_conductivityCoefficientB(nullptr),
    m_viscosityCoefficientA(nullptr),
    m_viscosityCoefficientB(nullptr),
    m_specificHeatCoefficientA(nullptr),
    m_specificHeatCoefficientB(nullptr),
    m_molecularWeight(nullptr)
{
  createLayout();
}

void WindowMaterialGasInspectorView::createLayout()
{
  MaterialBaseInspectorView::createLayout(); // call parent implementation

  QVBoxLayout * vLayout = nullptr;

  QLabel * label = nullptr;

  unsigned row = m_mainGridLayout->rowCount();

  // Gas Type

  label = new QLabel("Gas Type: ");
  label->setObjectName("H2");
  m_mainGridLayout->addWidget(label,row++,0);

  m_gasType = new OSComboBox();
  m_gasType->addItem("Air");
  m_gasType->addItem("Argon");
  m_gasType->addItem("Krypton");
  m_gasType->addItem("Xenon");
  m_gasType->addItem("Custom");
  m_mainGridLayout->addWidget(m_gasType,row++,0,1,3);

  // Thickness

  label = new QLabel("Thickness: ");
  label->setObjectName("H2");
  m_mainGridLayout->addWidget(label,row++,0);

  m_thickness = new OSQuantityEdit(m_isIP);
  connect(this, &WindowMaterialGasInspectorView::toggleUnitsClicked, m_thickness, &OSQuantityEdit::onUnitSystemChange);
  m_mainGridLayout->addWidget(m_thickness,row++,0,1,3);

  // Conductivity Coefficient A

  label = new QLabel("Conductivity Coefficient A: ");
  label->setObjectName("H2");
  m_mainGridLayout->addWidget(label,row++,0);

  m_conductivityCoefficientA = new OSQuantityEdit(m_isIP);
  connect(this, &WindowMaterialGasInspectorView::toggleUnitsClicked, m_conductivityCoefficientA, &OSQuantityEdit::onUnitSystemChange);
  m_mainGridLayout->addWidget(m_conductivityCoefficientA,row++,0,1,3);

  // Conductivity Coefficient B

  label = new QLabel("Conductivity Coefficient B: ");
  label->setObjectName("H2");
  m_mainGridLayout->addWidget(label,row++,0);

  m_conductivityCoefficientB = new OSQuantityEdit(m_isIP);
  connect(this, &WindowMaterialGasInspectorView::toggleUnitsClicked, m_conductivityCoefficientB, &OSQuantityEdit::onUnitSystemChange);
  m_mainGridLayout->addWidget(m_conductivityCoefficientB,row++,0,1,3);

  // Viscosity Coefficient A

  label = new QLabel("Viscosity Coefficient A: ");
  label->setObjectName("H2");
  m_mainGridLayout->addWidget(label,row++,0);

  m_viscosityCoefficientA = new OSQuantityEdit(m_isIP);
  connect(this, &WindowMaterialGasInspectorView::toggleUnitsClicked, m_viscosityCoefficientA, &OSQuantityEdit::onUnitSystemChange);
  m_mainGridLayout->addWidget(m_viscosityCoefficientA,row++,0,1,3);

  // Viscosity Coefficient B

  label = new QLabel("Viscosity Coefficient B: ");
  label->setObjectName("H2");
  m_mainGridLayout->addWidget(label,row++,0);

  m_viscosityCoefficientB = new OSQuantityEdit(m_isIP);
  connect(this, &WindowMaterialGasInspectorView::toggleUnitsClicked, m_viscosityCoefficientB, &OSQuantityEdit::onUnitSystemChange);
  m_mainGridLayout->addWidget(m_viscosityCoefficientB,row++,0,1,3);

  // Specific Heat Coefficient A

  label = new QLabel("Specific Heat Coefficient A: ");
  label->setObjectName("H2");
  m_mainGridLayout->addWidget(label,row++,0);

  m_specificHeatCoefficientA = new OSQuantityEdit(m_isIP);
  connect(this, &WindowMaterialGasInspectorView::toggleUnitsClicked, m_specificHeatCoefficientA, &OSQuantityEdit::onUnitSystemChange);
  m_mainGridLayout->addWidget(m_specificHeatCoefficientA,row++,0,1,3);

  // Specific Heat Coefficient B

  label = new QLabel("Specific Heat Coefficient B: ");
  label->setObjectName("H2");
  m_mainGridLayout->addWidget(label,row++,0);

  m_specificHeatCoefficientB = new OSQuantityEdit(m_isIP);
  connect(this, &WindowMaterialGasInspectorView::toggleUnitsClicked, m_specificHeatCoefficientB, &OSQuantityEdit::onUnitSystemChange);
  m_mainGridLayout->addWidget(m_specificHeatCoefficientB,row++,0,1,3);

  // Molecular Weight

  label = new QLabel("Molecular Weight: ");
  label->setObjectName("H2");
  m_mainGridLayout->addWidget(label,row++,0);

  m_molecularWeight = new OSQuantityEdit(m_isIP);
  connect(this, &WindowMaterialGasInspectorView::toggleUnitsClicked, m_molecularWeight, &OSQuantityEdit::onUnitSystemChange);
  m_mainGridLayout->addWidget(m_molecularWeight,row++,0,1,3);

  // Stretch

  m_mainGridLayout->setRowStretch(100,100);

  m_mainGridLayout->setColumnStretch(100,100);
}

void WindowMaterialGasInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  detach();
}

void WindowMaterialGasInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::Gas gas = modelObject.cast<model::Gas>();
  attach(gas);
  refresh();
}

void WindowMaterialGasInspectorView::onUpdate()
{
  refresh();
}

void WindowMaterialGasInspectorView::attach(openstudio::model::Gas & gas)
{
  m_gasType->bind(gas,"gasType");

  m_nameEdit->bind(gas,"name");
  m_thickness->bind(gas,"thickness",m_isIP);
  m_conductivityCoefficientA->bind(gas,"conductivityCoefficientA",m_isIP);
  m_conductivityCoefficientB->bind(gas,"conductivityCoefficientB",m_isIP);
  m_viscosityCoefficientA->bind(gas,"viscosityCoefficientA",m_isIP);
  m_viscosityCoefficientB->bind(gas,"viscosityCoefficientB",m_isIP);
  m_specificHeatCoefficientA->bind(gas,"specificHeatCoefficientA",m_isIP);
  m_specificHeatCoefficientB->bind(gas,"specificHeatCoefficientB",m_isIP);
  m_molecularWeight->bind(gas,"molecularWeight",m_isIP);

  this->stackedWidget()->setCurrentIndex(1);
}

void WindowMaterialGasInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_gasType->unbind();

  m_nameEdit->unbind();
  m_thickness->unbind();
  m_conductivityCoefficientA->unbind();
  m_conductivityCoefficientB->unbind();
  m_viscosityCoefficientA->unbind();
  m_viscosityCoefficientB->unbind();
  m_specificHeatCoefficientA->unbind();
  m_specificHeatCoefficientB->unbind();
  m_molecularWeight->unbind();
}

void WindowMaterialGasInspectorView::refresh()
{
}

} // openstudio
