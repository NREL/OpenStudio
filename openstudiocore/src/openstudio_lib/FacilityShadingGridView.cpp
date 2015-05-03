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

#include "../model/ShadingSurface.hpp"
#include "../model/ShadingSurface_Impl.hpp"
#include "../model/Schedule.hpp"
#include "../model/Schedule_Impl.hpp"
#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/ModelObject.hpp"
#include "../model/ModelObject_Impl.hpp"
#include "../model/ShadingSurfaceGroup.hpp"
#include "../model/ShadingSurfaceGroup_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/idd/IddEnums.hxx"
#include "../utilities/idd/OS_ShadingSurface_FieldEnums.hxx"

#include <QBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QScrollArea>

// These defines provide a common area for field display names
// used on column headers, and other grid widgets

#define NAME "Shading Surface Name"
#define SELECTED "All"

// GENERAL
#define SHADINGSURFACEGROUPNAME "Shading Surface Group Name" // (read only)
//#define NAME "Name"
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
    auto buildingShading = model.getModelObjects<model::ShadingSurface>();
    auto buildingShadingModelObjects = subsetCastVector<model::ModelObject>(buildingShading);

    m_gridController = new FacilityShadingGridController(isIP, "Shading Surface", IddObjectType::OS_ShadingSurface, model, buildingShadingModelObjects);
    auto gridView = new OSGridView(m_gridController, "Shading Surface", "Drop\nShading Surface", false, parent);

    setGridController(m_gridController);
    setGridView(gridView);

    // Filters

    gridView->m_contentLayout->addSpacing(7);
  }

  void FacilityShadingGridView::onDropZoneItemClicked(OSItem* item)
  {
  }

  void FacilityShadingGridView::addObject(const IddObjectType& iddObjectType)
  {
    if (IddObjectType::OS_ShadingSurface == iddObjectType.value()) {
      //model::ShadingSurface();
    }
  }

  void FacilityShadingGridView::purgeObjects(const IddObjectType& iddObjectType)
  {
    for (auto mo : this->m_model.getConcreteModelObjects<model::ShadingSurface>()){
      mo.remove();
    }
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
      //fields.push_back(SHADINGSURFACEGROUPNAME);
      //fields.push_back(NAME);
      //fields.push_back(CONSTRUCTIONNAME);
      //fields.push_back(TRANSMITTANCESCHEDULENAME);
      std::pair<QString, std::vector<QString> > categoryAndFields = std::make_pair(QString("General"), fields);
      m_categoriesAndFields.push_back(categoryAndFields);
    }

    OSGridController::setCategoriesAndFields();
  }

  void FacilityShadingGridController::filterChanged(const QString & text)
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

  //void FacilityShadingGridController::tiltFilterChanged();
  //void FacilityShadingGridController::orientationFilterChanged();
  //void FacilityShadingGridController::nameFilterChanged();
  //void FacilityShadingGridController::typeFilterChanged(const QString & text);

  FacilityShadingGridView * FacilityShadingGridController::gridView()
  {
    OS_ASSERT(qobject_cast<OSGridView *>(this->parent()));

    return qobject_cast<FacilityShadingGridView *>(this->parent()->parent());
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
          CastNullAdapter<model::ShadingSurface>(&model::ShadingSurface::name),
          CastNullAdapter<model::ShadingSurface>(&model::ShadingSurface::setName)
          );
      }
      else if (field == SELECTED) {
        auto checkbox = QSharedPointer<QCheckBox>(new QCheckBox());
        checkbox->setToolTip("Check to select all rows");
        connect(checkbox.data(), &QCheckBox::stateChanged, this, &FacilityShadingGridController::selectAllStateChanged);

        addSelectColumn(Heading(QString(SELECTED), false, false, checkbox), "Check to select this row");
      }
      else if (field == SHADINGSURFACEGROUPNAME) {

      }
      else if (field == CONSTRUCTIONNAME) {

      }
      else if (field == TRANSMITTANCESCHEDULENAME) {
        //addDropZoneColumn(Heading(QString(TRANSMITTANCESCHEDULENAME)),
        //  CastNullAdapter<model::ShadingSurface>(&model::ShadingSurface::transmittanceSchedule),
        //  CastNullAdapter<model::ShadingSurface>(&model::ShadingSurface::setTransmittanceSchedule),
        //  boost::optional<std::function<void(model::ShadingSurface*)>>(CastNullAdapter<model::ShadingSurface>(&model::ShadingSurface::resetTransmittanceSchedule))
        //  );
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
    auto exteriorLights = m_model.getModelObjects<model::ShadingSurface>();
    m_modelObjects = subsetCastVector<model::ModelObject>(exteriorLights);
    std::sort(m_modelObjects.begin(), m_modelObjects.end(), ModelObjectNameSorter());
  }

} // openstudio
