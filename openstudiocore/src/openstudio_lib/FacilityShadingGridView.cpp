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

#include "OSDropZone.hpp"
#include "OSItemSelectorButtons.hpp"

#include "../shared_gui_components/OSGridView.hpp"

#include "../model/Blind.hpp"
#include "../model/Blind_Impl.hpp"
#include "../model/ConstructionBase.hpp"
#include "../model/ConstructionBase_Impl.hpp"
#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/ModelObject.hpp"
#include "../model/ModelObject_Impl.hpp"
#include "../model/Schedule.hpp"
#include "../model/Schedule_Impl.hpp"
#include "../model/ShadingSurface.hpp"
#include "../model/ShadingSurface_Impl.hpp"
#include "../model/ShadingSurfaceGroup.hpp"
#include "../model/ShadingSurfaceGroup_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/idd/IddEnums.hxx"
#include "../utilities/idd/OS_ShadingSurface_FieldEnums.hxx"
#include "../utilities/idd/OS_ShadingSurfaceGroup_FieldEnums.hxx"
#include "../utilities/idd/OS_WindowMaterial_Blind_FieldEnums.hxx"

#include <QBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QRegExpValidator>

// These defines provide a common area for field display names
// used on column headers, and other grid widgets

#define NAME "Shading Surface Group Name"
#define SELECTED "All"

// GENERAL
#define TYPE "Type" // read only
#define SHADINGSURFACENAME "Shading Surface Name" // read only
#define CONSTRUCTIONNAME "Construction Name"
#define TRANSMITTANCESCHEDULENAME "Transmittance Schedule Name"

// FILTERS
#define SHADINGSURFACETYPE "Shading Surface Type"
#define SHADINGSURFACEGROUPNAME "Shading Surface Group Name"
#define ORIENTATION "Orientation"
#define TILT "Tilt"

namespace openstudio {

  struct ModelObjectNameSorter{
    // sort by name
    bool operator()(const model::ModelObject & lhs, const model::ModelObject & rhs){
      return (lhs.name() < rhs.name());
    }
  };

  FacilityShadingGridView::FacilityShadingGridView(bool isIP, const model::Model & model, QWidget * parent)
    : GridViewSubTab(isIP, model, parent)
  {
    auto modelObjects = subsetCastVector<model::ModelObject>(model.getModelObjects<model::ShadingSurfaceGroup>());
    std::sort(modelObjects.begin(), modelObjects.end(), ModelObjectNameSorter());

    m_gridController = new FacilityShadingGridController(isIP, "Shading Surface Group", IddObjectType::OS_ShadingSurfaceGroup, model, modelObjects);
    m_gridView = new OSGridView(m_gridController, "Shading Surface Group", "Drop\nShading Surface Group", false, parent);

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

    // SHADINGSURFACEGROUPNAME

    layout = new QVBoxLayout();

    label = new QLabel();
    label->setText(SHADINGSURFACENAME);
    label->setObjectName("H3");
    layout->addWidget(label, Qt::AlignTop | Qt::AlignLeft);

    m_nameFilter = new QLineEdit();
    m_nameFilter->setFixedWidth(OSItem::ITEM_WIDTH);
    // Evan note: there are issues with using the signal textChanged or textEdited, related to the design and updating of the gridview (loss of focus, and updates per key stroke)
    connect(m_nameFilter, &QLineEdit::editingFinished, this, &openstudio::FacilityShadingGridView::nameFilterChanged);

    QRegExp nameRegex("^\\S.*");
    auto nameValidator = new QRegExpValidator(nameRegex, this);
    m_nameFilter->setValidator(nameValidator);

    layout->addWidget(m_nameFilter, Qt::AlignTop | Qt::AlignLeft);
    layout->addStretch();
    filterGridLayout->addLayout(layout, filterGridLayout->rowCount() - 1, filterGridLayout->columnCount());

    // SHADINGSURFACETYPE

    layout = new QVBoxLayout();

    label = new QLabel();
    label->setText(SHADINGSURFACETYPE);
    label->setObjectName("H3");
    layout->addWidget(label, Qt::AlignTop | Qt::AlignLeft);

    m_typeFilter = new QComboBox();
    m_typeFilter->addItem("Building");
    m_typeFilter->addItem("Site");
    m_typeFilter->setFixedWidth(OSItem::ITEM_WIDTH);
    connect(m_typeFilter, &QComboBox::currentTextChanged, this, &openstudio::FacilityShadingGridView::typeFilterChanged);

    layout->addWidget(m_typeFilter, Qt::AlignTop | Qt::AlignLeft);
    layout->addStretch();
    filterGridLayout->addLayout(layout, filterGridLayout->rowCount() - 1, filterGridLayout->columnCount());

    // ORIENTATION

    layout = new QVBoxLayout();

    label = new QLabel();
    label->setText(ORIENTATION);
    label->setObjectName("H3");
    layout->addWidget(label, Qt::AlignTop | Qt::AlignLeft);

    m_orientationFilter = new QComboBox();
    m_orientationFilter->addItem("Horizontal");
    m_orientationFilter->addItem("Vertical");
    m_orientationFilter->setFixedWidth(OSItem::ITEM_WIDTH);
    connect(m_orientationFilter, &QComboBox::currentTextChanged, this, &openstudio::FacilityShadingGridView::orientationFilterChanged);

    layout->addWidget(m_orientationFilter, Qt::AlignTop | Qt::AlignLeft);
    layout->addStretch();
    filterGridLayout->addLayout(layout, filterGridLayout->rowCount() - 1, filterGridLayout->columnCount());

    // TILT

    layout = new QVBoxLayout();

    label = new QLabel();
    label->setText(TILT);
    label->setObjectName("H3");
    layout->addWidget(label, Qt::AlignTop | Qt::AlignLeft);

    m_tiltFilter = new QLineEdit();
    m_tiltFilter->setFixedWidth(0.7 * OSItem::ITEM_WIDTH);
        // Evan note: there are issues with using the signal textChanged or textEdited, related to the design and updating of the gridview (loss of focus, and updates per key stroke)
    connect(m_tiltFilter, &QLineEdit::editingFinished, this, &openstudio::FacilityShadingGridView::tiltFilterChanged);

    QRegExp regex("^(-?\\d*\\.?\\d+)?$");
    auto validator = new QRegExpValidator(regex, this);
    m_tiltFilter->setValidator(validator);

    label = new QLabel();
    label->setText("rad");
    label->setObjectName("H3");

    auto hLayout = new QHBoxLayout();
    hLayout->addWidget(m_tiltFilter, Qt::AlignTop | Qt::AlignLeft);
    hLayout->addWidget(label, Qt::AlignTop | Qt::AlignLeft);
    hLayout->addStretch();
    layout->addLayout(hLayout, Qt::AlignTop | Qt::AlignLeft);

    filterGridLayout->addLayout(layout, filterGridLayout->rowCount() - 1, filterGridLayout->columnCount());

    filterGridLayout->setRowStretch(filterGridLayout->rowCount(), 100);
    filterGridLayout->setColumnStretch(filterGridLayout->columnCount(), 100);

    m_gridView->m_contentLayout->addLayout(filterGridLayout);

    m_gridView->m_contentLayout->addSpacing(7);
  }

  void FacilityShadingGridView::nameFilterChanged()
  {
    m_objectsFilteredByName.clear();

    if (m_nameFilter->text().isEmpty()) {
      // nothing to filter
    }
    else {
      for (auto obj : this->m_gridController->getObjectSelector()->m_selectorObjects) {
        QString objName(obj.name().get().c_str());
        if (!objName.contains(m_nameFilter->text(), Qt::CaseInsensitive)) {
          m_objectsFilteredByName.insert(obj).second;
        }
      }
    }

    filterChanged();
  }

  void FacilityShadingGridView::typeFilterChanged(const QString& text)
  {
    m_objectsFilterdByType.clear();

    for (auto obj : this->m_gridController->getObjectSelector()->m_selectorObjects) {
      if (obj.iddObjectType() == IddObjectType::OS_ShadingSurfaceGroup){
        if (m_typeFilter->currentText() == obj.cast<model::ShadingSurfaceGroup>().shadingSurfaceType().c_str()) {
          m_objectsFilterdByType.insert(obj).second;
        }
      }
    }

    filterChanged();
  }

  void FacilityShadingGridView::orientationFilterChanged(const QString& text)
  {
    m_objectsFilteredByOrientation.clear();

    for (auto obj : this->m_gridController->getObjectSelector()->m_selectorObjects) {
      if (obj.iddObjectType() == IddObjectType::OS_WindowMaterial_Blind){
        if (m_orientationFilter->currentText() == obj.cast<model::Blind>().slatOrientation().c_str()) {
          m_objectsFilteredByOrientation.insert(obj).second;
        }
      }
    }

    filterChanged();
  }

  void FacilityShadingGridView::tiltFilterChanged()
  {
    m_objectsFilteredByTilt.clear();

    if (m_tiltFilter->text().isEmpty()) {
      // nothing to filter
    }
    else {
      for (auto obj : this->m_gridController->getObjectSelector()->m_selectorObjects) {
        QString objName(obj.name().get().c_str());
        if (!objName.contains(m_tiltFilter->text(), Qt::CaseInsensitive)) {
          m_objectsFilteredByTilt.insert(obj).second;
        }
      }
    }

    filterChanged();
  }

  void FacilityShadingGridView::filterChanged()
  {
    std::set<openstudio::model::ModelObject> allFilteredObjects = m_objectsFilteredByName;

    for (auto obj : m_objectsFilteredByTilt) {
      if (allFilteredObjects.count(obj) == 0) {
        allFilteredObjects.insert(obj).second;
      }
    }

    for (auto obj : m_objectsFilterdByType) {
      if (allFilteredObjects.count(obj) == 0) {
        allFilteredObjects.insert(obj).second;
      }
    }

    for (auto obj : m_objectsFilteredByOrientation) {
      if (allFilteredObjects.count(obj) == 0) {
        allFilteredObjects.insert(obj).second;
      }
    }

    this->m_gridController->getObjectSelector()->m_filteredObjects = allFilteredObjects;

    this->m_gridView->requestRefreshAll();

    onClearSelection();

  }

  void FacilityShadingGridView::addObject(const IddObjectType& iddObjectType)
  {
    if (IddObjectType::OS_ShadingSurfaceGroup == iddObjectType.value()) {
      model::ShadingSurfaceGroup(this->m_model);
    }
  }

  void FacilityShadingGridView::purgeObjects(const IddObjectType& iddObjectType)
  {
    for (auto mo : this->m_model.getConcreteModelObjects<model::ShadingSurface>()){
      mo.remove();
    }
  }

  void FacilityShadingGridView::onSelectItem()
  {
    //m_itemSelectorButtons->enableAddButton();
    //m_itemSelectorButtons->enableCopyButton();
    //m_itemSelectorButtons->enableRemoveButton();
    //m_itemSelectorButtons->enablePurgeButton();
  }

  void FacilityShadingGridView::onClearSelection()
  {
    m_itemSelectorButtons->disableAddButton();
    //m_itemSelectorButtons->disableCopyButton();
    //m_itemSelectorButtons->disableRemoveButton();
    m_itemSelectorButtons->disablePurgeButton();
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
    {
      std::vector<QString> fields;
      fields.push_back(SHADINGSURFACENAME);
      fields.push_back(TYPE);
      fields.push_back(TRANSMITTANCESCHEDULENAME);
      fields.push_back(CONSTRUCTIONNAME);
      std::pair<QString, std::vector<QString> > categoryAndFields = std::make_pair(QString("General"), fields);
      m_categoriesAndFields.push_back(categoryAndFields);
    }

    OSGridController::setCategoriesAndFields();
  }

  void FacilityShadingGridController::categorySelected(int index)
  {
    OSGridController::categorySelected(index);
  }

  void FacilityShadingGridController::addColumns(const QString &category, std::vector<QString> & fields)
  {
    // always show name and selected columns
    fields.insert(fields.begin(), { NAME, SELECTED });

    m_baseConcepts.clear();

    for (const auto &field : fields) {

      if (field == NAME) {
        addNameLineEditColumn(Heading(QString(NAME), false, false),
          false,
          false,
          CastNullAdapter<model::ShadingSurfaceGroup>(&model::ShadingSurfaceGroup::name),
          CastNullAdapter<model::ShadingSurfaceGroup>(&model::ShadingSurfaceGroup::setName)
          );
      }
      else {

        std::function<std::vector<model::ModelObject>(const model::ShadingSurfaceGroup &)> allShadingSurfaces(
          [](const model::ShadingSurfaceGroup &t_shadingSurfaceGroup) {
          std::vector<model::ModelObject> allModelObjects;
          auto shadingSurfaces = t_shadingSurfaceGroup.shadingSurfaces();
          allModelObjects.insert(allModelObjects.end(), shadingSurfaces.begin(), shadingSurfaces.end());
          return allModelObjects;
        }
        );

        std::function<std::vector<std::string>(const model::ShadingSurfaceGroup &)> allTypes(
          [](const model::ShadingSurfaceGroup &t_shadingSurfaceGroup) {
          std::vector<std::string> allTypes;
          auto shadingSurfaceType = t_shadingSurfaceGroup.shadingSurfaceType();
          auto shadingSurfaces = t_shadingSurfaceGroup.shadingSurfaces();
          for (auto shadingSurface : shadingSurfaces) {
            allTypes.push_back(shadingSurfaceType);
          }
          return allTypes;
        }
        );

        std::function<std::vector<boost::optional<model::ModelObject>>(const model::ShadingSurfaceGroup &)> allTransmittanceSchedules(
          [](const model::ShadingSurfaceGroup &t_shadingSurfaceGroup) {
          std::vector<boost::optional<model::ModelObject>> allModelObjects;
          auto shadingSurfaces = t_shadingSurfaceGroup.shadingSurfaces();
          for (auto shadingSurface : shadingSurfaces) {
            auto transmittanceSchedule = shadingSurface.transmittanceSchedule();
            if (transmittanceSchedule)
            {
              allModelObjects.push_back(transmittanceSchedule.get());
            }
            else {
              allModelObjects.push_back(boost::optional<model::ModelObject>());
            }
          }
          return allModelObjects;
        }
        );

        std::function<std::vector<boost::optional<model::ModelObject> >(const model::ShadingSurfaceGroup &)> allConstructions(
          [allShadingSurfaces](const model::ShadingSurfaceGroup &t_shadingSurfaceGroup) {
          std::vector<boost::optional<model::ModelObject> > allModelObjects;
          std::vector<boost::optional<model::ConstructionBase> > allConstructions;
          for (auto shadingSurface : allShadingSurfaces(t_shadingSurfaceGroup)) {
            auto construction = shadingSurface.cast<model::ShadingSurface>().construction();
            if (construction) {
              allConstructions.push_back(construction);
            }
            else {
              allConstructions.push_back(boost::optional<model::ConstructionBase>());
            }
          }
          allModelObjects.insert(allModelObjects.end(), allConstructions.begin(), allConstructions.end());

          return allModelObjects;
        }
        );

        if (field == SELECTED) {
          auto checkbox = QSharedPointer<QCheckBox>(new QCheckBox());
          checkbox->setToolTip("Check to select all rows");
          connect(checkbox.data(), &QCheckBox::stateChanged, this, &FacilityShadingGridController::selectAllStateChanged);
          connect(checkbox.data(), &QCheckBox::stateChanged, this->gridView(), &OSGridView::gridRowSelectionChanged);

          addSelectColumn(Heading(QString(SELECTED), false, false, checkbox), "Check to select this row",
            DataSource(
            allShadingSurfaces,
            true
            )
            );
        }
        else if (field == TYPE) {
          std::function<std::vector<std::string>()> choices(
            []() {
            std::vector<std::string> choices{"Building","Site"};
            return choices;
          }
          );

          addComboBoxColumn(Heading(QString(TYPE)),
            std::function<std::string(const std::string &)>(static_cast<std::string(*)(const std::string&)>(&openstudio::toString)),
            choices,
            CastNullAdapter<model::ShadingSurfaceGroup>(&model::ShadingSurfaceGroup::shadingSurfaceType),
            CastNullAdapter<model::ShadingSurfaceGroup>(&model::ShadingSurfaceGroup::setShadingSurfaceType),
            boost::optional<std::function<void(model::ShadingSurfaceGroup*)>>(),
            boost::optional<std::function<bool(model::ShadingSurfaceGroup*)>>(),
            DataSource(
            allTypes,
            true
            )
            );
        }
        else if (field == SHADINGSURFACENAME) {
          addLoadNameColumn(Heading(QString(SHADINGSURFACENAME), true, false),
            CastNullAdapter<model::ShadingSurface>(&model::ShadingSurface::name),
            CastNullAdapter<model::ShadingSurface>(&model::ShadingSurface::setName),
            boost::optional<std::function<void(model::ShadingSurface *)>>(
            std::function<void(model::ShadingSurface *)>(
            [](model::ShadingSurface *t_ss)
          {
            t_ss->remove();
          }
            )
            ),
            DataSource(
            allShadingSurfaces,
            true
            )
            );
        }
        else if (field == CONSTRUCTIONNAME) {
          addDropZoneColumn(Heading(QString(CONSTRUCTIONNAME), true, false),
            CastNullAdapter<model::ShadingSurface>(&model::ShadingSurface::construction),
            CastNullAdapter<model::ShadingSurface>(&model::ShadingSurface::setConstruction),
            boost::optional<std::function<void(model::ShadingSurface*)> >(NullAdapter(&model::ShadingSurface::resetConstruction)),
            DataSource(
            allConstructions,
            true
            )
            );
        }
        else if (field == TRANSMITTANCESCHEDULENAME) {
          std::function<bool(model::ShadingSurface *, const model::Schedule &)> setter(
            [](model::ShadingSurface *t_shadingSurface, const model::Schedule &t_schedule) {
            auto copy = t_schedule;
            return t_shadingSurface->setTransmittanceSchedule(copy);
          }
          );

          addDropZoneColumn(Heading(QString(TRANSMITTANCESCHEDULENAME), true, false),
            CastNullAdapter<model::ShadingSurface>(&model::ShadingSurface::transmittanceSchedule),
            setter,
            boost::optional<std::function<void(model::ShadingSurface*)>>(CastNullAdapter<model::ShadingSurface>(&model::ShadingSurface::resetTransmittanceSchedule)),
            DataSource(
            allTransmittanceSchedules,
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

  QString FacilityShadingGridController::getColor(const model::ModelObject & modelObject)
  {
    QColor defaultColor(Qt::lightGray);
    return defaultColor.name();
  }

  void FacilityShadingGridController::checkSelectedFields()
  {
    if (!this->m_hasHorizontalHeader) return;

    OSGridController::checkSelectedFields();
  }

  void FacilityShadingGridController::onItemDropped(const OSItemId& itemId)
  {
  }

  void FacilityShadingGridController::refreshModelObjects()
  {
    m_modelObjects = subsetCastVector<model::ModelObject>(m_model.getModelObjects<model::ShadingSurfaceGroup>());
    std::sort(m_modelObjects.begin(), m_modelObjects.end(), ModelObjectNameSorter());
  }

} // openstudio
