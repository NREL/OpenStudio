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

#include "LuminaireInspectorView.hpp"
#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSQuantityEdit.hpp"
#include "OSDropZone.hpp"
#include "../model/LuminaireDefinition.hpp"
#include "../model/LuminaireDefinition_Impl.hpp"
#include "../utilities/core/Assert.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGridLayout>
#include <QScrollArea>
#include <QStackedWidget>

namespace openstudio {

LuminaireDefinitionInspectorView::LuminaireDefinitionInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent)
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

  QLabel * label = new QLabel("Name: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,0,0);

  m_nameEdit = new OSLineEdit();
  mainGridLayout->addWidget(m_nameEdit,1,0,1,2);

  // Lighting Power

  label = new QLabel("Lighting Power: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,2,0);

  bool isConnected = false;

  m_lightingPowerEdit = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_lightingPowerEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_lightingPowerEdit,3,0);

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

  mainGridLayout->setColumnStretch(2,100);
}

void LuminaireDefinitionInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  detach();
}

void LuminaireDefinitionInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::LuminaireDefinition luminiareDefinition = modelObject.cast<model::LuminaireDefinition>();
  attach(luminiareDefinition);
  refresh();
}

void LuminaireDefinitionInspectorView::onUpdate()
{
  refresh();
}

void LuminaireDefinitionInspectorView::attach(openstudio::model::LuminaireDefinition & luminaireDefinition)
{
  m_nameEdit->bind(luminaireDefinition,"name");
  m_lightingPowerEdit->bind(luminaireDefinition,"lightingPower",m_isIP);
  m_fractionRadiantEdit->bind(luminaireDefinition,"fractionRadiant",m_isIP);
  m_fractionVisibleEdit->bind(luminaireDefinition,"fractionVisible",m_isIP);
  m_returnAirFractionEdit->bind(luminaireDefinition,"returnAirFraction",m_isIP);

  this->stackedWidget()->setCurrentIndex(1);
}

void LuminaireDefinitionInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_nameEdit->unbind();
  m_lightingPowerEdit->unbind();
  m_fractionRadiantEdit->unbind();
  m_fractionVisibleEdit->unbind();
  m_returnAirFractionEdit->unbind();
}

void LuminaireDefinitionInspectorView::refresh()
{
}

void LuminaireDefinitionInspectorView::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

} // openstudio

