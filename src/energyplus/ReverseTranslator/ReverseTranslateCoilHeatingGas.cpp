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

#include "../../model/CoilHeatingGas.hpp"
#include "../../model/CoilHeatingGas_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/Coil_Heating_Fuel_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateCoilHeatingGas(const WorkspaceObject& workspaceObject) {
    OptionalModelObject result, temp;

    OptionalSchedule schedule;

    //get the Schedule
    OptionalWorkspaceObject owo = workspaceObject.getTarget(Coil_Heating_FuelFields::AvailabilityScheduleName);
    if (!owo) {
      LOG(Error, "Error importing object: " << workspaceObject.briefDescription() << " Can't find Schedule.");
      return result;
    }
    temp = translateAndMapWorkspaceObject(*owo);
    if (temp) {
      schedule = temp->optionalCast<Schedule>();
    }

    if (!schedule) {
      LOG(Error, "Error importing object: " << workspaceObject.briefDescription()
                                            << "Failed to convert iddObject (schedule) into ModelObject. Maybe it does not exist in model yet");
      return result;
    }

    try {

      openstudio::model::CoilHeatingGas coil(m_model, *schedule);
      OptionalString optS = workspaceObject.name();
      if (optS) {
        coil.setName(*optS);
      }
      OptionalDouble d;
      d = workspaceObject.getDouble(openstudio::Coil_Heating_FuelFields::BurnerEfficiency);
      if (d) {
        coil.setGasBurnerEfficiency(*d);
      }
      d = workspaceObject.getDouble(openstudio::Coil_Heating_FuelFields::NominalCapacity);
      if (d) {
        coil.setNominalCapacity(*d);
      }
      //skip inlet and outlet node names. That should be done FOR us by the AirLoop Translator.
      d = workspaceObject.getDouble(openstudio::Coil_Heating_FuelFields::BurnerEfficiency);
      if (d) {
        coil.setGasBurnerEfficiency(*d);
      }

      d = workspaceObject.getDouble(openstudio::Coil_Heating_FuelFields::NominalCapacity);
      if (d) {
        coil.setNominalCapacity(*d);
      }

      d = workspaceObject.getDouble(openstudio::Coil_Heating_FuelFields::ParasiticElectricLoad);
      if (d) {
        coil.setParasiticElectricLoad(*d);
      }
      d = workspaceObject.getDouble(openstudio::Coil_Heating_FuelFields::ParasiticFuelLoad);
      if (d) {
        coil.setParasiticGasLoad(*d);
      }
      result = coil;
    } catch (std::exception& e) {
      LOG(Error, "Unable to reverse translate " << workspaceObject.briefDescription() << ", because " << e.what() << ".");
      return boost::none;
    }

    return result;
  }

}  // namespace energyplus

}  // namespace openstudio
