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

#include "OtherEquipmentInspectorView.hpp"
#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSQuantityEdit.hpp"
#include "OSDropZone.hpp"
#include "../model/OtherEquipmentDefinition.hpp"
#include "../model/OtherEquipmentDefinition_Impl.hpp"
#include "../utilities/core/Assert.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGridLayout>
#include <QScrollArea>
#include <QStackedWidget>

namespace openstudio {

OtherEquipmentDefinitionInspectorView::OtherEquipmentDefinitionInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent)
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

  // Design Level

  label = new QLabel("Design Level: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,2,0);

  bool isConnected = false;

  m_designLevelEdit = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_designLevelEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_designLevelEdit,3,0);

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

  // Fraction Latent

  label = new QLabel("Fraction Latent: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,4,0);

  m_fractionLatentEdit = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_fractionLatentEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_fractionLatentEdit,5,0);

  // Fraction Radiant

  label = new QLabel("Fraction Radiant: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,4,1);

  m_fractionRadiantEdit = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_fractionRadiantEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_fractionRadiantEdit,5,1);

  // Fraction Lost

  label = new QLabel("Fraction Lost: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,6,0);

  m_fractionLostEdit = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_fractionLostEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_fractionLostEdit,7,0);

  // Stretch

  mainGridLayout->setRowStretch(8,100);

  mainGridLayout->setColumnStretch(3,100);
}

void OtherEquipmentDefinitionInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  detach();
}

void OtherEquipmentDefinitionInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::OtherEquipmentDefinition otherEquipmentDefinition = modelObject.cast<model::OtherEquipmentDefinition>();
  attach(otherEquipmentDefinition);
  refresh();
}

void OtherEquipmentDefinitionInspectorView::onUpdate()
{
  refresh();
}

void OtherEquipmentDefinitionInspectorView::attach(openstudio::model::OtherEquipmentDefinition & otherEquipmentDefinition)
{
  m_nameEdit->bind(otherEquipmentDefinition,"name");
  m_designLevelEdit->bind(otherEquipmentDefinition,"designLevel",m_isIP);
  m_wattsPerSpaceFloorAreaEdit->bind(otherEquipmentDefinition,"wattsperSpaceFloorArea",m_isIP);
  m_wattsPerPersonEdit->bind(otherEquipmentDefinition,"wattsperPerson",m_isIP);
  m_fractionLatentEdit->bind(otherEquipmentDefinition,"fractionLatent",m_isIP);
  m_fractionRadiantEdit->bind(otherEquipmentDefinition,"fractionRadiant",m_isIP);
  m_fractionLostEdit->bind(otherEquipmentDefinition,"fractionLost",m_isIP);

  this->stackedWidget()->setCurrentIndex(1);
}

void OtherEquipmentDefinitionInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_nameEdit->unbind();
  m_designLevelEdit->unbind();
  m_wattsPerSpaceFloorAreaEdit->unbind();
  m_wattsPerPersonEdit->unbind();
  m_fractionLatentEdit->unbind();
  m_fractionRadiantEdit->unbind();
  m_fractionLostEdit->unbind();
}

void OtherEquipmentDefinitionInspectorView::refresh()
{
}

void OtherEquipmentDefinitionInspectorView::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

} // openstudio

