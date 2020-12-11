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

#include "../../model/ElectricLoadCenterStorageConverter.hpp"
#include "../../model/ElectricLoadCenterStorageConverter_Impl.hpp"

#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"

#include <utilities/idd/ElectricLoadCenter_Storage_Converter_FieldEnums.hxx>

#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/core/Compare.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateElectricLoadCenterStorageConverter(const WorkspaceObject& workspaceObject) {

    OptionalModelObject result, omo;
    OptionalDouble optD;
    boost::optional<WorkspaceObject> owo;
    OptionalString optS;
    bool isSimpleFixed = true;

    openstudio::model::ElectricLoadCenterStorageConverter elcConv(m_model);

    // Name
    optS = workspaceObject.name();
    if (optS) {
      elcConv.setName(*optS);
    }

    // AvailabilityScheduleName
    if ((owo = workspaceObject.getTarget(ElectricLoadCenter_Storage_ConverterFields::AvailabilityScheduleName))) {
      if ((omo = translateAndMapWorkspaceObject(*owo))) {
        if (boost::optional<Schedule> schedule = omo->optionalCast<Schedule>()) {
          elcConv.setAvailabilitySchedule(schedule.get());
        }
      }
    }

    // PowerConversionEfficiencyMethod, string
    optS = workspaceObject.getString(ElectricLoadCenter_Storage_ConverterFields::PowerConversionEfficiencyMethod);
    if (optS) {
      if (istringEqual(*optS, "FunctionOfPower")) {
        isSimpleFixed = false;
      }
    }

    if (isSimpleFixed) {
      // SimpleFixedEfficiency, optD
      optD = workspaceObject.getDouble(ElectricLoadCenter_Storage_ConverterFields::SimpleFixedEfficiency);
      if (optD) {
        elcConv.setSimpleFixedEfficiency(*optD);
      }
    } else {
      // designMaximumContinuousInputPower, optD
      optD = workspaceObject.getDouble(ElectricLoadCenter_Storage_ConverterFields::DesignMaximumContinuousInputPower);
      if (optD) {
        elcConv.setDesignMaximumContinuousInputPower(*optD);
      }

      // efficiencyFunctionofPowerCurveName, optCurve
      if ((owo = workspaceObject.getTarget(ElectricLoadCenter_Storage_ConverterFields::EfficiencyFunctionofPowerCurveName))) {
        if ((omo = translateAndMapWorkspaceObject(*owo))) {
          // Should technically make sure the curve is the right type, but in this case it's UnivariateCurves, lots of possibilities.
          if (boost::optional<Curve> effFPower = omo->optionalCast<Curve>()) {
            elcConv.setEfficiencyFunctionofPowerCurve(effFPower.get());
          }
        }
      }
    }

    // AncillaryPowerConsumedInStandby, defaults (double)
    optD = workspaceObject.getDouble(ElectricLoadCenter_Storage_ConverterFields::AncillaryPowerConsumedInStandby);
    if (optD) {
      elcConv.setAncillaryPowerConsumedInStandby(*optD);
    }

    // ZoneName
    if ((owo = workspaceObject.getTarget(ElectricLoadCenter_Storage_ConverterFields::ZoneName))) {
      if ((omo = translateAndMapWorkspaceObject(owo.get()))) {
        if (boost::optional<ThermalZone> thermalZone = omo->optionalCast<ThermalZone>()) {
          elcConv.setThermalZone(thermalZone.get());
        }
      }
    }

    // Radiative Fraction, defaults (double)
    optD = workspaceObject.getDouble(ElectricLoadCenter_Storage_ConverterFields::RadiativeFraction);
    if (optD) {
      elcConv.setRadiativeFraction(*optD);
    }

    result = elcConv;
    return result;
  }

}  // namespace energyplus

}  // namespace openstudio
