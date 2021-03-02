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

#include "../ReverseTranslator.hpp"
#include "../../model/RefrigerationCase.hpp"
#include "../../model/RefrigerationCase_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/CurveCubic.hpp"
#include "../../model/CurveCubic_Impl.hpp"
#include <utilities/idd/Refrigeration_Case_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateRefrigerationCase(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::Refrigeration_Case) {
      LOG(Error, "WorkspaceObject is not IddObjectType: Refrigeration_Case");
      return boost::none;
    }

    boost::optional<WorkspaceObject> wo = workspaceObject.getTarget(Refrigeration_CaseFields::CaseDefrostScheduleName);
    boost::optional<Schedule> caseDefrostSchedule;
    boost::optional<RefrigerationCase> refrigerationCase;

    if (wo) {
      if (boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(wo.get())) {
        if (!(caseDefrostSchedule = mo->optionalCast<Schedule>())) {
          LOG(Error, workspaceObject.briefDescription() << " does not have an associated case defrost schedule");

          return boost::none;
        }
      }
    }

    if (caseDefrostSchedule) {
      refrigerationCase = RefrigerationCase(m_model, caseDefrostSchedule.get());
    }

    if (refrigerationCase) {
      boost::optional<double> value;
      boost::optional<std::string> s = workspaceObject.getString(Refrigeration_CaseFields::Name);

      // Name
      if (s) {
        refrigerationCase->setName(s.get());
      }
      // AvailabilityScheduleName
      if ((wo = workspaceObject.getTarget(Refrigeration_CaseFields::AvailabilityScheduleName))) {
        if (boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(wo.get())) {
          if (boost::optional<Schedule> schedule = mo->optionalCast<Schedule>()) {
            refrigerationCase->setAvailabilitySchedule(schedule.get());
          }
        }
      }
      // ZoneName
      if ((wo = workspaceObject.getTarget(Refrigeration_CaseFields::ZoneName))) {
        if (boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(wo.get())) {
          if (boost::optional<ThermalZone> thermalZone = mo->optionalCast<ThermalZone>()) {
            refrigerationCase->setThermalZone(thermalZone.get());
          }
        }
      }
      // RatedAmbientTemperature
      value = workspaceObject.getDouble(Refrigeration_CaseFields::RatedAmbientTemperature);
      if (value) {
        refrigerationCase->setRatedAmbientTemperature(value.get());
      }
      // RatedAmbientRelativeHumidity
      value = workspaceObject.getDouble(Refrigeration_CaseFields::RatedAmbientRelativeHumidity);
      if (value) {
        refrigerationCase->setRatedAmbientRelativeHumidity(value.get());
      }
      // RatedTotalCoolingCapacityperUnitLength
      value = workspaceObject.getDouble(Refrigeration_CaseFields::RatedTotalCoolingCapacityperUnitLength);
      if (value) {
        refrigerationCase->setRatedTotalCoolingCapacityperUnitLength(value.get());
      }
      // RatedLatentHeatRatio
      value = workspaceObject.getDouble(Refrigeration_CaseFields::RatedLatentHeatRatio);
      if (value) {
        refrigerationCase->setRatedLatentHeatRatio(value.get());
      }
      // RatedRuntimeFraction
      value = workspaceObject.getDouble(Refrigeration_CaseFields::RatedRuntimeFraction);
      if (value) {
        refrigerationCase->setRatedRuntimeFraction(value.get());
      }
      // CaseLength
      value = workspaceObject.getDouble(Refrigeration_CaseFields::CaseLength);
      if (value) {
        refrigerationCase->setCaseLength(value.get());
      }
      // CaseOperatingTemperature
      value = workspaceObject.getDouble(Refrigeration_CaseFields::CaseOperatingTemperature);
      if (value) {
        refrigerationCase->setCaseOperatingTemperature(value.get());
      }
      // LatentCaseCreditCurveType
      s = workspaceObject.getString(Refrigeration_CaseFields::LatentCaseCreditCurveType);
      if (s) {
        refrigerationCase->setLatentCaseCreditCurveType(s.get());
      }
      // LatentCaseCreditCurveName
      if ((wo = workspaceObject.getTarget(Refrigeration_CaseFields::LatentCaseCreditCurveName))) {
        if (boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(wo.get())) {
          if (boost::optional<CurveCubic> curve = mo->optionalCast<CurveCubic>()) {
            refrigerationCase->setLatentCaseCreditCurve(curve.get());
          }
        }
      }
      // StandardCaseFanPowerperUnitLength
      value = workspaceObject.getDouble(Refrigeration_CaseFields::StandardCaseFanPowerperUnitLength);
      if (value) {
        refrigerationCase->setStandardCaseFanPowerperUnitLength(value.get());
      }
      // OperatingCaseFanPowerperUnitLength
      value = workspaceObject.getDouble(Refrigeration_CaseFields::OperatingCaseFanPowerperUnitLength);
      if (value) {
        refrigerationCase->setOperatingCaseFanPowerperUnitLength(value.get());
      }
      // StandardCaseLightingPowerperUnitLength
      value = workspaceObject.getDouble(Refrigeration_CaseFields::StandardCaseLightingPowerperUnitLength);
      if (value) {
        refrigerationCase->setStandardCaseLightingPowerperUnitLength(value.get());
      }
      // InstalledCaseLightingPowerperUnitLength
      value = workspaceObject.getDouble(Refrigeration_CaseFields::InstalledCaseLightingPowerperUnitLength);
      if (value) {
        refrigerationCase->setInstalledCaseLightingPowerperUnitLength(value.get());
      }
      // CaseLightingScheduleName
      if ((wo = workspaceObject.getTarget(Refrigeration_CaseFields::CaseLightingScheduleName))) {
        if (boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(wo.get())) {
          if (boost::optional<Schedule> schedule = mo->optionalCast<Schedule>()) {
            refrigerationCase->setCaseLightingSchedule(schedule.get());
          }
        }
      }
      // FractionofLightingEnergytoCase
      value = workspaceObject.getDouble(Refrigeration_CaseFields::FractionofLightingEnergytoCase);
      if (value) {
        refrigerationCase->setFractionofLightingEnergytoCase(value.get());
      }
      // CaseAntiSweatHeaterPowerperUnitLength
      value = workspaceObject.getDouble(Refrigeration_CaseFields::CaseAntiSweatHeaterPowerperUnitLength);
      if (value) {
        refrigerationCase->setCaseAntiSweatHeaterPowerperUnitLength(value.get());
      }
      // MinimumAntiSweatHeaterPowerperUnitLength
      value = workspaceObject.getDouble(Refrigeration_CaseFields::MinimumAntiSweatHeaterPowerperUnitLength);
      if (value) {
        refrigerationCase->setMinimumAntiSweatHeaterPowerperUnitLength(value.get());
      }
      // AntiSweatHeaterControlType
      s = workspaceObject.getString(Refrigeration_CaseFields::AntiSweatHeaterControlType);
      if (s) {
        refrigerationCase->setAntiSweatHeaterControlType(s.get());
      }
      // HumidityatZeroAntiSweatHeaterEnergy
      value = workspaceObject.getDouble(Refrigeration_CaseFields::HumidityatZeroAntiSweatHeaterEnergy);
      if (value) {
        refrigerationCase->setHumidityatZeroAntiSweatHeaterEnergy(value.get());
      }
      // CaseHeight
      value = workspaceObject.getDouble(Refrigeration_CaseFields::CaseHeight);
      if (value) {
        refrigerationCase->setCaseHeight(value.get());
      }
      // FractionofAntiSweatHeaterEnergytoCase
      value = workspaceObject.getDouble(Refrigeration_CaseFields::FractionofAntiSweatHeaterEnergytoCase);
      if (value) {
        refrigerationCase->setFractionofAntiSweatHeaterEnergytoCase(value.get());
      }
      // CaseDefrostPowerperUnitLength
      value = workspaceObject.getDouble(Refrigeration_CaseFields::CaseDefrostPowerperUnitLength);
      if (value) {
        refrigerationCase->setCaseDefrostPowerperUnitLength(value.get());
      }
      // CaseDefrostType
      s = workspaceObject.getString(Refrigeration_CaseFields::CaseDefrostType);
      if (s) {
        refrigerationCase->setCaseDefrostType(s.get());
      }
      // CaseDefrostScheduleName
      if ((wo = workspaceObject.getTarget(Refrigeration_CaseFields::CaseDefrostScheduleName))) {
        if (boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(wo.get())) {
          if (boost::optional<Schedule> schedule = mo->optionalCast<Schedule>()) {
            refrigerationCase->setCaseDefrostSchedule(schedule.get());
          }
        }
      }
      // CaseDefrostDripDownScheduleName
      if ((wo = workspaceObject.getTarget(Refrigeration_CaseFields::CaseDefrostDripDownScheduleName))) {
        if (boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(wo.get())) {
          if (boost::optional<Schedule> schedule = mo->optionalCast<Schedule>()) {
            refrigerationCase->setCaseDefrostDripDownSchedule(schedule.get());
          }
        }
      }
      // DefrostEnergyCorrectionCurveType
      s = workspaceObject.getString(Refrigeration_CaseFields::DefrostEnergyCorrectionCurveType);
      if (s) {
        refrigerationCase->setDefrostEnergyCorrectionCurveType(s.get());
      }
      // DefrostEnergyCorrectionCurveName
      if ((wo = workspaceObject.getTarget(Refrigeration_CaseFields::DefrostEnergyCorrectionCurveName))) {
        if (boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(wo.get())) {
          if (boost::optional<CurveCubic> curve = mo->optionalCast<CurveCubic>()) {
            refrigerationCase->setDefrostEnergyCorrectionCurve(curve.get());
          }
        }
      }
      // UnderCaseHVACReturnAirFraction
      value = workspaceObject.getDouble(Refrigeration_CaseFields::UnderCaseHVACReturnAirFraction);
      if (value) {
        refrigerationCase->setUnderCaseHVACReturnAirFraction(value.get());
      }
      // RefrigeratedCaseRestockingScheduleName
      if ((wo = workspaceObject.getTarget(Refrigeration_CaseFields::RefrigeratedCaseRestockingScheduleName))) {
        if (boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(wo.get())) {
          if (boost::optional<Schedule> schedule = mo->optionalCast<Schedule>()) {
            refrigerationCase->setRefrigeratedCaseRestockingSchedule(schedule.get());
          }
        }
      }
      // CaseCreditFractionScheduleName
      if ((wo = workspaceObject.getTarget(Refrigeration_CaseFields::CaseCreditFractionScheduleName))) {
        if (boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(wo.get())) {
          if (boost::optional<Schedule> schedule = mo->optionalCast<Schedule>()) {
            refrigerationCase->setCaseCreditFractionSchedule(schedule.get());
          }
        }
      }
      // DesignEvaporatorTemperatureorBrineInletTemperature
      value = workspaceObject.getDouble(Refrigeration_CaseFields::DesignEvaporatorTemperatureorBrineInletTemperature);
      if (value) {
        refrigerationCase->setDesignEvaporatorTemperatureorBrineInletTemperature(value.get());
      }
      // AverageRefrigerantChargeInventory
      value = workspaceObject.getDouble(Refrigeration_CaseFields::AverageRefrigerantChargeInventory);
      if (value) {
        refrigerationCase->setAverageRefrigerantChargeInventory(value.get());
      }

      return refrigerationCase.get();
    } else {
      LOG(Error, "Unknown error translating " << workspaceObject.briefDescription());

      return boost::none;
    }
  }

}  // namespace energyplus

}  // namespace openstudio
