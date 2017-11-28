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

#include "PeopleInspectorView.hpp"


#include "../shared_gui_components/OSCheckBox.hpp"
#include "../shared_gui_components/OSComboBox.hpp"
#include "../shared_gui_components/OSDoubleEdit.hpp"
#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSQuantityEdit.hpp"

#include "../model/PeopleDefinition.hpp"
#include "../model/PeopleDefinition_Impl.hpp"
#include "../model/Schedule.hpp"
#include "../model/SpaceLoadInstance.hpp"

#include "../utilities/core/Assert.hpp"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QStackedWidget>
#include <QVBoxLayout>

namespace openstudio {

PeopleDefinitionInspectorView::PeopleDefinitionInspectorView(bool isIP, 
                                                             const openstudio::model::Model& model,
                                                             QWidget * parent)
  : ModelObjectInspectorView(model, true, parent)
{
  m_isIP = isIP;

  //QWidget* hiddenWidget = new QWidget();
  //this->stackedWidget()->insertWidget(0, hiddenWidget);

  auto visibleWidget = new QWidget();
  this->stackedWidget()->addWidget(visibleWidget);

  //this->stackedWidget()->setCurrentIndex(0);

  auto mainGridLayout = new QGridLayout();
  mainGridLayout->setContentsMargins(7,7,7,7);
  mainGridLayout->setSpacing(14);
  visibleWidget->setLayout(mainGridLayout);

  // name
  auto vLayout = new QVBoxLayout();

  QLabel* label = new QLabel("Name: ");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_nameEdit = new OSLineEdit2();
  vLayout->addWidget(m_nameEdit);

  mainGridLayout->addLayout(vLayout,0,0,1,3, Qt::AlignTop);

  // number of people, people per area, and area per person
  vLayout = new QVBoxLayout();

  label = new QLabel("Number of People: ");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_numberofPeopleEdit = new OSDoubleEdit2();
  vLayout->addWidget(m_numberofPeopleEdit);

  mainGridLayout->addLayout(vLayout,1,0, Qt::AlignTop|Qt::AlignLeft);

  vLayout = new QVBoxLayout();

  label = new QLabel("People per Space Floor Area: ");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_peopleperSpaceFloorAreaEdit = new OSQuantityEdit2("people/m^2", "people/m^2", "people/ft^2", m_isIP);
  connect(this, &PeopleDefinitionInspectorView::toggleUnitsClicked, m_peopleperSpaceFloorAreaEdit, &OSQuantityEdit2::onUnitSystemChange);
  vLayout->addWidget(m_peopleperSpaceFloorAreaEdit);

  mainGridLayout->addLayout(vLayout,1,1, Qt::AlignTop|Qt::AlignLeft);

  vLayout = new QVBoxLayout();

  label = new QLabel("Space Floor Area per Person: ");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_spaceFloorAreaperPersonEdit = new OSQuantityEdit2("m^2/person", "m^2/person", "ft^2/person", m_isIP);
  connect(this, &PeopleDefinitionInspectorView::toggleUnitsClicked, m_spaceFloorAreaperPersonEdit, &OSQuantityEdit2::onUnitSystemChange);
  vLayout->addWidget(m_spaceFloorAreaperPersonEdit);

  mainGridLayout->addLayout(vLayout,1,2, Qt::AlignTop|Qt::AlignLeft);

  // fraction radiance, sensible heat fraction, carbon dioxide rate
  vLayout = new QVBoxLayout();

  label = new QLabel("Fraction Radiant: ");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_fractionRadiantEdit = new OSDoubleEdit2();
  vLayout->addWidget(m_fractionRadiantEdit);

  mainGridLayout->addLayout(vLayout,2,0, Qt::AlignTop|Qt::AlignLeft);

  vLayout = new QVBoxLayout();

  label = new QLabel("Sensible Heat Fraction: ");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_sensibleHeatFractionEdit = new OSDoubleEdit2();
  vLayout->addWidget(m_sensibleHeatFractionEdit);

  mainGridLayout->addLayout(vLayout,2,1, Qt::AlignTop|Qt::AlignLeft);

  vLayout = new QVBoxLayout();

  label = new QLabel("Carbon Dioxide Generation Rate: ");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_carbonDioxideGenerationRateEdit = new OSQuantityEdit2("m^3/s*W", "L/s*W", "ft^3*hr/min*Btu", m_isIP);
  connect(this, &PeopleDefinitionInspectorView::toggleUnitsClicked, m_carbonDioxideGenerationRateEdit, &OSQuantityEdit2::onUnitSystemChange);
  vLayout->addWidget(m_carbonDioxideGenerationRateEdit);

  mainGridLayout->addLayout(vLayout,2,2, Qt::AlignTop|Qt::AlignLeft);

  mainGridLayout->setColumnMinimumWidth(0, 80);
  mainGridLayout->setColumnMinimumWidth(1, 80);
  mainGridLayout->setColumnMinimumWidth(2, 80);
  mainGridLayout->setColumnStretch(3,1);
  mainGridLayout->setRowMinimumHeight(0, 30);
  mainGridLayout->setRowMinimumHeight(1, 30);
  mainGridLayout->setRowMinimumHeight(2, 30);
  mainGridLayout->setRowStretch(3,1);
}

void PeopleDefinitionInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  detach();
}

void PeopleDefinitionInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::PeopleDefinition peopleDefinition = modelObject.cast<model::PeopleDefinition>();
  attach(peopleDefinition);
  refresh();
}

void PeopleDefinitionInspectorView::onUpdate()
{
  refresh();
}

void PeopleDefinitionInspectorView::attach(openstudio::model::PeopleDefinition& peopleDefinition)
{
  m_peopleDefinition = peopleDefinition;
  m_nameEdit->bind(*m_peopleDefinition,
                   OptionalStringGetter(std::bind(&model::PeopleDefinition::name,m_peopleDefinition.get_ptr(),true)),
                   boost::optional<StringSetter>(std::bind(&model::PeopleDefinition::setName,m_peopleDefinition.get_ptr(),std::placeholders::_1)));
  
  // bind to PeopleDefinition methods
  m_numberofPeopleEdit->bind(
      *m_peopleDefinition,
      OptionalDoubleGetter(std::bind(&model::PeopleDefinition::numberofPeople,m_peopleDefinition.get_ptr())),
      boost::optional<DoubleSetter>(std::bind(&model::PeopleDefinition::setNumberofPeople,m_peopleDefinition.get_ptr(),std::placeholders::_1)));
  
  m_peopleperSpaceFloorAreaEdit->bind(
      m_isIP,
      *m_peopleDefinition,
      OptionalDoubleGetter(std::bind(&model::PeopleDefinition::peopleperSpaceFloorArea,m_peopleDefinition.get_ptr())),
      boost::optional<DoubleSetter>(std::bind(&model::PeopleDefinition::setPeopleperSpaceFloorArea,m_peopleDefinition.get_ptr(),std::placeholders::_1)));

  m_spaceFloorAreaperPersonEdit->bind(
      m_isIP,
      *m_peopleDefinition,
      OptionalDoubleGetter(std::bind(&model::PeopleDefinition::spaceFloorAreaperPerson,m_peopleDefinition.get_ptr())),
      boost::optional<DoubleSetter>(std::bind(&model::PeopleDefinition::setSpaceFloorAreaperPerson,m_peopleDefinition.get_ptr(),std::placeholders::_1)));

  // ETH: Note that this is overkill for this dimensionless value. Should switch to OSDoubleEdit(2).
  m_fractionRadiantEdit->bind(
      *m_peopleDefinition,
      DoubleGetter(std::bind(&model::PeopleDefinition::fractionRadiant,m_peopleDefinition.get_ptr())),
      boost::optional<DoubleSetter>(std::bind(&model::PeopleDefinition::setFractionRadiant,m_peopleDefinition.get_ptr(),std::placeholders::_1)));

  m_sensibleHeatFractionEdit->bind(
      *m_peopleDefinition,
      OptionalDoubleGetter(std::bind(&model::PeopleDefinition::sensibleHeatFraction,m_peopleDefinition.get_ptr())),
      boost::optional<DoubleSetter>(std::bind(&model::PeopleDefinition::setSensibleHeatFraction,m_peopleDefinition.get_ptr(),std::placeholders::_1)),
      boost::optional<NoFailAction>(std::bind(&model::PeopleDefinition::resetSensibleHeatFraction,m_peopleDefinition.get_ptr())),
      boost::none,
      boost::optional<NoFailAction>(std::bind(&model::PeopleDefinition::autocalculateSensibleHeatFraction,m_peopleDefinition.get_ptr())),
      boost::optional<BasicQuery>(std::bind(&model::PeopleDefinition::isSensibleHeatFractionDefaulted,m_peopleDefinition.get_ptr())),
      boost::none,
      boost::optional<BasicQuery>(std::bind(&model::PeopleDefinition::isSensibleHeatFractionAutocalculated,m_peopleDefinition.get_ptr())));

  m_carbonDioxideGenerationRateEdit->bind(
      m_isIP,
      *m_peopleDefinition,
      DoubleGetter(std::bind(&model::PeopleDefinition::carbonDioxideGenerationRate,m_peopleDefinition.get_ptr())),
      boost::optional<DoubleSetter>(std::bind(&model::PeopleDefinition::setCarbonDioxideGenerationRate,m_peopleDefinition.get_ptr(),std::placeholders::_1)),
      boost::optional<NoFailAction>(std::bind(&model::PeopleDefinition::resetCarbonDioxideGenerationRate,m_peopleDefinition.get_ptr())),
      boost::none,
      boost::none,
      boost::optional<BasicQuery>(std::bind(&model::PeopleDefinition::isCarbonDioxideGenerationRateDefaulted,m_peopleDefinition.get_ptr())));

  this->stackedWidget()->setCurrentIndex(1);
}

void PeopleDefinitionInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_nameEdit->unbind();
  m_numberofPeopleEdit->unbind();
  m_peopleperSpaceFloorAreaEdit->unbind();
  m_spaceFloorAreaperPersonEdit->unbind();
  m_fractionRadiantEdit->unbind();
  m_sensibleHeatFractionEdit->unbind();
  m_carbonDioxideGenerationRateEdit->unbind();
  m_peopleDefinition.reset();
}

void PeopleDefinitionInspectorView::refresh()
{
}

void PeopleDefinitionInspectorView::toggleUnits(bool displayIP)
{
}

} // openstudio

