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

#include "SpacesSubsurfacesGridView.hpp"

#include "OSDropZone.hpp"
#include "OSItemSelectorButtons.hpp"

#include "../shared_gui_components/OSGridView.hpp"

#include "../model/ConstructionBase.hpp"
#include "../model/ConstructionBase_Impl.hpp"
#include "../model/DaylightingDeviceShelf.hpp"
#include "../model/DaylightingDeviceShelf_Impl.hpp"
#include "../model/ShadingControl.hpp"
#include "../model/ShadingControl_Impl.hpp"
#include "../model/Space.hpp"
#include "../model/Space_Impl.hpp"
#include "../model/Surface.hpp"
#include "../model/Surface_Impl.hpp"
#include "../model/SubSurface.hpp"
#include "../model/SubSurface_Impl.hpp"
#include "../model/WindowPropertyFrameAndDivider.hpp"
#include "../model/WindowPropertyFrameAndDivider_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/idd/IddEnums.hxx"
#include "../utilities/idd/OS_Space_FieldEnums.hxx"

#include <QCheckBox>

// These defines provide a common area for field display names
// used on column headers, and other grid widgets

#define NAME "Space Name"
#define SELECTED "All"

// ALL GRID BUTTONS
#define SURFACENAME "Surface Name" // read only
#define SUBSURFACENAME "Subsurface Name"

// GENERAL "General"
#define SUBSURFACETYPE "Subsurface Type"
#define MULTIPLIER "Multiplier"
#define CONSTRUCTION "Construction"
#define OUTSIDEBOUNDARYCONDITIONOBJECT "Outside Boundary Condition Object" // Dan note: needs model method for suggestions
#define SHADINGSURFACENAME "Shading Surface Name" // read only

// SHADINGCONTROLS "Shading Controls"
#define SHADINGTYPE "Shading Type"
#define CONSTRUCTIONWITHSHADINGNAME "Construction with Shading Name"
#define SHADINGDEVICEMATERIALNAME "Shading Device Material Name"
#define SHADINGCONTROLTYPE "Shading Control Type"
#define SCHEDULENAME "Schedule Name"
#define SETPOINT "Setpoint"
#define SHADINGCONTROLISSCHEDULED "Shading Control Is Scheduled"
#define GLARECONTROLISACTIVE "Glare Control Is Active"
#define TYPEOFSLATANGLECONTROLFORBLINDS "Type of Slat Angle Control for Blinds"
#define SLATANGLESCHEDULENAME "Slat Angle Schedule Name"
#define SETPOINT2 "Setpoint 2"

// FRAMEANDDIVIDER "Frame and Divider" 
#define FRAMEWIDTH "Frame Width"
#define FRAMEOUTSIDEPROJECTION "Frame Outside Projection"
#define FRAMEINSIDEPROJECTION "Frame Inside Projection"
#define FRAMECONDUCTANCE "Frame Conductance"
#define FRAMEEDGEGLASSCONDUCTANCETOCENTEROFGLASSCONDUCTANCE "Frame - Edge Glass Conductance to Center - Of - Glass Conductance"
#define FRAMESOLARABSORPTANCE "Frame Solar Absorptance"
#define FRAMEVISIBLEABSORPTANCE "Frame Visible Absorptance"
#define FRAMETHERMALHEMISPHERICALEMISSIVITY "Frame Thermal Hemispherical Emissivity"
#define DIVIDERTYPE "Divider Type"
#define DIVIDERWIDTH "Divider Width"
#define NUMBEROFHORIZONTALDIVIDERS "Number of Horizontal Dividers"
#define NUMBEROFVERTICALDIVIDERS "Number of Vertical Dividers"
#define DIVIDEROUTSIDEPROJECTION "Divider Outside Projection"
#define DIVIDERINSIDEPROJECTION "Divider Inside Projection"
#define DIVIDERCONDUCTANCE "Divider Conductance"
#define RATIOOFDIVIDEREDGEGLASSCONDUCTANCETOCENTEROFGLASSCONDUCTANCE "Ratio of Divider - Edge Glass Conductance to Center - Of - Glass Conductance"
#define DIVIDERSOLARABSORPTANCE "Divider Solar Absorptance"
#define DIVIDERVISIBLEABSORPTANCE "Divider Visible Absorptance"
#define DIVIDERTHERMALHEMISPHERICALEMISSIVITY "Divider Thermal Hemispherical Emissivity"
#define OUTSIDEREVEALDEPTH "Outside Reveal Depth"
#define OUTSIDEREVEALSOLARABSORPTANCE "Outside Reveal Solar Absorptance"
#define INSIDESILLDEPTH "Inside Sill Depth"
#define INSIDESILLSOLARABSORPTANCE "Inside Sill Solar Absorptance"
#define INSIDEREVEALDEPTH "Inside Reveal Depth"
#define INSIDEREVEALSOLARABSORPTANCE "Inside Reveal Solar Absorptance"

// DAYLIGHTINGSHELVES "Daylighting Shelves"
#define SPACENAME "Space Name" // read only
#define SURFACENAME "Surface Name" // read only
#define WINDOWNAME "Window Name"
#define INSIDESHELFNAME "Inside Shelf Name" // Dan note: drop down need a model method for suggestions
#define OUTSIDESHELFNAME "Outside Shelf Name" // Dan note: drop down need a model method for suggestions
#define VIEWFACTORTOOUTSIDESHELF "View Factor to Outside Shelf"

namespace openstudio {

  struct ModelObjectNameSorter{
    // sort by name
    bool operator()(const model::ModelObject & lhs, const model::ModelObject & rhs){
      return (lhs.name() < rhs.name());
    }
  };

  SpacesSubsurfacesGridView::SpacesSubsurfacesGridView(bool isIP, const model::Model & model, QWidget * parent)
    : SpacesSubtabGridView(isIP, model, parent)
  {
    showStoryFilter();
    showThermalZoneFilter();
    showSpaceTypeFilter();
    showOutsideBoundaryConditionFilter();
    showSpaceNameFilter();
    
    m_filterGridLayout->setRowStretch(m_filterGridLayout->rowCount(), 100);
    m_filterGridLayout->setColumnStretch(m_filterGridLayout->columnCount(), 100);

    m_gridController = new SpacesSubsurfacesGridController(isIP, "Space", IddObjectType::OS_Space, model, m_spacesModelObjects);
    m_gridView = new OSGridView(m_gridController, "Space", "Drop\nSpace", false, parent);

    setGridController(m_gridController);
    setGridView(m_gridView);

    m_gridView->m_contentLayout->addLayout(m_filterGridLayout);
    m_gridView->m_contentLayout->addSpacing(7);
    m_gridView->m_dropZone->hide();
  }

  SpacesSubsurfacesGridController::SpacesSubsurfacesGridController(bool isIP,
    const QString & headerText,
    IddObjectType iddObjectType,
    model::Model model,
    std::vector<model::ModelObject> modelObjects) :
    OSGridController(isIP, headerText, iddObjectType, model, modelObjects)
  {
    setCategoriesAndFields();
  }

  void SpacesSubsurfacesGridView::onSelectItem(OSItem* item)
  {
    //m_itemSelectorButtons->enableAddButton();
    //m_itemSelectorButtons->enableCopyButton();
    m_itemSelectorButtons->enableRemoveButton();
    m_itemSelectorButtons->enablePurgeButton();
  }

  void SpacesSubsurfacesGridView::onClearSelection()
  {
    //m_itemSelectorButtons->disableAddButton();
    //m_itemSelectorButtons->disableCopyButton();
    m_itemSelectorButtons->disableRemoveButton();
    m_itemSelectorButtons->disablePurgeButton();
  }

  void SpacesSubsurfacesGridController::setCategoriesAndFields()
  {
    {
      std::vector<QString> fields;
      fields.push_back(SURFACENAME);
      fields.push_back(SUBSURFACENAME);
      fields.push_back(SUBSURFACETYPE);
      //fields.push_back(MULTIPLIER);
      //fields.push_back(CONSTRUCTION);
      fields.push_back(OUTSIDEBOUNDARYCONDITIONOBJECT);
      //fields.push_back(SHADINGSURFACENAME);
      std::pair<QString, std::vector<QString> > categoryAndFields = std::make_pair(QString("General"), fields);
      m_categoriesAndFields.push_back(categoryAndFields);
    }

    {
      std::vector<QString> fields;
      fields.push_back(SURFACENAME);
      fields.push_back(SUBSURFACENAME);
      //fields.push_back(NAME);
      //fields.push_back(SHADINGTYPE);
      //fields.push_back(CONSTRUCTIONWITHSHADINGNAME);
      //fields.push_back(SHADINGDEVICEMATERIALNAME);
      //fields.push_back(SHADINGCONTROLTYPE);
      //fields.push_back(SCHEDULENAME);
      //fields.push_back(SETPOINT);                        IN IDD, BUT NOT EXPOSED IN MODEL OBJECT
      //fields.push_back(SHADINGCONTROLISSCHEDULED);       IN IDD, BUT NOT EXPOSED IN MODEL OBJECT
      //fields.push_back(GLARECONTROLISACTIVE);            IN IDD, BUT NOT EXPOSED IN MODEL OBJECT
      //fields.push_back(TYPEOFSLATANGLECONTROLFORBLINDS); IN IDD, BUT NOT EXPOSED IN MODEL OBJECT
      //fields.push_back(SLATANGLESCHEDULENAME);           IN IDD, BUT NOT EXPOSED IN MODEL OBJECT
      //fields.push_back(SETPOINT2);                       IN IDD, BUT NOT EXPOSED IN MODEL OBJECT
      std::pair<QString, std::vector<QString> > categoryAndFields = std::make_pair(QString("Shading Controls"), fields);
      m_categoriesAndFields.push_back(categoryAndFields);
    }

    {
      std::vector<QString> fields;
      fields.push_back(SURFACENAME);  
      fields.push_back(SUBSURFACENAME);
      //fields.push_back(NAME);
      fields.push_back(FRAMEWIDTH);
      fields.push_back(FRAMEOUTSIDEPROJECTION);
      fields.push_back(FRAMEINSIDEPROJECTION);
      //fields.push_back(FRAMECONDUCTANCE);
      fields.push_back(FRAMEEDGEGLASSCONDUCTANCETOCENTEROFGLASSCONDUCTANCE);
      fields.push_back(FRAMESOLARABSORPTANCE);
      fields.push_back(FRAMEVISIBLEABSORPTANCE);
      fields.push_back(FRAMETHERMALHEMISPHERICALEMISSIVITY);
      //fields.push_back(DIVIDERTYPE);
      fields.push_back(DIVIDERWIDTH);
      fields.push_back(NUMBEROFHORIZONTALDIVIDERS);
      fields.push_back(NUMBEROFVERTICALDIVIDERS);
      fields.push_back(DIVIDEROUTSIDEPROJECTION);
      fields.push_back(DIVIDERINSIDEPROJECTION);
      fields.push_back(DIVIDERCONDUCTANCE);
      fields.push_back(RATIOOFDIVIDEREDGEGLASSCONDUCTANCETOCENTEROFGLASSCONDUCTANCE);
      fields.push_back(DIVIDERSOLARABSORPTANCE);
      fields.push_back(DIVIDERVISIBLEABSORPTANCE);
      fields.push_back(DIVIDERTHERMALHEMISPHERICALEMISSIVITY);
      fields.push_back(OUTSIDEREVEALDEPTH);
      fields.push_back(OUTSIDEREVEALSOLARABSORPTANCE);
      fields.push_back(INSIDESILLDEPTH);
      fields.push_back(INSIDESILLSOLARABSORPTANCE);
      fields.push_back(INSIDEREVEALDEPTH);
      fields.push_back(INSIDEREVEALSOLARABSORPTANCE);
      std::pair<QString, std::vector<QString> > categoryAndFields = std::make_pair(QString("Frame and Divider"), fields);
      m_categoriesAndFields.push_back(categoryAndFields);
    }

    {
      std::vector<QString> fields;
      fields.push_back(SURFACENAME);
      //fields.push_back(WINDOWNAME);
      //fields.push_back( DaylightingDeviceShelfNAME);
      //fields.push_back(INSIDESHELFNAME);
      //fields.push_back(OUTSIDESHELFNAME);
      fields.push_back(VIEWFACTORTOOUTSIDESHELF);
      std::pair<QString, std::vector<QString> > categoryAndFields = std::make_pair(QString("Daylighting Shelves"), fields);
      m_categoriesAndFields.push_back(categoryAndFields);
    }

    OSGridController::setCategoriesAndFields();
  }

  void SpacesSubsurfacesGridController::categorySelected(int index)
  {
    OSGridController::categorySelected(index);
  }

  void SpacesSubsurfacesGridController::addColumns(const QString &category, std::vector<QString> & fields)
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
        
        std::function<std::vector<model::ModelObject>(const model::Space &)> allSubSurfaces(
          [allSurfaces](const model::Space &t_space) {
          std::vector<model::ModelObject> allModelObjects;
          for (auto surface : allSurfaces(t_space)) {
            auto subSurfaces = surface.cast<model::Surface>().subSurfaces();
            allModelObjects.insert(allModelObjects.end(), subSurfaces.begin(), subSurfaces.end());
          }
          return allModelObjects;
        }
        );
        
        std::function<std::vector<boost::optional<model::ModelObject> >(const model::Space &)> allDaylightingDeviceShelfs(
          [allSubSurfaces](const model::Space &t_space) {
          std::vector<boost::optional<model::ModelObject> > allModelObjects;
          std::vector<boost::optional<model::DaylightingDeviceShelf> > allDaylightingDeviceShelfs;
          for (auto subSurface : allSubSurfaces(t_space)) {
            auto daylightingDeviceShelf = subSurface.cast<model::SubSurface>().daylightingDeviceShelf();
            if (daylightingDeviceShelf) {
              allDaylightingDeviceShelfs.push_back(daylightingDeviceShelf);
            }
            else {
              allDaylightingDeviceShelfs.push_back(boost::optional<model::DaylightingDeviceShelf>());
            }
          }
          allModelObjects.insert(allModelObjects.end(), allDaylightingDeviceShelfs.begin(), allDaylightingDeviceShelfs.end());

          return allModelObjects;
        }
        );

        std::function<std::vector<boost::optional<model::ModelObject> >(const model::Space &)> allWindowPropertyFrameAndDividers(
          [allSubSurfaces](const model::Space &t_space) {
          std::vector<boost::optional<model::ModelObject> > allModelObjects;
          std::vector<boost::optional<model::WindowPropertyFrameAndDivider> > allWindowPropertyFrameAndDividers;
          for (auto subSurface : allSubSurfaces(t_space)) {
            auto windowPropertyFrameAndDivider = subSurface.cast<model::SubSurface>().windowPropertyFrameAndDivider();
            if (windowPropertyFrameAndDivider) {
              allWindowPropertyFrameAndDividers.push_back(windowPropertyFrameAndDivider);
            }
            else {
              allWindowPropertyFrameAndDividers.push_back(boost::optional<model::WindowPropertyFrameAndDivider>());
            }
          }
          allModelObjects.insert(allModelObjects.end(), allWindowPropertyFrameAndDividers.begin(), allWindowPropertyFrameAndDividers.end());

          return allModelObjects;
        }
        );

        std::function<std::vector<boost::optional<model::ModelObject> >(const model::Space &)> allShadingControls(
          [allSubSurfaces](const model::Space &t_space) {
          std::vector<boost::optional<model::ModelObject> > allModelObjects;
          std::vector<boost::optional<model::ShadingControl> > allShadingControls;
          for (auto subSurface : allSubSurfaces(t_space)) {
            auto shadingControl = subSurface.cast<model::SubSurface>().shadingControl();
            if (shadingControl) {
              allShadingControls.push_back(shadingControl);
            }
            else {
              allShadingControls.push_back(boost::optional<model::ShadingControl>());
            }
          }
          allModelObjects.insert(allModelObjects.end(), allShadingControls.begin(), allShadingControls.end());

          return allModelObjects;
        }
        );

        std::function<std::vector<boost::optional<model::ModelObject> >(const model::Space &)> allConstructions(
          [allSubSurfaces](const model::Space &t_space) {
          std::vector<boost::optional<model::ModelObject> > allModelObjects;
          std::vector<boost::optional<model::ConstructionBase> > allConstructions;
          for (auto subSurface : allSubSurfaces(t_space)) {
            auto construction = subSurface.cast<model::SubSurface>().construction();
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

        std::function<std::vector<boost::optional<model::ModelObject> >(const model::Space &)> allOutsideBoundaryConditionObjects(
          [allSubSurfaces](const model::Space &t_space) {
          std::vector<boost::optional<model::ModelObject> > allModelObjects;
          std::vector<boost::optional<model::SubSurface> > allAdjacentSubSurfaces;
          for (auto subSurface : allSubSurfaces(t_space)) {
            auto adjacentSubSurface = subSurface.cast<model::SubSurface>().adjacentSubSurface();
            if (adjacentSubSurface) {
              allAdjacentSubSurfaces.push_back(adjacentSubSurface);
            }
            else {
              allAdjacentSubSurfaces.push_back(boost::optional<model::SubSurface>());
            }
          }
          allModelObjects.insert(allModelObjects.end(), allAdjacentSubSurfaces.begin(), allAdjacentSubSurfaces.end());

          return allModelObjects;
        }
        );

        if (field == SELECTED) {
          auto checkbox = QSharedPointer<QCheckBox>(new QCheckBox());
          checkbox->setToolTip("Check to select all rows");
          connect(checkbox.data(), &QCheckBox::stateChanged, this, &SpacesSubsurfacesGridController::selectAllStateChanged);

          addSelectColumn(Heading(QString(SELECTED), false, false, checkbox), "Check to select this row",
            DataSource(
            allSubSurfaces,
            true
            )
            );
        }

        //else if (field == SPACENAME) {

        //}

        else if (field == SURFACENAME) {
          addNameLineEditColumn(Heading(QString(NAME), false, false),
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

        else if (field == SUBSURFACENAME) {

          addNameLineEditColumn(Heading(QString(SUBSURFACENAME), false, false),
            false,
            false,
            CastNullAdapter<model::SubSurface>(&model::SubSurface::name),
            CastNullAdapter<model::SubSurface>(&model::SubSurface::setName),
            boost::optional<std::function<void(model::SubSurface *)>>(
            std::function<void(model::SubSurface *)>(
            [](model::SubSurface *t_s)
          {
            t_s->remove();
          }
            )
            ),
            DataSource(
            allSubSurfaces,
            true
            )
            );
        }

        else if (field == SUBSURFACETYPE) {
          addComboBoxColumn(Heading(QString(SUBSURFACETYPE)),
            std::function<std::string(const std::string &)>(static_cast<std::string(*)(const std::string&)>(&openstudio::toString)),
            std::function<std::vector<std::string>()>(&model::SubSurface::validSubSurfaceTypeValues),
            CastNullAdapter<model::SubSurface>(&model::SubSurface::subSurfaceType),
            CastNullAdapter<model::SubSurface>(&model::SubSurface::setSubSurfaceType),
            boost::optional<std::function<void(model::SubSurface*)>>(),
            boost::optional<std::function<bool(model::SubSurface*)>>(),
            DataSource(
            allSubSurfaces,
            true
            )
            );
        }

        else if (field == MULTIPLIER) {

          //addValueEditColumn(Heading(QString(MULTIPLIER)),
          //  NullAdapter(&model::SubSurface::multiplier),
          //  NullAdapter(&model::SubSurface::setMultiplier),
          //  boost::optional<std::function<void(model::ModelObject *)>>(),
          //  boost::optional<std::function<bool(model::ModelObject *)>>(),
          //  DataSource(
          //  allSubSurfaces,
          //  true
          //  )
          //  );
        }

        else if (field == CONSTRUCTION) {
          //addDropZoneColumn(Heading(QString(CONSTRUCTION)),
          //  CastNullAdapter<model::SubSurface>(&model::SubSurface::construction),
          //  CastNullAdapter<model::SubSurface>(&model::SubSurface::setConstruction),
          //  boost::optional<std::function<void(model::SubSurface*)> >(NullAdapter(&model::SubSurface::resetConstruction)),
          //  DataSource(
          //  allConstructions, 
          //  true
          //  )
          //  );
        }

        else if (field == OUTSIDEBOUNDARYCONDITIONOBJECT) {

          std::function<bool(model::SubSurface *, const model::SubSurface &)> setter(
            [](model::SubSurface *t_subSurface, const model::SubSurface &t_arg) {
            auto copy = t_arg;
            return t_subSurface->setAdjacentSubSurface(copy);
          }
          );

          addDropZoneColumn(Heading(QString(OUTSIDEBOUNDARYCONDITIONOBJECT)),
            CastNullAdapter<model::SubSurface>(&model::SubSurface::adjacentSubSurface),
            setter,
            boost::optional<std::function<void(model::SubSurface*)> >(NullAdapter(&model::SubSurface::resetAdjacentSubSurface)),
            DataSource(
            allOutsideBoundaryConditionObjects,
            true
            )
            );

        }

        else if (field == SHADINGSURFACENAME) {

        }

        else if (field == SPACENAME) {

        }

        else if (field == SURFACENAME) {

        }

        else if (field == SUBSURFACENAME) {

        }

        else if (field == NAME) {

        }

        else if (field == SHADINGTYPE) {
          // ShadingControl
          //std::string shadingType() const;
          //bool setShadingType(const std::string& shadingType);
          //static std::vector<std::string> shadingTypeValues();

        }

        else if (field == CONSTRUCTIONWITHSHADINGNAME) {
          // ShadingControl
          //boost::optional<Construction> construction() const;
        }

        else if (field == SHADINGDEVICEMATERIALNAME) {
          // ShadingControl
          //boost::optional<ShadingMaterial> shadingMaterial() const;
        }

        else if (field == SHADINGCONTROLTYPE) {
          // ShadingControl
          //std::string shadingControlType() const;
          //bool setShadingControlType(const std::string& shadingControlType);
          //static std::vector<std::string> shadingControlTypeValues();
        }

        else if (field == SCHEDULENAME) {
          // ShadingControl
          //boost::optional<Schedule> schedule() const;
          //bool setSchedule(const Schedule& schedule);
          //void resetSchedule();

        }

        else if (field == SETPOINT) {

        }

        else if (field == SHADINGCONTROLISSCHEDULED) {

        }

        else if (field == GLARECONTROLISACTIVE) {

        }

        else if (field == TYPEOFSLATANGLECONTROLFORBLINDS) {

        }

        else if (field == SLATANGLESCHEDULENAME) {

        }

        else if (field == SETPOINT2) {

        }

        else if (field == SPACENAME) {

        }

        else if (field == SURFACENAME) {

        }

        else if (field == SUBSURFACENAME) {

        }

        else if (field == NAME) {

        }

        else if (field == FRAMEWIDTH) {
          // WindowPropertyFrameAndDivider
          //double frameWidth() const;
          //bool isFrameWidthDefaulted() const;
          //bool setFrameWidth(double frameWidth);
          //void resetFrameWidth();

          addValueEditColumn(Heading(QString(INSIDEREVEALSOLARABSORPTANCE)),
            NullAdapter(&model::WindowPropertyFrameAndDivider::insideRevealSolarAbsorptance),
            NullAdapter(&model::WindowPropertyFrameAndDivider::setInsideRevealSolarAbsorptance),
            boost::optional<std::function<void(model::WindowPropertyFrameAndDivider*)>>(CastNullAdapter<model::WindowPropertyFrameAndDivider>(&model::WindowPropertyFrameAndDivider::resetInsideRevealSolarAbsorptance)),
            boost::optional<std::function<bool(model::WindowPropertyFrameAndDivider*)>>(CastNullAdapter<model::WindowPropertyFrameAndDivider>(&model::WindowPropertyFrameAndDivider::isInsideRevealSolarAbsorptanceDefaulted)),
            DataSource(
            allWindowPropertyFrameAndDividers,
            true
            )
            );
        }

        else if (field == FRAMEOUTSIDEPROJECTION) {
          // WindowPropertyFrameAndDivider
          //double frameOutsideProjection() const;
          //bool isFrameOutsideProjectionDefaulted() const;
          //bool setFrameOutsideProjection(double frameOutsideProjection);
          //void resetFrameOutsideProjection();

          addValueEditColumn(Heading(QString(FRAMEOUTSIDEPROJECTION)),
            NullAdapter(&model::WindowPropertyFrameAndDivider::frameOutsideProjection),
            NullAdapter(&model::WindowPropertyFrameAndDivider::setFrameOutsideProjection),
            boost::optional<std::function<void(model::WindowPropertyFrameAndDivider*)>>(CastNullAdapter<model::WindowPropertyFrameAndDivider>(&model::WindowPropertyFrameAndDivider::resetFrameOutsideProjection)),
            boost::optional<std::function<bool(model::WindowPropertyFrameAndDivider*)>>(CastNullAdapter<model::WindowPropertyFrameAndDivider>(&model::WindowPropertyFrameAndDivider::isInsideRevealSolarAbsorptanceDefaulted)),
            DataSource(
            allWindowPropertyFrameAndDividers,
            true
            )
            );
        }

        else if (field == FRAMEINSIDEPROJECTION) {
          // WindowPropertyFrameAndDivider
          //double frameInsideProjection() const;
          //bool isFrameInsideProjectionDefaulted() const;
          //bool setFrameInsideProjection(double frameInsideProjection);
          //void resetFrameInsideProjection();
          
          addValueEditColumn(Heading(QString(FRAMEINSIDEPROJECTION)),
            NullAdapter(&model::WindowPropertyFrameAndDivider::frameInsideProjection),
            NullAdapter(&model::WindowPropertyFrameAndDivider::setFrameInsideProjection),
            boost::optional<std::function<void(model::WindowPropertyFrameAndDivider*)>>(CastNullAdapter<model::WindowPropertyFrameAndDivider>(&model::WindowPropertyFrameAndDivider::resetFrameInsideProjection)),
            boost::optional<std::function<bool(model::WindowPropertyFrameAndDivider*)>>(CastNullAdapter<model::WindowPropertyFrameAndDivider>(&model::WindowPropertyFrameAndDivider::isFrameInsideProjectionDefaulted)),
            DataSource(
            allWindowPropertyFrameAndDividers,
            true
            )
            );
        }

        else if (field == FRAMECONDUCTANCE) {
          // WindowPropertyFrameAndDivider
          //boost::optional<double> frameConductance() const;
          //bool setFrameConductance(double frameConductance);
          //void resetFrameConductance();

        }

        else if (field == FRAMEEDGEGLASSCONDUCTANCETOCENTEROFGLASSCONDUCTANCE) {
          // WindowPropertyFrameAndDivider
          //double ratioOfFrameEdgeGlassConductanceToCenterOfGlassConductance() const;
          //bool isRatioOfFrameEdgeGlassConductanceToCenterOfGlassConductanceDefaulted() const;
          //bool setRatioOfFrameEdgeGlassConductanceToCenterOfGlassConductance(double ratioOfFrameEdgeGlassConductanceToCenterOfGlassConductance);
          //void resetRatioOfFrameEdgeGlassConductanceToCenterOfGlassConductance();

          addValueEditColumn(Heading(QString(FRAMEEDGEGLASSCONDUCTANCETOCENTEROFGLASSCONDUCTANCE)),
            NullAdapter(&model::WindowPropertyFrameAndDivider::ratioOfFrameEdgeGlassConductanceToCenterOfGlassConductance),
            NullAdapter(&model::WindowPropertyFrameAndDivider::setRatioOfFrameEdgeGlassConductanceToCenterOfGlassConductance),
            boost::optional<std::function<void(model::WindowPropertyFrameAndDivider*)>>(CastNullAdapter<model::WindowPropertyFrameAndDivider>(&model::WindowPropertyFrameAndDivider::resetRatioOfFrameEdgeGlassConductanceToCenterOfGlassConductance)),
            boost::optional<std::function<bool(model::WindowPropertyFrameAndDivider*)>>(CastNullAdapter<model::WindowPropertyFrameAndDivider>(&model::WindowPropertyFrameAndDivider::isRatioOfFrameEdgeGlassConductanceToCenterOfGlassConductanceDefaulted)),
            DataSource(
            allWindowPropertyFrameAndDividers,
            true
            )
            );
        }

        else if (field == FRAMESOLARABSORPTANCE) {
          // WindowPropertyFrameAndDivider
          //double frameSolarAbsorptance() const;
          //bool isFrameSolarAbsorptanceDefaulted() const;
          //bool setFrameSolarAbsorptance(double frameSolarAbsorptance);
          //void resetFrameSolarAbsorptance();

          addValueEditColumn(Heading(QString(FRAMESOLARABSORPTANCE)),
            NullAdapter(&model::WindowPropertyFrameAndDivider::insideRevealSolarAbsorptance),
            NullAdapter(&model::WindowPropertyFrameAndDivider::setInsideRevealSolarAbsorptance),
            boost::optional<std::function<void(model::WindowPropertyFrameAndDivider*)>>(CastNullAdapter<model::WindowPropertyFrameAndDivider>(&model::WindowPropertyFrameAndDivider::resetInsideRevealSolarAbsorptance)),
            boost::optional<std::function<bool(model::WindowPropertyFrameAndDivider*)>>(CastNullAdapter<model::WindowPropertyFrameAndDivider>(&model::WindowPropertyFrameAndDivider::isInsideRevealSolarAbsorptanceDefaulted)),
            DataSource(
            allWindowPropertyFrameAndDividers,
            true
            )
            );
        }

        else if (field == FRAMEVISIBLEABSORPTANCE) {
          // WindowPropertyFrameAndDivider
          //double frameVisibleAbsorptance() const;
          //bool isFrameVisibleAbsorptanceDefaulted() const;
          //bool setFrameVisibleAbsorptance(double frameVisibleAbsorptance);
          //void resetFrameVisibleAbsorptance();

          addValueEditColumn(Heading(QString(FRAMEVISIBLEABSORPTANCE)),
            NullAdapter(&model::WindowPropertyFrameAndDivider::frameVisibleAbsorptance),
            NullAdapter(&model::WindowPropertyFrameAndDivider::setFrameVisibleAbsorptance),
            boost::optional<std::function<void(model::WindowPropertyFrameAndDivider*)>>(CastNullAdapter<model::WindowPropertyFrameAndDivider>(&model::WindowPropertyFrameAndDivider::resetFrameVisibleAbsorptance)),
            boost::optional<std::function<bool(model::WindowPropertyFrameAndDivider*)>>(CastNullAdapter<model::WindowPropertyFrameAndDivider>(&model::WindowPropertyFrameAndDivider::isFrameVisibleAbsorptanceDefaulted)),
            DataSource(
            allWindowPropertyFrameAndDividers,
            true
            )
            );
        }

        else if (field == FRAMETHERMALHEMISPHERICALEMISSIVITY) {
          // WindowPropertyFrameAndDivider
          //double frameThermalHemisphericalEmissivity() const;
          //bool isFrameThermalHemisphericalEmissivityDefaulted() const;
          //bool setFrameThermalHemisphericalEmissivity(double frameThermalHemisphericalEmissivity);
          //void resetFrameThermalHemisphericalEmissivity();

          addValueEditColumn(Heading(QString(FRAMETHERMALHEMISPHERICALEMISSIVITY)),
            NullAdapter(&model::WindowPropertyFrameAndDivider::frameThermalHemisphericalEmissivity),
            NullAdapter(&model::WindowPropertyFrameAndDivider::setFrameThermalHemisphericalEmissivity),
            boost::optional<std::function<void(model::WindowPropertyFrameAndDivider*)>>(CastNullAdapter<model::WindowPropertyFrameAndDivider>(&model::WindowPropertyFrameAndDivider::resetFrameThermalHemisphericalEmissivity)),
            boost::optional<std::function<bool(model::WindowPropertyFrameAndDivider*)>>(CastNullAdapter<model::WindowPropertyFrameAndDivider>(&model::WindowPropertyFrameAndDivider::isFrameThermalHemisphericalEmissivityDefaulted)),
            DataSource(
            allWindowPropertyFrameAndDividers,
            true
            )
            );
        }

        else if (field == DIVIDERTYPE) {
          // WindowPropertyFrameAndDivider
          //std::string dividerType() const;
          //bool isDividerTypeDefaulted() const;
          //bool setDividerType(std::string dividerType);
          //void resetDividerType();
        }

        else if (field == DIVIDERWIDTH) {
          // WindowPropertyFrameAndDivider
          //double dividerWidth() const;
          //bool isDividerWidthDefaulted() const;
          //bool setDividerWidth(double dividerWidth);
          //void resetDividerWidth();

          addValueEditColumn(Heading(QString(DIVIDERWIDTH)),
            NullAdapter(&model::WindowPropertyFrameAndDivider::dividerWidth),
            NullAdapter(&model::WindowPropertyFrameAndDivider::setDividerWidth),
            boost::optional<std::function<void(model::WindowPropertyFrameAndDivider*)>>(CastNullAdapter<model::WindowPropertyFrameAndDivider>(&model::WindowPropertyFrameAndDivider::resetDividerWidth)),
            boost::optional<std::function<bool(model::WindowPropertyFrameAndDivider*)>>(CastNullAdapter<model::WindowPropertyFrameAndDivider>(&model::WindowPropertyFrameAndDivider::isDividerWidthDefaulted)),
            DataSource(
            allWindowPropertyFrameAndDividers,
            true
            )
            );
        }

        else if (field == NUMBEROFHORIZONTALDIVIDERS) {
          // WindowPropertyFrameAndDivider
          //bool setNumberOfHorizontalDividers(double numberOfHorizontalDividers);
          //void resetNumberOfHorizontalDividers();
          //double numberOfHorizontalDividers() const;
          //bool isNumberOfHorizontalDividersDefaulted() const;

          addValueEditColumn(Heading(QString(NUMBEROFHORIZONTALDIVIDERS)),
            NullAdapter(&model::WindowPropertyFrameAndDivider::numberOfHorizontalDividers),
            NullAdapter(&model::WindowPropertyFrameAndDivider::setNumberOfHorizontalDividers),
            boost::optional<std::function<void(model::WindowPropertyFrameAndDivider*)>>(CastNullAdapter<model::WindowPropertyFrameAndDivider>(&model::WindowPropertyFrameAndDivider::resetNumberOfHorizontalDividers)),
            boost::optional<std::function<bool(model::WindowPropertyFrameAndDivider*)>>(CastNullAdapter<model::WindowPropertyFrameAndDivider>(&model::WindowPropertyFrameAndDivider::isNumberOfHorizontalDividersDefaulted)),
            DataSource(
            allWindowPropertyFrameAndDividers,
            true
            )
            );
        }

        else if (field == NUMBEROFVERTICALDIVIDERS) {
          //// WindowPropertyFrameAndDivider
          //bool setNumberOfVerticalDividers(double numberOfVerticalDividers);
          //void resetNumberOfVerticalDividers();
          //double numberOfVerticalDividers() const;
          //bool isNumberOfVerticalDividersDefaulted() const;

          addValueEditColumn(Heading(QString(NUMBEROFVERTICALDIVIDERS)),
            NullAdapter(&model::WindowPropertyFrameAndDivider::numberOfVerticalDividers),
            NullAdapter(&model::WindowPropertyFrameAndDivider::setNumberOfVerticalDividers),
            boost::optional<std::function<void(model::WindowPropertyFrameAndDivider*)>>(CastNullAdapter<model::WindowPropertyFrameAndDivider>(&model::WindowPropertyFrameAndDivider::resetNumberOfVerticalDividers)),
            boost::optional<std::function<bool(model::WindowPropertyFrameAndDivider*)>>(CastNullAdapter<model::WindowPropertyFrameAndDivider>(&model::WindowPropertyFrameAndDivider::isNumberOfVerticalDividersDefaulted)),
            DataSource(
            allWindowPropertyFrameAndDividers,
            true
            )
            );
        }

        else if (field == DIVIDEROUTSIDEPROJECTION) {
          // WindowPropertyFrameAndDivider
          //double dividerOutsideProjection() const;
          //bool isDividerOutsideProjectionDefaulted() const;
          //bool setDividerOutsideProjection(double dividerOutsideProjection);
          //void resetDividerOutsideProjection();

          addValueEditColumn(Heading(QString(DIVIDEROUTSIDEPROJECTION)),
            NullAdapter(&model::WindowPropertyFrameAndDivider::dividerOutsideProjection),
            NullAdapter(&model::WindowPropertyFrameAndDivider::setDividerOutsideProjection),
            boost::optional<std::function<void(model::WindowPropertyFrameAndDivider*)>>(CastNullAdapter<model::WindowPropertyFrameAndDivider>(&model::WindowPropertyFrameAndDivider::resetDividerOutsideProjection)),
            boost::optional<std::function<bool(model::WindowPropertyFrameAndDivider*)>>(CastNullAdapter<model::WindowPropertyFrameAndDivider>(&model::WindowPropertyFrameAndDivider::isDividerOutsideProjectionDefaulted)),
            DataSource(
            allWindowPropertyFrameAndDividers,
            true
            )
            );
        }

        else if (field == DIVIDERINSIDEPROJECTION) {
          // WindowPropertyFrameAndDivider
          //double dividerInsideProjection() const;
          //bool isDividerInsideProjectionDefaulted() const;
          //bool setDividerInsideProjection(double dividerInsideProjection);
          //void resetDividerInsideProjection();

          addValueEditColumn(Heading(QString(DIVIDERINSIDEPROJECTION)),
            NullAdapter(&model::WindowPropertyFrameAndDivider::dividerInsideProjection),
            NullAdapter(&model::WindowPropertyFrameAndDivider::setDividerInsideProjection),
            boost::optional<std::function<void(model::WindowPropertyFrameAndDivider*)>>(CastNullAdapter<model::WindowPropertyFrameAndDivider>(&model::WindowPropertyFrameAndDivider::resetDividerInsideProjection)),
            boost::optional<std::function<bool(model::WindowPropertyFrameAndDivider*)>>(CastNullAdapter<model::WindowPropertyFrameAndDivider>(&model::WindowPropertyFrameAndDivider::isDividerInsideProjectionDefaulted)),
            DataSource(
            allWindowPropertyFrameAndDividers,
            true
            )
            );
        }

        else if (field == DIVIDERCONDUCTANCE) {
          // WindowPropertyFrameAndDivider
          //double dividerConductance() const;
          //bool isDividerConductanceDefaulted() const;
          //bool setDividerConductance(double dividerConductance);
          //void resetDividerConductance();

          addValueEditColumn(Heading(QString(DIVIDERCONDUCTANCE)),
            NullAdapter(&model::WindowPropertyFrameAndDivider::dividerConductance),
            NullAdapter(&model::WindowPropertyFrameAndDivider::setDividerConductance),
            boost::optional<std::function<void(model::WindowPropertyFrameAndDivider*)>>(CastNullAdapter<model::WindowPropertyFrameAndDivider>(&model::WindowPropertyFrameAndDivider::resetDividerConductance)),
            boost::optional<std::function<bool(model::WindowPropertyFrameAndDivider*)>>(CastNullAdapter<model::WindowPropertyFrameAndDivider>(&model::WindowPropertyFrameAndDivider::isDividerConductanceDefaulted)),
            DataSource(
            allWindowPropertyFrameAndDividers,
            true
            )
            );
        }

        else if (field == RATIOOFDIVIDEREDGEGLASSCONDUCTANCETOCENTEROFGLASSCONDUCTANCE) {
          // WindowPropertyFrameAndDivider
          //double ratioOfDividerEdgeGlassConductanceToCenterOfGlassConductance() const;
          //bool isRatioOfDividerEdgeGlassConductanceToCenterOfGlassConductanceDefaulted() const;
          //bool setRatioOfDividerEdgeGlassConductanceToCenterOfGlassConductance(double ratioOfDividerEdgeGlassConductanceToCenterOfGlassConductance);
          //void resetRatioOfDividerEdgeGlassConductanceToCenterOfGlassConductance();

          addValueEditColumn(Heading(QString(RATIOOFDIVIDEREDGEGLASSCONDUCTANCETOCENTEROFGLASSCONDUCTANCE)),
            NullAdapter(&model::WindowPropertyFrameAndDivider::ratioOfDividerEdgeGlassConductanceToCenterOfGlassConductance),
            NullAdapter(&model::WindowPropertyFrameAndDivider::setRatioOfDividerEdgeGlassConductanceToCenterOfGlassConductance),
            boost::optional<std::function<void(model::WindowPropertyFrameAndDivider*)>>(CastNullAdapter<model::WindowPropertyFrameAndDivider>(&model::WindowPropertyFrameAndDivider::resetRatioOfDividerEdgeGlassConductanceToCenterOfGlassConductance)),
            boost::optional<std::function<bool(model::WindowPropertyFrameAndDivider*)>>(CastNullAdapter<model::WindowPropertyFrameAndDivider>(&model::WindowPropertyFrameAndDivider::isRatioOfDividerEdgeGlassConductanceToCenterOfGlassConductanceDefaulted)),
            DataSource(
            allWindowPropertyFrameAndDividers,
            true
            )
            );
        }

        else if (field == DIVIDERSOLARABSORPTANCE) {
          // WindowPropertyFrameAndDivider
          //double dividerSolarAbsorptance() const;
          //bool isDividerSolarAbsorptanceDefaulted() const;
          //bool setDividerSolarAbsorptance(double dividerSolarAbsorptance);
          //void resetDividerSolarAbsorptance();

          addValueEditColumn(Heading(QString(DIVIDERSOLARABSORPTANCE)),
            NullAdapter(&model::WindowPropertyFrameAndDivider::dividerSolarAbsorptance),
            NullAdapter(&model::WindowPropertyFrameAndDivider::setDividerSolarAbsorptance),
            boost::optional<std::function<void(model::WindowPropertyFrameAndDivider*)>>(CastNullAdapter<model::WindowPropertyFrameAndDivider>(&model::WindowPropertyFrameAndDivider::resetDividerSolarAbsorptance)),
            boost::optional<std::function<bool(model::WindowPropertyFrameAndDivider*)>>(CastNullAdapter<model::WindowPropertyFrameAndDivider>(&model::WindowPropertyFrameAndDivider::isDividerSolarAbsorptanceDefaulted)),
            DataSource(
            allWindowPropertyFrameAndDividers,
            true
            )
            );
        }

        else if (field == DIVIDERVISIBLEABSORPTANCE) {
          // WindowPropertyFrameAndDivider
          //double dividerVisibleAbsorptance() const;
          //bool isDividerVisibleAbsorptanceDefaulted() const;
          //bool setDividerVisibleAbsorptance(double dividerVisibleAbsorptance);
          //void resetDividerVisibleAbsorptance();

          addValueEditColumn(Heading(QString(DIVIDERVISIBLEABSORPTANCE)),
            NullAdapter(&model::WindowPropertyFrameAndDivider::dividerVisibleAbsorptance),
            NullAdapter(&model::WindowPropertyFrameAndDivider::setDividerVisibleAbsorptance),
            boost::optional<std::function<void(model::WindowPropertyFrameAndDivider*)>>(CastNullAdapter<model::WindowPropertyFrameAndDivider>(&model::WindowPropertyFrameAndDivider::resetDividerVisibleAbsorptance)),
            boost::optional<std::function<bool(model::WindowPropertyFrameAndDivider*)>>(CastNullAdapter<model::WindowPropertyFrameAndDivider>(&model::WindowPropertyFrameAndDivider::isDividerVisibleAbsorptanceDefaulted)),
            DataSource(
            allWindowPropertyFrameAndDividers,
            true
            )
            );
        }

        else if (field == DIVIDERTHERMALHEMISPHERICALEMISSIVITY) {
          // WindowPropertyFrameAndDivider
          //double dividerThermalHemisphericalEmissivity() const;
          //bool isDividerThermalHemisphericalEmissivityDefaulted() const;
          //bool setDividerThermalHemisphericalEmissivity(double dividerThermalHemisphericalEmissivity);
          //void resetDividerThermalHemisphericalEmissivity();

          addValueEditColumn(Heading(QString(DIVIDERTHERMALHEMISPHERICALEMISSIVITY)),
            NullAdapter(&model::WindowPropertyFrameAndDivider::dividerThermalHemisphericalEmissivity),
            NullAdapter(&model::WindowPropertyFrameAndDivider::setDividerThermalHemisphericalEmissivity),
            boost::optional<std::function<void(model::WindowPropertyFrameAndDivider*)>>(CastNullAdapter<model::WindowPropertyFrameAndDivider>(&model::WindowPropertyFrameAndDivider::resetDividerThermalHemisphericalEmissivity)),
            boost::optional<std::function<bool(model::WindowPropertyFrameAndDivider*)>>(CastNullAdapter<model::WindowPropertyFrameAndDivider>(&model::WindowPropertyFrameAndDivider::isDividerThermalHemisphericalEmissivityDefaulted)),
            DataSource(
            allWindowPropertyFrameAndDividers,
            true
            )
            );
        }

        else if (field == OUTSIDEREVEALDEPTH) {
          // WindowPropertyFrameAndDivider
          //double outsideRevealDepth() const;
          //bool isOutsideRevealDepthDefaulted() const;
          //bool setOutsideRevealDepth(double outsideRevealDepth);
          //void resetOutsideRevealDepth();

          addValueEditColumn(Heading(QString(OUTSIDEREVEALDEPTH)),
            NullAdapter(&model::WindowPropertyFrameAndDivider::outsideRevealDepth),
            NullAdapter(&model::WindowPropertyFrameAndDivider::setOutsideRevealDepth),
            boost::optional<std::function<void(model::WindowPropertyFrameAndDivider*)>>(CastNullAdapter<model::WindowPropertyFrameAndDivider>(&model::WindowPropertyFrameAndDivider::resetOutsideRevealDepth)),
            boost::optional<std::function<bool(model::WindowPropertyFrameAndDivider*)>>(CastNullAdapter<model::WindowPropertyFrameAndDivider>(&model::WindowPropertyFrameAndDivider::isOutsideRevealDepthDefaulted)),
            DataSource(
            allWindowPropertyFrameAndDividers,
            true
            )
            );
        }

        else if (field == OUTSIDEREVEALSOLARABSORPTANCE) {
          // WindowPropertyFrameAndDivider
          //double outsideRevealSolarAbsorptance() const;
          //bool isOutsideRevealSolarAbsorptanceDefaulted() const;
          //bool setOutsideRevealSolarAbsorptance(double outsideRevealSolarAbsorptance);
          //void resetOutsideRevealSolarAbsorptance();

          addValueEditColumn(Heading(QString(OUTSIDEREVEALSOLARABSORPTANCE)),
            NullAdapter(&model::WindowPropertyFrameAndDivider::outsideRevealSolarAbsorptance),
            NullAdapter(&model::WindowPropertyFrameAndDivider::setOutsideRevealSolarAbsorptance),
            boost::optional<std::function<void(model::WindowPropertyFrameAndDivider*)>>(CastNullAdapter<model::WindowPropertyFrameAndDivider>(&model::WindowPropertyFrameAndDivider::resetOutsideRevealSolarAbsorptance)),
            boost::optional<std::function<bool(model::WindowPropertyFrameAndDivider*)>>(CastNullAdapter<model::WindowPropertyFrameAndDivider>(&model::WindowPropertyFrameAndDivider::isOutsideRevealSolarAbsorptanceDefaulted)),
            DataSource(
            allWindowPropertyFrameAndDividers,
            true
            )
            );
        }

        else if (field == INSIDESILLDEPTH) {
          // WindowPropertyFrameAndDivider
          //double insideSillDepth() const;
          //bool isInsideSillDepthDefaulted() const;
          //bool setInsideSillDepth(double insideSillDepth);
          //void resetInsideSillDepth();

          addValueEditColumn(Heading(QString(INSIDESILLDEPTH)),
            NullAdapter(&model::WindowPropertyFrameAndDivider::insideSillDepth),
            NullAdapter(&model::WindowPropertyFrameAndDivider::setInsideSillDepth),
            boost::optional<std::function<void(model::WindowPropertyFrameAndDivider*)>>(CastNullAdapter<model::WindowPropertyFrameAndDivider>(&model::WindowPropertyFrameAndDivider::resetInsideSillDepth)),
            boost::optional<std::function<bool(model::WindowPropertyFrameAndDivider*)>>(CastNullAdapter<model::WindowPropertyFrameAndDivider>(&model::WindowPropertyFrameAndDivider::isInsideSillDepthDefaulted)),
            DataSource(
            allWindowPropertyFrameAndDividers,
            true
            )
            );
        }

        else if (field == INSIDESILLSOLARABSORPTANCE) {
          // WindowPropertyFrameAndDivider
          //double insideSillSolarAbsorptance() const;
          //bool isInsideSillSolarAbsorptanceDefaulted() const;
          //bool setInsideSillSolarAbsorptance(double insideSillSolarAbsorptance);
          //void resetInsideSillSolarAbsorptance();

          addValueEditColumn(Heading(QString(INSIDESILLSOLARABSORPTANCE)),
            NullAdapter(&model::WindowPropertyFrameAndDivider::insideSillSolarAbsorptance),
            NullAdapter(&model::WindowPropertyFrameAndDivider::setInsideSillSolarAbsorptance),
            boost::optional<std::function<void(model::WindowPropertyFrameAndDivider*)>>(CastNullAdapter<model::WindowPropertyFrameAndDivider>(&model::WindowPropertyFrameAndDivider::resetInsideSillSolarAbsorptance)),
            boost::optional<std::function<bool(model::WindowPropertyFrameAndDivider*)>>(CastNullAdapter<model::WindowPropertyFrameAndDivider>(&model::WindowPropertyFrameAndDivider::isInsideSillSolarAbsorptanceDefaulted)),
            DataSource(
            allWindowPropertyFrameAndDividers,
            true
            )
            );
        }

        else if (field == INSIDEREVEALDEPTH) {
          // WindowPropertyFrameAndDivider
          //double insideRevealDepth() const;
          //bool isInsideRevealDepthDefaulted() const;
          //bool setInsideRevealDepth(double insideRevealDepth);
          //void resetInsideRevealDepth();

          addValueEditColumn(Heading(QString(INSIDEREVEALDEPTH)),
            NullAdapter(&model::WindowPropertyFrameAndDivider::insideRevealDepth),
            NullAdapter(&model::WindowPropertyFrameAndDivider::setInsideRevealDepth),
            boost::optional<std::function<void(model::WindowPropertyFrameAndDivider*)>>(CastNullAdapter<model::WindowPropertyFrameAndDivider>(&model::WindowPropertyFrameAndDivider::resetInsideRevealDepth)),
            boost::optional<std::function<bool(model::WindowPropertyFrameAndDivider*)>>(CastNullAdapter<model::WindowPropertyFrameAndDivider>(&model::WindowPropertyFrameAndDivider::isInsideRevealDepthDefaulted)),
            DataSource(
            allWindowPropertyFrameAndDividers,
            true
            )
            );
        }

        else if (field == INSIDEREVEALSOLARABSORPTANCE) {
          // WindowPropertyFrameAndDivider
          //double insideRevealSolarAbsorptance() const;
          //bool isInsideRevealSolarAbsorptanceDefaulted() const;
          //bool setInsideRevealSolarAbsorptance(double insideRevealSolarAbsorptance);
          //void resetInsideRevealSolarAbsorptance();

          addValueEditColumn(Heading(QString(INSIDEREVEALSOLARABSORPTANCE)),
            NullAdapter(&model::WindowPropertyFrameAndDivider::insideRevealSolarAbsorptance),
            NullAdapter(&model::WindowPropertyFrameAndDivider::setInsideRevealSolarAbsorptance),
            boost::optional<std::function<void(model::WindowPropertyFrameAndDivider*)>>(CastNullAdapter<model::WindowPropertyFrameAndDivider>(&model::WindowPropertyFrameAndDivider::resetInsideRevealSolarAbsorptance)),
            boost::optional<std::function<bool(model::WindowPropertyFrameAndDivider*)>>(CastNullAdapter<model::WindowPropertyFrameAndDivider>(&model::WindowPropertyFrameAndDivider::isInsideRevealSolarAbsorptanceDefaulted)),
            DataSource(
            allWindowPropertyFrameAndDividers,
            true
            )
            );
        }

        else if (field == SPACENAME) {

        }

        else if (field == SURFACENAME) {

        }

        else if (field == WINDOWNAME) {
          // WindowPropertyFrameAndDivider
          // name
        }

        //else if (field == NAME) {

        //}

        else if (field == INSIDESHELFNAME) {
          // DaylightingDeviceShelf
          //boost::optional<InteriorPartitionSurface> insideShelf() const;
          //bool setInsideShelf(const InteriorPartitionSurface& insideShelf);

        }

        else if (field == OUTSIDESHELFNAME) {
          // DaylightingDeviceShelf
          //boost::optional<ShadingSurface> outsideShelf() const;
          //bool setOutsideShelf(const ShadingSurface& outsideShelf);

        }

        else if (field == VIEWFACTORTOOUTSIDESHELF) {
          // DaylightingDeviceShelf
          //boost::optional<double> viewFactortoOutsideShelf() const;
          //bool setViewFactortoOutsideShelf(double viewFactortoOutsideShelf);
          //void resetViewFactortoOutsideShelf();

          addValueEditColumn(Heading(QString(VIEWFACTORTOOUTSIDESHELF)),
            NullAdapter(&model::DaylightingDeviceShelf::viewFactortoOutsideShelf),
            NullAdapter(&model::DaylightingDeviceShelf::setViewFactortoOutsideShelf),
            //boost::optional<std::function<void(model::DaylightingDeviceShelf*)>>(CastNullAdapter<model::DaylightingDeviceShelf>(&model::DaylightingDeviceShelf::resetViewFactortoOutsideShelf)),
            //boost::optional<std::function<bool(model::DaylightingDeviceShelf *)>>(),
            DataSource(
            allDaylightingDeviceShelfs,
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

  QString SpacesSubsurfacesGridController::getColor(const model::ModelObject & modelObject)
  {
    QColor defaultColor(Qt::lightGray);
    return defaultColor.name();
  }

  void SpacesSubsurfacesGridController::checkSelectedFields()
  {
    if (!this->m_hasHorizontalHeader) return;

    OSGridController::checkSelectedFields();
  }

  void SpacesSubsurfacesGridController::onItemDropped(const OSItemId& itemId)
  {
  }

  void SpacesSubsurfacesGridController::refreshModelObjects()
  {
    m_modelObjects = subsetCastVector<model::ModelObject>(m_model.getModelObjects<model::Space>());
    std::sort(m_modelObjects.begin(), m_modelObjects.end(), ModelObjectNameSorter());
  }

} // openstudio
