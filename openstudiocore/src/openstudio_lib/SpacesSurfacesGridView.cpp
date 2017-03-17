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
          m_constructionColumn = 4;
          addDropZoneColumn(Heading(QString(CONSTRUCTION)),
            CastNullAdapter<model::Surface>(&model::Surface::construction),
            CastNullAdapter<model::Surface>(&model::Surface::setConstruction),
            boost::optional<std::function<void(model::Surface*)> >(NullAdapter(&model::Surface::resetConstruction)),
            boost::optional<std::function<bool(model::Surface*)> >(NullAdapter(&model::Surface::isConstructionDefaulted)),
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
            boost::optional<std::function<bool(model::Surface*)> >(),
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
            boost::optional<std::function<bool(model::Surface*)> >(NullAdapter(&model::Surface::isSunExposureDefaulted)),
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
            boost::optional<std::function<bool(model::Surface*)> >(NullAdapter(&model::Surface::isWindExposureDefaulted)),
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
    m_modelObjects = subsetCastVector<model::ModelObject>(m_model.getConcreteModelObjects<model::Space>());
    std::sort(m_modelObjects.begin(), m_modelObjects.end(), ModelObjectNameSorter());
  }

} // openstudio
