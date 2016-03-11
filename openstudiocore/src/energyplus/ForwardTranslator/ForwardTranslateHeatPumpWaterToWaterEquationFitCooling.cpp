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
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/HeatPumpWaterToWaterEquationFitCooling.hpp"
#include "../../model/HeatPumpWaterToWaterEquationFitCooling_Impl.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/HeatPump_WaterToWater_EquationFit_Cooling_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateHeatPumpWaterToWaterEquationFitCooling( HeatPumpWaterToWaterEquationFitCooling & modelObject )
{
  IdfObject idfObject(IddObjectType::HeatPump_WaterToWater_EquationFit_Cooling);

  m_idfObjects.push_back(idfObject);

  if( auto s = modelObject.name() ) {
    idfObject.setName(*s);
  }

  if( auto value = modelObject.supplyOutletModelObject() ) {
    idfObject.setString(HeatPump_WaterToWater_EquationFit_CoolingFields::LoadSideOutletNodeName,value->name().get());
  }

  if( auto value = modelObject.supplyInletModelObject() ) {
    idfObject.setString(HeatPump_WaterToWater_EquationFit_CoolingFields::LoadSideInletNodeName,value->name().get());
  }

  if( auto value = modelObject.demandOutletModelObject() ) {
    idfObject.setString(HeatPump_WaterToWater_EquationFit_CoolingFields::SourceSideOutletNodeName,value->name().get());
  }

  if( auto value = modelObject.demandInletModelObject() ) {
    idfObject.setString(HeatPump_WaterToWater_EquationFit_CoolingFields::SourceSideInletNodeName,value->name().get());
  }

  {
    auto value = modelObject.ratedLoadSideFlowRate();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_CoolingFields::RatedLoadSideFlowRate,value);
  }

  {
    auto value = modelObject.ratedSourceSideFlowRate();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_CoolingFields::RatedSourceSideFlowRate,value);
  }

  {
    auto value = modelObject.ratedCoolingCapacity();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_CoolingFields::RatedCoolingCapacity,value);
  }

  {
    auto value = modelObject.ratedCoolingPowerConsumption();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_CoolingFields::RatedCoolingPowerConsumption,value);
  }

  {
    auto value = modelObject.coolingCapacityCoefficient1();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCapacityCoefficient1,value);
  }

  {
    auto value = modelObject.coolingCapacityCoefficient2();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCapacityCoefficient2,value);
  }

  {
    auto value = modelObject.coolingCapacityCoefficient3();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCapacityCoefficient3,value);
  }

  {
    auto value = modelObject.coolingCapacityCoefficient4();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCapacityCoefficient4,value);
  }

  {
    auto value = modelObject.coolingCapacityCoefficient5();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCapacityCoefficient5,value);
  }

  {
    auto value = modelObject.coolingCompressorPowerCoefficient1();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCompressorPowerCoefficient1,value);
  }

  {
    auto value = modelObject.coolingCompressorPowerCoefficient2();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCompressorPowerCoefficient2,value);
  }

  {
    auto value = modelObject.coolingCompressorPowerCoefficient3();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCompressorPowerCoefficient3,value);
  }

  {
    auto value = modelObject.coolingCompressorPowerCoefficient4();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCompressorPowerCoefficient4,value);
  }

  {
    auto value = modelObject.coolingCompressorPowerCoefficient5();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_CoolingFields::CoolingCompressorPowerCoefficient5,value);
  }

  return idfObject;
}

} // energyplus

} // openstudio

