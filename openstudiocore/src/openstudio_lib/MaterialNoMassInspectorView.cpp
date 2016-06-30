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

#include "MaterialNoMassInspectorView.hpp"

#include "StandardsInformationMaterialWidget.hpp"

#include "../shared_gui_components/OSComboBox.hpp"
#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSQuantityEdit.hpp"

#include "../model/MasslessOpaqueMaterial.hpp"
#include "../model/MasslessOpaqueMaterial_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <QGridLayout>
#include <QLabel>
#include <QStackedWidget>

namespace openstudio {

// MaterialNoMassInspectorView

MaterialNoMassInspectorView::MaterialNoMassInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent)
  : ModelObjectInspectorView(model, true, parent)
{
  createLayout();
}

void MaterialNoMassInspectorView::createLayout()
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

  // Roughness

  label = new QLabel("Roughness: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_roughness = new OSComboBox2();
  m_roughness->addItem("Very Rough");
  m_roughness->addItem("Rough");
  m_roughness->addItem("Medium Rough");
  m_roughness->addItem("Medium Smooth");
  m_roughness->addItem("Smooth");
  m_roughness->addItem("Very Smooth");
  mainGridLayout->addWidget(m_roughness,row++,0,1,3);

  // Thermal Resistance

  label = new QLabel("Thermal Resistance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_thermalResistance = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &MaterialNoMassInspectorView::toggleUnitsClicked, m_thermalResistance, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_thermalResistance,row++,0,1,3);

  // Thermal Absorptance

  label = new QLabel("Thermal Absorptance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_thermalAbsorptance = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &MaterialNoMassInspectorView::toggleUnitsClicked, m_thermalAbsorptance, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_thermalAbsorptance,row++,0,1,3);

  // Solar Absorptance

  label = new QLabel("Solar Absorptance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_solarAbsorptance = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &MaterialNoMassInspectorView::toggleUnitsClicked, m_solarAbsorptance, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_solarAbsorptance,row++,0,1,3);

  // Visible Absorptance

  label = new QLabel("Visible Absorptance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_visibleAbsorptance = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &MaterialNoMassInspectorView::toggleUnitsClicked, m_visibleAbsorptance, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_visibleAbsorptance,row++,0,1,3);

  // Stretch

  mainGridLayout->setRowStretch(100,100);

  mainGridLayout->setColumnStretch(100,100);
}

void MaterialNoMassInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  detach();
}

void MaterialNoMassInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::MasslessOpaqueMaterial masslessOpaqueMaterial = modelObject.cast<model::MasslessOpaqueMaterial>();
  attach(masslessOpaqueMaterial);
  refresh();
}

void MaterialNoMassInspectorView::onUpdate()
{
  refresh();
}

void MaterialNoMassInspectorView::attach(openstudio::model::MasslessOpaqueMaterial & masslessOpaqueMaterial)
{
  // m_roughness->bind(masslessOpaqueMaterial,"roughness");

  if(m_roughness){
    m_roughness->bind<std::string>(
      masslessOpaqueMaterial,
      static_cast<std::string (*)(const std::string&)>(&openstudio::toString),
      &model::MasslessOpaqueMaterial::roughnessValues,
      std::bind(&model::MasslessOpaqueMaterial::roughness, &masslessOpaqueMaterial),
      std::bind(&model::MasslessOpaqueMaterial::setRoughness, &masslessOpaqueMaterial, std::placeholders::_1),
      boost::none,
      boost::none);
  }
  
  boost::optional<model::MasslessOpaqueMaterial> m_masslessOpaqueMaterial = masslessOpaqueMaterial;
  // m_nameEdit->bind(masslessOpaqueMaterial,"name");
  m_nameEdit->bind(
    *m_masslessOpaqueMaterial,
    OptionalStringGetter(std::bind(&model::MasslessOpaqueMaterial::name, m_masslessOpaqueMaterial.get_ptr(),true)),
    boost::optional<StringSetter>(std::bind(&model::MasslessOpaqueMaterial::setName, m_masslessOpaqueMaterial.get_ptr(),std::placeholders::_1))
  );


  // m_thermalResistance->bind(masslessOpaqueMaterial,"thermalResistance",m_isIP);
  m_thermalResistance->bind(
    m_isIP,
    *m_masslessOpaqueMaterial,
    DoubleGetter(std::bind(&model::MasslessOpaqueMaterial::thermalResistance, m_masslessOpaqueMaterial.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::MasslessOpaqueMaterial::*)(double)>(&model::MasslessOpaqueMaterial::setThermalResistance), m_masslessOpaqueMaterial.get_ptr(), std::placeholders::_1))
  );

  // m_thermalAbsorptance->bind(masslessOpaqueMaterial,"thermalAbsorptance",m_isIP);
  m_thermalAbsorptance->bind(
    m_isIP,
    *m_masslessOpaqueMaterial,
    OptionalDoubleGetter(std::bind(&model::MasslessOpaqueMaterial::thermalAbsorptance, m_masslessOpaqueMaterial.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::MasslessOpaqueMaterial::*)(double)>(&model::MasslessOpaqueMaterial::setThermalAbsorptance), m_masslessOpaqueMaterial.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::MasslessOpaqueMaterial::resetThermalAbsorptance, m_masslessOpaqueMaterial.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::MasslessOpaqueMaterial::isThermalAbsorptanceDefaulted, m_masslessOpaqueMaterial.get_ptr()))
  );

  // m_solarAbsorptance->bind(masslessOpaqueMaterial,"solarAbsorptance",m_isIP);
  m_solarAbsorptance->bind(
    m_isIP,
    *m_masslessOpaqueMaterial,
    OptionalDoubleGetter(std::bind(&model::MasslessOpaqueMaterial::solarAbsorptance, m_masslessOpaqueMaterial.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::MasslessOpaqueMaterial::*)(double)>(&model::MasslessOpaqueMaterial::setSolarAbsorptance), m_masslessOpaqueMaterial.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::MasslessOpaqueMaterial::resetSolarAbsorptance, m_masslessOpaqueMaterial.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::MasslessOpaqueMaterial::isSolarAbsorptanceDefaulted, m_masslessOpaqueMaterial.get_ptr()))
  );

  // m_visibleAbsorptance->bind(masslessOpaqueMaterial,"visibleAbsorptance",m_isIP);
  m_visibleAbsorptance->bind(
    m_isIP,
    *m_masslessOpaqueMaterial,
    OptionalDoubleGetter(std::bind(&model::MasslessOpaqueMaterial::visibleAbsorptance, m_masslessOpaqueMaterial.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::MasslessOpaqueMaterial::*)(double)>(&model::MasslessOpaqueMaterial::setVisibleAbsorptance), m_masslessOpaqueMaterial.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::MasslessOpaqueMaterial::resetVisibleAbsorptance, m_masslessOpaqueMaterial.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::MasslessOpaqueMaterial::isVisibleAbsorptanceDefaulted, m_masslessOpaqueMaterial.get_ptr()))
  );

  m_standardsInformationWidget->attach(masslessOpaqueMaterial);

  this->stackedWidget()->setCurrentIndex(1);
}

void MaterialNoMassInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_nameEdit->unbind();

  m_roughness->unbind();

  m_thermalResistance->unbind();
  m_thermalAbsorptance->unbind();
  m_solarAbsorptance->unbind();
  m_visibleAbsorptance->unbind();

  m_standardsInformationWidget->detach();
}

void MaterialNoMassInspectorView::refresh()
{
}

} // openstudio
