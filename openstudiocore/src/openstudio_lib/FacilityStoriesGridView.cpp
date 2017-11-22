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

#include "FacilityStoriesGridView.hpp"

#include "OSDropZone.hpp"
#include "OSItemSelectorButtons.hpp"
#include "RenderingColorWidget.hpp"

#include "../shared_gui_components/OSDoubleEdit.hpp"
#include "../shared_gui_components/OSGridView.hpp"

#include "../model/BuildingStory.hpp"
#include "../model/BuildingStory_Impl.hpp"
#include "../model/DefaultConstructionSet.hpp"
#include "../model/DefaultConstructionSet_Impl.hpp"
#include "../model/DefaultScheduleSet.hpp"
#include "../model/DefaultScheduleSet_Impl.hpp"
#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/ModelObject.hpp"
#include "../model/ModelObject_Impl.hpp"
#include "../model/RenderingColor.hpp"
#include "../model/RenderingColor_Impl.hpp"

// For purgeObjects
#include "../model/Space.hpp"


#include "../utilities/core/Assert.hpp"
#include "../utilities/idd/IddEnums.hxx"
#include "../utilities/idd/OS_BuildingStory_FieldEnums.hxx"
#include "../utilities/units/QuantityConverter.hpp"

#include <QBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QRegExp>

// These defines provide a common area for field display names
// used on column headers, and other grid widgets

#define NAME "Story Name"
#define SELECTED "All"

// GENERAL
#define NOMINALZCOORDINATE "Nominal Z Coordinate"
#define NOMINALFLOORTOFLOORHEIGHT "Nominal Floor to Floor Height"
#define DEFAULTCONSTRUCTIONSETNAME "Default Construction Set Name"
#define DEFAULTSCHEDULESETNAME "Default Schedule Set Name"
#define GROUPRENDERINGNAME "Group Rendering Name"
#define NOMINALFLOORTOCEILINGHEIGHT "Nominal Floor to Ceiling Height"

// FILTERS
#define NOMINALZCOORDINATEGREATERTHAN "Nominal Z Coordinate >"
#define NOMINALZCOORDINATELESSTHAN "Nominal Z Coordinate <"

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
    auto modelObjects = subsetCastVector<model::ModelObject>(m_model.getConcreteModelObjects<model::BuildingStory>());
    std::sort(modelObjects.begin(), modelObjects.end(), ModelObjectNameSorter());

    m_gridController = new FacilityStoriesGridController(isIP, "Building Stories", IddObjectType::OS_BuildingStory, model, modelObjects);
    m_gridView = new OSGridView(m_gridController, "Building Stories", "Drop\nStory", false, parent);

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

    // NOMINALZCOORDINATEGREATERTHAN

    layout = new QVBoxLayout();

    label = new QLabel();
    label->setText(NOMINALZCOORDINATEGREATERTHAN);
    label->setObjectName("H3");
    layout->addWidget(label, Qt::AlignTop | Qt::AlignLeft);

    m_greaterThanFilter = new QLineEdit();
    m_greaterThanFilter->setFixedWidth(OSItem::ITEM_WIDTH);
    connect(m_greaterThanFilter, &QLineEdit::editingFinished, this, &openstudio::FacilityStoriesGridView::greaterThanFilterChanged);

    QRegExp regex("^(-?\\d*\\.?\\d+)?$");
    auto validator = new QRegExpValidator(regex, this);
    m_greaterThanFilter->setValidator(validator);

    layout->addWidget(m_greaterThanFilter, Qt::AlignTop | Qt::AlignLeft);
    layout->addStretch();
    filterGridLayout->addLayout(layout, filterGridLayout->rowCount() - 1, filterGridLayout->columnCount());

    // NOMINALZCOORDINATELESSTHAN

    layout = new QVBoxLayout();

    label = new QLabel();
    label->setText(NOMINALZCOORDINATELESSTHAN);
    label->setObjectName("H3");
    layout->addWidget(label, Qt::AlignTop | Qt::AlignLeft);

    m_lessThanFilter = new QLineEdit();
    m_lessThanFilter->setFixedWidth(OSItem::ITEM_WIDTH);
    // Evan note: there are issues with using the signal textChanged or textEdited, related to the design and updating of the gridview (loss of focus, and updates per key stroke)
    connect(m_lessThanFilter, &QLineEdit::editingFinished, this, &openstudio::FacilityStoriesGridView::lessThanFilterChanged);

    validator = new QRegExpValidator(regex, this);
    m_greaterThanFilter->setValidator(validator);

    layout->addWidget(m_lessThanFilter, Qt::AlignTop | Qt::AlignLeft);
    layout->addStretch();
    filterGridLayout->addLayout(layout, filterGridLayout->rowCount() - 1, filterGridLayout->columnCount());

    filterGridLayout->setRowStretch(filterGridLayout->rowCount(), 100);
    filterGridLayout->setColumnStretch(filterGridLayout->columnCount(), 100);

    m_gridView->m_contentLayout->addLayout(filterGridLayout);

    m_gridView->m_contentLayout->addSpacing(7);

    onClearSelection();

  }

  void FacilityStoriesGridView::addObject(const IddObjectType& iddObjectType)
  {
    if (IddObjectType::OS_BuildingStory == iddObjectType.value()) {
      model::BuildingStory(this->m_model);
    }
  }

  void FacilityStoriesGridView::purgeObjects(const IddObjectType& iddObjectType)
  {
    // If no spaces in the story -> remove
    for (model::BuildingStory story : this->m_model.getConcreteModelObjects<model::BuildingStory>()){
      if (story.spaces().empty()){
        story.remove();
      }
    }
  }

  void FacilityStoriesGridView::greaterThanFilterChanged()
  {
    filterChanged();
  }

  void FacilityStoriesGridView::lessThanFilterChanged()
  {
    filterChanged();
  }

  void FacilityStoriesGridView::filterChanged()
  {
    auto objectSelector = this->m_gridController->getObjectSelector();

    auto upperLimit = std::numeric_limits<double>::max();
    auto lowerLimit = std::numeric_limits<double>::min();

    if (!this->m_lessThanFilter->text().isEmpty()) {
      upperLimit = this->m_lessThanFilter->text().toDouble();
    }

    if (!this->m_greaterThanFilter->text().isEmpty()) {
      lowerLimit = this->m_greaterThanFilter->text().toDouble();
    }

    if (m_isIP == true) {
      auto convertedValue = convert(upperLimit, "ft", "m");
      OS_ASSERT(convertedValue);
      upperLimit = *convertedValue;

      convertedValue = convert(lowerLimit, "ft", "m");
      OS_ASSERT(convertedValue);
      lowerLimit = *convertedValue;
    }

    objectSelector->m_filteredObjects.clear();

    for (auto obj : objectSelector->m_selectorObjects) {
      auto nominalZCoordinate = obj.cast<model::BuildingStory>().nominalZCoordinate();
      if (nominalZCoordinate) {
        if (*nominalZCoordinate >= upperLimit || *nominalZCoordinate <= lowerLimit) {
          objectSelector->m_filteredObjects.insert(obj);
        }
      }
    }

    this->m_gridView->requestRefreshAll();
  }

  void FacilityStoriesGridView::onSelectItem()
  {
    //m_itemSelectorButtons->enableAddButton();
    //m_itemSelectorButtons->enableCopyButton();
    m_itemSelectorButtons->enableRemoveButton();
    //m_itemSelectorButtons->enablePurgeButton();
  }

  void FacilityStoriesGridView::onClearSelection()
  {
    //m_itemSelectorButtons->disableAddButton();
    //m_itemSelectorButtons->disableCopyButton();
    m_itemSelectorButtons->disableRemoveButton();
    //m_itemSelectorButtons->disablePurgeButton();
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
      fields.push_back(GROUPRENDERINGNAME);
      fields.push_back(NOMINALZCOORDINATE);
      fields.push_back(NOMINALFLOORTOFLOORHEIGHT);
      fields.push_back(NOMINALFLOORTOCEILINGHEIGHT);
      fields.push_back(DEFAULTCONSTRUCTIONSETNAME);
      fields.push_back(DEFAULTSCHEDULESETNAME);
      std::pair<QString, std::vector<QString> > categoryAndFields = std::make_pair(QString("General"), fields);
      m_categoriesAndFields.push_back(categoryAndFields);
    }

    OSGridController::setCategoriesAndFields();
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
        connect(checkbox.data(), &QCheckBox::stateChanged, this->gridView(), &OSGridView::gridRowSelectionChanged);

        addSelectColumn(Heading(QString(SELECTED), false, false, checkbox), "Check to select this row");
      }
      else if (field == NOMINALZCOORDINATE) {
        addQuantityEditColumn(Heading(QString(NOMINALFLOORTOFLOORHEIGHT)),
          QString("m"),
          QString("m"),
          QString("ft"),
          m_isIP,
          NullAdapter(&model::BuildingStory::nominalZCoordinate),
          NullAdapter(&model::BuildingStory::setNominalZCoordinate)
          );
      }
      else if (field == NOMINALFLOORTOFLOORHEIGHT) {
        addQuantityEditColumn(Heading(QString(NOMINALFLOORTOFLOORHEIGHT)),
          QString("m"),
          QString("m"),
          QString("ft"),
          m_isIP,
          NullAdapter(&model::BuildingStory::nominalFloortoFloorHeight),
          NullAdapter(&model::BuildingStory::setNominalFloortoFloorHeight)
          );
      }
      else if (field == DEFAULTCONSTRUCTIONSETNAME) {
        addDropZoneColumn(Heading(QString(DEFAULTCONSTRUCTIONSETNAME)),
          CastNullAdapter<model::BuildingStory>(&model::BuildingStory::defaultConstructionSet),
          CastNullAdapter<model::BuildingStory>(&model::BuildingStory::setDefaultConstructionSet),
          boost::optional<std::function<void(model::BuildingStory*)>>(CastNullAdapter<model::BuildingStory>(&model::BuildingStory::resetDefaultConstructionSet))
          );
      }
      else if (field == DEFAULTSCHEDULESETNAME) {
        addDropZoneColumn(Heading(QString(DEFAULTSCHEDULESETNAME)),
          CastNullAdapter<model::BuildingStory>(&model::BuildingStory::defaultScheduleSet),
          CastNullAdapter<model::BuildingStory>(&model::BuildingStory::setDefaultScheduleSet),
          boost::optional<std::function<void(model::BuildingStory*)>>(CastNullAdapter<model::BuildingStory>(&model::BuildingStory::resetDefaultScheduleSet))
          );
      }
      else if (field == GROUPRENDERINGNAME) {
        addRenderingColorColumn(Heading(QString(GROUPRENDERINGNAME), true, false),
          CastNullAdapter<model::BuildingStory>(&model::BuildingStory::renderingColor),
          CastNullAdapter<model::BuildingStory>(&model::BuildingStory::setRenderingColor)
          );
      }
      else if (field == NOMINALFLOORTOCEILINGHEIGHT) {
        addQuantityEditColumn(Heading(QString(NOMINALFLOORTOCEILINGHEIGHT)),
          QString("m"),
          QString("m"),
          QString("ft"),
          m_isIP,
          NullAdapter(&model::BuildingStory::nominalFloortoCeilingHeight),
          NullAdapter(&model::BuildingStory::setNominalFloortoCeilingHeight)
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
    m_modelObjects = subsetCastVector<model::ModelObject>(m_model.getConcreteModelObjects<model::BuildingStory>());
    std::sort(m_modelObjects.begin(), m_modelObjects.end(), ModelObjectNameSorter());
  }

} // openstudio
