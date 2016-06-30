/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#include "StandardsInformationMaterialWidget.hpp"

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
  : ModelObjectInspectorView(model, true, parent),
    m_isIP(isIP)
{
  createLayout();
}

void WindowMaterialGasInspectorView::createLayout()
{
  auto hiddenWidget = new QWidget();
  this->stackedWidget()->addWidget(hiddenWidget);

  auto visibleWidget = new QWidget();
  this->stackedWidget()->addWidget(visibleWidget);

  auto mainGridLayout = new QGridLayout();
  mainGridLayout->setContentsMargins(7, 7, 7, 7);
  mainGridLayout->setSpacing(14);
  visibleWidget->setLayout(mainGridLayout);

  int row = mainGridLayout->rowCount();

  QLabel * label = nullptr;

  // Name

  label = new QLabel("Name: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, row, 0);

  ++row;

  m_nameEdit = new OSLineEdit2();
  mainGridLayout->addWidget(m_nameEdit, row, 0, 1, 3);

  ++row;

  // Standards Information

  m_standardsInformationWidget = new StandardsInformationMaterialWidget(m_isIP, mainGridLayout, row);

  ++row;

  // Gas Type

  label = new QLabel("Gas Type: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_gasType = new OSComboBox2();
  m_gasType->addItem("Air");
  m_gasType->addItem("Argon");
  m_gasType->addItem("Krypton");
  m_gasType->addItem("Xenon");
  m_gasType->addItem("Custom");
  mainGridLayout->addWidget(m_gasType,row++,0,1,3);

  // Thickness

  label = new QLabel("Thickness: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_thickness = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialGasInspectorView::toggleUnitsClicked, m_thickness, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_thickness,row++,0,1,3);

  // Conductivity Coefficient A

  label = new QLabel("Conductivity Coefficient A: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_conductivityCoefficientA = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialGasInspectorView::toggleUnitsClicked, m_conductivityCoefficientA, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_conductivityCoefficientA,row++,0,1,3);

  // Conductivity Coefficient B

  label = new QLabel("Conductivity Coefficient B: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_conductivityCoefficientB = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialGasInspectorView::toggleUnitsClicked, m_conductivityCoefficientB, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_conductivityCoefficientB,row++,0,1,3);

  // Viscosity Coefficient A

  label = new QLabel("Viscosity Coefficient A: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_viscosityCoefficientA = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialGasInspectorView::toggleUnitsClicked, m_viscosityCoefficientA, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_viscosityCoefficientA,row++,0,1,3);

  // Viscosity Coefficient B

  label = new QLabel("Viscosity Coefficient B: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_viscosityCoefficientB = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialGasInspectorView::toggleUnitsClicked, m_viscosityCoefficientB, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_viscosityCoefficientB,row++,0,1,3);

  // Specific Heat Coefficient A

  label = new QLabel("Specific Heat Coefficient A: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_specificHeatCoefficientA = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialGasInspectorView::toggleUnitsClicked, m_specificHeatCoefficientA, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_specificHeatCoefficientA,row++,0,1,3);

  // Specific Heat Coefficient B

  label = new QLabel("Specific Heat Coefficient B: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_specificHeatCoefficientB = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialGasInspectorView::toggleUnitsClicked, m_specificHeatCoefficientB, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_specificHeatCoefficientB,row++,0,1,3);

  // Molecular Weight

  label = new QLabel("Molecular Weight: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_molecularWeight = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialGasInspectorView::toggleUnitsClicked, m_molecularWeight, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_molecularWeight,row++,0,1,3);

  // Stretch

  mainGridLayout->setRowStretch(100,100);

  mainGridLayout->setColumnStretch(100,100);
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
  // m_gasType->bind(gas,"gasType");

  m_gasType->bind<std::string>(
      gas,
      static_cast<std::string (*)(const std::string&)>(&openstudio::toString),
      &model::Gas::validGasTypes,
      std::bind(&model::Gas::gasType, &gas),
      std::bind(&model::Gas::setGasType, &gas, std::placeholders::_1),
      boost::none,
      boost::none);

  // m_nameEdit->bind(gas,"name");
  boost::optional<model::Gas> m_gas = gas;
  m_nameEdit->bind(
    *m_gas,
    OptionalStringGetter(std::bind(&model::Gas::name, m_gas.get_ptr(),true)),
    boost::optional<StringSetter>(std::bind(&model::Gas::setName, m_gas.get_ptr(),std::placeholders::_1))
  );


  m_thickness->bind(gas,"thickness",m_isIP);
  m_conductivityCoefficientA->bind(gas,"conductivityCoefficientA",m_isIP);
  m_conductivityCoefficientB->bind(gas,"conductivityCoefficientB",m_isIP);
  m_viscosityCoefficientA->bind(gas,"viscosityCoefficientA",m_isIP);
  m_viscosityCoefficientB->bind(gas,"viscosityCoefficientB",m_isIP);
  m_specificHeatCoefficientA->bind(gas,"specificHeatCoefficientA",m_isIP);
  m_specificHeatCoefficientB->bind(gas,"specificHeatCoefficientB",m_isIP);
  m_molecularWeight->bind(gas,"molecularWeight",m_isIP);

  m_standardsInformationWidget->attach(gas);

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

  m_standardsInformationWidget->detach();
}

void WindowMaterialGasInspectorView::refresh()
{
}

} // openstudio
