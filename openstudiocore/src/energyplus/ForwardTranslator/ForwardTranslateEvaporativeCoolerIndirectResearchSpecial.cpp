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
#include "../../model/EvaporativeCoolerIndirectResearchSpecial.hpp"
#include "../../model/EvaporativeCoolerIndirectResearchSpecial_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/core/Logger.hpp"
#include <utilities/idd/EvaporativeCooler_Indirect_ResearchSpecial_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateEvaporativeCoolerIndirectResearchSpecial( EvaporativeCoolerIndirectResearchSpecial & modelObject )
{
  OptionalString s;
  OptionalDouble d;
  OptionalModelObject temp;

  IdfObject idfObject(IddObjectType::EvaporativeCooler_Indirect_ResearchSpecial);

  m_idfObjects.push_back(idfObject);

  // Name
  s = modelObject.name();
  if(s)
  {
    idfObject.setName(*s);
  }

  // AvailabilityScheduleName
  boost::optional<Schedule> sched = modelObject.availabilitySchedule();
  if( sched )
  {
    boost::optional<IdfObject> _sched = translateAndMapModelObject(sched.get());
    OS_ASSERT(_sched);
    idfObject.setString(EvaporativeCooler_Indirect_ResearchSpecialFields::AvailabilityScheduleName,_sched->name().get());
  }

  // CoolerMaximumEffectiveness
  d = modelObject.coolerMaximumEffectiveness();
  if( d )
  {
    idfObject.setDouble(EvaporativeCooler_Indirect_ResearchSpecialFields::CoolerMaximumEffectiveness,d.get());
  }

  // RecirculatingWaterPumpPowerConsumption
  d = modelObject.recirculatingWaterPumpPowerConsumption();
  if( d )
  {
    idfObject.setDouble(EvaporativeCooler_Indirect_ResearchSpecialFields::RecirculatingWaterPumpPowerConsumption,d.get());
  }

  // SecondaryFanFlowRate
  if( modelObject.isSecondaryFanFlowRateAutosized() )
  {
    idfObject.setString(EvaporativeCooler_Indirect_ResearchSpecialFields::SecondaryFanFlowRate,"Autosize");
  }
  else if( (d = modelObject.secondaryFanFlowRate()) )
  {
    idfObject.setDouble(EvaporativeCooler_Indirect_ResearchSpecialFields::SecondaryFanFlowRate,d.get());
  }

  // SecondaryFanTotalEfficiency
  d = modelObject.secondaryFanTotalEfficiency();
  if( d )
  {
    idfObject.setDouble(EvaporativeCooler_Indirect_ResearchSpecialFields::SecondaryFanTotalEfficiency,d.get());
  }

  // SecondaryFanDeltaPressure
  d = modelObject.secondaryFanDeltaPressure();
  if( d )
  {
    idfObject.setDouble(EvaporativeCooler_Indirect_ResearchSpecialFields::SecondaryFanDeltaPressure,d.get());
  }

  // PrimaryAirInletNodeName
  if( boost::optional<model::ModelObject> mo = modelObject.inletModelObject() )
  {
    OS_ASSERT(mo->optionalCast<model::Node>());
    idfObject.setString(EvaporativeCooler_Indirect_ResearchSpecialFields::PrimaryAirInletNodeName,mo->name().get());
  }

  // PrimaryAirOutletNodeName
  if( boost::optional<model::ModelObject> mo = modelObject.outletModelObject() )
  {
    OS_ASSERT(mo->optionalCast<model::Node>());
    idfObject.setString(EvaporativeCooler_Indirect_ResearchSpecialFields::PrimaryAirOutletNodeName,mo->name().get());
    idfObject.setString(EvaporativeCooler_Indirect_ResearchSpecialFields::SensorNodeName,mo->name().get());
  }

  // DewpointEffectivenessFactor
  d = modelObject.dewpointEffectivenessFactor();
  if( d )
  {
    idfObject.setDouble(EvaporativeCooler_Indirect_ResearchSpecialFields::DewpointEffectivenessFactor,d.get());
  }

  // DriftLossFraction
  d = modelObject.driftLossFraction();
  if( d )
  {
    idfObject.setDouble(EvaporativeCooler_Indirect_ResearchSpecialFields::DriftLossFraction,d.get());
  }

  // BlowdownConcentrationRatio
  d = modelObject.blowdownConcentrationRatio();
  if( d )
  {
    idfObject.setDouble(EvaporativeCooler_Indirect_ResearchSpecialFields::BlowdownConcentrationRatio,d.get());
  }

  // SecondaryAirInletNode
  if( boost::optional<model::Node> node = modelObject.getImpl<model::detail::EvaporativeCoolerIndirectResearchSpecial_Impl>()->reliefAirInletNode() )
  {
    idfObject.setString(EvaporativeCooler_Indirect_ResearchSpecialFields::ReliefAirInletNodeName,node->name().get());
  }

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

