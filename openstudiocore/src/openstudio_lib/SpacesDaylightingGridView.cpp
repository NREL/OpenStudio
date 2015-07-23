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

#include "SpacesDaylightingGridView.hpp"

#include "OSDropZone.hpp"

#include "../shared_gui_components/OSGridView.hpp"

#include "../model/DaylightingControl.hpp"
#include "../model/DaylightingControl_Impl.hpp"
#include "../model/GlareSensor.hpp"
#include "../model/GlareSensor_Impl.hpp"
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

// DAYLIGHTINGCONTROLS "Daylighting Controls"
#define THERMALZONENAME "Thermal Zone Name" // read only
#define SPACENAME "Space Name"
#define POSITIONXCOORDINATE "Position X - Coordinate"
#define POSITIONYCOORDINATE "Position Y - Coordinate"
#define POSITIONZCOORDINATE "Position Z - Coordinate"
#define ROTATIONAROUNDXAXIS "Rotation Around X - Axis"
#define ROTATIONAROUNDYAXIS "Rotation Around Y - Axis"
#define ROTATIONAROUNDZAXIS "Rotation Around Z - Axis"
#define ILLUMINANCESETPOINT "Illuminance Setpoint"
#define LIGHTINGCONTROLTYPE "Lighting Control Type"
#define MINIMUMINPUTPOWERFRACTIONFORCONTINUOUSDIMMINGCONTROL "Minimum Input Power Fraction for Continuous Dimming Control"
#define MINIMUMLIGHTOUTPUTFRACTIONFORCONTINUOUSDIMMINGCONTROL "Minimum Light Output Fraction for Continuous Dimming Control"
#define NUMBEROFSTEPPEDCONTROLSTEPS "Number of Stepped Control Steps"
#define PROBABILITYLIGHTINGWILLBERESETWHENNEEDEDINMANUALSTEPPEDCONTROL "Probability Lighting will be Reset When Needed in Manual Stepped Control"
#define NUMBEROFDAYLIGHTINGVIEWS "Number of Daylighting Views"
#define MAXIMUMALLOWABLEDISCOMFORTGLAREINDEX "Maximum Allowable Discomfort Glare Index"

// ILLUMINANCEMAPS "Illuminance Maps"
#define SPACENAME "Space Name"
//#define NAME "Name"
#define ORIGINXCOORDINATE "Origin X - Coordinate"
#define ORIGINYCOORDINATE "Origin Y - Coordinate"
#define ORIGINZCOORDINATE "Origin Z - Coordinate"
#define ROTATIONAROUNDXAXIS "Rotation Around X - Axis"
#define ROTATIONAROUNDYAXIS "Rotation Around Y - Axis"
#define ROTATIONAROUNDZAXIS "Rotation Around Z - Axis"
#define XLENGTH "X Length"
#define NUMBEROFXGRIDPOINTS "Number of X Grid Points"
#define YLENGTH "Y Length"
#define NUMBEROFYGRIDPOINTS "Number of Y Grid Points"

// GLARESENSORS "Glare Sensors"
#define SPACENAME "Space Name"
//#define NAME "Name"
#define POSITIONXCOORDINATE "Position X - Coordinate"
#define POSITIONYCOORDINATE "Position Y - Coordinate"
#define POSITIONZCOORDINATE "Position Z - Coordinate"
#define ROTATIONAROUNDXAXIS "Rotation Around X - Axis"
#define ROTATIONAROUNDYAXIS "Rotation Around Y - Axis"
#define ROTATIONAROUNDZAXIS "Rotation Around Z - Axis"
#define NUMBEROFGLAREVIEWVECTORS "Number of Glare View Vectors"
#define MAXIMUMALLOWABLEDAYLIGHTGLAREPROBABILITY "Maximum Allowable Daylight Glare Probability"

namespace openstudio {

  struct ModelObjectNameSorter{
    // sort by name
    bool operator()(const model::ModelObject & lhs, const model::ModelObject & rhs){
      return (lhs.name() < rhs.name());
    }
  };

  SpacesDaylightingGridView::SpacesDaylightingGridView(bool isIP, const model::Model & model, QWidget * parent)
    : SpacesSubtabGridView(isIP, model, parent)
  {
    showStoryFilter();
    showThermalZoneFilter();
    showSpaceTypeFilter();
    showSpaceNameFilter();

    m_filterGridLayout->setRowStretch(m_filterGridLayout->rowCount(), 100);
    m_filterGridLayout->setColumnStretch(m_filterGridLayout->columnCount(), 100);

    m_gridController = new SpacesDaylightingGridController(isIP, "Space", IddObjectType::OS_Space, model, m_spacesModelObjects);
    m_gridView = new OSGridView(m_gridController, "Space", "Drop\nSpace", false, parent);

    setGridController(m_gridController);
    setGridView(m_gridView);

    m_gridView->m_contentLayout->addLayout(m_filterGridLayout);
    m_gridView->m_contentLayout->addSpacing(7);
    m_gridView->m_dropZone->hide();
  }

  SpacesDaylightingGridController::SpacesDaylightingGridController(bool isIP,
    const QString & headerText,
    IddObjectType iddObjectType,
    model::Model model,
    std::vector<model::ModelObject> modelObjects) :
    OSGridController(isIP, headerText, iddObjectType, model, modelObjects)
  {
    setCategoriesAndFields();
  }

  void SpacesDaylightingGridController::setCategoriesAndFields()
  {
    {
      std::vector<QString> fields;
      //fields.push_back(THERMALZONENAME);
      //fields.push_back(SPACENAME);
      //fields.push_back(POSITIONXCOORDINATE);
      //fields.push_back(POSITIONYCOORDINATE);
      //fields.push_back(POSITIONZCOORDINATE);
      //fields.push_back(ROTATIONAROUNDXAXIS);
      //fields.push_back(ROTATIONAROUNDYAXIS);
      //fields.push_back(ROTATIONAROUNDZAXIS);
      //fields.push_back(ILLUMINANCESETPOINT);
      //fields.push_back(LIGHTINGCONTROLTYPE);
      //fields.push_back(MINIMUMINPUTPOWERFRACTIONFORCONTINUOUSDIMMINGCONTROL);
      //fields.push_back(MINIMUMLIGHTOUTPUTFRACTIONFORCONTINUOUSDIMMINGCONTROL);
      //fields.push_back(NUMBEROFSTEPPEDCONTROLSTEPS);
      //fields.push_back(PROBABILITYLIGHTINGWILLBERESETWHENNEEDEDINMANUALSTEPPEDCONTROL);
      //fields.push_back(NUMBEROFDAYLIGHTINGVIEWS);
      //fields.push_back(MAXIMUMALLOWABLEDISCOMFORTGLAREINDEX);
      std::pair<QString, std::vector<QString> > categoryAndFields = std::make_pair(QString("Daylighting Controls"), fields);
      m_categoriesAndFields.push_back(categoryAndFields);
    }

    {
      std::vector<QString> fields;
      //fields.push_back(SPACENAME);
      //fields.push_back(NAME);
      //fields.push_back(ORIGINXCOORDINATE);
      //fields.push_back(ORIGINYCOORDINATE);
      //fields.push_back(ORIGINZCOORDINATE);
      //fields.push_back(ROTATIONAROUNDXAXIS);
      //fields.push_back(ROTATIONAROUNDYAXIS);
      //fields.push_back(ROTATIONAROUNDZAXIS);
      //fields.push_back(XLENGTH);
      //fields.push_back(NUMBEROFXGRIDPOINTS);
      //fields.push_back(YLENGTH);
      //fields.push_back(NUMBEROFYGRIDPOINTS);
      std::pair<QString, std::vector<QString> > categoryAndFields = std::make_pair(QString("Illuminance Maps"), fields);
      m_categoriesAndFields.push_back(categoryAndFields);
    }

    {
      std::vector<QString> fields;
      //fields.push_back(SPACENAME); 
      //fields.push_back(NAME);
      //fields.push_back(POSITIONXCOORDINATE);
      //fields.push_back(POSITIONYCOORDINATE);
      //fields.push_back(POSITIONZCOORDINATE);
      //fields.push_back(ROTATIONAROUNDXAXIS);
      //fields.push_back(ROTATIONAROUNDYAXIS);
      //fields.push_back(ROTATIONAROUNDZAXIS);
      //fields.push_back(NUMBEROFGLAREVIEWVECTORS);
      //fields.push_back(MAXIMUMALLOWABLEDAYLIGHTGLAREPROBABILITY);
      std::pair<QString, std::vector<QString> > categoryAndFields = std::make_pair(QString("Glare Sensors"), fields);
      m_categoriesAndFields.push_back(categoryAndFields);
    }

    OSGridController::setCategoriesAndFields();
  }

  void SpacesDaylightingGridController::categorySelected(int index)
  {
    OSGridController::categorySelected(index);
  }

  void SpacesDaylightingGridController::addColumns(const QString &category, std::vector<QString> & fields)
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
          connect(checkbox.data(), &QCheckBox::stateChanged, this, &SpacesDaylightingGridController::selectAllStateChanged);
          connect(checkbox.data(), &QCheckBox::stateChanged, this->gridView(), &OSGridView::gridRowSelectionChanged);

          addSelectColumn(Heading(QString(SELECTED), false, false, checkbox), "Check to select this row");
          //addSelectColumn(Heading(QString(SELECTED), false, false, checkbox), "Check to select this row",
          //  DataSource(
          //  allLoads,
          //  true
          //  )
          //  );
        }

        else if (field == THERMALZONENAME) {

        }

        else if (field == SPACENAME) {

        }

        else if (field == POSITIONXCOORDINATE) {

        }

        else if (field == POSITIONYCOORDINATE) {

        }

        else if (field == POSITIONZCOORDINATE) {

        }

        else if (field == ROTATIONAROUNDXAXIS) {

        }

        else if (field == ROTATIONAROUNDYAXIS) {

        }

        else if (field == ROTATIONAROUNDZAXIS) {

        }

        else if (field == ILLUMINANCESETPOINT) {

        }

        else if (field == LIGHTINGCONTROLTYPE) {

        }

        else if (field == MINIMUMINPUTPOWERFRACTIONFORCONTINUOUSDIMMINGCONTROL) {
          //DaylightingControl
          //double minimumInputPowerFractionforContinuousDimmingControl() const;
          //bool setMinimumInputPowerFractionforContinuousDimmingControl(double minimumInputPowerFractionforContinuousDimmingControl);
          //void resetMinimumInputPowerFractionforContinuousDimmingControl();
          //bool isMinimumInputPowerFractionforContinuousDimmingControlDefaulted() const;

        }

        else if (field == MINIMUMLIGHTOUTPUTFRACTIONFORCONTINUOUSDIMMINGCONTROL) {
          //DaylightingControl
          //  double minimumLightOutputFractionforContinuousDimmingControl() const;
          //bool setMinimumLightOutputFractionforContinuousDimmingControl(double minimumLightOutputFractionforContinuousDimmingControl);
          //void resetMinimumLightOutputFractionforContinuousDimmingControl();
          //bool isMinimumLightOutputFractionforContinuousDimmingControlDefaulted() const;

        }

        else if (field == NUMBEROFSTEPPEDCONTROLSTEPS) {
          //DaylightingControl
          //  int numberofSteppedControlSteps() const;
          //void setNumberofSteppedControlSteps(int numberofSteppedControlSteps);
          //void resetNumberofSteppedControlSteps();
          //bool isNumberofSteppedControlStepsDefaulted() const;

        }

        else if (field == PROBABILITYLIGHTINGWILLBERESETWHENNEEDEDINMANUALSTEPPEDCONTROL) {
          //DaylightingControl
          //  double probabilityLightingwillbeResetWhenNeededinManualSteppedControl() const;
          //bool setProbabilityLightingwillbeResetWhenNeededinManualSteppedControl(double probabilityLightingwillbeResetWhenNeededinManualSteppedControl);
          //void resetProbabilityLightingwillbeResetWhenNeededinManualSteppedControl();
          //bool isProbabilityLightingwillbeResetWhenNeededinManualSteppedControlDefaulted() const;

        }

        else if (field == NUMBEROFDAYLIGHTINGVIEWS) {
          //DaylightingControl
          //  int numberofDaylightingViews() const;
          //bool setNumberofDaylightingViews(int numberofDaylightingViews);
          //void resetNumberofDaylightingViews();
          //bool isNumberofDaylightingViewsDefaulted() const;

        }

        else if (field == MAXIMUMALLOWABLEDISCOMFORTGLAREINDEX) {
          //DaylightingControl
          //  boost::optional<double> maximumAllowableDiscomfortGlareIndex() const;
          //bool setMaximumAllowableDiscomfortGlareIndex(double maximumAllowableDiscomfortGlareIndex);
          //void resetMaximumAllowableDiscomfortGlareIndex();

        }

        else if (field == SPACENAME) {

        }

        else if (field == NAME) {

        }

        else if (field == ORIGINXCOORDINATE) {

        }

        else if (field == ORIGINYCOORDINATE) {

        }

        else if (field == ORIGINZCOORDINATE) {

        }

        else if (field == ROTATIONAROUNDXAXIS) {

        }

        else if (field == ROTATIONAROUNDYAXIS) {

        }

        else if (field == ROTATIONAROUNDZAXIS) {

        }

        else if (field == XLENGTH) {

        }

        else if (field == NUMBEROFXGRIDPOINTS) {

        }

        else if (field == YLENGTH) {

        }

        else if (field == NUMBEROFYGRIDPOINTS) {

        }

        else if (field == SPACENAME) {

        }

        else if (field == NAME) {

        }

        else if (field == POSITIONXCOORDINATE) {

        }

        else if (field == POSITIONYCOORDINATE) {

        }

        else if (field == POSITIONZCOORDINATE) {

        }

        else if (field == ROTATIONAROUNDXAXIS) {

        }

        else if (field == ROTATIONAROUNDYAXIS) {

        }

        else if (field == ROTATIONAROUNDZAXIS) {

        }

        else if (field == NUMBEROFGLAREVIEWVECTORS) {
          //GlareSensor
          //int numberofGlareViewVectors() const;
          //bool setNumberofGlareViewVectors(int numberofGlareViewVectors);
          //void resetNumberofGlareViewVectors();

        }

        else if (field == MAXIMUMALLOWABLEDAYLIGHTGLAREPROBABILITY) {
          //GlareSensor
          //boost::optional<double> maximumAllowableDaylightGlareProbability() const;
          //bool setMaximumAllowableDaylightGlareProbability(double maximumAllowableDaylightGlareProbability);
          //void resetMaximumAllowableDaylightGlareProbability();

        }
        else {
          // unhandled
          OS_ASSERT(false);
        }
      }
    }
  }

  QString SpacesDaylightingGridController::getColor(const model::ModelObject & modelObject)
  {
    QColor defaultColor(Qt::lightGray);
    return defaultColor.name();
  }

  void SpacesDaylightingGridController::checkSelectedFields()
  {
    if (!this->m_hasHorizontalHeader) return;

    OSGridController::checkSelectedFields();
  }

  void SpacesDaylightingGridController::onItemDropped(const OSItemId& itemId)
  {
  }

  void SpacesDaylightingGridController::refreshModelObjects()
  {
    m_modelObjects = subsetCastVector<model::ModelObject>(m_model.getModelObjects<model::Space>());
    std::sort(m_modelObjects.begin(), m_modelObjects.end(), ModelObjectNameSorter());
  }

} // openstudio
