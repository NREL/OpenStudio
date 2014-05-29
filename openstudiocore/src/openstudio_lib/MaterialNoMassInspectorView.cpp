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

#include "MaterialNoMassInspectorView.hpp"

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
  : ModelObjectInspectorView(model, true, parent),
    m_roughness(NULL),
    m_nameEdit(NULL),
    m_thermalResistance(NULL),
    m_thermalAbsorptance(NULL),
    m_solarAbsorptance(NULL),
    m_visibleAbsorptance(NULL),
    m_isIP(isIP)
{
  createLayout();
}

void MaterialNoMassInspectorView::createLayout()
{
  QWidget* visibleWidget = new QWidget();
  this->stackedWidget()->addWidget(visibleWidget);

  QGridLayout* mainGridLayout = new QGridLayout();
  mainGridLayout->setContentsMargins(7,7,7,7);
  mainGridLayout->setSpacing(14);
  visibleWidget->setLayout(mainGridLayout);

  unsigned row = 0;
  unsigned col = 0;

  bool isConnected = false;

  // Name

  QLabel * label = new QLabel("Name: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_nameEdit = new OSLineEdit();
  mainGridLayout->addWidget(m_nameEdit,row++,0,1,3);

  // Roughness

  label = new QLabel("Roughness: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_roughness = new OSComboBox();
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
  mainGridLayout->addWidget(label,row++,col);

  m_thermalResistance = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_thermalResistance, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_thermalResistance,row++,0,1,3);

  // Thermal Absorptance

  label = new QLabel("Thermal Absorptance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_thermalAbsorptance = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_thermalAbsorptance, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_thermalAbsorptance,row++,0,1,3);

  // Solar Absorptance

  label = new QLabel("Solar Absorptance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_solarAbsorptance = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_solarAbsorptance, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_solarAbsorptance,row++,0,1,3);

  // Visible Absorptance

  label = new QLabel("Visible Absorptance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_visibleAbsorptance = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_visibleAbsorptance, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
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
  m_roughness->bind(masslessOpaqueMaterial,"roughness");
  
  m_nameEdit->bind(masslessOpaqueMaterial,"name");
  m_thermalResistance->bind(masslessOpaqueMaterial,"thermalResistance",m_isIP);
  m_thermalAbsorptance->bind(masslessOpaqueMaterial,"thermalAbsorptance",m_isIP);
  m_solarAbsorptance->bind(masslessOpaqueMaterial,"solarAbsorptance",m_isIP);
  m_visibleAbsorptance->bind(masslessOpaqueMaterial,"visibleAbsorptance",m_isIP);

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
}

void MaterialNoMassInspectorView::refresh()
{
}

void MaterialNoMassInspectorView::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

} // openstudio
