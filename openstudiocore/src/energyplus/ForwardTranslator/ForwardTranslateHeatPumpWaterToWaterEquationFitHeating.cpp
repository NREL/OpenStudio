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
#include "../../model/HeatPumpWaterToWaterEquationFitHeating.hpp"
#include "../../model/HeatPumpWaterToWaterEquationFitHeating_Impl.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/HeatPump_WaterToWater_EquationFit_Heating_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateHeatPumpWaterToWaterEquationFitHeating( HeatPumpWaterToWaterEquationFitHeating & modelObject )
{
  IdfObject idfObject(IddObjectType::HeatPump_WaterToWater_EquationFit_Heating);

  m_idfObjects.push_back(idfObject);

  if( auto s = modelObject.name() ) {
    idfObject.setName(*s);
  }

  if( auto value = modelObject.supplyOutletModelObject() ) {
    idfObject.setString(HeatPump_WaterToWater_EquationFit_HeatingFields::LoadSideOutletNodeName,value->name().get());
  }

  if( auto value = modelObject.supplyInletModelObject() ) {
    idfObject.setString(HeatPump_WaterToWater_EquationFit_HeatingFields::LoadSideInletNodeName,value->name().get());
  }

  if( auto value = modelObject.demandOutletModelObject() ) {
    idfObject.setString(HeatPump_WaterToWater_EquationFit_HeatingFields::SourceSideOutletNodeName,value->name().get());
  }

  if( auto value = modelObject.demandInletModelObject() ) {
    idfObject.setString(HeatPump_WaterToWater_EquationFit_HeatingFields::SourceSideInletNodeName,value->name().get());
  }

  {
    auto value = modelObject.ratedLoadSideFlowRate();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_HeatingFields::RatedLoadSideFlowRate,value);
  }

  {
    auto value = modelObject.ratedSourceSideFlowRate();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_HeatingFields::RatedSourceSideFlowRate,value);
  }

  {
    auto value = modelObject.ratedHeatingCapacity();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_HeatingFields::RatedHeatingCapacity,value);
  }

  {
    auto value = modelObject.ratedHeatingPowerConsumption();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_HeatingFields::RatedHeatingPowerConsumption,value);
  }

  {
    auto value = modelObject.heatingCapacityCoefficient1();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCapacityCoefficient1,value);
  }

  {
    auto value = modelObject.heatingCapacityCoefficient2();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCapacityCoefficient2,value);
  }

  {
    auto value = modelObject.heatingCapacityCoefficient3();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCapacityCoefficient3,value);
  }

  {
    auto value = modelObject.heatingCapacityCoefficient4();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCapacityCoefficient4,value);
  }

  {
    auto value = modelObject.heatingCapacityCoefficient5();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCapacityCoefficient5,value);
  }

  {
    auto value = modelObject.heatingCompressorPowerCoefficient1();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCompressorPowerCoefficient1,value);
  }

  {
    auto value = modelObject.heatingCompressorPowerCoefficient2();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCompressorPowerCoefficient2,value);
  }

  {
    auto value = modelObject.heatingCompressorPowerCoefficient3();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCompressorPowerCoefficient3,value);
  }

  {
    auto value = modelObject.heatingCompressorPowerCoefficient4();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCompressorPowerCoefficient4,value);
  }

  {
    auto value = modelObject.heatingCompressorPowerCoefficient5();
    idfObject.setDouble(HeatPump_WaterToWater_EquationFit_HeatingFields::HeatingCompressorPowerCoefficient5,value);
  }

  return idfObject;
}

} // energyplus

} // openstudio

