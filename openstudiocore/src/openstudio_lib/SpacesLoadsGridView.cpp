/**********************************************************************
*  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#include "SpacesLoadsGridView.hpp"

#include "OSDropZone.hpp"
#include "OSItemSelectorButtons.hpp"

#include "../shared_gui_components/OSGridView.hpp"

#include "../model/ElectricEquipment.hpp"
#include "../model/ElectricEquipment_Impl.hpp"
#include "../model/ElectricEquipmentDefinition.hpp"
#include "../model/ElectricEquipmentDefinition_Impl.hpp"
#include "../model/GasEquipment.hpp"
#include "../model/GasEquipment_Impl.hpp"
#include "../model/GasEquipmentDefinition.hpp"
#include "../model/GasEquipmentDefinition_Impl.hpp"
#include "../model/HotWaterEquipment.hpp"
#include "../model/HotWaterEquipment_Impl.hpp"
#include "../model/HotWaterEquipmentDefinition.hpp"
#include "../model/HotWaterEquipmentDefinition_Impl.hpp"
#include "../model/InternalMass.hpp"
#include "../model/InternalMass_Impl.hpp"
#include "../model/InternalMassDefinition.hpp"
#include "../model/InternalMassDefinition_Impl.hpp"
#include "../model/Lights.hpp"
#include "../model/Lights_Impl.hpp"
#include "../model/LightsDefinition.hpp"
#include "../model/LightsDefinition_Impl.hpp"
#include "../model/Luminaire.hpp"
#include "../model/Luminaire_Impl.hpp"
#include "../model/LuminaireDefinition.hpp"
#include "../model/LuminaireDefinition_Impl.hpp"
#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/ModelObject.hpp"
#include "../model/ModelObject_Impl.hpp"
#include "../model/OtherEquipment.hpp"
#include "../model/OtherEquipment_Impl.hpp"
#include "../model/OtherEquipmentDefinition.hpp"
#include "../model/OtherEquipmentDefinition_Impl.hpp"
#include "../model/People.hpp"
#include "../model/People_Impl.hpp"
#include "../model/PeopleDefinition.hpp"
#include "../model/PeopleDefinition_Impl.hpp"
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
#include "../model/Space.hpp"
#include "../model/Space_Impl.hpp"
#include "../model/SteamEquipment.hpp"
#include "../model/SteamEquipment_Impl.hpp"
#include "../model/SteamEquipmentDefinition.hpp"
#include "../model/SteamEquipmentDefinition_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/idd/IddEnums.hxx"
#include "../utilities/idd/OS_Space_FieldEnums.hxx"

#include <QCheckBox>

// These defines provide a common area for field display names
// used on column headers, and other grid widgets

#define NAME "Space Name"
#define SELECTED "All"

// GENERAL
#define LOADNAME "Load Name"
#define MULTIPLIER "Multiplier"
#define DEFINITION "Definition"
#define SCHEDULE "Schedule"
#define ACTIVITYSCHEDULE "Activity Schedule\n(People Only)"

namespace openstudio {

  struct ModelObjectNameSorter{
    // sort by name
    bool operator()(const model::ModelObject & lhs, const model::ModelObject & rhs){
      return (lhs.name() < rhs.name());
    }
  };

  SpacesLoadsGridView::SpacesLoadsGridView(bool isIP, const model::Model & model, QWidget * parent)
    : SpacesSubtabGridView(isIP, model, parent)
  {
    showStoryFilter();
    showThermalZoneFilter();
    showSpaceTypeFilter();
    showLoadTypeFilter();

    m_filterGridLayout->setRowStretch(m_filterGridLayout->rowCount(), 100);
    m_filterGridLayout->setColumnStretch(m_filterGridLayout->columnCount(), 100);

    m_gridController = new SpacesLoadsGridController(isIP, "Space", IddObjectType::OS_Space, model, m_spacesModelObjects);
    m_gridView = new OSGridView(m_gridController, "Space", "Drop\nSpace", false, parent);

    setGridController(m_gridController);
    setGridView(m_gridView);

    m_gridView->m_contentLayout->addLayout(m_filterGridLayout);
    m_gridView->m_contentLayout->addSpacing(7);
    m_gridView->m_dropZone->hide();

    onClearSelection();
  }

  void SpacesLoadsGridView::onSelectItem()
  {
    //m_itemSelectorButtons->enableAddButton();
    m_itemSelectorButtons->enableCopyButton();
    m_itemSelectorButtons->enableRemoveButton();
    m_itemSelectorButtons->enablePurgeButton();
  }

  void SpacesLoadsGridView::onClearSelection()
  {
    //m_itemSelectorButtons->disableAddButton();
    m_itemSelectorButtons->disableCopyButton();
    m_itemSelectorButtons->disableRemoveButton();
    m_itemSelectorButtons->disablePurgeButton();
  }

  SpacesLoadsGridController::SpacesLoadsGridController(bool isIP,
    const QString & headerText,
    IddObjectType iddObjectType,
    model::Model model,
    std::vector<model::ModelObject> modelObjects) :
    OSGridController(isIP, headerText, iddObjectType, model, modelObjects)
  {
    setCategoriesAndFields();
  }

  void SpacesLoadsGridController::setCategoriesAndFields()
  {
    {
      std::vector<QString> fields;
      fields.push_back(LOADNAME);
      fields.push_back(MULTIPLIER);
      fields.push_back(DEFINITION);
      fields.push_back(SCHEDULE);
      fields.push_back(ACTIVITYSCHEDULE);
      std::pair<QString, std::vector<QString> > categoryAndFields = std::make_pair(QString("General"), fields);
      m_categoriesAndFields.push_back(categoryAndFields);
    }

    OSGridController::setCategoriesAndFields();
  }

  void SpacesLoadsGridController::categorySelected(int index)
  {
    OSGridController::categorySelected(index);
  }

  void SpacesLoadsGridController::addColumns(const QString &category, std::vector<QString> & fields)
  {
    // always show name and selected columns
    fields.insert(fields.begin(), { NAME, SELECTED });

    m_baseConcepts.clear();

    for (const auto &field : fields) {

      if (field == NAME) {
        addNameLineEditColumn(Heading(QString(NAME), false, false),
          false,
          false,
          CastNullAdapter<model::Space>(&model::Space::name),
          CastNullAdapter<model::Space>(&model::Space::setName)
          );
      }
      else {
        // Create a lambda function that collates all of the loads in a space 
        // and returns them as an std::vector
        std::function<std::vector<model::ModelObject>(const model::Space &)> allLoads(
          [](const model::Space &t_space) {
          std::vector<model::ModelObject> loads;
          auto InternalMass = t_space.internalMass();
          auto People = t_space.people();
          auto Lights = t_space.lights();
          auto Luminaire = t_space.luminaires();
          auto ElectricEquipment = t_space.electricEquipment();
          auto GasEquipment = t_space.gasEquipment();
          auto HotWaterEquipment = t_space.hotWaterEquipment();
          auto SteamEquipment = t_space.steamEquipment();
          auto OtherEquipment = t_space.otherEquipment();
          auto SpaceInfiltrationDesignFlowRate = t_space.spaceInfiltrationDesignFlowRates();
          auto SpaceInfiltrationEffectiveLeakageArea = t_space.spaceInfiltrationEffectiveLeakageAreas();

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

        std::function<std::vector<boost::optional<model::ModelObject>>(const model::Space &)> allLoadInstances(
          [allLoads](const model::Space &t_space) {
          std::vector<boost::optional<model::ModelObject>> loadInstances;
          for (const auto &l : allLoads(t_space))
          {
            loadInstances.push_back(boost::optional<model::ModelObject>(l.optionalCast<model::SpaceLoadInstance>()));
          }
          return loadInstances;
        }
        );

        std::function<std::vector<boost::optional<model::ModelObject>>(const model::Space &)> allLoadsWithSchedules(
          [allLoads](const model::Space &t_space) {
          std::vector<boost::optional<model::ModelObject>> retval;
          for (auto &l : allLoads(t_space))
          {
            // internal mass does not have a schedule
            if (!l.optionalCast<model::InternalMass>())
            {
              retval.push_back(boost::optional<model::ModelObject>(std::move(l)));
            }
            else {
              retval.emplace_back();
            }
          }
          return retval;
        }
        );

        std::function<std::vector<boost::optional<model::ModelObject>>(const model::Space &)> allLoadsWithActivityLevelSchedules(
          [allLoads](const model::Space &t_space) {
          std::vector<boost::optional<model::ModelObject>> retval;
          for (const auto &l : allLoads(t_space))
          {
            // only people have activity schedules, so this effectively gives us only
            // the People objects while inserting blanks for those which are not people,
            // which is what we want
            retval.push_back(boost::optional<model::ModelObject>(l.optionalCast<model::People>()));
          }
          return retval;
        }
        );

        std::function<std::vector<boost::optional<model::ModelObject>>(const model::Space &)> allDefinitions(
          [allLoadInstances](const model::Space &t_space) {
          std::vector<boost::optional<model::ModelObject>> definitions;
          for (const auto &l : allLoadInstances(t_space))
          {
            if (l)
            {
              definitions.push_back(l->cast<model::SpaceLoadInstance>().definition());
            }
            else {
              definitions.emplace_back();
            }
          }
          return definitions;
        }
        );

        std::function<std::vector<boost::optional<model::ModelObject>>(const model::Space &)> allLoadsWithMultipliers(
          [](const model::Space &t_space) {
          std::vector<boost::optional<model::ModelObject>> loads;
          auto InternalMass = t_space.internalMass();
          auto People = t_space.people();
          auto Lights = t_space.lights();
          auto Luminaire = t_space.luminaires();
          auto ElectricEquipment = t_space.electricEquipment();
          auto GasEquipment = t_space.gasEquipment();
          auto HotWaterEquipment = t_space.hotWaterEquipment();
          auto SteamEquipment = t_space.steamEquipment();
          auto OtherEquipment = t_space.otherEquipment();
          auto SpaceInfiltrationDesignFlowRate = t_space.spaceInfiltrationDesignFlowRates();
          auto SpaceInfiltrationEffectiveLeakageArea = t_space.spaceInfiltrationEffectiveLeakageAreas();

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

          for (unsigned i = 0; i < SpaceInfiltrationDesignFlowRate.size(); ++i)
          {
            loads.emplace_back();
          }

          for (unsigned i = 0; i < SpaceInfiltrationEffectiveLeakageArea.size(); ++i)
          {
            loads.emplace_back();
          }

          return loads;
        }
        );

        std::function<double(model::ModelObject *)> multiplier(
          [allLoads](model::ModelObject *t_modelObject) {
          double retval = 0;

          boost::optional<model::InternalMass> im = t_modelObject->optionalCast<model::InternalMass>();
          if (im)
          {
            retval = im->multiplier();
            return retval;
          }

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
          boost::optional<model::InternalMass> im = t_modelObject->optionalCast<model::InternalMass>();
          if (im)
          {
            im->setMultiplier(multiplier);
            return;
          }

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

          // Should never get here
          OS_ASSERT(false);
        }
        );

        std::function<bool(model::ModelObject *, const model::Schedule &)> setActivityLevelSchedule(
          [](model::ModelObject *l, model::Schedule t_s) {
          if (boost::optional<model::People> p = l->optionalCast<model::People>())
          {
            return p->setActivityLevelSchedule(t_s);
          }

          OS_ASSERT(false);
          return false;
        }
        );

        boost::optional<std::function<void(model::ModelObject *)> > resetActivityLevelSchedule(
          [](model::ModelObject *l) {
          if (boost::optional<model::People> p = l->optionalCast<model::People>())
          {
            p->resetActivityLevelSchedule();
          }
          else {
            //OS_ASSERT(false); TODO
          }
        }
        );

        std::function<bool(model::ModelObject *, const model::Schedule &)> setSchedule(
          [](model::ModelObject *l, model::Schedule t_s) {

          if (boost::optional<model::People> p = l->optionalCast<model::People>())
          {
            return p->setNumberofPeopleSchedule(t_s);
          }

          if (boost::optional<model::Lights> light = l->optionalCast<model::Lights>())
          {
            return light->setSchedule(t_s);
          }

          if (boost::optional<model::Luminaire> lum = l->optionalCast<model::Luminaire>())
          {
            return lum->setSchedule(t_s);
          }

          if (boost::optional<model::ElectricEquipment> e = l->optionalCast<model::ElectricEquipment>())
          {
            return e->setSchedule(t_s);
          }

          if (boost::optional<model::GasEquipment> g = l->optionalCast<model::GasEquipment>())
          {
            return g->setSchedule(t_s);
          }

          if (boost::optional<model::HotWaterEquipment> h = l->optionalCast<model::HotWaterEquipment>())
          {
            return h->setSchedule(t_s);
          }

          if (boost::optional<model::SteamEquipment> se = l->optionalCast<model::SteamEquipment>())
          {
            return se->setSchedule(t_s);
          }

          if (boost::optional<model::OtherEquipment> o = l->optionalCast<model::OtherEquipment>())
          {
            return o->setSchedule(t_s);
          }

          if (boost::optional<model::SpaceInfiltrationDesignFlowRate> f = l->optionalCast<model::SpaceInfiltrationDesignFlowRate>())
          {
            return f->setSchedule(t_s);
          }

          if (boost::optional<model::SpaceInfiltrationEffectiveLeakageArea> la = l->optionalCast<model::SpaceInfiltrationEffectiveLeakageArea>())
          {
            return la->setSchedule(t_s);
          }

          OS_ASSERT(false);
          return false;
        }
        );

        boost::optional<std::function<void(model::ModelObject *)> > resetSchedule(
          [](model::ModelObject *l) {

          if (boost::optional<model::People> p = l->optionalCast<model::People>())
          {
            p->resetNumberofPeopleSchedule();
          }
          else if (boost::optional<model::Lights> light = l->optionalCast<model::Lights>())
          {
            light->resetSchedule();
          }
          else if (boost::optional<model::Luminaire> lum = l->optionalCast<model::Luminaire>())
          {
            lum->resetSchedule();
          }
          else if (boost::optional<model::ElectricEquipment> e = l->optionalCast<model::ElectricEquipment>())
          {
            e->resetSchedule();
          }
          else if (boost::optional<model::GasEquipment> g = l->optionalCast<model::GasEquipment>())
          {
            g->resetSchedule();
          }
          else if (boost::optional<model::HotWaterEquipment> h = l->optionalCast<model::HotWaterEquipment>())
          {
            h->resetSchedule();
          }
          else if (boost::optional<model::SteamEquipment> se = l->optionalCast<model::SteamEquipment>())
          {
            se->resetSchedule();
          }
          else if (boost::optional<model::OtherEquipment> o = l->optionalCast<model::OtherEquipment>())
          {
            o->resetSchedule();
          }
          else if (boost::optional<model::SpaceInfiltrationDesignFlowRate> f = l->optionalCast<model::SpaceInfiltrationDesignFlowRate>())
          {
            f->resetSchedule();
          }
          else if (boost::optional<model::SpaceInfiltrationEffectiveLeakageArea> la = l->optionalCast<model::SpaceInfiltrationEffectiveLeakageArea>())
          {
            la->resetSchedule();
          }
          else if (boost::optional<model::InternalMass> im = l->optionalCast<model::InternalMass>())
          {
            // Note: InternalMass does not have a schedule
          }
          else
          {
            OS_ASSERT(false);
          }
        }
        );

        std::function<boost::optional<model::Schedule>(model::ModelObject *)> activityLevelSchedule(
          [](model::ModelObject *l) {
          if (boost::optional<model::People> p = l->optionalCast<model::People>())
          {
            return p->activityLevelSchedule();
          }

          // should be impossible to get here
          OS_ASSERT(false);
          return boost::optional<model::Schedule>();
        }
        );

        std::function<boost::optional<model::Schedule>(model::ModelObject *)> schedule(
          [](model::ModelObject *l) {
          if (boost::optional<model::InternalMass> im = l->optionalCast<model::InternalMass>())
          {
            // Note: InternalMass does not have a schedule
            return boost::optional<model::Schedule>();
          }

          if (boost::optional<model::People> p = l->optionalCast<model::People>())
          {
            return p->numberofPeopleSchedule();
          }

          if (boost::optional<model::Lights> light = l->optionalCast<model::Lights>())
          {
            return light->schedule();
          }

          if (boost::optional<model::Luminaire> lum = l->optionalCast<model::Luminaire>())
          {
            return lum->schedule();
          }

          if (boost::optional<model::ElectricEquipment> e = l->optionalCast<model::ElectricEquipment>())
          {
            return e->schedule();
          }

          if (boost::optional<model::GasEquipment> g = l->optionalCast<model::GasEquipment>())
          {
            return g->schedule();
          }

          if (boost::optional<model::HotWaterEquipment> h = l->optionalCast<model::HotWaterEquipment>())
          {
            return h->schedule();
          }

          if (boost::optional<model::SteamEquipment> se = l->optionalCast<model::SteamEquipment>())
          {
            return se->schedule();
          }

          if (boost::optional<model::OtherEquipment> o = l->optionalCast<model::OtherEquipment>())
          {
            return o->schedule();
          }

          if (boost::optional<model::SpaceInfiltrationDesignFlowRate> f = l->optionalCast<model::SpaceInfiltrationDesignFlowRate>())
          {
            return f->schedule();
          }

          if (boost::optional<model::SpaceInfiltrationEffectiveLeakageArea> la = l->optionalCast<model::SpaceInfiltrationEffectiveLeakageArea>())
          {
            return la->schedule();
          }

          // should be impossible to get here
          OS_ASSERT(false);
          return boost::optional<model::Schedule>();
        }
        );

        std::function<std::vector<boost::optional<model::ModelObject>>(const model::Space &)> schedules(
          [allLoads, schedule](const model::Space &t_space) {
          std::vector<boost::optional<model::ModelObject>> retval;

          for (auto &l : allLoads(t_space))
          {
            retval.push_back(boost::optional<model::ModelObject>(schedule(&l)));
          }

          return retval;
        }
        );

        std::function<std::vector<boost::optional<model::ModelObject>>(const model::Space &)> activityLevelSchedules(
          [allLoads](const model::Space &t_space) {
          std::vector<boost::optional<model::ModelObject>> retval;

          for (const auto &l : allLoads(t_space))
          {
            boost::optional<model::People> p = l.optionalCast<model::People>();
            if (p)
            {
              auto als = p->activityLevelSchedule();
              retval.push_back(boost::optional<model::ModelObject>(als));
            }
            else {
              retval.emplace_back();
            }
          }

          return retval;
        }
        );

        if (field == LOADNAME) {

          // Here we create a NameLineEdit column, but this one includes a "DataSource" object
          // The DataSource object is used in OSGridController::widgetAt to make a list of NameLineEdit widgets
          // for each Space that is passed in.
          //
          // Notice that it takes the "allLoads" functor from above.
          //
          // Just as an implementation note, it would be possible to use the DataSource as an alternative
          // to the ProxyAdapter function, if the DataSource were to return a vector of 1.
          //
          // The final argument to DataSource tells the system that we want an additional widget to be displayed
          // at the bottom of each list. In this case, it's a dropZone. Any type of BaseConcept would work.

          addLoadNameColumn(Heading(QString(LOADNAME), true, false),
            CastNullAdapter<model::SpaceLoad>(&model::SpaceLoad::name),
            CastNullAdapter<model::SpaceLoad>(&model::SpaceLoad::setName),
            boost::optional<std::function<void(model::SpaceLoad *)>>(
            std::function<void(model::SpaceLoad *)>(
            [](model::SpaceLoad *t_sl)
          {
            t_sl->remove();
          }
            )
            ),
            DataSource(
            allLoads,
            true
            )
            );

        }
        else if (field == SELECTED) {
          auto checkbox = QSharedPointer<QCheckBox>(new QCheckBox());
          checkbox->setToolTip("Check to select all rows");
          connect(checkbox.data(), &QCheckBox::stateChanged, this, &SpacesLoadsGridController::selectAllStateChanged);
          connect(checkbox.data(), &QCheckBox::stateChanged, this->gridView(), &OSGridView::gridRowSelectionChanged);

          addSelectColumn(Heading(QString(SELECTED), false, false, checkbox), "Check to select this row",
            DataSource(
            allLoads,
            true
            )
            );
        }
        else if (field == MULTIPLIER) {
          addValueEditColumn(Heading(QString(MULTIPLIER)),
            multiplier,
            setMultiplier,
            boost::optional<std::function<void(model::ModelObject *)>>(),
            boost::optional<std::function<bool(model::ModelObject *)>>(),
            DataSource(
            allLoadsWithMultipliers,
            true
            //QSharedPointer<DropZoneConcept>(new DropZoneConceptImpl<ValueType, DataSourceType>(headingLabel, getter, setter)
            )
            );

        }
        else if (field == DEFINITION) {
          std::function<boost::optional<model::SpaceLoadDefinition>(model::Space *)>  getter;

          std::function<bool(model::Space *, const model::SpaceLoadDefinition &)> setter(
            [](model::Space *t_space, const model::SpaceLoadDefinition &t_definition) {
            boost::optional<model::InternalMassDefinition> im = t_definition.optionalCast<model::InternalMassDefinition>();
            if (im)
            {
              model::InternalMass(*im).setParent(*t_space);
              return true;
            }

            boost::optional<model::PeopleDefinition> p = t_definition.optionalCast<model::PeopleDefinition>();
            if (p)
            {
              model::People(*p).setParent(*t_space);
              return true;
            }

            boost::optional<model::LightsDefinition> light = t_definition.optionalCast<model::LightsDefinition>();
            if (light)
            {
              model::Lights(*light).setParent(*t_space);
              return true;
            }

            boost::optional<model::LuminaireDefinition> lum = t_definition.optionalCast<model::LuminaireDefinition>();
            if (lum)
            {
              model::Luminaire(*lum).setParent(*t_space);
              return true;
            }

            boost::optional<model::ElectricEquipmentDefinition> e = t_definition.optionalCast<model::ElectricEquipmentDefinition>();
            if (e)
            {
              model::ElectricEquipment(*e).setParent(*t_space);
              return true;
            }

            boost::optional<model::GasEquipmentDefinition> g = t_definition.optionalCast<model::GasEquipmentDefinition>();
            if (g)
            {
              model::GasEquipment(*g).setParent(*t_space);
              return true;
            }

            boost::optional<model::HotWaterEquipmentDefinition> h = t_definition.optionalCast<model::HotWaterEquipmentDefinition>();
            if (h)
            {
              model::HotWaterEquipment(*h).setParent(*t_space);
              return true;
            }

            boost::optional<model::SteamEquipmentDefinition> se = t_definition.optionalCast<model::SteamEquipmentDefinition>();
            if (se)
            {
              model::SteamEquipment(*se).setParent(*t_space);
              return true;
            }

            boost::optional<model::OtherEquipmentDefinition> o = t_definition.optionalCast<model::OtherEquipmentDefinition>();
            if (o)
            {
              model::OtherEquipment(*o).setParent(*t_space);
              return true;
            }

            return false;
          }
          );

          addNameLineEditColumn(Heading(QString(DEFINITION), true, false),
            true,
            false,
            CastNullAdapter<model::SpaceLoadDefinition>(&model::SpaceLoadDefinition::name),
            CastNullAdapter<model::SpaceLoadDefinition>(&model::SpaceLoadDefinition::setName),
            boost::optional<std::function<void(model::SpaceLoadDefinition *)>>(),
            DataSource(
            allDefinitions,
            false,
            QSharedPointer<DropZoneConcept>(new DropZoneConceptImpl<model::SpaceLoadDefinition, model::Space>(Heading(DEFINITION),
            getter, setter))
            )
            );
        }
        else if (field == SCHEDULE) {
          addDropZoneColumn(Heading(QString(SCHEDULE)),
            schedule,
            setSchedule,
            resetSchedule,
            DataSource(
            allLoadsWithSchedules,
            true
            )
            );
        }
        else if (field == ACTIVITYSCHEDULE) {
          addDropZoneColumn(Heading(QString(SCHEDULE)),
            activityLevelSchedule,
            setActivityLevelSchedule,
            resetActivityLevelSchedule,
            DataSource(
            allLoadsWithActivityLevelSchedules,
            true
            )
            );
        }
        else {
          // unhandled
          OS_ASSERT(false);
        }
      }
    }
  }

  QString SpacesLoadsGridController::getColor(const model::ModelObject & modelObject)
  {
    QColor defaultColor(Qt::lightGray);
    return defaultColor.name();
  }

  void SpacesLoadsGridController::checkSelectedFields()
  {
    if (!this->m_hasHorizontalHeader) return;

    OSGridController::checkSelectedFields();
  }

  void SpacesLoadsGridController::onItemDropped(const OSItemId& itemId)
  {
  }

  void SpacesLoadsGridController::refreshModelObjects()
  {
    m_modelObjects = subsetCastVector<model::ModelObject>(m_model.getModelObjects<model::Space>());
    std::sort(m_modelObjects.begin(), m_modelObjects.end(), ModelObjectNameSorter());
  }

} // openstudio
