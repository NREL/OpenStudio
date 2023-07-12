/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/ElectricLoadCenterStorageConverter.hpp"
#include "../../model/ElectricLoadCenterStorageConverter_Impl.hpp"

#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
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

    OptionalModelObject omo;
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
      if (boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(owo.get())) {
        // Zone is translated, and a Space is returned instead
        if (boost::optional<Space> space_ = mo->optionalCast<Space>()) {
          if (auto z_ = space_->thermalZone()) {
            elcConv.setThermalZone(z_.get());
          }
        }
      }
    }

    // Radiative Fraction, defaults (double)
    optD = workspaceObject.getDouble(ElectricLoadCenter_Storage_ConverterFields::RadiativeFraction);
    if (optD) {
      elcConv.setRadiativeFraction(*optD);
    }

    return elcConv;
  }

}  // namespace energyplus

}  // namespace openstudio
