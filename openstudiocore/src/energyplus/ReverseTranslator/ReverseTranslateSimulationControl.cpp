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

#include "../ReverseTranslator.hpp"

#include "../../model/SimulationControl.hpp"
#include "../../model/SimulationControl_Impl.hpp"

#include <utilities/idd/SimulationControl_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateSimulationControl( const WorkspaceObject & workspaceObject )
{
  OptionalModelObject result;
  openstudio::model::SimulationControl simCon = m_model.getUniqueModelObject<model::SimulationControl>();
  OptionalString optS = workspaceObject.name();
  if(optS)
  {
    simCon.setName(*optS);
  }

  optS = workspaceObject.getString(SimulationControlFields::DoZoneSizingCalculation);
  if(optS)
  {
    std::string temp=*optS;
    boost::to_lower(temp);
    if( temp == "no")
    {
      simCon.setDoZoneSizingCalculation(false);
    }
    else
    {
      simCon.setDoZoneSizingCalculation(true);
    }
  }

  optS = workspaceObject.getString(SimulationControlFields::DoSystemSizingCalculation);
  if(optS)
  {
    std::string temp=*optS;
    boost::to_lower(temp);
    if( temp == "no")
    {
      simCon.setDoSystemSizingCalculation(false);
    }
    else
    {
      simCon.setDoSystemSizingCalculation(true);
    }
  }

  optS = workspaceObject.getString(SimulationControlFields::DoPlantSizingCalculation);
  if(optS)
  {
    std::string temp=*optS;
    boost::to_lower(temp);
    if( temp == "no")
    {
      simCon.setDoPlantSizingCalculation(false);
    }
    else
    {
      simCon.setDoPlantSizingCalculation(true);
    }
  }

  optS = workspaceObject.getString(SimulationControlFields::RunSimulationforSizingPeriods);
  if(optS)
  {
    std::string temp=*optS;
    boost::to_lower(temp);
    if( temp == "no")
    {
      simCon.setRunSimulationforSizingPeriods(false);
    }
    else
    {
      simCon.setRunSimulationforSizingPeriods(true);
    }
  }

  optS = workspaceObject.getString(SimulationControlFields::RunSimulationforWeatherFileRunPeriods);
  if(optS)
  {
    std::string temp=*optS;
    boost::to_lower(temp);
    if( temp == "no")
    {
      simCon.setRunSimulationforWeatherFileRunPeriods(false);
    }
    else
    {
      simCon.setRunSimulationforWeatherFileRunPeriods(true);
    }
  }

  result = simCon;
  return result;
}

} // energyplus

} // openstudio

