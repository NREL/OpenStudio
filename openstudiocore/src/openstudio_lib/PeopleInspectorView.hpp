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

#ifndef OPENSTUDIO_PEOPLEINSPECTORVIEW_HPP
#define OPENSTUDIO_PEOPLEINSPECTORVIEW_HPP

#include "ModelObjectInspectorView.hpp"
#include "../model/PeopleDefinition.hpp"

class QLabel;
class QPushButton;
class QColor;

namespace openstudio {

class OSDoubleEdit2;
class OSDropZone;
class OSLineEdit;
class OSLineEdit2;
class OSQuantityEdit;
class OSQuantityEdit2;
class OSCheckBox;
class OSComboBox;

class PeopleNumberofPeopleScheduleVectorController;
class PeopleActivityLevelScheduleVectorController;
class PeopleAngleFactorListVectorController;
class PeopleWorkEfficiencyScheduleVectorController;
class PeopleClothingInsulationScheduleVectorController;
class PeopleAirVelocityScheduleVectorController;

namespace model {
  class People;
}

class PeopleInspectorView : public ModelObjectInspectorView
{
  Q_OBJECT

  public:

    PeopleInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent = 0 );

    virtual ~PeopleInspectorView() {}

  protected:

    virtual void onClearSelection();

    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject);

    virtual void onUpdate();

  private:

    void attach(openstudio::model::People& people);

    void detach();

    void refresh();

    OSLineEdit* m_nameEdit;

    OSQuantityEdit* m_multiplierEdit;

    OSComboBox* m_peopleDefinitionComboBox;

    PeopleNumberofPeopleScheduleVectorController* m_numberofPeopleScheduleVectorController;
    OSDropZone* m_numberofPeopleScheduleDropZone;

    PeopleActivityLevelScheduleVectorController* m_activityLevelScheduleVectorController;
    OSDropZone* m_activityLevelScheduleDropZone;

    PeopleAngleFactorListVectorController* m_angleFactorListVectorController;
    OSDropZone* m_angleFactorListDropZone;

    PeopleWorkEfficiencyScheduleVectorController* m_workEfficiencyScheduleVectorController;
    OSDropZone* m_workEfficiencyScheduleDropZone;

    PeopleClothingInsulationScheduleVectorController* m_clothingInsulationScheduleVectorController;
    OSDropZone* m_clothingInsulationScheduleDropZone;

    PeopleAirVelocityScheduleVectorController* m_airVelocityScheduleVectorController;
    OSDropZone* m_airVelocityScheduleDropZone;

    bool m_isIP;
};

class PeopleDefinitionInspectorView : public ModelObjectInspectorView
{
  Q_OBJECT

  public:

    PeopleDefinitionInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent = 0 );

    virtual ~PeopleDefinitionInspectorView() {}

  protected:

    virtual void onClearSelection();

    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject);

    virtual void onUpdate();

  private:

    void attach(openstudio::model::PeopleDefinition& peopleDefinition);

    void detach();

    void refresh();

    OSLineEdit2* m_nameEdit;

    OSDoubleEdit2* m_numberofPeopleEdit;
    OSQuantityEdit2* m_peopleperSpaceFloorAreaEdit;
    OSQuantityEdit2* m_spaceFloorAreaperPersonEdit;
    OSDoubleEdit2* m_fractionRadiantEdit;
    OSDoubleEdit2* m_sensibleHeatFractionEdit;
    OSQuantityEdit2* m_carbonDioxideGenerationRateEdit;
    bool m_isIP;
    boost::optional<model::PeopleDefinition> m_peopleDefinition;

    // how to handle the extensible groups
    //OSCheckBox* m_enableASHRAE55ComfortWarningsCheckBox;
    //OSComboBox* m_meanRadiantTemperatureCalculationTypeComboBox;
    //OSComboBox* m_thermalComfortModel1TypeComboBox;

  public slots:

    void toggleUnits(bool displayIP);
};

} // openstudio

#endif // OPENSTUDIO_PEOPLEINSPECTORVIEW_HPP

