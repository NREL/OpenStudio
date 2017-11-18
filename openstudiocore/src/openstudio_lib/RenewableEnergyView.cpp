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

#include "RenewableEnergyView.hpp"
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
#include "PhotovoltaicInspectorView.hpp"
#include "PhotovoltaicThermalInspectorView.hpp"

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


RenewableEnergyView::RenewableEnergyView(bool isIP, const openstudio::model::Model& model, QWidget * parent)
                     : ModelSubTabView(new ModelObjectTypeListView(RenewableEnergyView::modelObjectTypesAndNames(), 
                                                                   model, 
                                                                   true, 
                                                                   OSItemType::CollapsibleListHeader, 
                                                                   parent),
                                       new RenewableEnergyInspectorView(isIP, model,parent),
                                       false,
                                       parent)
{
  connect(this, &RenewableEnergyView::toggleUnitsClicked, modelObjectInspectorView(), &ModelObjectInspectorView::toggleUnitsClicked);
}

std::vector<std::pair<IddObjectType, std::string> > RenewableEnergyView::modelObjectTypesAndNames()
{
  std::vector<std::pair<IddObjectType, std::string> > result;
  result.push_back(std::make_pair<IddObjectType, std::string>(IddObjectType::OS_Exterior_PV_Thermal, "Solar Thermal Definitions"));
  result.push_back(std::make_pair<IddObjectType, std::string>(IddObjectType::OS_Exterior_PV, "Photovoltaic Rooftop Definitions"));

  return result;
}

void RenewableEnergyView::toggleUnits(bool displayIP)
{
}

RenewableEnergyInspectorView::RenewableEnergyInspectorView(bool isIP,
                                       const model::Model& model,
                                       QWidget * parent )
  : ModelObjectInspectorView(model, false, parent)
{
  m_isIP = isIP;

  this->showDefaultView();
}

void RenewableEnergyInspectorView::onClearSelection()
{
  this->showDefaultView();
}

void RenewableEnergyInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  switch( modelObject.iddObjectType().value() )
  {
  case IddObjectType::OS_Exterior_PV_Thermal:
	  this->showPhotovoltaicThermalInspector(modelObject);
	  break;
    case IddObjectType::OS_Exterior_PV:
      this->showPhotovoltaicInspector(modelObject);
      break;
    default:
      showDefaultView();      
  }
}

void RenewableEnergyInspectorView::onUpdate()
{
}

void RenewableEnergyInspectorView::showInspector(QWidget * widget)
{
  if( QWidget * _widget = this->stackedWidget()->currentWidget() )
  {
    this->stackedWidget()->removeWidget(_widget);

    delete _widget;
  }

  this->stackedWidget()->addWidget(widget);
}

void RenewableEnergyInspectorView::showPeopleDefinitionsInspector(const openstudio::model::ModelObject& modelObject)
{
  PeopleDefinitionInspectorView * peopleDefinitionInspectorView = new PeopleDefinitionInspectorView(m_isIP, m_model);
  connect(this, &RenewableEnergyInspectorView::toggleUnitsClicked, peopleDefinitionInspectorView, &PeopleDefinitionInspectorView::toggleUnitsClicked);

  peopleDefinitionInspectorView->selectModelObject(modelObject);

  this->showInspector(peopleDefinitionInspectorView);
}

void RenewableEnergyInspectorView::showLightsDefinitionsInspector(const openstudio::model::ModelObject& modelObject)
{
  LightsDefinitionInspectorView * lightsDefinitionInspectorView = new LightsDefinitionInspectorView(m_isIP, m_model);
  connect(this, &RenewableEnergyInspectorView::toggleUnitsClicked, lightsDefinitionInspectorView, &LightsDefinitionInspectorView::toggleUnitsClicked);

  lightsDefinitionInspectorView->selectModelObject(modelObject);

  this->showInspector(lightsDefinitionInspectorView);
}

void RenewableEnergyInspectorView::showLuminaireDefinitionsInspector(const openstudio::model::ModelObject& modelObject)
{
  LuminaireDefinitionInspectorView * luminaireDefinitionInspectorView = new LuminaireDefinitionInspectorView(m_isIP, m_model);
  connect(this, &RenewableEnergyInspectorView::toggleUnitsClicked, luminaireDefinitionInspectorView, &LuminaireDefinitionInspectorView::toggleUnitsClicked);

  luminaireDefinitionInspectorView->selectModelObject(modelObject);

  this->showInspector(luminaireDefinitionInspectorView);
}

void RenewableEnergyInspectorView::showElectricEquipmentDefinitionsInspector(const openstudio::model::ModelObject& modelObject)
{
  ElectricEquipmentDefinitionInspectorView * electricEquipmentDefinitionInspectorView = new ElectricEquipmentDefinitionInspectorView(m_isIP, m_model);
  connect(this, &RenewableEnergyInspectorView::toggleUnitsClicked, electricEquipmentDefinitionInspectorView, &ElectricEquipmentDefinitionInspectorView::toggleUnitsClicked);

  electricEquipmentDefinitionInspectorView->selectModelObject(modelObject);

  this->showInspector(electricEquipmentDefinitionInspectorView);
}

void RenewableEnergyInspectorView::showGasEquipmentDefinitionsInspector(const openstudio::model::ModelObject& modelObject)
{
  GasEquipmentDefinitionInspectorView * gasEquipmentDefinitionInspectorView = new GasEquipmentDefinitionInspectorView(m_isIP, m_model);
  connect(this, &RenewableEnergyInspectorView::toggleUnitsClicked, gasEquipmentDefinitionInspectorView, &GasEquipmentDefinitionInspectorView::toggleUnitsClicked);

  gasEquipmentDefinitionInspectorView->selectModelObject(modelObject);

  this->showInspector(gasEquipmentDefinitionInspectorView);
}

void RenewableEnergyInspectorView::showSteamEquipmentDefinitionsInspector(const openstudio::model::ModelObject& modelObject)
{
  SteamEquipmentDefinitionInspectorView * steamEquipmentDefinitionInspectorView = new SteamEquipmentDefinitionInspectorView(m_isIP, m_model);
  connect(this, &RenewableEnergyInspectorView::toggleUnitsClicked, steamEquipmentDefinitionInspectorView, &SteamEquipmentDefinitionInspectorView::toggleUnitsClicked);

  steamEquipmentDefinitionInspectorView->selectModelObject(modelObject);

  this->showInspector(steamEquipmentDefinitionInspectorView);
}

void RenewableEnergyInspectorView::showOtherEquipmentDefinitionsInspector(const openstudio::model::ModelObject& modelObject)
{
  OtherEquipmentDefinitionInspectorView * otherEquipmentDefinitionInspectorView = new OtherEquipmentDefinitionInspectorView(m_isIP, m_model);
  connect(this, &RenewableEnergyInspectorView::toggleUnitsClicked, otherEquipmentDefinitionInspectorView, &OtherEquipmentDefinitionInspectorView::toggleUnitsClicked);

  otherEquipmentDefinitionInspectorView->selectModelObject(modelObject);

  this->showInspector(otherEquipmentDefinitionInspectorView);
}

void RenewableEnergyInspectorView::showWaterUseEquipmentDefinitionsInspector(const openstudio::model::ModelObject& modelObject)
{
  WaterUseEquipmentDefinitionInspectorView * waterUseEquipmentDefinitionInspectorView = new WaterUseEquipmentDefinitionInspectorView(m_isIP, m_model);
  connect(this, &RenewableEnergyInspectorView::toggleUnitsClicked, waterUseEquipmentDefinitionInspectorView, &WaterUseEquipmentDefinitionInspectorView::toggleUnitsClicked);

  waterUseEquipmentDefinitionInspectorView->selectModelObject(modelObject);

  showInspector(waterUseEquipmentDefinitionInspectorView);
}

void RenewableEnergyInspectorView::showPhotovoltaicInspector(const openstudio::model::ModelObject& modelObject)
{
	PhotovoltaicInspectorView * photovoltaicInspectorView = new PhotovoltaicInspectorView(m_isIP, m_model);
	connect(this, &RenewableEnergyInspectorView::toggleUnitsClicked, photovoltaicInspectorView, &PhotovoltaicInspectorView::toggleUnitsClicked);

	photovoltaicInspectorView->selectModelObject(modelObject);

	showInspector(photovoltaicInspectorView);
}

void RenewableEnergyInspectorView::showPhotovoltaicThermalInspector(const openstudio::model::ModelObject& modelObject)
{
	PhotovoltaicThermalInspectorView * photovoltaicThermalInspectorView = new PhotovoltaicThermalInspectorView(m_isIP, m_model);
	connect(this, &RenewableEnergyInspectorView::toggleUnitsClicked, photovoltaicThermalInspectorView, &PhotovoltaicThermalInspectorView::toggleUnitsClicked);

	photovoltaicThermalInspectorView->selectModelObject(modelObject);

	showInspector(photovoltaicThermalInspectorView);
}

void RenewableEnergyInspectorView::showInternalMassDefinitionsInspector(const openstudio::model::ModelObject& modelObject)
{
  InternalMassDefinitionInspectorView * internalMassDefinitionInspectorView = new InternalMassDefinitionInspectorView(m_isIP, m_model);
  connect(this, &RenewableEnergyInspectorView::toggleUnitsClicked, internalMassDefinitionInspectorView, &InternalMassDefinitionInspectorView::toggleUnitsClicked);

  internalMassDefinitionInspectorView->selectModelObject(modelObject);

  this->showInspector(internalMassDefinitionInspectorView);
}

void RenewableEnergyInspectorView::showDefaultView()
{
  if( QWidget * widget = this->stackedWidget()->currentWidget() )
  {
    this->stackedWidget()->removeWidget(widget);

    delete widget;
  }
}

void RenewableEnergyInspectorView::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

} // openstudio
