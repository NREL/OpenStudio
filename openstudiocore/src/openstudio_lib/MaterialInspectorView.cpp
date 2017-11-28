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

#include "MaterialInspectorView.hpp"

#include "StandardsInformationMaterialWidget.hpp"

#include "OSItem.hpp"

#include "../shared_gui_components/OSComboBox.hpp"
#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSQuantityEdit.hpp"

#include "../model/StandardOpaqueMaterial.hpp"
#include "../model/StandardOpaqueMaterial_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <QBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QStackedWidget>

namespace openstudio {

// MaterialInspectorView

  MaterialInspectorView::MaterialInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent)
  : ModelObjectInspectorView(model, true, parent),
  m_isIP(isIP)
{
  createLayout();
}

void MaterialInspectorView::createLayout()
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
  m_standardsInformationWidget->showComposite();

  ++row;

  QVBoxLayout * vLayout = nullptr;

  // Roughness
  vLayout = new QVBoxLayout();

  label = new QLabel("Roughness: ");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_roughness = new OSComboBox2();
  m_roughness->addItem("Very Rough");
  m_roughness->addItem("Rough");
  m_roughness->addItem("Medium Rough");
  m_roughness->addItem("Medium Smooth");
  m_roughness->addItem("Smooth");
  m_roughness->addItem("Very Smooth");
  vLayout->addWidget(m_roughness);

  mainGridLayout->addLayout(vLayout, row, 0);

  // Thickness
  vLayout = new QVBoxLayout();

  label = new QLabel("Thickness: ");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_thickness = new OSQuantityEdit2("m","m","in", m_isIP);
  connect(this, &MaterialInspectorView::toggleUnitsClicked, m_thickness, &OSQuantityEdit2::onUnitSystemChange);
  vLayout->addWidget(m_thickness);

  mainGridLayout->addLayout(vLayout, row++, 1);

  // Conductivity
  vLayout = new QVBoxLayout();

  label = new QLabel("Conductivity: ");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_conductivity = new OSQuantityEdit2("W/m*K","W/m*K","Btu*in/hr*ft^2*R", m_isIP);
  connect(this, &MaterialInspectorView::toggleUnitsClicked, m_conductivity, &OSQuantityEdit2::onUnitSystemChange);
  vLayout->addWidget(m_conductivity);

  mainGridLayout->addLayout(vLayout, row, 0);

  // Density
  vLayout = new QVBoxLayout();

  label = new QLabel("Density: ");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_density = new OSQuantityEdit2("kg/m^3","kg/m^3","lb/ft^3", m_isIP);
  connect(this, &MaterialInspectorView::toggleUnitsClicked, m_density, &OSQuantityEdit2::onUnitSystemChange);
  vLayout->addWidget(m_density);

  mainGridLayout->addLayout(vLayout, row++, 1);

  // Specific Heat
  vLayout = new QVBoxLayout();

  label = new QLabel("Specific Heat: ");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_specificHeat = new OSQuantityEdit2("J/kg*K","J/kg*K","Btu/lb*R", m_isIP);
  connect(this, &MaterialInspectorView::toggleUnitsClicked, m_specificHeat, &OSQuantityEdit2::onUnitSystemChange);
  vLayout->addWidget(m_specificHeat);

  mainGridLayout->addLayout(vLayout, row, 0);

  // Thermal Absorptance
  vLayout = new QVBoxLayout();

  label = new QLabel("Thermal Absorptance: ");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_thermalAbsorptance = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &MaterialInspectorView::toggleUnitsClicked, m_thermalAbsorptance, &OSQuantityEdit2::onUnitSystemChange);
  vLayout->addWidget(m_thermalAbsorptance);

  mainGridLayout->addLayout(vLayout, row++, 1);

  // Solar Absorptance
  vLayout = new QVBoxLayout();

  label = new QLabel("Solar Absorptance: ");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_solarAbsorptance = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &MaterialInspectorView::toggleUnitsClicked, m_solarAbsorptance, &OSQuantityEdit2::onUnitSystemChange);
  vLayout->addWidget(m_solarAbsorptance);

  mainGridLayout->addLayout(vLayout, row, 0);

  // Visible Absorptance
  vLayout = new QVBoxLayout();

  label = new QLabel("Visible Absorptance: ");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_visibleAbsorptance = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &MaterialInspectorView::toggleUnitsClicked, m_visibleAbsorptance, &OSQuantityEdit2::onUnitSystemChange);
  vLayout->addWidget(m_visibleAbsorptance);

  mainGridLayout->addLayout(vLayout, row++, 1);

  // Stretch

  mainGridLayout->setRowStretch(100,100);

  mainGridLayout->setColumnStretch(100,100);
}

void MaterialInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  detach();
}

void MaterialInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::StandardOpaqueMaterial standardOpaqueMaterial = modelObject.cast<model::StandardOpaqueMaterial>();
  attach(standardOpaqueMaterial);
  refresh();
}

void MaterialInspectorView::onUpdate()
{
  refresh();
}

void MaterialInspectorView::attach(openstudio::model::StandardOpaqueMaterial & standardOpaqueMaterial)
{
  m_standardOpaqueMaterial = standardOpaqueMaterial;
  // m_nameEdit->bind(standardOpaqueMaterial, "name");
  m_nameEdit->bind(
    *m_standardOpaqueMaterial,
    OptionalStringGetter(std::bind(&model::StandardOpaqueMaterial::name, m_standardOpaqueMaterial.get_ptr(),true)),
    boost::optional<StringSetter>(std::bind(&model::StandardOpaqueMaterial::setName, m_standardOpaqueMaterial.get_ptr(),std::placeholders::_1))
  );

  // m_roughness->bind(standardOpaqueMaterial,"roughness");
  if(m_roughness){
    m_roughness->bind<std::string>(
      *m_standardOpaqueMaterial,
      static_cast<std::string (*)(const std::string&)>(&openstudio::toString),
      &model::StandardOpaqueMaterial::roughnessValues,
      std::bind(&model::StandardOpaqueMaterial::roughness, m_standardOpaqueMaterial.get_ptr()),
      std::bind(&model::StandardOpaqueMaterial::setRoughness, m_standardOpaqueMaterial.get_ptr(), std::placeholders::_1),
      boost::none,
      boost::none);
  }

  // m_thickness->bind(standardOpaqueMaterial,"thickness",m_isIP);
  m_thickness->bind(
    m_isIP,
    *m_standardOpaqueMaterial,
    DoubleGetter(std::bind(&model::StandardOpaqueMaterial::thickness, m_standardOpaqueMaterial.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::StandardOpaqueMaterial::*)(double)>(&model::StandardOpaqueMaterial::setThickness), m_standardOpaqueMaterial.get_ptr(), std::placeholders::_1))
  );

  // m_conductivity->bind(standardOpaqueMaterial,"conductivity",m_isIP);
  m_conductivity->bind(
    m_isIP,
    *m_standardOpaqueMaterial,
    DoubleGetter(std::bind(&model::StandardOpaqueMaterial::conductivity, m_standardOpaqueMaterial.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::StandardOpaqueMaterial::*)(double)>(&model::StandardOpaqueMaterial::setConductivity), m_standardOpaqueMaterial.get_ptr(), std::placeholders::_1))
  );

  // m_density->bind(standardOpaqueMaterial,"density",m_isIP);
  m_density->bind(
    m_isIP,
    *m_standardOpaqueMaterial,
    DoubleGetter(std::bind(&model::StandardOpaqueMaterial::density, m_standardOpaqueMaterial.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::StandardOpaqueMaterial::*)(double)>(&model::StandardOpaqueMaterial::setDensity), m_standardOpaqueMaterial.get_ptr(), std::placeholders::_1))
  );

  // m_specificHeat->bind(standardOpaqueMaterial,"specificHeat",m_isIP);
  m_specificHeat->bind(
    m_isIP,
    *m_standardOpaqueMaterial,
    DoubleGetter(std::bind(&model::StandardOpaqueMaterial::specificHeat, m_standardOpaqueMaterial.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::StandardOpaqueMaterial::*)(double)>(&model::StandardOpaqueMaterial::setSpecificHeat), m_standardOpaqueMaterial.get_ptr(), std::placeholders::_1))
  );

  // m_thermalAbsorptance->bind(standardOpaqueMaterial,"thermalAbsorptance",m_isIP); // Fractional, no units
  m_thermalAbsorptance->bind(
    m_isIP,
    *m_standardOpaqueMaterial,
    DoubleGetter(std::bind(&model::StandardOpaqueMaterial::thermalAbsorptance, m_standardOpaqueMaterial.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::StandardOpaqueMaterial::*)(double)>(&model::StandardOpaqueMaterial::setThermalAbsorptance), m_standardOpaqueMaterial.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::StandardOpaqueMaterial::resetThermalAbsorptance, m_standardOpaqueMaterial.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::StandardOpaqueMaterial::isThermalAbsorptanceDefaulted, m_standardOpaqueMaterial.get_ptr()))
  );

  // m_solarAbsorptance->bind(standardOpaqueMaterial,"solarAbsorptance",m_isIP); // Fractional, no units
  m_solarAbsorptance->bind(
    m_isIP,
    *m_standardOpaqueMaterial,
    DoubleGetter(std::bind(&model::StandardOpaqueMaterial::solarAbsorptance, m_standardOpaqueMaterial.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::StandardOpaqueMaterial::*)(double)>(&model::StandardOpaqueMaterial::setSolarAbsorptance), m_standardOpaqueMaterial.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::StandardOpaqueMaterial::resetSolarAbsorptance, m_standardOpaqueMaterial.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::StandardOpaqueMaterial::isSolarAbsorptanceDefaulted, m_standardOpaqueMaterial.get_ptr()))
  );

  // m_visibleAbsorptance->bind(standardOpaqueMaterial,"visibleAbsorptance",m_isIP); // Fractional, no units
  m_visibleAbsorptance->bind(
    m_isIP,
    *m_standardOpaqueMaterial,
    DoubleGetter(std::bind(&model::StandardOpaqueMaterial::visibleAbsorptance, m_standardOpaqueMaterial.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::StandardOpaqueMaterial::*)(double)>(&model::StandardOpaqueMaterial::setVisibleAbsorptance), m_standardOpaqueMaterial.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::StandardOpaqueMaterial::resetVisibleAbsorptance, m_standardOpaqueMaterial.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::StandardOpaqueMaterial::isVisibleAbsorptanceDefaulted, m_standardOpaqueMaterial.get_ptr()))
  );


  m_standardsInformationWidget->attach(standardOpaqueMaterial);

  this->stackedWidget()->setCurrentIndex(1);
}

void MaterialInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_nameEdit->unbind();

  m_roughness->unbind();

  m_thickness->unbind();
  m_conductivity->unbind();
  m_density->unbind();
  m_specificHeat->unbind();
  m_thermalAbsorptance->unbind();
  m_solarAbsorptance->unbind();
  m_visibleAbsorptance->unbind();

  m_standardOpaqueMaterial = boost::none;

  m_standardsInformationWidget->detach();
}

void MaterialInspectorView::refresh()
{
}

} // openstudio
