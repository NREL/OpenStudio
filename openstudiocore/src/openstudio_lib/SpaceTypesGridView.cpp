/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include "SpaceTypesGridView.hpp"

#include "ModelObjectItem.hpp"
#include "OSAppBase.hpp"
#include "OSDocument.hpp"
#include "OSDropZone.hpp"

#include "../shared_gui_components/OSGridView.hpp"

#include "../model/DefaultConstructionSet.hpp"
#include "../model/DefaultConstructionSet_Impl.hpp"
#include "../model/DefaultScheduleSet.hpp"
#include "../model/DefaultScheduleSet_Impl.hpp"
#include "../model/DesignSpecificationOutdoorAir.hpp"
#include "../model/DesignSpecificationOutdoorAir_Impl.hpp"
#include "../model/ElectricEquipment.hpp"
#include "../model/ElectricEquipment_Impl.hpp"
#include "../model/GasEquipment.hpp"
#include "../model/GasEquipment_Impl.hpp"
#include "../model/HotWaterEquipment.hpp"
#include "../model/HotWaterEquipment_Impl.hpp"
#include "../model/InternalMass.hpp"
#include "../model/InternalMass_Impl.hpp"
#include "../model/Lights.hpp"
#include "../model/Lights_Impl.hpp"
#include "../model/Luminaire.hpp"
#include "../model/Luminaire_Impl.hpp"
#include "../model/Model.hpp"
#include "../model/ModelObject.hpp"
#include "../model/ModelObject_Impl.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/OtherEquipment.hpp"
#include "../model/OtherEquipment_Impl.hpp"
#include "../model/People.hpp"
#include "../model/People_Impl.hpp"
#include "../model/RenderingColor.hpp"
#include "../model/RenderingColor_Impl.hpp"
#include "../model/SpaceInfiltrationDesignFlowRate.hpp"
#include "../model/SpaceInfiltrationDesignFlowRate_Impl.hpp"
#include "../model/SpaceInfiltrationEffectiveLeakageArea.hpp"
#include "../model/SpaceInfiltrationEffectiveLeakageArea_Impl.hpp"
#include "../model/SpaceLoad.hpp"
#include "../model/SpaceLoad_Impl.hpp"
#include "../model/SpaceLoadInstance.hpp"
#include "../model/SpaceLoadInstance_Impl.hpp"
#include "../model/SpaceType.hpp"
#include "../model/SpaceType_Impl.hpp"
#include "../model/SteamEquipment.hpp"
#include "../model/SteamEquipment_Impl.hpp"

#include "../utilities/idd/OS_SpaceType_FieldEnums.hxx"

#include <QBoxLayout>
#include <QCheckBox>
#include <QLabel>
#include <QScrollArea>
#include <QSettings>
#include <QTimer>

// These defines provide a common area for field display names
// used on column headers, and other grid widgets

#define NAME "Space Type Name"

// GENERAL
#define RENDERINGCOLOR "Rendering color"
#define DEFAULTCONSTRUCTIONSET "Default Construction\nSet"
#define DEFAULTSCHEDULESET "Default Schedule\nSet"
#define DESIGNSPECIFICATIONOUTDOORAIR "Design Specification\nOutdoor Air"
#define SPACEINFILTRATIONDESIGNFLOWRATES "Space Infiltration\nDesign Flow Rates"
#define SPACEINFILTRATIONEFFECTIVELEAKAGEAREAS "Space Infiltration\nEffective Leakage Areas"

// LOADS
#define LOADNAME "Load Name"
#define MULTIPLIER "Multiplier"
#define DEFINITION "Definition"
#define SCHEDULE "Schedule"
#define ACTIVITYSCHEDULE "Activity Schedule\n(People Only)"

// MEASURE TAGS
#define STANDARDSBUILDINGTYPE "Standards Building Type\n(Optional)"
#define STANDARDSSPACETYPE "Standards Space Type\n(Optional)"

namespace openstudio {

struct ModelObjectNameSorter{
  // sort by name
  bool operator()(const model::ModelObject & lhs, const model::ModelObject & rhs){
    return (lhs.name() < rhs.name());
  }
};

SpaceTypesGridView::SpaceTypesGridView(bool isIP, const model::Model & model, QWidget * parent)
  : QWidget(parent),
  m_isIP(isIP)
{
  QVBoxLayout * layout = 0;

  layout = new QVBoxLayout();
  layout->setSpacing(0);
  layout->setContentsMargins(0,0,0,0);
  setLayout(layout);
  
  std::vector<model::SpaceType> spaceTypes = model.getModelObjects<model::SpaceType>();
  std::vector<model::ModelObject> spaceTypeModelObjects = subsetCastVector<model::ModelObject>(spaceTypes);

  SpaceTypesGridController * spaceTypesGridController  = new SpaceTypesGridController(m_isIP, "Space Types", model, spaceTypeModelObjects);
  OSGridView * gridView = new OSGridView(spaceTypesGridController, "Space Types", "Drop\nZone", parent);

  bool isConnected = false;

  isConnected = connect(gridView, SIGNAL(dropZoneItemClicked(OSItem*)), this, SIGNAL(dropZoneItemClicked(OSItem*)));
  OS_ASSERT(isConnected);

  isConnected = connect(this, SIGNAL(itemSelected(OSItem *)), gridView, SIGNAL(itemSelected(OSItem *)));
  OS_ASSERT(isConnected);

  isConnected = connect(this, SIGNAL(selectionCleared()), gridView, SLOT(onSelectionCleared()));
  OS_ASSERT(isConnected);

  isConnected = connect(gridView, SIGNAL(gridRowSelected(OSItem*)), this, SIGNAL(gridRowSelected(OSItem*)));
  OS_ASSERT(isConnected);

  gridView->m_dropZone->hide();

  layout->addWidget(gridView,0,Qt::AlignTop);

  layout->addStretch(1);

  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), spaceTypesGridController, SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), spaceTypesGridController, SLOT(toggleUnits(bool)));
  OS_ASSERT(isConnected);

  std::vector<model::SpaceType> spaceType = model.getModelObjects<model::SpaceType>(); // NOTE for horizontal system lists

}

SpaceTypesGridController::SpaceTypesGridController(bool isIP,
  const QString & headerText,
  model::Model model,
  std::vector<model::ModelObject> modelObjects) :
  OSGridController(isIP, headerText, model, modelObjects)
{
  setCategoriesAndFields();
}

void SpaceTypesGridController::setCategoriesAndFields()
{

  {
    std::vector<QString> fields;
    fields.push_back(RENDERINGCOLOR);
    fields.push_back(DEFAULTCONSTRUCTIONSET);
    fields.push_back(DEFAULTSCHEDULESET);
    fields.push_back(DESIGNSPECIFICATIONOUTDOORAIR);
    fields.push_back(SPACEINFILTRATIONDESIGNFLOWRATES); // TODO Extensible DropZone
    //fields.push_back(SPACEINFILTRATIONEFFECTIVELEAKAGEAREAS); // TODO Extensible DropZone
    std::pair<QString,std::vector<QString> > categoryAndFields = std::make_pair(QString("General"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back(LOADNAME); // Name Edit
    //fields.push_back(MULTIPLIER); // Value Edit
    //fields.push_back(DEFINITION); // DropZone
    //fields.push_back(SCHEDULE); // DropZone
    //fields.push_back(ACTIVITYSCHEDULE); // DropZone
    std::pair<QString,std::vector<QString> > categoryAndFields = std::make_pair(QString("Loads"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    //fields.push_back(STANDARDSBUILDINGTYPE);
    //fields.push_back(STANDARDSSPACETYPE);
    std::pair<QString, std::vector<QString> > categoryAndFields = std::make_pair(QString("Measure Tags"), fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  OSGridController::setCategoriesAndFields();

}

void SpaceTypesGridController::addColumns(std::vector<QString> & fields)
{
  // always show name column
  fields.insert(fields.begin(), NAME);

  m_baseConcepts.clear();

  Q_FOREACH(QString field, fields){
    if (field == NAME) {
      auto getter = CastNullAdapter<model::SpaceType>(&model::SpaceType::name);
      auto setter = CastNullAdapter<model::SpaceType>(&model::SpaceType::setName);

      addNameLineEditColumn(QString(NAME),
        getter,
        setter);

    } else if (field == LOADNAME) {
      // Create a lambda function that collates all of the loads in a space type 
      // and returns them as an std::vector
      std::function<std::vector<model::ModelObject> (const model::SpaceType &)> allLoads(
        [] (const model::SpaceType &t_spaceType) {
          std::vector<model::ModelObject> loads;
          auto InternalMass = t_spaceType.internalMass();
          auto People = t_spaceType.people();
          auto Lights = t_spaceType.lights();
          auto Luminaire = t_spaceType.luminaires();
          auto ElectricEquipment = t_spaceType.electricEquipment();
          auto GasEquipment = t_spaceType.gasEquipment();
          auto HotWaterEquipment = t_spaceType.hotWaterEquipment();
          auto SteamEquipment = t_spaceType.steamEquipment();
          auto OtherEquipment = t_spaceType.otherEquipment();
          auto SpaceInfiltrationDesignFlowRate = t_spaceType.spaceInfiltrationDesignFlowRates();
          auto SpaceInfiltrationEffectiveLeakageArea = t_spaceType.spaceInfiltrationEffectiveLeakageAreas();

          loads.insert(loads.end(), InternalMass.begin(), InternalMass.end());
          loads.insert(loads.end(), People.begin(), People.end());
          loads.insert(loads.end(), Lights.begin(), Lights.end());
          loads.insert(loads.end(), Luminaire.begin(), Luminaire.end());
          loads.insert(loads.end(), ElectricEquipment.begin(), ElectricEquipment.end());
          loads.insert(loads.end(), GasEquipment.begin(), GasEquipment.end());
          loads.insert(loads.end(), HotWaterEquipment.begin(), HotWaterEquipment.end());
          loads.insert(loads.end(), SteamEquipment.begin(), SteamEquipment.end());
          loads.insert(loads.end(), OtherEquipment.begin(), OtherEquipment.end());
          loads.insert(loads.end(), SpaceInfiltrationDesignFlowRate.begin(), SpaceInfiltrationDesignFlowRate.end());
          loads.insert(loads.end(), SpaceInfiltrationEffectiveLeakageArea.begin(), SpaceInfiltrationEffectiveLeakageArea.end());

          return loads;
        }
      );

      // Here we create a NameLineEdit column, but this one includes a "DataSource" object
      // The DataSource object is used in OSGridController::widgetAt to make a list of NameLineEdit widgets
      // for each SpaceType that is passed in.
      //
      // Notice that it takes the "allLoads" functor from above.
      //
      // Just as an implementation note, it would be possible to use the DataSource as an alternative
      // to the ProxyAdapter function, if the DataSource were to return a vector of 1.
      //
      // The final argument to DataSource tells the system that we want an additional widget to be displayed
      // at the bottom of each list. In this case, it's a dropZone. Any type of BaseConcept would work.

      addNameLineEditColumn(QString(LOADNAME),
        CastNullAdapter<model::SpaceLoad>(&model::SpaceLoad::name),
        CastNullAdapter<model::SpaceLoad>(&model::SpaceLoad::setName),
        DataSource(
          allLoads,
          true
          //QSharedPointer<DropZoneConcept>(new DropZoneConceptImpl<ValueType, DataSourceType>(headingLabel, getter, setter)
        )
      );

    } else if (field == DEFAULTCONSTRUCTIONSET){
      addDropZoneColumn(QString(DEFAULTCONSTRUCTIONSET),
        CastNullAdapter<model::SpaceType>(&model::SpaceType::defaultConstructionSet),
        CastNullAdapter<model::SpaceType>(&model::SpaceType::setDefaultConstructionSet));

    } else if (field == DEFAULTSCHEDULESET){
      addDropZoneColumn(QString(DEFAULTSCHEDULESET),
        CastNullAdapter<model::SpaceType>(&model::SpaceType::defaultScheduleSet),
        CastNullAdapter<model::SpaceType>(&model::SpaceType::setDefaultScheduleSet));

    } else if (field == DESIGNSPECIFICATIONOUTDOORAIR){
      addDropZoneColumn(QString(DESIGNSPECIFICATIONOUTDOORAIR),
        CastNullAdapter<model::SpaceType>(&model::SpaceType::designSpecificationOutdoorAir),
        CastNullAdapter<model::SpaceType>(&model::SpaceType::setDesignSpecificationOutdoorAir));

    } else if (field == RENDERINGCOLOR){
      addRenderingColorColumn(QString(RENDERINGCOLOR),
        CastNullAdapter<model::SpaceType>(&model::SpaceType::renderingColor),
        CastNullAdapter<model::SpaceType>(&model::SpaceType::setRenderingColor));    

    } else if (field == SPACEINFILTRATIONDESIGNFLOWRATES) {
      std::function<boost::optional<model::SpaceInfiltrationDesignFlowRate> (model::SpaceType *)>  getter;
      std::function<bool (model::SpaceType *, const model::SpaceInfiltrationDesignFlowRate &)> setter;

      addNameLineEditColumn(QString(LOADNAME),
        CastNullAdapter<model::SpaceInfiltrationDesignFlowRate>(&model::SpaceInfiltrationDesignFlowRate::name),
        CastNullAdapter<model::SpaceInfiltrationDesignFlowRate>(&model::SpaceInfiltrationDesignFlowRate::setName),
        DataSource(
          std::function<std::vector<model::SpaceInfiltrationDesignFlowRate> (const model::SpaceType &)>(&model::SpaceType::spaceInfiltrationDesignFlowRates),
          true,
          QSharedPointer<DropZoneConcept>(new DropZoneConceptImpl<model::SpaceInfiltrationDesignFlowRate, model::SpaceType>("Space Infiltration", 
             getter, setter))
        )
      );

    } else {
      // unhandled
      OS_ASSERT(false);
    }
  }
}

QString SpaceTypesGridController::getColor(const model:: ModelObject & modelObject)
{
  QColor defaultColor(Qt::lightGray);
  QString color(defaultColor.name());

  // TODO: The code below is currently commented out because a refresh crash bug is precluding rack color
  // updates due to rack assignments to cases and walk-ins.  No colors are better than wrong colors.

  //std::vector<model::RefrigerationSystem> refrigerationSystems = m_model.getModelObjects<model::RefrigerationSystem>();

  //boost::optional<model::SpaceType> refrigerationCase = modelObject.optionalCast<model::SpaceType>();
  //OS_ASSERT(refrigerationCase);

  //boost::optional<model::RefrigerationSystem> refrigerationSystem = refrigerationCase->system();
  //if(!refrigerationSystem){
  //  return color;
  //}

  //std::vector<model::RefrigerationSystem>::iterator it;
  //it = std::find(refrigerationSystems.begin(), refrigerationSystems.end(), refrigerationSystem.get());
  //if(it != refrigerationSystems.end()){
  //  int index = std::distance(refrigerationSystems.begin(), it);
  //  if(index >= static_cast<int>(m_colors.size())){
  //    index = m_colors.size() - 1; // similar to scheduleView's approach
  //  }
  //  color = this->m_colors.at(index).name();
  //}

  return color;
}

void SpaceTypesGridController::checkSelectedFields()
{
  if(!this->m_hasHorizontalHeader) return;

  // Don't show the name column check box
  // From above in addColumns, we know that NAME is the first entry
  HorizontalHeaderWidget * horizontalHeaderWidget = qobject_cast<HorizontalHeaderWidget *>(m_horizontalHeader.at(0));
  OS_ASSERT(horizontalHeaderWidget);
  horizontalHeaderWidget->m_checkBox->hide();

  OSGridController::checkSelectedFields();
}

void SpaceTypesGridController::onItemDropped(const OSItemId& itemId)
{
  boost::optional<model::ModelObject> modelObject = OSAppBase::instance()->currentDocument()->getModelObject(itemId);
  if (modelObject){
    if (modelObject->optionalCast<model::SpaceType>()){
      modelObject->clone(m_model);
      emit modelReset();
    }
  }
}

void SpaceTypesGridController::refreshModelObjects()
{
  std::vector<model::SpaceType> refrigerationCases = m_model.getModelObjects<model::SpaceType>();
  m_modelObjects = subsetCastVector<model::ModelObject>(refrigerationCases);
  std::sort(m_modelObjects.begin(), m_modelObjects.end(), ModelObjectNameSorter());
}

void SpaceTypesGridController::onComboBoxIndexChanged(int index)
{
}

} // openstudio

