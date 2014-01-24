/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
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

#include <openstudio_lib/ThermalZonesController.hpp>
#include <openstudio_lib/ThermalZonesView.hpp>
#include <openstudio_lib/OSAppBase.hpp>
#include <openstudio_lib/OSDocument.hpp>
#include <openstudio_lib/OSItemSelectorButtons.hpp>

#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/ZoneHVACComponent.hpp>
#include <model/ZoneHVACComponent_Impl.hpp>
#include <model/AirLoopHVAC.hpp>
#include <model/AirLoopHVAC_Impl.hpp>
#include <model/Space.hpp>
#include <model/Space_Impl.hpp>
#include <model/ThermostatSetpointDualSetpoint.hpp>
#include <model/ThermostatSetpointDualSetpoint_Impl.hpp>

#include <utilities/core/Assert.hpp>

#include <QApplication>
#include <QInputDialog>
#include <QMessageBox>

#include <boost/foreach.hpp>

namespace openstudio {

ThermalZonesController::ThermalZonesController(bool isIP, const model::Model & model)
  : ModelSubTabController(new ThermalZonesView(isIP,model),model)
{
  subTabView()->itemSelectorButtons()->disableCopyButton();

  //bool isConnected = false;

  QWidget * thermalZoneView = subTabView()->inspectorView(); 

  connect( thermalZoneView,SIGNAL(enableThermostatClicked(model::ThermalZone &, bool)),
           this,SLOT(enableThermostat(model::ThermalZone &, bool)) );

  connect( thermalZoneView,SIGNAL(modelObjectSelected(model::OptionalModelObject &, bool )),
           this,SIGNAL(modelObjectSelected(model::OptionalModelObject &, bool )) );

  connect( this,SIGNAL(toggleUnitsClicked( bool )),
           thermalZoneView,SIGNAL(toggleUnitsClicked( bool )) );

  //isConnected = connect(m_thermalZonesView,SIGNAL(componentDropped(model::ThermalZone &, Handle &)),
  //        this,SLOT(addComponentToZone(model::ThermalZone &, Handle &)));
  //OS_ASSERT(isConnected);

  //isConnected = connect( m_thermalZonesView,SIGNAL(removeZoneHVACComponentClicked( model::ZoneHVACComponent & )),
  //         this,SLOT(removeZoneHVACComponent( model::ZoneHVACComponent & )) );
  //OS_ASSERT(isConnected);

  //isConnected = connect( m_thermalZonesView,SIGNAL(zoneHVACComponentClicked( model::ZoneHVACComponent & )),
  //         this,SLOT(onZoneHVACComponentClicked( model::ZoneHVACComponent & )) );
  //OS_ASSERT(isConnected);
}

//void ThermalZonesController::removeThermalZone(model::ThermalZone & thermalZone)
//{
//  if( model::OptionalAirLoopHVAC airLoop = thermalZone.airLoopHVAC() )
//  {
//    airLoop->removeBranchForZone(thermalZone);
//
//    QApplication::processEvents();
//  }
//
//  thermalZone.remove();
//}

void ThermalZonesController::enableThermostat(model::ThermalZone & thermalZone, bool enable)
{
  if( enable )
  {
    model::ThermostatSetpointDualSetpoint thermostat(model());

    thermalZone.setThermostatSetpointDualSetpoint(thermostat);
  }
  else
  {
    thermalZone.resetThermostatSetpointDualSetpoint();
  }
}

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

    for( std::vector<model::ModelObject>::iterator it = existingComps.begin();
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
  //subTabView()->itemSelectorButtons()->enableCopyButton();
  subTabView()->itemSelectorButtons()->enableRemoveButton();
  subTabView()->itemSelectorButtons()->enablePurgeButton();
}

void ThermalZonesController::onAddObject(const openstudio::IddObjectType& iddObjectType)
{
  OS_ASSERT(IddObjectType::OS_ThermalZone == iddObjectType.value());
  openstudio::model::ThermalZone(this->model());
}

void ThermalZonesController::onCopyObject(const openstudio::model::ModelObject& modelObject)
{
  //DLM: ThermalZone::clone is not implemented yet
  //modelObject.clone(this->model());
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
  BOOST_FOREACH(model::ThermalZone thermalZone, this->model().getModelObjects<model::ThermalZone>()){
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

