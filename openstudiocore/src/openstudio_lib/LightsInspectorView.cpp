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

  // Lighting Level

  label = new QLabel("Lighting Power: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,2,0);

  m_lightingLevelEdit = new OSQuantityEdit2("W","W","W", m_isIP);
  connect(this, &LightsDefinitionInspectorView::toggleUnitsClicked, m_lightingLevelEdit, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_lightingLevelEdit,3,0);

  // Watts Per Space Floor Area

  label = new QLabel("Watts Per Space Floor Area: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,2,1);

  m_wattsPerSpaceFloorAreaEdit = new OSQuantityEdit2("W/m^2","W/m^2","W/ft^2", m_isIP);
  connect(this, &LightsDefinitionInspectorView::toggleUnitsClicked, m_wattsPerSpaceFloorAreaEdit, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_wattsPerSpaceFloorAreaEdit,3,1);

  // Watts Per Person

  label = new QLabel("Watts Per Person: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,2,2);

  m_wattsPerPersonEdit = new OSQuantityEdit2("W/person","W/person","W/person", m_isIP);
  connect(this, &LightsDefinitionInspectorView::toggleUnitsClicked, m_wattsPerPersonEdit, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_wattsPerPersonEdit,3,2);

  // Fraction Radiant

  label = new QLabel("Fraction Radiant: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,4,0);

  m_fractionRadiantEdit = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &LightsDefinitionInspectorView::toggleUnitsClicked, m_fractionRadiantEdit, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_fractionRadiantEdit,5,0);

  // Fraction Visible

  label = new QLabel("Fraction Visible: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,4,1);

  m_fractionVisibleEdit = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &LightsDefinitionInspectorView::toggleUnitsClicked, m_fractionVisibleEdit, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_fractionVisibleEdit,5,1);

  // Return Air Fraction

  label = new QLabel("Return Air Fraction: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,6,0);

  m_returnAirFractionEdit = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &LightsDefinitionInspectorView::toggleUnitsClicked, m_returnAirFractionEdit, &OSQuantityEdit2::onUnitSystemChange);
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
  m_lightsDefinition = lightsDefinition;
  // m_nameEdit->bind(lightsDefinition,"name");

  m_nameEdit->bind(
    *m_lightsDefinition,
    OptionalStringGetter(std::bind(&model::LightsDefinition::name, m_lightsDefinition.get_ptr(),true)),
    boost::optional<StringSetter>(std::bind(&model::LightsDefinition::setName, m_lightsDefinition.get_ptr(),std::placeholders::_1))
  );

  // m_lightingLevelEdit->bind(lightsDefinition,"lightingLevel",m_isIP);
  m_lightingLevelEdit->bind(
    m_isIP,
    *m_lightsDefinition,
    OptionalDoubleGetter(std::bind(&model::LightsDefinition::lightingLevel, m_lightsDefinition.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::LightsDefinition::*)(double)>(&model::LightsDefinition::setLightingLevel), m_lightsDefinition.get_ptr(), std::placeholders::_1))
  );

  // m_wattsPerSpaceFloorAreaEdit->bind(lightsDefinition,"wattsperSpaceFloorArea",m_isIP);
  m_wattsPerSpaceFloorAreaEdit->bind(
    m_isIP,
    *m_lightsDefinition,
    OptionalDoubleGetter(std::bind(&model::LightsDefinition::wattsperSpaceFloorArea, m_lightsDefinition.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::LightsDefinition::*)(double)>(&model::LightsDefinition::setWattsperSpaceFloorArea), m_lightsDefinition.get_ptr(), std::placeholders::_1))
  );

  // m_wattsPerPersonEdit->bind(lightsDefinition,"wattsperPerson",m_isIP);
  m_wattsPerPersonEdit->bind(
    m_isIP,
    *m_lightsDefinition,
    OptionalDoubleGetter(std::bind(&model::LightsDefinition::wattsperPerson, m_lightsDefinition.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::LightsDefinition::*)(double)>(&model::LightsDefinition::setWattsperPerson), m_lightsDefinition.get_ptr(), std::placeholders::_1))
  );

  // m_fractionRadiantEdit->bind(lightsDefinition,"fractionRadiant",m_isIP);
  m_fractionRadiantEdit->bind(
    m_isIP,
    *m_lightsDefinition,
    OptionalDoubleGetter(std::bind(&model::LightsDefinition::fractionRadiant, m_lightsDefinition.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::LightsDefinition::*)(double)>(&model::LightsDefinition::setFractionRadiant), m_lightsDefinition.get_ptr(), std::placeholders::_1))
  );

  // m_fractionVisibleEdit->bind(lightsDefinition,"fractionVisible",m_isIP);
  m_fractionVisibleEdit->bind(
    m_isIP,
    *m_lightsDefinition,
    OptionalDoubleGetter(std::bind(&model::LightsDefinition::fractionVisible, m_lightsDefinition.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::LightsDefinition::*)(double)>(&model::LightsDefinition::setFractionVisible), m_lightsDefinition.get_ptr(), std::placeholders::_1))
  );

  // m_returnAirFractionEdit->bind(lightsDefinition,"returnAirFraction",m_isIP);
  m_fractionVisibleEdit->bind(
    m_isIP,
    *m_lightsDefinition,
    OptionalDoubleGetter(std::bind(&model::LightsDefinition::returnAirFraction, m_lightsDefinition.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::LightsDefinition::*)(double)>(&model::LightsDefinition::setReturnAirFraction), m_lightsDefinition.get_ptr(), std::placeholders::_1))
  );

  this->stackedWidget()->setCurrentIndex(1);
}

void LightsDefinitionInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_nameEdit->unbind();
  m_lightingLevelEdit->unbind();
  m_wattsPerSpaceFloorAreaEdit->unbind();
  m_wattsPerPersonEdit->unbind();

  m_lightsDefinition = boost::none;
}

void LightsDefinitionInspectorView::refresh()
{
}

void LightsDefinitionInspectorView::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

} // openstudio

