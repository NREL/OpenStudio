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

#include "FacilityStoriesGridView.hpp"

#include "OSItemSelectorButtons.hpp"

#include "../shared_gui_components/OSGridView.hpp"

#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/ModelObject.hpp"
#include "../model/ModelObject_Impl.hpp"

#include "OSDropZone.hpp"
#include "RenderingColorWidget.hpp"

#include "../model/BuildingStory.hpp"
#include "../model/BuildingStory_Impl.hpp"
#include "../model/DefaultConstructionSet.hpp"
#include "../model/DefaultConstructionSet_Impl.hpp"
#include "../model/DefaultScheduleSet.hpp"
#include "../model/DefaultScheduleSet_Impl.hpp"
#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/RenderingColor.hpp"
#include "../model/Space.hpp"
#include "../model/Space_Impl.hpp"

#include <utilities/idd/OS_BuildingStory_FieldEnums.hxx>
#include <utilities/idd/OS_Space_FieldEnums.hxx>


#include "../utilities/core/Assert.hpp"
#include "../utilities/idd/IddEnums.hxx"
#include "../utilities/idd/OS_Exterior_Lights_FieldEnums.hxx"

#include <QBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QScrollArea>

// These defines provide a common area for field display names
// used on column headers, and other grid widgets

#define NAME "Story Name"
#define SELECTED "All"

// GENERAL
#define RENDERINGCOLOR "Rendering Color"
#define DEFAULTCONSTRUCTIONSET "Default Construction Set"
#define DEFAULTSCHEDULESET "Default Schedule Set"

namespace openstudio {

  struct ModelObjectNameSorter{
    // sort by name
    bool operator()(const model::ModelObject & lhs, const model::ModelObject & rhs){
      return (lhs.name() < rhs.name());
    }
  };

  FacilityStoriesGridView::FacilityStoriesGridView(bool isIP, const model::Model & model, QWidget * parent)
    : GridViewSubTab(isIP, model, parent)
  {
    auto buildingStories = model.getModelObjects<model::BuildingStory>();
    auto buildingStoriesModelObjects = subsetCastVector<model::ModelObject>(buildingStories);

    m_gridController = new FacilityStoriesGridController(isIP, "Building Stories", IddObjectType::OS_BuildingStory, model, buildingStoriesModelObjects);
    auto gridView = new OSGridView(m_gridController, "Building Stories", "Drop\nStory", false, parent);

    setGridController(m_gridController);
    setGridView(gridView);

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
    //isConnected = connect(m_filters, &QComboBox::currentTextChanged, m_gridController, &openstudio::FacilityStoriesGridController::filterChanged);
    //OS_ASSERT(isConnected);

    m_filters->setFixedWidth(OSItem::ITEM_WIDTH);

    {
      m_filters->addItem("Filter 1");
      m_filters->addItem("Filter 2");
    }

    {
      const QPixmap * pixMap = new QPixmap(":images/mini_icons/internal_mass.png");
      OS_ASSERT(pixMap);
      m_filters->addItem(*pixMap, "My Filter");
    }

    //disableFilter();
    layout->addWidget(m_filters, Qt::AlignTop | Qt::AlignLeft);

    layout->addStretch();

    filterGridLayout->addLayout(layout, filterGridLayout->rowCount() - 1, 1);

    filterGridLayout->setRowStretch(filterGridLayout->rowCount(), 100);
    filterGridLayout->setColumnStretch(filterGridLayout->columnCount(), 100);

    gridView->m_contentLayout->addLayout(filterGridLayout);

    gridView->m_contentLayout->addSpacing(7);
  }

  void FacilityStoriesGridView::onDropZoneItemClicked(OSItem* item)
  {
  }

  void FacilityStoriesGridView::addObject(const IddObjectType& iddObjectType)
  {
    if (IddObjectType::OS_BuildingStory == iddObjectType.value()) {
      model::BuildingStory(this->m_model);
    }
  }

  void FacilityStoriesGridView::purgeObjects(const IddObjectType& iddObjectType)
  {
    for (auto mo : this->m_model.getConcreteModelObjects<model::BuildingStory>()){
      mo.remove();
    }
  }

  FacilityStoriesGridController::FacilityStoriesGridController(bool isIP,
    const QString & headerText,
    IddObjectType iddObjectType,
    model::Model model,
    std::vector<model::ModelObject> modelObjects) :
    OSGridController(isIP, headerText, iddObjectType, model, modelObjects)
  {
    setCategoriesAndFields();
  }

  void FacilityStoriesGridController::setCategoriesAndFields()
  {
    {
      std::vector<QString> fields;
      //fields.push_back(RENDERINGCOLOR);
      fields.push_back(DEFAULTCONSTRUCTIONSET);
      fields.push_back(DEFAULTSCHEDULESET);
      std::pair<QString, std::vector<QString> > categoryAndFields = std::make_pair(QString("General"), fields);
      m_categoriesAndFields.push_back(categoryAndFields);
    }

    OSGridController::setCategoriesAndFields();
  }

  void FacilityStoriesGridController::filterChanged(const QString & text)
  {
    LOG(Debug, "Load filter changed: " << text);

    auto objectSelector = getObjectSelector();
    //if (text == SHOWALLLOADS)
    //{
    //  objectSelector->resetObjectFilter();
    //}
    //else {
    //  objectSelector->setObjectFilter(
    //    [text](const model::ModelObject &obj)->bool {
    //    try {
    //      obj.cast<model::SpaceLoadInstance>();
    //      // This is a spaceloadinstance, so we want to see if it matches our filter

    //      if (text == "")
    //      {
    //        return static_cast<bool>(obj.optionalCast<model::InternalMass>());
    //      }
    //      //else if (text == PEOPLE)
    //      //{
    //      //  return static_cast<bool>(obj.optionalCast<model::People>());
    //      //}
    //      else
    //      {
    //        // Should never get here
    //        OS_ASSERT(false);
    //        return false;
    //      }
    //    }
    //    catch (...) {
    //      return true; // this isn't a space load instance, so don't apply filtering
    //    }
    //  }
    //  );
    //}
  }

  FacilityStoriesGridView * FacilityStoriesGridController::gridView()
  {
    OS_ASSERT(qobject_cast<OSGridView *>(this->parent()));

    return qobject_cast<FacilityStoriesGridView *>(this->parent()->parent());
  }

  void FacilityStoriesGridController::categorySelected(int index)
  {
    OSGridController::categorySelected(index);
  }

  void FacilityStoriesGridController::addColumns(const QString &category, std::vector<QString> & fields)
  {
    // always show name and selected columns
    fields.insert(fields.begin(), { NAME, SELECTED });

    m_baseConcepts.clear();

    for (const auto &field : fields) {

      if (field == NAME) {
        addNameLineEditColumn(Heading(QString(NAME), false, false),
          false,
          false,
          CastNullAdapter<model::BuildingStory>(&model::BuildingStory::name),
          CastNullAdapter<model::BuildingStory>(&model::BuildingStory::setName)
          );
      }
      else if (field == SELECTED) {
        auto checkbox = QSharedPointer<QCheckBox>(new QCheckBox());
        checkbox->setToolTip("Check to select all rows");
        connect(checkbox.data(), &QCheckBox::stateChanged, this, &FacilityStoriesGridController::selectAllStateChanged);

        addSelectColumn(Heading(QString(SELECTED), false, false, checkbox), "Check to select this row");
      }
      else if (field == RENDERINGCOLOR) {
        //addRenderingColorColumn(Heading(QString(RENDERINGCOLOR), true, false),
        //  CastNullAdapter<model::BuildingStory>(&model::BuildingStory::renderingColor),
        //  CastNullAdapter<model::BuildingStory>(&model::BuildingStory::setRenderingColor)
        //  );
      }
      else if (field == DEFAULTCONSTRUCTIONSET) {
        addDropZoneColumn(Heading(QString(DEFAULTCONSTRUCTIONSET)),
          CastNullAdapter<model::BuildingStory>(&model::BuildingStory::defaultConstructionSet),
          CastNullAdapter<model::BuildingStory>(&model::BuildingStory::setDefaultConstructionSet),
          boost::optional<std::function<void(model::BuildingStory*)>>(CastNullAdapter<model::BuildingStory>(&model::BuildingStory::resetDefaultConstructionSet))
          );
      }
      else if (field == DEFAULTSCHEDULESET) {
        addDropZoneColumn(Heading(QString(DEFAULTSCHEDULESET)),
          CastNullAdapter<model::BuildingStory>(&model::BuildingStory::defaultScheduleSet),
          CastNullAdapter<model::BuildingStory>(&model::BuildingStory::setDefaultScheduleSet),
          boost::optional<std::function<void(model::BuildingStory*)>>(CastNullAdapter<model::BuildingStory>(&model::BuildingStory::resetDefaultScheduleSet))
          );
      }
      else {
        // unhandled
        OS_ASSERT(false);
      }
    }
  }

  QString FacilityStoriesGridController::getColor(const model::ModelObject & modelObject)
  {
    QColor defaultColor(Qt::lightGray);
    return defaultColor.name();
  }

  void FacilityStoriesGridController::checkSelectedFields()
  {
    if (!this->m_hasHorizontalHeader) return;

    OSGridController::checkSelectedFields();
  }

  void FacilityStoriesGridController::onItemDropped(const OSItemId& itemId)
  {
  }

  void FacilityStoriesGridController::refreshModelObjects()
  {
    auto exteriorLights = m_model.getModelObjects<model::BuildingStory>();
    m_modelObjects = subsetCastVector<model::ModelObject>(exteriorLights);
    std::sort(m_modelObjects.begin(), m_modelObjects.end(), ModelObjectNameSorter());
  }

  void FacilityStoriesGridController::onComboBoxIndexChanged(int index)
  {
  }

} // openstudio
