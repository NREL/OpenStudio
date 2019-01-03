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
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateElectricEquipmentITEAirCooled( ElectricEquipmentITEAirCooled & modelObject )
{
  IdfObject idfObject(openstudio::IddObjectType::ElectricEquipment_ITE_AirCooled);

  for (LifeCycleCost lifeCycleCost : modelObject.lifeCycleCosts()){
    translateAndMapModelObject(lifeCycleCost);
  }

  ElectricEquipmentITEAirCooledDefinition definition = modelObject.electricEquipmentITEAirCooledDefinition();

  // Pre-process: 
  // (1) assign to zone; (2) connect the supply air node (3) apply contraints
  // ITE object that is assigned to space type in OS has been switched to separate spaces in ForwardTranslator
  boost::optional<Space> space = modelObject.space();
  if (space) {
    boost::optional<ThermalZone> thermalZone = space->thermalZone();
    if (thermalZone) {
      // Assign object to zone
      idfObject.setString(ElectricEquipment_ITE_AirCooledFields::ZoneName, thermalZone->name().get());
      // attach the supply air node to zone if there is an available supply air node
      // search airloop first
      if (auto mo = thermalZone->inletPortList().airLoopHVACModelObject()) {
        if (auto node = mo->optionalCast<Node>()) {
          idfObject.setString(ElectricEquipment_ITE_AirCooledFields::SupplyAirNodeName, node->name().get());
        }
      } else {
        // if no airloop, just get a supply node of the thermal zone (could be zoneHVAC)
        if (thermalZone->inletPortList().modelObjects().size() > 0) {
          std::vector<ModelObject> objects = thermalZone->inletPortList().modelObjects();
          for (const auto & elem : objects) {
            if (auto node = elem.optionalCast<Node>()) {
              idfObject.setString(ElectricEquipment_ITE_AirCooledFields::SupplyAirNodeName, node->name().get());
            }
          }
        }
      }
    }

    // apply contraint #1: For multiple ITE objects defined for one zone, the same calculation method should apply.
    ElectricEquipmentITEAirCooledVector electricEquipmentITEAirCooled = space->electricEquipmentITEAirCooled();
    std::string thisMethod = definition.airFlowCalculationMethod();
    std::sort(electricEquipmentITEAirCooled.begin(), electricEquipmentITEAirCooled.end(), WorkspaceObjectNameLess());
    for (ElectricEquipmentITEAirCooled& iTequipment : electricEquipmentITEAirCooled) {
      if (istringEqual(thisMethod, iTequipment.electricEquipmentITEAirCooledDefinition().airFlowCalculationMethod())) {
        // do nothing
      } else {
        // if different calculation methods are assigned, all change to FlowControlWithApproachTemperatures
        if (!istringEqual(thisMethod, "FlowControlWithApproachTemperatures")) {
          definition.setAirFlowCalculationMethod("FlowControlWithApproachTemperatures");
          LOG(Warn, space->briefDescription() << " has multiple IT equipment with different air flow calculation methods, " << modelObject.briefDescription() << " is re-assigned to 'FlowControlWithApproachTemperatures'");
        }
      }
    }

    // Contraint #2:The FlowControlWithApproachTemperatures only applies to ITE zones with single duct VAV terminal unit.
    if (istringEqual(thisMethod, "FlowControlWithApproachTemperatures")) {
      if (thermalZone) {
        auto terminal = thermalZone->airLoopHVACTerminal();
        if (terminal) {
          if (!terminal->optionalCast<AirTerminalSingleDuctVAVReheat>() && !terminal->optionalCast<AirTerminalSingleDuctVAVNoReheat>()) {
            LOG(Warn, modelObject.briefDescription() << " The FlowControlWithApproachTemperatures only applies to ITE zones with single duct VAV terminal unit.");
            return boost::none;
          }
        }
      }
    }

  } else {
    LOG(Warn, modelObject.briefDescription() << " not assigned to a valid space.");
  } // end of pre-process

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
  if (d){
    idfObject.setDouble(ElectricEquipment_ITE_AirCooledFields::WattsperUnit, (*d)*multiplier);
  }

  d = definition.wattsperZoneFloorArea();
  if (d){
    idfObject.setDouble(ElectricEquipment_ITE_AirCooledFields::WattsperZoneFloorArea, (*d)*multiplier);
  }

  boost::optional<Schedule> designPowerInputSchedule = modelObject.designPowerInputSchedule();
  if (designPowerInputSchedule){
    idfObject.setString(ElectricEquipment_ITE_AirCooledFields::DesignPowerInputScheduleName, designPowerInputSchedule->name().get());
  }
  
  boost::optional<Schedule> cPULoadingSchedule = modelObject.cPULoadingSchedule();
  if (cPULoadingSchedule){
    idfObject.setString(ElectricEquipment_ITE_AirCooledFields::CPULoadingScheduleName, cPULoadingSchedule->name().get());
  }

  idfObject.setString(ElectricEquipment_ITE_AirCooledFields::CPUPowerInputFunctionofLoadingandAirTemperatureCurveName, definition.cPUPowerInputFunctionofLoadingandAirTemperatureCurve().name().get());

  idfObject.setDouble(ElectricEquipment_ITE_AirCooledFields::DesignFanPowerInputFraction, definition.designFanPowerInputFraction());

  d = definition.designFanAirFlowRateperPowerInput();
  if (d) {
    idfObject.setDouble(ElectricEquipment_ITE_AirCooledFields::DesignFanAirFlowRateperPowerInput, *d);
  }

  idfObject.setString(ElectricEquipment_ITE_AirCooledFields::AirFlowFunctionofLoadingandAirTemperatureCurveName, definition.airFlowFunctionofLoadingandAirTemperatureCurve().name().get());

  idfObject.setString(ElectricEquipment_ITE_AirCooledFields::FanPowerInputFunctionofFlowCurveName, definition.fanPowerInputFunctionofFlowCurve().name().get());

  idfObject.setDouble(ElectricEquipment_ITE_AirCooledFields::DesignEnteringAirTemperature, definition.designEnteringAirTemperature());

  idfObject.setString(ElectricEquipment_ITE_AirCooledFields::EnvironmentalClass, definition.environmentalClass());

  idfObject.setString(ElectricEquipment_ITE_AirCooledFields::AirInletConnectionType, definition.airInletConnectionType());

  idfObject.setDouble(ElectricEquipment_ITE_AirCooledFields::DesignRecirculationFraction, definition.designRecirculationFraction());

  boost::optional<Curve> recirculationFunctionofLoadingandSupplyTemperatureCurve = definition.recirculationFunctionofLoadingandSupplyTemperatureCurve();
  if (recirculationFunctionofLoadingandSupplyTemperatureCurve) {
    idfObject.setString(ElectricEquipment_ITE_AirCooledFields::RecirculationFunctionofLoadingandSupplyTemperatureCurveName, recirculationFunctionofLoadingandSupplyTemperatureCurve->name().get());
  }

  idfObject.setDouble(ElectricEquipment_ITE_AirCooledFields::DesignElectricPowerSupplyEfficiency, definition.designElectricPowerSupplyEfficiency());

  boost::optional<Curve> electricPowerSupplyEfficiencyFunctionofPartLoadRatioCurve = definition.electricPowerSupplyEfficiencyFunctionofPartLoadRatioCurve();
  if (electricPowerSupplyEfficiencyFunctionofPartLoadRatioCurve) {
    idfObject.setString(ElectricEquipment_ITE_AirCooledFields::ElectricPowerSupplyEfficiencyFunctionofPartLoadRatioCurveName, electricPowerSupplyEfficiencyFunctionofPartLoadRatioCurve->name().get());
  }

  idfObject.setDouble(ElectricEquipment_ITE_AirCooledFields::FractionofElectricPowerSupplyLossestoZone, definition.fractionofElectricPowerSupplyLossestoZone());

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
    idfObject.setString(ElectricEquipment_ITE_AirCooledFields::ElectricPowerSupplyEndUseSubcategory, modelObject.electricPowerSupplyEndUseSubcategory());
  }

  idfObject.setDouble(ElectricEquipment_ITE_AirCooledFields::SupplyTemperatureDifference, definition.supplyTemperatureDifference());

  boost::optional<Schedule> supplyTemperatureDifferenceSchedule = definition.supplyTemperatureDifferenceSchedule();
  if (supplyTemperatureDifferenceSchedule) {
    idfObject.setString(ElectricEquipment_ITE_AirCooledFields::SupplyTemperatureDifferenceSchedule, supplyTemperatureDifferenceSchedule->name().get());
  }

  idfObject.setDouble(ElectricEquipment_ITE_AirCooledFields::ReturnTemperatureDifference, definition.returnTemperatureDifference());

  boost::optional<Schedule> returnTemperatureDifferenceSchedule = definition.returnTemperatureDifferenceSchedule();
  if (returnTemperatureDifferenceSchedule) {
    idfObject.setString(ElectricEquipment_ITE_AirCooledFields::ReturnTemperatureDifferenceSchedule, returnTemperatureDifferenceSchedule->name().get());
  }

  return idfObject;
}




} // energyplus

} // openstudio

