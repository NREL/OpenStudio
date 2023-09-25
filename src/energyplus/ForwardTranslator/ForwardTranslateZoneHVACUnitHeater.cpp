/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/ZoneHVACUnitHeater.hpp"
#include "../../model/ZoneHVACUnitHeater_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/CoilHeatingWater.hpp"
#include "../../model/CoilHeatingWater_Impl.hpp"
#include <utilities/idd/ZoneHVAC_UnitHeater_FieldEnums.hxx>
#include <utilities/idd/Fan_OnOff_FieldEnums.hxx>
#include <utilities/idd/Fan_ConstantVolume_FieldEnums.hxx>
#include <utilities/idd/Fan_VariableVolume_FieldEnums.hxx>
#include <utilities/idd/Fan_SystemModel_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Water_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Fuel_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Electric_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateZoneHVACUnitHeater(ZoneHVACUnitHeater& modelObject) {
    // Make sure the modelObject gets ut into the map, and the new idfObject gets put into the final file.
    // Also sets the idfObjects name

    IdfObject idfObject = createRegisterAndNameIdfObject(IddObjectType::ZoneHVAC_UnitHeater, modelObject);

    boost::optional<std::string> s;
    boost::optional<double> value;
    boost::optional<Node> node;

    // Get model object name and define node names for future use

    // Model Name
    std::string baseName = modelObject.name().get();

    // Node Names
    std::string fanOutletNodeName = baseName + " Fan Outlet Node";

    // Field: Availability Schedule Name

    Schedule availabilitySchedule = modelObject.availabilitySchedule();
    translateAndMapModelObject(availabilitySchedule);

    s = availabilitySchedule.name();

    if (s) {
      idfObject.setString(ZoneHVAC_UnitHeaterFields::AvailabilityScheduleName, *s);
    }

    // Field: Air Inlet Node Name

    node = modelObject.inletNode();

    if (node) {
      s = node->name();

      if (s) {
        idfObject.setString(ZoneHVAC_UnitHeaterFields::AirInletNodeName, *s);
      }
    }

    // Field: Air Outlet Node Name

    node = modelObject.outletNode();

    if (node) {
      s = node->name();

      if (s) {
        idfObject.setString(ZoneHVAC_UnitHeaterFields::AirOutletNodeName, *s);
      }
    }

    //Field: Supply Air Fan Object Type

    HVACComponent supplyAirFan = modelObject.supplyAirFan();

    if (boost::optional<IdfObject> _supplyAirFan = translateAndMapModelObject(supplyAirFan)) {
      idfObject.setString(ZoneHVAC_UnitHeaterFields::SupplyAirFanObjectType, _supplyAirFan->iddObject().name());
    }

    // Field: Supply Air Fan Name

    s = modelObject.supplyAirFan().name();

    if (s)

    {
      idfObject.setString(ZoneHVAC_UnitHeaterFields::SupplyAirFanName, *s);
    }

    // Supply Air Fan Inlet and Outlet Nodes

    node = modelObject.inletNode();

    if (boost::optional<IdfObject> _supplyAirFan = translateAndMapModelObject(supplyAirFan)) {

      if (node) {
        s = node->name();

        if (_supplyAirFan->iddObject().type() == IddObjectType::Fan_ConstantVolume) {
          _supplyAirFan->setString(Fan_ConstantVolumeFields::AirInletNodeName, *s);
          _supplyAirFan->setString(Fan_ConstantVolumeFields::AirOutletNodeName, fanOutletNodeName);
        } else if (_supplyAirFan->iddObject().type() == IddObjectType::Fan_VariableVolume) {
          _supplyAirFan->setString(Fan_VariableVolumeFields::AirInletNodeName, *s);
          _supplyAirFan->setString(Fan_VariableVolumeFields::AirOutletNodeName, fanOutletNodeName);
        } else if (_supplyAirFan->iddObject().type() == IddObjectType::Fan_SystemModel) {
          _supplyAirFan->setString(Fan_SystemModelFields::AirInletNodeName, *s);
          _supplyAirFan->setString(Fan_SystemModelFields::AirOutletNodeName, fanOutletNodeName);
        } else if (_supplyAirFan->iddObject().type() == IddObjectType::Fan_OnOff) {
          _supplyAirFan->setString(Fan_OnOffFields::AirInletNodeName, *s);
          _supplyAirFan->setString(Fan_OnOffFields::AirOutletNodeName, fanOutletNodeName);
        } else {
          OS_ASSERT(false);  // We're missing a fan type!
        }
      }
    }

    // Field Maximum Supply Air Flow Rate

    if (modelObject.isMaximumSupplyAirFlowRateAutosized()) {
      idfObject.setString(ZoneHVAC_UnitHeaterFields::MaximumSupplyAirFlowRate, "Autosize");
    }

    else if ((value = modelObject.maximumSupplyAirFlowRate())) {
      idfObject.setDouble(ZoneHVAC_UnitHeaterFields::MaximumSupplyAirFlowRate, *value);
    }

    // Field: Fan Control Type
    // Maps to "Supply Air Fan Operation During No Heating" in 8.2.0 and above.

    if (istringEqual(modelObject.fanControlType(), "ONOFF")) {
      idfObject.setString(ZoneHVAC_UnitHeaterFields::SupplyAirFanOperationDuringNoHeating, "No");
    } else {
      idfObject.setString(ZoneHVAC_UnitHeaterFields::SupplyAirFanOperationDuringNoHeating, "Yes");
    }

    // Field: Heating Coil Object Type

    HVACComponent heatingCoil = modelObject.heatingCoil();

    if (boost::optional<IdfObject> _heatingCoil = translateAndMapModelObject(heatingCoil)) {
      idfObject.setString(ZoneHVAC_UnitHeaterFields::HeatingCoilObjectType, _heatingCoil->iddObject().name());
    }

    // Field: Heating Coil Name

    s = modelObject.heatingCoil().name();

    if (s) {
      idfObject.setString(ZoneHVAC_UnitHeaterFields::HeatingCoilName, *s);
    }

    // Heating coil inlet and outlet node names

    node = modelObject.outletNode();

    if (boost::optional<IdfObject> _heatingCoil = translateAndMapModelObject(heatingCoil)) {
      if (node) {
        s = node->name();

        if (_heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Electric) {
          _heatingCoil->setString(Coil_Heating_ElectricFields::AirInletNodeName, fanOutletNodeName);
          _heatingCoil->setString(Coil_Heating_ElectricFields::AirOutletNodeName, *s);
        } else if (_heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Fuel) {
          _heatingCoil->setString(Coil_Heating_FuelFields::AirInletNodeName, fanOutletNodeName);
          _heatingCoil->setString(Coil_Heating_FuelFields::AirOutletNodeName, *s);
        } else if (_heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Water) {
          _heatingCoil->setString(Coil_Heating_WaterFields::AirInletNodeName, fanOutletNodeName);
          _heatingCoil->setString(Coil_Heating_WaterFields::AirOutletNodeName, *s);
        } else {
          OS_ASSERT(false);  // We're missing a coil type!
        }
      }
    }

    // Field Maximum Hot Water [or Steam] Flow Rate

    if (modelObject.isMaximumHotWaterFlowRateAutosized())

    {
      idfObject.setString(ZoneHVAC_UnitHeaterFields::MaximumHotWaterorSteamFlowRate, "Autosize");
    }

    else if ((value = modelObject.maximumHotWaterFlowRate())) {
      idfObject.setDouble(ZoneHVAC_UnitHeaterFields::MaximumHotWaterorSteamFlowRate, *value);
    }

    // Field: Minimum Hot Water [or Steam] Flow Rate

    idfObject.setDouble(ZoneHVAC_UnitHeaterFields::MinimumHotWaterorSteamFlowRate, modelObject.minimumHotWaterFlowRate());

    // Field: Heating Convergence Tolerance

    idfObject.setDouble(ZoneHVAC_UnitHeaterFields::HeatingConvergenceTolerance, modelObject.heatingConvergenceTolerance());

    // Field: Availability Manager List Name: Ignored?

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
