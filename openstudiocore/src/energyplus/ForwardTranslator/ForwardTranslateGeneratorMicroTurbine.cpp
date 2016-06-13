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

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"

#include "../../model/GeneratorMicroTurbine.hpp"
#include "../../model/GeneratorMicroTurbine_Impl.hpp"

#include "../../model/StraightComponent.hpp"
#include "../../model/StraightComponent_Impl.hpp"
#include "../../model/Node.hpp" // Is this needed?
#include "../../model/Node_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"

#include "../../model/GeneratorMicroTurbineHeatRecovery.hpp"
#include "../../model/GeneratorMicroTurbineHeatRecovery_Impl.hpp"

#include <utilities/idd/Generator_MicroTurbine_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateGeneratorMicroTurbine(model::GeneratorMicroTurbine & modelObject)
{
  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::Generator_MicroTurbine, modelObject);


  //get the GeneratorMicroTurbineHeatRecovery as an optional straight component, if exists cast it to optional GeneratorMicroTurbineHeatRecovery
  // if cast is successful, get the object of class GeneratorMicroTurbineHeatRecovery
  boost::optional<GeneratorMicroTurbineHeatRecovery> generatorOptionalMicroTurbineHeatRecovery = modelObject.generatorMicroTurbineHeatRecovery();
  // If there is a Heat Recovery Object
  if (generatorOptionalMicroTurbineHeatRecovery) {
    // Get it
    GeneratorMicroTurbineHeatRecovery generatorMCHPHX = *generatorOptionalMicroTurbineHeatRecovery;
    
    // HeatRecoveryWaterInletNodeName
    // Inlet Node Name 
    if ( auto temp = generatorMCHPHX.inletModelObject() )
    {
      auto s = temp->name();
      if(s)
      {
        idfObject.setString(openstudio::Generator_MicroTurbineFields::HeatRecoveryWaterInletNodeName,*s);
      }
    }

    //HeatRecoveryWaterOutletNodeName
    if ( auto temp = generatorMCHPHX.outletModelObject() ) {
      auto s = temp->name();
      if (s) {
        idfObject.setString(openstudio::Generator_MicroTurbineFields::HeatRecoveryWaterOutletNodeName, *s);
      }
    }

    //ReferenceThermalEfficiencyUsingLowerHeatValue
    {
      auto value = generatorMCHPHX.referenceThermalEfficiencyUsingLowerHeatValue();
      idfObject.setDouble(Generator_MicroTurbineFields::ReferenceThermalEfficiencyUsingLowerHeatValue, value);
    }

    //ReferenceInletWaterTemperature (double)
    idfObject.setDouble(Generator_MicroTurbineFields::ReferenceInletWaterTemperature, generatorMCHPHX.referenceInletWaterTemperature());

    
    //HeatRecoveryWaterFlowOperatingMode
    {
      auto value = generatorMCHPHX.heatRecoveryWaterFlowOperatingMode();
      idfObject.setString(Generator_MicroTurbineFields::HeatRecoveryWaterFlowOperatingMode,value);
    }

    //ReferenceHeatRecoveryWaterFlowRate (double)
    idfObject.setDouble(Generator_MicroTurbineFields::ReferenceHeatRecoveryWaterFlowRate, generatorMCHPHX.referenceHeatRecoveryWaterFlowRate());


    //HeatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve
    if( auto curve = generatorMCHPHX.heatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve() )  {
      if( auto _curve = translateAndMapModelObject(curve.get()) ) {
        idfObject.setString(Generator_MicroTurbineFields::HeatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurveName,_curve->name().get());
      }
    }

    //ThermalEfficiencyFunctionofTemperatureandElevationCurve
    if( auto curve = generatorMCHPHX.thermalEfficiencyFunctionofTemperatureandElevationCurve() )  {
      if( auto _curve = translateAndMapModelObject(curve.get()) ) {
        idfObject.setString(Generator_MicroTurbineFields::ThermalEfficiencyFunctionofTemperatureandElevationCurveName,_curve->name().get());
      }
    }

    //HeatRecoveryRateFunctionofPartLoadRatioCurve
    if( auto curve = generatorMCHPHX.heatRecoveryRateFunctionofPartLoadRatioCurve() )  {
      if( auto _curve = translateAndMapModelObject(curve.get()) ) {
        idfObject.setString(Generator_MicroTurbineFields::HeatRecoveryRateFunctionofPartLoadRatioCurveName,_curve->name().get());
      }
    }

    //HeatRecoveryRateFunctionofInletWaterTemperatureCurve
    if( auto curve = generatorMCHPHX.heatRecoveryRateFunctionofInletWaterTemperatureCurve() )  {
      if( auto _curve = translateAndMapModelObject(curve.get()) ) {
        idfObject.setString(Generator_MicroTurbineFields::HeatRecoveryRateFunctionofInletWaterTemperatureCurveName,_curve->name().get());
      }
    }

    //HeatRecoveryRateFunctionofWaterFlowRateCurve
    if( auto curve = generatorMCHPHX.heatRecoveryRateFunctionofWaterFlowRateCurve() )  {
      if( auto _curve = translateAndMapModelObject(curve.get()) ) {
        idfObject.setString(Generator_MicroTurbineFields::HeatRecoveryRateFunctionofWaterFlowRateCurveName,_curve->name().get());
      }
    }

    //MinimumHeatRecoveryWaterFlowRate
    {
      auto value = generatorMCHPHX.minimumHeatRecoveryWaterFlowRate();
      idfObject.setDouble(Generator_MicroTurbineFields::MinimumHeatRecoveryWaterFlowRate,value);
    }

    //MaximumHeatRecoveryWaterFlowRate
    {
      auto value = generatorMCHPHX.maximumHeatRecoveryWaterFlowRate();
      idfObject.setDouble(Generator_MicroTurbineFields::MaximumHeatRecoveryWaterFlowRate,value);
    }

    //MaximumHeatRecoveryWaterTemperature
    if( auto value = generatorMCHPHX.maximumHeatRecoveryWaterTemperature() )
    {
      idfObject.setDouble(Generator_MicroTurbineFields::MaximumHeatRecoveryWaterTemperature,value.get());
    }
  }
  
  
  /// Doubles, non optional
  //ReferenceElectricalPowerOutput
  idfObject.setDouble(Generator_MicroTurbineFields::ReferenceElectricalPowerOutput, modelObject.referenceElectricalPowerOutput());

  //ReferenceElectricalEfficiencyUsingLowerHeatingValue
  idfObject.setDouble(Generator_MicroTurbineFields::ReferenceElectricalEfficiencyUsingLowerHeatingValue, modelObject.referenceElectricalEfficiencyUsingLowerHeatingValue());

  
  /// Doubles, optional
  //MinimumFullLoadElectricalPowerOutput
  {
    auto value = modelObject.minimumFullLoadElectricalPowerOutput();
    idfObject.setDouble(Generator_MicroTurbineFields::MinimumFullLoadElectricalPowerOutput,value);
  }

  //MaximumFullLoadElectricalPowerOutput
  {
    auto value = modelObject.maximumFullLoadElectricalPowerOutput();
    idfObject.setDouble(Generator_MicroTurbineFields::MaximumFullLoadElectricalPowerOutput,value);
  }

  //ReferenceCombustionAirInletTemperature
  {
    auto value = modelObject.referenceCombustionAirInletTemperature();
    idfObject.setDouble(Generator_MicroTurbineFields::ReferenceCombustionAirInletTemperature,value);
  }

  //ReferenceCombustionAirInletHumidityRatio
  {
    auto value = modelObject.referenceCombustionAirInletHumidityRatio();
    idfObject.setDouble(Generator_MicroTurbineFields::ReferenceCombustionAirInletHumidityRatio,value);
  }

  //ReferenceElevation
  {
    auto value = modelObject.referenceElevation();
    idfObject.setDouble(Generator_MicroTurbineFields::ReferenceElevation,value);
  }

  //FuelHigherHeatingValue
  {
    auto value = modelObject.fuelHigherHeatingValue();
    idfObject.setDouble(Generator_MicroTurbineFields::FuelHigherHeatingValue,value);
  }

  //FuelLowerHeatingValue
  {
    auto value = modelObject.fuelLowerHeatingValue();
    idfObject.setDouble(Generator_MicroTurbineFields::FuelLowerHeatingValue,value);
  }

  //StandbyPower
  {
    auto value = modelObject.standbyPower();
    idfObject.setDouble(Generator_MicroTurbineFields::StandbyPower,value);
  }

  //AncillaryPower
  {
    auto value = modelObject.ancillaryPower();
    idfObject.setDouble(Generator_MicroTurbineFields::AncillaryPower,value);
  }

  //ReferenceExhaustAirMassFlowRate
  if( auto value = modelObject.referenceExhaustAirMassFlowRate() )
  {
    idfObject.setDouble(Generator_MicroTurbineFields::ReferenceExhaustAirMassFlowRate,value.get());
  }
  
  //NominalExhaustAirOutletTemperature
  if( auto value = modelObject.nominalExhaustAirOutletTemperature() )
  {
    idfObject.setDouble(Generator_MicroTurbineFields::NominalExhaustAirOutletTemperature,value.get());
  }
  
  /// Choice
  //FuelType
  {
    auto s = modelObject.fuelType();
    idfObject.setString(Generator_MicroTurbineFields::FuelType,s);
  }
  
  
  /// Nodes
  
  // Will leave blank to assume outside air conditions
  //CombustionAirInletNodeName
  idfObject.setString(Generator_MicroTurbineFields::CombustionAirInletNodeName,"");

  //CombustionAirOutletNodeName
  idfObject.setString(Generator_MicroTurbineFields::CombustionAirOutletNodeName,"");

  /// Curves, Not Optional
  //ElectricalPowerFunctionofTemperatureandElevationCurve
  {
    auto curve = modelObject.electricalPowerFunctionofTemperatureandElevationCurve();
    if( auto _curve = translateAndMapModelObject(curve) ) {
      idfObject.setString(Generator_MicroTurbineFields::ElectricalPowerFunctionofTemperatureandElevationCurveName, _curve->name().get());
    }
  }

  //ElectricalEfficiencyFunctionofTemperatureCurve
  {
    auto curve = modelObject.electricalEfficiencyFunctionofTemperatureCurve();
    if( auto _curve = translateAndMapModelObject(curve) ) {
      idfObject.setString(Generator_MicroTurbineFields::ElectricalEfficiencyFunctionofTemperatureCurveName, _curve->name().get());
    }
  }

  //ElectricalEfficiencyFunctionofPartLoadRatioCurve
  {
    auto curve = modelObject.electricalEfficiencyFunctionofPartLoadRatioCurve();
    if( auto _curve = translateAndMapModelObject(curve) ) {
      idfObject.setString(Generator_MicroTurbineFields::ElectricalEfficiencyFunctionofPartLoadRatioCurveName, _curve->name().get());
    }
  }
  /// Curves, Optional

  //AncillaryPowerFunctionofFuelInputCurve
  if( auto curve = modelObject.ancillaryPowerFunctionofFuelInputCurve() )  {
    if( auto _curve = translateAndMapModelObject(curve.get()) ) {
      idfObject.setString(Generator_MicroTurbineFields::AncillaryPowerFunctionofFuelInputCurveName, _curve->name().get());
    }
  }

  //ExhaustAirFlowRateFunctionofTemperatureCurve
  if( auto curve = modelObject.exhaustAirFlowRateFunctionofTemperatureCurve() )  {
    if( auto _curve = translateAndMapModelObject(curve.get()) ) {
      idfObject.setString(Generator_MicroTurbineFields::ExhaustAirFlowRateFunctionofTemperatureCurveName, _curve->name().get());
    }
  }

  //ExhaustAirFlowRateFunctionofPartLoadRatioCurve
  if( auto curve = modelObject.exhaustAirFlowRateFunctionofPartLoadRatioCurve() )  {
    if( auto _curve = translateAndMapModelObject(curve.get()) ) {
      idfObject.setString(Generator_MicroTurbineFields::ExhaustAirFlowRateFunctionofPartLoadRatioCurveName,_curve->name().get());
    }
  }

  //ExhaustAirTemperatureFunctionofTemperatureCurve
  if( auto curve = modelObject.exhaustAirTemperatureFunctionofTemperatureCurve() )  {
    if( auto _curve = translateAndMapModelObject(curve.get()) ) {
      idfObject.setString(Generator_MicroTurbineFields::ExhaustAirTemperatureFunctionofTemperatureCurveName, _curve->name().get());
    }
  }

  //ExhaustAirTemperatureFunctionofPartLoadRatioCurve 
  if( auto curve = modelObject.exhaustAirTemperatureFunctionofPartLoadRatioCurve() )  {
    if( auto _curve = translateAndMapModelObject(curve.get()) ) {
      idfObject.setString(Generator_MicroTurbineFields::ExhaustAirTemperatureFunctionofPartLoadRatioCurveName, _curve->name().get());
    }
  }

  return idfObject;
}

} // energyplus

} // openstudio

