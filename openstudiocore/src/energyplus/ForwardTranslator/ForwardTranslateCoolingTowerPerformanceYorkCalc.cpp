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
#include "../../model/CoolingTowerPerformanceYorkCalc.hpp"
#include "../../model/CoolingTowerPerformanceYorkCalc_Impl.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/WorkspaceObjectOrder.hpp"
#include "../../utilities/core/Logger.hpp"
#include <utilities/idd/CoolingTowerPerformance_YorkCalc_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateCoolingTowerPerformanceYorkCalc( CoolingTowerPerformanceYorkCalc & modelObject )
{
  OptionalString s;
  OptionalDouble d;
  OptionalModelObject temp;

  IdfObject idfObject(IddObjectType::CoolingTowerPerformance_YorkCalc);

  m_idfObjects.push_back(idfObject);

  // Name

  s = modelObject.name();
  if(s)
  {
    idfObject.setName(*s);
  }

  // MinimumInletAirWetBulbTemperature

  if( (d = modelObject.minimumInletAirWetBulbTemperature()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_YorkCalcFields::MinimumInletAirWetBulbTemperature,d.get());
  }

  // MaximumInletAirWetBulbTemperature

  if( (d = modelObject.maximumInletAirWetBulbTemperature()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_YorkCalcFields::MaximumInletAirWetBulbTemperature,d.get());
  }

  // MinimumRangeTemperature
  
  if( (d = modelObject.minimumRangeTemperature()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_YorkCalcFields::MinimumRangeTemperature,d.get());
  }

  // MaximumRangeTemperature
  
  if( (d = modelObject.maximumRangeTemperature()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_YorkCalcFields::MaximumRangeTemperature,d.get());
  }

  // MinimumApproachTemperature

  if( (d = modelObject.minimumApproachTemperature()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_YorkCalcFields::MinimumApproachTemperature,d.get());
  }
  
  // MaximumApproachTemperature
  
  if( (d = modelObject.maximumApproachTemperature()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_YorkCalcFields::MaximumApproachTemperature,d.get());
  }

  // MinimumWaterFlowRateRatio

  if( (d = modelObject.minimumWaterFlowRateRatio()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_YorkCalcFields::MinimumWaterFlowRateRatio,d.get());
  }
  
  // MaximumWaterFlowRateRatio
  
  if( (d = modelObject.maximumWaterFlowRateRatio()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_YorkCalcFields::MaximumWaterFlowRateRatio,d.get());
  }

  // MaximumLiquidtoGasRatio

  if( (d = modelObject.maximumLiquidtoGasRatio()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_YorkCalcFields::MaximumLiquidtoGasRatio,d.get());
  }

  // Coefficient1
  
  if( (d = modelObject.coefficient1()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_YorkCalcFields::Coefficient1,d.get());
  }

  // Coefficient2
  
  if( (d = modelObject.coefficient2()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_YorkCalcFields::Coefficient2,d.get());
  }

  // Coefficient3
  
  if( (d = modelObject.coefficient3()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_YorkCalcFields::Coefficient3,d.get());
  }

  // Coefficient4
  
  if( (d = modelObject.coefficient4()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_YorkCalcFields::Coefficient4,d.get());
  }

  // Coefficient5
  
  if( (d = modelObject.coefficient5()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_YorkCalcFields::Coefficient5,d.get());
  }

  // Coefficient6
  
  if( (d = modelObject.coefficient6()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_YorkCalcFields::Coefficient6,d.get());
  }

  // Coefficient7
  
  if( (d = modelObject.coefficient7()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_YorkCalcFields::Coefficient7,d.get());
  }

  // Coefficient8
  
  if( (d = modelObject.coefficient8()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_YorkCalcFields::Coefficient8,d.get());
  }

  // Coefficient9
  
  if( (d = modelObject.coefficient9()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_YorkCalcFields::Coefficient9,d.get());
  }

  // Coefficient10
  
  if( (d = modelObject.coefficient10()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_YorkCalcFields::Coefficient10,d.get());
  }

  // Coefficient11
  
  if( (d = modelObject.coefficient11()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_YorkCalcFields::Coefficient11,d.get());
  }

  // Coefficient12
  
  if( (d = modelObject.coefficient12()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_YorkCalcFields::Coefficient12,d.get());
  }

  // Coefficient13
  
  if( (d = modelObject.coefficient13()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_YorkCalcFields::Coefficient13,d.get());
  }

  // Coefficient14
  
  if( (d = modelObject.coefficient14()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_YorkCalcFields::Coefficient14,d.get());
  }

  // Coefficient15
  
  if( (d = modelObject.coefficient15()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_YorkCalcFields::Coefficient15,d.get());
  }

  // Coefficient16
  
  if( (d = modelObject.coefficient16()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_YorkCalcFields::Coefficient16,d.get());
  }

  // Coefficient17
  
  if( (d = modelObject.coefficient17()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_YorkCalcFields::Coefficient17,d.get());
  }

  // Coefficient18
  
  if( (d = modelObject.coefficient18()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_YorkCalcFields::Coefficient18,d.get());
  }

  // Coefficient19
  
  if( (d = modelObject.coefficient19()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_YorkCalcFields::Coefficient19,d.get());
  }

  // Coefficient20
  
  if( (d = modelObject.coefficient20()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_YorkCalcFields::Coefficient20,d.get());
  }

  // Coefficient21
  
  if( (d = modelObject.coefficient21()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_YorkCalcFields::Coefficient21,d.get());
  }

  // Coefficient22
  
  if( (d = modelObject.coefficient22()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_YorkCalcFields::Coefficient22,d.get());
  }

  // Coefficient23
  
  if( (d = modelObject.coefficient23()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_YorkCalcFields::Coefficient23,d.get());
  }

  // Coefficient24
  
  if( (d = modelObject.coefficient24()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_YorkCalcFields::Coefficient24,d.get());
  }

  // Coefficient25
  
  if( (d = modelObject.coefficient25()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_YorkCalcFields::Coefficient25,d.get());
  }

  // Coefficient26
  
  if( (d = modelObject.coefficient26()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_YorkCalcFields::Coefficient26,d.get());
  }

  // Coefficient27
  
  if( (d = modelObject.coefficient27()) )
  {
    idfObject.setDouble(CoolingTowerPerformance_YorkCalcFields::Coefficient27,d.get());
  }

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

