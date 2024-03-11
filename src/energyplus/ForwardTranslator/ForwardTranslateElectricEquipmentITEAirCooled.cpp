/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/ElectricEquipmentITEAirCooled.hpp"
#include "../../model/ElectricEquipmentITEAirCooled_Impl.hpp"
#include "../../model/ElectricEquipmentITEAirCooledDefinition.hpp"
#include "../../model/ElectricEquipmentITEAirCooledDefinition_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"
#include "../../model/CurveBiquadratic.hpp"
#include "../../model/CurveBiquadratic_Impl.hpp"
#include "../../model/CurveQuadratic.hpp"
#include "../../model/CurveQuadratic_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/PortList.hpp"
#include "../../model/PortList_Impl.hpp"
#include "../../model/AirTerminalSingleDuctVAVNoReheat.hpp"
#include "../../model/AirTerminalSingleDuctVAVNoReheat_Impl.hpp"
#include "../../model/AirTerminalSingleDuctVAVReheat.hpp"
#include "../../model/AirTerminalSingleDuctVAVReheat_Impl.hpp"
#include "../../model/HVACComponent.hpp"
#include "../../model/LifeCycleCost.hpp"

#include <utilities/idd/ElectricEquipment_ITE_AirCooled_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include "../../utilities/core/ASCIIStrings.hpp"
#include <utilities/idd/IddEnums.hxx>

#include <algorithm>
#include <iterator>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateElectricEquipmentITEAirCooled(ElectricEquipmentITEAirCooled& modelObject) {

    boost::optional<Space> space_ = modelObject.space();
    if (!space_) {
      // This shouldn't happen
      LOG(Warn, modelObject.briefDescription() << " not assigned to a valid space.");
      return boost::none;
    }

    auto space = space_.get();

    boost::optional<ThermalZone> thermalZone_ = space.thermalZone();
    if (!thermalZone_) {
      // This shouldn't happen
      LOG(Warn, modelObject.briefDescription() << " not assigned to a valid thermal Zone.");
      return boost::none;
    }
    auto thermalZone = thermalZone_.get();

    IdfObject idfObject(openstudio::IddObjectType::ElectricEquipment_ITE_AirCooled);

    for (LifeCycleCost lifeCycleCost : modelObject.lifeCycleCosts()) {
      translateAndMapModelObject(lifeCycleCost);
    }

    ElectricEquipmentITEAirCooledDefinition definition = modelObject.electricEquipmentITEAirCooledDefinition();

    // Pre-process:
    // (1) assign to zone; (2) connect the supply air node (3) apply contraints
    // ITE object that is assigned to space type in OS has been switched to separate spaces in ForwardTranslator

    // TODO: There might be work to be done in the case we translate to E+ Spaces (m_forwardTranslatorOptions.excludeSpaceTranslation() = false)
    // Per IDD for this ITE object: ZoneList and SpaceList names are not allowed.
    // But Spaces are allowed. Currently in the ForwardTranslator::translateModelPrivate we have rules to "hard-apply" ITE equipment assigned at
    // spacetype-level to each spaces (clone for each space, then remove the SpaceType one)
    // cf https://github.com/NREL/OpenStudio/blob/bf4ffc49d5947f74b139efa5fb1dffec9b1fb013/src/energyplus/ForwardTranslator.cpp#L323-L341

    // Assign object to Zone/Space
    IdfObject parentIdfObject = getSpaceLoadParent(modelObject, false);  // We do not allow spaceType!
    idfObject.setString(ElectricEquipment_ITE_AirCooledFields::ZoneorSpaceName, parentIdfObject.nameString());

    // attach the supply air node to zone if there is an available supply air node
    // search airloop first
    if (auto mo = thermalZone.inletPortList().airLoopHVACModelObject()) {
      if (auto node = mo->optionalCast<Node>()) {
        idfObject.setString(ElectricEquipment_ITE_AirCooledFields::SupplyAirNodeName, node->name().get());
      }
    } else {
      // if no airloop, just get a supply node of the thermal zone (could be zoneHVAC)
      if (!thermalZone.inletPortList().modelObjects().empty()) {
        std::vector<ModelObject> objects = thermalZone.inletPortList().modelObjects();
        for (const auto& elem : objects) {
          if (auto node = elem.optionalCast<Node>()) {
            idfObject.setString(ElectricEquipment_ITE_AirCooledFields::SupplyAirNodeName, node->name().get());
          }
        }
      }
    }

    // (Note: I like the idea of having put the logic here, but it would be more efficient to do that in ForwardTranslateThermalZone (or
    // translateModelPrivate) to avoid doing it on every instance of a zone)
    // apply constraint #1: For multiple ITE objects defined for one zone, the same calculation method should apply.
    std::set<std::string> methods;

    auto caseInsensitiveMethodForITE = [](const ElectricEquipmentITEAirCooled& ite) {
      return openstudio::ascii_to_lower_copy(ite.electricEquipmentITEAirCooledDefinition().airFlowCalculationMethod());
    };

    if (m_forwardTranslatorOptions.excludeSpaceTranslation()) {
      // One Zone = One Space at this point
      auto ites = space.electricEquipmentITEAirCooled();
      std::transform(ites.begin(), ites.end(), std::inserter(methods, methods.begin()), caseInsensitiveMethodForITE);
    } else {
      // Does this constraint apply per Space? Per the I/O ref I think it applies for the entire zone.
      // I/O 9.6.0: "For multiple ITE objects defined for one zone, the same calculation method should apply."
      for (const auto& s : thermalZone.spaces()) {
        auto ites = s.electricEquipmentITEAirCooled();
        std::transform(ites.begin(), ites.end(), std::inserter(methods, methods.begin()), caseInsensitiveMethodForITE);
      }
    }

    std::string thisMethod = definition.airFlowCalculationMethod();
    if ((methods.size() > 1) && !openstudio::istringEqual(thisMethod, "FlowControlWithApproachTemperatures")) {
      definition.setAirFlowCalculationMethod("FlowControlWithApproachTemperatures");
      LOG(Warn, parentIdfObject.briefDescription() << " has multiple IT equipment with different air flow calculation methods, "
                                                   << modelObject.briefDescription() << " is re-assigned to 'FlowControlWithApproachTemperatures'");
    }

    // Constraint #2:The FlowControlWithApproachTemperatures only applies to ITE zones with single duct VAV terminal unit.
    if (istringEqual(thisMethod, "FlowControlWithApproachTemperatures")) {
      auto terminal = thermalZone.airLoopHVACTerminal();
      bool isTerminalOk =
        (terminal && (terminal->optionalCast<AirTerminalSingleDuctVAVReheat>() || terminal->optionalCast<AirTerminalSingleDuctVAVNoReheat>()));

      if (!isTerminalOk) {
        LOG(
          Error,
          modelObject.briefDescription() << " will not be translated. "
                                            " The FlowControlWithApproachTemperatures only applies to ITE zones with single duct VAV terminal unit.");
        return boost::none;
      }
    }

    // After pre-checking gets through, add the object to the translation list
    m_idfObjects.push_back(idfObject);

    idfObject.setString(ElectricEquipment_ITE_AirCooledFields::Name, modelObject.name().get());

    idfObject.setString(ElectricEquipment_ITE_AirCooledFields::AirFlowCalculationMethod, definition.airFlowCalculationMethod());

    idfObject.setString(ElectricEquipment_ITE_AirCooledFields::DesignPowerInputCalculationMethod, definition.designPowerInputCalculationMethod());

    // To match other equipment objects, the Number of Units field was dropped. Users can just use multiplier for both Watts/Area and Watts/Unit definitions
    // this is to get rid of the warning message for leaving the Number of Units field blank.
    idfObject.setDouble(ElectricEquipment_ITE_AirCooledFields::NumberofUnits, 1.0);

    double multiplier = modelObject.multiplier();

    OptionalDouble d = definition.wattsperUnit();
    if (d) {
      idfObject.setDouble(ElectricEquipment_ITE_AirCooledFields::WattsperUnit, (*d) * multiplier);
    }

    d = definition.wattsperSpaceFloorArea();
    if (d) {
      idfObject.setDouble(ElectricEquipment_ITE_AirCooledFields::WattsperFloorArea, (*d) * multiplier);
    }

    boost::optional<Schedule> designPowerInputSchedule = modelObject.designPowerInputSchedule();
    if (designPowerInputSchedule) {
      idfObject.setString(ElectricEquipment_ITE_AirCooledFields::DesignPowerInputScheduleName, designPowerInputSchedule->name().get());
    }

    boost::optional<Schedule> cPULoadingSchedule = modelObject.cPULoadingSchedule();
    if (cPULoadingSchedule) {
      idfObject.setString(ElectricEquipment_ITE_AirCooledFields::CPULoadingScheduleName, cPULoadingSchedule->name().get());
    }

    idfObject.setString(ElectricEquipment_ITE_AirCooledFields::CPUPowerInputFunctionofLoadingandAirTemperatureCurveName,
                        definition.cPUPowerInputFunctionofLoadingandAirTemperatureCurve().name().get());

    idfObject.setDouble(ElectricEquipment_ITE_AirCooledFields::DesignFanPowerInputFraction, definition.designFanPowerInputFraction());

    d = definition.designFanAirFlowRateperPowerInput();
    if (d) {
      idfObject.setDouble(ElectricEquipment_ITE_AirCooledFields::DesignFanAirFlowRateperPowerInput, *d);
    }

    idfObject.setString(ElectricEquipment_ITE_AirCooledFields::AirFlowFunctionofLoadingandAirTemperatureCurveName,
                        definition.airFlowFunctionofLoadingandAirTemperatureCurve().name().get());

    idfObject.setString(ElectricEquipment_ITE_AirCooledFields::FanPowerInputFunctionofFlowCurveName,
                        definition.fanPowerInputFunctionofFlowCurve().name().get());

    idfObject.setDouble(ElectricEquipment_ITE_AirCooledFields::DesignEnteringAirTemperature, definition.designEnteringAirTemperature());

    idfObject.setString(ElectricEquipment_ITE_AirCooledFields::EnvironmentalClass, definition.environmentalClass());

    idfObject.setString(ElectricEquipment_ITE_AirCooledFields::AirInletConnectionType, definition.airInletConnectionType());

    idfObject.setDouble(ElectricEquipment_ITE_AirCooledFields::DesignRecirculationFraction, definition.designRecirculationFraction());

    boost::optional<Curve> recirculationFunctionofLoadingandSupplyTemperatureCurve =
      definition.recirculationFunctionofLoadingandSupplyTemperatureCurve();
    if (recirculationFunctionofLoadingandSupplyTemperatureCurve) {
      idfObject.setString(ElectricEquipment_ITE_AirCooledFields::RecirculationFunctionofLoadingandSupplyTemperatureCurveName,
                          recirculationFunctionofLoadingandSupplyTemperatureCurve->name().get());
    }

    idfObject.setDouble(ElectricEquipment_ITE_AirCooledFields::DesignElectricPowerSupplyEfficiency, definition.designElectricPowerSupplyEfficiency());

    boost::optional<Curve> electricPowerSupplyEfficiencyFunctionofPartLoadRatioCurve =
      definition.electricPowerSupplyEfficiencyFunctionofPartLoadRatioCurve();
    if (electricPowerSupplyEfficiencyFunctionofPartLoadRatioCurve) {
      idfObject.setString(ElectricEquipment_ITE_AirCooledFields::ElectricPowerSupplyEfficiencyFunctionofPartLoadRatioCurveName,
                          electricPowerSupplyEfficiencyFunctionofPartLoadRatioCurve->name().get());
    }

    idfObject.setDouble(ElectricEquipment_ITE_AirCooledFields::FractionofElectricPowerSupplyLossestoZone,
                        definition.fractionofElectricPowerSupplyLossestoZone());

    OptionalString s = modelObject.cPUEndUseSubcategory();
    if (s) {
      idfObject.setString(ElectricEquipment_ITE_AirCooledFields::CPUEndUseSubcategory, modelObject.cPUEndUseSubcategory());
    }

    s = modelObject.fanEndUseSubcategory();
    if (s) {
      idfObject.setString(ElectricEquipment_ITE_AirCooledFields::FanEndUseSubcategory, modelObject.fanEndUseSubcategory());
    }

    s = modelObject.electricPowerSupplyEndUseSubcategory();
    if (s) {
      idfObject.setString(ElectricEquipment_ITE_AirCooledFields::ElectricPowerSupplyEndUseSubcategory,
                          modelObject.electricPowerSupplyEndUseSubcategory());
    }

    idfObject.setDouble(ElectricEquipment_ITE_AirCooledFields::SupplyTemperatureDifference, definition.supplyTemperatureDifference());

    boost::optional<Schedule> supplyTemperatureDifferenceSchedule = definition.supplyTemperatureDifferenceSchedule();
    if (supplyTemperatureDifferenceSchedule) {
      idfObject.setString(ElectricEquipment_ITE_AirCooledFields::SupplyTemperatureDifferenceSchedule,
                          supplyTemperatureDifferenceSchedule->name().get());
    }

    idfObject.setDouble(ElectricEquipment_ITE_AirCooledFields::ReturnTemperatureDifference, definition.returnTemperatureDifference());

    boost::optional<Schedule> returnTemperatureDifferenceSchedule = definition.returnTemperatureDifferenceSchedule();
    if (returnTemperatureDifferenceSchedule) {
      idfObject.setString(ElectricEquipment_ITE_AirCooledFields::ReturnTemperatureDifferenceSchedule,
                          returnTemperatureDifferenceSchedule->name().get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
