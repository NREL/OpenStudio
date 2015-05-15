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

#include "SpacesShadingGridView.hpp"

#include "OSDropZone.hpp"

#include "../shared_gui_components/OSGridView.hpp"

#include "../model/Space.hpp"
#include "../model/Space_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/idd/IddEnums.hxx"
#include "../utilities/idd/OS_Space_FieldEnums.hxx"

#include <QCheckBox>

// These defines provide a common area for field display names
// used on column headers, and other grid widgets

#define NAME "Space Name"
#define SELECTED "All"

// GENERAL
#define SPACENAME "Space Name" // read only
#define SHADINGSURFACEGROUP "Shading Surface Group" // read only
#define CONSTRUCTION "Construction"
#define TRANSMITTANCESCHEDULE "Transmittance Schedule"
#define SHADEDSURFACENAME "Shaded Surface Name" // DAN note: need model method for suggestions
#define DAYLIGHTINGSHELFNAME "Daylighting Shelf Name" // read only

namespace openstudio {

  struct ModelObjectNameSorter{
    // sort by name
    bool operator()(const model::ModelObject & lhs, const model::ModelObject & rhs){
      return (lhs.name() < rhs.name());
    }
  };

  SpacesShadingGridView::SpacesShadingGridView(bool isIP, const model::Model & model, QWidget * parent)
    : SpacesSubtabGridView(isIP, model, parent)
  {
    showStoryFilter();
    showThermalZoneFilter();
    showSpaceTypeFilter();
    showSpaceNameFilter();

    m_filterGridLayout->setRowStretch(m_filterGridLayout->rowCount(), 100);
    m_filterGridLayout->setColumnStretch(m_filterGridLayout->columnCount(), 100);

    m_gridController = new SpacesShadingGridController(isIP, "Space", IddObjectType::OS_Space, model, m_spacesModelObjects);
    m_gridView = new OSGridView(m_gridController, "Space", "Drop\nSpace", false, parent);

    setGridController(m_gridController);
    setGridView(m_gridView);

    m_gridView->m_contentLayout->addLayout(m_filterGridLayout);
    m_gridView->m_contentLayout->addSpacing(7);
    m_gridView->m_dropZone->hide();
  }

  SpacesShadingGridController::SpacesShadingGridController(bool isIP,
    const QString & headerText,
    IddObjectType iddObjectType,
    model::Model model,
    std::vector<model::ModelObject> modelObjects) :
    OSGridController(isIP, headerText, iddObjectType, model, modelObjects)
  {
    setCategoriesAndFields();
  }

  void SpacesShadingGridController::setCategoriesAndFields()
  {
    {
      std::vector<QString> fields;
      //fields.push_back(SPACENAME);
      //fields.push_back(SHADINGSURFACEGROUP);
      //fields.push_back(CONSTRUCTION);  
      //fields.push_back(TRANSMITTANCESCHEDULE);
      //fields.push_back(SHADEDSURFACENAME);
      //fields.push_back(DAYLIGHTINGSHELFNAME);
      std::pair<QString, std::vector<QString> > categoryAndFields = std::make_pair(QString("General"), fields);
      m_categoriesAndFields.push_back(categoryAndFields);
    }

    OSGridController::setCategoriesAndFields();
  }

  void SpacesShadingGridController::categorySelected(int index)
  {
    OSGridController::categorySelected(index);
  }

  void SpacesShadingGridController::addColumns(const QString &category, std::vector<QString> & fields)
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
      else {
        if (field == SELECTED) {
          auto checkbox = QSharedPointer<QCheckBox>(new QCheckBox());
          checkbox->setToolTip("Check to select all rows");
          connect(checkbox.data(), &QCheckBox::stateChanged, this, &SpacesShadingGridController::selectAllStateChanged);

          addSelectColumn(Heading(QString(SELECTED), false, false, checkbox), "Check to select this row");
          //addSelectColumn(Heading(QString(SELECTED), false, false, checkbox), "Check to select this row",
          //  DataSource(
          //  allLoads,
          //  true
          //  )
          //  );
        }

        else if (field == SPACENAME) {

        }

        else if (field == SHADINGSURFACEGROUP) {

        }

        else if (field == CONSTRUCTION) {

        }

        else if (field == TRANSMITTANCESCHEDULE) {

        }

        else if (field == SHADEDSURFACENAME) {

        }

        else if (field == DAYLIGHTINGSHELFNAME) {

        }

        else {
          // unhandled
          OS_ASSERT(false);
        }
      }
    }
  }

  QString SpacesShadingGridController::getColor(const model::ModelObject & modelObject)
  {
    QColor defaultColor(Qt::lightGray);
    return defaultColor.name();
  }

  void SpacesShadingGridController::checkSelectedFields()
  {
    if (!this->m_hasHorizontalHeader) return;

    OSGridController::checkSelectedFields();
  }

  void SpacesShadingGridController::onItemDropped(const OSItemId& itemId)
  {
  }

  void SpacesShadingGridController::refreshModelObjects()
  {
    m_modelObjects = subsetCastVector<model::ModelObject>(m_model.getModelObjects<model::Space>());
    std::sort(m_modelObjects.begin(), m_modelObjects.end(), ModelObjectNameSorter());
  }

} // openstudio
