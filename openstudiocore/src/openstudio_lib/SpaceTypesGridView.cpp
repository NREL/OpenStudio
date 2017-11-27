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

#include "SpaceTypesGridView.hpp"

#include "ModelObjectInspectorView.hpp"
#include "ModelObjectItem.hpp"
#include "ModelSubTabView.hpp"
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
#include "../model/ElectricEquipmentDefinition.hpp"
#include "../model/ElectricEquipmentDefinition_Impl.hpp"
#include "../model/ElectricEquipment_Impl.hpp"
#include "../model/GasEquipment.hpp"
#include "../model/GasEquipmentDefinition.hpp"
#include "../model/GasEquipmentDefinition_Impl.hpp"
#include "../model/GasEquipment_Impl.hpp"
#include "../model/HotWaterEquipment.hpp"
#include "../model/HotWaterEquipmentDefinition.hpp"
#include "../model/HotWaterEquipmentDefinition_Impl.hpp"
#include "../model/HotWaterEquipment_Impl.hpp"
#include "../model/InternalMass.hpp"
#include "../model/InternalMassDefinition.hpp"
#include "../model/InternalMassDefinition_Impl.hpp"
#include "../model/InternalMass_Impl.hpp"
#include "../model/Lights.hpp"
#include "../model/LightsDefinition.hpp"
#include "../model/LightsDefinition_Impl.hpp"
#include "../model/Lights_Impl.hpp"
#include "../model/Luminaire.hpp"
#include "../model/LuminaireDefinition.hpp"
#include "../model/LuminaireDefinition_Impl.hpp"
#include "../model/Luminaire_Impl.hpp"
#include "../model/Model.hpp"
#include "../model/ModelObject.hpp"
#include "../model/ModelObject_Impl.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/OtherEquipment.hpp"
#include "../model/OtherEquipmentDefinition.hpp"
#include "../model/OtherEquipmentDefinition_Impl.hpp"
#include "../model/OtherEquipment_Impl.hpp"
#include "../model/People.hpp"
#include "../model/PeopleDefinition.hpp"
#include "../model/PeopleDefinition_Impl.hpp"
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
#include "../model/SpaceLoadDefinition.hpp"
#include "../model/SpaceLoadDefinition_Impl.hpp"
#include "../model/SpaceLoadInstance.hpp"
#include "../model/SpaceLoadInstance_Impl.hpp"
#include "../model/SpaceLoad_Impl.hpp"
#include "../model/SpaceType.hpp"
#include "../model/SpaceType_Impl.hpp"
#include "../model/SteamEquipment.hpp"
#include "../model/SteamEquipmentDefinition.hpp"
#include "../model/SteamEquipmentDefinition_Impl.hpp"
#include "../model/SteamEquipment_Impl.hpp"

#include "../utilities/idd/IddEnums.hxx"
#include "../utilities/idd/OS_SpaceType_FieldEnums.hxx"

#include <QBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>

// These defines provide a common area for field display names
// used on column headers, and other grid widgets

#define NAME "Space Type Name"
#define SELECTED "All"

// GENERAL
#define RENDERINGCOLOR "Rendering Color"
#define DEFAULTCONSTRUCTIONSET "Default Construction Set"
#define DEFAULTSCHEDULESET "Default Schedule Set"
#define DESIGNSPECIFICATIONOUTDOORAIR "Design Specification Outdoor Air"
#define SPACEINFILTRATIONDESIGNFLOWRATES "Space Infiltration Design Flow Rates"
#define SPACEINFILTRATIONEFFECTIVELEAKAGEAREAS "Space Infiltration Effective Leakage Areas"

// LOADS
#define LOADNAME "Load Name"
#define MULTIPLIER "Multiplier"
#define DEFINITION "Definition"
#define SCHEDULE "Schedule"
#define ACTIVITYSCHEDULE "Activity Schedule\n(People Only)"

// MEASURE TAGS
#define STANDARDSBUILDINGTYPE "Standards Building Type\n(Optional)"
#define STANDARDSSPACETYPE "Standards Space Type\n(Optional)"

// LOAD TYPES
#define SHOWALLLOADS "Show all loads"
#define INTERNALMASS "Internal Mass"
#define PEOPLE "People"
#define LIGHTS "Lights"
#define LUMINAIRE "Luminaire"
#define ELECTRICEQUIPMENT "Electric Equipment"
#define GASEQUIPMENT "Gas Equipment"
#define HOTWATEREQUIPMENT "Hot Water Equipment"
#define STEAMEQUIPMENT "Steam Equipment"
#define OTHEREQUIPMENT "Other Equipment"
#define SPACEINFILTRATIONDESIGNFLOWRATE "Space Infiltration Design Flow Rate"
#define SPACEINFILTRATIONEFFECTIVELEAKAGEAREA "Space Infiltration Effective Leakage Area"

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
    auto mainLayout = new QVBoxLayout();
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(mainLayout);

    auto spaceTypes = model.getConcreteModelObjects<model::SpaceType>();
    auto spaceTypeModelObjects = subsetCastVector<model::ModelObject>(spaceTypes);

    m_gridController = new SpaceTypesGridController(m_isIP, "Space Types", IddObjectType::OS_SpaceType, model, spaceTypeModelObjects);
    auto gridView = new OSGridView(m_gridController, "Space Types", "Drop\nSpace Type", false, parent);

    // Load Filter

    QLabel * label = nullptr;

    QVBoxLayout * layout = nullptr;

    bool isConnected = false;

    auto filterGridLayout = new QGridLayout();
    filterGridLayout->setContentsMargins(7, 4, 0, 8);
    filterGridLayout->setSpacing(5);

    label = new QLabel();
    label->setText("Filter:");
    label->setObjectName("H2");
    filterGridLayout->addWidget(label, filterGridLayout->rowCount(), 0, Qt::AlignTop | Qt::AlignLeft);

    layout = new QVBoxLayout();

    m_filterLabel = new QLabel();
    m_filterLabel->setText("Load Type");
    m_filterLabel->setObjectName("H3");
    layout->addWidget(m_filterLabel, Qt::AlignTop | Qt::AlignLeft);

    m_filters = new QComboBox();
    m_filters->setFixedWidth(1.5*OSItem::ITEM_WIDTH);

    {
      m_filters->addItem(SHOWALLLOADS);
    }

    {
      const QPixmap * pixMap = new QPixmap(":images/mini_icons/internal_mass.png");
      OS_ASSERT(pixMap);
      m_filters->addItem(*pixMap, INTERNALMASS);
    }

    {
      const QPixmap * pixMap = new QPixmap(":images/mini_icons/people.png");
      OS_ASSERT(pixMap);
      m_filters->addItem(*pixMap, PEOPLE);
    }

    {
      const QPixmap * pixMap = new QPixmap(":images/mini_icons/lights.png");
      OS_ASSERT(pixMap);
      m_filters->addItem(*pixMap, LIGHTS);
    }

    {
      const QPixmap * pixMap = new QPixmap(":images/mini_icons/luminaire.png");
      OS_ASSERT(pixMap);
      m_filters->addItem(*pixMap, LUMINAIRE);
    }

    {
      const QPixmap * pixMap = new QPixmap(":images/mini_icons/electric_equipment.png");
      OS_ASSERT(pixMap);
      m_filters->addItem(*pixMap, ELECTRICEQUIPMENT);
    }

    {
      const QPixmap * pixMap = new QPixmap(":images/mini_icons/gas_equipment.png");
      OS_ASSERT(pixMap);
      m_filters->addItem(*pixMap, GASEQUIPMENT);
    }

    {
      const QPixmap * pixMap = new QPixmap(":images/mini_icons/steam_equipment.png");
      OS_ASSERT(pixMap);
      m_filters->addItem(*pixMap, HOTWATEREQUIPMENT);
    }

    {
      const QPixmap * pixMap = new QPixmap(":images/mini_icons/steam_equipment.png");
      OS_ASSERT(pixMap);
      m_filters->addItem(*pixMap, STEAMEQUIPMENT);
    }

    {
      const QPixmap * pixMap = new QPixmap(":images/mini_icons/other_equipment.png");
      OS_ASSERT(pixMap);
      m_filters->addItem(*pixMap, OTHEREQUIPMENT);
    }

    {
      auto pixMap = new QPixmap(":images/mini_icons/infiltration.png");
      OS_ASSERT(pixMap);
      m_filters->addItem(*pixMap, SPACEINFILTRATIONDESIGNFLOWRATE);
    }

    {
      auto pixMap = new QPixmap(":images/mini_icons/mini_infiltration_leak.png");
      OS_ASSERT(pixMap);
      m_filters->addItem(*pixMap, SPACEINFILTRATIONEFFECTIVELEAKAGEAREA);
    }

    disableFilter();
    layout->addWidget(m_filters, Qt::AlignTop | Qt::AlignLeft);

    layout->addStretch();

    filterGridLayout->addLayout(layout, filterGridLayout->rowCount() - 1, 1);

    filterGridLayout->setRowStretch(filterGridLayout->rowCount(), 100);
    filterGridLayout->setColumnStretch(filterGridLayout->columnCount(), 100);

    gridView->m_contentLayout->addLayout(filterGridLayout);

    gridView->m_contentLayout->addSpacing(7);

    mainLayout->addWidget(gridView, 0, Qt::AlignTop);

    mainLayout->addStretch(1);

    // GridController

    OS_ASSERT(m_gridController);
    isConnected = connect(m_filters, &QComboBox::currentTextChanged, m_gridController, &openstudio::SpaceTypesGridController::filterChanged);
    OS_ASSERT(isConnected);

    isConnected = connect(gridView, SIGNAL(dropZoneItemClicked(OSItem*)), this, SIGNAL(dropZoneItemClicked(OSItem*)));
    OS_ASSERT(isConnected);

    isConnected = connect(this, SIGNAL(selectionCleared()), gridView, SLOT(onSelectionCleared()));
    OS_ASSERT(isConnected);

    isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_gridController, SIGNAL(toggleUnitsClicked(bool)));
    OS_ASSERT(isConnected);

    isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_gridController, SLOT(toggleUnits(bool)));
    OS_ASSERT(isConnected);

    std::vector<model::SpaceType> spaceType = model.getConcreteModelObjects<model::SpaceType>(); // NOTE for horizontal system lists
  }

  std::vector<model::ModelObject> SpaceTypesGridView::selectedObjects() const
  {
    return m_gridController->selectedObjects();
  }

  void SpaceTypesGridView::enableFilter()
  {
    m_filterLabel->setEnabled(true);
    m_filters->setEnabled(true);
  }

  void SpaceTypesGridView::disableFilter()
  {
    m_filterLabel->setEnabled(false);
    m_filters->setEnabled(false);
  }

  SpaceTypesGridController::SpaceTypesGridController(bool isIP,
    const QString & headerText,
    IddObjectType iddObjectType,
    model::Model model,
    std::vector<model::ModelObject> modelObjects) :
    OSGridController(isIP, headerText, iddObjectType, model, modelObjects)
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
      std::pair<QString, std::vector<QString> > categoryAndFields = std::make_pair(QString("General"), fields);
      m_categoriesAndFields.push_back(categoryAndFields);
    }

  {
    std::vector<QString> fields;
    fields.push_back(LOADNAME);
    fields.push_back(MULTIPLIER);
    fields.push_back(DEFINITION);
    fields.push_back(SCHEDULE);
    fields.push_back(ACTIVITYSCHEDULE);
    std::pair<QString, std::vector<QString> > categoryAndFields = std::make_pair(QString("Loads"), fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    fields.push_back(STANDARDSBUILDINGTYPE);
    fields.push_back(STANDARDSSPACETYPE);
    std::pair<QString, std::vector<QString> > categoryAndFields = std::make_pair(QString("Measure\nTags"), fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  OSGridController::setCategoriesAndFields();

  }

  void SpaceTypesGridController::filterChanged(const QString & text)
  {
    LOG(Debug, "Load filter changed: " << toString(text));

    auto objectSelector = getObjectSelector();
    if (text == SHOWALLLOADS)
    {
      objectSelector->resetObjectFilter();
    }
    else {
      objectSelector->setObjectFilter(
        [text](const model::ModelObject &obj)->bool {
        try {
          //obj.cast<model::SpaceLoadInstance>();  TODO uncomment with correct type
          // This is a spaceloadinstance, so we want to see if it matches our filter

          if (text == INTERNALMASS)
          {
            return static_cast<bool>(obj.optionalCast<model::InternalMass>());
          }
          else if (text == PEOPLE)
          {
            return static_cast<bool>(obj.optionalCast<model::People>());
          }
          else if (text == LIGHTS)
          {
            return static_cast<bool>(obj.optionalCast<model::Lights>());
          }
          else if (text == LUMINAIRE)
          {
            return static_cast<bool>(obj.optionalCast<model::Luminaire>());
          }
          else if (text == ELECTRICEQUIPMENT)
          {
            return static_cast<bool>(obj.optionalCast<model::ElectricEquipment>());
          }
          else if (text == GASEQUIPMENT)
          {
            return static_cast<bool>(obj.optionalCast<model::GasEquipment>());
          }
          else if (text == HOTWATEREQUIPMENT)
          {
            return static_cast<bool>(obj.optionalCast<model::HotWaterEquipment>());
          }
          else if (text == STEAMEQUIPMENT)
          {
            return static_cast<bool>(obj.optionalCast<model::SteamEquipment>());
          }
          else if (text == OTHEREQUIPMENT)
          {
            return static_cast<bool>(obj.optionalCast<model::OtherEquipment>());
          }
          else if (text == SPACEINFILTRATIONDESIGNFLOWRATE)
          {
            return static_cast<bool>(obj.optionalCast<model::SpaceInfiltrationDesignFlowRate>());
          }
          else if (text == SPACEINFILTRATIONEFFECTIVELEAKAGEAREA)
          {
            return static_cast<bool>(obj.optionalCast<model::SpaceInfiltrationEffectiveLeakageArea>());
          }
          else
          {
            // Should never get here
            OS_ASSERT(false);
            return false;
          }
        }
        catch (...) {
          return true; // this isn't a space load instance, so don't apply filtering
        }
      }
      );
    }
  }

  SpaceTypesGridView * SpaceTypesGridController::spaceTypesGridView()
  {
    auto gridView = qobject_cast<OSGridView *>(this->parent());
    OS_ASSERT(gridView);

    auto spaceTypesGridView = qobject_cast<SpaceTypesGridView *>(gridView->parent());

    return spaceTypesGridView;
  }

  void SpaceTypesGridController::categorySelected(int index)
  {
    auto gridView = this->spaceTypesGridView();
    if (gridView) {
      if (gridView->m_filters) {
        gridView->m_filters->setCurrentIndex(0);
      }
    }

    if (spaceTypesGridView()) {
      // Enable filter when viewing loads tab...
      if (index == 1) {
        spaceTypesGridView()->enableFilter();
      }
      // ...or if the custom tab has a loads column
      else if (index == 3) {
        auto categoriesAndFields = this->categoriesAndFields();
        auto fields = categoriesAndFields.at(index);
        for (auto field : fields.second) {
          if (field == LOADNAME) {
            spaceTypesGridView()->enableFilter();
            break;
          }
        }
      }
      else {
        spaceTypesGridView()->disableFilter();
      }
    }

    OSGridController::categorySelected(index);
  }

  void SpaceTypesGridController::addColumns(const QString &category, std::vector<QString> & fields)
  {
    // always show name and selected columns
    fields.insert(fields.begin(), { NAME, SELECTED });

    m_baseConcepts.clear();

    for (const auto &field : fields) {

      if (field == NAME) {
        auto getter = CastNullAdapter<model::SpaceType>(&model::SpaceType::name);
        auto setter = CastNullAdapter<model::SpaceType>(&model::SpaceType::setNameProtected);

        addNameLineEditColumn(Heading(QString(NAME), false, false),
          false,
          false,
          getter,
          setter);

      }
      else if (field == SELECTED && category != "Loads") {
        auto checkbox = QSharedPointer<QCheckBox>(new QCheckBox());
        checkbox->setToolTip("Check to select all rows");
        connect(checkbox.data(), &QCheckBox::stateChanged, this, &SpaceTypesGridController::selectAllStateChanged);
        connect(checkbox.data(), &QCheckBox::stateChanged, this->gridView(), &OSGridView::gridRowSelectionChanged);

        addSelectColumn(Heading(QString(SELECTED), false, false, checkbox), "Check to select this row");
      }
      else if (field == LOADNAME || field == MULTIPLIER || field == DEFINITION || field == SCHEDULE || field == ACTIVITYSCHEDULE || field == SELECTED) {
        // Create a lambda function that collates all of the loads in a space type
        // and returns them as an std::vector
        std::function<std::vector<model::ModelObject>(const model::SpaceType &)> allLoads(
          [](const model::SpaceType &t_spaceType) {
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

        std::function<std::vector<boost::optional<model::ModelObject>>(const model::SpaceType &)> allLoadsWithSchedules(
          [allLoads](const model::SpaceType &t_spaceType) {
          std::vector<boost::optional<model::ModelObject>> retval;
          for (auto &l : allLoads(t_spaceType))
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

        std::function<std::vector<boost::optional<model::ModelObject>>(const model::SpaceType &)> allLoadsWithActivityLevelSchedules(
          [allLoads](const model::SpaceType &t_spaceType) {
          std::vector<boost::optional<model::ModelObject>> retval;
          for (const auto &l : allLoads(t_spaceType))
          {
            // only people have activity schedules, so this effectively gives us only
            // the People objects while inserting blanks for those which are not people,
            // which is what we want
            if (l.optionalCast<model::People>())
            {
              retval.push_back(boost::optional<model::ModelObject>(std::move(l)));
            }
            else 
            {
              retval.emplace_back();
            }
          }
          return retval;
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
            }
            else {
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

        const boost::optional<std::function<void(model::ModelObject *)> > resetMultiplier(
          [](model::ModelObject *t_modelObject) {
          boost::optional<model::InternalMass> im = t_modelObject->optionalCast<model::InternalMass>();
          if (im)
          {
            im->resetMultiplier();
            return;
          }

          boost::optional<model::People> p = t_modelObject->optionalCast<model::People>();
          if (p)
          {
            p->resetMultiplier();
            return;
          }

          boost::optional<model::Lights> light = t_modelObject->optionalCast<model::Lights>();
          if (light)
          {
            light->resetMultiplier();
            return;
          }

          boost::optional<model::Luminaire> lum = t_modelObject->optionalCast<model::Luminaire>();
          if (lum)
          {
            lum->resetMultiplier();
            return;
          }

          boost::optional<model::ElectricEquipment> e = t_modelObject->optionalCast<model::ElectricEquipment>();
          if (e)
          {
            e->resetMultiplier();
            return;
          }

          boost::optional<model::GasEquipment> g = t_modelObject->optionalCast<model::GasEquipment>();
          if (g)
          {
            g->resetMultiplier();
            return;
          }

          boost::optional<model::HotWaterEquipment> h = t_modelObject->optionalCast<model::HotWaterEquipment>();
          if (h)
          {
            h->resetMultiplier();
            return;
          }

          boost::optional<model::SteamEquipment> se = t_modelObject->optionalCast<model::SteamEquipment>();
          if (se)
          {
            se->resetMultiplier();
            return;
          }

          boost::optional<model::OtherEquipment> o = t_modelObject->optionalCast<model::OtherEquipment>();
          if (o)
          {
            o->resetMultiplier();
            return;
          }

          // Should never get here
          OS_ASSERT(false);
        }
        );

        const boost::optional<std::function<bool(model::ModelObject *)> > isMultiplierDefaulted(
          [](model::ModelObject *t_modelObject) {
          boost::optional<model::InternalMass> im = t_modelObject->optionalCast<model::InternalMass>();
          if (im)
          {
            return im->isMultiplierDefaulted();
          }

          boost::optional<model::People> p = t_modelObject->optionalCast<model::People>();
          if (p)
          {
            return p->isMultiplierDefaulted();
          }

          boost::optional<model::Lights> light = t_modelObject->optionalCast<model::Lights>();
          if (light)
          {
            return light->isMultiplierDefaulted();
          }

          boost::optional<model::Luminaire> lum = t_modelObject->optionalCast<model::Luminaire>();
          if (lum)
          {
            return lum->isMultiplierDefaulted();
          }

          boost::optional<model::ElectricEquipment> e = t_modelObject->optionalCast<model::ElectricEquipment>();
          if (e)
          {
            return e->isMultiplierDefaulted();
          }

          boost::optional<model::GasEquipment> g = t_modelObject->optionalCast<model::GasEquipment>();
          if (g)
          {
            return g->isMultiplierDefaulted();
          }

          boost::optional<model::HotWaterEquipment> h = t_modelObject->optionalCast<model::HotWaterEquipment>();
          if (h)
          {
            return h->isMultiplierDefaulted();
          }

          boost::optional<model::SteamEquipment> se = t_modelObject->optionalCast<model::SteamEquipment>();
          if (se)
          {
            return se->isMultiplierDefaulted();
          }

          boost::optional<model::OtherEquipment> o = t_modelObject->optionalCast<model::OtherEquipment>();
          if (o)
          {
            return o->isMultiplierDefaulted();
          }

          // Should never get here
          OS_ASSERT(false);
          return false;
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

        boost::optional<std::function<bool(model::ModelObject *)> > isActivityLevelScheduleDefaulted(
          [](model::ModelObject *l) {
          if (boost::optional<model::People> p = l->optionalCast<model::People>())
          {
            return p->isActivityLevelScheduleDefaulted();
          }

          OS_ASSERT(false);
          return false;
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

        boost::optional<std::function<bool(model::ModelObject *)> > isScheduleDefaulted(
          [](model::ModelObject *l) {

          if (boost::optional<model::People> p = l->optionalCast<model::People>())
          {
            return  p->isNumberofPeopleScheduleDefaulted();
          }
          else if (boost::optional<model::Lights> light = l->optionalCast<model::Lights>())
          {
            return light->isScheduleDefaulted();
          }
          else if (boost::optional<model::Luminaire> lum = l->optionalCast<model::Luminaire>())
          {
            return lum->isScheduleDefaulted();
          }
          else if (boost::optional<model::ElectricEquipment> e = l->optionalCast<model::ElectricEquipment>())
          {
            return e->isScheduleDefaulted();
          }
          else if (boost::optional<model::GasEquipment> g = l->optionalCast<model::GasEquipment>())
          {
            return g->isScheduleDefaulted();
          }
          else if (boost::optional<model::HotWaterEquipment> h = l->optionalCast<model::HotWaterEquipment>())
          {
            return h->isScheduleDefaulted();
          }
          else if (boost::optional<model::SteamEquipment> se = l->optionalCast<model::SteamEquipment>())
          {
            return se->isScheduleDefaulted();
          }
          else if (boost::optional<model::OtherEquipment> o = l->optionalCast<model::OtherEquipment>())
          {
            return o->isScheduleDefaulted();
          }
          else if (boost::optional<model::SpaceInfiltrationDesignFlowRate> f = l->optionalCast<model::SpaceInfiltrationDesignFlowRate>())
          {
            return f->isScheduleDefaulted();
          }
          else if (boost::optional<model::SpaceInfiltrationEffectiveLeakageArea> la = l->optionalCast<model::SpaceInfiltrationEffectiveLeakageArea>())
          {
            return la->isScheduleDefaulted();
          }
          else if (boost::optional<model::InternalMass> im = l->optionalCast<model::InternalMass>())
          {
            // Note: InternalMass does not have a schedule
            return false;
          }
          else
          {
            OS_ASSERT(false);
            return false;
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

        std::function<std::vector<boost::optional<model::ModelObject>>(const model::SpaceType &)> schedules(
          [allLoads, schedule](const model::SpaceType &t_spaceType) {
          std::vector<boost::optional<model::ModelObject>> retval;

          for (auto &l : allLoads(t_spaceType))
          {
            retval.push_back(boost::optional<model::ModelObject>(schedule(&l)));
          }

          return retval;
        }
        );

        std::function<std::vector<boost::optional<model::ModelObject>>(const model::SpaceType &)> activityLevelSchedules(
          [allLoads](const model::SpaceType &t_spaceType) {
          std::vector<boost::optional<model::ModelObject>> retval;

          for (const auto &l : allLoads(t_spaceType))
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
          // for each SpaceType that is passed in.
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
          connect(checkbox.data(), &QCheckBox::stateChanged, this, &SpaceTypesGridController::selectAllStateChanged);
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
            resetMultiplier,
            isMultiplierDefaulted,
            DataSource(
            allLoadsWithMultipliers,
            true
            //QSharedPointer<DropZoneConcept>(new DropZoneConceptImpl<ValueType, DataSourceType>(headingLabel, getter, setter)
            )
            );

        }
        else if (field == DEFINITION) {
          std::function<boost::optional<model::SpaceLoadDefinition>(model::SpaceType *)>  getter;

          std::function<bool(model::SpaceType *, const model::SpaceLoadDefinition &)> setter(
            [](model::SpaceType *t_spaceType, const model::SpaceLoadDefinition &t_definition) {
            boost::optional<model::InternalMassDefinition> im = t_definition.optionalCast<model::InternalMassDefinition>();
            if (im)
            {
              model::InternalMass(*im).setParent(*t_spaceType);
              return true;
            }

            boost::optional<model::PeopleDefinition> p = t_definition.optionalCast<model::PeopleDefinition>();
            if (p)
            {
              model::People(*p).setParent(*t_spaceType);
              return true;
            }

            boost::optional<model::LightsDefinition> light = t_definition.optionalCast<model::LightsDefinition>();
            if (light)
            {
              model::Lights(*light).setParent(*t_spaceType);
              return true;
            }

            boost::optional<model::LuminaireDefinition> lum = t_definition.optionalCast<model::LuminaireDefinition>();
            if (lum)
            {
              model::Luminaire(*lum).setParent(*t_spaceType);
              return true;
            }

            boost::optional<model::ElectricEquipmentDefinition> e = t_definition.optionalCast<model::ElectricEquipmentDefinition>();
            if (e)
            {
              model::ElectricEquipment(*e).setParent(*t_spaceType);
              return true;
            }

            boost::optional<model::GasEquipmentDefinition> g = t_definition.optionalCast<model::GasEquipmentDefinition>();
            if (g)
            {
              model::GasEquipment(*g).setParent(*t_spaceType);
              return true;
            }

            boost::optional<model::HotWaterEquipmentDefinition> h = t_definition.optionalCast<model::HotWaterEquipmentDefinition>();
            if (h)
            {
              model::HotWaterEquipment(*h).setParent(*t_spaceType);
              return true;
            }

            boost::optional<model::SteamEquipmentDefinition> se = t_definition.optionalCast<model::SteamEquipmentDefinition>();
            if (se)
            {
              model::SteamEquipment(*se).setParent(*t_spaceType);
              return true;
            }

            boost::optional<model::OtherEquipmentDefinition> o = t_definition.optionalCast<model::OtherEquipmentDefinition>();
            if (o)
            {
              model::OtherEquipment(*o).setParent(*t_spaceType);
              return true;
            }

            boost::optional<model::SpaceInfiltrationDesignFlowRate> dfr = t_definition.optionalCast<model::SpaceInfiltrationDesignFlowRate>();
            if (dfr)
            {
              model::SpaceInfiltrationDesignFlowRate(*dfr).setParent(*t_spaceType);
              return true;
            }

            boost::optional<model::SpaceInfiltrationEffectiveLeakageArea> ela = t_definition.optionalCast<model::SpaceInfiltrationEffectiveLeakageArea>();
            if (ela)
            {
              model::SpaceInfiltrationEffectiveLeakageArea(*ela).setParent(*t_spaceType);
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
            QSharedPointer<DropZoneConcept>(new DropZoneConceptImpl<model::SpaceLoadDefinition, model::SpaceType>(Heading(DEFINITION),
            getter, setter))
            )
            );

        }
        else if (field == SCHEDULE) {

          addDropZoneColumn(Heading(QString(SCHEDULE)),
            schedule,
            setSchedule,
            resetSchedule,
            isScheduleDefaulted,
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
            isActivityLevelScheduleDefaulted,
            DataSource(
            allLoadsWithActivityLevelSchedules,
            true
            )
            );

        }

      }
      else if (field == DEFAULTCONSTRUCTIONSET){
        addDropZoneColumn(Heading(QString(DEFAULTCONSTRUCTIONSET)),
          CastNullAdapter<model::SpaceType>(&model::SpaceType::defaultConstructionSet),
          CastNullAdapter<model::SpaceType>(&model::SpaceType::setDefaultConstructionSet),
          boost::optional<std::function<void(model::SpaceType*)>>(CastNullAdapter<model::SpaceType>(&model::SpaceType::resetDefaultConstructionSet))
          );

      }
      else if (field == DEFAULTSCHEDULESET){
        addDropZoneColumn(Heading(QString(DEFAULTSCHEDULESET)),
          CastNullAdapter<model::SpaceType>(&model::SpaceType::defaultScheduleSet),
          CastNullAdapter<model::SpaceType>(&model::SpaceType::setDefaultScheduleSet),
          boost::optional<std::function<void(model::SpaceType*)>>(CastNullAdapter<model::SpaceType>(&model::SpaceType::resetDefaultScheduleSet))
          );

      }
      else if (field == DESIGNSPECIFICATIONOUTDOORAIR){
        addDropZoneColumn(Heading(QString(DESIGNSPECIFICATIONOUTDOORAIR)),
          CastNullAdapter<model::SpaceType>(&model::SpaceType::designSpecificationOutdoorAir),
          CastNullAdapter<model::SpaceType>(&model::SpaceType::setDesignSpecificationOutdoorAir),
          boost::optional<std::function<void(model::SpaceType*)>>(CastNullAdapter<model::SpaceType>(&model::SpaceType::resetDesignSpecificationOutdoorAir)),
          boost::optional<std::function<bool(model::SpaceType*)>>(CastNullAdapter<model::SpaceType>(&model::SpaceType::isDesignSpecificationOutdoorAirDefaulted))
          );

      }
      else if (field == RENDERINGCOLOR){
        addRenderingColorColumn(Heading(QString(RENDERINGCOLOR), true, false),
          CastNullAdapter<model::SpaceType>(&model::SpaceType::renderingColor),
          CastNullAdapter<model::SpaceType>(&model::SpaceType::setRenderingColor));

      }
      else if (field == SPACEINFILTRATIONDESIGNFLOWRATES) {
        std::function<boost::optional<model::SpaceInfiltrationDesignFlowRate>(model::SpaceType *)>  getter;

        std::function<bool(model::SpaceType *, const model::SpaceInfiltrationDesignFlowRate &)> setter(
          [](model::SpaceType *t_type, model::SpaceInfiltrationDesignFlowRate t_rate) {
          if (t_rate.spaceType()) {
            boost::optional<model::ModelObject> clone_rate = t_rate.clone(t_rate.model());
            OS_ASSERT(clone_rate);
            auto success = clone_rate->cast<model::SpaceInfiltrationDesignFlowRate>().setSpaceType(*t_type);
            OS_ASSERT(success);
            return success;
          }
          else {
            auto success = t_rate.setSpaceType(*t_type);
            OS_ASSERT(success);
            return success;
          }
        }
        );

        std::function<std::vector<model::ModelObject>(const model::SpaceType &)> flowRates(
          [](const model::SpaceType &s) {
          auto rates = s.spaceInfiltrationDesignFlowRates();
          return std::vector<model::ModelObject>(rates.begin(), rates.end());
        }
        );

        addNameLineEditColumn(Heading(QString(SPACEINFILTRATIONDESIGNFLOWRATES)),
          true,
          true,
          CastNullAdapter<model::SpaceInfiltrationDesignFlowRate>(&model::SpaceInfiltrationDesignFlowRate::name),
          CastNullAdapter<model::SpaceInfiltrationDesignFlowRate>(&model::SpaceInfiltrationDesignFlowRate::setName),
          boost::optional<std::function<void(model::SpaceInfiltrationDesignFlowRate *)>>(
          std::function<void(model::SpaceInfiltrationDesignFlowRate *)>(
          [](model::SpaceInfiltrationDesignFlowRate *t_fr)
        {
          t_fr->resetSpaceType();
        }
          )
          ),
          DataSource(
          flowRates,
          false,
          QSharedPointer<DropZoneConcept>(new DropZoneConceptImpl<model::SpaceInfiltrationDesignFlowRate, model::SpaceType>(Heading(SPACEINFILTRATIONDESIGNFLOWRATES),
          getter, setter))
          )
          );

      }
      else if (field == SPACEINFILTRATIONEFFECTIVELEAKAGEAREAS) {
        std::function<boost::optional<model::SpaceInfiltrationEffectiveLeakageArea>(model::SpaceType *)>  getter;

        std::function<bool(model::SpaceType *, const model::SpaceInfiltrationEffectiveLeakageArea &)> setter(
          [](model::SpaceType *t_type, model::SpaceInfiltrationEffectiveLeakageArea t_area) {
          if (t_area.spaceType()) {
            boost::optional<model::ModelObject> clone_area = t_area.clone(t_area.model());
            OS_ASSERT(clone_area);
            auto success = clone_area->cast<model::SpaceInfiltrationEffectiveLeakageArea>().setSpaceType(*t_type);
            OS_ASSERT(success);
            return success;
          }
          else {
            auto success = t_area.setSpaceType(*t_type);
            OS_ASSERT(success);
            return success;
          }
        }
        );

        std::function<std::vector<model::ModelObject>(const model::SpaceType &)> leakageAreas(
          [](const model::SpaceType &s) {
          auto areas = s.spaceInfiltrationEffectiveLeakageAreas();
          return std::vector<model::ModelObject>(areas.begin(), areas.end());
        }
        );

        addNameLineEditColumn(Heading(QString(SPACEINFILTRATIONEFFECTIVELEAKAGEAREAS)),
          true,
          true,
          CastNullAdapter<model::SpaceInfiltrationEffectiveLeakageArea>(&model::SpaceInfiltrationEffectiveLeakageArea::name),
          CastNullAdapter<model::SpaceInfiltrationEffectiveLeakageArea>(&model::SpaceInfiltrationEffectiveLeakageArea::setName),
          boost::optional<std::function<void(model::SpaceInfiltrationEffectiveLeakageArea *)>>(
          std::function<void(model::SpaceInfiltrationEffectiveLeakageArea *)>(
          [](model::SpaceInfiltrationEffectiveLeakageArea *t_la)
        {
          t_la->resetSpaceType();
        }
          )
          ),
          DataSource(
          leakageAreas,
          false,
          QSharedPointer<DropZoneConcept>(new DropZoneConceptImpl<model::SpaceInfiltrationEffectiveLeakageArea, model::SpaceType>(Heading(SPACEINFILTRATIONEFFECTIVELEAKAGEAREAS),
          getter, setter))
          )
          );

      }
      else if (field == STANDARDSBUILDINGTYPE) {

        // nothing to do, it is a string already
        std::function<std::string(const std::string &)> toString = [](std::string t_s) { return t_s; };

        std::function<std::vector<std::string>(model::SpaceType *)> choices =
          [](model::SpaceType *t_spaceType){
          std::vector<std::string> retval;

          const auto &types = t_spaceType->suggestedStandardsBuildingTypes();

          retval.insert(retval.end(), types.begin(), types.end());
          return retval;
        };

        std::function<boost::optional<std::string>(model::SpaceType *)> getter =
          [](model::SpaceType *t_spaceType) {
          return t_spaceType->standardsBuildingType();
        };

        std::function<bool(model::SpaceType *, std::string)> setter =
          [](model::SpaceType *t_spaceType, std::string t_value) {
          t_spaceType->resetStandardsSpaceType();
          return t_spaceType->setStandardsBuildingType(t_value);
        };

        boost::optional<std::function<void(model::SpaceType *)>> resetter(
          [](model::SpaceType *t_spaceType) {
          t_spaceType->resetStandardsSpaceType();
          t_spaceType->resetStandardsBuildingType();
        });

        addComboBoxColumn(Heading(QString(STANDARDSBUILDINGTYPE)),
          toString,
          choices,
          getter,
          setter,
          resetter,
          boost::none,
          true);
      }
      else if (field == STANDARDSSPACETYPE) {

        // nothing to do, it is a string already
        std::function<std::string(const std::string &)> toString = [](std::string t_s) { return t_s; };

        std::function<std::vector<std::string>(model::SpaceType *)> choices =
          [](model::SpaceType *t_spaceType){
          std::vector<std::string> retval;

          const auto &types = t_spaceType->suggestedStandardsSpaceTypes();

          retval.insert(retval.end(), types.begin(), types.end());
          return retval;
        };

        std::function<boost::optional<std::string>(model::SpaceType *)> getter =
          [](model::SpaceType *t_spaceType) {
          return t_spaceType->standardsSpaceType();
        };

        std::function<bool(model::SpaceType *t_spaceType, std::string)> setter =
          [](model::SpaceType *t_spaceType, std::string t_val) {
          return t_spaceType->setStandardsSpaceType(t_val);
        };

        boost::optional<std::function<void(model::SpaceType *t_spaceType)>> resetter(
          [](model::SpaceType *t_spaceType) {
          t_spaceType->resetStandardsSpaceType();
        }
        );

        addComboBoxColumn(Heading(QString(STANDARDSSPACETYPE)),
          toString,
          choices,
          getter,
          setter,
          resetter,
          boost::none,
          true);
      }
      else {
        // unhandled
        OS_ASSERT(false);
      }
    }
  }

  QString SpaceTypesGridController::getColor(const model::ModelObject & modelObject)
  {
    QColor defaultColor(Qt::lightGray);
    QString color(defaultColor.name());

    // TODO: The code below is currently commented out because a refresh crash bug is precluding rack color
    // updates due to rack assignments to cases and walk-ins.  No colors are better than wrong colors.

    //std::vector<model::RefrigerationSystem> refrigerationSystems = m_model.getConcreteModelObjects<model::RefrigerationSystem>();

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
    if (!this->m_hasHorizontalHeader) return;

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
    std::vector<model::SpaceType> spaceTypes = m_model.getConcreteModelObjects<model::SpaceType>();
    m_modelObjects = subsetCastVector<model::ModelObject>(spaceTypes);
    std::sort(m_modelObjects.begin(), m_modelObjects.end(), ModelObjectNameSorter());
  }

  void SpaceTypesGridController::onComboBoxIndexChanged(int index)
  {
  }

} // openstudio
