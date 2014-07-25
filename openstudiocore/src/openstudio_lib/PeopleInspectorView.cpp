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

#include "PeopleInspectorView.hpp"

#include "ModelObjectItem.hpp"
#include "OSDropZone.hpp"
#include "OSVectorController.hpp"
#include "SpaceLoadInstancesWidget.hpp"

#include "../shared_gui_components/OSCheckBox.hpp"
#include "../shared_gui_components/OSComboBox.hpp"
#include "../shared_gui_components/OSDoubleEdit.hpp"
#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSQuantityEdit.hpp"

#include "../model/People.hpp"
#include "../model/People_Impl.hpp"
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
/*
  class PeopleVectorController : public OSVectorController
  {
  public:
    PeopleVectorController(const openstudio::model::Model& model, QWidget* parent = 0)
      : OSVectorController(model, parent)
    {}

    void setPeople(const model::People& people)
    {
      m_people = people;
    }

    void clearPeople()
    {
      m_people.reset();
    }

  protected:
    boost::optional<model::People> m_people;
  };

  class PeopleNumberofPeopleScheduleVectorController : public PeopleVectorController
  {
  public:
    PeopleNumberofPeopleScheduleVectorController(const openstudio::model::Model& model, QWidget* parent = 0)
      : PeopleVectorController(model, parent)
    {}

    virtual void makeVector()
    {
      m_objects.clear();
      if (m_people){
        boost::optional<model::Schedule> numberofPeopleSchedule = m_people->numberofPeopleSchedule();
        if (numberofPeopleSchedule){
          m_objects.push_back(*numberofPeopleSchedule);
        }
      }
    }
  };

  class PeopleActivityLevelScheduleVectorController : public PeopleVectorController
  {
  public:
    PeopleActivityLevelScheduleVectorController(const openstudio::model::Model& model, QWidget* parent = 0)
      : PeopleVectorController(model, parent)
    {}

    virtual void makeVector()
    {
      m_objects.clear();
      if (m_people){
        boost::optional<model::Schedule> activityLevelSchedule = m_people->activityLevelSchedule();
        if (activityLevelSchedule){
          m_objects.push_back(*activityLevelSchedule);
        }
      }
    }
  };

  class PeopleAngleFactorListVectorController : public PeopleVectorController
  {
  public:
    PeopleAngleFactorListVectorController(const openstudio::model::Model& model, QWidget* parent = 0)
      : PeopleVectorController(model, parent)
    {}

    virtual void makeVector()
    {
      m_objects.clear();
      if (m_people){
      }
    }
  };

  class PeopleWorkEfficiencyScheduleVectorController : public PeopleVectorController
  {
  public:
    PeopleWorkEfficiencyScheduleVectorController(const openstudio::model::Model& model, QWidget* parent = 0)
      : PeopleVectorController(model, parent)
    {}

    virtual void makeVector()
    {
      m_objects.clear();
      if (m_people){
        boost::optional<model::Schedule> workEfficiencySchedule = m_people->workEfficiencySchedule();
        if (workEfficiencySchedule){
          m_objects.push_back(*workEfficiencySchedule);
        }
      }
    }
  };

  class PeopleClothingInsulationScheduleVectorController : public PeopleVectorController
  {
  public:
    PeopleClothingInsulationScheduleVectorController(const openstudio::model::Model& model, QWidget* parent = 0)
      : PeopleVectorController(model, parent)
    {}

    virtual void makeVector()
    {
      m_objects.clear();
      if (m_people){
        boost::optional<model::Schedule> clothingInsulationSchedule = m_people->clothingInsulationSchedule();
        if (clothingInsulationSchedule){
          m_objects.push_back(*clothingInsulationSchedule);
        }
      }
    }
  };

  class PeopleAirVelocityScheduleVectorController : public PeopleVectorController
  {
  public:
    PeopleAirVelocityScheduleVectorController(const openstudio::model::Model& model, QWidget* parent = 0)
      : PeopleVectorController(model, parent)
    {}

    virtual void makeVector()
    {
      m_objects.clear();
      if (m_people){
        boost::optional<model::Schedule> airVelocitySchedule = m_people->airVelocitySchedule();
        if (airVelocitySchedule){
          m_objects.push_back(*airVelocitySchedule);
        }
      }
    }
  };

*/
PeopleInspectorView::PeopleInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent )
  : ModelObjectInspectorView(model, true, parent)
{
  m_isIP = isIP;

  QWidget* hiddenWidget = new QWidget();
  this->stackedWidget()->insertWidget(0, hiddenWidget);

  QWidget* visibleWidget = new QWidget();
  this->stackedWidget()->insertWidget(1, visibleWidget);

  this->stackedWidget()->setCurrentIndex(0);

  QGridLayout* mainGridLayout = new QGridLayout();
  mainGridLayout->setContentsMargins(7,7,7,7);
  mainGridLayout->setSpacing(14);
  visibleWidget->setLayout(mainGridLayout);

  // name
  QVBoxLayout* vLayout = new QVBoxLayout();

  QLabel* label = new QLabel("Name:");
  label->setObjectName("H1");
  vLayout->addWidget(label);

  m_nameEdit = new OSLineEdit();
  vLayout->addWidget(m_nameEdit);

  mainGridLayout->addLayout(vLayout,0,0,1,2, Qt::AlignTop);

  // multiplier and definition
  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Multiplier: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_multiplierEdit = new OSQuantityEdit(m_isIP);
  bool isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_multiplierEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  vLayout->addWidget(m_multiplierEdit);

  mainGridLayout->addLayout(vLayout,1,0, Qt::AlignTop|Qt::AlignLeft);

  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("People Definition: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_peopleDefinitionComboBox = new OSComboBox();
  vLayout->addWidget(m_peopleDefinitionComboBox);

  mainGridLayout->addLayout(vLayout,1,1, Qt::AlignTop|Qt::AlignLeft);

  mainGridLayout->setColumnMinimumWidth(0, 100);
  mainGridLayout->setColumnMinimumWidth(1, 100);
  mainGridLayout->setColumnStretch(2,1);
  mainGridLayout->setRowMinimumHeight(0, 30);
  mainGridLayout->setRowMinimumHeight(1, 30);
  mainGridLayout->setRowStretch(2,1);
}

void PeopleInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  detach();
}

void PeopleInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::People people = modelObject.cast<model::People>();
  attach(people);
  refresh();
}

void PeopleInspectorView::onUpdate()
{
  refresh();
}

void PeopleInspectorView::attach(openstudio::model::People& people)
{
  /*
  m_nameEdit->bind(people,"name");
  m_multiplierEdit->bind();
  //m_peopleDefinitionComboBox->bind();
  m_numberofPeopleScheduleVectorController->setPeople(people);
  m_activityLevelScheduleVectorController->setPeople(people);
  m_angleFactorListVectorController->setPeople(people);
  m_workEfficiencyScheduleVectorController->setPeople(people);
  m_clothingInsulationScheduleVectorController->setPeople(people);
  m_airVelocityScheduleVectorController->setPeople(people);
*/

  this->stackedWidget()->setCurrentIndex(1);
}

void PeopleInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_nameEdit->unbind();
  m_multiplierEdit->unbind();
  //m_peopleDefinitionComboBox->unbind();
  //m_numberofPeopleScheduleVectorController->clearPeople();
  //m_activityLevelScheduleVectorController->clearPeople();
  //m_angleFactorListVectorController->clearPeople();
  //m_workEfficiencyScheduleVectorController->clearPeople();
  //m_clothingInsulationScheduleVectorController->clearPeople();
  //m_airVelocityScheduleVectorController->clearPeople();
}

void PeopleInspectorView::refresh()
{
}

PeopleDefinitionInspectorView::PeopleDefinitionInspectorView(bool isIP, 
                                                             const openstudio::model::Model& model,
                                                             QWidget * parent)
  : ModelObjectInspectorView(model, true, parent)
{
  m_isIP = isIP;

  //QWidget* hiddenWidget = new QWidget();
  //this->stackedWidget()->insertWidget(0, hiddenWidget);

  QWidget* visibleWidget = new QWidget();
  this->stackedWidget()->addWidget(visibleWidget);

  //this->stackedWidget()->setCurrentIndex(0);

  QGridLayout* mainGridLayout = new QGridLayout();
  mainGridLayout->setContentsMargins(7,7,7,7);
  mainGridLayout->setSpacing(14);
  visibleWidget->setLayout(mainGridLayout);

  // name
  QVBoxLayout* vLayout = new QVBoxLayout();

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

  bool isConnected = false;

  m_numberofPeopleEdit = new OSDoubleEdit2();
  vLayout->addWidget(m_numberofPeopleEdit);

  mainGridLayout->addLayout(vLayout,1,0, Qt::AlignTop|Qt::AlignLeft);

  vLayout = new QVBoxLayout();

  label = new QLabel("People per Space Floor Area: ");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_peopleperSpaceFloorAreaEdit = new OSQuantityEdit2("people/m^2", "people/m^2", "people/ft^2", m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_peopleperSpaceFloorAreaEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  vLayout->addWidget(m_peopleperSpaceFloorAreaEdit);

  mainGridLayout->addLayout(vLayout,1,1, Qt::AlignTop|Qt::AlignLeft);

  vLayout = new QVBoxLayout();

  label = new QLabel("Space Floor Area per Person: ");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_spaceFloorAreaperPersonEdit = new OSQuantityEdit2("m^2/person", "m^2/person", "ft^2/person", m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_spaceFloorAreaperPersonEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
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

  label = new QLabel("Sensible Heat Fraction Radiant: ");
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
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_carbonDioxideGenerationRateEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
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

