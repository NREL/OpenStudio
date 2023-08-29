/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/CoilHeatingDesuperheater.hpp"

#include "../../model/ModelObject.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"

#include <utilities/idd/Coil_Heating_Desuperheater_FieldEnums.hxx>
#include <utilities/idd/CoilSystem_Cooling_DX_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCoilHeatingDesuperheater(CoilHeatingDesuperheater& modelObject) {
    OptionalModelObject omo;
    OptionalString optS;
    boost::optional<std::string> s;
    boost::optional<double> d;

    // Name
    IdfObject object = createRegisterAndNameIdfObject(openstudio::IddObjectType::Coil_Heating_Desuperheater, modelObject);

    // Availability Schedule Name
    boost::optional<Schedule> availabilitySchedule = modelObject.availabilitySchedule();

    if (availabilitySchedule) {
      boost::optional<IdfObject> _availabilitySchedule = translateAndMapModelObject(availabilitySchedule.get());

      if (_availabilitySchedule && _availabilitySchedule->name()) {
        object.setString(Coil_Heating_DesuperheaterFields::AvailabilityScheduleName, _availabilitySchedule->name().get());
      }
    }

    // Heat Recovery Efficiency
    d = modelObject.heatReclaimRecoveryEfficiency();
    if (d) {
      object.setDouble(Coil_Heating_DesuperheaterFields::HeatReclaimRecoveryEfficiency, d.get());
    }

    // Air Inlet Node Name
    if (boost::optional<ModelObject> mo = modelObject.inletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        object.setString(Coil_Heating_DesuperheaterFields::AirInletNodeName, node->name().get());
      }
    }

    // Air Outlet Node Name
    if (boost::optional<ModelObject> mo = modelObject.outletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        object.setString(Coil_Heating_DesuperheaterFields::AirOutletNodeName, node->name().get());
        object.setString(Coil_Heating_DesuperheaterFields::TemperatureSetpointNodeName, node->name().get());
      }
    }

    // HeatingSourceObjectType
    // Heating Source Name
    boost::optional<ModelObject> heatingSource = modelObject.heatingSource();

    if (heatingSource) {

      boost::optional<IdfObject> _heatingSource = translateAndMapModelObject(*heatingSource);

      if (_heatingSource) {

        // Note JM 2019-03-14:
        // If the coil in question is a DX coil (CoilCoolingDXSingleSpeed, CoilCoolingDXTwoSpeed, CoilCoolingDXTwoStageWithHumidityControlMode, CoilCoolingDX)
        // and this DX coil isn't already wrapped in a Unitary, then the FT will wrap it into a CoilSystem:Cooling:DX object and return that, but we
        // need the DX coil here and not the wrapper.
        //
        // Note: Other accepted types are Refrigeration objects and don't suffer the same problem
        // (Refrigeration:Condenser:AirCooled, Refrigeration:Condenser:EvaporativeCooled, Refrigeration:Condenser:WaterCooled, Refrigeration:CompressorRack)

        std::string objectType;
        std::string objectName;

        if (_heatingSource->iddObject().type() == IddObjectType::CoilSystem_Cooling_DX) {
          // We must retrieve the coil itself, not the wrapper
          objectType = _heatingSource->getString(CoilSystem_Cooling_DXFields::CoolingCoilObjectType).get();
          objectName = _heatingSource->getString(CoilSystem_Cooling_DXFields::CoolingCoilName).get();
        } else {
          objectType = _heatingSource->iddObject().name();
          objectName = _heatingSource->name().get();
        }

        if (objectType.empty() || objectName.empty()) {
          // We log an error but let E+ fail eventually
          LOG(Error, "Something went wrong in the translation of " << modelObject.briefDescription() << ", couldn't retrieve the coil");
          // OS_ASSERT(false);
        } else {
          object.setString(Coil_Heating_DesuperheaterFields::HeatingSourceObjectType, objectType);
          object.setString(Coil_Heating_DesuperheaterFields::HeatingSourceName, objectName);
        }
      }
    }

    // Temperature Setpoint Node Name
    // if( boost::optional<Node> node = modelObject.temperatureSetpointNode() )
    // {
    //   object.setString(Coil_Heating_DesuperheaterFields::TemperatureSetpointNodeName,node->name().get());
    // }
    // object.setString(Coil_Heating_DesuperheaterFields::TemperatureSetpointNodeName,"");

    // On Cycle Parasitic Electric Load
    d = modelObject.parasiticElectricLoad();
    if (d) {
      object.setDouble(Coil_Heating_DesuperheaterFields::OnCycleParasiticElectricLoad, d.get());
    }

    return object;
  }
}  // namespace energyplus
}  // namespace openstudio
