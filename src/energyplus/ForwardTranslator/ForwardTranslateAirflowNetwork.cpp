/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../../model/AirflowNetworkSimulationControl.hpp"
#include "../../model/AirflowNetworkSimulationControl_Impl.hpp"
#include "../../model/AirflowNetworkZone.hpp"
#include "../../model/AirflowNetworkZone_Impl.hpp"
#include "../../model/AirflowNetworkSurface.hpp"
#include "../../model/AirflowNetworkSurface_Impl.hpp"
#include "../../model/AirflowNetworkExternalNode.hpp"
#include "../../model/AirflowNetworkExternalNode_Impl.hpp"
#include "../../model/AirflowNetworkComponent.hpp"
#include "../../model/AirflowNetworkReferenceCrackConditions.hpp"
#include "../../model/AirflowNetworkReferenceCrackConditions_Impl.hpp"
#include "../../model/AirflowNetworkCrack.hpp"
#include "../../model/AirflowNetworkCrack_Impl.hpp"
#include "../../model/AirflowNetworkEffectiveLeakageArea.hpp"
#include "../../model/AirflowNetworkEffectiveLeakageArea_Impl.hpp"
#include "../../model/AirflowNetworkDetailedOpening.hpp"
#include "../../model/AirflowNetworkDetailedOpening_Impl.hpp"
#include "../../model/AirflowNetworkSimpleOpening.hpp"
#include "../../model/AirflowNetworkSimpleOpening_Impl.hpp"
#include "../../model/AirflowNetworkHorizontalOpening.hpp"
#include "../../model/AirflowNetworkHorizontalOpening_Impl.hpp"
#include "../../model/AirflowNetworkZoneExhaustFan.hpp"
#include "../../model/AirflowNetworkZoneExhaustFan_Impl.hpp"
#include "../../model/AirflowNetworkDistributionNode.hpp"
#include "../../model/AirflowNetworkDistributionNode_Impl.hpp"
#include "../../model/AirflowNetworkLeakageRatio.hpp"
#include "../../model/AirflowNetworkLeakageRatio_Impl.hpp"
#include "../../model/AirflowNetworkDuct.hpp"
#include "../../model/AirflowNetworkDuct_Impl.hpp"
#include "../../model/AirflowNetworkFan.hpp"
#include "../../model/AirflowNetworkFan_Impl.hpp"
#include "../../model/AirflowNetworkEquivalentDuct.hpp"
#include "../../model/AirflowNetworkEquivalentDuct_Impl.hpp"
#include "../../model/AirflowNetworkConstantPressureDrop.hpp"
#include "../../model/AirflowNetworkConstantPressureDrop_Impl.hpp"
#include "../../model/AirflowNetworkOutdoorAirflow.hpp"
#include "../../model/AirflowNetworkOutdoorAirflow_Impl.hpp"
#include "../../model/AirflowNetworkDistributionLinkage.hpp"
#include "../../model/AirflowNetworkDistributionLinkage_Impl.hpp"
#include "../../model/AirflowNetworkDuctViewFactors.hpp"
#include "../../model/AirflowNetworkDuctViewFactors_Impl.hpp"
#include "../../model/AirflowNetworkOccupantVentilationControl.hpp"
#include "../../model/AirflowNetworkOccupantVentilationControl_Impl.hpp"

#include "../../model/Schedule.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/PlanarSurface.hpp"
#include "../../model/Curve.hpp"
#include "../../model/StraightComponent.hpp"
#include "../../model/FanConstantVolume.hpp"
#include "../../model/FanVariableVolume.hpp"
#include "../../model/FanOnOff.hpp"
#include "../../model/FanSystemModel.hpp"
#include "../../model/Node.hpp"
#include "../../model/AirLoopHVACZoneMixer.hpp"
#include "../../model/AirLoopHVACZoneSplitter.hpp"
#include "../../model/AirLoopHVACOutdoorAirSystem.hpp"

#include "../../utilities/core/Assert.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/AirflowNetwork_SimulationControl_FieldEnums.hxx>
#include <utilities/idd/AirflowNetwork_MultiZone_Zone_FieldEnums.hxx>
#include <utilities/idd/AirflowNetwork_MultiZone_Surface_FieldEnums.hxx>
#include <utilities/idd/AirflowNetwork_MultiZone_ReferenceCrackConditions_FieldEnums.hxx>
#include <utilities/idd/AirflowNetwork_MultiZone_Surface_Crack_FieldEnums.hxx>
#include <utilities/idd/AirflowNetwork_MultiZone_Surface_EffectiveLeakageArea_FieldEnums.hxx>
#include <utilities/idd/AirflowNetwork_MultiZone_Component_DetailedOpening_FieldEnums.hxx>
#include <utilities/idd/AirflowNetwork_MultiZone_Component_SimpleOpening_FieldEnums.hxx>
#include <utilities/idd/AirflowNetwork_MultiZone_Component_HorizontalOpening_FieldEnums.hxx>
#include <utilities/idd/AirflowNetwork_MultiZone_Component_ZoneExhaustFan_FieldEnums.hxx>
#include <utilities/idd/AirflowNetwork_MultiZone_ExternalNode_FieldEnums.hxx>
#include <utilities/idd/AirflowNetwork_Distribution_Node_FieldEnums.hxx>
#include <utilities/idd/AirflowNetwork_Distribution_Component_LeakageRatio_FieldEnums.hxx>
#include <utilities/idd/AirflowNetwork_Distribution_Component_Duct_FieldEnums.hxx>
#include <utilities/idd/AirflowNetwork_Distribution_Component_Fan_FieldEnums.hxx>
#include <utilities/idd/AirflowNetwork_Distribution_Component_Coil_FieldEnums.hxx>
#include <utilities/idd/AirflowNetwork_Distribution_Component_HeatExchanger_FieldEnums.hxx>
#include <utilities/idd/AirflowNetwork_Distribution_Component_TerminalUnit_FieldEnums.hxx>
#include <utilities/idd/AirflowNetwork_Distribution_Component_ConstantPressureDrop_FieldEnums.hxx>
#include <utilities/idd/AirflowNetwork_Distribution_Component_OutdoorAirFlow_FieldEnums.hxx>
#include <utilities/idd/AirflowNetwork_Distribution_Linkage_FieldEnums.hxx>
#include <utilities/idd/AirflowNetwork_Distribution_DuctViewFactors_FieldEnums.hxx>
#include <utilities/idd/AirflowNetwork_OccupantVentilationControl_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {
namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateAirflowNetworkSimulationControl(AirflowNetworkSimulationControl& modelObject) {
    IdfObject idfObject(IddObjectType::AirflowNetwork_SimulationControl);

    m_idfObjects.push_back(idfObject);

    if (modelObject.name()) {
      if (modelObject.nameString().empty()) {
        idfObject.setString(AirflowNetwork_SimulationControlFields::Name, modelObject.nameString());
      } else {
        idfObject.setString(AirflowNetwork_SimulationControlFields::Name, "AFNSimControl 1");
      }
    } else {
      idfObject.setString(AirflowNetwork_SimulationControlFields::Name, "AFNSimControl 1");
    }
    if (modelObject.airflowNetworkControl()) {
      idfObject.setString(AirflowNetwork_SimulationControlFields::AirflowNetworkControl, modelObject.airflowNetworkControl().get());
    }
    if (modelObject.windPressureCoefficientType()) {
      idfObject.setString(AirflowNetwork_SimulationControlFields::WindPressureCoefficientType, modelObject.windPressureCoefficientType().get());
    }
    if (modelObject.heightSelectionforLocalWindPressureCalculation()) {
      idfObject.setString(AirflowNetwork_SimulationControlFields::HeightSelectionforLocalWindPressureCalculation,
                          modelObject.heightSelectionforLocalWindPressureCalculation().get());
    }
    if (modelObject.buildingType()) {
      idfObject.setString(AirflowNetwork_SimulationControlFields::BuildingType, modelObject.buildingType().get());
    }
    if (modelObject.maximumNumberofIterations()) {
      idfObject.setInt(AirflowNetwork_SimulationControlFields::MaximumNumberofIterations, modelObject.maximumNumberofIterations().get());
    }
    if (modelObject.initializationType()) {
      idfObject.setString(AirflowNetwork_SimulationControlFields::InitializationType, modelObject.initializationType().get());
    }
    if (modelObject.relativeAirflowConvergenceTolerance()) {
      idfObject.setDouble(AirflowNetwork_SimulationControlFields::RelativeAirflowConvergenceTolerance,
                          modelObject.relativeAirflowConvergenceTolerance().get());
    }
    if (modelObject.absoluteAirflowConvergenceTolerance()) {
      idfObject.setDouble(AirflowNetwork_SimulationControlFields::AbsoluteAirflowConvergenceTolerance,
                          modelObject.absoluteAirflowConvergenceTolerance().get());
    }
    if (modelObject.convergenceAccelerationLimit()) {
      idfObject.setDouble(AirflowNetwork_SimulationControlFields::ConvergenceAccelerationLimit, modelObject.convergenceAccelerationLimit().get());
    }
    if (modelObject.azimuthAngleofLongAxisofBuilding()) {
      idfObject.setDouble(AirflowNetwork_SimulationControlFields::AzimuthAngleofLongAxisofBuilding,
                          modelObject.azimuthAngleofLongAxisofBuilding().get());
    }
    if (modelObject.buildingAspectRatio()) {
      idfObject.setDouble(AirflowNetwork_SimulationControlFields::RatioofBuildingWidthAlongShortAxistoWidthAlongLongAxis,
                          modelObject.buildingAspectRatio().get());
    }
    if (modelObject.heightDependenceofExternalNodeTemperature()) {
      idfObject.setString(AirflowNetwork_SimulationControlFields::HeightDependenceofExternalNodeTemperature, "Yes");
    }
    //if (modelObject.solver()){
    //  idfObject.setString(AirflowNetwork_SimulationControlFields::Solver,modelObject.solver().get());
    //}

    return idfObject;
  }

  boost::optional<IdfObject> ForwardTranslator::translateAirflowNetworkZone(AirflowNetworkZone& modelObject) {
    IdfObject idfObject(IddObjectType::AirflowNetwork_MultiZone_Zone);

    m_idfObjects.push_back(idfObject);

    idfObject.setString(AirflowNetwork_MultiZone_ZoneFields::ZoneName, modelObject.thermalZone().nameString());
    idfObject.setString(AirflowNetwork_MultiZone_ZoneFields::VentilationControlMode, modelObject.ventilationControlMode());
    if (modelObject.ventilationControlZoneTemperatureSetpointSchedule()) {
      idfObject.setString(AirflowNetwork_MultiZone_ZoneFields::VentilationControlZoneTemperatureSetpointScheduleName,
                          modelObject.ventilationControlZoneTemperatureSetpointSchedule()->nameString());
    }
    idfObject.setDouble(AirflowNetwork_MultiZone_ZoneFields::MinimumVentingOpenFactor, modelObject.minimumVentingOpenFactor());
    idfObject.setDouble(AirflowNetwork_MultiZone_ZoneFields::IndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor,
                        modelObject.indoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor());
    idfObject.setDouble(AirflowNetwork_MultiZone_ZoneFields::IndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor,
                        modelObject.indoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor());
    idfObject.setDouble(AirflowNetwork_MultiZone_ZoneFields::IndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor,
                        modelObject.indoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor());
    idfObject.setDouble(AirflowNetwork_MultiZone_ZoneFields::IndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor,
                        modelObject.indoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor());
    if (modelObject.ventingAvailabilitySchedule()) {
      idfObject.setString(AirflowNetwork_MultiZone_ZoneFields::VentingAvailabilityScheduleName,
                          modelObject.ventingAvailabilitySchedule()->nameString());
    }
    idfObject.setString(AirflowNetwork_MultiZone_ZoneFields::SingleSidedWindPressureCoefficientAlgorithm,
                        modelObject.singleSidedWindPressureCoefficientAlgorithm());
    idfObject.setDouble(AirflowNetwork_MultiZone_ZoneFields::FacadeWidth, modelObject.facadeWidth());
    if (modelObject.occupantVentilationControl()) {
      idfObject.setString(AirflowNetwork_MultiZone_ZoneFields::OccupantVentilationControlName,
                          modelObject.occupantVentilationControl()->nameString());
    }

    return idfObject;
  }

  boost::optional<IdfObject> ForwardTranslator::translateAirflowNetworkSurface(AirflowNetworkSurface& modelObject) {
    IdfObject idfObject(IddObjectType::AirflowNetwork_MultiZone_Surface);

    m_idfObjects.push_back(idfObject);

    idfObject.setString(AirflowNetwork_MultiZone_SurfaceFields::SurfaceName, modelObject.surface().nameString());
    idfObject.setString(AirflowNetwork_MultiZone_SurfaceFields::LeakageComponentName, modelObject.leakageComponent()->nameString());
    if (modelObject.externalNode()) {
      idfObject.setString(AirflowNetwork_MultiZone_SurfaceFields::ExternalNodeName, modelObject.externalNode()->nameString());
    }
    idfObject.setDouble(AirflowNetwork_MultiZone_SurfaceFields::Window_DoorOpeningFactororCrackFactor,
                        modelObject.windowDoorOpeningFactorOrCrackFactor());
    idfObject.setString(AirflowNetwork_MultiZone_SurfaceFields::VentilationControlMode, modelObject.ventilationControlMode());
    if (modelObject.ventilationControlZoneTemperatureSetpointSchedule()) {
      idfObject.setString(AirflowNetwork_MultiZone_SurfaceFields::VentilationControlZoneTemperatureSetpointScheduleName,
                          modelObject.ventilationControlZoneTemperatureSetpointSchedule()->nameString());
    }
    idfObject.setDouble(AirflowNetwork_MultiZone_SurfaceFields::MinimumVentingOpenFactor, modelObject.minimumVentingOpenFactor());
    idfObject.setDouble(AirflowNetwork_MultiZone_SurfaceFields::IndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor,
                        modelObject.indoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor());
    idfObject.setDouble(AirflowNetwork_MultiZone_SurfaceFields::IndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor,
                        modelObject.indoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor());
    idfObject.setDouble(AirflowNetwork_MultiZone_SurfaceFields::IndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor,
                        modelObject.indoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor());
    idfObject.setDouble(AirflowNetwork_MultiZone_SurfaceFields::IndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor,
                        modelObject.indoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor());
    if (modelObject.ventingAvailabilitySchedule()) {
      idfObject.setString(AirflowNetwork_MultiZone_SurfaceFields::VentingAvailabilityScheduleName,
                          modelObject.ventingAvailabilitySchedule()->nameString());
    }
    if (modelObject.occupantVentilationControl()) {
      idfObject.setString(AirflowNetwork_MultiZone_SurfaceFields::OccupantVentilationControlName,
                          modelObject.occupantVentilationControl()->nameString());
    }
    //if (modelObject.equivalentRectangleMethod()) {
    //  idfObject.setString(AirflowNetwork_MultiZone_SurfaceFields::EquivalentRectangleMethod, modelObject.equivalentRectangleMethod().get());
    //}
    //if (modelObject.equivalentRectangleAspectRatio()) {
    //  idfObject.setDouble(AirflowNetwork_MultiZone_SurfaceFields::EquivalentRectangleAspectRatio, modelObject.equivalentRectangleAspectRatio().get());
    //}

    return idfObject;
  }

  boost::optional<IdfObject> ForwardTranslator::translateAirflowNetworkReferenceCrackConditions(AirflowNetworkReferenceCrackConditions& modelObject) {
    IdfObject idfObject(IddObjectType::AirflowNetwork_MultiZone_ReferenceCrackConditions);

    m_idfObjects.push_back(idfObject);

    idfObject.setString(AirflowNetwork_MultiZone_ReferenceCrackConditionsFields::Name, modelObject.nameString());
    idfObject.setDouble(AirflowNetwork_MultiZone_ReferenceCrackConditionsFields::ReferenceTemperature, modelObject.temperature());
    idfObject.setDouble(AirflowNetwork_MultiZone_ReferenceCrackConditionsFields::ReferenceBarometricPressure, modelObject.barometricPressure());
    idfObject.setDouble(AirflowNetwork_MultiZone_ReferenceCrackConditionsFields::ReferenceHumidityRatio, modelObject.humidityRatio());

    return idfObject;
  }

  boost::optional<IdfObject> ForwardTranslator::translateAirflowNetworkCrack(AirflowNetworkCrack& modelObject) {
    IdfObject idfObject(IddObjectType::AirflowNetwork_MultiZone_Surface_Crack);

    m_idfObjects.push_back(idfObject);

    idfObject.setString(AirflowNetwork_MultiZone_Surface_CrackFields::Name, modelObject.nameString());
    idfObject.setDouble(AirflowNetwork_MultiZone_Surface_CrackFields::AirMassFlowCoefficientatReferenceConditions,
                        modelObject.airMassFlowCoefficient());
    idfObject.setDouble(AirflowNetwork_MultiZone_Surface_CrackFields::AirMassFlowExponent, modelObject.airMassFlowExponent());
    if (modelObject.referenceCrackConditions()) {
      idfObject.setString(AirflowNetwork_MultiZone_Surface_CrackFields::ReferenceCrackConditions,
                          modelObject.referenceCrackConditions()->nameString());
    }

    return idfObject;
  }

  boost::optional<IdfObject> ForwardTranslator::translateAirflowNetworkEffectiveLeakageArea(AirflowNetworkEffectiveLeakageArea& modelObject) {
    IdfObject idfObject(IddObjectType::AirflowNetwork_MultiZone_Surface_EffectiveLeakageArea);

    m_idfObjects.push_back(idfObject);

    idfObject.setString(AirflowNetwork_MultiZone_Surface_EffectiveLeakageAreaFields::Name, modelObject.nameString());
    idfObject.setDouble(AirflowNetwork_MultiZone_Surface_EffectiveLeakageAreaFields::EffectiveLeakageArea, modelObject.effectiveLeakageArea());
    idfObject.setDouble(AirflowNetwork_MultiZone_Surface_EffectiveLeakageAreaFields::DischargeCoefficient, modelObject.dischargeCoefficient());
    idfObject.setDouble(AirflowNetwork_MultiZone_Surface_EffectiveLeakageAreaFields::ReferencePressureDifference,
                        modelObject.referencePressureDifference());
    idfObject.setDouble(AirflowNetwork_MultiZone_Surface_EffectiveLeakageAreaFields::AirMassFlowExponent, modelObject.airMassFlowExponent());

    return idfObject;
  }

  boost::optional<IdfObject> ForwardTranslator::translateAirflowNetworkDetailedOpening(AirflowNetworkDetailedOpening& modelObject) {
    IdfObject idfObject(IddObjectType::AirflowNetwork_MultiZone_Component_DetailedOpening);

    m_idfObjects.push_back(idfObject);

    idfObject.setString(AirflowNetwork_MultiZone_Component_DetailedOpeningFields::Name, modelObject.nameString());
    idfObject.setDouble(AirflowNetwork_MultiZone_Component_DetailedOpeningFields::AirMassFlowCoefficientWhenOpeningisClosed,
                        modelObject.airMassFlowCoefficientWhenOpeningisClosed());
    idfObject.setDouble(AirflowNetwork_MultiZone_Component_DetailedOpeningFields::AirMassFlowExponentWhenOpeningisClosed,
                        modelObject.airMassFlowExponentWhenOpeningisClosed());
    idfObject.setString(AirflowNetwork_MultiZone_Component_DetailedOpeningFields::TypeofRectangularLargeVerticalOpening_LVO_,
                        modelObject.typeofRectangularLargeVerticalOpening());
    idfObject.setDouble(AirflowNetwork_MultiZone_Component_DetailedOpeningFields::ExtraCrackLengthorHeightofPivotingAxis,
                        modelObject.extraCrackLengthorHeightofPivotingAxis());
    auto factors = modelObject.openingFactors();
    int N = static_cast<int>(factors.size());
    idfObject.setInt(AirflowNetwork_MultiZone_Component_DetailedOpeningFields::NumberofSetsofOpeningFactorData, std::min(N, 4));

    idfObject.setDouble(AirflowNetwork_MultiZone_Component_DetailedOpeningFields::OpeningFactor1, factors[0].openingFactor());
    idfObject.setDouble(AirflowNetwork_MultiZone_Component_DetailedOpeningFields::DischargeCoefficientforOpeningFactor1,
                        factors[0].dischargeCoefficient());
    idfObject.setDouble(AirflowNetwork_MultiZone_Component_DetailedOpeningFields::WidthFactorforOpeningFactor1, factors[0].widthFactor());
    idfObject.setDouble(AirflowNetwork_MultiZone_Component_DetailedOpeningFields::HeightFactorforOpeningFactor1, factors[0].heightFactor());
    idfObject.setDouble(AirflowNetwork_MultiZone_Component_DetailedOpeningFields::StartHeightFactorforOpeningFactor1, factors[0].startHeightFactor());

    idfObject.setDouble(AirflowNetwork_MultiZone_Component_DetailedOpeningFields::OpeningFactor2, factors[1].openingFactor());
    idfObject.setDouble(AirflowNetwork_MultiZone_Component_DetailedOpeningFields::DischargeCoefficientforOpeningFactor2,
                        factors[1].dischargeCoefficient());
    idfObject.setDouble(AirflowNetwork_MultiZone_Component_DetailedOpeningFields::WidthFactorforOpeningFactor2, factors[1].widthFactor());
    idfObject.setDouble(AirflowNetwork_MultiZone_Component_DetailedOpeningFields::HeightFactorforOpeningFactor2, factors[1].heightFactor());
    idfObject.setDouble(AirflowNetwork_MultiZone_Component_DetailedOpeningFields::StartHeightFactorforOpeningFactor2, factors[1].startHeightFactor());

    if (N > 2) {
      idfObject.setDouble(AirflowNetwork_MultiZone_Component_DetailedOpeningFields::OpeningFactor3, factors[2].openingFactor());
      idfObject.setDouble(AirflowNetwork_MultiZone_Component_DetailedOpeningFields::DischargeCoefficientforOpeningFactor3,
                          factors[2].dischargeCoefficient());
      idfObject.setDouble(AirflowNetwork_MultiZone_Component_DetailedOpeningFields::WidthFactorforOpeningFactor3, factors[2].widthFactor());
      idfObject.setDouble(AirflowNetwork_MultiZone_Component_DetailedOpeningFields::HeightFactorforOpeningFactor3, factors[2].heightFactor());
      idfObject.setDouble(AirflowNetwork_MultiZone_Component_DetailedOpeningFields::StartHeightFactorforOpeningFactor3,
                          factors[2].startHeightFactor());

      if (N > 3) {
        idfObject.setDouble(AirflowNetwork_MultiZone_Component_DetailedOpeningFields::OpeningFactor4, factors[3].openingFactor());
        idfObject.setDouble(AirflowNetwork_MultiZone_Component_DetailedOpeningFields::DischargeCoefficientforOpeningFactor4,
                            factors[3].dischargeCoefficient());
        idfObject.setDouble(AirflowNetwork_MultiZone_Component_DetailedOpeningFields::WidthFactorforOpeningFactor4, factors[3].widthFactor());
        idfObject.setDouble(AirflowNetwork_MultiZone_Component_DetailedOpeningFields::HeightFactorforOpeningFactor4, factors[3].heightFactor());
        idfObject.setDouble(AirflowNetwork_MultiZone_Component_DetailedOpeningFields::StartHeightFactorforOpeningFactor4,
                            factors[3].startHeightFactor());
      }
    }

    return idfObject;
  }

  boost::optional<IdfObject> ForwardTranslator::translateAirflowNetworkSimpleOpening(AirflowNetworkSimpleOpening& modelObject) {
    IdfObject idfObject(IddObjectType::AirflowNetwork_MultiZone_Component_SimpleOpening);

    m_idfObjects.push_back(idfObject);

    idfObject.setString(AirflowNetwork_MultiZone_Component_SimpleOpeningFields::Name, modelObject.nameString());
    idfObject.setDouble(AirflowNetwork_MultiZone_Component_SimpleOpeningFields::AirMassFlowCoefficientWhenOpeningisClosed,
                        modelObject.airMassFlowCoefficientWhenOpeningisClosed());
    idfObject.setDouble(AirflowNetwork_MultiZone_Component_SimpleOpeningFields::AirMassFlowExponentWhenOpeningisClosed,
                        modelObject.airMassFlowExponentWhenOpeningisClosed());
    idfObject.setDouble(AirflowNetwork_MultiZone_Component_SimpleOpeningFields::MinimumDensityDifferenceforTwoWayFlow,
                        modelObject.minimumDensityDifferenceforTwoWayFlow());
    idfObject.setDouble(AirflowNetwork_MultiZone_Component_SimpleOpeningFields::DischargeCoefficient, modelObject.dischargeCoefficient());

    return idfObject;
  }

  boost::optional<IdfObject> ForwardTranslator::translateAirflowNetworkHorizontalOpening(AirflowNetworkHorizontalOpening& modelObject) {
    IdfObject idfObject(IddObjectType::AirflowNetwork_MultiZone_Component_HorizontalOpening);

    m_idfObjects.push_back(idfObject);

    idfObject.setString(AirflowNetwork_MultiZone_Component_HorizontalOpeningFields::Name, modelObject.nameString());
    idfObject.setDouble(AirflowNetwork_MultiZone_Component_HorizontalOpeningFields::AirMassFlowCoefficientWhenOpeningisClosed,
                        modelObject.airMassFlowCoefficientWhenOpeningisClosed());
    idfObject.setDouble(AirflowNetwork_MultiZone_Component_HorizontalOpeningFields::AirMassFlowExponentWhenOpeningisClosed,
                        modelObject.airMassFlowExponentWhenOpeningisClosed());
    idfObject.setDouble(AirflowNetwork_MultiZone_Component_HorizontalOpeningFields::SlopingPlaneAngle, modelObject.slopingPlaneAngle());
    idfObject.setDouble(AirflowNetwork_MultiZone_Component_HorizontalOpeningFields::DischargeCoefficient, modelObject.dischargeCoefficient());

    return idfObject;
  }

  boost::optional<IdfObject> ForwardTranslator::translateAirflowNetworkZoneExhaustFan(AirflowNetworkZoneExhaustFan& modelObject) {
    IdfObject idfObject(IddObjectType::AirflowNetwork_MultiZone_Component_ZoneExhaustFan);

    if (modelObject.crack()) {
      m_idfObjects.push_back(idfObject);

      idfObject.setString(AirflowNetwork_MultiZone_Component_ZoneExhaustFanFields::Name, modelObject.nameString());
      idfObject.setDouble(
        AirflowNetwork_MultiZone_Component_ZoneExhaustFanFields::AirMassFlowCoefficientWhentheZoneExhaustFanisOffatReferenceConditions,
        modelObject.crack()->airMassFlowCoefficient());
      idfObject.setDouble(AirflowNetwork_MultiZone_Component_ZoneExhaustFanFields::AirMassFlowExponentWhentheZoneExhaustFanisOff,
                          modelObject.crack()->airMassFlowExponent());
      if (modelObject.crack()->referenceCrackConditions()) {
        idfObject.setString(AirflowNetwork_MultiZone_Component_ZoneExhaustFanFields::ReferenceCrackConditions,
                            modelObject.crack()->referenceCrackConditions()->nameString());
      }
    }  // else warning?
    return idfObject;
  }

  boost::optional<IdfObject> ForwardTranslator::translateAirflowNetworkExternalNode(AirflowNetworkExternalNode& modelObject) {
    IdfObject idfObject(IddObjectType::AirflowNetwork_MultiZone_ExternalNode);

    m_idfObjects.push_back(idfObject);

    idfObject.setString(AirflowNetwork_MultiZone_ExternalNodeFields::Name, modelObject.nameString());
    idfObject.setDouble(AirflowNetwork_MultiZone_ExternalNodeFields::ExternalNodeHeight, modelObject.externalNodeHeight());
    idfObject.setString(AirflowNetwork_MultiZone_ExternalNodeFields::WindPressureCoefficientCurveName,
                        modelObject.windPressureCoefficientCurve().nameString());
    if (modelObject.symmetricWindPressureCoefficientCurve()) {
      idfObject.setString(AirflowNetwork_MultiZone_ExternalNodeFields::SymmetricWindPressureCoefficientCurve, "Yes");
    }
    idfObject.setString(AirflowNetwork_MultiZone_ExternalNodeFields::WindAngleType, modelObject.windAngleType());

    return idfObject;
  }

  boost::optional<IdfObject> ForwardTranslator::translateAirflowNetworkDistributionNode(AirflowNetworkDistributionNode& modelObject) {
    IdfObject idfObject(IddObjectType::AirflowNetwork_Distribution_Node);

    m_idfObjects.push_back(idfObject);

    idfObject.setString(AirflowNetwork_Distribution_NodeFields::Name, modelObject.nameString());

    if (modelObject.node()) {
      idfObject.setString(AirflowNetwork_Distribution_NodeFields::ComponentNameorNodeName, modelObject.node().get().nameString());
      idfObject.setString(AirflowNetwork_Distribution_NodeFields::ComponentObjectTypeorNodeType, "Other");
    } else if (modelObject.airLoopHVACZoneMixer()) {
      // JWD: This and the comments below will probably be uncommented and used once multiple airloops hit.
      //idfObject.setString(AirflowNetwork_Distribution_NodeFields::ComponentNameorNodeName, modelObject.airLoopHVACZoneMixer().get().nameString());
      idfObject.setString(AirflowNetwork_Distribution_NodeFields::ComponentObjectTypeorNodeType, "AirLoopHVAC:ZoneMixer");
    } else if (modelObject.airLoopHVACZoneSplitter()) {
      //idfObject.setString(AirflowNetwork_Distribution_NodeFields::ComponentNameorNodeName, modelObject.airLoopHVACZoneSplitter().get().nameString());
      idfObject.setString(AirflowNetwork_Distribution_NodeFields::ComponentObjectTypeorNodeType, "AirLoopHVAC:ZoneSplitter");
    } else if (modelObject.airLoopHVACOutdoorAirSystem()) {
      //idfObject.setString(AirflowNetwork_Distribution_NodeFields::ComponentNameorNodeName, modelObject.airLoopHVACOutdoorAirSystem().get().nameString());
      idfObject.setString(AirflowNetwork_Distribution_NodeFields::ComponentObjectTypeorNodeType, "AirLoopHVAC:OutdoorAirSystem");
    } else {
      idfObject.setString(AirflowNetwork_Distribution_NodeFields::ComponentObjectTypeorNodeType, "Other");
    }

    idfObject.setDouble(AirflowNetwork_Distribution_NodeFields::NodeHeight, modelObject.nodeHeight());
    return idfObject;
  }

  boost::optional<IdfObject> ForwardTranslator::translateAirflowNetworkLeakageRatio(AirflowNetworkLeakageRatio& modelObject) {
    IdfObject idfObject(IddObjectType::AirflowNetwork_Distribution_Component_LeakageRatio);

    m_idfObjects.push_back(idfObject);

    idfObject.setString(AirflowNetwork_Distribution_Component_LeakageRatioFields::Name, modelObject.nameString());
    if (modelObject.effectiveLeakageRatio()) {
      idfObject.setDouble(AirflowNetwork_Distribution_Component_LeakageRatioFields::EffectiveLeakageRatio, modelObject.effectiveLeakageRatio().get());
    }
    idfObject.setDouble(AirflowNetwork_Distribution_Component_LeakageRatioFields::MaximumFlowRate, modelObject.maximumFlowRate());
    idfObject.setDouble(AirflowNetwork_Distribution_Component_LeakageRatioFields::ReferencePressureDifference,
                        modelObject.referencePressureDifference());
    idfObject.setDouble(AirflowNetwork_Distribution_Component_LeakageRatioFields::AirMassFlowExponent, modelObject.airMassFlowExponent());

    return idfObject;
  }

  boost::optional<IdfObject> ForwardTranslator::translateAirflowNetworkDuct(AirflowNetworkDuct& modelObject) {
    IdfObject idfObject(IddObjectType::AirflowNetwork_Distribution_Component_Duct);

    m_idfObjects.push_back(idfObject);

    idfObject.setString(AirflowNetwork_Distribution_Component_DuctFields::Name, modelObject.nameString());
    idfObject.setDouble(AirflowNetwork_Distribution_Component_DuctFields::DuctLength, modelObject.ductLength());
    idfObject.setDouble(AirflowNetwork_Distribution_Component_DuctFields::HydraulicDiameter, modelObject.hydraulicDiameter());
    idfObject.setDouble(AirflowNetwork_Distribution_Component_DuctFields::CrossSectionArea, modelObject.crossSectionArea());
    idfObject.setDouble(AirflowNetwork_Distribution_Component_DuctFields::SurfaceRoughness, modelObject.surfaceRoughness());
    idfObject.setDouble(AirflowNetwork_Distribution_Component_DuctFields::CoefficientforLocalDynamicLossDuetoFitting,
                        modelObject.coefficientforLocalDynamicLossDuetoFitting());
    idfObject.setDouble(AirflowNetwork_Distribution_Component_DuctFields::HeatTransmittanceCoefficient_UFactor_forDuctWallConstruction,
                        modelObject.ductWallHeatTransmittanceCoefficient());
    idfObject.setDouble(AirflowNetwork_Distribution_Component_DuctFields::OverallMoistureTransmittanceCoefficientfromAirtoAir,
                        modelObject.overallMoistureTransmittanceCoefficientfromAirtoAir());
    idfObject.setDouble(AirflowNetwork_Distribution_Component_DuctFields::OutsideConvectionCoefficient, modelObject.outsideConvectionCoefficient());
    idfObject.setDouble(AirflowNetwork_Distribution_Component_DuctFields::InsideConvectionCoefficient, modelObject.insideConvectionCoefficient());

    return idfObject;
  }

  boost::optional<IdfObject> ForwardTranslator::translateAirflowNetworkFan(AirflowNetworkFan& modelObject) {
    IdfObject idfObject(IddObjectType::AirflowNetwork_Distribution_Component_Fan);

    if (boost::optional<FanConstantVolume> _fan = modelObject.fanConstantVolume()) {
      idfObject.setString(AirflowNetwork_Distribution_Component_FanFields::FanName, _fan->nameString());
      idfObject.setString(AirflowNetwork_Distribution_Component_FanFields::SupplyFanObjectType, "Fan:ConstantVolume");
      m_idfObjects.push_back(idfObject);
    } else if (boost::optional<FanVariableVolume> _fan = modelObject.fanVariableVolume()) {
      idfObject.setString(AirflowNetwork_Distribution_Component_FanFields::FanName, _fan->nameString());
      idfObject.setString(AirflowNetwork_Distribution_Component_FanFields::SupplyFanObjectType, "Fan:VariableVolume");
      m_idfObjects.push_back(idfObject);
    } else if (boost::optional<FanOnOff> _fan = modelObject.fanOnOff()) {
      idfObject.setString(AirflowNetwork_Distribution_Component_FanFields::FanName, _fan->nameString());
      idfObject.setString(AirflowNetwork_Distribution_Component_FanFields::SupplyFanObjectType, "Fan:OnOff");
      m_idfObjects.push_back(idfObject);
    } else if (boost::optional<FanSystemModel> _fan = modelObject.fanSystemModel()) {
      idfObject.setString(AirflowNetwork_Distribution_Component_FanFields::FanName, _fan->nameString());
      idfObject.setString(AirflowNetwork_Distribution_Component_FanFields::SupplyFanObjectType, "Fan:SystemModel");
      m_idfObjects.push_back(idfObject);
    } else {
      // Warning or error?
    }

    return idfObject;
  }

  boost::optional<IdfObject> ForwardTranslator::translateAirflowNetworkEquivalentDuct(AirflowNetworkEquivalentDuct& modelObject) {
    boost::optional<std::string> opttype = modelObject.coilObjectType();
    if (opttype) {
      // Coil
      IdfObject idfObject(IddObjectType::AirflowNetwork_Distribution_Component_Coil);
      m_idfObjects.push_back(idfObject);
      idfObject.setString(AirflowNetwork_Distribution_Component_CoilFields::CoilName, modelObject.straightComponent().get().nameString());
      idfObject.setString(AirflowNetwork_Distribution_Component_CoilFields::CoilObjectType, opttype.get());
      idfObject.setDouble(AirflowNetwork_Distribution_Component_CoilFields::AirPathLength, modelObject.airPathLength());
      idfObject.setDouble(AirflowNetwork_Distribution_Component_CoilFields::AirPathHydraulicDiameter, modelObject.airPathHydraulicDiameter());
      return idfObject;
    }

    opttype = modelObject.heatExchangerObjectType();
    if (opttype) {
      // Heat exchanger
      IdfObject idfObject(IddObjectType::AirflowNetwork_Distribution_Component_HeatExchanger);
      m_idfObjects.push_back(idfObject);
      idfObject.setString(AirflowNetwork_Distribution_Component_HeatExchangerFields::HeatExchangerName, modelObject.nameString());
      idfObject.setString(AirflowNetwork_Distribution_Component_HeatExchangerFields::HeatExchangerObjectType, opttype.get());
      idfObject.setDouble(AirflowNetwork_Distribution_Component_HeatExchangerFields::AirPathLength, modelObject.airPathLength());
      idfObject.setDouble(AirflowNetwork_Distribution_Component_HeatExchangerFields::AirPathHydraulicDiameter,
                          modelObject.airPathHydraulicDiameter());
      return idfObject;
    }

    opttype = modelObject.terminalUnitObjectType();
    if (opttype) {
      // Terminal Unit
      IdfObject idfObject(IddObjectType::AirflowNetwork_Distribution_Component_TerminalUnit);
      m_idfObjects.push_back(idfObject);
      idfObject.setString(AirflowNetwork_Distribution_Component_TerminalUnitFields::TerminalUnitName, modelObject.nameString());
      idfObject.setString(AirflowNetwork_Distribution_Component_TerminalUnitFields::TerminalUnitObjectType, opttype.get());
      idfObject.setDouble(AirflowNetwork_Distribution_Component_TerminalUnitFields::AirPathLength, modelObject.airPathLength());
      idfObject.setDouble(AirflowNetwork_Distribution_Component_TerminalUnitFields::AirPathHydraulicDiameter, modelObject.airPathHydraulicDiameter());
      return idfObject;
    }

    return boost::none;
  }

  boost::optional<IdfObject> ForwardTranslator::translateAirflowNetworkConstantPressureDrop(AirflowNetworkConstantPressureDrop& modelObject) {
    IdfObject idfObject(IddObjectType::AirflowNetwork_Distribution_Component_ConstantPressureDrop);

    m_idfObjects.push_back(idfObject);

    idfObject.setString(AirflowNetwork_Distribution_Component_ConstantPressureDropFields::Name, modelObject.nameString());
    idfObject.setDouble(AirflowNetwork_Distribution_Component_ConstantPressureDropFields::PressureDifferenceAcrosstheComponent,
                        modelObject.pressureDrop());

    return idfObject;
  }

  boost::optional<IdfObject> ForwardTranslator::translateAirflowNetworkOutdoorAirflow(AirflowNetworkOutdoorAirflow& modelObject) {
    IdfObject idfObject(IddObjectType::AirflowNetwork_Distribution_Component_OutdoorAirFlow);

    if (modelObject.crack()) {
      m_idfObjects.push_back(idfObject);

      idfObject.setString(AirflowNetwork_MultiZone_Component_ZoneExhaustFanFields::Name, modelObject.nameString());
      idfObject.setDouble(
        AirflowNetwork_MultiZone_Component_ZoneExhaustFanFields::AirMassFlowCoefficientWhentheZoneExhaustFanisOffatReferenceConditions,
        modelObject.crack()->airMassFlowCoefficient());
      idfObject.setDouble(AirflowNetwork_MultiZone_Component_ZoneExhaustFanFields::AirMassFlowExponentWhentheZoneExhaustFanisOff,
                          modelObject.crack()->airMassFlowExponent());
      if (modelObject.crack()->referenceCrackConditions()) {
        idfObject.setString(AirflowNetwork_MultiZone_Component_ZoneExhaustFanFields::ReferenceCrackConditions,
                            modelObject.crack()->referenceCrackConditions()->nameString());
      }
    }  // else warning?

    return idfObject;
  }

  boost::optional<IdfObject> ForwardTranslator::translateAirflowNetworkDistributionLinkage(AirflowNetworkDistributionLinkage& modelObject) {
    IdfObject idfObject(IddObjectType::AirflowNetwork_Distribution_Linkage);

    m_idfObjects.push_back(idfObject);

    idfObject.setString(AirflowNetwork_Distribution_LinkageFields::Name, modelObject.nameString());

    boost::optional<IdfObject> obj;

    AirflowNetworkNode node1 = modelObject.node1();
    if (node1.optionalCast<AirflowNetworkZone>()) {
      // DLM: Jason, the thermal zone and other objects may be renamed on translation, need to get the translated object's name
      auto z = node1.cast<AirflowNetworkZone>().thermalZone();
      obj = translateAndMapModelObject(z);
      OS_ASSERT(obj);
      idfObject.setString(AirflowNetwork_Distribution_LinkageFields::Node1Name, obj->nameString());
    } else {
      idfObject.setString(AirflowNetwork_Distribution_LinkageFields::Node1Name, node1.nameString());
    }

    AirflowNetworkNode node2 = modelObject.node2();
    if (node2.optionalCast<AirflowNetworkZone>()) {
      auto z = node1.cast<AirflowNetworkZone>().thermalZone();
      obj = translateAndMapModelObject(z);
      OS_ASSERT(obj);
      idfObject.setString(AirflowNetwork_Distribution_LinkageFields::Node2Name, obj->nameString());
    } else {
      idfObject.setString(AirflowNetwork_Distribution_LinkageFields::Node2Name, node2.nameString());
    }

    AirflowNetworkComponent component = modelObject.component();
    switch (component.iddObject().type().value()) {
      case openstudio::IddObjectType::OS_AirflowNetworkFan:
      case openstudio::IddObjectType::OS_AirflowNetworkEquivalentDuct:
        OS_ASSERT(component.componentModelObject());
        obj = translateAndMapModelObject(component.componentModelObject().get());
        OS_ASSERT(obj);
        idfObject.setString(AirflowNetwork_Distribution_LinkageFields::ComponentName, obj->nameString());
        break;
      default:
        idfObject.setString(AirflowNetwork_Distribution_LinkageFields::ComponentName, component.nameString());
        break;
    }

    if (modelObject.thermalZone()) {
      obj = translateAndMapModelObject(modelObject.thermalZone().get());
      OS_ASSERT(obj);
      idfObject.setString(AirflowNetwork_Distribution_LinkageFields::ThermalZoneName, obj->nameString());
    }

    return idfObject;
  }

  boost::optional<IdfObject> ForwardTranslator::translateAirflowNetworkDuctViewFactors(AirflowNetworkDuctViewFactors& modelObject) {
    IdfObject idfObject(IddObjectType::AirflowNetwork_Distribution_DuctViewFactors);

    std::vector<ViewFactorData> vfs = modelObject.viewFactors();

    if (vfs.size() > 0) {
      m_idfObjects.push_back(idfObject);

      idfObject.setString(AirflowNetwork_Distribution_DuctViewFactorsFields::LinkageName, modelObject.linkage().nameString());
      idfObject.setDouble(AirflowNetwork_Distribution_DuctViewFactorsFields::DuctSurfaceExposureFraction, modelObject.ductSurfaceExposureFraction());
      idfObject.setDouble(AirflowNetwork_Distribution_DuctViewFactorsFields::DuctSurfaceEmittance, modelObject.ductSurfaceEmittance());

      idfObject.clearExtensibleGroups();
      for (const auto& vf : vfs) {
        IdfExtensibleGroup group = idfObject.pushExtensibleGroup();
        OS_ASSERT(group.numFields() == 2);
        group.setString(0, vf.planarSurface().nameString());
        group.setDouble(1, vf.viewFactor());
      }

      return idfObject;
    } else {
      LOG(Warn, "Empty view factor object " << modelObject.briefDescription() << " not translated.");
    }

    return boost::none;
  }

  boost::optional<IdfObject>
    ForwardTranslator::translateAirflowNetworkOccupantVentilationControl(AirflowNetworkOccupantVentilationControl& modelObject) {
    IdfObject idfObject(IddObjectType::AirflowNetwork_OccupantVentilationControl);

    m_idfObjects.push_back(idfObject);

    idfObject.setString(AirflowNetwork_OccupantVentilationControlFields::Name, modelObject.nameString());
    idfObject.setDouble(AirflowNetwork_OccupantVentilationControlFields::MinimumOpeningTime, modelObject.minimumOpeningTime());
    idfObject.setDouble(AirflowNetwork_OccupantVentilationControlFields::MinimumClosingTime, modelObject.minimumClosingTime());
    idfObject.setString(AirflowNetwork_OccupantVentilationControlFields::ThermalComfortLowTemperatureCurveName,
                        modelObject.thermalComfortLowTemperatureCurve().nameString());
    idfObject.setDouble(AirflowNetwork_OccupantVentilationControlFields::ThermalComfortTemperatureBoundaryPoint,
                        modelObject.thermalComfortTemperatureBoundaryPoint());
    if (modelObject.thermalComfortHighTemperatureCurve()) {
      idfObject.setString(AirflowNetwork_OccupantVentilationControlFields::ThermalComfortHighTemperatureCurveName,
                          modelObject.thermalComfortHighTemperatureCurve()->nameString());
    }
    idfObject.setDouble(AirflowNetwork_OccupantVentilationControlFields::MaximumThresholdforPersonsDissatisfiedPPD,
                        modelObject.maximumPredictedPercentageofDissatisfiedThreshold());
    if (modelObject.occupancyCheck()) {
      idfObject.setString(AirflowNetwork_OccupantVentilationControlFields::OccupancyCheck, "Yes");
    }
    if (modelObject.openingProbabilitySchedule()) {
      idfObject.setString(AirflowNetwork_OccupantVentilationControlFields::OpeningProbabilityScheduleName,
                          modelObject.openingProbabilitySchedule()->nameString());
    }
    if (modelObject.closingProbabilitySchedule()) {
      idfObject.setString(AirflowNetwork_OccupantVentilationControlFields::ClosingProbabilityScheduleName,
                          modelObject.closingProbabilitySchedule()->nameString());
    }

    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
