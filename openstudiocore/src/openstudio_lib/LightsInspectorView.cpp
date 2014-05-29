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

#include "LightsInspectorView.hpp"
#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSQuantityEdit.hpp"
#include "OSDropZone.hpp"
#include "../model/LightsDefinition.hpp"
#include "../model/LightsDefinition_Impl.hpp"
#include "../utilities/core/Assert.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGridLayout>
#include <QScrollArea>
#include <QStackedWidget>

namespace openstudio {

LightsDefinitionInspectorView::LightsDefinitionInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent)
  : ModelObjectInspectorView(model, true, parent)
{
  m_isIP = isIP;

  QWidget* visibleWidget = new QWidget();
  this->stackedWidget()->addWidget(visibleWidget);

  QGridLayout* mainGridLayout = new QGridLayout();
  mainGridLayout->setContentsMargins(7,7,7,7);
  mainGridLayout->setSpacing(14);
  visibleWidget->setLayout(mainGridLayout);

  // Name

  QLabel* label = new QLabel("Name: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,0,0);

  m_nameEdit = new OSLineEdit();
  mainGridLayout->addWidget(m_nameEdit,1,0,1,3);

  // Lighting Level

  label = new QLabel("Lighting Power: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,2,0);

  bool isConnected = false;

  m_lightingLevelEdit = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_lightingLevelEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_lightingLevelEdit,3,0);

  // Energy Per Space Floor Area

  label = new QLabel("Energy Per Space Floor Area: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,2,1);

  m_wattsPerSpaceFloorAreaEdit = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_wattsPerSpaceFloorAreaEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_wattsPerSpaceFloorAreaEdit,3,1);

  // Energy Per Person

  label = new QLabel("Energy Per Person: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,2,2);

  m_wattsPerPersonEdit = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_wattsPerPersonEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_wattsPerPersonEdit,3,2);

  // Fraction Radiant

  label = new QLabel("Fraction Radiant: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,4,0);

  m_fractionRadiantEdit = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_fractionRadiantEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_fractionRadiantEdit,5,0);

  // Fraction Visible

  label = new QLabel("Fraction Visible: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,4,1);

  m_fractionVisibleEdit = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_fractionVisibleEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_fractionVisibleEdit,5,1);

  // Return Air Fraction

  label = new QLabel("Return Air Fraction: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,6,0);

  m_returnAirFractionEdit = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_returnAirFractionEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_returnAirFractionEdit,7,0);

  // Stretch

  mainGridLayout->setRowStretch(8,100);

  mainGridLayout->setColumnStretch(3,100);
}

void LightsDefinitionInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  detach();
}

void LightsDefinitionInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::LightsDefinition lightsDefinition = modelObject.cast<model::LightsDefinition>();
  attach(lightsDefinition);
  refresh();
}

void LightsDefinitionInspectorView::onUpdate()
{
  refresh();
}

void LightsDefinitionInspectorView::attach(openstudio::model::LightsDefinition & lightsDefinition)
{
  m_nameEdit->bind(lightsDefinition,"name");
  m_lightingLevelEdit->bind(lightsDefinition,"lightingLevel",m_isIP);
  m_wattsPerSpaceFloorAreaEdit->bind(lightsDefinition,"wattsperSpaceFloorArea",m_isIP);
  m_wattsPerPersonEdit->bind(lightsDefinition,"wattsperPerson",m_isIP);
  m_fractionRadiantEdit->bind(lightsDefinition,"fractionRadiant",m_isIP);
  m_fractionVisibleEdit->bind(lightsDefinition,"fractionVisible",m_isIP);
  m_returnAirFractionEdit->bind(lightsDefinition,"returnAirFraction",m_isIP);

  this->stackedWidget()->setCurrentIndex(1);
}

void LightsDefinitionInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_nameEdit->unbind();
  m_lightingLevelEdit->unbind();
  m_wattsPerSpaceFloorAreaEdit->unbind();
  m_wattsPerPersonEdit->unbind();
}

void LightsDefinitionInspectorView::refresh()
{
}

void LightsDefinitionInspectorView::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

} // openstudio

