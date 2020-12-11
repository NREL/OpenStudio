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
      idfObject.setDouble(ElectricLoadCenter_Storage_ConverterFields::SimpleFixedEfficiency, *optD);
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
