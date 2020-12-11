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
#include "../../model/ElectricLoadCenterTransformer.hpp"
#include "../../model/ElectricLoadCenterTransformer_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Schedule.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/ElectricLoadCenter_Transformer_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateElectricLoadCenterTransformer(model::ElectricLoadCenterTransformer& modelObject) {
    boost::optional<double> optD;
    std::string s;
    boost::optional<std::string> optS;
    boost::optional<Schedule> schedule;
    boost::optional<ThermalZone> zone;

    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::ElectricLoadCenter_Transformer, modelObject);

    schedule = modelObject.availabilitySchedule();
    if (schedule) {
      boost::optional<IdfObject> scheduleIdf = translateAndMapModelObject(*schedule);
      if (scheduleIdf) {
        idfObject.setString(ElectricLoadCenter_TransformerFields::AvailabilityScheduleName, scheduleIdf->name().get());
      }
    }

    idfObject.setString(ElectricLoadCenter_TransformerFields::TransformerUsage, modelObject.transformerUsage());

    zone = modelObject.zone();
    if (zone) {
      idfObject.setString(ElectricLoadCenter_TransformerFields::ZoneName, zone->name().get());
    }

    idfObject.setDouble(ElectricLoadCenter_TransformerFields::RadiativeFraction, modelObject.radiativeFraction());

    optD = modelObject.ratedCapacity();
    if (optD) {
      idfObject.setDouble(ElectricLoadCenter_TransformerFields::RatedCapacity, optD.get());
    }

    idfObject.setString(ElectricLoadCenter_TransformerFields::Phase, modelObject.phase());

    idfObject.setString(ElectricLoadCenter_TransformerFields::ConductorMaterial, modelObject.conductorMaterial());

    idfObject.setDouble(ElectricLoadCenter_TransformerFields::FullLoadTemperatureRise, modelObject.fullLoadTemperatureRise());

    idfObject.setDouble(ElectricLoadCenter_TransformerFields::FractionofEddyCurrentLosses, modelObject.fractionofEddyCurrentLosses());

    idfObject.setString(ElectricLoadCenter_TransformerFields::PerformanceInputMethod, modelObject.performanceInputMethod());

    optD = modelObject.ratedNoLoadLoss();
    if (optD) {
      idfObject.setDouble(ElectricLoadCenter_TransformerFields::RatedNoLoadLoss, optD.get());
    }

    optD = modelObject.ratedLoadLoss();
    if (optD) {
      idfObject.setDouble(ElectricLoadCenter_TransformerFields::RatedLoadLoss, optD.get());
    }

    idfObject.setDouble(ElectricLoadCenter_TransformerFields::NameplateEfficiency, modelObject.nameplateEfficiency());

    idfObject.setDouble(ElectricLoadCenter_TransformerFields::PerUnitLoadforNameplateEfficiency, modelObject.perUnitLoadforNameplateEfficiency());

    idfObject.setDouble(ElectricLoadCenter_TransformerFields::ReferenceTemperatureforNameplateEfficiency,
                        modelObject.referenceTemperatureforNameplateEfficiency());

    optD = modelObject.perUnitLoadforMaximumEfficiency();
    if (optD) {
      idfObject.setDouble(ElectricLoadCenter_TransformerFields::PerUnitLoadforMaximumEfficiency, optD.get());
    }
    if (modelObject.considerTransformerLossforUtilityCost()) {
      s = "Yes";
    } else {
      s = "No";
    }
    idfObject.setString(ElectricLoadCenter_TransformerFields::ConsiderTransformerLossforUtilityCost, s);

    //meters
    std::vector<std::string> meters = modelObject.meters();
    if (!meters.empty()) {
      for (auto meter : meters) {
        auto eg = idfObject.pushExtensibleGroup();
        eg.setString(ElectricLoadCenter_TransformerExtensibleFields::MeterName, meter);
      }
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
