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

#include "SteamEquipmentInspectorView.hpp"
#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSQuantityEdit.hpp"
#include "OSDropZone.hpp"
#include "../model/SteamEquipmentDefinition.hpp"
#include "../model/SteamEquipmentDefinition_Impl.hpp"
#include "../utilities/core/Assert.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGridLayout>
#include <QScrollArea>
#include <QStackedWidget>

namespace openstudio {

SteamEquipmentDefinitionInspectorView::SteamEquipmentDefinitionInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent)
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
  connect(this, &SteamEquipmentDefinitionInspectorView::toggleUnitsClicked, m_designLevelEdit, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_designLevelEdit,3,0);

  // Energy Per Space Floor Area

  label = new QLabel("Energy Per Space Floor Area: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,2,1);

  m_wattsPerSpaceFloorAreaEdit = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &SteamEquipmentDefinitionInspectorView::toggleUnitsClicked, m_wattsPerSpaceFloorAreaEdit, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_wattsPerSpaceFloorAreaEdit,3,1);

  // Energy Per Person

  label = new QLabel("Energy Per Person: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,2,2);

  m_wattsPerPersonEdit = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &SteamEquipmentDefinitionInspectorView::toggleUnitsClicked, m_wattsPerPersonEdit, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_wattsPerPersonEdit,3,2);

  // Fraction Latent

  label = new QLabel("Fraction Latent: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,4,0);

  m_fractionLatentEdit = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &SteamEquipmentDefinitionInspectorView::toggleUnitsClicked, m_fractionLatentEdit, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_fractionLatentEdit,5,0);

  // Fraction Radiant

  label = new QLabel("Fraction Radiant: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,4,1);

  m_fractionRadiantEdit = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &SteamEquipmentDefinitionInspectorView::toggleUnitsClicked, m_fractionRadiantEdit, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_fractionRadiantEdit,5,1);

  // Fraction Lost

  label = new QLabel("Fraction Lost: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,6,0);

  m_fractionLostEdit = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &SteamEquipmentDefinitionInspectorView::toggleUnitsClicked, m_fractionRadiantEdit, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_fractionLostEdit,7,0);

  // Stretch

  mainGridLayout->setRowStretch(8,100);

  mainGridLayout->setColumnStretch(3,100);
}

void SteamEquipmentDefinitionInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  detach();
}

void SteamEquipmentDefinitionInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::SteamEquipmentDefinition steamEquipmentDefinition = modelObject.cast<model::SteamEquipmentDefinition>();
  attach(steamEquipmentDefinition);
  refresh();
}

void SteamEquipmentDefinitionInspectorView::onUpdate()
{
  refresh();
}

void SteamEquipmentDefinitionInspectorView::attach(openstudio::model::SteamEquipmentDefinition & steamEquipmentDefinition)
{
  // m_nameEdit->bind(steamEquipmentDefinition,"name");
  m_steamEquipmentDefinition = steamEquipmentDefinition;
  m_nameEdit->bind(
    *m_steamEquipmentDefinition,
    OptionalStringGetter(std::bind(&model::SteamEquipmentDefinition::name, m_steamEquipmentDefinition.get_ptr(),true)),
    boost::optional<StringSetter>(std::bind(&model::SteamEquipmentDefinition::setName, m_steamEquipmentDefinition.get_ptr(),std::placeholders::_1))
  );

  // m_designLevelEdit->bind(steamEquipmentDefinition,"designLevel",m_isIP);
  m_designLevelEdit->bind(
    m_isIP,
    *m_steamEquipmentDefinition,
    OptionalDoubleGetter(std::bind(&model::SteamEquipmentDefinition::designLevel, m_steamEquipmentDefinition.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::SteamEquipmentDefinition::*)(double)>(&model::SteamEquipmentDefinition::setDesignLevel), m_steamEquipmentDefinition.get_ptr(), std::placeholders::_1))
  );

  // m_wattsPerSpaceFloorAreaEdit->bind(steamEquipmentDefinition,"wattsperSpaceFloorArea",m_isIP);
  m_wattsPerSpaceFloorAreaEdit->bind(
    m_isIP,
    *m_steamEquipmentDefinition,
    OptionalDoubleGetter(std::bind(&model::SteamEquipmentDefinition::wattsperSpaceFloorArea, m_steamEquipmentDefinition.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::SteamEquipmentDefinition::*)(double)>(&model::SteamEquipmentDefinition::setWattsperSpaceFloorArea), m_steamEquipmentDefinition.get_ptr(), std::placeholders::_1))
  );

  // m_wattsPerPersonEdit->bind(steamEquipmentDefinition,"wattsperPerson",m_isIP);
  m_wattsPerPersonEdit->bind(
    m_isIP,
    *m_steamEquipmentDefinition,
    OptionalDoubleGetter(std::bind(&model::SteamEquipmentDefinition::wattsperPerson, m_steamEquipmentDefinition.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::SteamEquipmentDefinition::*)(double)>(&model::SteamEquipmentDefinition::setWattsperPerson), m_steamEquipmentDefinition.get_ptr(), std::placeholders::_1))
  );

  // m_fractionLatentEdit->bind(steamEquipmentDefinition,"fractionLatent",m_isIP);
  m_fractionLatentEdit->bind(
    m_isIP,
    *m_steamEquipmentDefinition,
    DoubleGetter(std::bind(&model::SteamEquipmentDefinition::fractionLatent, m_steamEquipmentDefinition.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::SteamEquipmentDefinition::*)(double)>(&model::SteamEquipmentDefinition::setFractionLatent), m_steamEquipmentDefinition.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::SteamEquipmentDefinition::resetFractionLatent, m_steamEquipmentDefinition.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::SteamEquipmentDefinition::isFractionLatentDefaulted, m_steamEquipmentDefinition.get_ptr()))
  );

  // m_fractionRadiantEdit->bind(steamEquipmentDefinition,"fractionRadiant",m_isIP);
  m_fractionRadiantEdit->bind(
    m_isIP,
    *m_steamEquipmentDefinition,
    DoubleGetter(std::bind(&model::SteamEquipmentDefinition::fractionRadiant, m_steamEquipmentDefinition.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::SteamEquipmentDefinition::*)(double)>(&model::SteamEquipmentDefinition::setFractionRadiant), m_steamEquipmentDefinition.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::SteamEquipmentDefinition::resetFractionRadiant, m_steamEquipmentDefinition.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::SteamEquipmentDefinition::isFractionRadiantDefaulted, m_steamEquipmentDefinition.get_ptr()))
  );

  // m_fractionLostEdit->bind(steamEquipmentDefinition,"fractionLost",m_isIP);
  m_fractionLostEdit->bind(
    m_isIP,
    *m_steamEquipmentDefinition,
    DoubleGetter(std::bind(&model::SteamEquipmentDefinition::fractionLost, m_steamEquipmentDefinition.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::SteamEquipmentDefinition::*)(double)>(&model::SteamEquipmentDefinition::setFractionLost), m_steamEquipmentDefinition.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::SteamEquipmentDefinition::resetFractionLost, m_steamEquipmentDefinition.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::SteamEquipmentDefinition::isFractionLostDefaulted, m_steamEquipmentDefinition.get_ptr()))
  );

  this->stackedWidget()->setCurrentIndex(1);
}

void SteamEquipmentDefinitionInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_nameEdit->unbind();
  m_designLevelEdit->unbind();
  m_wattsPerSpaceFloorAreaEdit->unbind();
  m_wattsPerPersonEdit->unbind();
  m_fractionLatentEdit->unbind();
  m_fractionRadiantEdit->unbind();
  m_fractionLostEdit->unbind();

  m_steamEquipmentDefinition = boost::none;
}

void SteamEquipmentDefinitionInspectorView::refresh()
{
}

void SteamEquipmentDefinitionInspectorView::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

} // openstudio

