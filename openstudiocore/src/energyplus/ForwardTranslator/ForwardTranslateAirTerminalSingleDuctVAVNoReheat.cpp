/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

// copy from AirTerminalSingleDuctVAVReheat
#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/AirTerminalSingleDuctVAVNoReheat.hpp"
#include "../../model/AirTerminalSingleDuctVAVNoReheat_Impl.hpp"
#include "../../model/HVACComponent.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/DesignSpecificationOutdoorAir.hpp"
#include "../../model/DesignSpecificationOutdoorAir_Impl.hpp"
#include <utilities/idd/AirTerminal_SingleDuct_VAV_NoReheat_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_AirDistributionUnit_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateAirTerminalSingleDuctVAVNoReheat( AirTerminalSingleDuctVAVNoReheat & modelObject )
{
  OptionalModelObject temp;
  OptionalString optS;
  boost::optional<std::string> s;

  std::string baseName = modelObject.name().get();

  IdfObject _airDistributionUnit(openstudio::IddObjectType::ZoneHVAC_AirDistributionUnit);
  _airDistributionUnit.setName("ADU " + baseName ); //ADU: Air Distribution Unit

  IdfObject idfObject(openstudio::IddObjectType::AirTerminal_SingleDuct_VAV_NoReheat);

  idfObject.setName(baseName);

  //HVACComponent coil = modelObject.NoReheatCoil();

  m_idfObjects.push_back(_airDistributionUnit);

  m_idfObjects.push_back(idfObject);

  //boost::optional<IdfObject> _ReheatCoil = translateAndMapModelObject(coil);

 // if( _NoReheatCoil && _NoReheatCoil->name() )
  //{
  //  std::string damperOutletNodeName = modelObject.name().get() + " Damper Outlet";

    boost::optional<std::string> inletNodeName;
    boost::optional<std::string> outletNodeName;

    if( boost::optional<ModelObject> inletModelObject = modelObject.inletModelObject() )
    {
      if( boost::optional<Node> inletNode = inletModelObject->optionalCast<Node>() )
      {
        inletNodeName = inletNode->name().get();
      }
    }

    if( boost::optional<ModelObject> outletModelObject = modelObject.outletModelObject() )
    {
      if( boost::optional<Node> outletNode = outletModelObject->optionalCast<Node>() )
      {
        outletNodeName = outletNode->name().get();
      }
    }

    // Reheat Coil Name
  //  idfObject.setString(AirTerminal_SingleDuct_VAV_NoReheatFields::ReheatCoilName,_reheatCoil->name().get());

    // Reheat Coil Type
    //idfObject.setString(AirTerminal_SingleDuct_VAV_NoReheatFields::ReheatCoilObjectType,_reheatCoil->iddObject().name());

    //idfObject.setString(AirTerminal_SingleDuct_VAV_NoReheatFields::DamperAirOutletNodeName,damperOutletNodeName);

    if( outletNodeName && inletNodeName )
    {
    //  if( _reheatCoil->iddObject().type() == IddObjectType::Coil_Heating_Gas )
    //  {
    //    _reheatCoil->setString(Coil_Heating_GasFields::AirInletNodeName,damperOutletNodeName);
    //    _reheatCoil->setString(Coil_Heating_GasFields::AirOutletNodeName,outletNodeName.get());
    //  }
    //  else if( _reheatCoil->iddObject().type() == IddObjectType::Coil_Heating_Electric )
    //  {
    //    _reheatCoil->setString(Coil_Heating_ElectricFields::AirInletNodeName,damperOutletNodeName);
    //    _reheatCoil->setString(Coil_Heating_ElectricFields::AirOutletNodeName,outletNodeName.get());
    //  }
    //  else if( _reheatCoil->iddObject().type() == IddObjectType::Coil_Heating_Water )
    //  {
    //    _reheatCoil->setString(Coil_Heating_WaterFields::AirInletNodeName,damperOutletNodeName);
    //    _reheatCoil->setString(Coil_Heating_WaterFields::AirOutletNodeName,outletNodeName.get());
    //  }
    //
      idfObject.setString(AirTerminal_SingleDuct_VAV_NoReheatFields::AirOutletNodeName,outletNodeName.get());
      idfObject.setString(AirTerminal_SingleDuct_VAV_NoReheatFields::AirInletNodeName,inletNodeName.get());
    }
  //}
  //else
  //{
    //LOG(Error,modelObject.briefDescription() << ": Could not translate heating coil");

    //return boost::none;
 // }

  // AvailabilityScheduleName
  Schedule availabilitySchedule = modelObject.availabilitySchedule();

  boost::optional<IdfObject> _availabilitySchedule = translateAndMapModelObject(availabilitySchedule);

  if( _availabilitySchedule && _availabilitySchedule->name() )
  {
    idfObject.setString(AirTerminal_SingleDuct_VAV_NoReheatFields::AvailabilityScheduleName,_availabilitySchedule->name().get());
  }

  // MaximumAirFlowRate
  boost::optional<double> value = modelObject.maximumAirFlowRate();
  if( value )
  {
    idfObject.setDouble(AirTerminal_SingleDuct_VAV_NoReheatFields::MaximumAirFlowRate,value.get());
  }
  else if( modelObject.isMaximumAirFlowRateAutosized() )
  {
    idfObject.setString(AirTerminal_SingleDuct_VAV_NoReheatFields::MaximumAirFlowRate,"Autosize");
  }

  // ZoneMinimumAirFlowMethod
  s = modelObject.zoneMinimumAirFlowInputMethod();
  if( s )
  {
    idfObject.setString(AirTerminal_SingleDuct_VAV_NoReheatFields::ZoneMinimumAirFlowInputMethod,s.get());
  }

  // ConstantMinimumAirFlowFraction
  value = modelObject.constantMinimumAirFlowFraction();
  idfObject.setDouble(AirTerminal_SingleDuct_VAV_NoReheatFields::ConstantMinimumAirFlowFraction,value.get());

  // FixedMinimumAirFlowRate
  value = modelObject.fixedMinimumAirFlowRate();
  idfObject.setDouble(AirTerminal_SingleDuct_VAV_NoReheatFields::FixedMinimumAirFlowRate,value.get());

  // MinimumAirFlowFractionScheduleName
  boost::optional<Schedule> minAirFlowFractionSchedule = modelObject.minimumAirFlowFractionSchedule();

  if( minAirFlowFractionSchedule )
  {
    boost::optional<IdfObject> _minAirFlowFractionSchedule = translateAndMapModelObject(minAirFlowFractionSchedule.get());

    if( _minAirFlowFractionSchedule && _minAirFlowFractionSchedule->name() )
    {
      idfObject.setString(AirTerminal_SingleDuct_VAV_NoReheatFields::MinimumAirFlowFractionScheduleName,_minAirFlowFractionSchedule->name().get());
    }
  }

  // ControlForOutdoorAir
  {
    if( modelObject.controlForOutdoorAir() ) {
      if( auto airLoopHVAC = modelObject.airLoopHVAC() ) {
        auto zones = airLoopHVAC->demandComponents(modelObject,airLoopHVAC->demandOutletNode(),model::ThermalZone::iddObjectType());
        if( ! zones.empty() ) {
          auto zone = zones.front();
          auto spaces = zone.cast<model::ThermalZone>().spaces();
          if( ! spaces.empty() ) {
            if( auto designSpecificationOutdoorAir = spaces.front().designSpecificationOutdoorAir() ) {
              idfObject.setString(AirTerminal_SingleDuct_VAV_NoReheatFields::DesignSpecificationOutdoorAirObjectName,
                designSpecificationOutdoorAir->name().get());
            }
          }
        }
      }
    }
  }

  // MaximumHotWaterOrSteamFlowRate
   // value = modelObject.maximumHotWaterOrSteamFlowRate();
   //
   // if( value )
   // {
  //    idfObject.setDouble(AirTerminal_SingleDuct_VAV_NoReheatFields::MaximumHotWaterorSteamFlowRate,value.get());
    //}
   // else if( modelObject.isMaximumHotWaterOrSteamFlowRateAutosized() )
   // {
   //   idfObject.setString(AirTerminal_SingleDuct_VAV_NoReheatFields::MaximumHotWaterorSteamFlowRate,"Autosize");
   // }

  // MinimumHotWaterOrSteamFlowRate
   // value = modelObject.minimumHotWaterOrSteamFlowRate();
   // idfObject.setDouble(AirTerminal_SingleDuct_VAV_NoReheatFields::MinimumHotWaterorSteamFlowRate,value.get());

  // ConvergenceTolerance
  //value = modelObject.convergenceTolerance();
  //idfObject.setDouble(AirTerminal_SingleDuct_VAV_NoReheatFields::ConvergenceTolerance,value.get());

  // DamperHeatingAction
  //  s = modelObject.damperHeatingAction();
   // idfObject.setString(AirTerminal_SingleDuct_VAV_NoReheatFields::DamperHeatingAction,s.get());

  // MaximumFlowPerZoneFloorAreaDuringReheat
    //value = modelObject.maximumFlowPerZoneFloorAreaDuringReheat();

   // if( value )
   // {
   //   idfObject.setDouble(AirTerminal_SingleDuct_VAV_NoReheatFields::MaximumFlowperZoneFloorAreaDuringReheat,value.get());
   // }
  //  else if ( modelObject.isMaximumFlowPerZoneFloorAreaDuringReheatAutosized() )
  //  {
   //   idfObject.setString(AirTerminal_SingleDuct_VAV_NoReheatFields::MaximumFlowperZoneFloorAreaDuringReheat,"Autocalculate");
  //  }

  // MaximumFlowFractionDuringReheat
   // value = modelObject.maximumFlowPerZoneFloorAreaDuringReheat();

    //if( value )
    //{
    //  idfObject.setDouble(AirTerminal_SingleDuct_VAV_NoReheatFields::MaximumFlowFractionDuringReheat,value.get());
   // }
   // else if( modelObject.isMaximumFlowFractionDuringReheatAutosized() )
   // {
   //   idfObject.setString(AirTerminal_SingleDuct_VAV_NoReheatFields::MaximumFlowFractionDuringReheat,"Autocalculate");
   // }

  // MaximumReheatAirTemperature
  //  value = modelObject.maximumReheatAirTemperature();
   // idfObject.setDouble(AirTerminal_SingleDuct_VAV_NoReheatFields::MaximumReheatAirTemperature,value.get());

  // Populate fields for AirDistributionUnit
  if( boost::optional<ModelObject> outletNode = modelObject.outletModelObject() )
  {
    _airDistributionUnit.setString(ZoneHVAC_AirDistributionUnitFields::AirDistributionUnitOutletNodeName,outletNode->name().get());
  }
  _airDistributionUnit.setString(ZoneHVAC_AirDistributionUnitFields::AirTerminalObjectType,idfObject.iddObject().name());
  _airDistributionUnit.setString(ZoneHVAC_AirDistributionUnitFields::AirTerminalName,idfObject.name().get());

  return _airDistributionUnit;
}

} // energyplus

} // openstudio

