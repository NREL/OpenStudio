/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"

#include "../../model/AirConditionerVariableRefrigerantFlowFluidTemperatureControl.hpp"
#include "../../model/AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl.hpp"
#include "../../model/LoadingIndex.hpp"
#include "../../model/LoadingIndex_Impl.hpp"

#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/PlantLoop.hpp"
#include "../../model/PlantLoop_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/ZoneHVACTerminalUnitVariableRefrigerantFlow.hpp"
#include "../../model/ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"

#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include "../../utilities/idd/IddEnums.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_FieldEnums.hxx>
#include <utilities/idd/ZoneTerminalUnitList_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/FluidProperties_Name_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateAirConditionerVariableRefrigerantFlowFluidTemperatureControl(
    model::AirConditionerVariableRefrigerantFlowFluidTemperatureControl& modelObject) {

    // Terminals
    std::vector<ZoneHVACTerminalUnitVariableRefrigerantFlow> terminals = modelObject.terminals();
    if (terminals.empty()) {
      LOG(Warn, modelObject.briefDescription() << " will not be translated as it has no terminals.");
      return boost::none;
    }

    // Name
    IdfObject idfObject =
      createRegisterAndNameIdfObject(openstudio::IddObjectType::AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl, modelObject);

    // Availability Schedule Name: Optional Object, required in Model
    {
      auto sch = modelObject.availabilitySchedule();
      if (boost::optional<IdfObject> owo_ = translateAndMapModelObject(sch)) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::AvailabilityScheduleName, owo_->nameString());
      }
    }

    // Refrigerant Type: Optional Object
    boost::optional<IdfObject> fluidProperties = createFluidProperties(modelObject.refrigerantType());
    if (fluidProperties) {
      boost::optional<std::string> value = fluidProperties.get().getString(FluidProperties_NameFields::FluidName, true);
      if (value) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::RefrigerantType, value.get());
      }
    }

    if (modelObject.isRatedEvaporativeCapacityAutosized()) {
      idfObject.setString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::RatedEvaporativeCapacity, "Autosize");
    } else {
      // Rated Evaporative Capacity: boost::optional<double>
      if (boost::optional<double> _ratedEvaporativeCapacity = modelObject.ratedEvaporativeCapacity()) {
        idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::RatedEvaporativeCapacity,
                            _ratedEvaporativeCapacity.get());
      }
    }

    // Rated Compressor Power Per Unit of Rated Evaporative Capacity: Optional Double
    double ratedCompressorPowerPerUnitofRatedEvaporativeCapacity = modelObject.ratedCompressorPowerPerUnitofRatedEvaporativeCapacity();
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::RatedCompressorPowerPerUnitofRatedEvaporativeCapacity,
                        ratedCompressorPowerPerUnitofRatedEvaporativeCapacity);

    // Minimum Outdoor Air Temperature in Cooling Mode: Optional Double
    double minimumOutdoorAirTemperatureinCoolingMode = modelObject.minimumOutdoorAirTemperatureinCoolingMode();
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MinimumOutdoorAirTemperatureinCoolingMode,
                        minimumOutdoorAirTemperatureinCoolingMode);

    // Maximum Outdoor Air Temperature in Cooling Mode: Optional Double
    double maximumOutdoorAirTemperatureinCoolingMode = modelObject.maximumOutdoorAirTemperatureinCoolingMode();
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MaximumOutdoorAirTemperatureinCoolingMode,
                        maximumOutdoorAirTemperatureinCoolingMode);

    // Minimum Outdoor Air Temperature in Heating Mode: Optional Double
    double minimumOutdoorAirTemperatureinHeatingMode = modelObject.minimumOutdoorAirTemperatureinHeatingMode();
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MinimumOutdoorAirTemperatureinHeatingMode,
                        minimumOutdoorAirTemperatureinHeatingMode);

    // Maximum Outdoor Air Temperature in Heating Mode: Optional Double
    double maximumOutdoorAirTemperatureinHeatingMode = modelObject.maximumOutdoorAirTemperatureinHeatingMode();
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MaximumOutdoorAirTemperatureinHeatingMode,
                        maximumOutdoorAirTemperatureinHeatingMode);

    // Reference Outdoor Unit Superheating: Optional Double
    double referenceOutdoorUnitSuperheating = modelObject.referenceOutdoorUnitSuperheating();
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ReferenceOutdoorUnitSuperheating,
                        referenceOutdoorUnitSuperheating);

    // Reference Outdoor Unit Subcooling: Optional Double
    double referenceOutdoorUnitSubcooling = modelObject.referenceOutdoorUnitSubcooling();
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ReferenceOutdoorUnitSubcooling,
                        referenceOutdoorUnitSubcooling);

    // Refrigerant Temperature Control Algorithm for Indoor Unit: Optional String
    std::string refrigerantTemperatureControlAlgorithmforIndoorUnit = modelObject.refrigerantTemperatureControlAlgorithmforIndoorUnit();
    idfObject.setString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::RefrigerantTemperatureControlAlgorithmforIndoorUnit,
                        refrigerantTemperatureControlAlgorithmforIndoorUnit);

    // Reference Evaporating Temperature for Indoor Unit: Optional Double
    double referenceEvaporatingTemperatureforIndoorUnit = modelObject.referenceEvaporatingTemperatureforIndoorUnit();
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ReferenceEvaporatingTemperatureforIndoorUnit,
                        referenceEvaporatingTemperatureforIndoorUnit);

    // Reference Condensing Temperature for Indoor Unit: Optional Double
    double referenceCondensingTemperatureforIndoorUnit = modelObject.referenceCondensingTemperatureforIndoorUnit();
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ReferenceCondensingTemperatureforIndoorUnit,
                        referenceCondensingTemperatureforIndoorUnit);

    // Variable Evaporating Temperature Minimum for Indoor Unit: Optional Double
    double variableEvaporatingTemperatureMinimumforIndoorUnit = modelObject.variableEvaporatingTemperatureMinimumforIndoorUnit();
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::VariableEvaporatingTemperatureMinimumforIndoorUnit,
                        variableEvaporatingTemperatureMinimumforIndoorUnit);

    // Variable Evaporating Temperature Maximum for Indoor Unit: Optional Double
    double variableEvaporatingTemperatureMaximumforIndoorUnit = modelObject.variableEvaporatingTemperatureMaximumforIndoorUnit();
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::VariableEvaporatingTemperatureMaximumforIndoorUnit,
                        variableEvaporatingTemperatureMaximumforIndoorUnit);

    // Variable Condensing Temperature Minimum for Indoor Unit: Optional Double
    double variableCondensingTemperatureMinimumforIndoorUnit = modelObject.variableCondensingTemperatureMinimumforIndoorUnit();
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::VariableCondensingTemperatureMinimumforIndoorUnit,
                        variableCondensingTemperatureMinimumforIndoorUnit);

    // Variable Condensing Temperature Maximum for Indoor Unit: Optional Double
    double variableCondensingTemperatureMaximumforIndoorUnit = modelObject.variableCondensingTemperatureMaximumforIndoorUnit();
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::VariableCondensingTemperatureMaximumforIndoorUnit,
                        variableCondensingTemperatureMaximumforIndoorUnit);

    // Outdoor Unit Fan Power Per Unit of Rated Evaporative Capacity: Optional Double
    double outdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity = modelObject.outdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity();
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::OutdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity,
                        outdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity);

    // Outdoor Unit Fan Flow Rate Per Unit of Rated Evaporative Capacity: Optional Double
    double outdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity = modelObject.outdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity();
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::OutdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity,
                        outdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity);

    // Outdoor Unit Evaporating Temperature Function of Superheating Curve Name: Required Object
    Curve outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve = modelObject.outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve();
    if (boost::optional<IdfObject> owo_ = translateAndMapModelObject(outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve)) {
      idfObject.setString(
        AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::OutdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurveName,
        owo_->nameString());
    }

    // Outdoor Unit Condensing Temperature Function of Subcooling Curve Name: Required Object
    Curve outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve = modelObject.outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve();
    if (boost::optional<IdfObject> owo_ = translateAndMapModelObject(outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve)) {
      idfObject.setString(
        AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::OutdoorUnitCondensingTemperatureFunctionofSubcoolingCurveName,
        owo_->nameString());
    }

    // Diameter of Main Pipe Connecting Outdoor Unit to the First Branch Joint: Optional Double
    double diameterofMainPipeConnectingOutdoorUnittotheFirstBranchJoint = modelObject.diameterofMainPipeConnectingOutdoorUnittotheFirstBranchJoint();
    idfObject.setDouble(
      AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::DiameterofMainPipeConnectingOutdoorUnittotheFirstBranchJoint,
      diameterofMainPipeConnectingOutdoorUnittotheFirstBranchJoint);

    // Length of Main Pipe Connecting Outdoor Unit to the First Branch Joint: Optional Double
    double lengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint = modelObject.lengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint();
    idfObject.setDouble(
      AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::LengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint,
      lengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint);

    // Equivalent Length of Main Pipe Connecting Outdoor Unit to the First Branch Joint: Optional Double
    double equivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint =
      modelObject.equivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint();
    idfObject.setDouble(
      AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::EquivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint,
      equivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint);

    // Height Difference Between Outdoor Unit and Indoor Units: Optional Double
    double heightDifferenceBetweenOutdoorUnitandIndoorUnits = modelObject.heightDifferenceBetweenOutdoorUnitandIndoorUnits();
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeightDifferenceBetweenOutdoorUnitandIndoorUnits,
                        heightDifferenceBetweenOutdoorUnitandIndoorUnits);

    // Main Pipe Insulation Thickness: Optional Double
    double mainPipeInsulationThickness = modelObject.mainPipeInsulationThickness();
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MainPipeInsulationThickness,
                        mainPipeInsulationThickness);

    // Main Pipe Insulation Thermal Conductivity: Optional Double
    double mainPipeInsulationThermalConductivity = modelObject.mainPipeInsulationThermalConductivity();
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MainPipeInsulationThermalConductivity,
                        mainPipeInsulationThermalConductivity);

    // Crankcase Heater Power per Compressor: Optional Double
    double crankcaseHeaterPowerperCompressor = modelObject.crankcaseHeaterPowerperCompressor();
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CrankcaseHeaterPowerperCompressor,
                        crankcaseHeaterPowerperCompressor);

    // Number of Compressors: Optional Integer
    int numberofCompressors = modelObject.numberofCompressors();
    idfObject.setInt(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::NumberofCompressors, numberofCompressors);

    // Ratio of Compressor Size to Total Compressor Capacity: Optional Double
    double ratioofCompressorSizetoTotalCompressorCapacity = modelObject.ratioofCompressorSizetoTotalCompressorCapacity();
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::RatioofCompressorSizetoTotalCompressorCapacity,
                        ratioofCompressorSizetoTotalCompressorCapacity);

    // Maximum Outdoor Dry-Bulb Temperature for Crankcase Heater: Optional Double
    double maximumOutdoorDryBulbTemperatureforCrankcaseHeater = modelObject.maximumOutdoorDryBulbTemperatureforCrankcaseHeater();
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeater,
                        maximumOutdoorDryBulbTemperatureforCrankcaseHeater);

    // Defrost Strategy: Optional String
    std::string defrostStrategy = modelObject.defrostStrategy();
    idfObject.setString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::DefrostStrategy, defrostStrategy);

    // Defrost Control: Optional String
    std::string defrostControl = modelObject.defrostControl();
    idfObject.setString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::DefrostControl, defrostControl);

    // Defrost Energy Input Ratio Modifier Function of Temperature Curve Name: Optional Object
    if (boost::optional<Curve> _defrostEnergyInputRatioModifierFunctionofTemperatureCurve =
          modelObject.defrostEnergyInputRatioModifierFunctionofTemperatureCurve()) {
      if (boost::optional<IdfObject> owo_ = translateAndMapModelObject(_defrostEnergyInputRatioModifierFunctionofTemperatureCurve.get())) {
        idfObject.setString(
          AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::DefrostEnergyInputRatioModifierFunctionofTemperatureCurveName,
          owo_->nameString());
      }
    }

    // Defrost Time Period Fraction: Optional Double
    double defrostTimePeriodFraction = modelObject.defrostTimePeriodFraction();
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::DefrostTimePeriodFraction, defrostTimePeriodFraction);

    if (modelObject.isResistiveDefrostHeaterCapacityAutosized()) {
      idfObject.setString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ResistiveDefrostHeaterCapacity, "Autosize");
    } else {
      // Resistive Defrost Heater Capacity: boost::optional<double>
      if (boost::optional<double> _resistiveDefrostHeaterCapacity = modelObject.resistiveDefrostHeaterCapacity()) {
        idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ResistiveDefrostHeaterCapacity,
                            _resistiveDefrostHeaterCapacity.get());
      }
    }

    // Maximum Outdoor Dry-bulb Temperature for Defrost Operation: Optional Double
    double maximumOutdoorDrybulbTemperatureforDefrostOperation = modelObject.maximumOutdoorDrybulbTemperatureforDefrostOperation();
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MaximumOutdoorDrybulbTemperatureforDefrostOperation,
                        maximumOutdoorDrybulbTemperatureforDefrostOperation);

    // Compressor Maximum Delta Pressure: Optional Double
    double compressorMaximumDeltaPressure = modelObject.compressorMaximumDeltaPressure();
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CompressormaximumdeltaPressure,
                        compressorMaximumDeltaPressure);

    // Number of Compressor Loading Index Entries: Optional Integer
    std::vector<LoadingIndex> loadingIndexes = modelObject.loadingIndexes();
    if (auto num = loadingIndexes.size()) {
      idfObject.setInt(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::NumberofCompressorLoadingIndexEntries, num);
    }

    // Zone Terminal Unit List Name
    IdfObject _zoneTerminalUnitList(IddObjectType::ZoneTerminalUnitList);

    std::string terminalUnitListName = modelObject.name().get() + " Terminal List";

    _zoneTerminalUnitList.setString(ZoneTerminalUnitListFields::ZoneTerminalUnitListName, terminalUnitListName);

    idfObject.setString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ZoneTerminalUnitListName, terminalUnitListName);

    m_idfObjects.push_back(_zoneTerminalUnitList);

    for (auto& terminal : terminals) {
      boost::optional<IdfObject> terminal_ = translateAndMapModelObject(terminal);

      OS_ASSERT(terminal_);

      IdfExtensibleGroup eg = _zoneTerminalUnitList.pushExtensibleGroup();

      eg.setString(ZoneTerminalUnitListExtensibleFields::ZoneTerminalUnitName, terminal_->name().get());
    }

    // Loading Index Extensible Groups
    for (auto& loadingIndex : loadingIndexes) {
      auto eg = idfObject.pushExtensibleGroup();

      // Compressor Speed at Loading Index
      eg.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlExtensibleFields::CompressorSpeedatLoadingIndex,
                   loadingIndex.compressorSpeed());

      // Loading Index Evaporative Capacity Multiplier Function of Temperature Curve Name
      {
        auto curve = loadingIndex.evaporativeCapacityMultiplierFunctionofTemperatureCurve();
        if (auto curve_ = translateAndMapModelObject(curve)) {
          eg.setString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlExtensibleFields::
                         LoadingIndexEvaporativeCapacityMultiplierFunctionofTemperatureCurveName,
                       curve_->name().get());
        }
      }

      // Loading Index Compressor Power Multiplier Function of Temperature Curve Name
      {
        auto curve = loadingIndex.compressorPowerMultiplierFunctionofTemperatureCurve();
        if (auto curve_ = translateAndMapModelObject(curve)) {
          eg.setString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlExtensibleFields::
                         LoadingIndexCompressorPowerMultiplierFunctionofTemperatureCurveName,
                       curve_->name().get());
        }
      }
    }

    return idfObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
