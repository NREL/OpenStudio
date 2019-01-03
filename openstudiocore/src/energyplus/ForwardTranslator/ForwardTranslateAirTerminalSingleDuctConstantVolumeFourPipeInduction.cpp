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
#include "../../model/AirTerminalSingleDuctConstantVolumeFourPipeInduction.hpp"
#include "../../model/AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include <utilities/idd/AirTerminal_SingleDuct_ConstantVolume_FourPipeInduction_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_ZoneMixer_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_Water_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Water_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_AirDistributionUnit_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateAirTerminalSingleDuctConstantVolumeFourPipeInduction( AirTerminalSingleDuctConstantVolumeFourPipeInduction & modelObject )
{
  OptionalString s;
  OptionalDouble d;
  OptionalModelObject temp;

  IdfObject _airDistributionUnit(openstudio::IddObjectType::ZoneHVAC_AirDistributionUnit);
  _airDistributionUnit.setName(modelObject.name().get() + " Air Distribution Unit");
  m_idfObjects.push_back(_airDistributionUnit);

  // Name
  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::AirTerminal_SingleDuct_ConstantVolume_FourPipeInduction, modelObject);

  // Availability Schedule Name
  if( (temp = modelObject.availabilitySchedule()) )
  {
    if( boost::optional<IdfObject> _schedule = translateAndMapModelObject(temp.get()) )
    {
      idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::AvailabilityScheduleName,_schedule->name().get());
    }
  }

  // Maximum Total Air Flow Rate
  if( (d = modelObject.maximumTotalAirFlowRate()) )
  {
    idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MaximumTotalAirFlowRate,d.get());
  }
  else if( modelObject.isMaximumTotalAirFlowRateAutosized() )
  {
    idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MaximumTotalAirFlowRate,"Autosize");
  }

  // Induction Ratio
  if( (d = modelObject.inductionRatio()) )
  {
    idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::InductionRatio,d.get());
  }

  // Supply Air Inlet Node Name
  if( auto node = modelObject.inletModelObject() ) {
    idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::SupplyAirInletNodeName,node->name().get());
  }

  // Induced Air Inlet Node Name
  if( auto node = modelObject.inducedAirInletNode() ) {
    idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::InducedAirInletNodeName,node->name().get());
  }

  // Air Outlet Node Name
  if( auto node = modelObject.outletModelObject() ) {
    idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::AirOutletNodeName,node->name().get());
  }

  // Hot Water Inlet Node Name
  // deprecated

  // Cold Water Inlet Node Name
  //deprecated

  // Heating Coil Object Type
  // Heating Coil Name
  boost::optional<IdfObject> _heatingCoil;
  {
    auto heatingCoil = modelObject.heatingCoil();
    if( (_heatingCoil = translateAndMapModelObject(heatingCoil)) ) {
      idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::HeatingCoilObjectType,_heatingCoil->iddObject().name());
      idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::HeatingCoilName,_heatingCoil->name().get());
    }
  }

  // Maximum Hot Water Flow Rate
  if( (d = modelObject.maximumHotWaterFlowRate()) )
  {
    idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MaximumHotWaterFlowRate,d.get());
  }
  else if( modelObject.isMaximumHotWaterFlowRateAutosized() )
  {
    idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MaximumHotWaterFlowRate,"Autosize");
  }

  // Minimum Hot Water Flow Rate
  if( (d = modelObject.minimumHotWaterFlowRate()) )
  {
    idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MinimumHotWaterFlowRate,d.get());
  }

  // Heating Convergence Tolerance
  if( (d = modelObject.heatingConvergenceTolerance()) )
  {
    idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::HeatingConvergenceTolerance,d.get());
  }

  // Cooling Coil Object Type
  // Cooling Coil Name
  boost::optional<IdfObject> _coolingCoil;
  if( auto coolingCoil = modelObject.coolingCoil() ) {
    if( (_coolingCoil = translateAndMapModelObject(coolingCoil.get())) ) {
      idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::CoolingCoilObjectType,_coolingCoil->iddObject().name());
      idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::CoolingCoilName,_coolingCoil->name().get());
    }
  }

  // Maximum Cold Water Flow Rate
  if( (d = modelObject.maximumColdWaterFlowRate()) )
  {
    idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MaximumColdWaterFlowRate,d.get());
  }
  else if( modelObject.isMaximumColdWaterFlowRateAutosized() )
  {
    idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MaximumColdWaterFlowRate,"Autosize");
  }

  // Minimum Cold Water Flow Rate
  if( (d = modelObject.minimumColdWaterFlowRate()) )
  {
    idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::MinimumColdWaterFlowRate,d.get());
  }

  // Cooling Convergence Tolerance
  if( (d = modelObject.coolingConvergenceTolerance()) )
  {
    idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::CoolingConvergenceTolerance,d.get());
  }

  // Zone Mixer Name
  IdfObject _mixer(IddObjectType::AirLoopHVAC_ZoneMixer);
  _mixer.setName(modelObject.name().get() + " Mixer");
  m_idfObjects.push_back(_mixer);
  _mixer.clearExtensibleGroups();
  idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::ZoneMixerName,_mixer.name().get());

  std::string baseName = modelObject.name().get();

  std::string heatingCoilInlet;
  std::string heatingCoilOutlet;
  std::string coolingCoilInlet;
  std::string coolingCoilOutlet;
  std::string mixerAirSystemInlet;
  std::string mixerInducedInlet;
  std::string mixerOutlet;

  if( auto inducedAirInletNode = modelObject.inducedAirInletNode() ) {
    heatingCoilInlet = inducedAirInletNode->name().get();
  }
  heatingCoilOutlet = baseName + " Heating Coil Outlet";

  if( _coolingCoil ) {
    coolingCoilInlet = heatingCoilOutlet;
    coolingCoilOutlet = baseName + " Cooling Coil Outlet";

    mixerInducedInlet = coolingCoilOutlet;
  } else {
    mixerInducedInlet = heatingCoilOutlet;
  }

  if( auto node = modelObject.inletModelObject() ) {
    mixerAirSystemInlet = node->name().get();
  }
  if( auto node = modelObject.outletModelObject() ) {
    mixerOutlet = node->name().get();
  }

  if( _heatingCoil && (_heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Water) ) {
    _heatingCoil->setString(Coil_Heating_WaterFields::AirInletNodeName,heatingCoilInlet);
    _heatingCoil->setString(Coil_Heating_WaterFields::AirOutletNodeName,heatingCoilOutlet);
  }

  if( _coolingCoil && (_coolingCoil->iddObject().type() == IddObjectType::Coil_Cooling_Water) ) {
    _coolingCoil->setString(Coil_Cooling_WaterFields::AirInletNodeName,coolingCoilInlet);
    _coolingCoil->setString(Coil_Cooling_WaterFields::AirOutletNodeName,coolingCoilOutlet);
  }

  _mixer.setString(AirLoopHVAC_ZoneMixerFields::OutletNodeName,mixerOutlet);
  IdfExtensibleGroup eg = _mixer.pushExtensibleGroup();
  eg.setString(AirLoopHVAC_ZoneMixerExtensibleFields::InletNodeName,mixerAirSystemInlet);
  eg = _mixer.pushExtensibleGroup();
  eg.setString(AirLoopHVAC_ZoneMixerExtensibleFields::InletNodeName,mixerInducedInlet);

  if( auto node = modelObject.outletModelObject() ) {
    _airDistributionUnit.setString(ZoneHVAC_AirDistributionUnitFields::AirDistributionUnitOutletNodeName,node->name().get());
  }
  _airDistributionUnit.setString(ZoneHVAC_AirDistributionUnitFields::AirTerminalObjectType,idfObject.iddObject().name());
  _airDistributionUnit.setString(ZoneHVAC_AirDistributionUnitFields::AirTerminalName,idfObject.name().get());

  return _airDistributionUnit;
}

} // energyplus

} // openstudio
