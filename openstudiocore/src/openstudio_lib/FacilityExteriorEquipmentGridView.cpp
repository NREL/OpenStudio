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

#include "FacilityExteriorEquipmentGridView.hpp"

#include "OSItemSelectorButtons.hpp"

#include "../shared_gui_components/OSGridView.hpp"

#include "../model/ExteriorLights.hpp"
#include "../model/ExteriorLights_Impl.hpp"
#include "../model/ExteriorLightsDefinition.hpp"
#include "../model/ExteriorLightsDefinition_Impl.hpp"
#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/ModelObject.hpp"
#include "../model/ModelObject_Impl.hpp"
#include "../model/Schedule.hpp"
#include "../model/Schedule_Impl.hpp"

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

#define NAME "Exterior Lights Name"
#define SELECTED "All"

// EXTERIOR LIGHTS
#define EXTERIORLIGHTSDEFINITION "Exterior Lights Definition"
#define SCHEDULE "Schedule"
#define CONTROLOPTION "Control Option"
#define MULTIPLIER "Multiplier"
#define ENDUSESUBCATEGORY "End Use Subcategory"

namespace openstudio {

  struct ModelObjectNameSorter{
    // sort by name
    bool operator()(const model::ModelObject & lhs, const model::ModelObject & rhs){
      return (lhs.name() < rhs.name());
    }
  };

  FacilityExteriorEquipmentGridView::FacilityExteriorEquipmentGridView(bool isIP, const model::Model & model, QWidget * parent)
    : GridViewSubTab(isIP, model, parent)
  {
    auto modelObjects = subsetCastVector<model::ModelObject>(model.getModelObjects<model::ExteriorLights>());
    std::sort(modelObjects.begin(), modelObjects.end(), ModelObjectNameSorter());

    m_gridController = new FacilityExteriorEquipmentGridController(isIP, "Exterior Lights", IddObjectType::OS_Exterior_Lights, model, modelObjects);
    auto gridView = new OSGridView(m_gridController, "Exterior Lights", "Drop\nExterior Lights", false, parent);

    setGridController(m_gridController);
    setGridView(gridView);
  }

  void FacilityExteriorEquipmentGridView::addObject(const IddObjectType& iddObjectType)
  {
    if (IddObjectType::OS_Exterior_Lights == iddObjectType.value()) {
      model::ExteriorLights(model::ExteriorLightsDefinition(this->m_model));
    }
  }

  void FacilityExteriorEquipmentGridView::purgeObjects(const IddObjectType& iddObjectType)
  {
    for (auto mo : this->m_model.getConcreteModelObjects<model::ExteriorLights>()){
      mo.remove();
    }
  }

  void FacilityExteriorEquipmentGridView::onSelectItem()
  {
    //m_itemSelectorButtons->enableAddButton();
    m_itemSelectorButtons->enableCopyButton();
    m_itemSelectorButtons->enableRemoveButton();
    m_itemSelectorButtons->enablePurgeButton();
  }

  void FacilityExteriorEquipmentGridView::onClearSelection()
  {
    //m_itemSelectorButtons->disableAddButton();
    m_itemSelectorButtons->disableCopyButton();
    m_itemSelectorButtons->disableRemoveButton();
    m_itemSelectorButtons->disablePurgeButton();
  }
  
  FacilityExteriorEquipmentGridController::FacilityExteriorEquipmentGridController(bool isIP,
    const QString & headerText,
    IddObjectType iddObjectType,
    model::Model model,
    std::vector<model::ModelObject> modelObjects) :
    OSGridController(isIP, headerText, iddObjectType, model, modelObjects)
  {
    setCategoriesAndFields();
  }

  void FacilityExteriorEquipmentGridController::setCategoriesAndFields()
  {
    {
      std::vector<QString> fields;
      fields.push_back(EXTERIORLIGHTSDEFINITION);
      fields.push_back(SCHEDULE);
      fields.push_back(CONTROLOPTION);
      fields.push_back(MULTIPLIER);
      fields.push_back(ENDUSESUBCATEGORY);
      std::pair<QString, std::vector<QString> > categoryAndFields = std::make_pair(QString("Exterior Lights"), fields);
      m_categoriesAndFields.push_back(categoryAndFields);
    }

    OSGridController::setCategoriesAndFields();
  }

  void FacilityExteriorEquipmentGridController::categorySelected(int index)
  {
    OSGridController::categorySelected(index);
  }

  void FacilityExteriorEquipmentGridController::addColumns(const QString &category, std::vector<QString> & fields)
  {
    // always show name and selected columns
    fields.insert(fields.begin(), { NAME, SELECTED });

    m_baseConcepts.clear();

    for (const auto &field : fields) {

      if (field == NAME) {
        addNameLineEditColumn(Heading(QString(NAME), false, false),
          false,
          false,
          CastNullAdapter<model::ExteriorLights>(&model::ExteriorLights::name),
          CastNullAdapter<model::ExteriorLights>(&model::ExteriorLights::setName)
          );
      }
      else if (field == SELECTED) {
        auto checkbox = QSharedPointer<QCheckBox>(new QCheckBox());
        checkbox->setToolTip("Check to select all rows");
        connect(checkbox.data(), &QCheckBox::stateChanged, this, &FacilityExteriorEquipmentGridController::selectAllStateChanged);
        connect(checkbox.data(), &QCheckBox::stateChanged, this->gridView(), &OSGridView::gridRowSelectionChanged);

        addSelectColumn(Heading(QString(SELECTED), false, false, checkbox), "Check to select this row");
      }
      else if (field == EXTERIORLIGHTSDEFINITION) {
        std::function<boost::optional<model::ExteriorLightsDefinition>(model::ExteriorLights*)> get(
          [](model::ExteriorLights* el) {
          boost::optional<model::ExteriorLightsDefinition> optional = el->exteriorLightsDefinition();
          return optional;
        }
        );

        addDropZoneColumn(Heading(QString(EXTERIORLIGHTSDEFINITION)),
          get,
          CastNullAdapter<model::ExteriorLights>(&model::ExteriorLights::setExteriorLightsDefinition),
          boost::optional<std::function<void(model::ExteriorLights *)>>()
          );
      }
      else if (field == SCHEDULE) {

        std::function<bool(model::ExteriorLights*, const model::Schedule&)> set(
          [](model::ExteriorLights* el, const model::Schedule& s) {
          model::Schedule copy = s;
          return el->setSchedule(copy);
        }
        );

        addDropZoneColumn(Heading(QString(SCHEDULE)),
          CastNullAdapter<model::ExteriorLights>(&model::ExteriorLights::schedule),
          set,
          boost::optional<std::function<void(model::ExteriorLights*)>>(CastNullAdapter<model::ExteriorLights>(&model::ExteriorLights::resetSchedule))
        );
      }
      else if (field == CONTROLOPTION){
        addComboBoxColumn<std::string, model::ExteriorLights>(
          Heading(QString(CONTROLOPTION)),
          static_cast<std::string(*)(const std::string&)>(&openstudio::toString),
          std::function<std::vector<std::string>()>(&model::ExteriorLights::controlOptionValues),
          CastNullAdapter<model::ExteriorLights>(&model::ExteriorLights::controlOption),
          CastNullAdapter<model::ExteriorLights>(&model::ExteriorLights::setControlOption),
          boost::optional<std::function<void(model::ExteriorLights *)> >(),
          boost::optional<DataSource>()
          );
      }
      else if (field == MULTIPLIER){  
        addValueEditColumn(Heading(QString(MULTIPLIER)),
          NullAdapter(&model::ExteriorLights::multiplier),
          NullAdapter(&model::ExteriorLights::setMultiplier)
          );
      }
      else if (field == ENDUSESUBCATEGORY){
        addValueEditColumn(Heading(QString(ENDUSESUBCATEGORY)),
          CastNullAdapter<model::ExteriorLights>(&model::ExteriorLights::endUseSubcategory),
          CastNullAdapter<model::ExteriorLights>(&model::ExteriorLights::setEndUseSubcategory)
          );
      }
      else {
        // unhandled
        OS_ASSERT(false);
      }
    }
  }

  QString FacilityExteriorEquipmentGridController::getColor(const model::ModelObject & modelObject)
  {
    QColor defaultColor(Qt::lightGray);
    return defaultColor.name();
  }

  void FacilityExteriorEquipmentGridController::checkSelectedFields()
  {
    if (!this->m_hasHorizontalHeader) return;

    OSGridController::checkSelectedFields();
  }

  void FacilityExteriorEquipmentGridController::onItemDropped(const OSItemId& itemId)
  {
  }

  void FacilityExteriorEquipmentGridController::refreshModelObjects()
  {
    m_modelObjects = subsetCastVector<model::ModelObject>(m_model.getModelObjects<model::ExteriorLights>());
    std::sort(m_modelObjects.begin(), m_modelObjects.end(), ModelObjectNameSorter());
  }

  void FacilityExteriorEquipmentGridController::onComboBoxIndexChanged(int index)
  {
  }

} // openstudio
