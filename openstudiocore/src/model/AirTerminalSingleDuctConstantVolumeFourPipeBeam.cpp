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

#include "AirTerminalSingleDuctConstantVolumeFourPipeBeam.hpp"
#include "AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl.hpp"

#include "CoilCoolingFourPipeBeam.hpp"
#include "CoilCoolingFourPipeBeam_Impl.hpp"
#include "CoilHeatingFourPipeBeam.hpp"
#include "CoilHeatingFourPipeBeam_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "PortList.hpp"
#include "PortList_Impl.hpp"
#include "HVACComponent.hpp"
#include "HVACComponent_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "AirLoopHVACZoneSplitter.hpp"
#include "AirLoopHVACZoneSplitter_Impl.hpp"
#include "AirLoopHVACZoneMixer.hpp"
#include "AirLoopHVACZoneMixer_Impl.hpp"


#include "../../model/ScheduleTypeLimits.hpp"
#include "../../model/ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeam_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl(const IdfObject& idfObject,
                                                                                                             Model_Impl* model,
                                                                                                             bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AirTerminalSingleDuctConstantVolumeFourPipeBeam::iddObjectType());
  }

  AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                             Model_Impl* model,
                                                                                                             bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AirTerminalSingleDuctConstantVolumeFourPipeBeam::iddObjectType());
  }

  AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl(const AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl& other,
                                                                                                             Model_Impl* model,
                                                                                                             bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result{
      // These apply to all AirTerminals
      "Zone Air Terminal Sensible Heating Energy",
      "Zone Air Terminal Sensible Heating Rate",
      "Zone Air Terminal Sensible Cooling Energy",
      "Zone Air Terminal Sensible Cooling Rate",

      // Object specific output variables
      "Zone Air Terminal Beam Sensible Cooling Rate",
      "Zone Air Terminal Beam Sensible Cooling Energy",
      "Zone Air Terminal Beam Sensible Heating Rate",
      "Zone Air Terminal Beam Sensible Heating Energy",
      "Zone Air Terminal Primary Air Sensible Cooling Rate",
      "Zone Air Terminal Primary Air Sensible Cooling Energy",
      "Zone Air Terminal Primary Air Sensible Heating Rate",
      "Zone Air Terminal Primary Air Sensible Heating Energy",
      "Zone Air Terminal Primary Air Flow Rate",
    };
    return result;
  }

  IddObjectType AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::iddObjectType() const {
    return AirTerminalSingleDuctConstantVolumeFourPipeBeam::iddObjectType();
  }

  std::vector<ScheduleTypeKey> AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::PrimaryAirAvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("AirTerminalSingleDuctConstantVolumeFourPipeBeam","Primary Air Availability"));
    }
    if (std::find(b,e,OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::CoolingAvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("AirTerminalSingleDuctConstantVolumeFourPipeBeam","Cooling Availability"));
    }
    if (std::find(b,e,OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::HeatingAvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("AirTerminalSingleDuctConstantVolumeFourPipeBeam","Heating Availability"));
    }
    return result;
  }

  std::vector<IdfObject> AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::remove()
  {
    Model _model = this->model();
    ModelObject thisObject = this->getObject<ModelObject>();

    boost::optional<PlantLoop> loop;
    boost::optional<HVACComponent> _coolingCoil = this->coolingCoil();
    boost::optional<HVACComponent> _heatingCoil = this->heatingCoil();

    boost::optional<ModelObject> sourceModelObject = this->inletModelObject();
    boost::optional<unsigned> sourcePort = this->connectedObjectPort(this->inletPort());

    boost::optional<ModelObject> targetModelObject = this->outletModelObject();
    boost::optional<unsigned> targetPort = this->connectedObjectPort(this->outletPort());

    // Remove from any ZoneHVACEquipmentList
    std::vector<ThermalZone> thermalZones = _model.getConcreteModelObjects<ThermalZone>();
    for( auto & thermalZone : thermalZones )
    {
      std::vector<ModelObject> equipment = thermalZone.equipment();

      if( std::find(equipment.begin(),equipment.end(),thisObject) != equipment.end() )
      {
        thermalZone.removeEquipment(thisObject);
        break;
      }
    }

    if( sourcePort && sourceModelObject
        && targetPort && targetModelObject )
    {
      if( boost::optional<Node> inletNode = sourceModelObject->optionalCast<Node>() )
      {
        if( boost::optional<ModelObject> source2ModelObject = inletNode->inletModelObject() )
        {
          if( boost::optional<unsigned> source2Port = inletNode->connectedObjectPort(inletNode->inletPort()) )
          {
            _model.connect( source2ModelObject.get(),
                            source2Port.get(),
                            targetModelObject.get(),
                            targetPort.get() );

            inletNode->disconnect();
            inletNode->remove();

            if( _coolingCoil && ( loop = _coolingCoil->plantLoop() ) )
            {
              loop->removeDemandBranchWithComponent(*_coolingCoil);
            }

            if( _heatingCoil && ( loop = _heatingCoil->plantLoop() ) )
            {
              loop->removeDemandBranchWithComponent(*_heatingCoil);
            }

            return StraightComponent_Impl::remove();
          }
        }
      }
    }

    model().disconnect(thisObject, inletPort());
    model().disconnect(thisObject, outletPort());

    if ( _coolingCoil && ( loop = _coolingCoil->plantLoop() ) )
    {
      loop->removeDemandBranchWithComponent(*_coolingCoil);
    }

    if ( _heatingCoil && ( loop = _heatingCoil->plantLoop() ) )
    {
      loop->removeDemandBranchWithComponent(*_heatingCoil);
    }

    return StraightComponent_Impl::remove();
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::isRemovable() const
  {
    return true;
  }

  /* Clone this + any cooling and heating coils */
  ModelObject AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::clone(Model model) const
  {
    AirTerminalSingleDuctConstantVolumeFourPipeBeam airTerminalCVFourPipeBeamClone = StraightComponent_Impl::clone(model).cast<AirTerminalSingleDuctConstantVolumeFourPipeBeam>();

    if (boost::optional<HVACComponent> cc = coolingCoil()) {
      HVACComponent coilCoolingClone = cc->clone(model).cast<HVACComponent>();
      airTerminalCVFourPipeBeamClone.setCoolingCoil(coilCoolingClone);
    }

    if (boost::optional<HVACComponent> hc = heatingCoil()) {
      HVACComponent coilHeatingClone = hc->clone(model).cast<HVACComponent>();
      airTerminalCVFourPipeBeamClone.setHeatingCoil(coilHeatingClone);
    }

    return airTerminalCVFourPipeBeamClone;
  }



  /* Children are the (optional) subclasses corresponding to the cooling and heating water side */
  std::vector<ModelObject> AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::children() const
  {
    std::vector<ModelObject> result;
    if (boost::optional<HVACComponent> cc = coolingCoil())
    {
      result.push_back(*cc);
    }
    if (boost::optional<HVACComponent> hc = heatingCoil())
    {
      result.push_back(*hc);
    }
    return result;
  }

  unsigned AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::inletPort() const
  {
    return OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::PrimaryAirInletNodeName;
  }

  unsigned AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::outletPort() const
  {
    return OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::PrimaryAirOutletNodeName;
  }

  /* Only allow connection on a demand side branch of an AirLoopHVAC */
  bool AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::addToNode(Node & node)
  {
    Model _model = node.model();

    if( boost::optional<ModelObject> outlet = node.outletModelObject() )
    {
      boost::optional<ThermalZone> thermalZone;

      if( boost::optional<PortList> portList = outlet->optionalCast<PortList>()  )
      {
        thermalZone = portList->thermalZone();
      }

      if( thermalZone || (outlet->optionalCast<Mixer>() && node.airLoopHVAC()) )
      {
        if( boost::optional<ModelObject> inlet = node.inletModelObject() )
        {
          if( boost::optional<Splitter> splitter = inlet->optionalCast<Splitter>() )
          {
            boost::optional<ModelObject> sourceModelObject = inlet;
            boost::optional<unsigned> sourcePort = node.connectedObjectPort(node.inletPort());

            if( sourcePort && sourceModelObject )
            {
              Node inletNode(_model);

              _model.connect( sourceModelObject.get(),
                              sourcePort.get(),
                              inletNode,
                              inletNode.inletPort() );

              _model.connect( inletNode,
                              inletNode.outletPort(),
                              this->getObject<ModelObject>(),
                              this->inletPort() );

              _model.connect( this->getObject<ModelObject>(),
                              outletPort(),
                              node,
                              node.inletPort() );

              if( thermalZone )
              {
                AirTerminalSingleDuctConstantVolumeFourPipeBeam mo = this->getObject<AirTerminalSingleDuctConstantVolumeFourPipeBeam>();

                thermalZone->addEquipment(mo);
              }

              return true;
            }
          }
        }
      }
    }

    return false;
  }





  /* Primary Air Availability */
  Schedule AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::primaryAirAvailabilitySchedule() const {
    boost::optional<Schedule> value = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::PrimaryAirAvailabilityScheduleName);
    if(!value){
      value = this->model().alwaysOnDiscreteSchedule();
      OS_ASSERT(value);
      const_cast<AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl*>(this)->setPrimaryAirAvailabilitySchedule(*value);
    }
    OS_ASSERT(value);
    return value.get();
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::setPrimaryAirAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::PrimaryAirAvailabilityScheduleName,
                              "AirTerminalSingleDuctConstantVolumeFourPipeBeam",
                              "Primary Air Availability",
                              schedule);
    return result;
  }

  /* Cooling Availability */
  Schedule AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::coolingAvailabilitySchedule() const {
    boost::optional<Schedule> value = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::CoolingAvailabilityScheduleName);
    if(!value){
      value = this->model().alwaysOnDiscreteSchedule();
      OS_ASSERT(value);
      const_cast<AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl*>(this)->setCoolingAvailabilitySchedule(*value);
    }
    OS_ASSERT(value);
    return value.get();
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::setCoolingAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::CoolingAvailabilityScheduleName,
                              "AirTerminalSingleDuctConstantVolumeFourPipeBeam",
                              "Cooling Availability",
                              schedule);
    return result;
  }

  /* Heating Availability */
  Schedule AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::heatingAvailabilitySchedule() const {
    boost::optional<Schedule> value = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::HeatingAvailabilityScheduleName);
    if(!value){
      value = this->model().alwaysOnDiscreteSchedule();
      OS_ASSERT(value);
      const_cast<AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl*>(this)->setHeatingAvailabilitySchedule(*value);
    }
    OS_ASSERT(value);
    return value.get();
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::setHeatingAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::HeatingAvailabilityScheduleName,
                              "AirTerminalSingleDuctConstantVolumeFourPipeBeam",
                              "Heating Availability",
                              schedule);
    return result;
  }



  /* Air Nodes */
  boost::optional<Node> AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::primaryAirInletNode() const {
    unsigned port = inletPort();
    boost::optional<ModelObject> mo = connectedObject(port);
    boost::optional<Node> result;
    if( mo )
    {
      if( boost::optional<Node> node = mo->optionalCast<Node>() )
      {
        result = node;
      }
    }
    return result;
  }

  boost::optional<Node> AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::primaryAirOutletNode() const {
    unsigned port = outletPort();
    boost::optional<ModelObject> mo = connectedObject(port);
    boost::optional<Node> result;

    if( mo )
    {
      if( boost::optional<Node> node = mo->optionalCast<Node>() )
      {
        result = node;
      }
    }

    return result;
  }



  /* Coils */

  /* Cooling Coil */
  boost::optional<HVACComponent> AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::coolingCoil() const {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::CoolingCoilName);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::setCoolingCoil(const HVACComponent& coilCoolingFourPipeBeam) {
    bool result = setPointer(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::CoolingCoilName, coilCoolingFourPipeBeam.handle());
    if (!result) {
      if ( !coilCoolingFourPipeBeam.optionalCast<CoilCoolingFourPipeBeam>() ) {
        LOG(Error, "Cannot set the cooling coil to something else than CoilCoolingFourPipeBeam for " << briefDescription());
      }
    }
    return result;
  }

  /* Heating Coil */
  boost::optional<HVACComponent> AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::heatingCoil() const {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::HeatingCoilName);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::setHeatingCoil(const HVACComponent& coilHeatingFourPipeBeam) {
    bool result = setPointer(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::HeatingCoilName, coilHeatingFourPipeBeam.handle());
    if (!result) {
      if ( !coilHeatingFourPipeBeam.optionalCast<CoilHeatingFourPipeBeam>() ) {
        LOG(Error, "Cannot set the heating coil to something else than CoilHeatingFourPipeBeam for " << briefDescription());
      }
    }
    return result;
  }



  /* Autosizable Fields */


  // Design Primary Air Volume Flow Rate

  boost::optional<double> AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::designPrimaryAirVolumeFlowRate() const {
    return getDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::DesignPrimaryAirVolumeFlowRate,true);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::isDesignPrimaryAirVolumeFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::DesignPrimaryAirVolumeFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional <double> AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::autosizedDesignPrimaryAirVolumeFlowRate() {
    return getAutosizedValue("Supply Air Flow Rate", "m3/s");
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::setDesignPrimaryAirVolumeFlowRate(double designPrimaryAirVolumeFlowRate) {
    bool result = setDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::DesignPrimaryAirVolumeFlowRate, designPrimaryAirVolumeFlowRate);
    return result;
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::autosizeDesignPrimaryAirVolumeFlowRate() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::DesignPrimaryAirVolumeFlowRate, "autosize");
    OS_ASSERT(result);
  }



  // Design Chilled Water Volume Flow Rate

  boost::optional<double> AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::designChilledWaterVolumeFlowRate() const {
    return getDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::DesignChilledWaterVolumeFlowRate,true);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::isDesignChilledWaterVolumeFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::DesignChilledWaterVolumeFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional <double> AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::autosizedDesignChilledWaterVolumeFlowRate() {
    return getAutosizedValue("Maximum Total Chilled Water Flow Rate", "m3/s");
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::setDesignChilledWaterVolumeFlowRate(double designChilledWaterVolumeFlowRate) {
    bool result = setDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::DesignChilledWaterVolumeFlowRate, designChilledWaterVolumeFlowRate);
    return result;
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::autosizeDesignChilledWaterVolumeFlowRate() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::DesignChilledWaterVolumeFlowRate, "autosize");
    OS_ASSERT(result);
  }



  // Design Hot Water Volume Flow Rate

  boost::optional<double> AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::designHotWaterVolumeFlowRate() const {
    return getDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::DesignHotWaterVolumeFlowRate,true);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::isDesignHotWaterVolumeFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::DesignHotWaterVolumeFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional <double> AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::autosizedDesignHotWaterVolumeFlowRate() {
    return getAutosizedValue("Maximum Total Hot Water Flow Rate", "m3/s");
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::setDesignHotWaterVolumeFlowRate(double designHotWaterVolumeFlowRate) {
    bool result = setDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::DesignHotWaterVolumeFlowRate, designHotWaterVolumeFlowRate);
    return result;
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::autosizeDesignHotWaterVolumeFlowRate() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::DesignHotWaterVolumeFlowRate, "autosize");
    OS_ASSERT(result);
  }


  // Zone Total Beam Length

  boost::optional<double> AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::zoneTotalBeamLength() const {
    return getDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::ZoneTotalBeamLength,true);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::isZoneTotalBeamLengthAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::ZoneTotalBeamLength, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional <double> AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::autosizedZoneTotalBeamLength() {
    return getAutosizedValue("Zone Total Beam Length", "m");
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::setZoneTotalBeamLength(double zoneTotalBeamLength) {
    bool result = setDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::ZoneTotalBeamLength, zoneTotalBeamLength);
    return result;
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::autosizeZoneTotalBeamLength() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::ZoneTotalBeamLength, "autosize");
    OS_ASSERT(result);
  }


  /* Double with defaults */

  // Rated Primary Air Flow Rate per Beam Length
  double AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::ratedPrimaryAirFlowRateperBeamLength() const {
    boost::optional<double> value = getDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::RatedPrimaryAirFlowRateperBeamLength,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::isRatedPrimaryAirFlowRateperBeamLengthDefaulted() const {
    return isEmpty(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::RatedPrimaryAirFlowRateperBeamLength);
  }

  bool AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::setRatedPrimaryAirFlowRateperBeamLength(double ratedPrimaryAirFlowRateperBeamLength) {
    bool result = setDouble(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::RatedPrimaryAirFlowRateperBeamLength, ratedPrimaryAirFlowRateperBeamLength);
    return result;
  }

  void AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::resetRatedPrimaryAirFlowRateperBeamLength() {
    bool result = setString(OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeamFields::RatedPrimaryAirFlowRateperBeamLength, "");
    OS_ASSERT(result);
  }


  // Global Autosize
  void AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::autosize() {
    autosizeDesignPrimaryAirVolumeFlowRate();
    autosizeDesignChilledWaterVolumeFlowRate();
    autosizeDesignHotWaterVolumeFlowRate();
    autosizeZoneTotalBeamLength();
  }

  // Global Apply Sizing Values
  void AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::applySizingValues() {
    boost::optional<double> val;
    val = autosizedDesignPrimaryAirVolumeFlowRate();
    if (val) {
      setDesignPrimaryAirVolumeFlowRate(val.get());
    }

    val = autosizedDesignChilledWaterVolumeFlowRate();
    if (val) {
      setDesignChilledWaterVolumeFlowRate(val.get());
    }

    val = autosizedDesignHotWaterVolumeFlowRate();
    if (val) {
      setDesignHotWaterVolumeFlowRate(val.get());
    }

    val = autosizedZoneTotalBeamLength();
    if (val) {
      setZoneTotalBeamLength(val.get());
    }

  }


  /* Convenience functions  */


  /* Convenience method to return the chilled water PlantLoop */
  boost::optional<PlantLoop> AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::chilledWaterPlantLoop() const {
    boost::optional<PlantLoop> plantLoop;
    if ( boost::optional<HVACComponent> _comp = coolingCoil() ) {
      if ( boost::optional<CoilCoolingFourPipeBeam> _hc = _comp->optionalCast<CoilCoolingFourPipeBeam>() ) {
        plantLoop = _hc->plantLoop();
      }
    }
    return plantLoop;
  }
  boost::optional<Node> AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::chilledWaterInletNode() const {
    boost::optional<Node> node;
    if ( boost::optional<HVACComponent> _comp = coolingCoil() ) {
      if ( boost::optional<CoilCoolingFourPipeBeam> _hc = _comp->optionalCast<CoilCoolingFourPipeBeam>() ) {
        node = _hc->chilledWaterInletNode();
      }
    }
    return node;
  }
  boost::optional<Node> AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::chilledWaterOutletNode() const {
    boost::optional<Node> node;
    if ( boost::optional<HVACComponent> _comp = coolingCoil() ) {
      if ( boost::optional<CoilCoolingFourPipeBeam> _hc = _comp->optionalCast<CoilCoolingFourPipeBeam>() ) {
        node = _hc->chilledWaterOutletNode();
      }
    }
    return node;
  }

  /* Convenience method to return the hot water PlantLoop */
  boost::optional<PlantLoop> AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::hotWaterPlantLoop() const {
    boost::optional<PlantLoop> plantLoop;
    if ( boost::optional<HVACComponent> _comp = heatingCoil() ) {
      if ( boost::optional<CoilHeatingFourPipeBeam> _hc = _comp->optionalCast<CoilHeatingFourPipeBeam>() ) {
        plantLoop = _hc->plantLoop();
      }
    }
    return plantLoop;
  }
  boost::optional<Node> AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::hotWaterInletNode() const {
    boost::optional<Node> node;
    if ( boost::optional<HVACComponent> _comp = heatingCoil() ) {
      if ( boost::optional<CoilHeatingFourPipeBeam> _hc = _comp->optionalCast<CoilHeatingFourPipeBeam>() ) {
        node = _hc->hotWaterInletNode();
      }
    }
    return node;
  }
  boost::optional<Node> AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl::hotWaterOutletNode() const {
    boost::optional<Node> node;
    if ( boost::optional<HVACComponent> _comp = heatingCoil() ) {
      if ( boost::optional<CoilHeatingFourPipeBeam> _hc = _comp->optionalCast<CoilHeatingFourPipeBeam>() ) {
        node = _hc->hotWaterOutletNode();
      }
    }
    return node;
  }



} // detail

AirTerminalSingleDuctConstantVolumeFourPipeBeam::AirTerminalSingleDuctConstantVolumeFourPipeBeam(const Model& model)
  : StraightComponent(AirTerminalSingleDuctConstantVolumeFourPipeBeam::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>());

  bool ok = true;

  Schedule always_on = model.alwaysOnDiscreteSchedule();
  ok = setPrimaryAirAvailabilitySchedule(always_on);
  OS_ASSERT(ok);
  ok = setCoolingAvailabilitySchedule(always_on);
  OS_ASSERT(ok);
  ok = setHeatingAvailabilitySchedule(always_on);
  OS_ASSERT(ok);

  autosize();;
}

// More explicit Ctor
//
AirTerminalSingleDuctConstantVolumeFourPipeBeam::AirTerminalSingleDuctConstantVolumeFourPipeBeam(const Model& model,
                                                                                                 HVACComponent& coilCoolingFourPipeBeam,
                                                                                                 HVACComponent& coilHeatingFourPipeBeam)
  : StraightComponent(AirTerminalSingleDuctConstantVolumeFourPipeBeam::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>());

  bool ok = true;

  Schedule always_on = model.alwaysOnDiscreteSchedule();
  ok = setPrimaryAirAvailabilitySchedule(always_on);
  OS_ASSERT(ok);
  ok = setCoolingAvailabilitySchedule(always_on);
  OS_ASSERT(ok);
  ok = setHeatingAvailabilitySchedule(always_on);
  OS_ASSERT(ok);

  autosize();

  ok = setCoolingCoil(coilCoolingFourPipeBeam);
  if (!ok) {
    remove();
    LOG_AND_THROW("Cannot set the cooling coil, make sure you use a CoilCoolingFourPipeBeam")
  }
  OS_ASSERT(ok);
  ok = setHeatingCoil(coilHeatingFourPipeBeam);

  if (!ok) {
    remove();
    LOG_AND_THROW("Cannot set the heating coil, make sure you use a CoilHeatingFourPipeBeam")
  }
  OS_ASSERT(ok);
}
IddObjectType AirTerminalSingleDuctConstantVolumeFourPipeBeam::iddObjectType() {
  return IddObjectType(IddObjectType::OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeam);
}


/* Primary Air Availability Schedule */
Schedule AirTerminalSingleDuctConstantVolumeFourPipeBeam::primaryAirAvailabilitySchedule() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->primaryAirAvailabilitySchedule();
}
bool AirTerminalSingleDuctConstantVolumeFourPipeBeam::setPrimaryAirAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->setPrimaryAirAvailabilitySchedule(schedule);
}

/* Cooling Availability Schedule */
Schedule AirTerminalSingleDuctConstantVolumeFourPipeBeam::coolingAvailabilitySchedule() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->coolingAvailabilitySchedule();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeBeam::setCoolingAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->setCoolingAvailabilitySchedule(schedule);
}

/* Heating Availability Schedule */
Schedule AirTerminalSingleDuctConstantVolumeFourPipeBeam::heatingAvailabilitySchedule() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->heatingAvailabilitySchedule();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeBeam::setHeatingAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->setHeatingAvailabilitySchedule(schedule);
}

/* Nodes */

boost::optional<Node> AirTerminalSingleDuctConstantVolumeFourPipeBeam::primaryAirInletNode() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->primaryAirInletNode();
}

boost::optional<Node> AirTerminalSingleDuctConstantVolumeFourPipeBeam::primaryAirOutletNode() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->primaryAirOutletNode();
}


/* Coils */

boost::optional<HVACComponent> AirTerminalSingleDuctConstantVolumeFourPipeBeam::coolingCoil() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->coolingCoil();
}
bool AirTerminalSingleDuctConstantVolumeFourPipeBeam::setCoolingCoil(const HVACComponent& coilCoolingFourPipeBeam) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->setCoolingCoil(coilCoolingFourPipeBeam);
}


boost::optional<HVACComponent> AirTerminalSingleDuctConstantVolumeFourPipeBeam::heatingCoil() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->heatingCoil();
}
bool AirTerminalSingleDuctConstantVolumeFourPipeBeam::setHeatingCoil(const HVACComponent& coilHeatingFourPipeBeam) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->setHeatingCoil(coilHeatingFourPipeBeam);
}


/* Autosizable fields */


// Design Primary Air Volume Flow Rate

boost::optional<double> AirTerminalSingleDuctConstantVolumeFourPipeBeam::designPrimaryAirVolumeFlowRate() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->designPrimaryAirVolumeFlowRate();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeBeam::isDesignPrimaryAirVolumeFlowRateAutosized() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->isDesignPrimaryAirVolumeFlowRateAutosized();
}

boost::optional <double> AirTerminalSingleDuctConstantVolumeFourPipeBeam::autosizedDesignPrimaryAirVolumeFlowRate() {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->autosizedDesignPrimaryAirVolumeFlowRate();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeBeam::setDesignPrimaryAirVolumeFlowRate(double designPrimaryAirVolumeFlowRate) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->setDesignPrimaryAirVolumeFlowRate(designPrimaryAirVolumeFlowRate);
}

void AirTerminalSingleDuctConstantVolumeFourPipeBeam::autosizeDesignPrimaryAirVolumeFlowRate() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->autosizeDesignPrimaryAirVolumeFlowRate();
}


// Design Chilled Water Volume Flow Rate

boost::optional<double> AirTerminalSingleDuctConstantVolumeFourPipeBeam::designChilledWaterVolumeFlowRate() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->designChilledWaterVolumeFlowRate();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeBeam::isDesignChilledWaterVolumeFlowRateAutosized() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->isDesignChilledWaterVolumeFlowRateAutosized();
}

boost::optional <double> AirTerminalSingleDuctConstantVolumeFourPipeBeam::autosizedDesignChilledWaterVolumeFlowRate() {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->autosizedDesignChilledWaterVolumeFlowRate();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeBeam::setDesignChilledWaterVolumeFlowRate(double designChilledWaterVolumeFlowRate) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->setDesignChilledWaterVolumeFlowRate(designChilledWaterVolumeFlowRate);
}

void AirTerminalSingleDuctConstantVolumeFourPipeBeam::autosizeDesignChilledWaterVolumeFlowRate() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->autosizeDesignChilledWaterVolumeFlowRate();
}


// Design Hot Water Volume Flow Rate

boost::optional<double> AirTerminalSingleDuctConstantVolumeFourPipeBeam::designHotWaterVolumeFlowRate() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->designHotWaterVolumeFlowRate();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeBeam::isDesignHotWaterVolumeFlowRateAutosized() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->isDesignHotWaterVolumeFlowRateAutosized();
}

boost::optional <double> AirTerminalSingleDuctConstantVolumeFourPipeBeam::autosizedDesignHotWaterVolumeFlowRate() {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->autosizedDesignHotWaterVolumeFlowRate();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeBeam::setDesignHotWaterVolumeFlowRate(double designHotWaterVolumeFlowRate) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->setDesignHotWaterVolumeFlowRate(designHotWaterVolumeFlowRate);
}

void AirTerminalSingleDuctConstantVolumeFourPipeBeam::autosizeDesignHotWaterVolumeFlowRate() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->autosizeDesignHotWaterVolumeFlowRate();
}


// Zone Total Beam Length

boost::optional<double> AirTerminalSingleDuctConstantVolumeFourPipeBeam::zoneTotalBeamLength() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->zoneTotalBeamLength();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeBeam::isZoneTotalBeamLengthAutosized() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->isZoneTotalBeamLengthAutosized();
}

boost::optional <double> AirTerminalSingleDuctConstantVolumeFourPipeBeam::autosizedZoneTotalBeamLength() {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->autosizedZoneTotalBeamLength();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeBeam::setZoneTotalBeamLength(double zoneTotalBeamLength) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->setZoneTotalBeamLength(zoneTotalBeamLength);
}

void AirTerminalSingleDuctConstantVolumeFourPipeBeam::autosizeZoneTotalBeamLength() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->autosizeZoneTotalBeamLength();
}


/* Double with defaults */

// Rated Primary Air Flow Rate per Beam Length

double AirTerminalSingleDuctConstantVolumeFourPipeBeam::ratedPrimaryAirFlowRateperBeamLength() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->ratedPrimaryAirFlowRateperBeamLength();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeBeam::isRatedPrimaryAirFlowRateperBeamLengthDefaulted() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->isRatedPrimaryAirFlowRateperBeamLengthDefaulted();
}

bool AirTerminalSingleDuctConstantVolumeFourPipeBeam::setRatedPrimaryAirFlowRateperBeamLength(double ratedPrimaryAirFlowRateperBeamLength) {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->setRatedPrimaryAirFlowRateperBeamLength(ratedPrimaryAirFlowRateperBeamLength);
}

void AirTerminalSingleDuctConstantVolumeFourPipeBeam::resetRatedPrimaryAirFlowRateperBeamLength() {
  getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->resetRatedPrimaryAirFlowRateperBeamLength();
}



/* Convenience functions  */

/* Convenience method to return the chilled water PlantLoop */
boost::optional<PlantLoop> AirTerminalSingleDuctConstantVolumeFourPipeBeam::chilledWaterPlantLoop() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->chilledWaterPlantLoop();
}

boost::optional<Node> AirTerminalSingleDuctConstantVolumeFourPipeBeam::chilledWaterInletNode() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->chilledWaterInletNode();
}

boost::optional<Node> AirTerminalSingleDuctConstantVolumeFourPipeBeam::chilledWaterOutletNode() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->chilledWaterOutletNode();
}

/* Convenience method to return the hot water PlantLoop */
boost::optional<PlantLoop> AirTerminalSingleDuctConstantVolumeFourPipeBeam::hotWaterPlantLoop() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->hotWaterPlantLoop();
}

boost::optional<Node> AirTerminalSingleDuctConstantVolumeFourPipeBeam::hotWaterInletNode() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->hotWaterInletNode();
}

boost::optional<Node> AirTerminalSingleDuctConstantVolumeFourPipeBeam::hotWaterOutletNode() const {
  return getImpl<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl>()->hotWaterOutletNode();
}


/// @cond
AirTerminalSingleDuctConstantVolumeFourPipeBeam::AirTerminalSingleDuctConstantVolumeFourPipeBeam(std::shared_ptr<detail::AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

