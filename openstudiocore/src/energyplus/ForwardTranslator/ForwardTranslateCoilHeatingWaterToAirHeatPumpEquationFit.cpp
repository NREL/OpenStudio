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
#include <model/CoilHeatingWaterToAirHeatPumpEquationFit.hpp>
#include <model/CoilHeatingWaterToAirHeatPumpEquationFit_Impl.hpp>
#include <utilities/idd/Coil_Heating_WaterToAirHeatPump_EquationFit_FieldEnums.hxx>
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

boost::optional<IdfObject> ForwardTranslator::translateCoilHeatingWaterToAirHeatPumpEquationFit(CoilHeatingWaterToAirHeatPumpEquationFit & modelObject)
{
  //setup boost optionals to use to store get method returns
  boost::optional<std::string> s;
  boost::optional<double> value;
  boost::optional<Node> node;
  
  // Make sure the modelObject gets put into the map, and the new idfObject gets put into the final file.
  // Also sets the idfObjects name

  IdfObject idfObject = createRegisterAndNameIdfObject(IddObjectType::Coil_Heating_WaterToAirHeatPump_EquationFit,modelObject);
  
  // Object Name
  //std::string baseName = idfObject.name().get();

  // Water Inlet Node Name  
  if( boost::optional<ModelObject> mo = modelObject.waterInletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      idfObject.setString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::WaterInletNodeName,node->name().get());
    }
  }
  
  // Water Outlet Node Name
  if( boost::optional<ModelObject> mo = modelObject.waterOutletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      idfObject.setString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::WaterOutletNodeName,node->name().get());
    }
  }
  
  //Air Inlet Node Name
  if( boost::optional<ModelObject> mo = modelObject.airInletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      idfObject.setString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::AirInletNodeName,node->name().get());
    }
  }

  //Air Outlet Node Name
  if( boost::optional<ModelObject> mo = modelObject.airOutletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      idfObject.setString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::AirOutletNodeName,node->name().get());
    }
  }

  //Rated Air Flow Rate
  if( modelObject.isRatedAirFlowRateAutosized())
  {
    idfObject.setString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedAirFlowRate,"Autosize");
  }
  else if((value = modelObject.ratedAirFlowRate()))
  {
    idfObject.setDouble(Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedAirFlowRate,value.get());
  }
  
  //Rated Water Flow Rate
  if( modelObject.isRatedWaterFlowRateAutosized())
  {
    idfObject.setString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedWaterFlowRate,"Autosize");
  }
  else if((value = modelObject.ratedAirFlowRate()))
  {
    idfObject.setDouble(Coil_Heating_WaterToAirHeatPump_EquationFitFields::RatedWaterFlowRate,value.get());
  }  

  
  // Rated Heating Capacity
  if( modelObject.isRatedHeatingCapacityAutosized())
  {
    idfObject.setString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::GrossRatedHeatingCapacity,"Autosize");
  }
  else if((value = modelObject.ratedHeatingCapacity()))
  {
    idfObject.setDouble(Coil_Heating_WaterToAirHeatPump_EquationFitFields::GrossRatedHeatingCapacity,value.get());
  }  

  // Heating Coefficient of Performance
  if((value = modelObject.ratedHeatingCoefficientofPerformance()))
  {
    idfObject.setDouble(Coil_Heating_WaterToAirHeatPump_EquationFitFields::GrossRatedHeatingCOP,value.get());
  }
  
  //Heating Capacity Coefficient 1
  if((value = modelObject.heatingCapacityCoefficient1()))
  {
    idfObject.setDouble(Coil_Heating_WaterToAirHeatPump_EquationFitFields::HeatingCapacityCoefficient1,value.get());
  }
  
  //Heating Capacity Coefficient 2
  if((value = modelObject.heatingCapacityCoefficient2()))
  {
    idfObject.setDouble(Coil_Heating_WaterToAirHeatPump_EquationFitFields::HeatingCapacityCoefficient2,value.get());
  }
  
  //Heating Capacity Coefficient 3
  if((value = modelObject.heatingCapacityCoefficient3()))
  {
    idfObject.setDouble(Coil_Heating_WaterToAirHeatPump_EquationFitFields::HeatingCapacityCoefficient3,value.get());
  }

  //Heating Capacity Coefficient 4
  if((value = modelObject.heatingCapacityCoefficient4()))
  {
    idfObject.setDouble(Coil_Heating_WaterToAirHeatPump_EquationFitFields::HeatingCapacityCoefficient4,value.get());
  }
  
  //Heating Capacity Coefficient 5
  if((value = modelObject.heatingCapacityCoefficient5()))
  {
    idfObject.setDouble(Coil_Heating_WaterToAirHeatPump_EquationFitFields::HeatingCapacityCoefficient5,value.get());
  }

  //Heating Power Consumption Coefficient 1
  if((value = modelObject.heatingPowerConsumptionCoefficient1()))
  {
    idfObject.setDouble(Coil_Heating_WaterToAirHeatPump_EquationFitFields::HeatingPowerConsumptionCoefficient1,value.get());
  }

  //Heating Power Consumption Coefficient 2
  if((value = modelObject.heatingPowerConsumptionCoefficient2()))
  {
    idfObject.setDouble(Coil_Heating_WaterToAirHeatPump_EquationFitFields::HeatingPowerConsumptionCoefficient2,value.get());
  }

  //Heating Power Consumption Coefficient 3
  if((value = modelObject.heatingPowerConsumptionCoefficient3()))
  {
    idfObject.setDouble(Coil_Heating_WaterToAirHeatPump_EquationFitFields::HeatingPowerConsumptionCoefficient3,value.get());
  }

  //Heating Power Consumption Coefficient 4
  if((value = modelObject.heatingPowerConsumptionCoefficient4()))
  {
    idfObject.setDouble(Coil_Heating_WaterToAirHeatPump_EquationFitFields::HeatingPowerConsumptionCoefficient4,value.get());
  }

  //Heating Power Consumption Coefficient 5
  if((value = modelObject.heatingPowerConsumptionCoefficient5()))
  {
    idfObject.setDouble(Coil_Heating_WaterToAirHeatPump_EquationFitFields::HeatingPowerConsumptionCoefficient5,value.get());
  }
 
  return idfObject;
}

} // energyplus

} // openstudio

