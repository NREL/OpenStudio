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

#include "../shared_gui_components/OSGridView.hpp"

#include "../model/ConstructionBase.hpp"
#include "../model/ConstructionBase_Impl.hpp"
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

// GENERAL "General"
#define SPACENAME "Space Name" // read only
#define SURFACENAME "Surface Name" // read only
#define SUBSURFACENAME "Subsurface Name"
#define SUBSURFACETYPE "Subsurface Type"
#define MULTIPLIER "Multiplier"
#define CONSTRUCTION "Construction"
#define OUTSIDEBOUNDARYCONDITIONOBJECT "Outside Boundary Condition Object" // Dan note: needs model method for suggestions
#define SHADINGSURFACENAME "Shading Surface Name" // read only

// SHADINGCONTROLS "Shading Controls"
#define SPACENAME "Space Name" // read only
#define SURFACENAME "Surface Name" // read only
#define SUBSURFACENAME "Subsurface Name"
//#define NAME "Name"
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
#define SPACENAME "Space Name" // read only
#define SURFACENAME "Surface Name" // read only
#define SUBSURFACENAME "Subsurface Name"
//#define NAME "Name"
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
//#define NAME "Name"
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

  void SpacesSubsurfacesGridController::setCategoriesAndFields()
  {
    {
      std::vector<QString> fields;
      //fields.push_back(SPACENAME);
      //fields.push_back(SURFACENAME);
      //fields.push_back(SUBSURFACENAME);
      //fields.push_back(SUBSURFACETYPE);
      //fields.push_back(MULTIPLIER);
      //fields.push_back(CONSTRUCTION);
      //fields.push_back(OUTSIDEBOUNDARYCONDITIONOBJECT);
      //fields.push_back(SHADINGSURFACENAME);
      std::pair<QString, std::vector<QString> > categoryAndFields = std::make_pair(QString("General"), fields);
      m_categoriesAndFields.push_back(categoryAndFields);
    }

    {
      std::vector<QString> fields;
      //fields.push_back(SPACENAME);
      //fields.push_back(SURFACENAME);
      //fields.push_back(SUBSURFACENAME);
      //fields.push_back(NAME);
      //fields.push_back(SHADINGTYPE);
      //fields.push_back(CONSTRUCTIONWITHSHADINGNAME);
      //fields.push_back(SHADINGDEVICEMATERIALNAME);
      //fields.push_back(SHADINGCONTROLTYPE);
      //fields.push_back(SCHEDULENAME);
      //fields.push_back(SETPOINT);
      //fields.push_back(SHADINGCONTROLISSCHEDULED);
      //fields.push_back(GLARECONTROLISACTIVE);
      //fields.push_back(TYPEOFSLATANGLECONTROLFORBLINDS);
      //fields.push_back(SLATANGLESCHEDULENAME);
      //fields.push_back(SETPOINT2);
      std::pair<QString, std::vector<QString> > categoryAndFields = std::make_pair(QString("Shading Controls"), fields);
      m_categoriesAndFields.push_back(categoryAndFields);
    }

    {
      std::vector<QString> fields;
      //fields.push_back(SPACENAME);
      //fields.push_back(SURFACENAME);  
      //fields.push_back(SUBSURFACENAME);
      //fields.push_back(NAME);
      //fields.push_back(FRAMEWIDTH);
      //fields.push_back(FRAMEOUTSIDEPROJECTION);
      //fields.push_back(FRAMEINSIDEPROJECTION);
      //fields.push_back(FRAMECONDUCTANCE);
      //fields.push_back(FRAMEEDGEGLASSCONDUCTANCETOCENTEROFGLASSCONDUCTANCE);
      //fields.push_back(FRAMESOLARABSORPTANCE);
      //fields.push_back(FRAMEVISIBLEABSORPTANCE);
      //fields.push_back(FRAMETHERMALHEMISPHERICALEMISSIVITY);
      //fields.push_back(DIVIDERTYPE);
      //fields.push_back(DIVIDERWIDTH);
      //fields.push_back(NUMBEROFHORIZONTALDIVIDERS);
      //fields.push_back(NUMBEROFVERTICALDIVIDERS);
      //fields.push_back(DIVIDEROUTSIDEPROJECTION);
      //fields.push_back(DIVIDERINSIDEPROJECTION);
      //fields.push_back(DIVIDERCONDUCTANCE);
      //fields.push_back(RATIOOFDIVIDEREDGEGLASSCONDUCTANCETOCENTEROFGLASSCONDUCTANCE);
      //fields.push_back(DIVIDERSOLARABSORPTANCE);
      //fields.push_back(DIVIDERVISIBLEABSORPTANCE);
      //fields.push_back(DIVIDERTHERMALHEMISPHERICALEMISSIVITY);
      //fields.push_back(OUTSIDEREVEALDEPTH);
      //fields.push_back(OUTSIDEREVEALSOLARABSORPTANCE);
      //fields.push_back(INSIDESILLDEPTH);
      //fields.push_back(INSIDESILLSOLARABSORPTANCE);
      //fields.push_back(INSIDEREVEALDEPTH);
      //fields.push_back(INSIDEREVEALSOLARABSORPTANCE);
      std::pair<QString, std::vector<QString> > categoryAndFields = std::make_pair(QString("Frame and Divider"), fields);
      m_categoriesAndFields.push_back(categoryAndFields);
    }

    {
      std::vector<QString> fields;
      //fields.push_back(SPACENAME);
      //fields.push_back(SURFACENAME);
      //fields.push_back(WINDOWNAME);
      //fields.push_back(NAME);
      //fields.push_back(INSIDESHELFNAME);
      //fields.push_back(OUTSIDESHELFNAME);
      //fields.push_back(VIEWFACTORTOOUTSIDESHELF);
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
        if (field == SELECTED) {
          auto checkbox = QSharedPointer<QCheckBox>(new QCheckBox());
          checkbox->setToolTip("Check to select all rows");
          connect(checkbox.data(), &QCheckBox::stateChanged, this, &SpacesSubsurfacesGridController::selectAllStateChanged);

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

        else if (field == SURFACENAME) {

        }

        else if (field == SUBSURFACENAME) {

        }

        else if (field == SUBSURFACETYPE) {

        }

        else if (field == MULTIPLIER) {
          //addValueEditColumn(Heading(QString(MULTIPLIER)),
          //  NullAdapter(&model::Surface::multiplier),
          //  NullAdapter(&model::Surface::setMultiplier)
          //  );

        }

        else if (field == CONSTRUCTION) {
          addDropZoneColumn(Heading(QString(CONSTRUCTION)),
            CastNullAdapter<model::Surface>(&model::Surface::construction),
            CastNullAdapter<model::Surface>(&model::Surface::setConstruction),
            boost::optional<std::function<void(model::Surface*)> >(NullAdapter(&model::Surface::resetConstruction))//,
            //DataSource(
            //allConstructions, 
            //true
            //)
            );
        }

        else if (field == OUTSIDEBOUNDARYCONDITIONOBJECT) {

          std::function<bool(model::Surface *, const model::Surface &)> setter(
            [](model::Surface *t_surface, const model::Surface &t_arg) {
            auto copy = t_arg;
            return t_surface->setAdjacentSurface(copy);
          }
          );

          addDropZoneColumn(Heading(QString(OUTSIDEBOUNDARYCONDITIONOBJECT)),
            CastNullAdapter<model::Surface>(&model::Surface::adjacentSurface),
            setter,
            boost::optional<std::function<void(model::Surface*)> >(NullAdapter(&model::Surface::resetAdjacentSurface))
            //DataSource(
            //allSurfaces,// need allOutsideBoundaryConditions
            //true
            //)
            );

          //boost::optional<Surface> adjacentSurface() const
          //bool setAdjacentSurface(Surface& surface);
          //void resetAdjacentSurface();

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

        }

        else if (field == CONSTRUCTIONWITHSHADINGNAME) {

        }

        else if (field == SHADINGDEVICEMATERIALNAME) {

        }

        else if (field == SHADINGCONTROLTYPE) {

        }

        else if (field == SCHEDULENAME) {

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

        }

        else if (field == FRAMEOUTSIDEPROJECTION) {

        }

        else if (field == FRAMEINSIDEPROJECTION) {

        }

        else if (field == FRAMECONDUCTANCE) {

        }

        else if (field == FRAMEEDGEGLASSCONDUCTANCETOCENTEROFGLASSCONDUCTANCE) {

        }

        else if (field == FRAMESOLARABSORPTANCE) {

        }

        else if (field == FRAMEVISIBLEABSORPTANCE) {

        }

        else if (field == FRAMETHERMALHEMISPHERICALEMISSIVITY) {

        }

        else if (field == DIVIDERTYPE) {

        }

        else if (field == DIVIDERWIDTH) {

        }

        else if (field == NUMBEROFHORIZONTALDIVIDERS) {

        }

        else if (field == NUMBEROFVERTICALDIVIDERS) {

        }

        else if (field == DIVIDEROUTSIDEPROJECTION) {

        }

        else if (field == DIVIDERINSIDEPROJECTION) {

        }

        else if (field == DIVIDERCONDUCTANCE) {

        }

        else if (field == RATIOOFDIVIDEREDGEGLASSCONDUCTANCETOCENTEROFGLASSCONDUCTANCE) {

        }

        else if (field == DIVIDERSOLARABSORPTANCE) {

        }

        else if (field == DIVIDERVISIBLEABSORPTANCE) {

        }

        else if (field == DIVIDERTHERMALHEMISPHERICALEMISSIVITY) {

        }

        else if (field == OUTSIDEREVEALDEPTH) {

        }

        else if (field == OUTSIDEREVEALSOLARABSORPTANCE) {

        }

        else if (field == INSIDESILLDEPTH) {

        }

        else if (field == INSIDESILLSOLARABSORPTANCE) {

        }

        else if (field == INSIDEREVEALDEPTH) {

        }

        else if (field == INSIDEREVEALSOLARABSORPTANCE) {

        }

        else if (field == SPACENAME) {

        }

        else if (field == SURFACENAME) {

        }

        else if (field == WINDOWNAME) {

        }

        else if (field == NAME) {

        }

        else if (field == INSIDESHELFNAME) {

        }

        else if (field == OUTSIDESHELFNAME) {

        }

        else if (field == VIEWFACTORTOOUTSIDESHELF) {

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
