/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateElectricLoadCenterStorageConverter( const WorkspaceObject & workspaceObject )
{

  OptionalModelObject result,temp;
  OptionalDouble optD;
  boost::optional<WorkspaceObject> owo;
  OptionalString optS;
  bool isSimpleFixed = true;
  
  // TODO: The availability schedule is in the ElectricLoadCenter:Generators (list) in E+, here it's carried by the generator itself
  // Should also get the Rated Thermal To Electrical Power Ratio in the list

  //Generator:MicroTurbine,
  //    Capstone C65,            !- Name

  openstudio::model::ElectricLoadCenterStorageConverter elcConv( m_model );
  
  // Name
  optS = workspaceObject.name();
  if(optS)
  {
    elcConv.setName(*optS);
  }

  // AvailabilityScheduleName
  if( (owo = workspaceObject.getTarget(ElectricLoadCenter_Storage_ConverterFields::AvailabilityScheduleName)) )
  {
    if( boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(owo.get()) )
    {
      if( boost::optional<Schedule> schedule = mo->optionalCast<Schedule>() )
      {
        elcConv.setAvailabilitySchedule(schedule.get());
      }
    }
  }
  
  // PowerConversionEfficiencyMethod, string
  optS = workspaceObject.getString(ElectricLoadCenter_Storage_ConverterFields::PowerConversionEfficiencyMethod);
  if(optS)
  {
    if ((*optS).toUpper() == 'FUNCTIONOFPOWER')
    {
      isSimpleFixed = false;
    }
  }
  
  if (isSimpleFixed) {
    // SimpleFixedEfficiency, optD
    optD = workspaceObject.getDouble(ElectricLoadCenter_Storage_ConverterFields::SimpleFixedEfficiency);
    if(optD)
    {
      elcConv.setSimpleFixedEfficiency(*optD);
    }
  } else {
    // designMaximumContinuousInputPower, optD
    optD = workspaceObject.getDouble(ElectricLoadCenter_Storage_ConverterFields::DesignMaximumContinuousInputPower);
    if(optD)
    {
      elcConv.setDesignMaximumContinuousInputPower(*optD);
    }
    
    // efficiencyFunctionofPowerCurveName, optCurve
    if( (owo = workspaceObject.getTarget(ElectricLoadCenter_Storage_ConverterFields::EfficiencyFunctionofPowerCurveName)) )
    {
      if( boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(owo.get()) )
      {
        // Should technically make sure the curve is the right type, but in this case it's UnivariateCurves, lots of possibilities.
        if( boost::optional<Curve> effFPower = mo->optionalCast<Curve>() )
        {
          elcConv.setEfficiencyFunctionofPowerCurveName(effFPower.get());
        }
      }
    }
  }
  
  // ZoneName
  if( (owo = workspaceObject.getTarget(ElectricLoadCenter_Storage_ConverterFields::ZoneName)) )
  {
    if( boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(owo.get()) )
    {
      if( boost::optional<ThermalZone> thermalZone = mo->optionalCast<ThermalZone>() )
      {
        elcConv.setThermalZone(thermalZone.get());
      }
    }
  }

  // Radiative Fraction, defaults (double)
  optD = workspaceObject.getDouble(ElectricLoadCenter_Storage_ConverterFields::RadiativeFractionforZoneHeatGains);
  if(optD)
  {
    elcConv.setRadiativeFraction(*optD);
  }
     
  
  
  result=elcConv;
  return result;
}

} // energyplus

} // openstudio
