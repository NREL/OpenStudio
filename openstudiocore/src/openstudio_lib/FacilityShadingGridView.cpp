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
#include "../utilities/units/QuantityConverter.hpp"

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
#define ORIENTATIONGREATERTHAN "Degrees Orientation >"
#define ORIENTATIONLESSTHAN "Degrees Orientation <"
#define TILTGREATERTHAN "Degrees Tilt >"
#define TILTLESSTHAN "Degrees Tilt <"

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
    m_typeFilter->addItem("Site");
    m_typeFilter->addItem("Building");
    m_typeFilter->addItem("Space");
    m_typeFilter->setFixedWidth(OSItem::ITEM_WIDTH);
    connect(m_typeFilter, &QComboBox::currentTextChanged, this, &openstudio::FacilityShadingGridView::typeFilterChanged);

    layout->addWidget(m_typeFilter, Qt::AlignTop | Qt::AlignLeft);
    layout->addStretch();
    filterGridLayout->addLayout(layout, filterGridLayout->rowCount() - 1, filterGridLayout->columnCount());

    // TILTGREATERTHAN

    layout = new QVBoxLayout();

    label = new QLabel();
    label->setText(TILTGREATERTHAN);
    label->setObjectName("H3");
    layout->addWidget(label, Qt::AlignTop | Qt::AlignLeft);

    m_tiltGreaterThanFilter = new QLineEdit();
    m_tiltGreaterThanFilter->setFixedWidth(OSItem::ITEM_WIDTH);
    connect(m_tiltGreaterThanFilter, &QLineEdit::editingFinished, this, &openstudio::FacilityShadingGridView::tiltFilterChanged);

    QRegExp regex("^(-?\\d*\\.?\\d+)?$");
    auto validator = new QRegExpValidator(regex, this);
    m_tiltGreaterThanFilter->setValidator(validator);

    layout->addWidget(m_tiltGreaterThanFilter, Qt::AlignTop | Qt::AlignLeft);
    layout->addStretch();
    filterGridLayout->addLayout(layout, filterGridLayout->rowCount() - 1, filterGridLayout->columnCount());

    //TILTLESSTHAN

    layout = new QVBoxLayout();

    label = new QLabel();
    label->setText(TILTLESSTHAN);
    label->setObjectName("H3");
    layout->addWidget(label, Qt::AlignTop | Qt::AlignLeft);

    m_tiltLessThanFilter = new QLineEdit();
    m_tiltLessThanFilter->setFixedWidth(OSItem::ITEM_WIDTH);
    // Evan note: there are issues with using the signal textChanged or textEdited, related to the design and updating of the gridview (loss of focus, and updates per key stroke)
    connect(m_tiltLessThanFilter, &QLineEdit::editingFinished, this, &openstudio::FacilityShadingGridView::tiltFilterChanged);

    validator = new QRegExpValidator(regex, this);
    m_tiltLessThanFilter->setValidator(validator);

    layout->addWidget(m_tiltLessThanFilter, Qt::AlignTop | Qt::AlignLeft);
    layout->addStretch();
    filterGridLayout->addLayout(layout, filterGridLayout->rowCount() - 1, filterGridLayout->columnCount());

    // ORIENTATIONGREATERTHAN

    layout = new QVBoxLayout();

    label = new QLabel();
    label->setText(ORIENTATIONGREATERTHAN);
    label->setObjectName("H3");
    layout->addWidget(label, Qt::AlignTop | Qt::AlignLeft);

    m_orientationGreaterThanFilter = new QLineEdit();
    m_orientationGreaterThanFilter->setFixedWidth(OSItem::ITEM_WIDTH);
    connect(m_orientationGreaterThanFilter, &QLineEdit::editingFinished, this, &openstudio::FacilityShadingGridView::orientationFilterChanged);

    validator = new QRegExpValidator(regex, this);
    m_orientationGreaterThanFilter->setValidator(validator);

    layout->addWidget(m_orientationGreaterThanFilter, Qt::AlignTop | Qt::AlignLeft);
    layout->addStretch();
    filterGridLayout->addLayout(layout, filterGridLayout->rowCount() - 1, filterGridLayout->columnCount());

    // ORIENTATIONLESSTHAN

    layout = new QVBoxLayout();

    label = new QLabel();
    label->setText(ORIENTATIONLESSTHAN);
    label->setObjectName("H3");
    layout->addWidget(label, Qt::AlignTop | Qt::AlignLeft);

    m_orientationLessThanFilter = new QLineEdit();
    m_orientationLessThanFilter->setFixedWidth(OSItem::ITEM_WIDTH);
    // Evan note: there are issues with using the signal textChanged or textEdited, related to the design and updating of the gridview (loss of focus, and updates per key stroke)
    connect(m_orientationLessThanFilter, &QLineEdit::editingFinished, this, &openstudio::FacilityShadingGridView::orientationFilterChanged);

    validator = new QRegExpValidator(regex, this);
    m_orientationLessThanFilter->setValidator(validator);

    layout->addWidget(m_orientationLessThanFilter, Qt::AlignTop | Qt::AlignLeft);
    layout->addStretch();
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
          m_objectsFilteredByName.insert(obj);
        }
      }
    }

    filterChanged();
  }

  void FacilityShadingGridView::typeFilterChanged(const QString& text)
  {
    m_objectsFilterdByType.clear();

    for (auto obj : this->m_gridController->getObjectSelector()->m_selectorObjects) {
      auto parent = obj.parent();
      if (parent && parent->iddObjectType() == IddObjectType::OS_ShadingSurfaceGroup){
        if (m_typeFilter->currentText() != parent->cast<model::ShadingSurfaceGroup>().shadingSurfaceType().c_str()) {
          if (m_objectsFilterdByType.count(obj) == 0) {
            m_objectsFilterdByType.insert(obj);
          }
        }
      }
    }

    filterChanged();
  }

  void FacilityShadingGridView::orientationFilterChanged()
  {
    m_objectsFilteredByOrientation.clear();

    auto objectSelector = this->m_gridController->getObjectSelector();

    auto upperLimit = std::numeric_limits<double>::max();
    auto lowerLimit = std::numeric_limits<double>::min();

    if (!this->m_orientationLessThanFilter->text().isEmpty()) {
      upperLimit = this->m_orientationLessThanFilter->text().toDouble();
    }

    if (!this->m_orientationGreaterThanFilter->text().isEmpty()) {
      lowerLimit = this->m_orientationGreaterThanFilter->text().toDouble();
    }

    auto convertedValue = convert(upperLimit, "deg", "rad");
    OS_ASSERT(convertedValue);
    upperLimit = *convertedValue;

    convertedValue = convert(lowerLimit, "deg", "rad");
    OS_ASSERT(convertedValue);
    lowerLimit = *convertedValue;

    objectSelector->m_filteredObjects.clear();


    for (auto obj : this->m_gridController->getObjectSelector()->m_selectorObjects) {
      if (obj.iddObjectType() == IddObjectType::OS_ShadingSurfaceGroup){
        for (auto shadingSurface : obj.cast<model::ShadingSurfaceGroup>().shadingSurfaces()) {
          auto orientation = shadingSurface.azimuth();
          if (orientation >= upperLimit || orientation <= lowerLimit) {
            m_objectsFilteredByOrientation.insert(obj);
          }
        }
      }
    }

    this->m_gridView->requestRefreshAll();

    filterChanged();
  }
  
  void FacilityShadingGridView::tiltFilterChanged()
  {
    m_objectsFilteredByTilt.clear();

    auto objectSelector = this->m_gridController->getObjectSelector();

    auto upperLimit = std::numeric_limits<double>::max();
    auto lowerLimit = std::numeric_limits<double>::min();

    if (!this->m_tiltLessThanFilter->text().isEmpty()) {
      upperLimit = this->m_tiltLessThanFilter->text().toDouble();
    }

    if (!this->m_tiltGreaterThanFilter->text().isEmpty()) {
      lowerLimit = this->m_tiltGreaterThanFilter->text().toDouble();
    }

    auto convertedValue = convert(upperLimit, "deg", "rad");
    OS_ASSERT(convertedValue);
    upperLimit = *convertedValue;

    convertedValue = convert(lowerLimit, "deg", "rad");
    OS_ASSERT(convertedValue);
    lowerLimit = *convertedValue;

    objectSelector->m_filteredObjects.clear();

    for (auto obj : this->m_gridController->getObjectSelector()->m_selectorObjects) {
      if (obj.iddObjectType() == IddObjectType::OS_ShadingSurfaceGroup){
        for (auto shadingSurface : obj.cast<model::ShadingSurfaceGroup>().shadingSurfaces()) {
          auto tilt = shadingSurface.tilt();
          if (tilt >= upperLimit || tilt <= lowerLimit) {
            m_objectsFilteredByOrientation.insert(obj);
          }
        }
      }
    }

    this->m_gridView->requestRefreshAll();

    filterChanged();
  }

  void FacilityShadingGridView::filterChanged()
  {
    std::set<openstudio::model::ModelObject> allFilteredObjects = m_objectsFilteredByName;

    for (auto obj : m_objectsFilteredByTilt) {
      if (allFilteredObjects.count(obj) == 0) {
        allFilteredObjects.insert(obj);
      }
    }

    for (auto obj : m_objectsFilterdByType) {
      if (allFilteredObjects.count(obj) == 0) {
        allFilteredObjects.insert(obj);
      }
    }

    for (auto obj : m_objectsFilteredByOrientation) {
      if (allFilteredObjects.count(obj) == 0) {
        allFilteredObjects.insert(obj);
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
    // show type next to name, since it comes from the groups
    fields.insert(fields.begin(), { NAME, TYPE, SELECTED });

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
      // Evan note: TODO to correctly use this column we need a new control --
      // a dropzone for spaces, and a combo box with site and building as choices
      else if (field == TYPE) {
        std::function<std::vector<std::string>()> choices(
          []() {
          std::vector<std::string> choices{  "Site", "Building", "Space" };
          return choices;
        }
        );

        addComboBoxColumn(Heading(QString(TYPE)),
          std::function<std::string(const std::string &)>(static_cast<std::string(*)(const std::string&)>(&openstudio::toString)),
          choices,
          CastNullAdapter<model::ShadingSurfaceGroup>(&model::ShadingSurfaceGroup::shadingSurfaceType),
          CastNullAdapter<model::ShadingSurfaceGroup>(&model::ShadingSurfaceGroup::setShadingSurfaceType),
          boost::optional<std::function<void(model::ShadingSurfaceGroup*)>>(),
          boost::optional<std::function<bool(model::ShadingSurfaceGroup*)>>()
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
            allShadingSurfaces,
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
            allShadingSurfaces,
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
