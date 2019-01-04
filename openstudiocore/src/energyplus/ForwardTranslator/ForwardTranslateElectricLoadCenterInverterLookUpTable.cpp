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
#include "../../model/ElectricLoadCenterInverterLookUpTable.hpp"
#include "../../model/ElectricLoadCenterInverterLookUpTable_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"

#include <utilities/idd/ElectricLoadCenter_Inverter_LookUpTable_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateElectricLoadCenterInverterLookUpTable(model::ElectricLoadCenterInverterLookUpTable & modelObject)
{
  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::ElectricLoadCenter_Inverter_LookUpTable, modelObject);

  if (modelObject.name()) {
    idfObject.setString(ElectricLoadCenter_Inverter_LookUpTableFields::Name, modelObject.name().get());
  }

  if (modelObject.availabilitySchedule() && modelObject.availabilitySchedule().get().name()) {
    idfObject.setString(ElectricLoadCenter_Inverter_LookUpTableFields::AvailabilityScheduleName, modelObject.availabilitySchedule().get().name().get());
  }

  if (modelObject.thermalZone() && modelObject.thermalZone().get().name()) {
    idfObject.setString(ElectricLoadCenter_Inverter_LookUpTableFields::ZoneName, modelObject.thermalZone().get().name().get());
  }

  if (modelObject.radiativeFraction()) {
    idfObject.setDouble(ElectricLoadCenter_Inverter_LookUpTableFields::RadiativeFraction, modelObject.radiativeFraction().get());
  }

  if (modelObject.ratedMaximumContinuousOutputPower()) {
    idfObject.setDouble(ElectricLoadCenter_Inverter_LookUpTableFields::RatedMaximumContinuousOutputPower, modelObject.ratedMaximumContinuousOutputPower().get());
  }

  if (modelObject.nightTareLossPower()) {
    idfObject.setDouble(ElectricLoadCenter_Inverter_LookUpTableFields::NightTareLossPower, modelObject.nightTareLossPower().get());
  }

  if (modelObject.nominalVoltageInput()) {
    idfObject.setDouble(ElectricLoadCenter_Inverter_LookUpTableFields::NominalVoltageInput, modelObject.nominalVoltageInput().get());
  }

  if (modelObject.efficiencyAt10PowerAndNominalVoltage()) {
    idfObject.setDouble(ElectricLoadCenter_Inverter_LookUpTableFields::Efficiencyat10_PowerandNominalVoltage, modelObject.efficiencyAt10PowerAndNominalVoltage().get());
  }

  if (modelObject.efficiencyAt20PowerAndNominalVoltage()) {
    idfObject.setDouble(ElectricLoadCenter_Inverter_LookUpTableFields::Efficiencyat20_PowerandNominalVoltage, modelObject.efficiencyAt20PowerAndNominalVoltage().get());
  }

  if (modelObject.efficiencyAt30PowerAndNominalVoltage()) {
    idfObject.setDouble(ElectricLoadCenter_Inverter_LookUpTableFields::Efficiencyat30_PowerandNominalVoltage, modelObject.efficiencyAt30PowerAndNominalVoltage().get());
  }

  if (modelObject.efficiencyAt50PowerAndNominalVoltage()) {
    idfObject.setDouble(ElectricLoadCenter_Inverter_LookUpTableFields::Efficiencyat50_PowerandNominalVoltage, modelObject.efficiencyAt50PowerAndNominalVoltage().get());
  }

  if (modelObject.efficiencyAt75PowerAndNominalVoltage()) {
    idfObject.setDouble(ElectricLoadCenter_Inverter_LookUpTableFields::Efficiencyat75_PowerandNominalVoltage, modelObject.efficiencyAt75PowerAndNominalVoltage().get());
  }

  if (modelObject.efficiencyAt100PowerAndNominalVoltage()) {
    idfObject.setDouble(ElectricLoadCenter_Inverter_LookUpTableFields::Efficiencyat100_PowerandNominalVoltage, modelObject.efficiencyAt100PowerAndNominalVoltage().get());
  }

  return idfObject;
}

} // energyplus

} // openstudio

