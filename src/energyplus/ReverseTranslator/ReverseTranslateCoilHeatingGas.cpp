/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
    OptionalModelObject result;
    OptionalModelObject temp;

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

      d = workspaceObject.getDouble(openstudio::Coil_Heating_FuelFields::OnCycleParasiticElectricLoad);
      if (d) {
        coil.setOnCycleParasiticElectricLoad(*d);
      }
      d = workspaceObject.getDouble(openstudio::Coil_Heating_FuelFields::OffCycleParasiticFuelLoad);
      if (d) {
        coil.setOffCycleParasiticGasLoad(*d);
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
