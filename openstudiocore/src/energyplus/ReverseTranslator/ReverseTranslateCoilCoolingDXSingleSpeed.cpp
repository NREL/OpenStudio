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

#include <energyplus/ReverseTranslator.hpp>

#include <model/CoilCoolingDXSingleSpeed.hpp>
#include <model/CoilCoolingDXSingleSpeed_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/Curve.hpp>
#include <model/Curve_Impl.hpp>

#include <utilities/idd/Coil_Cooling_DX_SingleSpeed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateCoilCoolingDXSingleSpeed( const WorkspaceObject & workspaceObject )
{
OptionalModelObject result,temp;
  OptionalSchedule schedule;

  //get the Schedule
  OptionalWorkspaceObject owo = workspaceObject.getTarget(Coil_Cooling_DX_SingleSpeedFields::AvailabilityScheduleName);
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
    LOG(Error,"Error importing object: "
             << workspaceObject.briefDescription()
             <<"Failed to convert iddObject (schedule) into ModelObject. Maybe it does not exist in model yet");
    return result;
  }

  //collect the curves
  owo = workspaceObject.getTarget(Coil_Cooling_DX_SingleSpeedFields::TotalCoolingCapacityFunctionofTemperatureCurveName);
  if(!owo)
  {
    LOG(Error, "Error importing object: "
             << workspaceObject.briefDescription()
             << " Can't find TotalCoolingCapacityFunctionOfTemperatureCurve.");
    return result;
  }
  if( owo->numSources() > 1 )
  {
    owo = owo->workspace().addObject(owo.get());
  }

  temp= translateAndMapWorkspaceObject( *owo );

  if(!temp)
  {
    LOG(Error, "Error importing object: "
             << workspaceObject.briefDescription()
             << " Can't convert workspace curve into a model curve. ");
    return result;
  }
  boost::optional<Curve> tccfot = temp->optionalCast<Curve>();
  if( ! tccfot )
  {
    LOG(Error, "Error importing object: "
             << workspaceObject.briefDescription()
             << " curve is wrong type. ");
    return result;
  }

  owo = workspaceObject.getTarget(Coil_Cooling_DX_SingleSpeedFields::EnergyInputRatioFunctionofTemperatureCurveName);
  if(!owo)
  {
    LOG(Error, "Error importing object: "
             << workspaceObject.briefDescription()
             << " Can't find EnergyInputRatioFunctionofTemperatureCurveName.");
    return result;
  }
  if( owo->numSources() > 1 )
  {
    owo = owo->workspace().addObject(owo.get());
  }
  temp = translateAndMapWorkspaceObject( *owo );
  if(!temp)
  {
    LOG(Error, "Error importing object: "
             << workspaceObject.briefDescription()
             << " Can't convert workspace curve into a model curve. ");
    return result;
  }
  boost::optional<Curve> eirfot = temp->optionalCast<Curve>();
  if( ! eirfot )
  {
    LOG(Error, "Error importing object: "
             << workspaceObject.briefDescription()
             << " curve is wrong type. ");
    return result;
  }


  owo = workspaceObject.getTarget(Coil_Cooling_DX_SingleSpeedFields::TotalCoolingCapacityFunctionofFlowFractionCurveName);
  if(!owo)
  {
    LOG(Error, "Error importing object: "
             << workspaceObject.briefDescription()
             << " Can't find TotalCoolingCapacityFunctionofFlowFractionCurveName.");
    return result;
  }
  if( owo->numSources() > 1 )
  {
    owo = owo->workspace().addObject(owo.get());
  }
  temp = translateAndMapWorkspaceObject( *owo );
  if(!temp)
  {
    LOG(Error, "Error importing object: "
             << workspaceObject.briefDescription()
             << " Can't convert workspace curve into a model curve. ");
    return result;
  }
  boost::optional<Curve> tccfoff = temp->optionalCast<Curve>();
  if( ! tccfoff )
  {
    LOG(Error, "Error importing object: "
             << workspaceObject.briefDescription()
             << " curve is wrong type. ");
    return result;
  }

  owo = workspaceObject.getTarget(Coil_Cooling_DX_SingleSpeedFields::EnergyInputRatioFunctionofFlowFractionCurveName);
  if(!owo)
  {
    LOG(Error, "Error importing object: "
             << workspaceObject.briefDescription()
             << " Can't find EnergyInputRatioFunctionofFlowFractionCurveName.");
    return result;
  }
  if( owo->numSources() > 1 )
  {
    owo = owo->workspace().addObject(owo.get());
  }
  temp = translateAndMapWorkspaceObject( *owo );
  if(!temp)
  {
    LOG(Error, "Error importing object: "
             << workspaceObject.briefDescription()
             << " Can't convert workspace curve into a model curve. ");
    return result;
  }
  boost::optional<Curve> eirfoff = temp->optionalCast<Curve>();
  if( ! eirfoff )
  {
    LOG(Error, "Error importing object: "
             << workspaceObject.briefDescription()
             << " curve is wrong type. ");
    return result;
  }

  owo = workspaceObject.getTarget(Coil_Cooling_DX_SingleSpeedFields::PartLoadFractionCorrelationCurveName);
  if(!owo)
  {
    LOG(Error, "Error importing object: "
             << workspaceObject.briefDescription()
             << " Can't find PartLoadFractionCorrelationCurveName.");
    return result;
  }
  if( owo->numSources() > 1 )
  {
    owo = owo->workspace().addObject(owo.get());
  }
  temp = translateAndMapWorkspaceObject( *owo );
  if(!temp)
  {
    LOG(Error, "Error importing object: "
             << workspaceObject.briefDescription()
             << " Can't convert workspace curve into a model curve. ");
    return result;
  }
  boost::optional<Curve> plfcc = temp->optionalCast<Curve>();
  if( ! plfcc )
  {
    LOG(Error, "Error importing object: "
             << workspaceObject.briefDescription()
             << " curve is wrong type. ");
    return result;
  }


  try {
    CoilCoolingDXSingleSpeed coil(m_model,
                                  *schedule,
                                  *tccfot,
                                  *tccfoff,
                                  *eirfot,
                                  *eirfoff,
                                  *plfcc);

    OptionalString optS = workspaceObject.name();
    if( optS )
    {
      coil.setName( *optS );
    }


    OptionalDouble d = workspaceObject.getDouble(Coil_Cooling_DX_SingleSpeedFields::GrossRatedTotalCoolingCapacity);
    if(d)
    {
      coil.setRatedTotalCoolingCapacity(*d);
    }

    d=workspaceObject.getDouble(Coil_Cooling_DX_SingleSpeedFields::GrossRatedSensibleHeatRatio);
    if(d)
    {
      coil.setRatedSensibleHeatRatio(*d);
    }

    d=workspaceObject.getDouble(Coil_Cooling_DX_SingleSpeedFields::GrossRatedCoolingCOP);
    if(d)
    {
      coil.setRatedCOP(*d);
    }

    d=workspaceObject.getDouble(Coil_Cooling_DX_SingleSpeedFields::RatedAirFlowRate);
    if(d)
    {
      coil.setRatedAirFlowRate(*d);
    }

    d=workspaceObject.getDouble(Coil_Cooling_DX_SingleSpeedFields::RatedEvaporatorFanPowerPerVolumeFlowRate);
    if(d)
    {
      coil.setRatedEvaporatorFanPowerPerVolumeFlowRate(*d);
    }

    d=workspaceObject.getDouble(Coil_Cooling_DX_SingleSpeedFields::NominalTimeforCondensateRemovaltoBegin);
    if(d)
    {
      coil.setNominalTimeForCondensateRemovalToBegin(*d);
    }

    d=workspaceObject.getDouble(Coil_Cooling_DX_SingleSpeedFields::RatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity);
    if(d)
    {
      coil.setRatioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity(*d);
    }

    d=workspaceObject.getDouble(Coil_Cooling_DX_SingleSpeedFields::MaximumCyclingRate);
    if(d)
    {
      coil.setMaximumCyclingRate(*d);
    }

    d=workspaceObject.getDouble(Coil_Cooling_DX_SingleSpeedFields::LatentCapacityTimeConstant);
    if(d)
    {
      coil.setLatentCapacityTimeConstant(*d);
    }

    optS=workspaceObject.getString(Coil_Cooling_DX_SingleSpeedFields::CondenserAirInletNodeName);
    if(optS)
    {
      coil.setCondenserAirInletNodeName(*optS);
    }

    optS = workspaceObject.getString(Coil_Cooling_DX_SingleSpeedFields::CondenserType);
    if(optS)
    {
      coil.setCondenserType(*optS);
    }

    d=workspaceObject.getDouble(Coil_Cooling_DX_SingleSpeedFields::EvaporativeCondenserEffectiveness);
    if(d)
    {
      coil.setEvaporativeCondenserEffectiveness(*d);
    }

    d=workspaceObject.getDouble(Coil_Cooling_DX_SingleSpeedFields::EvaporativeCondenserAirFlowRate);
    if(d)
    {
      coil.setEvaporativeCondenserAirFlowRate(*d);
    }

    d=workspaceObject.getDouble(Coil_Cooling_DX_SingleSpeedFields::EvaporativeCondenserPumpRatedPowerConsumption);
    if(d)
    {
      coil.setEvaporativeCondenserPumpRatedPowerConsumption(*d);
    }

    d=workspaceObject.getDouble(Coil_Cooling_DX_SingleSpeedFields::CrankcaseHeaterCapacity);
    if(d)
    {
      coil.setCrankcaseHeaterCapacity(*d);
    }

    d=workspaceObject.getDouble(Coil_Cooling_DX_SingleSpeedFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);
    if(d)
    {
      coil.setMaximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation(*d);
    }

    result=coil;

  }
  catch (std::exception& e) {
    LOG(Error,"Could not reverse translate " << workspaceObject.briefDescription()
        << ", because " << e.what() << ".");
  }
  return result;
}

} // energyplus

} // openstudio

