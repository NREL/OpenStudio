/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

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
      } else if( idf->iddObject().type() == IddObjectType::Coil_Cooling_DX_VariableSpeed ) {
        idf->setString(Coil_Cooling_DX_VariableSpeedFields::IndoorAirInletNodeName,hxSupplyAirOutletNodeName);
        idf->setString(Coil_Cooling_DX_VariableSpeedFields::IndoorAirOutletNodeName,hxExhaustAirInletNodeName);
      } else {
        // Shouldn't happen, accepts only Coil:Cooling:DX:SingleSpeed or Coil:Cooling:DX:VariableSpeed
        LOG(Fatal, modelObject.briefDescription() << " appears to have a cooling coil that shouldn't have been accepted: "
            << coolingCoil.briefDescription());
        OS_ASSERT(false);
      }
    }
  }

  return idfObject;
}

} // energyplus
} // openstudio
