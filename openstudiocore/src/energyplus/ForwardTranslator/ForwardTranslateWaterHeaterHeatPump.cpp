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

#include "../ForwardTranslator.hpp"
#include "../../model/WaterHeaterHeatPump.hpp"
#include "../../model/WaterHeaterHeatPump_Impl.hpp"
#include "../../model/WaterHeaterMixed.hpp"
#include "../../model/WaterHeaterMixed_Impl.hpp"
#include "../../model/WaterHeaterStratified.hpp"
#include "../../model/WaterHeaterStratified_Impl.hpp"
#include "../../model/CoilWaterHeatingAirToWaterHeatPump.hpp"
#include "../../model/CoilWaterHeatingAirToWaterHeatPump_Impl.hpp"
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/FanOnOff.hpp"
#include "../../model/FanOnOff_Impl.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/WaterHeater_HeatPump_FieldEnums.hxx>
#include <utilities/idd/Coil_WaterHeating_AirToWaterHeatPump_FieldEnums.hxx>
#include <utilities/idd/WaterHeater_Mixed_FieldEnums.hxx>
#include <utilities/idd/WaterHeater_Stratified_FieldEnums.hxx>
#include <utilities/idd/Fan_OnOff_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateWaterHeaterHeatPump( 
    WaterHeaterHeatPump & modelObject)
{
  IdfObject idfObject(IddObjectType::WaterHeater_HeatPump);
  m_idfObjects.push_back(idfObject);

  // Name
  if( auto s = modelObject.name() ) {
    idfObject.setName(*s);
  }

  if( auto mo = modelObject.availabilitySchedule() ) {
    idfObject.setString(WaterHeater_HeatPumpFields::AvailabilityScheduleName,mo->name().get());
  }

  {
    auto mo = modelObject.compressorSetpointTemperatureSchedule();
    idfObject.setString(WaterHeater_HeatPumpFields::CompressorSetpointTemperatureScheduleName,mo.name().get());
  }

  {
    auto value = modelObject.deadBandTemperatureDifference();
    idfObject.setDouble(WaterHeater_HeatPumpFields::DeadBandTemperatureDifference,value);
  }

  if( modelObject.isCondenserWaterFlowRateAutosized() ) {
    idfObject.setString(WaterHeater_HeatPumpFields::CondenserWaterFlowRate,"Autosize");
  } else if( auto value = modelObject.condenserWaterFlowRate() ) {
    idfObject.setDouble(WaterHeater_HeatPumpFields::CondenserWaterFlowRate,value.get());
  }

  if( modelObject.isEvaporatorAirFlowRateAutosized() ) {
    idfObject.setString(WaterHeater_HeatPumpFields::EvaporatorAirFlowRate,"Autosize");
  } else if( auto value = modelObject.evaporatorAirFlowRate() ) {
    idfObject.setDouble(WaterHeater_HeatPumpFields::EvaporatorAirFlowRate,value.get());
  }

  {
    auto value = modelObject.inletAirConfiguration();
    idfObject.setString(WaterHeater_HeatPumpFields::InletAirConfiguration,value);
  }

  if( auto mo = modelObject.inletAirTemperatureSchedule() ) {
    idfObject.setString(WaterHeater_HeatPumpFields::InletAirTemperatureScheduleName,mo->name().get());
  }

  if( auto mo = modelObject.inletAirHumiditySchedule() ) {
    idfObject.setString(WaterHeater_HeatPumpFields::InletAirHumidityScheduleName,mo->name().get());
  }

  {
    auto value = modelObject.minimumInletAirTemperatureforCompressorOperation();
    idfObject.setDouble(WaterHeater_HeatPumpFields::MinimumInletAirTemperatureforCompressorOperation,value);
  }

  {
    auto value = modelObject.compressorLocation();
    idfObject.setString(WaterHeater_HeatPumpFields::CompressorLocation,value);
  }

  if( auto mo = modelObject.compressorAmbientTemperatureSchedule() ) {
    idfObject.setString(WaterHeater_HeatPumpFields::CompressorAmbientTemperatureScheduleName,mo->name().get());
  }

  {
    auto value = modelObject.fanPlacement();
    idfObject.setString(WaterHeater_HeatPumpFields::FanPlacement,value);
  }

  {
    auto value = modelObject.onCycleParasiticElectricLoad();
    idfObject.setDouble(WaterHeater_HeatPumpFields::OnCycleParasiticElectricLoad,value);
  }

  {
    auto value = modelObject.offCycleParasiticElectricLoad();
    idfObject.setDouble(WaterHeater_HeatPumpFields::OffCycleParasiticElectricLoad,value);
  }

  {
    auto value = modelObject.parasiticHeatRejectionLocation();
    idfObject.setString(WaterHeater_HeatPumpFields::ParasiticHeatRejectionLocation,value);
  }

  {
    auto mo = modelObject.inletAirMixerSchedule();
    idfObject.setString(WaterHeater_HeatPumpFields::InletAirMixerScheduleName,mo.name().get());
  }

  {
    auto value = modelObject.controlSensorLocationInStratifiedTank();
    idfObject.setString(WaterHeater_HeatPumpFields::ControlSensorLocationInStratifiedTank,value);
  }

  std::string condOutletTankInletNodeName =
    modelObject.name().get() + " Condenser Outlet - Tank Inlet";

  std::string tankOutletCondInletNodeName = 
    modelObject.name().get() + " Tank Outlet - Condenser Inlet";

  idfObject.setString(WaterHeater_HeatPumpFields::CondenserWaterInletNodeName,tankOutletCondInletNodeName);
  idfObject.setString(WaterHeater_HeatPumpFields::CondenserWaterOutletNodeName,condOutletTankInletNodeName);

  std::string airInletNodeName;
  std::string airOutletNodeName;
  std::string outdoorAirNodeName;
  std::string exhaustAirNodeName;
  std::string inletAirZoneName;
  std::string inletAirMixerNodeName;
  std::string outletAirSplitterNodeName;
  std::string fanInletNodeName;
  std::string fanOutletNodeName;
  std::string evapInletNodeName;
  std::string evapOutletNodeName;

  auto inletAirConfiguration = modelObject.inletAirConfiguration();
  if( istringEqual(modelObject.fanPlacement(),"DrawThrough") ) {

    if( istringEqual(inletAirConfiguration,"ZoneAirOnly") ) {
      if( auto thermalZone = modelObject.thermalZone() ) {
        auto inletNode = modelObject.inletNode();
        OS_ASSERT(inletNode);
        airInletNodeName = inletNode->name().get();

        auto outletNode = modelObject.outletNode();
        OS_ASSERT(outletNode);
        airOutletNodeName = outletNode->name().get();
        
        inletAirZoneName = thermalZone->name().get();
        fanInletNodeName = modelObject.name().get() + " Evap Outlet - Fan Inlet";
        fanOutletNodeName = airOutletNodeName;
        evapInletNodeName = airInletNodeName;
        evapOutletNodeName = fanInletNodeName;
      }
    } else if( istringEqual(inletAirConfiguration,"ZoneAndOutdoorAir") ) {
      if( auto thermalZone = modelObject.thermalZone() ) {
        auto inletNode = modelObject.inletNode();
        OS_ASSERT(inletNode);
        airInletNodeName = inletNode->name().get();

        auto outletNode = modelObject.outletNode();
        OS_ASSERT(outletNode);
        airOutletNodeName = outletNode->name().get();

        outdoorAirNodeName = modelObject.name().get() + " Outdoor Air";
        exhaustAirNodeName = modelObject.name().get() + " Exhaust Air";
        inletAirZoneName = thermalZone->name().get();
        inletAirMixerNodeName = modelObject.name().get() + " Mixer Outlet - Evap Inlet";
        outletAirSplitterNodeName = modelObject.name().get() + " Fan Outlet - Splitter Inlet";
        fanInletNodeName = modelObject.name().get() + " Evap Outlet - Fan Inlet";
        fanOutletNodeName = outletAirSplitterNodeName;
        evapInletNodeName = inletAirMixerNodeName;
        evapOutletNodeName = fanInletNodeName;
      }
    } else if( istringEqual(inletAirConfiguration,"OutdoorAirOnly") ) {
      outdoorAirNodeName = modelObject.name().get() + " Outdoor Air";
      exhaustAirNodeName = modelObject.name().get() + " Exhaust Air";
      fanInletNodeName = modelObject.name().get() + " Evap Outlet - Fan Inlet";
      fanOutletNodeName = exhaustAirNodeName;
      evapInletNodeName = outdoorAirNodeName;
      evapOutletNodeName = fanInletNodeName;
    } else if( istringEqual(inletAirConfiguration,"Schedule") ) {
      airInletNodeName = modelObject.name().get() + " Inlet";
      airOutletNodeName = modelObject.name().get() + " Outlet";
      fanInletNodeName = modelObject.name().get() + " Evap Outlet - Fan Inlet";
      fanOutletNodeName = airOutletNodeName;
      evapInletNodeName = airInletNodeName;
      evapOutletNodeName = fanInletNodeName;
    }

  } else { // BlowThrough

    if( istringEqual(inletAirConfiguration,"ZoneAirOnly") ) {
      if( auto thermalZone = modelObject.thermalZone() ) {
        auto inletNode = modelObject.inletNode();
        OS_ASSERT(inletNode);
        airInletNodeName = inletNode->name().get();

        auto outletNode = modelObject.outletNode();
        OS_ASSERT(outletNode);
        airOutletNodeName = outletNode->name().get();
        
        inletAirZoneName = thermalZone->name().get();
        fanInletNodeName = airInletNodeName;
        fanOutletNodeName = modelObject.name().get() + " Fan Outlet - Evap Inlet";
        evapInletNodeName = fanOutletNodeName;
        evapOutletNodeName = airOutletNodeName;
      }
    } else if( istringEqual(inletAirConfiguration,"ZoneAndOutdoorAir") ) {
      if( auto thermalZone = modelObject.thermalZone() ) {
        auto inletNode = modelObject.inletNode();
        OS_ASSERT(inletNode);
        airInletNodeName = inletNode->name().get();

        auto outletNode = modelObject.outletNode();
        OS_ASSERT(outletNode);
        airOutletNodeName = outletNode->name().get();

        outdoorAirNodeName = modelObject.name().get() + " Outdoor Air";
        exhaustAirNodeName = modelObject.name().get() + " Exhaust Air";
        inletAirZoneName = thermalZone->name().get();
        inletAirMixerNodeName = modelObject.name().get() + " Mixer Outlet - Fan Inlet";
        outletAirSplitterNodeName = modelObject.name().get() + " Evap Outlet - Splitter Inlet";
        fanInletNodeName = inletAirMixerNodeName;
        fanOutletNodeName = modelObject.name().get() + " Fan Outlet - Evap Inlet";
        evapInletNodeName = fanOutletNodeName;
        evapOutletNodeName = outletAirSplitterNodeName;
      }
    } else if( istringEqual(inletAirConfiguration,"OutdoorAirOnly") ) {
      outdoorAirNodeName = modelObject.name().get() + " Outdoor Air";
      exhaustAirNodeName = modelObject.name().get() + " Exhaust Air";
      fanInletNodeName = outdoorAirNodeName;
      fanOutletNodeName = modelObject.name().get() + " Fan Outlet - Evap Inlet";
      evapInletNodeName = fanOutletNodeName;
      evapOutletNodeName = exhaustAirNodeName;
    } else if( istringEqual(inletAirConfiguration,"Schedule") ) {
      airInletNodeName = modelObject.name().get() + " Inlet";
      airOutletNodeName = modelObject.name().get() + " Outlet";
      fanInletNodeName = airInletNodeName;
      fanOutletNodeName = modelObject.name().get() + " Fan Outlet - Evap Inlet";
      evapInletNodeName = fanOutletNodeName;
      evapOutletNodeName = airOutletNodeName;
    }

  }

  {
    auto mo = modelObject.tank();
    if( auto idf = translateAndMapModelObject(mo) ) {
      idfObject.setString(WaterHeater_HeatPumpFields::TankName,idf->name().get());
      idfObject.setString(WaterHeater_HeatPumpFields::TankObjectType,idf->iddObject().name());
      if( mo.iddObjectType() == model::WaterHeaterMixed::iddObjectType() ) {
        idf->setString(WaterHeater_MixedFields::SourceSideOutletNodeName,tankOutletCondInletNodeName);
        idf->setString(WaterHeater_MixedFields::SourceSideInletNodeName,condOutletTankInletNodeName);
        auto waterHeaterMixed = mo.cast<model::WaterHeaterMixed>();
        if( auto node = waterHeaterMixed.supplyInletModelObject() ) {
          idfObject.setString(WaterHeater_HeatPumpFields::TankUseSideInletNodeName,node->name().get());
        }
        if( auto node = waterHeaterMixed.supplyOutletModelObject() ) {
          idfObject.setString(WaterHeater_HeatPumpFields::TankUseSideOutletNodeName,node->name().get());
        }
      } else if( mo.iddObjectType() == model::WaterHeaterStratified::iddObjectType() ) {
        idf->setString(WaterHeater_StratifiedFields::SourceSideOutletNodeName,tankOutletCondInletNodeName);
        idf->setString(WaterHeater_StratifiedFields::SourceSideInletNodeName,condOutletTankInletNodeName);
        auto waterHeaterStratified = mo.cast<model::WaterHeaterStratified>();
        if( auto node = waterHeaterStratified.supplyInletModelObject() ) {
          idfObject.setString(WaterHeater_HeatPumpFields::TankUseSideInletNodeName,node->name().get());
        }
        if( auto node = waterHeaterStratified.supplyOutletModelObject() ) {
          idfObject.setString(WaterHeater_HeatPumpFields::TankUseSideOutletNodeName,node->name().get());
        }
      } else {
        LOG(Error, modelObject.briefDescription() << " is attached to an unsupported type of tank: " << mo.briefDescription() );
      }
    }
  }

  {
    auto mo = modelObject.dXCoil();
    if( auto idf = translateAndMapModelObject(mo) ) {
      idfObject.setString(WaterHeater_HeatPumpFields::DXCoilName,idf->name().get());
      idfObject.setString(WaterHeater_HeatPumpFields::DXCoilObjectType,idf->iddObject().name());
      if( mo.iddObjectType() == model::CoilWaterHeatingAirToWaterHeatPump::iddObjectType() ) {
        idf->setString(Coil_WaterHeating_AirToWaterHeatPumpFields::CondenserWaterInletNodeName,tankOutletCondInletNodeName);
        idf->setString(Coil_WaterHeating_AirToWaterHeatPumpFields::CondenserWaterOutletNodeName,condOutletTankInletNodeName);
        idf->setString(Coil_WaterHeating_AirToWaterHeatPumpFields::EvaporatorAirInletNodeName,evapInletNodeName);
        idf->setString(Coil_WaterHeating_AirToWaterHeatPumpFields::EvaporatorAirOutletNodeName,evapOutletNodeName);
      } else {
        LOG(Error, modelObject.briefDescription() << " is attached to an unsupported type of coil: " << mo.briefDescription() );
      }
    }
  }

  {
    auto mo = modelObject.fan();
    if( auto idf = translateAndMapModelObject(mo) ) {
      idfObject.setString(WaterHeater_HeatPumpFields::FanName,idf->name().get());
      idfObject.setString(WaterHeater_HeatPumpFields::FanObjectType,idf->iddObject().name());
      if( mo.iddObjectType() == model::FanOnOff::iddObjectType() ) {
        idf->setString(Fan_OnOffFields::AirInletNodeName,fanInletNodeName);
        idf->setString(Fan_OnOffFields::AirOutletNodeName,fanOutletNodeName);
      } else {
        LOG(Error, modelObject.briefDescription() << " is attached to an unsupported type of fan: " << mo.briefDescription() );
      }
    }
  }

  idfObject.setString(WaterHeater_HeatPumpFields::AirInletNodeName,airInletNodeName);
  idfObject.setString(WaterHeater_HeatPumpFields::AirOutletNodeName,airOutletNodeName);
  idfObject.setString(WaterHeater_HeatPumpFields::OutdoorAirNodeName,outdoorAirNodeName);
  idfObject.setString(WaterHeater_HeatPumpFields::ExhaustAirNodeName,exhaustAirNodeName);
  idfObject.setString(WaterHeater_HeatPumpFields::InletAirZoneName,inletAirZoneName);
  idfObject.setString(WaterHeater_HeatPumpFields::InletAirMixerNodeName,inletAirMixerNodeName);
  idfObject.setString(WaterHeater_HeatPumpFields::OutletAirSplitterNodeName,outletAirSplitterNodeName);

  return idfObject;
}

} // energyplus
} // openstudio
