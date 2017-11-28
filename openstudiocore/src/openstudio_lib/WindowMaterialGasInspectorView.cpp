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

  m_thickness = new OSQuantityEdit2("m","m","in", m_isIP);
  connect(this, &WindowMaterialGasInspectorView::toggleUnitsClicked, m_thickness, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_thickness,row++,0,1,3);

  // Conductivity Coefficient A

  label = new QLabel("Conductivity Coefficient A: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_conductivityCoefficientA = new OSQuantityEdit2("W/m*K", "W/m*K", "Btu*in/hr*ft^2*R", m_isIP);
  connect(this, &WindowMaterialGasInspectorView::toggleUnitsClicked, m_conductivityCoefficientA, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_conductivityCoefficientA,row++,0,1,3);

  // Conductivity Coefficient B

  label = new QLabel("Conductivity Coefficient B: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_conductivityCoefficientB = new OSQuantityEdit2("W/m*K^2", "W/m*K^2", "Btu*in/hr*ft^2*R^2", m_isIP);
  connect(this, &WindowMaterialGasInspectorView::toggleUnitsClicked, m_conductivityCoefficientB, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_conductivityCoefficientB,row++,0,1,3);

  // Viscosity Coefficient A

  label = new QLabel("Viscosity Coefficient A: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_viscosityCoefficientA = new OSQuantityEdit2("mkg/m*s","mkg/m*s","m(min^2*ton/ft^3)", m_isIP);
  connect(this, &WindowMaterialGasInspectorView::toggleUnitsClicked, m_viscosityCoefficientA, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_viscosityCoefficientA,row++,0,1,3);

  // Viscosity Coefficient B

  label = new QLabel("Viscosity Coefficient B: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_viscosityCoefficientB = new OSQuantityEdit2("mkg/m*s*K","mkg/m*s*K","m(min^2*ton/ft^3*R)", m_isIP);
  connect(this, &WindowMaterialGasInspectorView::toggleUnitsClicked, m_viscosityCoefficientB, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_viscosityCoefficientB,row++,0,1,3);

  // Specific Heat Coefficient A

  label = new QLabel("Specific Heat Coefficient A: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_specificHeatCoefficientA = new OSQuantityEdit2("J/kg*K","J/kg*K","Btu/lb*R", m_isIP);
  connect(this, &WindowMaterialGasInspectorView::toggleUnitsClicked, m_specificHeatCoefficientA, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_specificHeatCoefficientA,row++,0,1,3);

  // Specific Heat Coefficient B

  label = new QLabel("Specific Heat Coefficient B: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);
  m_specificHeatCoefficientB = new OSQuantityEdit2("J/kg*K^2","J/kg*K^2","Btu/lb*R^2", m_isIP);
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
  m_gas = gas;

  // m_gasType->bind(gas,"gasType");
  m_gasType->bind<std::string>(
      *m_gas,
      static_cast<std::string (*)(const std::string&)>(&openstudio::toString),
      &model::Gas::validGasTypes,
      std::bind(&model::Gas::gasType, m_gas.get_ptr()),
      std::bind(&model::Gas::setGasType, m_gas.get_ptr(), std::placeholders::_1),
      boost::none,
      boost::none);

  // m_nameEdit->bind(gas,"name");
  m_nameEdit->bind(
    *m_gas,
    OptionalStringGetter(std::bind(&model::Gas::name, m_gas.get_ptr(),true)),
    boost::optional<StringSetter>(std::bind(&model::Gas::setName, m_gas.get_ptr(),std::placeholders::_1))
  );


  // m_thickness->bind(gas,"thickness",m_isIP);
  m_thickness->bind(
    m_isIP,
    *m_gas,
    DoubleGetter(std::bind(&model::Gas::thickness, m_gas.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Gas::*)(double)>(&model::Gas::setThickness), m_gas.get_ptr(), std::placeholders::_1))
  );

  // m_conductivityCoefficientA->bind(gas,"conductivityCoefficientA",m_isIP);
  m_conductivityCoefficientA->bind(
    m_isIP,
    *m_gas,
    OptionalDoubleGetter(std::bind(&model::Gas::conductivityCoefficientA, m_gas.get_ptr())),
    DoubleSetterVoidReturn(std::bind(static_cast<void(model::Gas::*)(double)>(&model::Gas::setConductivityCoefficientA), m_gas.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::Gas::resetConductivityCoefficientA, m_gas.get_ptr()))
  );

  // m_conductivityCoefficientB->bind(gas,"conductivityCoefficientB",m_isIP);
  m_conductivityCoefficientB->bind(
    m_isIP,
    *m_gas,
    OptionalDoubleGetter(std::bind(&model::Gas::conductivityCoefficientB, m_gas.get_ptr())),
    DoubleSetterVoidReturn(std::bind(static_cast<void(model::Gas::*)(double)>(&model::Gas::setConductivityCoefficientB), m_gas.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::Gas::resetConductivityCoefficientB, m_gas.get_ptr()))
  );

  // m_viscosityCoefficientA->bind(gas,"viscosityCoefficientA",m_isIP);
  m_viscosityCoefficientA->bind(
    m_isIP,
    *m_gas,
    OptionalDoubleGetter(std::bind(&model::Gas::viscosityCoefficientA, m_gas.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Gas::*)(double)>(&model::Gas::setViscosityCoefficientA), m_gas.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::Gas::resetViscosityCoefficientA, m_gas.get_ptr()))
  );

  // m_viscosityCoefficientB->bind(gas,"viscosityCoefficientB",m_isIP);
  m_viscosityCoefficientB->bind(
    m_isIP,
    *m_gas,
    OptionalDoubleGetter(std::bind(&model::Gas::viscosityCoefficientB, m_gas.get_ptr())),
    DoubleSetterVoidReturn(std::bind(static_cast<void(model::Gas::*)(double)>(&model::Gas::setViscosityCoefficientB), m_gas.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::Gas::resetViscosityCoefficientB, m_gas.get_ptr()))
  );

  // m_specificHeatCoefficientA->bind(gas,"specificHeatCoefficientA",m_isIP);
  m_specificHeatCoefficientA->bind(
    m_isIP,
    *m_gas,
    OptionalDoubleGetter(std::bind(&model::Gas::specificHeatCoefficientA, m_gas.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Gas::*)(double)>(&model::Gas::setSpecificHeatCoefficientA), m_gas.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::Gas::resetSpecificHeatCoefficientA, m_gas.get_ptr()))
  );

  // m_specificHeatCoefficientB->bind(gas,"specificHeatCoefficientB",m_isIP);
  m_specificHeatCoefficientB->bind(
    m_isIP,
    *m_gas,
    OptionalDoubleGetter(std::bind(&model::Gas::specificHeatCoefficientB, m_gas.get_ptr())),
    DoubleSetterVoidReturn(std::bind(static_cast<void(model::Gas::*)(double)>(&model::Gas::setSpecificHeatCoefficientB), m_gas.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::Gas::resetSpecificHeatCoefficientB, m_gas.get_ptr()))
  );

  // m_molecularWeight->bind(gas,"molecularWeight",m_isIP);
  m_molecularWeight->bind(
    m_isIP,
    *m_gas,
    OptionalDoubleGetter(std::bind(&model::Gas::molecularWeight, m_gas.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Gas::*)(double)>(&model::Gas::setMolecularWeight), m_gas.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::Gas::resetMolecularWeight, m_gas.get_ptr()))
  );

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

  m_gas = boost::none;

  m_standardsInformationWidget->detach();
}

void WindowMaterialGasInspectorView::refresh()
{
}

} // openstudio
