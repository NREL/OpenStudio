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
#include "../../model/Model.hpp"
#include "../../model/EvaporativeCoolerIndirectResearchSpecial.hpp"
#include "../../model/EvaporativeCoolerIndirectResearchSpecial_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/core/Logger.hpp"
#include <utilities/idd/EvaporativeCooler_Indirect_ResearchSpecial_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateEvaporativeCoolerIndirectResearchSpecial(EvaporativeCoolerIndirectResearchSpecial& modelObject) {
    OptionalString s;
    OptionalDouble d;
    OptionalModelObject temp;

    IdfObject idfObject(IddObjectType::EvaporativeCooler_Indirect_ResearchSpecial);

    m_idfObjects.push_back(idfObject);

    // Name
    s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }

    // AvailabilityScheduleName
    boost::optional<Schedule> sched = modelObject.availabilitySchedule();
    if (sched) {
      boost::optional<IdfObject> _sched = translateAndMapModelObject(sched.get());
      OS_ASSERT(_sched);
      idfObject.setString(EvaporativeCooler_Indirect_ResearchSpecialFields::AvailabilityScheduleName, _sched->name().get());
    }

    // CoolerMaximumEffectiveness
    d = modelObject.coolerMaximumEffectiveness();
    if (d) {
      idfObject.setDouble(EvaporativeCooler_Indirect_ResearchSpecialFields::CoolerWetbulbDesignEffectiveness, d.get());
    }

    // RecirculatingWaterPumpPowerConsumption
    d = modelObject.recirculatingWaterPumpPowerConsumption();
    if (modelObject.isRecirculatingWaterPumpPowerConsumptionAutosized()) {
      idfObject.setString(EvaporativeCooler_Indirect_ResearchSpecialFields::RecirculatingWaterPumpDesignPower, "Autosize");
    } else if (d) {
      idfObject.setDouble(EvaporativeCooler_Indirect_ResearchSpecialFields::RecirculatingWaterPumpDesignPower, d.get());
    }

    // SecondaryFanFlowRate
    if (modelObject.isSecondaryFanFlowRateAutosized()) {
      idfObject.setString(EvaporativeCooler_Indirect_ResearchSpecialFields::SecondaryAirDesignFlowRate, "Autosize");
    } else if ((d = modelObject.secondaryFanFlowRate())) {
      idfObject.setDouble(EvaporativeCooler_Indirect_ResearchSpecialFields::SecondaryAirDesignFlowRate, d.get());
    }

    // SecondaryFanTotalEfficiency
    auto fanEff = modelObject.secondaryFanTotalEfficiency();

    // SecondaryFanDeltaPressure
    auto fanDp = modelObject.secondaryFanDeltaPressure();

    // SecondaryAirFanSizingSpecificPower
    if (fanEff > 0.0) {
      idfObject.setDouble(EvaporativeCooler_Indirect_ResearchSpecialFields::SecondaryAirFanSizingSpecificPower, fanDp / fanEff);
    }

    // PrimaryAirInletNodeName
    if (boost::optional<model::ModelObject> mo = modelObject.inletModelObject()) {
      OS_ASSERT(mo->optionalCast<model::Node>());
      idfObject.setString(EvaporativeCooler_Indirect_ResearchSpecialFields::PrimaryAirInletNodeName, mo->name().get());
    }

    // PrimaryAirOutletNodeName
    if (boost::optional<model::ModelObject> mo = modelObject.outletModelObject()) {
      OS_ASSERT(mo->optionalCast<model::Node>());
      idfObject.setString(EvaporativeCooler_Indirect_ResearchSpecialFields::PrimaryAirOutletNodeName, mo->name().get());
      idfObject.setString(EvaporativeCooler_Indirect_ResearchSpecialFields::SensorNodeName, mo->name().get());
    }

    // DewpointEffectivenessFactor
    d = modelObject.dewpointEffectivenessFactor();
    if (d) {
      idfObject.setDouble(EvaporativeCooler_Indirect_ResearchSpecialFields::DewpointEffectivenessFactor, d.get());
    }

    // DriftLossFraction
    d = modelObject.driftLossFraction();
    if (d) {
      idfObject.setDouble(EvaporativeCooler_Indirect_ResearchSpecialFields::DriftLossFraction, d.get());
    }

    // BlowdownConcentrationRatio
    d = modelObject.blowdownConcentrationRatio();
    if (d) {
      idfObject.setDouble(EvaporativeCooler_Indirect_ResearchSpecialFields::BlowdownConcentrationRatio, d.get());
    }

    // ReliefAirInletNodeName
    if (boost::optional<model::Node> node =
          modelObject.getImpl<model::detail::EvaporativeCoolerIndirectResearchSpecial_Impl>()->reliefAirInletNode()) {
      idfObject.setString(EvaporativeCooler_Indirect_ResearchSpecialFields::ReliefAirInletNodeName, node->name().get());
    }

    // Secondary Air Inlet Node Name
    {
      IdfObject inletNode(openstudio::IddObjectType::OutdoorAir_NodeList);
      auto inletNodeName = modelObject.name().get() + " Secondary Air Inlet";
      inletNode.setString(0, inletNodeName);
      m_idfObjects.push_back(inletNode);
      idfObject.setString(EvaporativeCooler_Indirect_ResearchSpecialFields::SecondaryAirInletNodeName, inletNodeName);
    }

    // Secondary Air Outlet Node Name
    {
      auto outletNodeName = modelObject.name().get() + " Secondary Air Outlet";
      idfObject.setString(EvaporativeCooler_Indirect_ResearchSpecialFields::SecondaryAirOutletNodeName, outletNodeName);
    }

    // WetbulbEffectivenessFlowRatioModifierCurveName
    if (auto curve = modelObject.wetbulbEffectivenessFlowRatioModifierCurve()) {
      auto _curve = translateAndMapModelObject(curve.get());
      OS_ASSERT(_curve);
      idfObject.setString(EvaporativeCooler_Indirect_ResearchSpecialFields::WetbulbEffectivenessFlowRatioModifierCurveName, _curve->name().get());
    }

    // CoolerDrybulbDesignEffectiveness
    d = modelObject.coolerDrybulbDesignEffectiveness();
    if (d) {
      idfObject.setDouble(EvaporativeCooler_Indirect_ResearchSpecialFields::CoolerDrybulbDesignEffectiveness, d.get());
    }

    // DrybulbEffectivenessFlowRatioModifierCurveName
    if (auto curve = modelObject.drybulbEffectivenessFlowRatioModifierCurve()) {
      auto _curve = translateAndMapModelObject(curve.get());
      OS_ASSERT(_curve);
      idfObject.setString(EvaporativeCooler_Indirect_ResearchSpecialFields::DrybulbEffectivenessFlowRatioModifierCurveName, _curve->name().get());
    }

    // WaterPumpPowerSizingFactor
    d = modelObject.waterPumpPowerSizingFactor();
    if (d) {
      idfObject.setDouble(EvaporativeCooler_Indirect_ResearchSpecialFields::WaterPumpPowerSizingFactor, d.get());
    }

    // WaterPumpPowerModifierCurveName
    if (auto curve = modelObject.waterPumpPowerModifierCurve()) {
      auto _curve = translateAndMapModelObject(curve.get());
      OS_ASSERT(_curve);
      idfObject.setString(EvaporativeCooler_Indirect_ResearchSpecialFields::WaterPumpPowerModifierCurveName, _curve->name().get());
    }

    // SecondaryAirFlowScalingFactor
    d = modelObject.secondaryAirFlowScalingFactor();
    if (d) {
      idfObject.setDouble(EvaporativeCooler_Indirect_ResearchSpecialFields::SecondaryAirFlowScalingFactor, d.get());
    }

    // SecondaryAirFanDesignPower
    d = modelObject.secondaryAirFanDesignPower();
    if (modelObject.isSecondaryAirFanDesignPowerAutosized()) {
      idfObject.setString(EvaporativeCooler_Indirect_ResearchSpecialFields::SecondaryAirFanDesignPower, "Autosize");
    } else if (d) {
      idfObject.setDouble(EvaporativeCooler_Indirect_ResearchSpecialFields::SecondaryAirFanDesignPower, d.get());
    }

    // SecondaryAirFanPowerModifierCurveName
    if (auto curve = modelObject.secondaryAirFanPowerModifierCurve()) {
      auto _curve = translateAndMapModelObject(curve.get());
      OS_ASSERT(_curve);
      idfObject.setString(EvaporativeCooler_Indirect_ResearchSpecialFields::SecondaryAirFanPowerModifierCurveName, _curve->name().get());
    }

    // PrimaryDesignAirFlowRate
    d = modelObject.primaryDesignAirFlowRate();
    if (modelObject.isPrimaryDesignAirFlowRateAutosized()) {
      idfObject.setString(EvaporativeCooler_Indirect_ResearchSpecialFields::PrimaryAirDesignFlowRate, "Autosize");
    } else if (d) {
      idfObject.setDouble(EvaporativeCooler_Indirect_ResearchSpecialFields::PrimaryAirDesignFlowRate, d.get());
    }

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
