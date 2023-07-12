/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/CoilSystemCoolingWaterHeatExchangerAssisted.hpp"
#include "../../model/CoilSystemCoolingWaterHeatExchangerAssisted_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/HeatExchangerAirToAirSensibleAndLatent.hpp"
#include "../../model/HeatExchangerAirToAirSensibleAndLatent_Impl.hpp"
#include "../../model/CoilCoolingWater.hpp"
#include "../../model/CoilCoolingWater_Impl.hpp"
#include "../../model/ControllerWaterCoil.hpp"
#include "../../model/ControllerWaterCoil_Impl.hpp"

#include "../../model/AirLoopHVAC.hpp"
#include "../../model/FanVariableVolume.hpp"
#include "../../model/FanVariableVolume_Impl.hpp"
#include "../../model/FanConstantVolume.hpp"
#include "../../model/FanConstantVolume_Impl.hpp"
#include "../../model/FanOnOff.hpp"
#include "../../model/FanOnOff_Impl.hpp"
#include "../../model/FanSystemModel.hpp"
#include "../../model/FanSystemModel_Impl.hpp"

#include "../../model/Model.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/CoilSystem_Cooling_Water_HeatExchangerAssisted_FieldEnums.hxx>
#include <utilities/idd/HeatExchanger_AirToAir_SensibleAndLatent_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_Water_FieldEnums.hxx>
#include <utilities/idd/Controller_WaterCoil_FieldEnums.hxx>
#include <utilities/idd/SetpointManager_MixedAir_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateCoilSystemCoolingWaterHeatExchangerAssisted(CoilSystemCoolingWaterHeatExchangerAssisted& modelObject) {

    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::CoilSystem_Cooling_Water_HeatExchangerAssisted, modelObject);

    std::string hxSupplyAirInletNodeName;
    // InletNodeName
    if (auto mo = modelObject.inletModelObject()) {
      if (auto node = mo->optionalCast<Node>()) {
        hxSupplyAirInletNodeName = node->nameString();
      }
    }

    std::string hxExhaustAirOutletNodeName;
    // OutletNodeName
    if (auto mo = modelObject.outletModelObject()) {
      if (auto node = mo->optionalCast<Node>()) {
        hxExhaustAirOutletNodeName = node->nameString();
      }
    }

    std::string hxSupplyAirOutletNodeName = modelObject.nameString() + " HX Supply Air Outlet - Cooling Inlet Node";
    std::string hxExhaustAirInletNodeName = modelObject.nameString() + " HX Exhaust Air Inlet - Cooling Outlet Node";

    // HeatExchangerObjectType
    // HeatExchangerName
    {
      auto hx = modelObject.heatExchanger();
      if (auto idf = translateAndMapModelObject(hx)) {
        idfObject.setString(CoilSystem_Cooling_Water_HeatExchangerAssistedFields::HeatExchangerObjectType, idf->iddObject().name());
        idfObject.setString(CoilSystem_Cooling_Water_HeatExchangerAssistedFields::HeatExchangerName, idf->nameString());
        if (idf->iddObject().type() == IddObjectType::HeatExchanger_AirToAir_SensibleAndLatent) {
          idf->setString(HeatExchanger_AirToAir_SensibleAndLatentFields::SupplyAirInletNodeName, hxSupplyAirInletNodeName);
          idf->setString(HeatExchanger_AirToAir_SensibleAndLatentFields::SupplyAirOutletNodeName, hxSupplyAirOutletNodeName);
          idf->setString(HeatExchanger_AirToAir_SensibleAndLatentFields::ExhaustAirOutletNodeName, hxExhaustAirOutletNodeName);
          idf->setString(HeatExchanger_AirToAir_SensibleAndLatentFields::ExhaustAirInletNodeName, hxExhaustAirInletNodeName);
        }
      }
    }

    // CoolingCoilObjectType
    // CoolingCoilName
    {
      auto coolingCoil = modelObject.coolingCoil();
      if (auto idf = translateAndMapModelObject(coolingCoil)) {
        idfObject.setString(CoilSystem_Cooling_Water_HeatExchangerAssistedFields::CoolingCoilObjectType, idf->iddObject().name());
        idfObject.setString(CoilSystem_Cooling_Water_HeatExchangerAssistedFields::CoolingCoilName, idf->nameString());
        if (idf->iddObject().type() == IddObjectType::Coil_Cooling_Water) {
          idf->setString(Coil_Cooling_WaterFields::AirInletNodeName, hxSupplyAirOutletNodeName);
          idf->setString(Coil_Cooling_WaterFields::AirOutletNodeName, hxExhaustAirInletNodeName);
          // Add IddObjectType::Coil_Cooling_Water_DetailedGeometry if implemented
        } else {
          // Shouldn't happen, accepts only Coil:Cooling:Water or Coil:Cooling:Water:DetailedGeometry
          // Shouldn't happen, accepts only Coil:Cooling:DX:SingleSpeed or Coil:Cooling:DX:VariableSpeed
          LOG(Fatal, modelObject.briefDescription()
                       << " appears to have a cooling coil that shouldn't have been accepted: " << coolingCoil.briefDescription());
          OS_ASSERT(false);
        }
      }
      if (auto coilCoolingWater = coolingCoil.optionalCast<CoilCoolingWater>()) {
        if (auto controller = coilCoolingWater->controllerWaterCoil()) {
          if (auto idf = translateAndMapModelObject(controller.get())) {
            idf->setString(Controller_WaterCoilFields::SensorNodeName, hxExhaustAirInletNodeName);
          }
        }
      }

      // Need a SPM:MixedAir on the Coil:Cooling:Water outlet node (that we **created** just above in IDF directly, so it won't get picked up by the
      // ForwardTranslateAirLoopHVAC method)
      if (boost::optional<AirLoopHVAC> airLoop_ = modelObject.airLoopHVAC()) {
        std::vector<StraightComponent> fans;
        std::vector<ModelObject> supplyComponents = airLoop_->supplyComponents();

        for (auto it = supplyComponents.begin(); it != supplyComponents.end(); ++it) {
          if (auto fan_ = it->optionalCast<FanVariableVolume>()) {
            fans.insert(fans.begin(), std::move(*fan_));
          } else if (auto fan_ = it->optionalCast<FanConstantVolume>()) {
            fans.insert(fans.begin(), std::move(*fan_));
          } else if (auto fan_ = it->optionalCast<FanSystemModel>()) {
            fans.insert(fans.begin(), std::move(*fan_));
          } else if (auto fan_ = it->optionalCast<FanOnOff>()) {
            fans.insert(fans.begin(), std::move(*fan_));
          }
        }

        if (!fans.empty()) {
          // Fan closest to the supply outlet node
          StraightComponent fan = fans.front();
          OptionalNode inletNode = fan.inletModelObject()->optionalCast<Node>();
          OptionalNode outletNode = fan.outletModelObject()->optionalCast<Node>();

          // No reason this wouldn't be ok at this point really...
          // TODO: change to OS_ASSERT?
          if (inletNode && outletNode) {

            IdfObject idf_spm(IddObjectType::SetpointManager_MixedAir);
            idf_spm.setString(SetpointManager_MixedAirFields::Name, hxExhaustAirInletNodeName + " OS Default SPM");
            idf_spm.setString(SetpointManager_MixedAirFields::ControlVariable, "Temperature");

            Node supplyOutletNode = airLoop_->supplyOutletNode();
            idf_spm.setString(SetpointManager_MixedAirFields::ReferenceSetpointNodeName, supplyOutletNode.nameString());

            idf_spm.setString(SetpointManager_MixedAirFields::FanInletNodeName, inletNode->nameString());
            idf_spm.setString(SetpointManager_MixedAirFields::FanOutletNodeName, outletNode->nameString());

            idf_spm.setString(SetpointManager_MixedAirFields::SetpointNodeorNodeListName, hxExhaustAirInletNodeName);

            m_idfObjects.push_back(idf_spm);
          }
        }
      }
    }

    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
