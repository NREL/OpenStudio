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

#include "SpacesSurfacesGridView.hpp"

#include "OSDropZone.hpp"
#include "OSItemSelectorButtons.hpp"

#include "../shared_gui_components/OSGridController.hpp"
#include "../shared_gui_components/OSGridView.hpp"

#include "../model/ConstructionBase.hpp"
#include "../model/ConstructionBase_Impl.hpp"
#include "../model/ShadingSurfaceGroup.hpp"
#include "../model/ShadingSurfaceGroup_Impl.hpp"
#include "../model/Space.hpp"
#include "../model/Space_Impl.hpp"
#include "../model/Surface.hpp"
#include "../model/Surface_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/idd/IddEnums.hxx"
#include "../utilities/idd/OS_Space_FieldEnums.hxx"

#include <QCheckBox>

// These defines provide a common area for field display names
// used on column headers, and other grid widgets

#define NAME "Space Name"
#define SELECTED "All"

// GENERAL
#define SURFACENAME "Surface Name"
#define SURFACETYPE "Surface Type"
#define CONSTRUCTION "Construction"
#define OUTSIDEBOUNDARYCONDITION "Outside Boundary Condition" // Dan note: cannot chose Surface if not already surface
#define OUTSIDEBOUNDARYCONDITIONOBJECT "Outside Boundary Condition Object" // read only
#define SUNEXPOSURE "Sun Exposure"
#define WINDEXPOSURE "Wind Exposure"
#define SHADINGSURFACENAME "Shading Surface Name" // read only

namespace openstudio {

  struct ModelObjectNameSorter{
    // sort by name
    bool operator()(const model::ModelObject & lhs, const model::ModelObject & rhs){
      return (lhs.name() < rhs.name());
    }
  };

  SpacesSurfacesGridView::SpacesSurfacesGridView(bool isIP, const model::Model & model, QWidget * parent)
    : SpacesSubtabGridView(isIP, model, parent)
  {
    showStoryFilter();
    showThermalZoneFilter();
    showSpaceTypeFilter();
    showSurfaceTypeFilter();
    showOutsideBoundaryConditionFilter();
    showSunExposureFilter();
    showWindExposureFilter();

    m_filterGridLayout->setRowStretch(m_filterGridLayout->rowCount(), 100);
    m_filterGridLayout->setColumnStretch(m_filterGridLayout->columnCount(), 100);

    m_gridController = new SpacesSurfacesGridController(isIP, "Space", IddObjectType::OS_Space, model, m_spacesModelObjects);
    m_gridView = new OSGridView(m_gridController, "Space", "Drop\nSpace", false, parent);

    setGridController(m_gridController);
    setGridView(m_gridView);

    m_gridView->m_contentLayout->addLayout(m_filterGridLayout);
    m_gridView->m_contentLayout->addSpacing(7);
    m_gridView->m_dropZone->hide();

    onClearSelection();
  }

  void SpacesSurfacesGridView::onSelectItem()
  {
    //m_itemSelectorButtons->enableAddButton();
    //m_itemSelectorButtons->enableCopyButton();
    //m_itemSelectorButtons->enableRemoveButton();
    //m_itemSelectorButtons->enablePurgeButton();
  }

  void SpacesSurfacesGridView::onClearSelection()
  {
    m_itemSelectorButtons->disableAddButton();
    //m_itemSelectorButtons->disableCopyButton();
    //m_itemSelectorButtons->disableRemoveButton();
    m_itemSelectorButtons->disablePurgeButton();
  }

  SpacesSurfacesGridController::SpacesSurfacesGridController(bool isIP,
    const QString & headerText,
    IddObjectType iddObjectType,
    model::Model model,
    std::vector<model::ModelObject> modelObjects) :
    OSGridController(isIP, headerText, iddObjectType, model, modelObjects)
  {
    setCategoriesAndFields();
  }

  void SpacesSurfacesGridController::setCategoriesAndFields()
  {
    {
      std::vector<QString> fields;
      fields.push_back(SURFACENAME);
      fields.push_back(SURFACETYPE); 
      fields.push_back(CONSTRUCTION);
      fields.push_back(OUTSIDEBOUNDARYCONDITION);
      fields.push_back(OUTSIDEBOUNDARYCONDITIONOBJECT); 
      fields.push_back(SUNEXPOSURE);
      fields.push_back(WINDEXPOSURE);
      //fields.push_back(SHADINGSURFACENAME); // UNDESIRABLE TO SHOW THIS VECTOR IN THIS VIEW
      std::pair<QString, std::vector<QString> > categoryAndFields = std::make_pair(QString("General"), fields);
      m_categoriesAndFields.push_back(categoryAndFields);
    }

    OSGridController::setCategoriesAndFields();
  }

  void SpacesSurfacesGridController::categorySelected(int index)
  {
    OSGridController::categorySelected(index);
  }

  void SpacesSurfacesGridController::addColumns(const QString &category, std::vector<QString> & fields)
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

        std::function<std::vector<model::ModelObject>(const model::Space &)> allSurfaces(
          [](const model::Space &t_space) {
          std::vector<model::ModelObject> allModelObjects;
          auto surfaces = t_space.surfaces();
          allModelObjects.insert(allModelObjects.end(), surfaces.begin(), surfaces.end());
          return allModelObjects;
        }
        );

        if (field == SELECTED) {
        auto checkbox = QSharedPointer<QCheckBox>(new QCheckBox());
        checkbox->setToolTip("Check to select all rows");
        connect(checkbox.data(), &QCheckBox::stateChanged, this, &SpacesSurfacesGridController::selectAllStateChanged);
        connect(checkbox.data(), &QCheckBox::stateChanged, this->gridView(), &OSGridView::gridRowSelectionChanged);

        addSelectColumn(Heading(QString(SELECTED), false, false, checkbox), "Check to select this row",
          DataSource(
          allSurfaces,
          true
          )
          );
        }
        else if (field == SURFACENAME) {
          addNameLineEditColumn(Heading(QString(NAME), true, false),
            false,
            false,
            CastNullAdapter<model::Surface>(&model::Surface::name),
            CastNullAdapter<model::Surface>(&model::Surface::setName),
            boost::optional<std::function<void(model::Surface *)>>(
            std::function<void(model::Surface *)>(
            [](model::Surface *t_s)
          {
            t_s->remove();
          }
            )
            ),
            DataSource(
            allSurfaces,
            true
            )
            );
        }
        else if (field == SURFACETYPE) {
          addComboBoxColumn(Heading(QString(SURFACETYPE)),
            std::function<std::string(const std::string &)>(static_cast<std::string(*)(const std::string&)>(&openstudio::toString)),
            std::function<std::vector<std::string>()>(&model::Surface::validSurfaceTypeValues),
            CastNullAdapter<model::Surface>(&model::Surface::surfaceType),
            CastNullAdapter<model::Surface>(&model::Surface::setSurfaceType),
            boost::optional<std::function<void(model::Surface*)>>(),
            boost::optional<std::function<bool(model::Surface*)>>(),
            DataSource(
            allSurfaces,
            true
            )
            );
        }
        else if (field == CONSTRUCTION) {
          addDropZoneColumn(Heading(QString(CONSTRUCTION)),
            CastNullAdapter<model::Surface>(&model::Surface::construction),
            CastNullAdapter<model::Surface>(&model::Surface::setConstruction),
            boost::optional<std::function<void(model::Surface*)> >(NullAdapter(&model::Surface::resetConstruction)),
            DataSource(
            allSurfaces,
            true
            )
            );
        }
        else if (field == OUTSIDEBOUNDARYCONDITION) {
          addComboBoxColumn(Heading(QString(OUTSIDEBOUNDARYCONDITION)),
            std::function<std::string(const std::string &)>(static_cast<std::string(*)(const std::string&)>(&openstudio::toString)),
            std::function<std::vector<std::string>()>(&model::Surface::validOutsideBoundaryConditionValues),
            CastNullAdapter<model::Surface>(&model::Surface::outsideBoundaryCondition),
            CastNullAdapter<model::Surface>(&model::Surface::setOutsideBoundaryCondition),
            boost::optional<std::function<void(model::Surface*)>>(),
            boost::optional<std::function<bool(model::Surface*)>>(),
            DataSource(
            allSurfaces,
            true
            )
            );
        }
        else if (field == OUTSIDEBOUNDARYCONDITIONOBJECT) {
          std::function<bool(model::Surface *, const model::Surface &)> setter(
            [](model::Surface *t_surface, const model::Surface &t_arg) {
            auto copy = t_arg;
            return t_surface->setAdjacentSurface(copy);
          }
          );

          addDropZoneColumn(Heading(QString(OUTSIDEBOUNDARYCONDITIONOBJECT), true, false),
            CastNullAdapter<model::Surface>(&model::Surface::adjacentSurface),
            setter,
            boost::optional<std::function<void(model::Surface*)> >(NullAdapter(&model::Surface::resetAdjacentSurface)),
            DataSource(
            allSurfaces,
            true
            )
            );
        }
        else if (field == SUNEXPOSURE) {
          addComboBoxColumn(Heading(QString(SUNEXPOSURE)),
            std::function<std::string(const std::string &)>(static_cast<std::string(*)(const std::string&)>(&openstudio::toString)),
            std::function<std::vector<std::string>()>(&model::Surface::validSunExposureValues),
            CastNullAdapter<model::Surface>(&model::Surface::sunExposure),
            CastNullAdapter<model::Surface>(&model::Surface::setSunExposure),
            boost::optional<std::function<void(model::Surface*)> >(NullAdapter(&model::Surface::resetSunExposure)),
            boost::optional<std::function<bool(model::Surface*)>>(),
            DataSource(
            allSurfaces,
            true
            )
            );
        }
        else if (field == WINDEXPOSURE) {
          addComboBoxColumn(Heading(QString(WINDEXPOSURE)),
            std::function<std::string(const std::string &)>(static_cast<std::string(*)(const std::string&)>(&openstudio::toString)),
            std::function<std::vector<std::string>()>(&model::Surface::validWindExposureValues),
            CastNullAdapter<model::Surface>(&model::Surface::windExposure),
            CastNullAdapter<model::Surface>(&model::Surface::setWindExposure),
            boost::optional<std::function<void(model::Surface*)> >(NullAdapter(&model::Surface::resetWindExposure)),
            boost::optional<std::function<bool(model::Surface*)>>(),
            DataSource(
            allSurfaces,
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

  QString SpacesSurfacesGridController::getColor(const model::ModelObject & modelObject)
  {
    QColor defaultColor(Qt::lightGray);
    return defaultColor.name();
  }

  void SpacesSurfacesGridController::checkSelectedFields()
  {
    if (!this->m_hasHorizontalHeader) return;

    OSGridController::checkSelectedFields();
  }

  void SpacesSurfacesGridController::onItemDropped(const OSItemId& itemId)
  {
  }

  void SpacesSurfacesGridController::refreshModelObjects()
  {
    m_modelObjects = subsetCastVector<model::ModelObject>(m_model.getModelObjects<model::Space>());
    std::sort(m_modelObjects.begin(), m_modelObjects.end(), ModelObjectNameSorter());
  }

} // openstudio
