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

#include <openstudio_lib/LoadsView.hpp>
#include <openstudio_lib/ModelObjectTypeListView.hpp>
#include <openstudio_lib/PeopleInspectorView.hpp>
#include <openstudio_lib/InternalMassInspectorView.hpp>
#include <openstudio_lib/LightsInspectorView.hpp>
#include <openstudio_lib/LuminaireInspectorView.hpp>
#include <openstudio_lib/ElectricEquipmentInspectorView.hpp>
#include <openstudio_lib/GasEquipmentInspectorView.hpp>
#include <openstudio_lib/SteamEquipmentInspectorView.hpp>
#include <openstudio_lib/OtherEquipmentInspectorView.hpp>
#include <openstudio_lib/WaterUseEquipmentInspectorView.hpp>

#include <model/Model_Impl.hpp>

#include <utilities/core/Assert.hpp>

#include <QStyleOption>
#include <QPainter>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QScrollArea>
#include <sstream>

namespace openstudio {


LoadsView::LoadsView(bool isIP, const openstudio::model::Model& model, QWidget * parent)
                     : ModelSubTabView(new ModelObjectTypeListView(LoadsView::modelObjectTypesAndNames(), 
                                                                   model, 
                                                                   true, 
                                                                   OSItem::COLLAPSIBLE_LIST_HEADER, 
                                                                   parent),
                                       new LoadsInspectorView(isIP, model,parent),
                                       parent)
{
  bool isConnected = connect(this,SIGNAL(toggleUnitsClicked(bool)),
                        modelObjectInspectorView(),SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);
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
  if(modelObject.iddObjectType() == IddObjectType::OS_People_Definition)
  {
    this->showPeopleDefinitionsInspector(modelObject);
  }
  else if(modelObject.iddObjectType() == IddObjectType::OS_Lights_Definition)
  {
    this->showLightsDefinitionsInspector(modelObject);
  }
  else if(modelObject.iddObjectType() == IddObjectType::OS_Luminaire_Definition)
  {
    this->showLuminaireDefinitionsInspector(modelObject);
  }
  else if(modelObject.iddObjectType() == IddObjectType::OS_ElectricEquipment_Definition)
  {
    this->showElectricEquipmentDefinitionsInspector(modelObject);
  }
  else if(modelObject.iddObjectType() == IddObjectType::OS_GasEquipment_Definition)
  {
    this->showGasEquipmentDefinitionsInspector(modelObject);
  }
  else if(modelObject.iddObjectType() == IddObjectType::OS_SteamEquipment_Definition)
  {
    this->showSteamEquipmentDefinitionsInspector(modelObject);
  }
  else if(modelObject.iddObjectType() == IddObjectType::OS_OtherEquipment_Definition)
  {
    this->showOtherEquipmentDefinitionsInspector(modelObject);
  }
  else if(modelObject.iddObjectType() == IddObjectType::OS_InternalMass_Definition)
  {
    this->showInternalMassDefinitionsInspector(modelObject);
  }
  else if(modelObject.iddObjectType() == IddObjectType::OS_WaterUse_Equipment_Definition)
  {
    this->showWaterUseEquipmentDefinitionsInspector(modelObject);
  }
  else
  {
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
  PeopleDefinitionInspectorView * peopleDefinitionInspectorView = new PeopleDefinitionInspectorView(m_isIP, m_model);
  bool isConnected = connect(this, 
                             SIGNAL(toggleUnitsClicked(bool)),
                             peopleDefinitionInspectorView, 
                             SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  peopleDefinitionInspectorView->selectModelObject(modelObject);

  this->showInspector(peopleDefinitionInspectorView);
}

void LoadsInspectorView::showLightsDefinitionsInspector(const openstudio::model::ModelObject& modelObject)
{
  LightsDefinitionInspectorView * lightsDefinitionInspectorView = new LightsDefinitionInspectorView(m_isIP, m_model);
  bool isConnected = connect(this, 
                             SIGNAL(toggleUnitsClicked(bool)),
                             lightsDefinitionInspectorView, 
                             SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  lightsDefinitionInspectorView->selectModelObject(modelObject);

  this->showInspector(lightsDefinitionInspectorView);
}

void LoadsInspectorView::showLuminaireDefinitionsInspector(const openstudio::model::ModelObject& modelObject)
{
  LuminaireDefinitionInspectorView * luminaireDefinitionInspectorView = new LuminaireDefinitionInspectorView(m_isIP, m_model);
  bool isConnected = connect(this, 
                             SIGNAL(toggleUnitsClicked(bool)),
                             luminaireDefinitionInspectorView, 
                             SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  luminaireDefinitionInspectorView->selectModelObject(modelObject);

  this->showInspector(luminaireDefinitionInspectorView);
}

void LoadsInspectorView::showElectricEquipmentDefinitionsInspector(const openstudio::model::ModelObject& modelObject)
{
  ElectricEquipmentDefinitionInspectorView * electricEquipmentDefinitionInspectorView = new ElectricEquipmentDefinitionInspectorView(m_isIP, m_model);
  bool isConnected = connect(this, 
                             SIGNAL(toggleUnitsClicked(bool)),
                             electricEquipmentDefinitionInspectorView, 
                             SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  electricEquipmentDefinitionInspectorView->selectModelObject(modelObject);

  this->showInspector(electricEquipmentDefinitionInspectorView);
}

void LoadsInspectorView::showGasEquipmentDefinitionsInspector(const openstudio::model::ModelObject& modelObject)
{
  GasEquipmentDefinitionInspectorView * gasEquipmentDefinitionInspectorView = new GasEquipmentDefinitionInspectorView(m_isIP, m_model);
  bool isConnected = connect(this, 
                             SIGNAL(toggleUnitsClicked(bool)),
                             gasEquipmentDefinitionInspectorView, 
                             SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  gasEquipmentDefinitionInspectorView->selectModelObject(modelObject);

  this->showInspector(gasEquipmentDefinitionInspectorView);
}

void LoadsInspectorView::showSteamEquipmentDefinitionsInspector(const openstudio::model::ModelObject& modelObject)
{
  SteamEquipmentDefinitionInspectorView * steamEquipmentDefinitionInspectorView = new SteamEquipmentDefinitionInspectorView(m_isIP, m_model);
  bool isConnected = connect(this, 
                             SIGNAL(toggleUnitsClicked(bool)),
                             steamEquipmentDefinitionInspectorView, 
                             SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  steamEquipmentDefinitionInspectorView->selectModelObject(modelObject);

  this->showInspector(steamEquipmentDefinitionInspectorView);
}

void LoadsInspectorView::showOtherEquipmentDefinitionsInspector(const openstudio::model::ModelObject& modelObject)
{
  OtherEquipmentDefinitionInspectorView * otherEquipmentDefinitionInspectorView = new OtherEquipmentDefinitionInspectorView(m_isIP, m_model);
  bool isConnected = connect(this, 
                             SIGNAL(toggleUnitsClicked(bool)),
                             otherEquipmentDefinitionInspectorView, 
                             SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  otherEquipmentDefinitionInspectorView->selectModelObject(modelObject);

  this->showInspector(otherEquipmentDefinitionInspectorView);
}

void LoadsInspectorView::showWaterUseEquipmentDefinitionsInspector(const openstudio::model::ModelObject& modelObject)
{
  WaterUseEquipmentDefinitionInspectorView * waterUseEquipmentDefinitionInspectorView = new WaterUseEquipmentDefinitionInspectorView(m_isIP, m_model);
  bool isConnected = connect(this, 
                             SIGNAL(toggleUnitsClicked(bool)),
                             waterUseEquipmentDefinitionInspectorView, 
                             SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  waterUseEquipmentDefinitionInspectorView->selectModelObject(modelObject);

  showInspector(waterUseEquipmentDefinitionInspectorView);
}

void LoadsInspectorView::showInternalMassDefinitionsInspector(const openstudio::model::ModelObject& modelObject)
{
  InternalMassDefinitionInspectorView * internalMassDefinitionInspectorView = new InternalMassDefinitionInspectorView(m_isIP, m_model);
  bool isConnected = connect(this, 
                             SIGNAL(toggleUnitsClicked(bool)),
                             internalMassDefinitionInspectorView, 
                             SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

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
