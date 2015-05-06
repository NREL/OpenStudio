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

#include "SpacesSubsurfacesGridView.hpp"

//#include "OSDropZone.hpp"
//#include "OSItemSelectorButtons.hpp"

#include "../shared_gui_components/OSGridView.hpp"

#include "../model/ShadingSurface.hpp"
#include "../model/ShadingSurface_Impl.hpp"
//#include "../model/Schedule.hpp"
//#include "../model/Schedule_Impl.hpp"
#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/ModelObject.hpp"
#include "../model/ModelObject_Impl.hpp"
//#include "../model/ShadingSurfaceGroup.hpp"
//#include "../model/ShadingSurfaceGroup_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/idd/IddEnums.hxx"
#include "../utilities/idd/OS_ShadingSurface_FieldEnums.hxx"

#include <QBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QRegExpValidator>

// These defines provide a common area for field display names
// used on column headers, and other grid widgets

#define NAME "Shading Surface Name"
#define SELECTED "All"

// GENERAL
#define SHADINGSURFACEGROUPNAME "Shading Surface Group Name" // (read only)
#define CONSTRUCTIONNAME "Construction Name"
#define TRANSMITTANCESCHEDULENAME "Transmittance Schedule Name"

// FILTERS
#define STORY "Story"
#define THERMALZONE "Thermal Zone"
#define SPACETYPE "Space Type"
#define SPACENAME "Space Name"

namespace openstudio {

  struct ModelObjectNameSorter{
    // sort by name
    bool operator()(const model::ModelObject & lhs, const model::ModelObject & rhs){
      return (lhs.name() < rhs.name());
    }
  };

  SpacesSubsurfacesGridView::SpacesSubsurfacesGridView(bool isIP, const model::Model & model, QWidget * parent)
    : GridViewSubTab(isIP, model, parent)
  {
    auto buildingShading = model.getModelObjects<model::ShadingSurface>();
    auto buildingShadingModelObjects = subsetCastVector<model::ModelObject>(buildingShading);

    m_gridController = new SpacesSubsurfacesGridController(isIP, "Shading Surface", IddObjectType::OS_ShadingSurface, model, buildingShadingModelObjects);
    m_gridView = new OSGridView(m_gridController, "Shading Surface", "Drop\nShading Surface", false, parent);

    setGridController(m_gridController);
    setGridView(m_gridView);

    // Filters

    QLabel * label = nullptr;

    QVBoxLayout * layout = nullptr;

    auto filterGridLayout = new QGridLayout();
    filterGridLayout->setContentsMargins(7, 4, 0, 8);
    filterGridLayout->setSpacing(5);

    label = new QLabel();
    label->setText("Filters:");
    label->setObjectName("H2");
    filterGridLayout->addWidget(label, filterGridLayout->rowCount(), filterGridLayout->columnCount(), Qt::AlignTop | Qt::AlignLeft);

    // STORY

    layout = new QVBoxLayout();

    label = new QLabel();
    label->setText(STORY);
    label->setObjectName("H3");
    layout->addWidget(label, Qt::AlignTop | Qt::AlignLeft);

    m_storyFilter = new QComboBox();
    m_storyFilter->addItem("Building");
    m_storyFilter->addItem("Site");
    m_storyFilter->setFixedWidth(OSItem::ITEM_WIDTH);
    connect(m_storyFilter, &QComboBox::currentTextChanged, this, &openstudio::SpacesSubsurfacesGridView::storyFilterChanged);

    layout->addWidget(m_storyFilter, Qt::AlignTop | Qt::AlignLeft);
    layout->addStretch();
    filterGridLayout->addLayout(layout, filterGridLayout->rowCount() - 1, filterGridLayout->columnCount());

    // THERMALZONE

    layout = new QVBoxLayout();

    label = new QLabel();
    label->setText(THERMALZONE);
    label->setObjectName("H3");
    layout->addWidget(label, Qt::AlignTop | Qt::AlignLeft);

    m_thermalZoneFilter = new QComboBox();
    m_thermalZoneFilter->setFixedWidth(OSItem::ITEM_WIDTH);
    connect(m_thermalZoneFilter, &QComboBox::currentTextChanged, this, &openstudio::SpacesSubsurfacesGridView::thermalZoneFilterChanged);

    QRegExp nameRegex("^\\S.*");
    auto nameValidator = new QRegExpValidator(nameRegex, this);
    m_thermalZoneFilter->setValidator(nameValidator);

    layout->addWidget(m_thermalZoneFilter, Qt::AlignTop | Qt::AlignLeft);
    layout->addStretch();
    filterGridLayout->addLayout(layout, filterGridLayout->rowCount() - 1, filterGridLayout->columnCount());

    // SPACETYPE

    layout = new QVBoxLayout();

    label = new QLabel();
    label->setText(SPACETYPE);
    label->setObjectName("H3");
    layout->addWidget(label, Qt::AlignTop | Qt::AlignLeft);

    m_spaceTypeFilter = new QComboBox();
    m_spaceTypeFilter->addItem("Horizontal");
    m_spaceTypeFilter->addItem("Vertical");
    m_spaceTypeFilter->setFixedWidth(OSItem::ITEM_WIDTH);
    connect(m_spaceTypeFilter, &QComboBox::currentTextChanged, this, &openstudio::SpacesSubsurfacesGridView::thermalZoneFilterChanged);

    layout->addWidget(m_spaceTypeFilter, Qt::AlignTop | Qt::AlignLeft);
    layout->addStretch();
    filterGridLayout->addLayout(layout, filterGridLayout->rowCount() - 1, filterGridLayout->columnCount());

    // SPACENAME

    layout = new QVBoxLayout();

    label = new QLabel();
    label->setText(SPACENAME);
    label->setObjectName("H3");
    layout->addWidget(label, Qt::AlignTop | Qt::AlignLeft);

    m_nameFilter = new QLineEdit();
    m_nameFilter->setFixedWidth(OSItem::ITEM_WIDTH);
    connect(m_nameFilter, &QLineEdit::editingFinished, this, &openstudio::SpacesSubsurfacesGridView::nameFilterChanged);

    nameValidator = new QRegExpValidator(nameRegex, this);
    m_nameFilter->setValidator(nameValidator);

    layout->addWidget(m_nameFilter, Qt::AlignTop | Qt::AlignLeft);
    layout->addStretch();
    filterGridLayout->addLayout(layout, filterGridLayout->rowCount() - 1, filterGridLayout->columnCount());

    filterGridLayout->setRowStretch(filterGridLayout->rowCount(), 100);
    filterGridLayout->setColumnStretch(filterGridLayout->columnCount(), 100);

    m_gridView->m_contentLayout->addLayout(filterGridLayout);

    m_gridView->m_contentLayout->addSpacing(7);
  }

  void SpacesSubsurfacesGridView::storyFilterChanged(const QString& text)
  {
    filterChanged();
  }

  void SpacesSubsurfacesGridView::thermalZoneFilterChanged(const QString& text)
  {
    filterChanged();
  }

  void SpacesSubsurfacesGridView::spaceTypeFilterChanged(const QString& text)
  {
    filterChanged();
  }
  
  void SpacesSubsurfacesGridView::nameFilterChanged()
  {
    filterChanged();
  }

  void SpacesSubsurfacesGridView::filterChanged()
  {
    auto objectSelector = this->m_gridController->getObjectSelector();

    auto tilt = m_storyFilter->currentText();

    auto type = m_thermalZoneFilter->currentText();

    auto orintation = m_spaceTypeFilter->currentText();

    for (auto obj : objectSelector->m_selectorObjects) {

    }

    this->m_gridView->requestRefreshAll();
  }

  void SpacesSubsurfacesGridView::onDropZoneItemClicked(OSItem* item)
  {
  }

  void SpacesSubsurfacesGridView::addObject(const IddObjectType& iddObjectType)
  {
    if (IddObjectType::OS_ShadingSurface == iddObjectType.value()) {
      //model::ShadingSurface();
    }
  }

  void SpacesSubsurfacesGridView::purgeObjects(const IddObjectType& iddObjectType)
  {
    for (auto mo : this->m_model.getConcreteModelObjects<model::ShadingSurface>()){
      mo.remove();
    }
  }

  SpacesSubsurfacesGridController::SpacesSubsurfacesGridController(bool isIP,
    const QString & headerText,
    IddObjectType iddObjectType,
    model::Model model,
    std::vector<model::ModelObject> modelObjects) :
    OSGridController(isIP, headerText, iddObjectType, model, modelObjects)
  {
    setCategoriesAndFields();
  }

  void SpacesSubsurfacesGridController::setCategoriesAndFields()
  {
    {
      std::vector<QString> fields;
      //fields.push_back(SHADINGSURFACEGROUPNAME);
      //fields.push_back(CONSTRUCTIONNAME);
      //fields.push_back(TRANSMITTANCESCHEDULENAME);
      std::pair<QString, std::vector<QString> > categoryAndFields = std::make_pair(QString("General"), fields);
      m_categoriesAndFields.push_back(categoryAndFields);
    }

    OSGridController::setCategoriesAndFields();
  }

  void SpacesSubsurfacesGridController::categorySelected(int index)
  {
    OSGridController::categorySelected(index);
  }

  void SpacesSubsurfacesGridController::addColumns(const QString &category, std::vector<QString> & fields)
  {
    // always show name and selected columns
    fields.insert(fields.begin(), { NAME, SELECTED });

    m_baseConcepts.clear();

    for (const auto &field : fields) {

      if (field == NAME) {
        addNameLineEditColumn(Heading(QString(NAME), false, false),
          false,
          false,
          CastNullAdapter<model::ShadingSurface>(&model::ShadingSurface::name),
          CastNullAdapter<model::ShadingSurface>(&model::ShadingSurface::setName)
          );
      }
      else if (field == SELECTED) {
        auto checkbox = QSharedPointer<QCheckBox>(new QCheckBox());
        checkbox->setToolTip("Check to select all rows");
        connect(checkbox.data(), &QCheckBox::stateChanged, this, &SpacesSubsurfacesGridController::selectAllStateChanged);

        addSelectColumn(Heading(QString(SELECTED), false, false, checkbox), "Check to select this row");
      }
      else {
        // unhandled
        OS_ASSERT(false);
      }
    }
  }

  QString SpacesSubsurfacesGridController::getColor(const model::ModelObject & modelObject)
  {
    QColor defaultColor(Qt::lightGray);
    return defaultColor.name();
  }

  void SpacesSubsurfacesGridController::checkSelectedFields()
  {
    if (!this->m_hasHorizontalHeader) return;

    OSGridController::checkSelectedFields();
  }

  void SpacesSubsurfacesGridController::onItemDropped(const OSItemId& itemId)
  {
  }

  void SpacesSubsurfacesGridController::refreshModelObjects()
  {
    auto exteriorLights = m_model.getModelObjects<model::ShadingSurface>();
    m_modelObjects = subsetCastVector<model::ModelObject>(exteriorLights);
    std::sort(m_modelObjects.begin(), m_modelObjects.end(), ModelObjectNameSorter());
  }

} // openstudio
