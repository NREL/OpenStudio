/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/CoilSystemCoolingDXHeatExchangerAssisted.hpp"
#include "../../model/CoilSystemCoolingDXHeatExchangerAssisted_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/HeatExchangerAirToAirSensibleAndLatent.hpp"
#include "../../model/HeatExchangerAirToAirSensibleAndLatent_Impl.hpp"
#include "../../model/CoilCoolingDX.hpp"
#include "../../model/CoilCoolingDX_Impl.hpp"
#include "../../model/CoilCoolingDXSingleSpeed.hpp"
#include "../../model/CoilCoolingDXSingleSpeed_Impl.hpp"
#include "../../model/CoilCoolingDXVariableSpeed.hpp"
#include "../../model/CoilCoolingDXVariableSpeed_Impl.hpp"
#include "../../model/Model.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/CoilSystem_Cooling_DX_HeatExchangerAssisted_FieldEnums.hxx>
#include <utilities/idd/HeatExchanger_AirToAir_SensibleAndLatent_FieldEnums.hxx>
#include <utilities/idd/HeatExchanger_Desiccant_BalancedFlow_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_SingleSpeed_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_VariableSpeed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateCoilSystemCoolingDXHeatExchangerAssisted(CoilSystemCoolingDXHeatExchangerAssisted& modelObject) {
    IdfObject idfObject(IddObjectType::CoilSystem_Cooling_DX_HeatExchangerAssisted);
    m_idfObjects.push_back(idfObject);

    // Name
    if (auto s = modelObject.name()) {
      idfObject.setName(*s);
    }

    std::string hxSupplyAirInletNodeName;
    // InletNodeName
    if (auto mo = modelObject.inletModelObject()) {
      if (auto node = mo->optionalCast<Node>()) {
        hxSupplyAirInletNodeName = node->name().get();
      }
    }

    std::string hxExhaustAirOutletNodeName;
    // OutletNodeName
    if (auto mo = modelObject.outletModelObject()) {
      if (auto node = mo->optionalCast<Node>()) {
        hxExhaustAirOutletNodeName = node->name().get();
      }
    }

    std::string hxSupplyAirOutletNodeName = modelObject.name().get() + " HX Supply Air Outlet - Cooling Inlet Node";
    std::string hxExhaustAirInletNodeName = modelObject.name().get() + " HX Exhaust Air Inlet - Cooling Outlet Node";

    // HeatExchangerObjectType
    // HeatExchangerName
    {
      auto hx = modelObject.heatExchanger();
      if (auto idf = translateAndMapModelObject(hx)) {
        idfObject.setString(CoilSystem_Cooling_DX_HeatExchangerAssistedFields::HeatExchangerObjectType, idf->iddObject().name());
        idfObject.setString(CoilSystem_Cooling_DX_HeatExchangerAssistedFields::HeatExchangerName, idf->name().get());
        if (idf->iddObject().type() == IddObjectType::HeatExchanger_AirToAir_SensibleAndLatent) {
          idf->setString(HeatExchanger_AirToAir_SensibleAndLatentFields::SupplyAirInletNodeName, hxSupplyAirInletNodeName);
          idf->setString(HeatExchanger_AirToAir_SensibleAndLatentFields::SupplyAirOutletNodeName, hxSupplyAirOutletNodeName);
          idf->setString(HeatExchanger_AirToAir_SensibleAndLatentFields::ExhaustAirOutletNodeName, hxExhaustAirOutletNodeName);
          idf->setString(HeatExchanger_AirToAir_SensibleAndLatentFields::ExhaustAirInletNodeName, hxExhaustAirInletNodeName);
        } else if (idf->iddObject().type() == IddObjectType::HeatExchanger_Desiccant_BalancedFlow) {
          idf->setString(HeatExchanger_Desiccant_BalancedFlowFields::RegenerationAirInletNodeName, hxSupplyAirInletNodeName);
          idf->setString(HeatExchanger_Desiccant_BalancedFlowFields::RegenerationAirOutletNodeName, hxSupplyAirOutletNodeName);
          idf->setString(HeatExchanger_Desiccant_BalancedFlowFields::ProcessAirOutletNodeName, hxExhaustAirOutletNodeName);
          idf->setString(HeatExchanger_Desiccant_BalancedFlowFields::ProcessAirInletNodeName, hxExhaustAirInletNodeName);
        } else {
          // Shouldn't happen
          LOG(Fatal, modelObject.briefDescription()
                       << " appears to have a heat exchanger that shouldn't have been accepted: " << hx.briefDescription());
          OS_ASSERT(false);
        }
      }
    }

    // CoolingCoilObjectType
    // CoolingCoilName
    {
      auto coolingCoil = modelObject.coolingCoil();
      if (auto idf = translateAndMapModelObject(coolingCoil)) {
        idfObject.setString(CoilSystem_Cooling_DX_HeatExchangerAssistedFields::CoolingCoilObjectType, idf->iddObject().name());
        idfObject.setString(CoilSystem_Cooling_DX_HeatExchangerAssistedFields::CoolingCoilName, idf->name().get());
        if (idf->iddObject().type() == IddObjectType::Coil_Cooling_DX_SingleSpeed) {
          idf->setString(Coil_Cooling_DX_SingleSpeedFields::AirInletNodeName, hxSupplyAirOutletNodeName);
          idf->setString(Coil_Cooling_DX_SingleSpeedFields::AirOutletNodeName, hxExhaustAirInletNodeName);
        } else if (idf->iddObject().type() == IddObjectType::Coil_Cooling_DX_VariableSpeed) {
          idf->setString(Coil_Cooling_DX_VariableSpeedFields::IndoorAirInletNodeName, hxSupplyAirOutletNodeName);
          idf->setString(Coil_Cooling_DX_VariableSpeedFields::IndoorAirOutletNodeName, hxExhaustAirInletNodeName);
        } else if (idf->iddObject().type() == IddObjectType::Coil_Cooling_DX) {
          idf->setString(Coil_Cooling_DXFields::EvaporatorInletNodeName, hxSupplyAirOutletNodeName);
          idf->setString(Coil_Cooling_DXFields::EvaporatorOutletNodeName, hxExhaustAirInletNodeName);
        } else {
          // Shouldn't happen, accepts only Coil:Cooling:DX or Coil:Cooling:DX:SingleSpeed or Coil:Cooling:DX:VariableSpeed
          LOG(Fatal, modelObject.briefDescription()
                       << " appears to have a cooling coil that shouldn't have been accepted: " << coolingCoil.briefDescription());
          OS_ASSERT(false);
        }
      }
    }

    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
