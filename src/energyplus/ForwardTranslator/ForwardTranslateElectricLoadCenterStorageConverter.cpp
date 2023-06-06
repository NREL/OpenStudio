/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
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

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateElectricLoadCenterStorageConverter(model::ElectricLoadCenterStorageConverter& modelObject) {

    boost::optional<double> optD;

    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::ElectricLoadCenter_Storage_Converter, modelObject);

    if (modelObject.name()) {
      idfObject.setString(ElectricLoadCenter_Storage_ConverterFields::Name, modelObject.name().get());
    }

    // Availability Schedule, defaults to model.alwaysOnDiscrete
    Schedule sched = modelObject.availabilitySchedule();
    boost::optional<IdfObject> _sched = translateAndMapModelObject(sched);
    if (_sched) {
      idfObject.setString(ElectricLoadCenter_Storage_ConverterFields::AvailabilityScheduleName, _sched->name().get());
    }

    // PowerConversionEfficiencyMethod, string
    idfObject.setString(ElectricLoadCenter_Storage_ConverterFields::PowerConversionEfficiencyMethod, modelObject.powerConversionEfficiencyMethod());

    // SimpleFixedEfficiency, optD
    optD = modelObject.simpleFixedEfficiency();
    if (optD) {
      idfObject.setDouble(ElectricLoadCenter_Storage_ConverterFields::SimpleFixedEfficiency, *optD);
    }

    // designMaximumContinuousInputPower, optD
    optD = modelObject.designMaximumContinuousInputPower();
    if (optD) {
      idfObject.setDouble(ElectricLoadCenter_Storage_ConverterFields::DesignMaximumContinuousInputPower, *optD);
    }

    // efficiencyFunctionofPowerCurveName, optCurve
    boost::optional<Curve> effFPower = modelObject.efficiencyFunctionofPowerCurve();
    if (effFPower) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(effFPower.get())) {
        idfObject.setString(ElectricLoadCenter_Storage_ConverterFields::EfficiencyFunctionofPowerCurveName, _curve->name().get());
      }
    }

    // ancillaryPowerConsumedInStandby, double
    {
      double value = modelObject.ancillaryPowerConsumedInStandby();
      idfObject.setDouble(ElectricLoadCenter_Storage_ConverterFields::AncillaryPowerConsumedInStandby, value);
    }

    // ZoneName
    if (modelObject.thermalZone() && modelObject.thermalZone().get().name()) {
      idfObject.setString(ElectricLoadCenter_Storage_ConverterFields::ZoneName, modelObject.thermalZone().get().name().get());
    }

    // Radiative Fraction, defaults (double)
    {
      double value = modelObject.radiativeFraction();
      idfObject.setDouble(ElectricLoadCenter_Storage_ConverterFields::RadiativeFraction, value);
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
