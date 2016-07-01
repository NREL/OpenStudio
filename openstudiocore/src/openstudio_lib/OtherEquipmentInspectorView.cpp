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

  auto visibleWidget = new QWidget();
  this->stackedWidget()->addWidget(visibleWidget);

  auto mainGridLayout = new QGridLayout();
  mainGridLayout->setContentsMargins(7,7,7,7);
  mainGridLayout->setSpacing(14);
  visibleWidget->setLayout(mainGridLayout);

  // Name

  QLabel* label = new QLabel("Name: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,0,0);

  m_nameEdit = new OSLineEdit2();
  mainGridLayout->addWidget(m_nameEdit,1,0,1,3);

  // Design Level

  label = new QLabel("Design Level: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,2,0);

  m_designLevelEdit = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &OtherEquipmentDefinitionInspectorView::toggleUnitsClicked, m_designLevelEdit, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_designLevelEdit,3,0);

  // Energy Per Space Floor Area

  label = new QLabel("Energy Per Space Floor Area: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,2,1);

  m_wattsPerSpaceFloorAreaEdit = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &OtherEquipmentDefinitionInspectorView::toggleUnitsClicked, m_wattsPerSpaceFloorAreaEdit, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_wattsPerSpaceFloorAreaEdit,3,1);

  // Energy Per Person

  label = new QLabel("Energy Per Person: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,2,2);

  m_wattsPerPersonEdit = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &OtherEquipmentDefinitionInspectorView::toggleUnitsClicked, m_wattsPerPersonEdit, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_wattsPerPersonEdit,3,2);

  // Fraction Latent

  label = new QLabel("Fraction Latent: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,4,0);

  m_fractionLatentEdit = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &OtherEquipmentDefinitionInspectorView::toggleUnitsClicked, m_fractionLatentEdit, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_fractionLatentEdit,5,0);

  // Fraction Radiant

  label = new QLabel("Fraction Radiant: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,4,1);

  m_fractionRadiantEdit = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &OtherEquipmentDefinitionInspectorView::toggleUnitsClicked, m_fractionRadiantEdit, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_fractionRadiantEdit,5,1);

  // Fraction Lost

  label = new QLabel("Fraction Lost: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,6,0);

  m_fractionLostEdit = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &OtherEquipmentDefinitionInspectorView::toggleUnitsClicked, m_fractionLostEdit, &OSQuantityEdit2::onUnitSystemChange);
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
  boost::optional<model::OtherEquipmentDefinition> m_otherEquipmentDefinition = otherEquipmentDefinition;
  // m_nameEdit->bind(otherEquipmentDefinition,"name");
  m_nameEdit->bind(
    *m_otherEquipmentDefinition,
    OptionalStringGetter(std::bind(&model::OtherEquipmentDefinition::name, m_otherEquipmentDefinition.get_ptr(),true)),
    boost::optional<StringSetter>(std::bind(&model::OtherEquipmentDefinition::setName, m_otherEquipmentDefinition.get_ptr(),std::placeholders::_1))
  );

  // m_designLevelEdit->bind(otherEquipmentDefinition,"designLevel",m_isIP);
  m_designLevelEdit->bind(
    m_isIP,
    *m_otherEquipmentDefinition,
    OptionalDoubleGetter(std::bind(&model::OtherEquipmentDefinition::designLevel, m_otherEquipmentDefinition.get_ptr())),
    DoubleSetterVoidReturn(std::bind(static_cast<void(model::OtherEquipmentDefinition::*)(double)>(&model::OtherEquipmentDefinition::setDesignLevel), m_otherEquipmentDefinition.get_ptr(), std::placeholders::_1))
  );

  // m_wattsPerSpaceFloorAreaEdit->bind(otherEquipmentDefinition,"wattsperSpaceFloorArea",m_isIP);
  m_wattsPerSpaceFloorAreaEdit->bind(
    m_isIP,
    *m_otherEquipmentDefinition,
    OptionalDoubleGetter(std::bind(&model::OtherEquipmentDefinition::wattsperSpaceFloorArea, m_otherEquipmentDefinition.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::OtherEquipmentDefinition::*)(double)>(&model::OtherEquipmentDefinition::setWattsperSpaceFloorArea), m_otherEquipmentDefinition.get_ptr(), std::placeholders::_1))
  );

  // m_wattsPerPersonEdit->bind(otherEquipmentDefinition,"wattsperPerson",m_isIP);
  m_wattsPerPersonEdit->bind(
    m_isIP,
    *m_otherEquipmentDefinition,
    OptionalDoubleGetter(std::bind(&model::OtherEquipmentDefinition::wattsperPerson, m_otherEquipmentDefinition.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::OtherEquipmentDefinition::*)(double)>(&model::OtherEquipmentDefinition::setWattsperPerson), m_otherEquipmentDefinition.get_ptr(), std::placeholders::_1))
  );

  // m_fractionLatentEdit->bind(otherEquipmentDefinition,"fractionLatent",m_isIP);
  m_fractionLatentEdit->bind(
    m_isIP,
    *m_otherEquipmentDefinition,
    DoubleGetter(std::bind(&model::OtherEquipmentDefinition::fractionLatent, m_otherEquipmentDefinition.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::OtherEquipmentDefinition::*)(double)>(&model::OtherEquipmentDefinition::setFractionLatent), m_otherEquipmentDefinition.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::OtherEquipmentDefinition::resetFractionLatent, m_otherEquipmentDefinition.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::OtherEquipmentDefinition::isFractionLatentDefaulted, m_otherEquipmentDefinition.get_ptr()))
  );

  // m_fractionRadiantEdit->bind(otherEquipmentDefinition,"fractionRadiant",m_isIP);
  m_fractionRadiantEdit->bind(
    m_isIP,
    *m_otherEquipmentDefinition,
    DoubleGetter(std::bind(&model::OtherEquipmentDefinition::fractionRadiant, m_otherEquipmentDefinition.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::OtherEquipmentDefinition::*)(double)>(&model::OtherEquipmentDefinition::setFractionRadiant), m_otherEquipmentDefinition.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::OtherEquipmentDefinition::resetFractionRadiant, m_otherEquipmentDefinition.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::OtherEquipmentDefinition::isFractionRadiantDefaulted, m_otherEquipmentDefinition.get_ptr()))
  );

  // m_fractionLostEdit->bind(otherEquipmentDefinition,"fractionLost",m_isIP);
  m_fractionLostEdit->bind(
    m_isIP,
    *m_otherEquipmentDefinition,
    DoubleGetter(std::bind(&model::OtherEquipmentDefinition::fractionLost, m_otherEquipmentDefinition.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::OtherEquipmentDefinition::*)(double)>(&model::OtherEquipmentDefinition::setFractionLost), m_otherEquipmentDefinition.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::OtherEquipmentDefinition::resetFractionLost, m_otherEquipmentDefinition.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::OtherEquipmentDefinition::isFractionLostDefaulted, m_otherEquipmentDefinition.get_ptr()))
  );

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

