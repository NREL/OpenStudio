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

#include "SpacesInteriorPartitionsGridView.hpp"

#include "OSDropZone.hpp"
#include "OSItemSelectorButtons.hpp"

#include "../shared_gui_components/OSGridController.hpp"
#include "../shared_gui_components/OSGridView.hpp"

#include "../model/ConstructionBase.hpp"
#include "../model/ConstructionBase_Impl.hpp"
#include "../model/DaylightingDeviceShelf.hpp"
#include "../model/DaylightingDeviceShelf_Impl.hpp"
#include "../model/InteriorPartitionSurface.hpp"
#include "../model/InteriorPartitionSurface_Impl.hpp"
#include "../model/InteriorPartitionSurfaceGroup.hpp"
#include "../model/InteriorPartitionSurfaceGroup_Impl.hpp"
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
#define INTERIORPARTITIONGROUPNAME "Interior Partition Group Name" // read only
#define INTERIORPARTITIONNAME "Interior Partition Name"
#define CONSTRUCTIONNAME "Construction Name"
#define CONVERTTOINTERNALMASS "Convert to Internal Mass"
#define SURFACEAREA "Surface Area"
#define DAYLIGHTINGSHELFNAME "Daylighting Shelf Name" // read only

namespace openstudio {

  struct ModelObjectNameSorter{
    // sort by name
    bool operator()(const model::ModelObject & lhs, const model::ModelObject & rhs){
      return (lhs.name() < rhs.name());
    }
  };

  SpacesInteriorPartitionsGridView::SpacesInteriorPartitionsGridView(bool isIP, const model::Model & model, QWidget * parent)
    : SpacesSubtabGridView(isIP, model, parent)
  {
    showStoryFilter();
    showThermalZoneFilter();
    showSpaceTypeFilter();
    showInteriorPartitionGroupFilter();

    m_filterGridLayout->setRowStretch(m_filterGridLayout->rowCount(), 100);
    m_filterGridLayout->setColumnStretch(m_filterGridLayout->columnCount(), 100);

    m_gridController = new SpacesInteriorPartitionsGridController(isIP, "Space", IddObjectType::OS_Space, model, m_spacesModelObjects);
    m_gridView = new OSGridView(m_gridController, "Space", "Drop\nSpace", false, parent);

    setGridController(m_gridController);
    setGridView(m_gridView);

    m_gridView->m_contentLayout->addLayout(m_filterGridLayout);
    m_gridView->m_contentLayout->addSpacing(7);
    m_gridView->m_dropZone->hide();

    onClearSelection();
  }

  void SpacesInteriorPartitionsGridView::onSelectItem()
  {
    //m_itemSelectorButtons->enableAddButton();
    //m_itemSelectorButtons->enableCopyButton();
    //m_itemSelectorButtons->enableRemoveButton();
    //m_itemSelectorButtons->enablePurgeButton();
  }

  void SpacesInteriorPartitionsGridView::onClearSelection()
  {
    m_itemSelectorButtons->disableAddButton();
    //m_itemSelectorButtons->disableCopyButton();
    //m_itemSelectorButtons->disableRemoveButton();
    m_itemSelectorButtons->disablePurgeButton();
  }

  SpacesInteriorPartitionsGridController::SpacesInteriorPartitionsGridController(bool isIP,
    const QString & headerText,
    IddObjectType iddObjectType,
    model::Model model,
    std::vector<model::ModelObject> modelObjects) :
    OSGridController(isIP, headerText, iddObjectType, model, modelObjects)
  {
    setCategoriesAndFields();
  }

  void SpacesInteriorPartitionsGridController::setCategoriesAndFields()
  {
    {
      std::vector<QString> fields;
      fields.push_back(INTERIORPARTITIONNAME);
      fields.push_back(INTERIORPARTITIONGROUPNAME);
      fields.push_back(CONSTRUCTIONNAME); 
      fields.push_back(CONVERTTOINTERNALMASS);
      //fields.push_back(SURFACEAREA);
      //fields.push_back(DAYLIGHTINGSHELFNAME);
      std::pair<QString, std::vector<QString> > categoryAndFields = std::make_pair(QString("General"), fields);
      m_categoriesAndFields.push_back(categoryAndFields);
    }

    OSGridController::setCategoriesAndFields();
  }

  void SpacesInteriorPartitionsGridController::categorySelected(int index)
  {
    OSGridController::categorySelected(index);
  }

  void SpacesInteriorPartitionsGridController::addColumns(const QString &category, std::vector<QString> & fields)
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

          std::function<std::vector<model::ModelObject>(const model::Space &)> allInteriorPartitionSurfaceGroups(
            [](const model::Space &t_space) {
            std::vector<model::ModelObject> allModelObjects;
            auto interiorPartitionSurfaceGroups = t_space.interiorPartitionSurfaceGroups();
            allModelObjects.insert(allModelObjects.end(), interiorPartitionSurfaceGroups.begin(), interiorPartitionSurfaceGroups.end());
            return allModelObjects;
          }
          );

          std::function<std::vector<model::ModelObject>(const model::Space &)> allInteriorPartitionSurfaces(
            [allInteriorPartitionSurfaceGroups](const model::Space &t_space) {
            std::vector<model::ModelObject> allModelObjects;
            for (auto interiorPartitionSurfaceGroup : allInteriorPartitionSurfaceGroups(t_space)) {
              auto interiorPartitionSurfaces = interiorPartitionSurfaceGroup.cast<model::InteriorPartitionSurfaceGroup>().interiorPartitionSurfaces();
              for (auto interiorPartitionSurface : interiorPartitionSurfaces) {
                allModelObjects.push_back(interiorPartitionSurface);
              }
            }
            return allModelObjects;
          }
          );

          std::function<std::vector<boost::optional<model::ModelObject> >(const model::Space &)> allInteriorPartitionSurfaceInteriorPartitionSurfaceGroups(
            [allInteriorPartitionSurfaceGroups](const model::Space &t_space) {
            std::vector<boost::optional<model::ModelObject> > allModelObjects;
            for (auto interiorPartitionSurfaceGroup : allInteriorPartitionSurfaceGroups(t_space)) {
              auto interiorPartitionSurfaces = interiorPartitionSurfaceGroup.cast<model::InteriorPartitionSurfaceGroup>().interiorPartitionSurfaces();
              for (auto interiorPartitionSurface : interiorPartitionSurfaces) {
                auto group = interiorPartitionSurface.interiorPartitionSurfaceGroup();
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
          connect(checkbox.data(), &QCheckBox::stateChanged, this, &SpacesInteriorPartitionsGridController::selectAllStateChanged);
          connect(checkbox.data(), &QCheckBox::stateChanged, this->gridView(), &OSGridView::gridRowSelectionChanged);

          addSelectColumn(Heading(QString(SELECTED), false, false, checkbox), "Check to select this row",
            DataSource(
            allInteriorPartitionSurfaces,
            true
            )
            );
        }
        else if (field == INTERIORPARTITIONGROUPNAME) {
          addNameLineEditColumn(Heading(QString(INTERIORPARTITIONGROUPNAME), true, false),
            false,
            false,
            CastNullAdapter<model::InteriorPartitionSurfaceGroup>(&model::InteriorPartitionSurfaceGroup::name),
            CastNullAdapter<model::InteriorPartitionSurfaceGroup>(&model::InteriorPartitionSurfaceGroup::setName),
            boost::optional<std::function<void(model::InteriorPartitionSurfaceGroup *)>>(),
            DataSource(
            allInteriorPartitionSurfaceInteriorPartitionSurfaceGroups,
            true)
            );
        }
        else if (field == INTERIORPARTITIONNAME) {
          addNameLineEditColumn(Heading(QString(INTERIORPARTITIONNAME), true, false),
          false,
          false,
          CastNullAdapter<model::InteriorPartitionSurface>(&model::InteriorPartitionSurface::name),
          CastNullAdapter<model::InteriorPartitionSurface>(&model::InteriorPartitionSurface::setName),
          boost::optional<std::function<void(model::InteriorPartitionSurface *)>>(),
          DataSource(
          allInteriorPartitionSurfaces,
          true)
          );
        }
        else if (field == CONSTRUCTIONNAME) {
          addDropZoneColumn(Heading(QString(CONSTRUCTIONNAME), true, false),
            CastNullAdapter<model::InteriorPartitionSurface>(&model::InteriorPartitionSurface::construction),
            CastNullAdapter<model::InteriorPartitionSurface>(&model::InteriorPartitionSurface::setConstruction),
            boost::optional<std::function<void(model::InteriorPartitionSurface*)> >(NullAdapter(&model::InteriorPartitionSurface::resetConstruction)),
            DataSource(
            allInteriorPartitionSurfaces,
            true
            )
            );
        }
        else if (field == CONVERTTOINTERNALMASS) {
          addCheckBoxColumn(Heading(QString(CONVERTTOINTERNALMASS), true, false),
            std::string("Check to enable convert to InternalMass."),
            NullAdapter(&model::InteriorPartitionSurface::converttoInternalMass),
            NullAdapter(&model::InteriorPartitionSurface::setConverttoInternalMass),
            DataSource(
            allInteriorPartitionSurfaces,
            true
            )
            );
        }
        else if (field == SURFACEAREA) {
          std::function<bool(model::InteriorPartitionSurface *, double)> setter(
            [](model::InteriorPartitionSurface *t_interiorPartitionSurface, double t_arg) {
            return t_interiorPartitionSurface->setSurfaceArea(t_arg);
          }
          );

          addValueEditColumn(Heading(QString(SURFACEAREA)),
            CastNullAdapter<model::InteriorPartitionSurface>(&model::InteriorPartitionSurface::surfaceArea),
            setter//,
            //boost::optional<std::function<void(model::ModelObject *)>>(),
            //boost::optional<std::function<bool(model::ModelObject *)>>()//,
            //DataSource(
            //allInteriorPartitionSurfaces,
            //true
            //)
            );

          //boost::optional<double> surfaceArea() const; // TODO this optional is causing troubles
          //bool setSurfaceArea(boost::optional<double> surfaceArea);
          //bool setSurfaceArea(double surfaceArea);
          //void resetSurfaceArea();

        }
        else if (field == DAYLIGHTINGSHELFNAME) {
          //boost::optional<DaylightingDeviceShelf> daylightingDeviceShelf() const;

        }
        else {
          // unhandled
          OS_ASSERT(false);
        }
      }
    }
  }

  QString SpacesInteriorPartitionsGridController::getColor(const model::ModelObject & modelObject)
  {
    QColor defaultColor(Qt::lightGray);
    return defaultColor.name();
  }

  void SpacesInteriorPartitionsGridController::checkSelectedFields()
  {
    if (!this->m_hasHorizontalHeader) return;

    OSGridController::checkSelectedFields();
  }

  void SpacesInteriorPartitionsGridController::onItemDropped(const OSItemId& itemId)
  {
  }

  void SpacesInteriorPartitionsGridController::refreshModelObjects()
  {
    m_modelObjects = subsetCastVector<model::ModelObject>(m_model.getModelObjects<model::Space>());
    std::sort(m_modelObjects.begin(), m_modelObjects.end(), ModelObjectNameSorter());
  }

} // openstudio
