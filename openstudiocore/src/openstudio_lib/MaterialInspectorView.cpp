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

#include "MaterialInspectorView.hpp"

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
  : MaterialBaseInspectorView(isIP, model, parent)
{
  createLayout();
}

void MaterialInspectorView::createLayout()
{
  MaterialBaseInspectorView::createLayout(); // call parent implementation

  QVBoxLayout * vLayout = nullptr;

  QLabel * label = nullptr;

  unsigned row = m_mainGridLayout->rowCount();

  // Roughness
  vLayout = new QVBoxLayout();

  label = new QLabel("Roughness: ");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_roughness = new OSComboBox();
  m_roughness->addItem("Very Rough");
  m_roughness->addItem("Rough");
  m_roughness->addItem("Medium Rough");
  m_roughness->addItem("Medium Smooth");
  m_roughness->addItem("Smooth");
  m_roughness->addItem("Very Smooth");
  vLayout->addWidget(m_roughness);

  m_mainGridLayout->addLayout(vLayout, row, 0);

  // Thickness
  vLayout = new QVBoxLayout();

  label = new QLabel("Thickness: ");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_thickness = new OSQuantityEdit(m_isIP);
  connect(this, &MaterialInspectorView::toggleUnitsClicked, m_thickness, &OSQuantityEdit::onUnitSystemChange);
  vLayout->addWidget(m_thickness);

  m_mainGridLayout->addLayout(vLayout, row++, 1);

  // Conductivity
  vLayout = new QVBoxLayout();

  label = new QLabel("Conductivity: ");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_conductivity = new OSQuantityEdit(m_isIP);
  connect(this, &MaterialInspectorView::toggleUnitsClicked, m_conductivity, &OSQuantityEdit::onUnitSystemChange);
  vLayout->addWidget(m_conductivity);

  m_mainGridLayout->addLayout(vLayout, row, 0);

  // Density
  vLayout = new QVBoxLayout();

  label = new QLabel("Density: ");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_density = new OSQuantityEdit(m_isIP);
  connect(this, &MaterialInspectorView::toggleUnitsClicked, m_density, &OSQuantityEdit::onUnitSystemChange);
  vLayout->addWidget(m_density);

  m_mainGridLayout->addLayout(vLayout, row++, 1);

  // Specific Heat
  vLayout = new QVBoxLayout();

  label = new QLabel("Specific Heat: ");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_specificHeat = new OSQuantityEdit(m_isIP);
  connect(this, &MaterialInspectorView::toggleUnitsClicked, m_specificHeat, &OSQuantityEdit::onUnitSystemChange);
  vLayout->addWidget(m_specificHeat);

  m_mainGridLayout->addLayout(vLayout, row, 0);

  // Thermal Absorptance
  vLayout = new QVBoxLayout();

  label = new QLabel("Thermal Absorptance: ");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_thermalAbsorptance = new OSQuantityEdit(m_isIP);
  connect(this, &MaterialInspectorView::toggleUnitsClicked, m_thermalAbsorptance, &OSQuantityEdit::onUnitSystemChange);
  vLayout->addWidget(m_thermalAbsorptance);

  m_mainGridLayout->addLayout(vLayout, row++, 1);

  // Solar Absorptance
  vLayout = new QVBoxLayout();

  label = new QLabel("Solar Absorptance: ");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_solarAbsorptance = new OSQuantityEdit(m_isIP);
  connect(this, &MaterialInspectorView::toggleUnitsClicked, m_solarAbsorptance, &OSQuantityEdit::onUnitSystemChange);
  vLayout->addWidget(m_solarAbsorptance);

  m_mainGridLayout->addLayout(vLayout, row, 0);

  // Visible Absorptance
  vLayout = new QVBoxLayout();

  label = new QLabel("Visible Absorptance: ");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_visibleAbsorptance = new OSQuantityEdit(m_isIP);
  connect(this, &MaterialInspectorView::toggleUnitsClicked, m_visibleAbsorptance, &OSQuantityEdit::onUnitSystemChange);
  vLayout->addWidget(m_visibleAbsorptance);

  m_mainGridLayout->addLayout(vLayout, row++, 1);

  // Stretch

  m_mainGridLayout->setRowStretch(100,100);

  m_mainGridLayout->setColumnStretch(100,100);
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

void MaterialInspectorView::attach(openstudio::model::Material & standardOpaqueMaterial)
{
  m_nameEdit->bind(standardOpaqueMaterial, "name"); // TODO

  m_roughness->bind(standardOpaqueMaterial,"roughness");

  m_thickness->bind(standardOpaqueMaterial,"thickness",m_isIP);
  m_conductivity->bind(standardOpaqueMaterial,"conductivity",m_isIP);
  m_density->bind(standardOpaqueMaterial,"density",m_isIP);
  m_specificHeat->bind(standardOpaqueMaterial,"specificHeat",m_isIP);
  m_thermalAbsorptance->bind(standardOpaqueMaterial,"thermalAbsorptance",m_isIP); // Fractional, no units
  m_solarAbsorptance->bind(standardOpaqueMaterial,"solarAbsorptance",m_isIP); // Fractional, no units
  m_visibleAbsorptance->bind(standardOpaqueMaterial,"visibleAbsorptance",m_isIP); // Fractional, no units

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
}

void MaterialInspectorView::refresh()
{
}

} // openstudio
