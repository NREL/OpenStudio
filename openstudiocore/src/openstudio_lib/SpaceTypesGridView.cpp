/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include "SpaceTypesGridView.hpp"

#include "ModelObjectItem.hpp"
#include "OSAppBase.hpp"
#include "OSDocument.hpp"
#include "OSDropZone.hpp"

#include "../shared_gui_components/OSGridView.hpp"

#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/ModelObject.hpp"
#include "../model/ModelObject_Impl.hpp"
#include "../model/SpaceType.hpp"
#include "../model/SpaceType_Impl.hpp"

#include "../utilities/idd/OS_SpaceType_FieldEnums.hxx"

#include <QBoxLayout>
#include <QCheckBox>
#include <QLabel>
#include <QScrollArea>
#include <QSettings>
#include <QTimer>

// These defines provide a common area for field display names
// used on column headers, and other grid widgets

#define NAME "Name"

// GENERAL
#define RENDERINGCOLOR "Rendering color" // colored button
#define DEFAULTCONSTRUCTIONSET "Default Construction Set" // DropZone
#define DEFAULTSCHEDULESET "Default Schedule Set" // DropZone
#define DESIGNSPECIFICATIONOUTDOORAIR "Design Specification Outdoor Air" // DropZone
#define SPACEINFILTRATIONDESIGNFLOWRATES "Space Infiltration Design Flow Rates" // DropZone
#define SPACEINFILTRATIONEFFECTIVELEAKAGEAREAS "Space Infiltration Effective Leakage Areas" // DropZone

// LOADS
#define LOADNAME "Name" // Name Edit
#define MULTIPLIER "Multiplier" // Value Edit
#define DEFINITION "Definition" // DropZone
#define SCHEDULE "Schedule" // DropZone
#define ACTIVITYSCHEDULE "Activity Schedule" // DropZone

// MEASURE TAGS
#define STANDARDSBUILDINGTYPE "Standards Building Type" // comboBox
#define STANDARDSSPACETYPE "Standards Space Type" // comboBox

namespace openstudio {

struct ModelObjectNameSorter{
  // sort by name
  bool operator()(const model::ModelObject & lhs, const model::ModelObject & rhs){
    return (lhs.name() < rhs.name());
  }
};

SpaceTypesGridView::SpaceTypesGridView(bool isIP, const model::Model & model, QWidget * parent)
  : QWidget(parent),
  m_isIP(isIP)
{
  QVBoxLayout * layout = 0;

  layout = new QVBoxLayout();
  layout->setSpacing(0);
  layout->setContentsMargins(0,0,0,0);
  setLayout(layout);
  
  std::vector<model::SpaceType> spaceTypes = model.getModelObjects<model::SpaceType>();
  std::vector<model::ModelObject> spaceTypeModelObjects = subsetCastVector<model::ModelObject>(spaceTypes);

  SpaceTypesGridController * spaceTypesGridController  = new SpaceTypesGridController(m_isIP, "Space Types", model, spaceTypeModelObjects);
  OSGridView * gridView = new OSGridView(spaceTypesGridController, "Space Types", "Drop\nZone", parent);
  gridView->m_dropZone->hide();
 
  layout->addWidget(gridView,0,Qt::AlignTop);

  layout->addStretch(1);

  bool isConnected = false;

  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                        spaceTypesGridController, SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                        spaceTypesGridController, SLOT(toggleUnits(bool)));
  OS_ASSERT(isConnected);

  std::vector<model::SpaceType> spaceType = model.getModelObjects<model::SpaceType>(); // NOTE for horizontal system lists

}

SpaceTypesGridController::SpaceTypesGridController(bool isIP,
  const QString & headerText,
  model::Model model,
  std::vector<model::ModelObject> modelObjects) :
  OSGridController(isIP, headerText, model, modelObjects)
{
  setCategoriesAndFields();
}

void SpaceTypesGridController::setCategoriesAndFields()
{

  {
    std::vector<QString> fields;
    //fields.push_back(RENDERINGCOLOR); // colored button
    //fields.push_back(DEFAULTCONSTRUCTIONSET); // TODO DropZone
    //fields.push_back(DEFAULTSCHEDULESET); // TODO DropZone
    //fields.push_back(DESIGNSPECIFICATIONOUTDOORAIR); // TODO DropZone
    //fields.push_back(SPACEINFILTRATIONDESIGNFLOWRATES); // TODO DropZone
    //fields.push_back(SPACEINFILTRATIONEFFECTIVELEAKAGEAREAS); // TODO DropZone
    std::pair<QString,std::vector<QString> > categoryAndFields = std::make_pair(QString("General"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    //fields.push_back(LOADNAME); // Name Edit
    //fields.push_back(MULTIPLIER); // Value Edit
    //fields.push_back(DEFINITION); // DropZone
    //fields.push_back(SCHEDULE); // DropZone
    //fields.push_back(ACTIVITYSCHEDULE); // DropZone
    std::pair<QString,std::vector<QString> > categoryAndFields = std::make_pair(QString("Loads"),fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  {
    std::vector<QString> fields;
    //fields.push_back(STANDARDSBUILDINGTYPE);
    //fields.push_back(STANDARDSSPACETYPE);
    std::pair<QString, std::vector<QString> > categoryAndFields = std::make_pair(QString("Measure Tags"), fields);
    m_categoriesAndFields.push_back(categoryAndFields);
  }

  OSGridController::setCategoriesAndFields();

}

void SpaceTypesGridController::addColumns(std::vector<QString> & fields)
{
  // always show name column
  fields.insert(fields.begin(), NAME);

  m_baseConcepts.clear();

  Q_FOREACH(QString field, fields){
    if(field == NAME){
      addNameLineEditColumn(QString(NAME),
                            NullAdapter(&model::SpaceType::name),
                            NullAdapter(&model::SpaceType::setName));

    }else{
      // unhandled
      //OS_ASSERT(false);
    }
  }
}

QString SpaceTypesGridController::getColor(const model:: ModelObject & modelObject)
{
  QColor defaultColor(Qt::lightGray);
  QString color(defaultColor.name());

  // TODO: The code below is currently commented out because a refresh crash bug is precluding rack color
  // updates due to rack assignments to cases and walk-ins.  No colors are better than wrong colors.

  //std::vector<model::RefrigerationSystem> refrigerationSystems = m_model.getModelObjects<model::RefrigerationSystem>();

  //boost::optional<model::SpaceType> refrigerationCase = modelObject.optionalCast<model::SpaceType>();
  //OS_ASSERT(refrigerationCase);

  //boost::optional<model::RefrigerationSystem> refrigerationSystem = refrigerationCase->system();
  //if(!refrigerationSystem){
  //  return color;
  //}

  //std::vector<model::RefrigerationSystem>::iterator it;
  //it = std::find(refrigerationSystems.begin(), refrigerationSystems.end(), refrigerationSystem.get());
  //if(it != refrigerationSystems.end()){
  //  int index = std::distance(refrigerationSystems.begin(), it);
  //  if(index >= static_cast<int>(m_colors.size())){
  //    index = m_colors.size() - 1; // similar to scheduleView's approach
  //  }
  //  color = this->m_colors.at(index).name();
  //}

  return color;
}

void SpaceTypesGridController::checkSelectedFields()
{
  if(!this->m_hasHorizontalHeader) return;

  // Don't show the name column check box
  // From above in addColumns, we know that NAME is the first entry
  HorizontalHeaderWidget * horizontalHeaderWidget = qobject_cast<HorizontalHeaderWidget *>(m_horizontalHeader.at(0));
  OS_ASSERT(horizontalHeaderWidget);
  horizontalHeaderWidget->m_checkBox->hide();

  OSGridController::checkSelectedFields();
}

void SpaceTypesGridController::onItemDropped(const OSItemId& itemId)
{
  boost::optional<model::ModelObject> modelObject = OSAppBase::instance()->currentDocument()->getModelObject(itemId);
  if (modelObject){
    if (modelObject->optionalCast<model::SpaceType>()){
      modelObject->clone(m_model);
      emit modelReset();
    }
  }
}

void SpaceTypesGridController::refreshModelObjects()
{
  std::vector<model::SpaceType> refrigerationCases = m_model.getModelObjects<model::SpaceType>();
  m_modelObjects = subsetCastVector<model::ModelObject>(refrigerationCases);
  std::sort(m_modelObjects.begin(), m_modelObjects.end(), ModelObjectNameSorter());
}

void SpaceTypesGridController::onComboBoxIndexChanged(int index)
{
}

} // openstudio

