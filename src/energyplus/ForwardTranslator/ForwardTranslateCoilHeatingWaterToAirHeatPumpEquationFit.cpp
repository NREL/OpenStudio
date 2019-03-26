/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/CoilHeatingWaterToAirHeatPumpEquationFit.hpp"
#include "../../model/CoilHeatingWaterToAirHeatPumpEquationFit_Impl.hpp"
#include <utilities/idd/Coil_Heating_WaterToAirHeatPump_EquationFit_FieldEnums.hxx>
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"

#include "../../utilities/idd/IddEnums.hpp"
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
  else if((value = modelObject.ratedWaterFlowRate()))
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

