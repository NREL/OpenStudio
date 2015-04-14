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

#include "FacilityShadingGridView.hpp"

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

// LOADS
#define LOADNAME "Load Name"


// MEASURE TAGS
#define STANDARDSBUILDINGTYPE "Standards Building Type\n(Optional)"


// LOAD TYPES
#define SHOWALLLOADS "Show all loads"
#define INTERNALMASS "Interal Mass"

namespace openstudio {

  struct ModelObjectNameSorter{
    // sort by name
    bool operator()(const model::ModelObject & lhs, const model::ModelObject & rhs){
      return (lhs.name() < rhs.name());
    }
  };

  FacilityShadingGridView::FacilityShadingGridView(bool isIP, const model::Model & model, QWidget * parent)
    : QWidget(parent),
    m_isIP(isIP)
  {
    auto mainLayout = new QVBoxLayout();
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(mainLayout);

    auto spaceTypes = model.getModelObjects<model::SpaceType>();
    auto spaceTypeModelObjects = subsetCastVector<model::ModelObject>(spaceTypes);

    m_gridController = new FacilityShadingGridController(m_isIP, "Space Types", IddObjectType::OS_SpaceType, model, spaceTypeModelObjects);
    auto gridView = new OSGridView(m_gridController, "Space Types", "Drop\nZone", false, parent);

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
    m_filters->setFixedWidth(OSItem::ITEM_WIDTH);

    {
      m_filters->addItem(SHOWALLLOADS);
      m_filters->addItem(INTERNALMASS);
    }

  {
    const QPixmap * pixMap = new QPixmap(":images/mini_icons/internal_mass.png");
    OS_ASSERT(pixMap);
    m_filters->addItem(*pixMap, INTERNALMASS);
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
  isConnected = connect(m_filters, &QComboBox::currentTextChanged, m_gridController, &openstudio::FacilityShadingGridController::filterChanged);
  OS_ASSERT(isConnected);

  isConnected = connect(gridView, SIGNAL(dropZoneItemClicked(OSItem*)), this, SIGNAL(dropZoneItemClicked(OSItem*)));
  OS_ASSERT(isConnected);

  isConnected = connect(this, SIGNAL(itemSelected(OSItem *)), gridView, SIGNAL(itemSelected(OSItem *)));
  OS_ASSERT(isConnected);

  isConnected = connect(this, SIGNAL(selectionCleared()), gridView, SLOT(onSelectionCleared()));
  OS_ASSERT(isConnected);

  isConnected = connect(gridView, SIGNAL(gridRowSelected(OSItem*)), this, SIGNAL(gridRowSelected(OSItem*)));
  OS_ASSERT(isConnected);

  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_gridController, SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_gridController, SLOT(toggleUnits(bool)));
  OS_ASSERT(isConnected);

  std::vector<model::SpaceType> spaceType = model.getModelObjects<model::SpaceType>(); // NOTE for horizontal system lists
  }

  std::vector<model::ModelObject> FacilityShadingGridView::selectedObjects() const
  {
    return m_gridController->selectedObjects();
  }

  void FacilityShadingGridView::enableFilter()
  {
    m_filterLabel->setEnabled(true);
    m_filters->setEnabled(true);
  }

  void FacilityShadingGridView::disableFilter()
  {
    m_filterLabel->setEnabled(false);
    m_filters->setEnabled(false);
  }

  void FacilityShadingGridView::onDropZoneItemClicked(OSItem* item)
  {
  }

  FacilityShadingGridController::FacilityShadingGridController(bool isIP,
    const QString & headerText,
    IddObjectType iddObjectType,
    model::Model model,
    std::vector<model::ModelObject> modelObjects) :
    OSGridController(isIP, headerText, iddObjectType, model, modelObjects)
  {
    setCategoriesAndFields();
  }

  void FacilityShadingGridController::setCategoriesAndFields()
  {

    //{
    //  std::vector<QString> fields;
    //  fields.push_back(RENDERINGCOLOR);
    //  std::pair<QString, std::vector<QString> > categoryAndFields = std::make_pair(QString("General"), fields);
    //  m_categoriesAndFields.push_back(categoryAndFields);
    //}

    //{
    //  std::vector<QString> fields;
    //  fields.push_back(LOADNAME);
    //  std::pair<QString, std::vector<QString> > categoryAndFields = std::make_pair(QString("Loads"), fields);
    //  m_categoriesAndFields.push_back(categoryAndFields);
    //}

    //{
    //  std::vector<QString> fields;
    //  fields.push_back(STANDARDSBUILDINGTYPE);
    //  std::pair<QString, std::vector<QString> > categoryAndFields = std::make_pair(QString("Measure\nTags"), fields);
    //  m_categoriesAndFields.push_back(categoryAndFields);
    //}

    OSGridController::setCategoriesAndFields();

  }

  void FacilityShadingGridController::filterChanged(const QString & text)
  {
    LOG(Debug, "Load filter changed: " << text);

    auto objectSelector = getObjectSelector();
    if (text == SHOWALLLOADS)
    {
      objectSelector->resetObjectFilter();
    }
    else {
      objectSelector->setObjectFilter(
        [text](const model::ModelObject &obj)->bool {
        try {
          obj.cast<model::SpaceLoadInstance>();
          // This is a spaceloadinstance, so we want to see if it matches our filter

          if (text == INTERNALMASS)
          {
            return static_cast<bool>(obj.optionalCast<model::InternalMass>());
          }
          //else if (text == PEOPLE)
          //{
          //  return static_cast<bool>(obj.optionalCast<model::People>());
          //}
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

  FacilityShadingGridView * FacilityShadingGridController::gridView()
  {
    OS_ASSERT(qobject_cast<OSGridView *>(this->parent()));

    return qobject_cast<FacilityShadingGridView *>(this->parent()->parent());
  }

  void FacilityShadingGridController::categorySelected(int index)
  {
    auto gv = this->gridView();
    if (gv) {
      if (gv->m_filters) {
        gv->m_filters->setCurrentIndex(0);
      }
    }

    if (gridView()) {
      // Enable filter when viewing loads tab...
      if (index == 1) {
        gridView()->enableFilter();
      }
      // ...or if the custom tab has a loads column
      else if (index == 3) {
        auto categoriesAndFields = this->categoriesAndFields();
        auto fields = categoriesAndFields.at(index);
        for (auto field : fields.second) {
          if (field == LOADNAME) {
            gridView()->enableFilter();
            break;
          }
        }
      }
      else {
        gridView()->disableFilter();
      }
    }

    OSGridController::categorySelected(index);
  }

  void FacilityShadingGridController::addColumns(const QString &category, std::vector<QString> & fields)
  {
    // always show name and selected columns
    fields.insert(fields.begin(), { NAME, SELECTED });

    m_baseConcepts.clear();

    for (const auto &field : fields) {

      if (field == NAME) {
        auto getter = CastNullAdapter<model::SpaceType>(&model::SpaceType::name);
        auto setter = CastNullAdapter<model::SpaceType>(&model::SpaceType::setName);

        addNameLineEditColumn(Heading(QString(NAME), false, false),
          false,
          false,
          getter,
          setter);

      }
      else if (field == SELECTED && category != "Loads") {
        auto checkbox = QSharedPointer<QCheckBox>(new QCheckBox());
        checkbox->setToolTip("Check to select all rows");
        connect(checkbox.data(), &QCheckBox::stateChanged, this, &FacilityShadingGridController::selectAllStateChanged);

        addSelectColumn(Heading(QString(SELECTED), false, false, checkbox), "Check to select this row");
      }
      else {
        // unhandled
        OS_ASSERT(false);
      }
    }
  }

  QString FacilityShadingGridController::getColor(const model::ModelObject & modelObject)
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

  void FacilityShadingGridController::checkSelectedFields()
  {
    if (!this->m_hasHorizontalHeader) return;

    OSGridController::checkSelectedFields();
  }

  void FacilityShadingGridController::onItemDropped(const OSItemId& itemId)
  {
    boost::optional<model::ModelObject> modelObject = OSAppBase::instance()->currentDocument()->getModelObject(itemId);
    if (modelObject){
      if (modelObject->optionalCast<model::SpaceType>()){
        modelObject->clone(m_model);
        emit modelReset();
      }
    }
  }

  void FacilityShadingGridController::refreshModelObjects()
  {
    std::vector<model::SpaceType> spaceTypes = m_model.getModelObjects<model::SpaceType>();
    m_modelObjects = subsetCastVector<model::ModelObject>(spaceTypes);
    std::sort(m_modelObjects.begin(), m_modelObjects.end(), ModelObjectNameSorter());
  }

  void FacilityShadingGridController::onComboBoxIndexChanged(int index)
  {
  }

} // openstudio
