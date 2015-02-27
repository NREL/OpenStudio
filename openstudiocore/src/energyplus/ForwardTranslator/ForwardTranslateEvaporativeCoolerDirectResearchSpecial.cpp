/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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
#include "../../model/EvaporativeCoolerDirectResearchSpecial.hpp"
#include "../../model/EvaporativeCoolerDirectResearchSpecial_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/core/Logger.hpp"
#include <utilities/idd/EvaporativeCooler_Direct_ResearchSpecial_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateEvaporativeCoolerDirectResearchSpecial( EvaporativeCoolerDirectResearchSpecial & modelObject )
{
  OptionalString s;
  OptionalDouble d;
  OptionalModelObject temp;
  double value;

  IdfObject idfObject(IddObjectType::EvaporativeCooler_Direct_ResearchSpecial);

  m_idfObjects.push_back(idfObject);

  // Name
  s = modelObject.name();
  if(s)
  {
    idfObject.setName(*s);
  }

  // AvailabilityScheduleName
  Schedule sched = modelObject.availableSchedule();
  translateAndMapModelObject(sched);
  idfObject.setString(EvaporativeCooler_Direct_ResearchSpecialFields::AvailabilityScheduleName,sched.name().get());

  // CoolerEffectiveness
  value = modelObject.coolerEffectiveness();
  idfObject.setDouble(EvaporativeCooler_Direct_ResearchSpecialFields::CoolerEffectiveness,value);

  // RecirculatingWaterPumpPowerConsumption
  value = modelObject.recirculatingWaterPumpPowerConsumption();
  idfObject.setDouble(EvaporativeCooler_Direct_ResearchSpecialFields::RecirculatingWaterPumpPowerConsumption,value);

  // AirInletNodeName
  temp = modelObject.inletModelObject();
  if(temp)
  {
    s = temp->name();
    if(s)
    {
      idfObject.setString(EvaporativeCooler_Direct_ResearchSpecialFields::AirInletNodeName,*s);
    }
  }

  // AirOutletNodeName
  temp = modelObject.outletModelObject();
  if(temp)
  {
    s = temp->name();
    if(s)
    {
      idfObject.setString(EvaporativeCooler_Direct_ResearchSpecialFields::AirOutletNodeName,*s);
    }
  }

  // SensorNodeName
  temp = modelObject.sensorNode();
  if(temp)
  {
    s = temp->name();
    if(s)
    {
      idfObject.setString(EvaporativeCooler_Direct_ResearchSpecialFields::SensorNodeName,*s);
    }
  }

  // DriftLossFraction
  value = modelObject.driftLossFraction();
  idfObject.setDouble(EvaporativeCooler_Direct_ResearchSpecialFields::DriftLossFraction,value);

  // BlowdownConcentrationRatio
  value = modelObject.blowdownConcentrationRatio();
  if( value < 2 )
  {
    idfObject.setDouble(EvaporativeCooler_Direct_ResearchSpecialFields::BlowdownConcentrationRatio,2.0);
  }
  else
  {
    idfObject.setDouble(EvaporativeCooler_Direct_ResearchSpecialFields::BlowdownConcentrationRatio,value);
  }

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

