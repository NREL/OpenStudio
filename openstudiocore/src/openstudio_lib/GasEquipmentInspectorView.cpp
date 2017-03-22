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

#include "GasEquipmentInspectorView.hpp"
#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSQuantityEdit.hpp"
#include "OSDropZone.hpp"
#include "../model/GasEquipmentDefinition.hpp"
#include "../model/GasEquipmentDefinition_Impl.hpp"
#include "../utilities/core/Assert.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGridLayout>
#include <QScrollArea>
#include <QStackedWidget>

namespace openstudio {

GasEquipmentDefinitionInspectorView::GasEquipmentDefinitionInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent)
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

  m_designLevelEdit = new OSQuantityEdit2("W","W","Btu/h", m_isIP);
  connect(this, &GasEquipmentDefinitionInspectorView::toggleUnitsClicked, m_designLevelEdit, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_designLevelEdit,3,0);

  // Power Per Space Floor Area

  label = new QLabel("Power Per Space Floor Area: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,2,1);

  m_wattsPerSpaceFloorAreaEdit = new OSQuantityEdit2("W/m^2","W/m^2","Btu/hr*ft^2", m_isIP);
  connect(this, &GasEquipmentDefinitionInspectorView::toggleUnitsClicked, m_wattsPerSpaceFloorAreaEdit, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_wattsPerSpaceFloorAreaEdit,3,1);

  // Power Per Person

  label = new QLabel("Power Per Person: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,2,2);

  m_wattsPerPersonEdit = new OSQuantityEdit2("W/person","W/person","Btu/hr*person", m_isIP);
  connect(this, &GasEquipmentDefinitionInspectorView::toggleUnitsClicked, m_wattsPerPersonEdit, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_wattsPerPersonEdit,3,2);

  // Fraction Latent

  label = new QLabel("Fraction Latent: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,4,0);

  m_fractionLatentEdit = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &GasEquipmentDefinitionInspectorView::toggleUnitsClicked, m_fractionLatentEdit, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_fractionLatentEdit,5,0);

  // Fraction Radiant

  label = new QLabel("Fraction Radiant: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,4,1);

  m_fractionRadiantEdit = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &GasEquipmentDefinitionInspectorView::toggleUnitsClicked, m_fractionRadiantEdit, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_fractionRadiantEdit,5,1);

  // Fraction Lost

  label = new QLabel("Fraction Lost: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,6,0);

  m_fractionLostEdit = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &GasEquipmentDefinitionInspectorView::toggleUnitsClicked, m_fractionLostEdit, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_fractionLostEdit,7,0);

  // Carbon Dioxide Generation Rate

  label = new QLabel("Carbon Dioxide Generation Rate: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,8,0);

  m_carbonDioxideGenerationRateEdit = new OSQuantityEdit2("m^3/s*W", "L/s*W", "ft^3*hr/min*Btu", m_isIP);
  connect(this, &GasEquipmentDefinitionInspectorView::toggleUnitsClicked, m_carbonDioxideGenerationRateEdit, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_carbonDioxideGenerationRateEdit,9,0);

  // Stretch

  mainGridLayout->setRowStretch(10,100);

  mainGridLayout->setColumnStretch(3,100);
}

void GasEquipmentDefinitionInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  detach();
}

void GasEquipmentDefinitionInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::GasEquipmentDefinition gasEquipmentDefinition = modelObject.cast<model::GasEquipmentDefinition>();
  attach(gasEquipmentDefinition);
  refresh();
}

void GasEquipmentDefinitionInspectorView::onUpdate()
{
  refresh();
}

void GasEquipmentDefinitionInspectorView::attach(openstudio::model::GasEquipmentDefinition & gasEquipmentDefinition)
{
  m_gasEquipmentDefinition = gasEquipmentDefinition;

  // m_nameEdit->bind(gasEquipmentDefinition,"name");
  m_nameEdit->bind(
    *m_gasEquipmentDefinition,
    OptionalStringGetter(std::bind(&model::GasEquipmentDefinition::name, m_gasEquipmentDefinition.get_ptr(),true)),
    boost::optional<StringSetter>(std::bind(&model::GasEquipmentDefinition::setName, m_gasEquipmentDefinition.get_ptr(),std::placeholders::_1))
  );

  // m_designLevelEdit->bind(gasEquipmentDefinition,"designLevel",m_isIP);
  m_designLevelEdit->bind(
    m_isIP,
    *m_gasEquipmentDefinition,
    OptionalDoubleGetter(std::bind(&model::GasEquipmentDefinition::designLevel, m_gasEquipmentDefinition.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(&model::GasEquipmentDefinition::setDesignLevel, m_gasEquipmentDefinition.get_ptr(), std::placeholders::_1))
  );

  // m_wattsPerSpaceFloorAreaEdit->bind(gasEquipmentDefinition,"wattsperSpaceFloorArea",m_isIP);
  m_wattsPerSpaceFloorAreaEdit->bind(
    m_isIP,
    *m_gasEquipmentDefinition,
    OptionalDoubleGetter(std::bind(&model::GasEquipmentDefinition::wattsperSpaceFloorArea, m_gasEquipmentDefinition.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(&model::GasEquipmentDefinition::setWattsperSpaceFloorArea, m_gasEquipmentDefinition.get_ptr(), std::placeholders::_1))
  );

  // m_wattsPerPersonEdit->bind(gasEquipmentDefinition,"wattsperPerson",m_isIP);
  m_wattsPerPersonEdit->bind(
    m_isIP,
    *m_gasEquipmentDefinition,
    OptionalDoubleGetter(std::bind(&model::GasEquipmentDefinition::wattsperPerson, m_gasEquipmentDefinition.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(&model::GasEquipmentDefinition::setWattsperPerson, m_gasEquipmentDefinition.get_ptr(), std::placeholders::_1))
  );

  // m_fractionLatentEdit->bind(gasEquipmentDefinition,"fractionLatent",m_isIP);
  m_fractionLatentEdit->bind(
    m_isIP,
    *m_gasEquipmentDefinition,
    OptionalDoubleGetter(std::bind(&model::GasEquipmentDefinition::fractionLatent, m_gasEquipmentDefinition.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(&model::GasEquipmentDefinition::setFractionLatent, m_gasEquipmentDefinition.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::GasEquipmentDefinition::resetFractionLatent, m_gasEquipmentDefinition.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::GasEquipmentDefinition::isFractionLatentDefaulted, m_gasEquipmentDefinition.get_ptr()))
  );

  // m_fractionRadiantEdit->bind(gasEquipmentDefinition,"fractionRadiant",m_isIP);
  m_fractionRadiantEdit->bind(
    m_isIP,
    *m_gasEquipmentDefinition,
    OptionalDoubleGetter(std::bind(&model::GasEquipmentDefinition::fractionRadiant, m_gasEquipmentDefinition.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(&model::GasEquipmentDefinition::setFractionRadiant, m_gasEquipmentDefinition.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::GasEquipmentDefinition::resetFractionRadiant, m_gasEquipmentDefinition.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::GasEquipmentDefinition::isFractionRadiantDefaulted, m_gasEquipmentDefinition.get_ptr()))
  );

  // m_fractionLostEdit->bind(gasEquipmentDefinition,"fractionLost",m_isIP);
  m_fractionLostEdit->bind(
    m_isIP,
    *m_gasEquipmentDefinition,
    OptionalDoubleGetter(std::bind(&model::GasEquipmentDefinition::fractionLost, m_gasEquipmentDefinition.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(&model::GasEquipmentDefinition::setFractionLost, m_gasEquipmentDefinition.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::GasEquipmentDefinition::resetFractionLost, m_gasEquipmentDefinition.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::GasEquipmentDefinition::isFractionLostDefaulted, m_gasEquipmentDefinition.get_ptr()))
  );

  // m_carbonDioxideGenerationRateEdit->bind(gasEquipmentDefinition,"carbonDioxideGenerationRate",m_isIP);
  m_carbonDioxideGenerationRateEdit->bind(
    m_isIP,
    *m_gasEquipmentDefinition,
    OptionalDoubleGetter(std::bind(&model::GasEquipmentDefinition::carbonDioxideGenerationRate, m_gasEquipmentDefinition.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(&model::GasEquipmentDefinition::setCarbonDioxideGenerationRate, m_gasEquipmentDefinition.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::GasEquipmentDefinition::resetCarbonDioxideGenerationRate, m_gasEquipmentDefinition.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::GasEquipmentDefinition::isCarbonDioxideGenerationRateDefaulted, m_gasEquipmentDefinition.get_ptr()))
  );

  this->stackedWidget()->setCurrentIndex(1);
}

void GasEquipmentDefinitionInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_nameEdit->unbind();
  m_designLevelEdit->unbind();
  m_wattsPerSpaceFloorAreaEdit->unbind();
  m_wattsPerPersonEdit->unbind();
  m_fractionLatentEdit->unbind();
  m_fractionRadiantEdit->unbind();
  m_fractionLostEdit->unbind();
  m_carbonDioxideGenerationRateEdit->unbind();

  m_gasEquipmentDefinition = boost::none;
}

void GasEquipmentDefinitionInspectorView::refresh()
{
}

void GasEquipmentDefinitionInspectorView::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

} // openstudio

