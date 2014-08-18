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
#include "../model/Schedule.hpp"
#include "../model/Schedule_Impl.hpp"
#include "../model/SpaceInfiltrationDesignFlowRate.hpp"
#include "../model/SpaceInfiltrationDesignFlowRate_Impl.hpp"
#include "../model/SpaceInfiltrationEffectiveLeakageArea.hpp"
#include "../model/SpaceInfiltrationEffectiveLeakageArea_Impl.hpp"
#include "../model/SpaceLoad.hpp"
#include "../model/SpaceLoad_Impl.hpp"
#include "../model/SpaceLoadDefinition.hpp"
#include "../model/SpaceLoadDefinition_Impl.hpp"
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
    fields.push_back(SPACEINFILTRATIONDESIGNFLOWRATES);
    fields.push_back(SPACEINFILTRATIONEFFECTIVELEAKAGEAREAS);
    std::pair<QString,std::vector<QString> > categoryAndFields = std::make_pair(QString("General"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back(LOADNAME);
    fields.push_back(MULTIPLIER);
    fields.push_back(DEFINITION);
    fields.push_back(SCHEDULE);
    fields.push_back(ACTIVITYSCHEDULE);
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

    if ( field == NAME ) {
      auto getter = CastNullAdapter<model::SpaceType>(&model::SpaceType::name);
      auto setter = CastNullAdapter<model::SpaceType>(&model::SpaceType::setName);

      addNameLineEditColumn(QString(NAME),
        getter,
        setter);

    } else if (field == LOADNAME || field == MULTIPLIER || field == DEFINITION || field == SCHEDULE || field == ACTIVITYSCHEDULE) {
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

      std::function<std::vector<boost::optional<model::ModelObject>>(const model::SpaceType &)> allLoadInstances(
          [allLoads](const model::SpaceType &t_spaceType) {
            std::vector<boost::optional<model::ModelObject>> loadInstances;
            for (const auto &l : allLoads(t_spaceType))
            {
              loadInstances.push_back(boost::optional<model::ModelObject>(l.optionalCast<model::SpaceLoadInstance>()));
            }
            return loadInstances;
          }
          );

      std::function<std::vector<boost::optional<model::ModelObject>>(const model::SpaceType &)> allDefinitions(
          [allLoadInstances](const model::SpaceType &t_spaceType) {
            std::vector<boost::optional<model::ModelObject>> definitions;
            for (const auto &l : allLoadInstances(t_spaceType))
            {
              if (l)
              {
                definitions.push_back(l->cast<model::SpaceLoadInstance>().definition());
              } else {
                definitions.emplace_back();
              }
            }
            return definitions;
          }
          );


      std::function<std::vector<boost::optional<model::ModelObject>>(const model::SpaceType &)> allLoadsWithMultipliers(
        [](const model::SpaceType &t_spaceType) {
          std::vector<boost::optional<model::ModelObject>> loads;
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
          //loads.insert(loads.end(), SpaceInfiltrationDesignFlowRate.begin(), SpaceInfiltrationDesignFlowRate.end());
          //loads.insert(loads.end(), SpaceInfiltrationEffectiveLeakageArea.begin(), SpaceInfiltrationEffectiveLeakageArea.end());

          for (const auto &l : SpaceInfiltrationDesignFlowRate)
          {
            loads.emplace_back();
          }

          for (const auto &l : SpaceInfiltrationEffectiveLeakageArea)
          {
            loads.emplace_back();
          }

          return loads;
        }
      );

      std::function<double(model::ModelObject *)> multiplier(
        [allLoads](model::ModelObject *t_modelObject) {
          double retval = 0;

          boost::optional<model::People> p = t_modelObject->optionalCast<model::People>();
          if (p)
          {
            retval = p->multiplier();
            return retval;
          }

          boost::optional<model::Lights> light = t_modelObject->optionalCast<model::Lights>();
          if (light)
          {
            retval = light->multiplier();
            return retval;
          }

          boost::optional<model::Luminaire> lum = t_modelObject->optionalCast<model::Luminaire>();
          if (lum)
          {
            retval = lum->multiplier();
            return retval;
          }

          boost::optional<model::ElectricEquipment> e = t_modelObject->optionalCast<model::ElectricEquipment>();
          if (e)
          {
            retval = e->multiplier();
            return retval;
          }

          boost::optional<model::GasEquipment> g = t_modelObject->optionalCast<model::GasEquipment>();
          if (g)
          {
            retval = g->multiplier();
            return retval;
          }

          boost::optional<model::HotWaterEquipment> h = t_modelObject->optionalCast<model::HotWaterEquipment>();
          if (h)
          {
            retval = h->multiplier();
            return retval;
          }

          boost::optional<model::SteamEquipment> se = t_modelObject->optionalCast<model::SteamEquipment>();
          if (se)
          {
            retval = se->multiplier();
            return retval;
          }

          boost::optional<model::OtherEquipment> o = t_modelObject->optionalCast<model::OtherEquipment>();
          if (o)
          {
            retval = o->multiplier();
            return retval;
          }

          // Should never get here
          OS_ASSERT(false);
          return retval;
        }
      );

      std::function<void(model::ModelObject *, double)> setMultiplier(
        [](model::ModelObject *t_modelObject, double multiplier) {
          boost::optional<model::People> p = t_modelObject->optionalCast<model::People>();
          if (p)
          {
            p->setMultiplier(multiplier);
            return;
          }

          boost::optional<model::Lights> light = t_modelObject->optionalCast<model::Lights>();
          if (light)
          {
            light->setMultiplier(multiplier);
            return;
          }

          boost::optional<model::Luminaire> lum = t_modelObject->optionalCast<model::Luminaire>();
          if (lum)
          {
            lum->setMultiplier(multiplier);
            return;
          }

          boost::optional<model::ElectricEquipment> e = t_modelObject->optionalCast<model::ElectricEquipment>();
          if (e)
          {
            e->setMultiplier(multiplier);
            return;
          }

          boost::optional<model::GasEquipment> g = t_modelObject->optionalCast<model::GasEquipment>();
          if (g)
          {
            g->setMultiplier(multiplier);
            return;
          }

          boost::optional<model::HotWaterEquipment> h = t_modelObject->optionalCast<model::HotWaterEquipment>();
          if (h)
          {
            h->setMultiplier(multiplier);
            return;
          }

          boost::optional<model::SteamEquipment> se = t_modelObject->optionalCast<model::SteamEquipment>();
          if (se)
          {
            se->setMultiplier(multiplier);
            return;
          }

          boost::optional<model::OtherEquipment> o = t_modelObject->optionalCast<model::OtherEquipment>();
          if (o)
          {
            o->setMultiplier(multiplier);
            return;
          }

          else {
            // Should never get here
            OS_ASSERT(false);
          }
        }
      );

      std::function<std::vector<boost::optional<model::ModelObject>>(const model::SpaceType &)> schedules(
        [allLoads](const model::SpaceType &t_spaceType) {
          std::vector<boost::optional<model::ModelObject>> retval;

          for (const auto &l : allLoads(t_spaceType))
          {

            boost::optional<model::People> p = l.optionalCast<model::People>();
            if (p)
            {
              auto s = p->numberofPeopleSchedule();
              retval.push_back(boost::optional<model::ModelObject>(s));
              continue;
            }

            boost::optional<model::Lights> light = l.optionalCast<model::Lights>();
            if (light)
            {
              auto s = light->schedule();
              retval.push_back(boost::optional<model::ModelObject>(s));
              continue;
            }

            boost::optional<model::Luminaire> lum = l.optionalCast<model::Luminaire>();
            if (lum)
            {
              auto s = lum->schedule();
              retval.push_back(boost::optional<model::ModelObject>(s));
              continue;
            }

            boost::optional<model::ElectricEquipment> e = l.optionalCast<model::ElectricEquipment>();
            if (e)
            {
              auto s = e->schedule();
              retval.push_back(boost::optional<model::ModelObject>(s));
              continue;
            }

            boost::optional<model::GasEquipment> g = l.optionalCast<model::GasEquipment>();
            if (g)
            {
              auto s = g->schedule();
              retval.push_back(boost::optional<model::ModelObject>(s));
              continue;
            }

            boost::optional<model::HotWaterEquipment> h = l.optionalCast<model::HotWaterEquipment>();
            if (h)
            {
              auto s = h->schedule();
              retval.push_back(boost::optional<model::ModelObject>(s));
              continue;
            }

            boost::optional<model::SteamEquipment> se = l.optionalCast<model::SteamEquipment>();
            if (se)
            {
              auto s = se->schedule();
              retval.push_back(boost::optional<model::ModelObject>(s));
              continue;
            }

            boost::optional<model::OtherEquipment> o = l.optionalCast<model::OtherEquipment>();
            if (o)
            {
              auto s = o->schedule();
              retval.push_back(boost::optional<model::ModelObject>(s));
              continue;
            }

            boost::optional<model::SpaceInfiltrationDesignFlowRate> f = l.optionalCast<model::SpaceInfiltrationDesignFlowRate>();
            if (f)
            {
              auto s = f->schedule();
              retval.push_back(boost::optional<model::ModelObject>(s));
              continue;
            }

            boost::optional<model::SpaceInfiltrationEffectiveLeakageArea> la = l.optionalCast<model::SpaceInfiltrationEffectiveLeakageArea>();
            if (la)
            {
              auto s = la->schedule();
              retval.push_back(boost::optional<model::ModelObject>(s));
              continue;
            }

            else {
              // Should never get here
              OS_ASSERT(false);
            }

          }

          return retval;
        }
      );

      std::function<std::vector<boost::optional<model::ModelObject>> (const model::SpaceType &)> activityLevelSchedules(
        [allLoads] (const model::SpaceType &t_spaceType) {
          std::vector<boost::optional<model::ModelObject>> retval;

          for (const auto &l : allLoads(t_spaceType))
          {
            boost::optional<model::People> p = l.optionalCast<model::People>();
            if (p)
            {
              auto als = p->activityLevelSchedule();
              retval.push_back(boost::optional<model::ModelObject>(als));
            } else {
              retval.emplace_back();
            }
          }

          return retval;
        }
      );
 
      if (field == LOADNAME) {

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
          )
        );

      }

      else if (field == MULTIPLIER) {

        addValueEditColumn(QString(MULTIPLIER),
          multiplier,
          setMultiplier,
          DataSource(
            allLoadsWithMultipliers,
            true
            //QSharedPointer<DropZoneConcept>(new DropZoneConceptImpl<ValueType, DataSourceType>(headingLabel, getter, setter)
          )
        );

      } else if (field == DEFINITION) {
        std::function<boost::optional<model::SpaceLoadDefinition> (model::SpaceType *)>  getter;
        std::function<bool (model::SpaceType *, const model::SpaceLoadDefinition &)> setter;

        addNameLineEditColumn(QString(DEFINITION),
          CastNullAdapter<model::SpaceLoadDefinition>(&model::SpaceLoadDefinition::name),
          CastNullAdapter<model::SpaceLoadDefinition>(&model::SpaceLoadDefinition::setName),
          DataSource(
            allDefinitions,
            false ,
            QSharedPointer<DropZoneConcept>(new DropZoneConceptImpl<model::SpaceLoadDefinition, model::SpaceType>(DEFINITION,
                getter, setter))
          )
          );

      } else if (field == SCHEDULE) {

        addNameLineEditColumn(QString(SCHEDULE),
          CastNullAdapter<model::Schedule>(&model::Schedule::name),
          CastNullAdapter<model::Schedule>(&model::Schedule::setName),
          DataSource(
            schedules,
            true
          )
        );

      } else if (field == ACTIVITYSCHEDULE) {

        addNameLineEditColumn(QString(ACTIVITYSCHEDULE),
          CastNullAdapter<model::Schedule>(&model::Schedule::name),
          CastNullAdapter<model::Schedule>(&model::Schedule::setName),
          DataSource(
            activityLevelSchedules,
            true
          )
        );

      }

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
      std::function<std::vector<model::ModelObject> (const model::SpaceType &)> flowRates(
        [](const model::SpaceType &s) {
          auto rates = s.spaceInfiltrationDesignFlowRates();
          return std::vector<model::ModelObject>(rates.begin(), rates.end());
        }
      );

      addNameLineEditColumn(QString(SPACEINFILTRATIONDESIGNFLOWRATES),
        CastNullAdapter<model::SpaceInfiltrationDesignFlowRate>(&model::SpaceInfiltrationDesignFlowRate::name),
        CastNullAdapter<model::SpaceInfiltrationDesignFlowRate>(&model::SpaceInfiltrationDesignFlowRate::setName),
        DataSource(
          flowRates,
          true, 
          QSharedPointer<DropZoneConcept>(new DropZoneConceptImpl<model::SpaceInfiltrationDesignFlowRate, model::SpaceType>(SPACEINFILTRATIONDESIGNFLOWRATES,
             getter, setter))
        )
      );

    } else if (field == SPACEINFILTRATIONEFFECTIVELEAKAGEAREAS) {
      std::function<boost::optional<model::SpaceInfiltrationEffectiveLeakageArea>(model::SpaceType *)>  getter;
      std::function<bool(model::SpaceType *, const model::SpaceInfiltrationEffectiveLeakageArea &)> setter;
      std::function<std::vector<model::SpaceInfiltrationEffectiveLeakageArea>(const model::SpaceType &)> leakageAreas(
        [](const model::SpaceType &s) {
        return s.spaceInfiltrationEffectiveLeakageAreas();
      }
      );

      addNameLineEditColumn(QString(SPACEINFILTRATIONEFFECTIVELEAKAGEAREAS),
        CastNullAdapter<model::SpaceInfiltrationEffectiveLeakageArea>(&model::SpaceInfiltrationEffectiveLeakageArea::name),
        CastNullAdapter<model::SpaceInfiltrationEffectiveLeakageArea>(&model::SpaceInfiltrationEffectiveLeakageArea::setName),
        DataSource(
        leakageAreas,
        true,
        QSharedPointer<DropZoneConcept>(new DropZoneConceptImpl<model::SpaceInfiltrationEffectiveLeakageArea, model::SpaceType>(SPACEINFILTRATIONEFFECTIVELEAKAGEAREAS,
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

