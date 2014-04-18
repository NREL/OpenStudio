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

#include <energyplus/ForwardTranslator.hpp>
#include <model/Model.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/CoilCoolingWaterToAirHeatPumpEquationFit.hpp>
#include <model/CoilCoolingWaterToAirHeatPumpEquationFit_Impl.hpp>
#include <utilities/idd/Coil_Cooling_WaterToAirHeatPump_EquationFit_FieldEnums.hxx>
#include <model/Curve.hpp>
#include <model/Curve_Impl.hpp>
#include <utilities/core/Logger.hpp>
#include <utilities/core/Assert.hpp>

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

//using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateCoilCoolingWaterToAirHeatPumpEquationFit( CoilCoolingWaterToAirHeatPumpEquationFit & modelObject )
{
  //setup boost optionals to use to store get method returns
  boost::optional<std::string> s;
  boost::optional<double> value;
  boost::optional<Node> node;
  
  // Make sure the modelObject gets ut into the map, and the new idfObject gets put into the final file.
  // Also sets the idfObjects name

  IdfObject idfObject = createRegisterAndNameIdfObject(IddObjectType::Coil_Cooling_WaterToAirHeatPump_EquationFit,modelObject);
  
  // Object Name
  //std::string baseName = idfObject.name().get();

  //  A3 ,Field Water Inlet Node Name
  
 if( boost::optional<ModelObject> mo = modelObject.waterInletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      idfObject.setString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::WaterInletNodeName,node->name().get());
    }
  }
  
  // A4, Field Water Outlet Node Name
  
  if( boost::optional<ModelObject> mo = modelObject.waterOutletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      idfObject.setString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::WaterOutletNodeName,node->name().get());
    }
  }
  
  // A5, Field Air Inlet Node Name
  
  if( boost::optional<ModelObject> mo = modelObject.airInletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      idfObject.setString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::AirInletNodeName,node->name().get());
    }
  }

  // A6 , \field Air Outlet Node Name
  
  if( boost::optional<ModelObject> mo = modelObject.airOutletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      idfObject.setString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::AirOutletNodeName,node->name().get());
    }
  }

  // N1 Field Rated Air Flow Rate
  
  value = modelObject.ratedAirFlowRate();
  
  if( value )
  {
    idfObject.setDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedAirFlowRate,*value);
  }
  
  else
  {
    idfObject.setString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedAirFlowRate,"Autosize");
  }
  
  //  N2 Rated Water Flow Rate
  
  value = modelObject.ratedWaterFlowRate();
  
  if( value )
  {
    idfObject.setDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedWaterFlowRate,*value);
  }
  
  else
  {
    idfObject.setString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedWaterFlowRate,"Autosize");
  }
  
  // N3,  Field Rated Total Cooling Capacity
  
  value = modelObject.ratedTotalCoolingCapacity();
  
  if( value )
  {
    idfObject.setDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::GrossRatedTotalCoolingCapacity,*value);
  }
  
  else
  {
    idfObject.setString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::GrossRatedTotalCoolingCapacity,"Autosize");
  }
  
  // N4,  Field Rated Sensible Cooling Capacity
  
  value = modelObject.ratedSensibleCoolingCapacity();
  
  if( value )
  {
    idfObject.setDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::GrossRatedSensibleCoolingCapacity,*value);
  }
  
  else
  {
    idfObject.setString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::GrossRatedSensibleCoolingCapacity,"Autosize");
  }
  
  //   N5, Field Rated Cooling Coefficient of Performance

  value = modelObject.ratedCoolingCoefficientofPerformance();
  
  if( value )
  {
    idfObject.setDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::GrossRatedCoolingCOP,*value);
  }
  
  // N6,  Field Total Cooling Capacity Coefficient 1

  value = modelObject.totalCoolingCapacityCoefficient1();
  
  if( value )
  {
    idfObject.setDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::TotalCoolingCapacityCoefficient1,*value);
  }
  
  // N7,  Field Total Cooling Capacity Coefficient 2

  value = modelObject.totalCoolingCapacityCoefficient2();
  
  if( value )
  {
    idfObject.setDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::TotalCoolingCapacityCoefficient2,*value);
  }
  
  // N8,  Field Total Cooling Capacity Coefficient 3

  value = modelObject.totalCoolingCapacityCoefficient3();
  
  if( value )
  {
    idfObject.setDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::TotalCoolingCapacityCoefficient3,*value);
  }
  
  // N9,  Field Total Cooling Capacity Coefficient 4

  value = modelObject.totalCoolingCapacityCoefficient4();
  
  if( value )
  {
    idfObject.setDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::TotalCoolingCapacityCoefficient4,*value);
  }
  
  // N10,  Field Total Cooling Capacity Coefficient 5

  value = modelObject.totalCoolingCapacityCoefficient5();
  
  if( value )
  {
    idfObject.setDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::TotalCoolingCapacityCoefficient5,*value);
  }
  
  // N11,  Sensible Cooling Capacity Coefficient 1

  value = modelObject.sensibleCoolingCapacityCoefficient1();
  
  if( value )
  {
    idfObject.setDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::SensibleCoolingCapacityCoefficient1,*value);
  }
  
  // N12,  Sensible Cooling Capacity Coefficient 2

  value = modelObject.sensibleCoolingCapacityCoefficient2();
  
  if( value )
  {
    idfObject.setDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::SensibleCoolingCapacityCoefficient2,*value);
  }
  
   // N13,  Sensible Cooling Capacity Coefficient 3

  value = modelObject.sensibleCoolingCapacityCoefficient3();
  
  if( value )
  {
    idfObject.setDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::SensibleCoolingCapacityCoefficient3,*value);
  }
  
   // N14,  Sensible Cooling Capacity Coefficient 4

  value = modelObject.sensibleCoolingCapacityCoefficient4();
  
  if( value )
  {
    idfObject.setDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::SensibleCoolingCapacityCoefficient4,*value);
  }
  
   // N15,  Sensible Cooling Capacity Coefficient 5

  value = modelObject.sensibleCoolingCapacityCoefficient5();
  
  if( value )
  {
    idfObject.setDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::SensibleCoolingCapacityCoefficient5,*value);
  }
  
   // N16,  Sensible Cooling Capacity Coefficient 6

  value = modelObject.sensibleCoolingCapacityCoefficient6();
  
  if( value )
  {
    idfObject.setDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::SensibleCoolingCapacityCoefficient6,*value);
  }
  
  // N17,  Sensible Power Consumption Coefficient 1

  value = modelObject.coolingPowerConsumptionCoefficient1();
  
  if( value )
  {
    idfObject.setDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::CoolingPowerConsumptionCoefficient1,*value);
  }
  
  // N18,  Sensible Power Consumption Coefficient 2

  value = modelObject.coolingPowerConsumptionCoefficient2();
  
  if( value )
  {
    idfObject.setDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::CoolingPowerConsumptionCoefficient2,*value);
  }
  
  // N19,  Sensible Power Consumption Coefficient 3

  value = modelObject.coolingPowerConsumptionCoefficient3();
  
  if( value )
  {
    idfObject.setDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::CoolingPowerConsumptionCoefficient3,*value);
  }
  
  // N20,  Sensible Power Consumption Coefficient 4

  value = modelObject.coolingPowerConsumptionCoefficient4();
  
  if( value )
  {
    idfObject.setDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::CoolingPowerConsumptionCoefficient4,*value);
  }
  
  // N21,  Sensible Power Consumption Coefficient 5

  value = modelObject.coolingPowerConsumptionCoefficient5();
  
  if( value )
  {
    idfObject.setDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::CoolingPowerConsumptionCoefficient5,*value);
  }
  
  // N22, Field Nominal Time for Condensate Removal to Begin
  
  value = modelObject.nominalTimeforCondensateRemovaltoBegin();
  
  if( value )
  {
    idfObject.setDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::NominalTimeforCondensateRemovaltoBegin,*value);
  }
  
  //  N23, Field Ratio of Initial Moisture Evaporation Rate and Steady State Latent
  
  value = modelObject.ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity();
  
  if( value )
  {
    idfObject.setDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity,*value);
  }

  return idfObject;
}

} // energyplus

} // openstudio

