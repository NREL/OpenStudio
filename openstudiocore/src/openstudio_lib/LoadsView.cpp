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

#include "LoadsView.hpp"
#include "ModelObjectTypeListView.hpp"
#include "PeopleInspectorView.hpp"
#include "InternalMassInspectorView.hpp"
#include "LightsInspectorView.hpp"
#include "LuminaireInspectorView.hpp"
#include "ElectricEquipmentInspectorView.hpp"
#include "GasEquipmentInspectorView.hpp"
#include "SteamEquipmentInspectorView.hpp"
#include "OtherEquipmentInspectorView.hpp"
#include "WaterUseEquipmentInspectorView.hpp"

#include "../model/Model_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <QStyleOption>
#include <QPainter>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QScrollArea>
#include <sstream>

#include <utilities/idd/IddEnums.hxx>

namespace openstudio {


LoadsView::LoadsView(bool isIP, const openstudio::model::Model& model, QWidget * parent)
                     : ModelSubTabView(new ModelObjectTypeListView(LoadsView::modelObjectTypesAndNames(), 
                                                                   model, 
                                                                   true, 
                                                                   OSItemType::CollapsibleListHeader, 
                                                                   parent),
                                       new LoadsInspectorView(isIP, model,parent),
                                       false,
                                       parent)
{
  connect(this, &LoadsView::toggleUnitsClicked, modelObjectInspectorView(), &ModelObjectInspectorView::toggleUnitsClicked);
}

std::vector<std::pair<IddObjectType, std::string> > LoadsView::modelObjectTypesAndNames()
{
  std::vector<std::pair<IddObjectType, std::string> > result;
  
  result.push_back(std::make_pair<IddObjectType, std::string>(IddObjectType::OS_People_Definition, "People Definitions"));
  result.push_back(std::make_pair<IddObjectType, std::string>(IddObjectType::OS_Lights_Definition, "Lights Definitions"));
  result.push_back(std::make_pair<IddObjectType, std::string>(IddObjectType::OS_Luminaire_Definition, "Luminaire Definitions"));
  result.push_back(std::make_pair<IddObjectType, std::string>(IddObjectType::OS_ElectricEquipment_Definition, "Electric Equipment Definitions"));
  result.push_back(std::make_pair<IddObjectType, std::string>(IddObjectType::OS_GasEquipment_Definition, "Gas Equipment Definitions"));
  result.push_back(std::make_pair<IddObjectType, std::string>(IddObjectType::OS_SteamEquipment_Definition, "Steam Equipment Definitions"));
  result.push_back(std::make_pair<IddObjectType, std::string>(IddObjectType::OS_OtherEquipment_Definition, "Other Equipment Definitions"));
  result.push_back(std::make_pair<IddObjectType, std::string>(IddObjectType::OS_InternalMass_Definition, "Internal Mass Definitions"));
  result.push_back(std::make_pair<IddObjectType, std::string>(IddObjectType::OS_WaterUse_Equipment_Definition, "Water Use Equipment Definitions"));

  return result;
}

void LoadsView::toggleUnits(bool displayIP)
{
}

LoadsInspectorView::LoadsInspectorView(bool isIP,
                                       const model::Model& model,
                                       QWidget * parent )
  : ModelObjectInspectorView(model, false, parent)
{
  m_isIP = isIP;

  this->showDefaultView();
}

void LoadsInspectorView::onClearSelection()
{
  this->showDefaultView();
}

void LoadsInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  switch( modelObject.iddObjectType().value() )
  {
    case IddObjectType::OS_People_Definition:
      this->showPeopleDefinitionsInspector(modelObject);
      break;
    case IddObjectType::OS_Lights_Definition:
      this->showLightsDefinitionsInspector(modelObject);
      break;
    case IddObjectType::OS_Luminaire_Definition:
      this->showLuminaireDefinitionsInspector(modelObject);
      break;
    case IddObjectType::OS_ElectricEquipment_Definition:
      this->showElectricEquipmentDefinitionsInspector(modelObject);
      break;
    case IddObjectType::OS_GasEquipment_Definition:
      this->showGasEquipmentDefinitionsInspector(modelObject);
      break;
    case IddObjectType::OS_SteamEquipment_Definition:
      this->showSteamEquipmentDefinitionsInspector(modelObject);
      break;
    case IddObjectType::OS_OtherEquipment_Definition:
      this->showOtherEquipmentDefinitionsInspector(modelObject);
      break;
    case IddObjectType::OS_InternalMass_Definition:
      this->showInternalMassDefinitionsInspector(modelObject);
      break;
    case IddObjectType::OS_WaterUse_Equipment_Definition:
      this->showWaterUseEquipmentDefinitionsInspector(modelObject);
      break;
    default:
      showDefaultView();      
  }
}

void LoadsInspectorView::onUpdate()
{
}

void LoadsInspectorView::showInspector(QWidget * widget)
{
  if( QWidget * _widget = this->stackedWidget()->currentWidget() )
  {
    this->stackedWidget()->removeWidget(_widget);

    delete _widget;
  }

  this->stackedWidget()->addWidget(widget);
}

void LoadsInspectorView::showPeopleDefinitionsInspector(const openstudio::model::ModelObject& modelObject)
{
  auto peopleDefinitionInspectorView = new PeopleDefinitionInspectorView(m_isIP, m_model);
  connect(this, &LoadsInspectorView::toggleUnitsClicked, peopleDefinitionInspectorView, &PeopleDefinitionInspectorView::toggleUnitsClicked);

  peopleDefinitionInspectorView->selectModelObject(modelObject);

  this->showInspector(peopleDefinitionInspectorView);
}

void LoadsInspectorView::showLightsDefinitionsInspector(const openstudio::model::ModelObject& modelObject)
{
  auto lightsDefinitionInspectorView = new LightsDefinitionInspectorView(m_isIP, m_model);
  connect(this, &LoadsInspectorView::toggleUnitsClicked, lightsDefinitionInspectorView, &LightsDefinitionInspectorView::toggleUnitsClicked);

  lightsDefinitionInspectorView->selectModelObject(modelObject);

  this->showInspector(lightsDefinitionInspectorView);
}

void LoadsInspectorView::showLuminaireDefinitionsInspector(const openstudio::model::ModelObject& modelObject)
{
  auto luminaireDefinitionInspectorView = new LuminaireDefinitionInspectorView(m_isIP, m_model);
  connect(this, &LoadsInspectorView::toggleUnitsClicked, luminaireDefinitionInspectorView, &LuminaireDefinitionInspectorView::toggleUnitsClicked);

  luminaireDefinitionInspectorView->selectModelObject(modelObject);

  this->showInspector(luminaireDefinitionInspectorView);
}

void LoadsInspectorView::showElectricEquipmentDefinitionsInspector(const openstudio::model::ModelObject& modelObject)
{
  auto electricEquipmentDefinitionInspectorView = new ElectricEquipmentDefinitionInspectorView(m_isIP, m_model);
  connect(this, &LoadsInspectorView::toggleUnitsClicked, electricEquipmentDefinitionInspectorView, &ElectricEquipmentDefinitionInspectorView::toggleUnitsClicked);

  electricEquipmentDefinitionInspectorView->selectModelObject(modelObject);

  this->showInspector(electricEquipmentDefinitionInspectorView);
}

void LoadsInspectorView::showGasEquipmentDefinitionsInspector(const openstudio::model::ModelObject& modelObject)
{
  auto gasEquipmentDefinitionInspectorView = new GasEquipmentDefinitionInspectorView(m_isIP, m_model);
  connect(this, &LoadsInspectorView::toggleUnitsClicked, gasEquipmentDefinitionInspectorView, &GasEquipmentDefinitionInspectorView::toggleUnitsClicked);

  gasEquipmentDefinitionInspectorView->selectModelObject(modelObject);

  this->showInspector(gasEquipmentDefinitionInspectorView);
}

void LoadsInspectorView::showSteamEquipmentDefinitionsInspector(const openstudio::model::ModelObject& modelObject)
{
  auto steamEquipmentDefinitionInspectorView = new SteamEquipmentDefinitionInspectorView(m_isIP, m_model);
  connect(this, &LoadsInspectorView::toggleUnitsClicked, steamEquipmentDefinitionInspectorView, &SteamEquipmentDefinitionInspectorView::toggleUnitsClicked);

  steamEquipmentDefinitionInspectorView->selectModelObject(modelObject);

  this->showInspector(steamEquipmentDefinitionInspectorView);
}

void LoadsInspectorView::showOtherEquipmentDefinitionsInspector(const openstudio::model::ModelObject& modelObject)
{
  auto otherEquipmentDefinitionInspectorView = new OtherEquipmentDefinitionInspectorView(m_isIP, m_model);
  connect(this, &LoadsInspectorView::toggleUnitsClicked, otherEquipmentDefinitionInspectorView, &OtherEquipmentDefinitionInspectorView::toggleUnitsClicked);

  otherEquipmentDefinitionInspectorView->selectModelObject(modelObject);

  this->showInspector(otherEquipmentDefinitionInspectorView);
}

void LoadsInspectorView::showWaterUseEquipmentDefinitionsInspector(const openstudio::model::ModelObject& modelObject)
{
  auto waterUseEquipmentDefinitionInspectorView = new WaterUseEquipmentDefinitionInspectorView(m_isIP, m_model);
  connect(this, &LoadsInspectorView::toggleUnitsClicked, waterUseEquipmentDefinitionInspectorView, &WaterUseEquipmentDefinitionInspectorView::toggleUnitsClicked);

  waterUseEquipmentDefinitionInspectorView->selectModelObject(modelObject);

  showInspector(waterUseEquipmentDefinitionInspectorView);
}

void LoadsInspectorView::showInternalMassDefinitionsInspector(const openstudio::model::ModelObject& modelObject)
{
  auto internalMassDefinitionInspectorView = new InternalMassDefinitionInspectorView(m_isIP, m_model);
  connect(this, &LoadsInspectorView::toggleUnitsClicked, internalMassDefinitionInspectorView, &InternalMassDefinitionInspectorView::toggleUnitsClicked);

  internalMassDefinitionInspectorView->selectModelObject(modelObject);

  this->showInspector(internalMassDefinitionInspectorView);
}

void LoadsInspectorView::showDefaultView()
{
  if( QWidget * widget = this->stackedWidget()->currentWidget() )
  {
    this->stackedWidget()->removeWidget(widget);

    delete widget;
  }
}

void LoadsInspectorView::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

} // openstudio
