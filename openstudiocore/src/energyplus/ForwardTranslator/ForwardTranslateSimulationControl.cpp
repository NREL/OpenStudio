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
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVAC_Impl.hpp"
#include "../../model/PlantLoop.hpp"
#include "../../model/PlantLoop_Impl.hpp"
#include "../../model/SimulationControl.hpp"
#include "../../model/SizingPeriod.hpp"
#include "../../model/SizingPeriod_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include <utilities/idd/SimulationControl_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateSimulationControl( SimulationControl& modelObject )
{
  IdfObject simCon( openstudio::IddObjectType::SimulationControl );

  m_idfObjects.push_back(simCon);

  OptionalString s = modelObject.name();
  if( s )
  {
    simCon.setName(*s);
  }

  unsigned numSizingPeriods = modelObject.model().getModelObjects<SizingPeriod>().size();

  if( modelObject.doZoneSizingCalculation() )
  {
    simCon.setString(openstudio::SimulationControlFields::DoZoneSizingCalculation,"Yes");
  }
  else if( (numSizingPeriods > 0) && (modelObject.model().getConcreteModelObjects<ThermalZone>().size() > 0) )
  {
    simCon.setString(openstudio::SimulationControlFields::DoZoneSizingCalculation,"Yes");
  }
  else
  {
    simCon.setString(openstudio::SimulationControlFields::DoZoneSizingCalculation,"No");
  }

  if( modelObject.doSystemSizingCalculation() )
  {
    simCon.setString(openstudio::SimulationControlFields::DoSystemSizingCalculation,"Yes");
  }
  else if( (numSizingPeriods > 0) && (modelObject.model().getConcreteModelObjects<AirLoopHVAC>().size() > 0) )
  {
    simCon.setString(openstudio::SimulationControlFields::DoSystemSizingCalculation,"Yes");
  }
  else
  {
    simCon.setString(openstudio::SimulationControlFields::DoSystemSizingCalculation,"No");
  }

  if( modelObject.doPlantSizingCalculation() )
  {
    simCon.setString(openstudio::SimulationControlFields::DoPlantSizingCalculation,"Yes");
  }
  else if( (numSizingPeriods > 0) && (modelObject.model().getConcreteModelObjects<PlantLoop>().size() > 0) )
  {
    simCon.setString(openstudio::SimulationControlFields::DoPlantSizingCalculation,"Yes");
  }
  else
  {
    simCon.setString(openstudio::SimulationControlFields::DoPlantSizingCalculation,"No");
  }

  if( modelObject.runSimulationforSizingPeriods() )
  {
    simCon.setString(openstudio::SimulationControlFields::RunSimulationforSizingPeriods,"Yes");
  }
  else
  {
    simCon.setString(openstudio::SimulationControlFields::RunSimulationforSizingPeriods,"No");
  }

  // DLM: might want to check for weather file object?
  if( modelObject.runSimulationforWeatherFileRunPeriods() )
  {
    simCon.setString(openstudio::SimulationControlFields::RunSimulationforWeatherFileRunPeriods,"Yes");
  }
  else
  {
    simCon.setString(openstudio::SimulationControlFields::RunSimulationforWeatherFileRunPeriods,"No");
  }

  // other fields mapped to Building

  return boost::optional<IdfObject>(simCon);
}

} // energyplus

} // openstudio

