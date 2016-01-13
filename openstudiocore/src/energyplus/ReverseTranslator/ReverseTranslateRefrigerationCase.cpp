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
#include "../../model/RefrigerationCase.hpp"
#include "../../model/RefrigerationCase_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/CurveCubic.hpp"
#include "../../model/CurveCubic_Impl.hpp"
#include <utilities/idd/Refrigeration_Case_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateRefrigerationCase( const WorkspaceObject & workspaceObject )
{
  if( workspaceObject.iddObject().type() != IddObjectType::Refrigeration_Case )
  {
     LOG(Error, "WorkspaceObject is not IddObjectType: Refrigeration_Case");
     return boost::none;
  }

  boost::optional<WorkspaceObject> wo = workspaceObject.getTarget(Refrigeration_CaseFields::CaseDefrostScheduleName);
  boost::optional<Schedule> caseDefrostSchedule;
  boost::optional<RefrigerationCase> refrigerationCase;

  if( wo )
  {
    if( boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(wo.get()) )
    {
      if( ! (caseDefrostSchedule = mo->optionalCast<Schedule>()) )
      {
        LOG(Error, workspaceObject.briefDescription() << " does not have an associated case defrost schedule");

        return boost::none;
      }
    }
  }

  if( caseDefrostSchedule )
  {
    refrigerationCase = RefrigerationCase( m_model,caseDefrostSchedule.get() );
  }

  if( refrigerationCase )
  {
    boost::optional<double> value;
    boost::optional<std::string> s = workspaceObject.getString(Refrigeration_CaseFields::Name);

// Name
    if( s )
    {
      refrigerationCase->setName(s.get());
    }
// AvailabilityScheduleName
    if( (wo = workspaceObject.getTarget(Refrigeration_CaseFields::AvailabilityScheduleName)) )
    {
      if( boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(wo.get()) )
      {
        if( boost::optional<Schedule> schedule = mo->optionalCast<Schedule>() )
        {
          refrigerationCase->setAvailabilitySchedule(schedule.get());
        }
      }
    }
// ZoneName
    if( (wo = workspaceObject.getTarget(Refrigeration_CaseFields::ZoneName)) )
    {
      if( boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(wo.get()) )
      {
        if( boost::optional<ThermalZone> thermalZone = mo->optionalCast<ThermalZone>() )
        {
          refrigerationCase->setThermalZone(thermalZone.get());
        }
      }
    }
// RatedAmbientTemperature
    value = workspaceObject.getDouble(Refrigeration_CaseFields::RatedAmbientTemperature);
    if( value )
    {
      refrigerationCase->setRatedAmbientTemperature(value.get());
    }
// RatedAmbientRelativeHumidity
    value = workspaceObject.getDouble(Refrigeration_CaseFields::RatedAmbientRelativeHumidity);
    if( value )
    {
      refrigerationCase->setRatedAmbientRelativeHumidity(value.get());
    }
// RatedTotalCoolingCapacityperUnitLength
    value = workspaceObject.getDouble(Refrigeration_CaseFields::RatedTotalCoolingCapacityperUnitLength);
    if( value )
    {
      refrigerationCase->setRatedTotalCoolingCapacityperUnitLength(value.get());
    }
// RatedLatentHeatRatio
    value = workspaceObject.getDouble(Refrigeration_CaseFields::RatedLatentHeatRatio);
    if( value )
    {
      refrigerationCase->setRatedLatentHeatRatio(value.get());
    }
// RatedRuntimeFraction
    value = workspaceObject.getDouble(Refrigeration_CaseFields::RatedRuntimeFraction);
    if( value )
    {
      refrigerationCase->setRatedRuntimeFraction(value.get());
    }
// CaseLength
    value = workspaceObject.getDouble(Refrigeration_CaseFields::CaseLength);
    if( value )
    {
      refrigerationCase->setCaseLength(value.get());
    }
// CaseOperatingTemperature
    value = workspaceObject.getDouble(Refrigeration_CaseFields::CaseOperatingTemperature);
    if( value )
    {
      refrigerationCase->setCaseOperatingTemperature(value.get());
    }
// LatentCaseCreditCurveType
    s = workspaceObject.getString(Refrigeration_CaseFields::LatentCaseCreditCurveType);
    if( s )
    {
      refrigerationCase->setLatentCaseCreditCurveType(s.get());
    }
// LatentCaseCreditCurveName
    if( (wo = workspaceObject.getTarget(Refrigeration_CaseFields::LatentCaseCreditCurveName)) )
    {
      if( boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(wo.get()) )
      {
        if( boost::optional<CurveCubic> curve = mo->optionalCast<CurveCubic>() )
        {
          refrigerationCase->setLatentCaseCreditCurve(curve.get());
        }
      }
    }
// StandardCaseFanPowerperUnitLength
    value = workspaceObject.getDouble(Refrigeration_CaseFields::StandardCaseFanPowerperUnitLength);
    if( value )
    {
      refrigerationCase->setStandardCaseFanPowerperUnitLength(value.get());
    }
// OperatingCaseFanPowerperUnitLength
    value = workspaceObject.getDouble(Refrigeration_CaseFields::OperatingCaseFanPowerperUnitLength);
    if( value )
    {
      refrigerationCase->setOperatingCaseFanPowerperUnitLength(value.get());
    }
// StandardCaseLightingPowerperUnitLength
    value = workspaceObject.getDouble(Refrigeration_CaseFields::StandardCaseLightingPowerperUnitLength);
    if( value )
    {
      refrigerationCase->setStandardCaseLightingPowerperUnitLength(value.get());
    }
// InstalledCaseLightingPowerperUnitLength
    value = workspaceObject.getDouble(Refrigeration_CaseFields::InstalledCaseLightingPowerperUnitLength);
    if( value )
    {
      refrigerationCase->setInstalledCaseLightingPowerperUnitLength(value.get());
    }
// CaseLightingScheduleName
    if( (wo = workspaceObject.getTarget(Refrigeration_CaseFields::CaseLightingScheduleName)) )
    {
      if( boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(wo.get()) )
      {
        if( boost::optional<Schedule> schedule = mo->optionalCast<Schedule>() )
        {
          refrigerationCase->setCaseLightingSchedule(schedule.get());
        }
      }
    }
// FractionofLightingEnergytoCase
    value = workspaceObject.getDouble(Refrigeration_CaseFields::FractionofLightingEnergytoCase);
    if( value )
    {
      refrigerationCase->setFractionofLightingEnergytoCase(value.get());
    }
// CaseAntiSweatHeaterPowerperUnitLength
    value = workspaceObject.getDouble(Refrigeration_CaseFields::CaseAntiSweatHeaterPowerperUnitLength);
    if( value )
    {
      refrigerationCase->setCaseAntiSweatHeaterPowerperUnitLength(value.get());
    }
// MinimumAntiSweatHeaterPowerperUnitLength
    value = workspaceObject.getDouble(Refrigeration_CaseFields::MinimumAntiSweatHeaterPowerperUnitLength);
    if( value )
    {
      refrigerationCase->setMinimumAntiSweatHeaterPowerperUnitLength(value.get());
    }
// AntiSweatHeaterControlType
    s = workspaceObject.getString(Refrigeration_CaseFields::AntiSweatHeaterControlType);
    if( s )
    {
      refrigerationCase->setAntiSweatHeaterControlType(s.get());
    }
// HumidityatZeroAntiSweatHeaterEnergy
    value = workspaceObject.getDouble(Refrigeration_CaseFields::HumidityatZeroAntiSweatHeaterEnergy);
    if( value )
    {
      refrigerationCase->setHumidityatZeroAntiSweatHeaterEnergy(value.get());
    }
// CaseHeight
    value = workspaceObject.getDouble(Refrigeration_CaseFields::CaseHeight);
    if( value )
    {
      refrigerationCase->setCaseHeight(value.get());
    }
// FractionofAntiSweatHeaterEnergytoCase
    value = workspaceObject.getDouble(Refrigeration_CaseFields::FractionofAntiSweatHeaterEnergytoCase);
    if( value )
    {
      refrigerationCase->setFractionofAntiSweatHeaterEnergytoCase(value.get());
    }
// CaseDefrostPowerperUnitLength
    value = workspaceObject.getDouble(Refrigeration_CaseFields::CaseDefrostPowerperUnitLength);
    if( value )
    {
      refrigerationCase->setCaseDefrostPowerperUnitLength(value.get());
    }
// CaseDefrostType
    s = workspaceObject.getString(Refrigeration_CaseFields::CaseDefrostType);
    if( s )
    {
      refrigerationCase->setCaseDefrostType(s.get());
    }
// CaseDefrostScheduleName
    if( (wo = workspaceObject.getTarget(Refrigeration_CaseFields::CaseDefrostScheduleName)) )
    {
      if( boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(wo.get()) )
      {
        if( boost::optional<Schedule> schedule = mo->optionalCast<Schedule>() )
        {
          refrigerationCase->setCaseDefrostSchedule(schedule.get());
        }
      }
    }
// CaseDefrostDripDownScheduleName
    if( (wo = workspaceObject.getTarget(Refrigeration_CaseFields::CaseDefrostDripDownScheduleName)) )
    {
      if( boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(wo.get()) )
      {
        if( boost::optional<Schedule> schedule = mo->optionalCast<Schedule>() )
        {
          refrigerationCase->setCaseDefrostDripDownSchedule(schedule.get());
        }
      }
    }
// DefrostEnergyCorrectionCurveType
    s = workspaceObject.getString(Refrigeration_CaseFields::DefrostEnergyCorrectionCurveType);
    if( s )
    {
      refrigerationCase->setDefrostEnergyCorrectionCurveType(s.get());
    }
// DefrostEnergyCorrectionCurveName
    if( (wo = workspaceObject.getTarget(Refrigeration_CaseFields::DefrostEnergyCorrectionCurveName)) )
    {
      if( boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(wo.get()) )
      {
        if( boost::optional<CurveCubic> curve = mo->optionalCast<CurveCubic>() )
        {
          refrigerationCase->setDefrostEnergyCorrectionCurve(curve.get());
        }
      }
    }
// UnderCaseHVACReturnAirFraction
    value = workspaceObject.getDouble(Refrigeration_CaseFields::UnderCaseHVACReturnAirFraction);
    if( value )
    {
      refrigerationCase->setUnderCaseHVACReturnAirFraction(value.get());
    }
// RefrigeratedCaseRestockingScheduleName
    if( (wo = workspaceObject.getTarget(Refrigeration_CaseFields::RefrigeratedCaseRestockingScheduleName)) )
    {
      if( boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(wo.get()) )
      {
        if( boost::optional<Schedule> schedule = mo->optionalCast<Schedule>() )
        {
          refrigerationCase->setRefrigeratedCaseRestockingSchedule(schedule.get());
        }
      }
    }
// CaseCreditFractionScheduleName
    if( (wo = workspaceObject.getTarget(Refrigeration_CaseFields::CaseCreditFractionScheduleName)) )
    {
      if( boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(wo.get()) )
      {
        if( boost::optional<Schedule> schedule = mo->optionalCast<Schedule>() )
        {
          refrigerationCase->setCaseCreditFractionSchedule(schedule.get());
        }
      }
    }
// DesignEvaporatorTemperatureorBrineInletTemperature
    value = workspaceObject.getDouble(Refrigeration_CaseFields::DesignEvaporatorTemperatureorBrineInletTemperature);
    if( value )
    {
      refrigerationCase->setDesignEvaporatorTemperatureorBrineInletTemperature(value.get());
    }
// AverageRefrigerantChargeInventory
    value = workspaceObject.getDouble(Refrigeration_CaseFields::AverageRefrigerantChargeInventory);
    if( value )
    {
      refrigerationCase->setAverageRefrigerantChargeInventory(value.get());
    }

    return refrigerationCase.get();
  }
  else
  {
    LOG(Error, "Unknown error translating " << workspaceObject.briefDescription());

    return boost::none;
  }
}

} // energyplus

} // openstudio

