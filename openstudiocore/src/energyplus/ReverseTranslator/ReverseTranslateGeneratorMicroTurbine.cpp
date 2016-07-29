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

#include "../../model/GeneratorMicroTurbine.hpp"
#include "../../model/GeneratorMicroTurbine_Impl.hpp"
#include "../../model/GeneratorMicroTurbineHeatRecovery.hpp"
#include "../../model/GeneratorMicroTurbineHeatRecovery_Impl.hpp"

#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"
#include "../../model/CurveBicubic.hpp"
#include "../../model/CurveBicubic_Impl.hpp"
#include "../../model/CurveBiquadratic.hpp"
#include "../../model/CurveBiquadratic_Impl.hpp"
#include "../../model/CurveCubic.hpp"
#include "../../model/CurveCubic_Impl.hpp"
#include "../../model/CurveQuadratic.hpp"
#include "../../model/CurveQuadratic_Impl.hpp"

#include <utilities/idd/Generator_MicroTurbine_FieldEnums.hxx>
//#include <utilities/idd/Generator_MicroTurbine_HeatRecoveryFieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateGeneratorMicroTurbine( const WorkspaceObject & workspaceObject )
{

  OptionalModelObject result,temp;
  OptionalDouble d;
  boost::optional<WorkspaceObject> owo;
  OptionalString optS;
  
  
  // TODO: The availability schedule is in the ElectricLoadCenter:Generators (list) in E+, here it's carried by the generator itself
  // Should also get the Rated Thermal To Electrical Power Ratio in the list

  //Generator:MicroTurbine,
  //    Capstone C65,            !- Name

  openstudio::model::GeneratorMicroTurbine mchp( m_model );
  
  // Name
  optS = workspaceObject.name();
  if(optS)
  {
    mchp.setName(*optS);
  }

  
    
  //    65000,                   !- Reference Electrical Power Output {W}
  d = workspaceObject.getDouble(Generator_MicroTurbineFields::ReferenceElectricalPowerOutput);
  if(d)
  {
    mchp.setReferenceElectricalPowerOutput(*d);
  }
  //    29900,                   !- Minimum Full Load Electrical Power Output {W}
  d = workspaceObject.getDouble(Generator_MicroTurbineFields::MinimumFullLoadElectricalPowerOutput);
  if(d)
  {
    mchp.setMinimumFullLoadElectricalPowerOutput(*d);
  }
  //    65000,                   !- Maximum Full Load Electrical Power Output {W} setMaximumFullLoadElectricalPowerOutput
  d = workspaceObject.getDouble(Generator_MicroTurbineFields::MaximumFullLoadElectricalPowerOutput);
  if(d)
  {
    mchp.setMaximumFullLoadElectricalPowerOutput(*d);
  }

  //    0.29,                    !- Reference Electrical Efficiency Using Lower Heating Value
  d = workspaceObject.getDouble(Generator_MicroTurbineFields::ReferenceElectricalEfficiencyUsingLowerHeatingValue);
  if(d)
  {
    mchp.setReferenceElectricalEfficiencyUsingLowerHeatingValue(*d);
  }

  //    15.0,                    !- Reference Combustion Air Inlet Temperature {C}
  d = workspaceObject.getDouble(Generator_MicroTurbineFields::ReferenceCombustionAirInletTemperature);
  if(d)
  {
    mchp.setReferenceCombustionAirInletTemperature(*d);
  }

  //    0.00638,                 !- Reference Combustion Air Inlet Humidity Ratio {kgWater/kgDryAir}
  d = workspaceObject.getDouble(Generator_MicroTurbineFields::ReferenceCombustionAirInletHumidityRatio);
  if(d)
  {
    mchp.setReferenceCombustionAirInletHumidityRatio(*d);
  }

  //    0.0,                     !- Reference Elevation {m}
  d = workspaceObject.getDouble(Generator_MicroTurbineFields::MinimumFullLoadElectricalPowerOutput);
  if(d)
  {
    mchp.setMinimumFullLoadElectricalPowerOutput(*d);
  }

  //    Capstone C65 Power_vs_Temp_Elev,  !- Electrical Power Function of Temperature and Elevation Curve Name
  // BiquadraticCurves
  if( (owo = workspaceObject.getTarget(Generator_MicroTurbineFields::ElectricalPowerFunctionofTemperatureandElevationCurveName)) )
    {
      if( boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(owo.get()) )
      {
        if( boost::optional<CurveBiquadratic> curve = mo->optionalCast<CurveBiquadratic>() )
        {
          mchp.setElectricalPowerFunctionofTemperatureandElevationCurve(curve.get());
        }
        else
        {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type curve for field Electrical Power Function of Temperature and Elevation Curve Name");
        }
      }
    }

  //    Capstone C65 Efficiency_vs_Temp,  !- Electrical Efficiency Function of Temperature Curve Name
  // QuadraticCubicCurves
  if( (owo = workspaceObject.getTarget(Generator_MicroTurbineFields::ElectricalEfficiencyFunctionofTemperatureCurveName)) )
    {
      if( boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(owo.get()) )
      {
        if( boost::optional<CurveQuadratic> curve = mo->optionalCast<CurveQuadratic>() )
        {
          mchp.setElectricalEfficiencyFunctionofTemperatureCurve(curve.get());
        }
        else if( boost::optional<CurveCubic> curve = mo->optionalCast<CurveCubic>() )
        {
          mchp.setElectricalEfficiencyFunctionofTemperatureCurve(curve.get());
        }
        else
        {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type curve for field Electrical Efficiency Function of Temperature Curve Name");
        }
      }
    }
  

  //    Capstone C65 Efficiency_vs_PLR,  !- Electrical Efficiency Function of Part Load Ratio Curve Name
  // QuadraticCubicCurves
  // setElectricalEfficiencyFunctionofPartLoadRatioCurve
  if( (owo = workspaceObject.getTarget(Generator_MicroTurbineFields::ElectricalEfficiencyFunctionofPartLoadRatioCurveName)) )
    {
      if( boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(owo.get()) )
      {
        if( boost::optional<CurveQuadratic> curve = mo->optionalCast<CurveQuadratic>() )
        {
          mchp.setElectricalEfficiencyFunctionofPartLoadRatioCurve(curve.get());
        }
        else if( boost::optional<CurveCubic> curve = mo->optionalCast<CurveCubic>() )
        {
          mchp.setElectricalEfficiencyFunctionofPartLoadRatioCurve(curve.get());
        }
        else
        {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type curve for field Electrical Efficiency Function of Part Load Ratio Curve Name");
        }
      }
    }

  //    NaturalGas,              !- Fuel Type
  optS = workspaceObject.getString(Generator_MicroTurbineFields::FuelType);
  if(optS)
  {
    mchp.setFuelType(*optS);
  }
    
  //    50000,                   !- Fuel Higher Heating Value {kJ/kg}
  d = workspaceObject.getDouble(Generator_MicroTurbineFields::FuelHigherHeatingValue);
  if(d)
  {
    mchp.setFuelHigherHeatingValue(*d);
  }

  //    45450,                   !- Fuel Lower Heating Value {kJ/kg}
  d = workspaceObject.getDouble(Generator_MicroTurbineFields::FuelLowerHeatingValue);
  if(d)
  {
    mchp.setFuelLowerHeatingValue(*d);
  }

  //    300,                     !- Standby Power {W}
  d = workspaceObject.getDouble(Generator_MicroTurbineFields::StandbyPower);
  if(d)
  {
    mchp.setStandbyPower(*d);
  }

  //    4500,                    !- Ancillary Power {W}
  d = workspaceObject.getDouble(Generator_MicroTurbineFields::AncillaryPower);
  if(d)
  {
    mchp.setAncillaryPower(*d);
  }

  //    ,                        !- Ancillary Power Function of Fuel Input Curve Name
  // QuadraticCurves
  // mchp.setAncillaryPowerFunctionofFuelInputCurve
  if( (owo = workspaceObject.getTarget(Generator_MicroTurbineFields::AncillaryPowerFunctionofFuelInputCurveName)) )
    {
      if( boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(owo.get()) )
      {
        if( boost::optional<CurveQuadratic> curve = mo->optionalCast<CurveQuadratic>() )
        {
          mchp.setAncillaryPowerFunctionofFuelInputCurve(curve.get());
        }
        else
        {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type curve for field Ancillary Power Function of Fuel Input Curve Name");
        }
      }
    }

  
  // Fields in between (in E+.idd) are moved at the end in the Heat Recovery section

  //    Capstone C65 Combustion Air Inlet Node,  !- Combustion Air Inlet Node Name
  //    Capstone C65 Combustion Air Outlet Node,  !- Combustion Air Outlet Node Name


  //    0.489885,                !- Reference Exhaust Air Mass Flow Rate {kg/s}
  d = workspaceObject.getDouble(Generator_MicroTurbineFields::ReferenceExhaustAirMassFlowRate);
  if(d)
  {
    mchp.setReferenceExhaustAirMassFlowRate(*d);
  }

  //    Capstone C65 ExhAirFlowRate_vs_InletTemp,  !- Exhaust Air Flow Rate Function of Temperature Curve Name
  // QuadraticCubicCurves
  // mchp.setExhaustAirFlowRateFunctionofTemperatureCurve
  if( (owo = workspaceObject.getTarget(Generator_MicroTurbineFields::ExhaustAirFlowRateFunctionofTemperatureCurveName)) )
    {
      if( boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(owo.get()) )
      {
        if( boost::optional<CurveQuadratic> curve = mo->optionalCast<CurveQuadratic>() )
        {
          mchp.setExhaustAirFlowRateFunctionofTemperatureCurve(curve.get());
        }
        else if( boost::optional<CurveCubic> curve = mo->optionalCast<CurveCubic>() )
        {
          mchp.setExhaustAirFlowRateFunctionofTemperatureCurve(curve.get());
        }
        else
        {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type curve for field Exhaust Air Flow Rate Function of Temperature Curve Name");
        }
      }
    }

  //    Capstone C65 ExhAirFlowRate_vs_PLR,  !- Exhaust Air Flow Rate Function of Part Load Ratio Curve Name
  // QuadraticCubicCurves
  // mchp.setExhaustAirFlowRateFunctionofPartLoadRatioCurve(curve)
  if( (owo = workspaceObject.getTarget(Generator_MicroTurbineFields::ExhaustAirFlowRateFunctionofPartLoadRatioCurveName)) )
    {
      if( boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(owo.get()) )
      {
        if( boost::optional<CurveQuadratic> curve = mo->optionalCast<CurveQuadratic>() )
        {
          mchp.setExhaustAirFlowRateFunctionofPartLoadRatioCurve(curve.get());
        }
        else if( boost::optional<CurveCubic> curve = mo->optionalCast<CurveCubic>() )
        {
          mchp.setExhaustAirFlowRateFunctionofPartLoadRatioCurve(curve.get());
        }
        else
        {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type curve for field Exhaust Air Flow Rate Function of Part Load Ratio Curve Name");
        }
      }
    }

  //    308.9,                   !- Nominal Exhaust Air Outlet Temperature
  d = workspaceObject.getDouble(Generator_MicroTurbineFields::NominalExhaustAirOutletTemperature);
  if(d)
  {
    mchp.setNominalExhaustAirOutletTemperature(*d);
  }

  //    Capstone C65 ExhaustTemp_vs_InletTemp,  !- Exhaust Air Temperature Function of Temperature Curve Name
  // QuadraticCubicCurves
  // mchp.setExhaustAirTemperatureFunctionofTemperatureCurve(curve)
  if( (owo = workspaceObject.getTarget(Generator_MicroTurbineFields::ExhaustAirTemperatureFunctionofTemperatureCurveName)) )
    {
      if( boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(owo.get()) )
      {
        if( boost::optional<CurveQuadratic> curve = mo->optionalCast<CurveQuadratic>() )
        {
          mchp.setExhaustAirTemperatureFunctionofTemperatureCurve(curve.get());
        }
        else if( boost::optional<CurveCubic> curve = mo->optionalCast<CurveCubic>() )
        {
          mchp.setExhaustAirTemperatureFunctionofTemperatureCurve(curve.get());
        }
        else
        {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type curve for field Exhaust Air Temperature Function of Temperature Curve Name");
        }
      }
    }

  //    Capstone C65 ExhaustTemp_vs_PLR;  !- Exhaust Air Temperature Function of Part Load Ratio Curve Name
  // QuadraticCubicCurves
  // mchp.setExhaustAirTemperatureFunctionofPartLoadRatioCurve(curve)
  if( (owo = workspaceObject.getTarget(Generator_MicroTurbineFields::ExhaustAirTemperatureFunctionofPartLoadRatioCurveName)) )
    {
      if( boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(owo.get()) )
      {
        if( boost::optional<CurveQuadratic> curve = mo->optionalCast<CurveQuadratic>() )
        {
          mchp.setExhaustAirTemperatureFunctionofPartLoadRatioCurve(curve.get());
        }
        else if( boost::optional<CurveCubic> curve = mo->optionalCast<CurveCubic>() )
        {
          mchp.setExhaustAirTemperatureFunctionofPartLoadRatioCurve(curve.get());
        }
        else
        {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type curve for field Exhaust Air Temperature Function of Part Load Ratio Curve Name");
        }
      }
    }
    
    
  /// HEAT RECOVERY PORTION
  
  // Would need to add that to the plantLoop reverse translator
  //    Capstone C65 Heat Recovery Water Inlet Node,  !- Heat Recovery Water Inlet Node Name
  //    Capstone C65 Heat Recovery Water Outlet Node,  !- Heat Recovery Water Outlet Node Name
  

  
  
  // TODO: For now, I trigger the creation is the 'Reference Thermal Efficiency Using Lower Heat Value' is filled.
  // TODO: I should trigger it based on the `Rated Thermal To Electrical Power Ratio in the list`  in the ElectricLoadCenter:Generators (list)
  // TODO: But in order to do that, the ElectricLoadCenter:Distribution & ElectricLoadCenter:Generators need to have a reverse translator
  
  //    0.4975,                  !- Reference Thermal Efficiency Using Lower Heat Value
  d = workspaceObject.getDouble(Generator_MicroTurbineFields::ReferenceThermalEfficiencyUsingLowerHeatValue);
  
  if(d)
  {
   
    // Create a GeneratorMicroTurbineHeatRecovery module, and assign it to the MicroTurbine
    // I've Set the Name in the constructor
    openstudio::model::GeneratorMicroTurbineHeatRecovery mchpHR (m_model, mchp);
    
    // Assign the Reference Thermal Efficiency Using Lower Heat Value
    mchpHR.setReferenceThermalEfficiencyUsingLowerHeatValue(*d);
    

    //    60.0,                    !- Reference Inlet Water Temperature {C}
    d = workspaceObject.getDouble(Generator_MicroTurbineFields::ReferenceInletWaterTemperature);
    if(d)
    {
      mchpHR.setReferenceInletWaterTemperature(*d);
    }
    
    //    PlantControl,            !- Heat Recovery Water Flow Operating Mode
    optS = workspaceObject.getString(Generator_MicroTurbineFields::HeatRecoveryWaterFlowOperatingMode);
    if(optS)
    {
      mchpHR.setHeatRecoveryWaterFlowOperatingMode(*optS);
    }
    
    //    0.00252362,              !- Reference Heat Recovery Water Flow Rate {m3/s}
    d = workspaceObject.getDouble(Generator_MicroTurbineFields::ReferenceHeatRecoveryWaterFlowRate);
    if(d)
    {
      mchpHR.setReferenceHeatRecoveryWaterFlowRate(*d);
    }
 
    //    ,                        !- Heat Recovery Water Flow Rate Function of Temperature and Power Curve Name
    // BiquadraticCurves
    // mchpHR.setHeatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve();
    if( (owo = workspaceObject.getTarget(Generator_MicroTurbineFields::HeatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurveName)) )
      {
        if( boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(owo.get()) )
        {
          if( boost::optional<CurveBiquadratic> curve = mo->optionalCast<CurveBiquadratic>() )
          {
            mchpHR.setHeatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve(curve.get());
          }
          else
          {
            LOG(Warn, workspaceObject.briefDescription() << " has a wrong type curve for field Heat Recovery Water Flow Rate Function of Temperature and Power Curve Name");
          }
        }
      }

    //    Capstone C65 ThermalEff_vs_Temp_Elev,  !- Thermal Efficiency Function of Temperature and Elevation Curve Name
    // BicubicBiquadraticCurves
    // mchpHR.setThermalEfficiencyFunctionofTemperatureandElevationCurve();
    if( (owo = workspaceObject.getTarget(Generator_MicroTurbineFields::ThermalEfficiencyFunctionofTemperatureandElevationCurveName)) )
      {
        if( boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(owo.get()) )
        {
          if( boost::optional<CurveBicubic> curve = mo->optionalCast<CurveBicubic>() )
          {
            mchpHR.setThermalEfficiencyFunctionofTemperatureandElevationCurve(curve.get());
          }
          else if( boost::optional<CurveBiquadratic> curve = mo->optionalCast<CurveBiquadratic>() )
          {
            mchpHR.setThermalEfficiencyFunctionofTemperatureandElevationCurve(curve.get());
          }
          else
          {
            LOG(Warn, workspaceObject.briefDescription() << " has a wrong type curve for field Thermal Efficiency Function of Temperature and Elevation Curve Name");
          }
        }
      }

    //    Capstone C65 HeatRecoveryRate_vs_PLR,  !- Heat Recovery Rate Function of Part Load Ratio Curve Name
    // QuadraticCubicCurves
    // mchpHR.setHeatRecoveryRateFunctionofPartLoadRatioCurve();
    if( (owo = workspaceObject.getTarget(Generator_MicroTurbineFields::HeatRecoveryRateFunctionofPartLoadRatioCurveName)) )
      {
        if( boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(owo.get()) )
        {
          if( boost::optional<CurveQuadratic> curve = mo->optionalCast<CurveQuadratic>() )
          {
            mchpHR.setHeatRecoveryRateFunctionofPartLoadRatioCurve(curve.get());
          }
          else if( boost::optional<CurveCubic> curve = mo->optionalCast<CurveCubic>() )
          {
            mchpHR.setHeatRecoveryRateFunctionofPartLoadRatioCurve(curve.get());
          }
          else
          {
            LOG(Warn, workspaceObject.briefDescription() << " has a wrong type curve for field Heat Recovery Rate Function of Part Load Ratio Curve Name");
          }
        }
      }

    //    Capstone C65 HeatRecoveryRate_vs_InletTemp,  !- Heat Recovery Rate Function of Inlet Water Temperature Curve Name
    // QuadraticCurves
    // mchpHR.setHeatRecoveryRateFunctionofInletWaterTemperatureCurve();
    if ((owo = workspaceObject.getTarget(Generator_MicroTurbineFields::HeatRecoveryRateFunctionofInletWaterTemperatureCurveName)))
      {
        if( boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(owo.get()) )
        {
          if( boost::optional<CurveQuadratic> curve = mo->optionalCast<CurveQuadratic>() )
          {
            mchpHR.setHeatRecoveryRateFunctionofInletWaterTemperatureCurve(curve.get());
          }
          else
          {
            LOG(Warn, workspaceObject.briefDescription() << " has a wrong type curve for field Heat Recovery Rate Function of Part Load Ratio Curve Name");
          }
        }
      }

    //    Capstone C65 HeatRecoveryRate_vs_WaterFlow,  !- Heat Recovery Rate Function of Water Flow Rate Curve Name
    // QuadraticCurves
    // mchpHR.setHeatRecoveryRateFunctionofInletWaterFlowRateCurve();
    if( (owo = workspaceObject.getTarget(Generator_MicroTurbineFields::HeatRecoveryRateFunctionofWaterFlowRateCurveName)) )
      {
        if( boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(owo.get()) )
        {
          if( boost::optional<CurveQuadratic> curve = mo->optionalCast<CurveQuadratic>() )
          {
            mchpHR.setHeatRecoveryRateFunctionofWaterFlowRateCurve(curve.get());
          }
          else
          {
            LOG(Warn, workspaceObject.briefDescription() << " has a wrong type curve for field Heat Recovery Rate Function of Water Flow Rate Curve Name");
          }
        }
      }

    //    0.001577263,             !- Minimum Heat Recovery Water Flow Rate {m3/s}
    d = workspaceObject.getDouble(Generator_MicroTurbineFields::MinimumHeatRecoveryWaterFlowRate);
    if(d)
    {
      mchpHR.setMinimumHeatRecoveryWaterFlowRate(*d);
    }

    //    0.003785432,             !- Maximum Heat Recovery Water Flow Rate {m3/s}
    d = workspaceObject.getDouble(Generator_MicroTurbineFields::MaximumHeatRecoveryWaterFlowRate);
    if(d)
    {
      mchpHR.setMaximumHeatRecoveryWaterFlowRate(*d);
    }

    //    82.2,                    !- Maximum Heat Recovery Water Temperature {C}
    d = workspaceObject.getDouble(Generator_MicroTurbineFields::MaximumHeatRecoveryWaterTemperature);
    if(d)
    {
      mchpHR.setMaximumHeatRecoveryWaterTemperature(*d);
    }
    
    
  }
    
    
  result=mchp;
  return result;
}

} // energyplus

} // openstudio
