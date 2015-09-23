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
#include "OSItemSelectorButtons.hpp"

#include "../shared_gui_components/OSGridController.hpp"
#include "../shared_gui_components/OSGridView.hpp"

#include "../model/ConstructionBase.hpp"
#include "../model/ConstructionBase_Impl.hpp"
#include "../model/Schedule.hpp"
#include "../model/Schedule_Impl.hpp"
#include "../model/ShadingSurface.hpp"
#include "../model/ShadingSurface_Impl.hpp"
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

    onClearSelection();
  }

  void SpacesShadingGridView::onSelectItem()
  {
    //m_itemSelectorButtons->enableAddButton();
    //m_itemSelectorButtons->enableCopyButton();
    //m_itemSelectorButtons->enableRemoveButton();
    //m_itemSelectorButtons->enablePurgeButton();
  }

  void SpacesShadingGridView::onClearSelection()
  {
    m_itemSelectorButtons->disableAddButton();
    //m_itemSelectorButtons->disableCopyButton();
    //m_itemSelectorButtons->disableRemoveButton();
    m_itemSelectorButtons->disablePurgeButton();
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
      //fields.push_back(SHADEDSURFACENAME);
      fields.push_back(SHADINGSURFACEGROUP);
      fields.push_back(CONSTRUCTION);  
      fields.push_back(TRANSMITTANCESCHEDULE);
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

          std::function<std::vector<model::ModelObject>(const model::Space &)> allShadingSurfaceGroups(
            [](const model::Space &t_space) {
            std::vector<model::ModelObject> allModelObjects;
            auto shadingSurfaceGroups = t_space.shadingSurfaceGroups();
            allModelObjects.insert(allModelObjects.end(), shadingSurfaceGroups.begin(), shadingSurfaceGroups.end());
            return allModelObjects;
          }
          );

          std::function<std::vector<model::ModelObject>(const model::Space &)> allShadingSurfaces(
            [allShadingSurfaceGroups](const model::Space &t_space) {
            std::vector<model::ModelObject> allModelObjects;
            for (auto shadingSurfaceGroup : allShadingSurfaceGroups(t_space)) {
              auto shadingSurfaces = shadingSurfaceGroup.cast<model::ShadingSurfaceGroup>().shadingSurfaces();
              allModelObjects.insert(allModelObjects.end(), shadingSurfaces.begin(), shadingSurfaces.end());
            }
            return allModelObjects;
          }
          );

          std::function<std::vector<boost::optional<model::ModelObject> >(const model::Space &)> allShadingSurfaceShadingSurfaceGroups(
            [allShadingSurfaceGroups](const model::Space &t_space) {
            std::vector<boost::optional<model::ModelObject> > allModelObjects;
            for (auto shadingSurfaceGroup : allShadingSurfaceGroups(t_space)) {
              auto shadingSurfaces = shadingSurfaceGroup.cast<model::ShadingSurfaceGroup>().shadingSurfaces();
              for (auto shadingSurface : shadingSurfaces) {
                auto group = shadingSurface.shadingSurfaceGroup();
                if (group) {
                  allModelObjects.push_back(*group);
                }
                else {
                  allModelObjects.emplace_back();
                }
              }
            }
            return allModelObjects;
          }
          );

        if (field == SELECTED) {
          auto checkbox = QSharedPointer<QCheckBox>(new QCheckBox());
          checkbox->setToolTip("Check to select all rows");
          connect(checkbox.data(), &QCheckBox::stateChanged, this, &SpacesShadingGridController::selectAllStateChanged);
          connect(checkbox.data(), &QCheckBox::stateChanged, this->gridView(), &OSGridView::gridRowSelectionChanged);

          addSelectColumn(Heading(QString(SELECTED), false, false, checkbox), "Check to select this row",
            DataSource(
            allShadingSurfaceGroups,
            true
            )
            );
        }
        else if (field == SHADINGSURFACEGROUP) {
          addNameLineEditColumn(Heading(QString(SHADINGSURFACEGROUP), true, false),
            false,
            false,
            CastNullAdapter<model::ShadingSurfaceGroup>(&model::ShadingSurfaceGroup::name),
            CastNullAdapter<model::ShadingSurfaceGroup>(&model::ShadingSurfaceGroup::setName),
            boost::optional<std::function<void(model::ShadingSurfaceGroup *)>>(),
            DataSource(
            allShadingSurfaceShadingSurfaceGroups,
            true)
            );
        }
        else if (field == CONSTRUCTION) {
          addDropZoneColumn(Heading(QString(CONSTRUCTION)),
            CastNullAdapter<model::ShadingSurface>(&model::ShadingSurface::construction),
            CastNullAdapter<model::ShadingSurface>(&model::ShadingSurface::setConstruction),
            boost::optional<std::function<void(model::ShadingSurface*)> >(NullAdapter(&model::ShadingSurface::resetConstruction)),
            DataSource(
            allShadingSurfaces,
            true
            )
            );
        }
        else if (field == TRANSMITTANCESCHEDULE) {
          std::function<bool(model::ShadingSurface *, const model::Schedule &)> setter(
            [](model::ShadingSurface *t_shadingSurface, const model::Schedule &t_arg) {
            auto copy = t_arg;
            return t_shadingSurface->setTransmittanceSchedule(copy);
          }
          );

          addDropZoneColumn(Heading(QString(TRANSMITTANCESCHEDULE)),
            CastNullAdapter<model::ShadingSurface>(&model::ShadingSurface::transmittanceSchedule),
            setter,
            boost::optional<std::function<void(model::ShadingSurface*)> >(NullAdapter(&model::ShadingSurface::resetTransmittanceSchedule)),
            DataSource(
            allShadingSurfaces,
            true
            )
            );
        }
        else if (field == SHADEDSURFACENAME) {
          //ShadingSurfaceGroup
          //boost::optional<Surface> shadedSurface() const;
          //bool setShadedSurface(const Surface& surface);

        }
        else if (field == DAYLIGHTINGSHELFNAME) {
          //ShadingSurface
          //boost::optional<DaylightingDeviceShelf> daylightingDeviceShelf() const;
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
