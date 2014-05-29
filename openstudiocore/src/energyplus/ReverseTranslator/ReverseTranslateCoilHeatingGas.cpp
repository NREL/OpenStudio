/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/CoilHeatingGas.hpp"
#include "../../model/CoilHeatingGas_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/Coil_Heating_Gas_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateCoilHeatingGas( const WorkspaceObject & workspaceObject )
{
  OptionalModelObject result,temp;

  OptionalSchedule schedule;

 //get the Schedule
  OptionalWorkspaceObject owo = workspaceObject.getTarget(Coil_Heating_GasFields::AvailabilityScheduleName);
  if(!owo)
  {
    LOG(Error, "Error importing object: "
             << workspaceObject.briefDescription()
             << " Can't find Schedule.");
    return result;
  }
  temp = translateAndMapWorkspaceObject(*owo);
  if(temp)
  {
    schedule=temp->optionalCast<Schedule>();
  }

  if( !schedule  )
  {
    LOG(Error, "Error importing object: "
             << workspaceObject.briefDescription()
             <<"Failed to convert iddObject (schedule) into ModelObject. Maybe it does not exist in model yet");
    return result;
  }

  try {

    openstudio::model::CoilHeatingGas coil( m_model,
                                            *schedule );
    OptionalString optS = workspaceObject.name();
    if(optS)
    {
      coil.setName(*optS);
    }
    OptionalDouble d;
    d = workspaceObject.getDouble(openstudio::Coil_Heating_GasFields::GasBurnerEfficiency);
    if(d)
    {
      coil.setGasBurnerEfficiency(*d);
    }
    d = workspaceObject.getDouble(openstudio::Coil_Heating_GasFields::NominalCapacity);
    if(d)
    {
      coil.setNominalCapacity(*d);
    }
    //skip inlet and outlet node names. That should be done FOR us by the AirLoop Translator.
    d = workspaceObject.getDouble(openstudio::Coil_Heating_GasFields::GasBurnerEfficiency);
    if(d)
    {
      coil.setGasBurnerEfficiency(*d);
    }

    d = workspaceObject.getDouble(openstudio::Coil_Heating_GasFields::NominalCapacity);
    if(d)
    {
      coil.setNominalCapacity(*d);
    }

    d = workspaceObject.getDouble(openstudio::Coil_Heating_GasFields::ParasiticElectricLoad);
    if(d)
    {
      coil.setParasiticElectricLoad(*d);
    }
    d = workspaceObject.getDouble(openstudio::Coil_Heating_GasFields::ParasiticGasLoad);
    if(d)
    {
      coil.setParasiticGasLoad(*d);
    }
    result = coil;
  }
  catch (std::exception& e) {
    LOG(Error,"Unable to reverse translate " << workspaceObject.briefDescription() << ", because "
        << e.what() << ".");
    return boost::none;
  }

  return result;
}

} // energyplus

} // openstudio

