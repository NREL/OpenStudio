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
  auto constructionInspectorView = new ConstructionInspectorView(m_isIP, m_model);
  connect(this, &ConstructionsInspectorView::toggleUnitsClicked, constructionInspectorView, &ConstructionInspectorView::toggleUnitsClicked);

  constructionInspectorView->selectModelObject(modelObject);

  this->showInspector(constructionInspectorView);
}

void ConstructionsInspectorView::showCfactorUndergroundWallInspector(const openstudio::model::ModelObject & modelObject)
{
  auto constructionCfactorUndergroundWallInspectorView = new ConstructionCfactorUndergroundWallInspectorView(m_isIP, m_model);
  connect(this, &ConstructionsInspectorView::toggleUnitsClicked, constructionCfactorUndergroundWallInspectorView, &ConstructionCfactorUndergroundWallInspectorView::toggleUnitsClicked);

  constructionCfactorUndergroundWallInspectorView->selectModelObject(modelObject);

  this->showInspector(constructionCfactorUndergroundWallInspectorView);
}

void ConstructionsInspectorView::showFfactorGroundFloorInspector(const openstudio::model::ModelObject & modelObject)
{
  auto constructionFfactorGroundFloorInspectorView = new ConstructionFfactorGroundFloorInspectorView(m_isIP, m_model);
  connect(this, &ConstructionsInspectorView::toggleUnitsClicked, constructionFfactorGroundFloorInspectorView, &ConstructionFfactorGroundFloorInspectorView::toggleUnitsClicked);

  constructionFfactorGroundFloorInspectorView->selectModelObject(modelObject);

  this->showInspector(constructionFfactorGroundFloorInspectorView);
}

void ConstructionsInspectorView::showInternalSourceInspector(const openstudio::model::ModelObject & modelObject)
{
  auto constructionInternalSourceInspectorView = new ConstructionInternalSourceInspectorView(m_isIP, m_model);
  connect(this, &ConstructionsInspectorView::toggleUnitsClicked, constructionInternalSourceInspectorView, &ConstructionInternalSourceInspectorView::toggleUnitsClicked);

  constructionInternalSourceInspectorView->selectModelObject(modelObject);

  this->showInspector(constructionInternalSourceInspectorView);
}

void ConstructionsInspectorView::showWindowDataFileInspector(const openstudio::model::ModelObject & modelObject)
{
  auto constructionWindowDataFileInspectorView = new ConstructionWindowDataFileInspectorView(m_isIP, m_model);
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
