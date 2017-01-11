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

#include "ThermalZonesController.hpp"

#include "OSAppBase.hpp"
#include "OSDocument.hpp"
#include "OSItemSelectorButtons.hpp"
#include "OSItem.hpp"
#include "ThermalZonesView.hpp"

#include "../model/AirLoopHVAC.hpp"
#include "../model/AirLoopHVAC_Impl.hpp"
#include "../model/Space.hpp"
#include "../model/Space_Impl.hpp"
#include "../model/ThermalZone.hpp"
#include "../model/ThermalZone_Impl.hpp"
#include "../model/ThermostatSetpointDualSetpoint.hpp"
#include "../model/ThermostatSetpointDualSetpoint_Impl.hpp"
#include "../model/ZoneControlHumidistat.hpp"
#include "../model/ZoneControlHumidistat_Impl.hpp"
#include "../model/ZoneHVACComponent.hpp"
#include "../model/ZoneHVACComponent_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <utilities/idd/IddEnums.hxx>

#include <QApplication>
#include <QMessageBox>

namespace openstudio {

ThermalZonesController::ThermalZonesController(bool isIP, const model::Model & model)
  : ModelSubTabController(new ThermalZonesView(isIP,model),model)
{
  //subTabView()->itemSelectorButtons()->disableCopyButton(); 

  ThermalZoneView * thermalZoneView = static_cast<ThermalZoneView *>(subTabView()->inspectorView());

  //connect(thermalZoneView, &ThermalZoneView::enableThermostatClicked, this, &ThermalZonesController::enableThermostat);

  //connect(thermalZoneView, &ThermalZoneView::enableHumidistatClicked, this, &ThermalZonesController::enableHumidistat);

  connect(thermalZoneView, &ThermalZoneView::modelObjectSelected, this, &ThermalZonesController::modelObjectSelected);

  connect(thermalZoneView, &ThermalZoneView::dropZoneItemSelected, this, &ThermalZonesController::dropZoneItemSelected);

  connect(this, &ThermalZonesController::toggleUnitsClicked, thermalZoneView, &ThermalZoneView::toggleUnitsClicked);
}

//void ThermalZonesController::enableThermostat(model::ThermalZone & thermalZone, bool enable)
//{
//  if( enable )
//  {
//    model::ThermostatSetpointDualSetpoint thermostat(model());
//
//    thermalZone.setThermostatSetpointDualSetpoint(thermostat);
//  }
//  else
//  {
//    thermalZone.resetThermostatSetpointDualSetpoint();
//  }
//}

void ThermalZonesController::setHeatingSchedule(model::ThermalZone & thermalZone, model::OptionalSchedule & schedule)
{
  model::OptionalThermostatSetpointDualSetpoint thermostat = thermalZone.thermostatSetpointDualSetpoint();

  if( thermostat )
  {
    if( schedule )
    {
      thermostat->setHeatingSchedule(schedule.get());
    }
    else
    {
      thermostat->resetHeatingSchedule();
    }
  }
}

void ThermalZonesController::setCoolingSchedule(model::ThermalZone & thermalZone, model::OptionalSchedule & schedule)
{
  model::OptionalThermostatSetpointDualSetpoint thermostat = thermalZone.thermostatSetpointDualSetpoint();

  if( thermostat )
  {
    if( schedule )
    {
      thermostat->setCoolingSchedule(schedule.get());
    }
    else
    {
      thermostat->resetCoolingSchedule();
    }
  }
}

//void ThermalZonesController::enableHumidistat(model::ThermalZone & thermalZone, bool enable)
//{
//  if( enable )
//  {
//    model::ZoneControlHumidistat humidistat(model());
//
//    thermalZone.setZoneControlHumidistat(humidistat);
//  }
//  else
//  {
//    thermalZone.resetZoneControlHumidistat();
//  }
//}

void ThermalZonesController::setHumidifyingSchedule(model::ThermalZone & thermalZone, model::OptionalSchedule & schedule)
{
  model::OptionalZoneControlHumidistat humidistat = thermalZone.zoneControlHumidistat();

  if( humidistat )
  {
    if( schedule )
    {
      humidistat->setHumidifyingRelativeHumiditySetpointSchedule(schedule.get());
    }
    else
    {
      humidistat->resetHumidifyingRelativeHumiditySetpointSchedule();
    }
  }
}

void ThermalZonesController::setDehumidifyingSchedule(model::ThermalZone & thermalZone, model::OptionalSchedule & schedule)
{
  model::OptionalZoneControlHumidistat humidistat = thermalZone.zoneControlHumidistat();

  if( humidistat )
  {
    if( schedule )
    {
      humidistat->setDehumidifyingRelativeHumiditySetpointSchedule(schedule.get());
    }
    else
    {
      humidistat->resetDehumidifyingRelativeHumiditySetpointSchedule();
    }
  }
}

void ThermalZonesController::addComponentToZone(model::ThermalZone & zone, Handle & h)
{
  boost::optional<model::ZoneHVACComponent> libraryComp;

  model::Model library = OSAppBase::instance()->currentDocument()->componentLibrary();
  libraryComp = library.getModelObject<model::ZoneHVACComponent>(h);

  if( libraryComp )
  {
    std::vector<model::ModelObject> existingComps;
    existingComps = zone.equipment();

    std::vector<model::ZoneHVACComponent> exisitngHVACComps;

    for( auto it = existingComps.begin();
         it < existingComps.end();
         ++it )
    {
      if( boost::optional<model::ZoneHVACComponent> hvacComp = it->optionalCast<model::ZoneHVACComponent>() )
      {
        exisitngHVACComps.push_back(hvacComp.get());
      }
    }

    if( exisitngHVACComps.size() > 0 )
    {
      QMessageBox message(subTabView());
      
      message.setText("Sorry, only one piece of zone equipment is allowed at this time.");

      message.exec();

      return;
    }
  }

  bool wasSuccessful = false;

  if( libraryComp )
  {
    model::ZoneHVACComponent compClone = libraryComp->clone(model()).cast<model::ZoneHVACComponent>();
  
    bool added = compClone.addToThermalZone(zone);

    if( added )
    {
      wasSuccessful = true;
    }
    else
    {
      compClone.remove();
    }
  }

  if( ! wasSuccessful )
  {
    QMessageBox message(subTabView());
    
    message.setText("The selected component is not allowed at this location.");

    message.exec();
  }
}

void ThermalZonesController::removeZoneHVACComponent( model::ZoneHVACComponent & zoneHVACComponent )
{
  zoneHVACComponent.removeFromThermalZone();

  QApplication::processEvents();

  zoneHVACComponent.remove();
}

void ThermalZonesController::onZoneHVACComponentClicked( model::ZoneHVACComponent & comp )
{
  boost::optional<model::ModelObject> mo = comp;

  emit modelObjectSelected(mo, false);
}

void ThermalZonesController::onSelectItem(OSItem *item)
{
  subTabView()->inspectorView()->selectItem(item);
  subTabView()->itemSelectorButtons()->enableCopyButton();
  subTabView()->itemSelectorButtons()->enableRemoveButton();
  subTabView()->itemSelectorButtons()->enablePurgeButton();
}

void ThermalZonesController::onAddObject(const openstudio::IddObjectType& iddObjectType)
{
  OS_ASSERT(IddObjectType::OS_ThermalZone == iddObjectType.value());
  openstudio::model::ThermalZone(this->model());
}

void ThermalZonesController::onAddObject(const openstudio::model::ModelObject& modelObject)
{
  OS_ASSERT(IddObjectType::OS_ThermalZone == modelObject.iddObjectType().value());
  openstudio::model::ThermalZone(this->model());
}

void ThermalZonesController::onCopyObject(const openstudio::model::ModelObject& modelObject)
{
  modelObject.clone(this->model());
}

void ThermalZonesController::onRemoveObject(openstudio::model::ModelObject modelObject)
{
  // DLM: should this check isRemovable?
  modelObject.remove();
}

void ThermalZonesController::onReplaceObject(openstudio::model::ModelObject modelObject, const OSItemId& replacementItemId)
{
  // not yet implemented
}

void ThermalZonesController::onPurgeObjects(const openstudio::IddObjectType& iddObjectType)
{
  //std::vector<Handle> toRemove;
  for (model::ThermalZone thermalZone : this->model().getConcreteModelObjects<model::ThermalZone>()){
    if (thermalZone.spaces().empty() && thermalZone.isRemovable()){
      //toRemove.push_back(thermalZone.handle());

      // DLM: just remove it
      thermalZone.remove();
    }
  }
  
  // DLM: this method doesn't actually call ThermalZone::remove which is important to preserve model validity
  //this->model().removeObjects(toRemove);
}

void ThermalZonesController::onDrop(const OSItemId& itemId)
{
  boost::optional<model::ModelObject> modelObject = this->getModelObject(itemId);
  if(modelObject){
    if(modelObject->optionalCast<model::ThermalZone>()){
      if (this->fromComponentLibrary(itemId)){
        //DLM: ThermalZone::clone is not implemented yet
        //modelObject = modelObject->clone(this->model());
      }
    }
  }
}

void ThermalZonesController::onInspectItem(OSItem* item)
{
}

} // openstudio
