/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/ElectricEquipmentITEAirCooledDefinition.hpp"
#include "../../model/ElectricEquipmentITEAirCooledDefinition_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include "../../utilities/core/Compare.hpp"

#include <utilities/idd/ElectricEquipment_ITE_AirCooled_Definition_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateElectricEquipmentITEAirCooledDefinition(ElectricEquipmentITEAirCooledDefinition& modelObject) {

    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::ElectricEquipment_ITE_AirCooled_Definition, modelObject);

    idfObject.setString(ElectricEquipment_ITE_AirCooled_DefinitionFields::AirFlowCalculationMethod, modelObject.airFlowCalculationMethod());

    // The model (like the legacy ElectricEquipment:ITE:AirCooled object) uses 'Watts/Unit', but the E+ ...:Definition object
    // renamed that choice to 'EquipmentLevel' to match the naming convention used by the other *:Definition objects.
    std::string designPowerInputCalculationMethod = modelObject.designPowerInputCalculationMethod();
    if (istringEqual(designPowerInputCalculationMethod, "Watts/Unit")) {
      designPowerInputCalculationMethod = "EquipmentLevel";
    }
    idfObject.setString(ElectricEquipment_ITE_AirCooled_DefinitionFields::DesignPowerInputCalculationMethod, designPowerInputCalculationMethod);

    if (boost::optional<double> d = modelObject.designLevel()) {
      idfObject.setDouble(ElectricEquipment_ITE_AirCooled_DefinitionFields::WattsperUnit, *d);
    }

    if (boost::optional<double> d = modelObject.wattsperSpaceFloorArea()) {
      idfObject.setDouble(ElectricEquipment_ITE_AirCooled_DefinitionFields::WattsperFloorArea, *d);
    }

    idfObject.setString(ElectricEquipment_ITE_AirCooled_DefinitionFields::CPUPowerInputFunctionofLoadingandAirTemperatureCurveName,
                         modelObject.cPUPowerInputFunctionofLoadingandAirTemperatureCurve().name().get());

    idfObject.setDouble(ElectricEquipment_ITE_AirCooled_DefinitionFields::DesignFanPowerInputFraction,
                         modelObject.designFanPowerInputFraction());

    if (boost::optional<double> d = modelObject.designFanAirFlowRateperPowerInput()) {
      idfObject.setDouble(ElectricEquipment_ITE_AirCooled_DefinitionFields::DesignFanAirFlowRateperPowerInput, *d);
    }

    idfObject.setString(ElectricEquipment_ITE_AirCooled_DefinitionFields::AirFlowFunctionofLoadingandAirTemperatureCurveName,
                         modelObject.airFlowFunctionofLoadingandAirTemperatureCurve().name().get());

    idfObject.setString(ElectricEquipment_ITE_AirCooled_DefinitionFields::FanPowerInputFunctionofFlowCurveName,
                         modelObject.fanPowerInputFunctionofFlowCurve().name().get());

    idfObject.setDouble(ElectricEquipment_ITE_AirCooled_DefinitionFields::DesignEnteringAirTemperature,
                         modelObject.designEnteringAirTemperature());

    idfObject.setString(ElectricEquipment_ITE_AirCooled_DefinitionFields::EnvironmentalClass, modelObject.environmentalClass());

    idfObject.setString(ElectricEquipment_ITE_AirCooled_DefinitionFields::AirInletConnectionType, modelObject.airInletConnectionType());

    idfObject.setDouble(ElectricEquipment_ITE_AirCooled_DefinitionFields::DesignRecirculationFraction,
                         modelObject.designRecirculationFraction());

    if (boost::optional<Curve> curve = modelObject.recirculationFunctionofLoadingandSupplyTemperatureCurve()) {
      idfObject.setString(ElectricEquipment_ITE_AirCooled_DefinitionFields::RecirculationFunctionofLoadingandSupplyTemperatureCurveName,
                           curve->name().get());
    }

    idfObject.setDouble(ElectricEquipment_ITE_AirCooled_DefinitionFields::DesignElectricPowerSupplyEfficiency,
                         modelObject.designElectricPowerSupplyEfficiency());

    if (boost::optional<Curve> curve = modelObject.electricPowerSupplyEfficiencyFunctionofPartLoadRatioCurve()) {
      idfObject.setString(ElectricEquipment_ITE_AirCooled_DefinitionFields::ElectricPowerSupplyEfficiencyFunctionofPartLoadRatioCurveName,
                           curve->name().get());
    }

    idfObject.setDouble(ElectricEquipment_ITE_AirCooled_DefinitionFields::FractionofElectricPowerSupplyLossestoZone,
                         modelObject.fractionofElectricPowerSupplyLossestoZone());

    idfObject.setDouble(ElectricEquipment_ITE_AirCooled_DefinitionFields::SupplyTemperatureDifference, modelObject.supplyTemperatureDifference());

    if (boost::optional<Schedule> schedule = modelObject.supplyTemperatureDifferenceSchedule()) {
      idfObject.setString(ElectricEquipment_ITE_AirCooled_DefinitionFields::SupplyTemperatureDifferenceSchedule, schedule->name().get());
    }

    idfObject.setDouble(ElectricEquipment_ITE_AirCooled_DefinitionFields::ReturnTemperatureDifference, modelObject.returnTemperatureDifference());

    if (boost::optional<Schedule> schedule = modelObject.returnTemperatureDifferenceSchedule()) {
      idfObject.setString(ElectricEquipment_ITE_AirCooled_DefinitionFields::ReturnTemperatureDifferenceSchedule, schedule->name().get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
