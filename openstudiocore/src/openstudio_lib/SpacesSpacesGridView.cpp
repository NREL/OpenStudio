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

#include "SpacesSpacesGridView.hpp"

#include "OSDropZone.hpp"
#include "OSItemSelectorButtons.hpp"

#include "../shared_gui_components/OSGridView.hpp"

#include "../model/BuildingStory.hpp"
#include "../model/BuildingStory_Impl.hpp"
#include "../model/DefaultConstructionSet.hpp"
#include "../model/DefaultConstructionSet_Impl.hpp"
#include "../model/DefaultScheduleSet.hpp"
#include "../model/DefaultScheduleSet_Impl.hpp"
#include "../model/DesignSpecificationOutdoorAir.hpp"
#include "../model/DesignSpecificationOutdoorAir_Impl.hpp"
#include "../model/Space.hpp"
#include "../model/Space_Impl.hpp"
#include "../model/SpaceInfiltrationDesignFlowRate.hpp"
#include "../model/SpaceInfiltrationDesignFlowRate_Impl.hpp"
#include "../model/SpaceInfiltrationEffectiveLeakageArea.hpp"
#include "../model/SpaceInfiltrationEffectiveLeakageArea_Impl.hpp"
#include "../model/SpaceType.hpp"
#include "../model/SpaceType_Impl.hpp"
#include "../model/ThermalZone.hpp"
#include "../model/ThermalZone_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/idd/IddEnums.hxx"
#include "../utilities/idd/OS_Space_FieldEnums.hxx"

#include <QCheckBox>

// These defines provide a common area for field display names
// used on column headers, and other grid widgets

#define NAME "Space Name"
#define SELECTED "All"

// GENERAL
#define STORY "Story"
#define THERMALZONE "Thermal Zone"
#define SPACETYPE "Space Type"
#define DEFAULTCONSTRUCTIONSET "Default Construction Set"
#define DEFAULTSCHEDULESET "Default Schedule Set"
#define PARTOFTOTALFLOORAREA "Part of Total Floor Area"

// AIRFLOW
#define SPACEINFILTRATIONDESIGNFLOWRATES "Space Infiltration Design Flow Rates"
#define SPACEINFILTRATIONEFFECTIVELEAKAGEAREAS "Space Infiltration Effective Leakage Areas"
#define DESIGNSPECIFICATIONOUTDOORAIROBJECTNAME "Design Specification Outdoor Air Object Name"

namespace openstudio {

  struct ModelObjectNameSorter{
    // sort by name
    bool operator()(const model::ModelObject & lhs, const model::ModelObject & rhs){
      return (lhs.name() < rhs.name());
    }
  };

  SpacesSpacesGridView::SpacesSpacesGridView(bool isIP, const model::Model & model, QWidget * parent)
    : SpacesSubtabGridView(isIP, model, parent)
  {
    showStoryFilter();
    showThermalZoneFilter();
    showSpaceTypeFilter();

    m_filterGridLayout->setRowStretch(m_filterGridLayout->rowCount(), 100);
    m_filterGridLayout->setColumnStretch(m_filterGridLayout->columnCount(), 100);

    m_gridController = new SpacesSpacesGridController(isIP, "Space", IddObjectType::OS_Space, model, m_spacesModelObjects);
    m_gridView = new OSGridView(m_gridController, "Space", "Drop\nSpace", false, parent);

    setGridController(m_gridController);
    setGridView(m_gridView);

    m_gridView->m_contentLayout->addLayout(m_filterGridLayout);
    m_gridView->m_contentLayout->addSpacing(7);
    m_gridView->m_dropZone->hide();

    onClearSelection();
  }

  void SpacesSpacesGridView::onSelectItem()
  {
    //m_itemSelectorButtons->enableAddButton();
    //m_itemSelectorButtons->enableCopyButton();
    m_itemSelectorButtons->enableRemoveButton();
    // m_itemSelectorButtons->enablePurgeButton();
  }

  void SpacesSpacesGridView::onClearSelection()
  {
    //m_itemSelectorButtons->disableAddButton();
    //m_itemSelectorButtons->disableCopyButton();
    m_itemSelectorButtons->disableRemoveButton();
    // m_itemSelectorButtons->disablePurgeButton();
  }

  SpacesSpacesGridController::SpacesSpacesGridController(bool isIP,
    const QString & headerText,
    IddObjectType iddObjectType,
    model::Model model,
    std::vector<model::ModelObject> modelObjects) :
    OSGridController(isIP, headerText, iddObjectType, model, modelObjects)
  {
    setCategoriesAndFields();
  }

  void SpacesSpacesGridController::setCategoriesAndFields()
  {
    {
      std::vector<QString> fields;
      fields.push_back(STORY);
      fields.push_back(THERMALZONE);
      fields.push_back(SPACETYPE);
      fields.push_back(DEFAULTCONSTRUCTIONSET);
      fields.push_back(DEFAULTSCHEDULESET);
      fields.push_back(PARTOFTOTALFLOORAREA);
      std::pair<QString, std::vector<QString> > categoryAndFields = std::make_pair(QString("General"), fields);
      m_categoriesAndFields.push_back(categoryAndFields);
    }

    {
      std::vector<QString> fields;
      fields.push_back(DESIGNSPECIFICATIONOUTDOORAIROBJECTNAME);
      fields.push_back(SPACEINFILTRATIONDESIGNFLOWRATES);
      fields.push_back(SPACEINFILTRATIONEFFECTIVELEAKAGEAREAS);
      std::pair<QString, std::vector<QString> > categoryAndFields = std::make_pair(QString("Airflow"), fields);
      m_categoriesAndFields.push_back(categoryAndFields);
    }

    OSGridController::setCategoriesAndFields();
  }

  void SpacesSpacesGridController::categorySelected(int index)
  {
    OSGridController::categorySelected(index);
  }

  void SpacesSpacesGridController::addColumns(const QString &category, std::vector<QString> & fields)
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
      else if (field == SELECTED) {
        auto checkbox = QSharedPointer<QCheckBox>(new QCheckBox());
        checkbox->setToolTip("Check to select all rows");
        connect(checkbox.data(), &QCheckBox::stateChanged, this, &SpacesSpacesGridController::selectAllStateChanged);
        connect(checkbox.data(), &QCheckBox::stateChanged, this->gridView(), &OSGridView::gridRowSelectionChanged);

        addSelectColumn(Heading(QString(SELECTED), false, false, checkbox), "Check to select this row");
      }
      else if (field == STORY) {
        addDropZoneColumn(Heading(QString(STORY)),
          CastNullAdapter<model::Space>(&model::Space::buildingStory),
          CastNullAdapter<model::Space>(&model::Space::setBuildingStory),
          boost::optional<std::function<void(model::Space*)>>(CastNullAdapter<model::Space>(&model::Space::resetBuildingStory))
          );
      }
      else if (field == THERMALZONE) {
        std::function<bool(model::Space*, const model::ThermalZone&)> set(
          [](model::Space* s, const model::ThermalZone& tz) {
          model::ThermalZone copy = tz;
          return s->setThermalZone(copy);
        }
        );

        addDropZoneColumn(Heading(QString(THERMALZONE)),
          CastNullAdapter<model::Space>(&model::Space::thermalZone),
          set,
          boost::optional<std::function<void(model::Space*)>>(CastNullAdapter<model::Space>(&model::Space::resetThermalZone))
          );
      }
      else if (field == SPACETYPE) {
        addDropZoneColumn(Heading(QString(SPACETYPE)),
          CastNullAdapter<model::Space>(&model::Space::spaceType),
          CastNullAdapter<model::Space>(&model::Space::setSpaceType),
          boost::optional<std::function<void(model::Space*)>>(CastNullAdapter<model::Space>(&model::Space::resetSpaceType)),
          boost::optional<std::function<bool(model::Space*)>>(CastNullAdapter<model::Space>(&model::Space::isSpaceTypeDefaulted)),
          boost::optional<DataSource>()
          );
      }
      else if (field == DEFAULTCONSTRUCTIONSET) {
        addDropZoneColumn(Heading(QString(DEFAULTCONSTRUCTIONSET)),
          CastNullAdapter<model::Space>(&model::Space::defaultConstructionSet),
          CastNullAdapter<model::Space>(&model::Space::setDefaultConstructionSet),
          boost::optional<std::function<void(model::Space*)>>(CastNullAdapter<model::Space>(&model::Space::resetDefaultConstructionSet))
          );
      }
      else if (field == DEFAULTSCHEDULESET) {
        addDropZoneColumn(Heading(QString(DEFAULTSCHEDULESET)),
          CastNullAdapter<model::Space>(&model::Space::defaultScheduleSet),
          CastNullAdapter<model::Space>(&model::Space::setDefaultScheduleSet),
          boost::optional<std::function<void(model::Space*)>>(CastNullAdapter<model::Space>(&model::Space::resetDefaultScheduleSet))
          );
      }
      else if (field == PARTOFTOTALFLOORAREA) {
        addCheckBoxColumn(Heading(QString(PARTOFTOTALFLOORAREA), true, false),
          std::string("Check to enable part of total floor area."),
          NullAdapter(&model::Space::partofTotalFloorArea),
          NullAdapter(&model::Space::setPartofTotalFloorArea)
          );
      }
      else if (field == SPACEINFILTRATIONDESIGNFLOWRATES) {
        std::function<boost::optional<model::SpaceInfiltrationDesignFlowRate>(model::Space *)>  getter;

        std::function<bool(model::Space *, const model::SpaceInfiltrationDesignFlowRate &)> setter(
          [](model::Space *t_space, model::SpaceInfiltrationDesignFlowRate t_rate) {
          if (t_rate.space()) {
            boost::optional<model::ModelObject> clone_rate = t_rate.clone(t_rate.model());
            OS_ASSERT(clone_rate);
            auto success = clone_rate->cast<model::SpaceInfiltrationDesignFlowRate>().setSpace(*t_space);
            OS_ASSERT(success);
            return success;
          }
          else {
            auto success = t_rate.setSpace(*t_space);
            OS_ASSERT(success);
            return success;
          }
        }
        );

        std::function<std::vector<model::ModelObject>(const model::Space &)> flowRates(
          [](const model::Space &s) {
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
          t_fr->resetSpace();
        }
          )
          ),
          DataSource(
          flowRates,
          false,
          QSharedPointer<DropZoneConcept>(new DropZoneConceptImpl<model::SpaceInfiltrationDesignFlowRate, model::Space>(Heading(SPACEINFILTRATIONDESIGNFLOWRATES),
          getter, setter))
          )
          );
      }
      else if (field == SPACEINFILTRATIONEFFECTIVELEAKAGEAREAS) {
        std::function<boost::optional<model::SpaceInfiltrationEffectiveLeakageArea>(model::Space *)>  getter;

        std::function<bool(model::Space *, const model::SpaceInfiltrationEffectiveLeakageArea &)> setter(
          [](model::Space *t_space, model::SpaceInfiltrationEffectiveLeakageArea t_area) {
          if (t_area.space()) {
            boost::optional<model::ModelObject> clone_area = t_area.clone(t_area.model());
            OS_ASSERT(clone_area);
            auto success = clone_area->cast<model::SpaceInfiltrationEffectiveLeakageArea>().setSpace(*t_space);
            OS_ASSERT(success);
            return success;
          }
          else {
            auto success = t_area.setSpace(*t_space);
            OS_ASSERT(success);
            return success;
          }
        }
        );

        std::function<std::vector<model::ModelObject>(const model::Space &)> leakageAreas(
          [](const model::Space &s) {
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
          t_la->resetSpace();
        }
          )
          ),
          DataSource(
          leakageAreas,
          false,
          QSharedPointer<DropZoneConcept>(new DropZoneConceptImpl<model::SpaceInfiltrationEffectiveLeakageArea, model::Space>(Heading(SPACEINFILTRATIONEFFECTIVELEAKAGEAREAS),
          getter, setter))
          )
          );
      }
      else if (field == DESIGNSPECIFICATIONOUTDOORAIROBJECTNAME) {
        addDropZoneColumn(Heading(QString(DESIGNSPECIFICATIONOUTDOORAIROBJECTNAME)),
          CastNullAdapter<model::Space>(&model::Space::designSpecificationOutdoorAir),
          CastNullAdapter<model::Space>(&model::Space::setDesignSpecificationOutdoorAir),
          boost::optional<std::function<void(model::Space*)>>(CastNullAdapter<model::Space>(&model::Space::resetDesignSpecificationOutdoorAir)),
          boost::optional<std::function<bool(model::Space*)>>(CastNullAdapter<model::Space>(&model::Space::isDesignSpecificationOutdoorAirDefaulted)),
          boost::optional<DataSource>()
          );
      }
      else {
        // unhandled
        OS_ASSERT(false);
      }
    }
  }

  QString SpacesSpacesGridController::getColor(const model::ModelObject & modelObject)
  {
    QColor defaultColor(Qt::lightGray);
    return defaultColor.name();
  }

  void SpacesSpacesGridController::checkSelectedFields()
  {
    if (!this->m_hasHorizontalHeader) return;

    OSGridController::checkSelectedFields();
  }

  void SpacesSpacesGridController::onItemDropped(const OSItemId& itemId)
  {
  }

  void SpacesSpacesGridController::refreshModelObjects()
  {
    auto spaces = m_model.getModelObjects<model::Space>();
    m_modelObjects = subsetCastVector<model::ModelObject>(spaces);
    std::sort(m_modelObjects.begin(), m_modelObjects.end(), ModelObjectNameSorter());
  }

} // openstudio
