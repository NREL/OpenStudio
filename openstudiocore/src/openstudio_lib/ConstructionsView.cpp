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

#include "ConstructionsView.hpp"

#include "ConstructionCfactorUndergroundWallInspectorView.hpp"
#include "ConstructionFfactorGroundFloorInspectorView.hpp"
#include "ConstructionInspectorView.hpp"
#include "ConstructionInternalSourceInspectorView.hpp"
#include "ConstructionWindowDataFileInspectorView.hpp"
#include "ModelObjectTypeListView.hpp"

#include "../model/Model_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <QStackedWidget>

namespace openstudio {

ConstructionsView::ConstructionsView(bool isIP,
                                     const openstudio::model::Model& model,
                                     QWidget * parent)
: ModelSubTabView(new ModelObjectTypeListView(ConstructionsView::modelObjectTypesAndNames(), model, true, OSItemType::ListItem, parent),
                  new ConstructionsInspectorView(isIP, model, parent),
                  false,
                  parent)
{
  connect(this, &ConstructionsView::toggleUnitsClicked, modelObjectInspectorView(), &ModelObjectInspectorView::toggleUnitsClicked);
}

std::vector<std::pair<IddObjectType, std::string> > ConstructionsView::modelObjectTypesAndNames()
{
  std::vector<std::pair<IddObjectType, std::string> > result;
  result.push_back(std::make_pair<IddObjectType, std::string>(IddObjectType::OS_Construction, "Constructions"));
  result.push_back(std::make_pair<IddObjectType, std::string>(IddObjectType::OS_Construction_InternalSource, "Internal Source Constructions"));
  result.push_back(std::make_pair<IddObjectType, std::string>(IddObjectType::OS_Construction_CfactorUndergroundWall, "C-factor Underground Wall Constructions"));
  result.push_back(std::make_pair<IddObjectType, std::string>(IddObjectType::OS_Construction_FfactorGroundFloor, "F-factor Ground Floor Constructions"));
  // Not currently supported
  //result.push_back(std::make_pair<IddObjectType, std::string>(IddObjectType::OS_Construction_WindowDataFile, "Window Data File Constructions"));

  return result;
}

ConstructionsInspectorView::ConstructionsInspectorView(bool isIP,
                                                       const model::Model& model,
                                                       QWidget * parent )
  : ModelObjectInspectorView(model, false, parent),
  m_isIP(isIP)
{
}

void ConstructionsInspectorView::onClearSelection()
{
  QWidget* widget = this->stackedWidget()->currentWidget();
  ModelObjectInspectorView* modelObjectInspectorView = qobject_cast<ModelObjectInspectorView*>(widget);
  if(modelObjectInspectorView){
    modelObjectInspectorView->clearSelection();
  }

  this->stackedWidget()->setCurrentIndex(0);
}

void ConstructionsInspectorView::onUpdate()
{
}

void ConstructionsInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  switch( modelObject.iddObjectType().value() )
  {
    case IddObjectType::OS_Construction:
      this->showConstructionInspector(modelObject);
      break;
    case IddObjectType::OS_Construction_CfactorUndergroundWall:
      this->showCfactorUndergroundWallInspector(modelObject);
      break;
    case IddObjectType::OS_Construction_FfactorGroundFloor:
      this->showFfactorGroundFloorInspector(modelObject);
      break;
    case IddObjectType::OS_Construction_InternalSource:
      this->showInternalSourceInspector(modelObject);
      break;
    case IddObjectType::OS_Construction_WindowDataFile:
      this->showWindowDataFileInspector(modelObject);
      break;
    default:
      showDefaultView();      
  }
}

void ConstructionsInspectorView::showConstructionInspector(const openstudio::model::ModelObject & modelObject)
{
  ConstructionInspectorView * constructionInspectorView = new ConstructionInspectorView(m_isIP, m_model);
  connect(this, &ConstructionsInspectorView::toggleUnitsClicked, constructionInspectorView, &ConstructionInspectorView::toggleUnitsClicked);

  constructionInspectorView->selectModelObject(modelObject);

  this->showInspector(constructionInspectorView);
}

void ConstructionsInspectorView::showCfactorUndergroundWallInspector(const openstudio::model::ModelObject & modelObject)
{
  ConstructionCfactorUndergroundWallInspectorView * constructionCfactorUndergroundWallInspectorView = new ConstructionCfactorUndergroundWallInspectorView(m_isIP, m_model);
  connect(this, &ConstructionsInspectorView::toggleUnitsClicked, constructionCfactorUndergroundWallInspectorView, &ConstructionCfactorUndergroundWallInspectorView::toggleUnitsClicked);

  constructionCfactorUndergroundWallInspectorView->selectModelObject(modelObject);

  this->showInspector(constructionCfactorUndergroundWallInspectorView);
}

void ConstructionsInspectorView::showFfactorGroundFloorInspector(const openstudio::model::ModelObject & modelObject)
{
  ConstructionFfactorGroundFloorInspectorView * constructionFfactorGroundFloorInspectorView = new ConstructionFfactorGroundFloorInspectorView(m_isIP, m_model);
  connect(this, &ConstructionsInspectorView::toggleUnitsClicked, constructionFfactorGroundFloorInspectorView, &ConstructionFfactorGroundFloorInspectorView::toggleUnitsClicked);

  constructionFfactorGroundFloorInspectorView->selectModelObject(modelObject);

  this->showInspector(constructionFfactorGroundFloorInspectorView);
}

void ConstructionsInspectorView::showInternalSourceInspector(const openstudio::model::ModelObject & modelObject)
{
  ConstructionInternalSourceInspectorView * constructionInternalSourceInspectorView = new ConstructionInternalSourceInspectorView(m_isIP, m_model);
  connect(this, &ConstructionsInspectorView::toggleUnitsClicked, constructionInternalSourceInspectorView, &ConstructionInternalSourceInspectorView::toggleUnitsClicked);

  constructionInternalSourceInspectorView->selectModelObject(modelObject);

  this->showInspector(constructionInternalSourceInspectorView);
}

void ConstructionsInspectorView::showWindowDataFileInspector(const openstudio::model::ModelObject & modelObject)
{
  ConstructionWindowDataFileInspectorView * constructionWindowDataFileInspectorView = new ConstructionWindowDataFileInspectorView(m_isIP, m_model);
  connect(this, &ConstructionsInspectorView::toggleUnitsClicked, constructionWindowDataFileInspectorView, &ConstructionWindowDataFileInspectorView::toggleUnitsClicked);

  constructionWindowDataFileInspectorView->selectModelObject(modelObject);

  this->showInspector(constructionWindowDataFileInspectorView);
}

void ConstructionsInspectorView::showInspector(QWidget * widget)
{
  if( QWidget * _widget = this->stackedWidget()->currentWidget() )
  {
    this->stackedWidget()->removeWidget(_widget);

    delete _widget;
  }

  this->stackedWidget()->addWidget(widget);
}

void ConstructionsInspectorView::showDefaultView()
{
  if( QWidget * widget = this->stackedWidget()->currentWidget() )
  {
    this->stackedWidget()->removeWidget(widget);

    delete widget;
  }
}

/****************** SLOTS ******************/

void ConstructionsInspectorView::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

} // openstudio
