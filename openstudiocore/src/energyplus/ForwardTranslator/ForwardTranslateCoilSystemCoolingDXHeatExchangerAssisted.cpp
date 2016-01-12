/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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
#include "../../model/CoilSystemCoolingDXHeatExchangerAssisted.hpp"
#include "../../model/CoilSystemCoolingDXHeatExchangerAssisted_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/HeatExchangerAirToAirSensibleAndLatent.hpp"
#include "../../model/HeatExchangerAirToAirSensibleAndLatent_Impl.hpp"
#include "../../model/CoilCoolingDXSingleSpeed.hpp"
#include "../../model/CoilCoolingDXSingleSpeed_Impl.hpp"
#include "../../model/Model.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/CoilSystem_Cooling_DX_HeatExchangerAssisted_FieldEnums.hxx>
#include <utilities/idd/HeatExchanger_AirToAir_SensibleAndLatent_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_SingleSpeed_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_SingleSpeed_ThermalStorage_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_MultiSpeed_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_TwoSpeed_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_TwoStageWithHumidityControlMode_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_VariableSpeed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateCoilSystemCoolingDXHeatExchangerAssisted( 
    CoilSystemCoolingDXHeatExchangerAssisted & modelObject)
{
  IdfObject idfObject(IddObjectType::CoilSystem_Cooling_DX_HeatExchangerAssisted);
  m_idfObjects.push_back(idfObject);

  // Name
  if( auto s = modelObject.name() ) {
    idfObject.setName(*s);
  }

  std::string hxSupplyAirInletNodeName;
  // InletNodeName
  if( auto mo = modelObject.inletModelObject() ) {
    if( auto node = mo->optionalCast<Node>() ) {
      hxSupplyAirInletNodeName = node->name().get();
    }
  }

  std::string hxExhaustAirOutletNodeName;
  // OutletNodeName
  if( auto mo = modelObject.outletModelObject() ) {
    if( auto node = mo->optionalCast<Node>() ) {
      hxExhaustAirOutletNodeName = node->name().get();
    }
  }

  std::string hxSupplyAirOutletNodeName = modelObject.name().get() + " HX Supply Air Outlet - Cooling Inlet Node";
  std::string hxExhaustAirInletNodeName = modelObject.name().get() + " HX Exhaust Air Inlet - Cooling Outlet Node";

  // HeatExchangerObjectType
  // HeatExchangerName
  {
    auto hx = modelObject.heatExchanger();
    if( auto idf = translateAndMapModelObject(hx) ) {
      idfObject.setString(CoilSystem_Cooling_DX_HeatExchangerAssistedFields::HeatExchangerObjectType,idf->iddObject().name());
      idfObject.setString(CoilSystem_Cooling_DX_HeatExchangerAssistedFields::HeatExchangerName,idf->name().get());
      if( idf->iddObject().type() == IddObjectType::HeatExchanger_AirToAir_SensibleAndLatent ) {
        idf->setString(HeatExchanger_AirToAir_SensibleAndLatentFields::SupplyAirInletNodeName,hxSupplyAirInletNodeName);
        idf->setString(HeatExchanger_AirToAir_SensibleAndLatentFields::SupplyAirOutletNodeName,hxSupplyAirOutletNodeName);
        idf->setString(HeatExchanger_AirToAir_SensibleAndLatentFields::ExhaustAirOutletNodeName,hxExhaustAirOutletNodeName);
        idf->setString(HeatExchanger_AirToAir_SensibleAndLatentFields::ExhaustAirInletNodeName,hxExhaustAirInletNodeName);
      }
    }
  }

  // CoolingCoilObjectType
  // CoolingCoilName
  {
    auto coolingCoil = modelObject.coolingCoil();
    if( auto idf = translateAndMapModelObject(coolingCoil) ) {
      idfObject.setString(CoilSystem_Cooling_DX_HeatExchangerAssistedFields::CoolingCoilObjectType,idf->iddObject().name());
      idfObject.setString(CoilSystem_Cooling_DX_HeatExchangerAssistedFields::CoolingCoilName,idf->name().get());
      if( idf->iddObject().type() == IddObjectType::Coil_Cooling_DX_SingleSpeed ) {
        idf->setString(Coil_Cooling_DX_SingleSpeedFields::AirInletNodeName,hxSupplyAirOutletNodeName);
        idf->setString(Coil_Cooling_DX_SingleSpeedFields::AirOutletNodeName,hxExhaustAirInletNodeName);
      } else if( idf->iddObject().type() == IddObjectType::Coil_Cooling_DX_MultiSpeed ) {
        idf->setString(Coil_Cooling_DX_MultiSpeedFields::AirInletNodeName,hxSupplyAirOutletNodeName);
        idf->setString(Coil_Cooling_DX_MultiSpeedFields::AirOutletNodeName,hxExhaustAirInletNodeName);
      } else if( idf->iddObject().type() == IddObjectType::Coil_Cooling_DX_SingleSpeed_ThermalStorage ) {
        idf->setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondenserAirInletNodeName,hxSupplyAirOutletNodeName);
        idf->setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondenserAirOutletNodeName,hxExhaustAirInletNodeName);
      } else if( idf->iddObject().type() == IddObjectType::Coil_Cooling_DX_TwoSpeed ) {
        idf->setString(Coil_Cooling_DX_TwoSpeedFields::AirInletNodeName,hxSupplyAirOutletNodeName);
        idf->setString(Coil_Cooling_DX_TwoSpeedFields::AirOutletNodeName,hxExhaustAirInletNodeName);
      } else if( idf->iddObject().type() == IddObjectType::Coil_Cooling_DX_TwoStageWithHumidityControlMode ) {
        idf->setString(Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::AirInletNodeName,hxSupplyAirOutletNodeName);
        idf->setString(Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::AirOutletNodeName,hxExhaustAirInletNodeName);
      } else if( idf->iddObject().type() == IddObjectType::Coil_Cooling_DX_VariableSpeed ) {
        idf->setString(Coil_Cooling_DX_VariableSpeedFields::IndoorAirInletNodeName,hxSupplyAirOutletNodeName);
        idf->setString(Coil_Cooling_DX_VariableSpeedFields::IndoorAirOutletNodeName,hxExhaustAirInletNodeName);
      }
    }
  }

  return idfObject;
}

} // energyplus
} // openstudio
