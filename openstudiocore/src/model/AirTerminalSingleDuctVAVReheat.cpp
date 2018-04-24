/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "AirTerminalSingleDuctVAVReheat.hpp"
#include "AirTerminalSingleDuctVAVReheat_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "PortList.hpp"
#include "PortList_Impl.hpp"
#include "HVACComponent.hpp"
#include "HVACComponent_Impl.hpp"
#include "AirLoopHVACZoneSplitter.hpp"
#include "AirLoopHVACZoneSplitter_Impl.hpp"
#include "AirLoopHVACZoneMixer.hpp"
#include "AirLoopHVACZoneMixer_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Model.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "AirflowNetworkEquivalentDuct.hpp"
#include "AirflowNetworkEquivalentDuct_Impl.hpp"
#include <utilities/idd/OS_AirTerminal_SingleDuct_VAV_Reheat_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Compare.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {

namespace model {

namespace detail{

  AirTerminalSingleDuctVAVReheat_Impl::AirTerminalSingleDuctVAVReheat_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : StraightComponent_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AirTerminalSingleDuctVAVReheat::iddObjectType());
  }

  AirTerminalSingleDuctVAVReheat_Impl::AirTerminalSingleDuctVAVReheat_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other,Model_Impl* model,bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AirTerminalSingleDuctVAVReheat::iddObjectType());
  }

  AirTerminalSingleDuctVAVReheat_Impl::AirTerminalSingleDuctVAVReheat_Impl(
      const AirTerminalSingleDuctVAVReheat_Impl& other,
      Model_Impl* model,
      bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  // Get all output variable names that could be associated with this object.
  const std::vector<std::string>& AirTerminalSingleDuctVAVReheat_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result{
      // These applies to all AirTerminals
      "Zone Air Terminal Sensible Heating Energy",
      "Zone Air Terminal Sensible Heating Rate",
      "Zone Air Terminal Sensible Cooling Energy",
      "Zone Air Terminal Sensible Cooling Rate",

      "Zone Air Terminal VAV Damper Position",
      "Zone Air Terminal Minimum Air Flow Fraction",
      "Zone Air Terminal Outdoor Air Volume Flow Rate"
    };
    return result;
  }

  IddObjectType AirTerminalSingleDuctVAVReheat_Impl::iddObjectType() const {
    return AirTerminalSingleDuctVAVReheat::iddObjectType();
  }

  std::vector<ScheduleTypeKey> AirTerminalSingleDuctVAVReheat_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_AirTerminal_SingleDuct_VAV_ReheatFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("AirTerminalSingleDuctVAVReheat","Availability"));
    }
    if (std::find(b,e,OS_AirTerminal_SingleDuct_VAV_ReheatFields::MinimumAirFlowFractionScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("AirTerminalSingleDuctVAVReheat","Minimum Air Flow Fraction"));
    }
    return result;
  }

  Schedule AirTerminalSingleDuctVAVReheat_Impl::availabilitySchedule()  const
  {
    boost::optional<Schedule> value = optionalAvailabilitySchedule();
    if (!value){
      // it is an error if we get here, however we don't want to crash
      // so we hook up to global always on schedule
      LOG(Error, "Required availability schedule not set, using 'Always On' schedule");
      value = this->model().alwaysOnDiscreteSchedule();
      OS_ASSERT(value);
      const_cast<AirTerminalSingleDuctVAVReheat_Impl*>(this)->setAvailabilitySchedule(*value);
      value = optionalAvailabilitySchedule();
    }
    OS_ASSERT(value);
    return value.get();
  }

  bool AirTerminalSingleDuctVAVReheat_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_AirTerminal_SingleDuct_VAV_ReheatFields::AvailabilityScheduleName,
                              "AirTerminalSingleDuctVAVReheat",
                              "Availability",
                              schedule);
    return result;
  }

  unsigned AirTerminalSingleDuctVAVReheat_Impl::inletPort() const
  {
    return OS_AirTerminal_SingleDuct_VAV_ReheatFields::AirInletNodeName;
  }

  unsigned AirTerminalSingleDuctVAVReheat_Impl::outletPort() const
  {
    return OS_AirTerminal_SingleDuct_VAV_ReheatFields::AirOutletNodeName;
  }

  bool AirTerminalSingleDuctVAVReheat_Impl::addToNode(Node & node)
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
                AirTerminalSingleDuctVAVReheat mo = this->getObject<AirTerminalSingleDuctVAVReheat>();

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

  std::vector<IdfObject> AirTerminalSingleDuctVAVReheat_Impl::remove()
  {
    Model _model = this->model();
    ModelObject thisObject = this->getObject<ModelObject>();

    HVACComponent _reheatCoil = reheatCoil();

    boost::optional<ModelObject> sourceModelObject = this->inletModelObject();
    boost::optional<unsigned> sourcePort = this->connectedObjectPort(this->inletPort());

    boost::optional<ModelObject> targetModelObject = this->outletModelObject();
    boost::optional<unsigned> targetPort = this->connectedObjectPort(this->outletPort());

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

            if( boost::optional<PlantLoop> loop = _reheatCoil.plantLoop() )
            {
              loop->removeDemandBranchWithComponent(_reheatCoil);
            }

            return StraightComponent_Impl::remove();
          }
        }
      }
    }

    model().disconnect(getObject<ModelObject>(),inletPort());
    model().disconnect(getObject<ModelObject>(),outletPort());

    if( boost::optional<PlantLoop> loop = _reheatCoil.plantLoop() )
    {
      loop->removeDemandBranchWithComponent(_reheatCoil);
    }

    return StraightComponent_Impl::remove();
  }

  bool AirTerminalSingleDuctVAVReheat_Impl::isRemovable() const
  {
    return true;
  }

  std::vector<ModelObject> AirTerminalSingleDuctVAVReheat_Impl::children() const
  {
    std::vector<ModelObject> result;
    if (OptionalHVACComponent intermediate = optionalReheatCoil()) {
      result.push_back(*intermediate);
    }
    std::vector<AirflowNetworkEquivalentDuct> myAFNItems = getObject<ModelObject>().getModelObjectSources<AirflowNetworkEquivalentDuct>(AirflowNetworkEquivalentDuct::iddObjectType());
    result.insert(result.end(), myAFNItems.begin(), myAFNItems.end());
    return result;
  }

  ModelObject AirTerminalSingleDuctVAVReheat_Impl::clone(Model model) const
  {
    AirTerminalSingleDuctVAVReheat modelObjectClone = StraightComponent_Impl::clone(model).cast<AirTerminalSingleDuctVAVReheat>();

    HVACComponent coil = this->reheatCoil();

    HVACComponent coilClone = coil.clone(model).cast<HVACComponent>();

    modelObjectClone.setReheatCoil(coilClone);

    return modelObjectClone;
  }

  boost::optional<double> AirTerminalSingleDuctVAVReheat_Impl::maximumAirFlowRate() const
  {
    return this->getDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumAirFlowRate);
  }

  bool AirTerminalSingleDuctVAVReheat_Impl::setMaximumAirFlowRate( double value )
  {
    return this->setDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumAirFlowRate,value);
  }

  void AirTerminalSingleDuctVAVReheat_Impl::autosizeMaximumAirFlowRate()
  {
    this->setString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumAirFlowRate,"AutoSize");
  }

  bool AirTerminalSingleDuctVAVReheat_Impl::isMaximumAirFlowRateAutosized() const
  {
    boost::optional<std::string> text = this->getString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumAirFlowRate);
    if( text )
    {
      if( istringEqual(text.get(),"Autocalculate") )
      {
        return true;
      }
      else if( istringEqual(text.get(),"AutoSize") )
      {
        return true;
      }
    }

    return false;
  }

  std::string AirTerminalSingleDuctVAVReheat_Impl::zoneMinimumAirFlowMethod()
  {
    return this->getString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::ZoneMinimumAirFlowInputMethod).get();
  }

  bool AirTerminalSingleDuctVAVReheat_Impl::setZoneMinimumAirFlowMethod( std::string value )
  {
    if( istringEqual(value,"Constant") )
    {
      return this->setString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::ZoneMinimumAirFlowInputMethod,"Constant");
    }
    else if( istringEqual(value,"FixedFlowRate") )
    {
      return this->setString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::ZoneMinimumAirFlowInputMethod,"FixedFlowRate");
    }
    else if( istringEqual(value,"Scheduled") )
    {
      return this->setString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::ZoneMinimumAirFlowInputMethod,"Scheduled");
    } else {
      return false;
    }
  }

  double AirTerminalSingleDuctVAVReheat_Impl::constantMinimumAirFlowFraction()
  {
    return this->getDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::ConstantMinimumAirFlowFraction).get();
  }

  bool AirTerminalSingleDuctVAVReheat_Impl::setConstantMinimumAirFlowFraction( double value )
  {
    return this->setDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::ConstantMinimumAirFlowFraction,value);
  }

  double AirTerminalSingleDuctVAVReheat_Impl::fixedMinimumAirFlowRate()
  {
    return this->getDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::FixedMinimumAirFlowRate).get();
  }

  bool AirTerminalSingleDuctVAVReheat_Impl::setFixedMinimumAirFlowRate( double value )
  {
    return this->setDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::FixedMinimumAirFlowRate,value);
  }

  boost::optional<Schedule> AirTerminalSingleDuctVAVReheat_Impl::minimumAirFlowFractionSchedule() const
  {
    return this->getObject<AirTerminalSingleDuctVAVReheat>().getModelObjectTarget<Schedule>(
        OS_AirTerminal_SingleDuct_VAV_ReheatFields::MinimumAirFlowFractionScheduleName);
  }

  bool AirTerminalSingleDuctVAVReheat_Impl::setMinimumAirFlowFractionSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MinimumAirFlowFractionScheduleName,
                              "AirTerminalSingleDuctVAVReheat",
                              "Minimum Air Flow Fraction",
                              schedule);
    return result;
  }

  void AirTerminalSingleDuctVAVReheat_Impl::resetMinimumAirFlowFractionSchedule() {
    bool result = setString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MinimumAirFlowFractionScheduleName, "");
    OS_ASSERT(result);
  }

  boost::optional<double> AirTerminalSingleDuctVAVReheat_Impl::maximumHotWaterOrSteamFlowRate()
  {
    return this->getDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumHotWaterorSteamFlowRate);
  }

  bool AirTerminalSingleDuctVAVReheat_Impl::setMaximumHotWaterOrSteamFlowRate( double value )
  {
    return this->setDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumHotWaterorSteamFlowRate,value);
  }

  void AirTerminalSingleDuctVAVReheat_Impl::autosizeMaximumHotWaterOrSteamFlowRate()
  {
    this->setString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumHotWaterorSteamFlowRate,"AutoSize");
  }

  bool AirTerminalSingleDuctVAVReheat_Impl::isMaximumHotWaterOrSteamFlowRateAutosized() const
  {
    boost::optional<std::string> text = this->getString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumHotWaterorSteamFlowRate);
    if( text )
    {
      if( istringEqual(text.get(),"Autocalculate") )
      {
        return true;
      }
      else if( istringEqual(text.get(),"AutoSize") )
      {
        return true;
      }
    }

    return false;
  }

  double AirTerminalSingleDuctVAVReheat_Impl::minimumHotWaterOrSteamFlowRate()
  {
    return this->getDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MinimumHotWaterorSteamFlowRate).get();
  }

  bool AirTerminalSingleDuctVAVReheat_Impl::setMinimumHotWaterOrStreamFlowRate( double value )
  {
    return this->setDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MinimumHotWaterorSteamFlowRate,value);
  }

  double AirTerminalSingleDuctVAVReheat_Impl::convergenceTolerance()
  {
    return this->getDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::ConvergenceTolerance).get();
  }

  bool AirTerminalSingleDuctVAVReheat_Impl::setConvergenceTolerance( double value )
  {
    return this->setDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::ConvergenceTolerance,value);
  }

  std::string AirTerminalSingleDuctVAVReheat_Impl::damperHeatingAction()
  {
    return this->getString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::DamperHeatingAction).get();
  }

  bool AirTerminalSingleDuctVAVReheat_Impl::setDamperHeatingAction( std::string value )
  {
    return setString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::DamperHeatingAction,value);;
  }

  boost::optional<double> AirTerminalSingleDuctVAVReheat_Impl::maximumFlowPerZoneFloorAreaDuringReheat()
  {
    return this->getDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumFlowperZoneFloorAreaDuringReheat);
  }

  void AirTerminalSingleDuctVAVReheat_Impl::resetMaximumFlowPerZoneFloorAreaDuringReheat()
  {
    bool result = setString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumFlowperZoneFloorAreaDuringReheat, "");
    OS_ASSERT(result);
  }

  bool AirTerminalSingleDuctVAVReheat_Impl::setMaximumFlowPerZoneFloorAreaDuringReheat( double value )
  {
    return this->setDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumFlowperZoneFloorAreaDuringReheat,value);
  }

  void AirTerminalSingleDuctVAVReheat_Impl::autosizeMaximumFlowPerZoneFloorAreaDuringReheat()
  {
    this->setString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumFlowperZoneFloorAreaDuringReheat,"AutoSize");
  }

  bool AirTerminalSingleDuctVAVReheat_Impl::isMaximumFlowPerZoneFloorAreaDuringReheatAutosized() const
  {
    boost::optional<std::string> text = this->getString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumFlowperZoneFloorAreaDuringReheat);
    if( text )
    {
      if( istringEqual(text.get(),"Autocalculate") )
      {
        return true;
      }
      else if( istringEqual(text.get(),"AutoSize") )
      {
        return true;
      }
    }

    return false;
  }

  boost::optional<double> AirTerminalSingleDuctVAVReheat_Impl::maximumFlowFractionDuringReheat()
  {
    return this->getDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumFlowFractionDuringReheat);
  }

  bool AirTerminalSingleDuctVAVReheat_Impl::setMaximumFlowFractionDuringReheat( double value )
  {
    return this->setDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumFlowFractionDuringReheat,value);
  }

  void AirTerminalSingleDuctVAVReheat_Impl::autosizeMaximumFlowFractionDuringReheat()
  {
    this->setString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumFlowFractionDuringReheat,"AutoSize");
  }

  bool AirTerminalSingleDuctVAVReheat_Impl::isMaximumFlowFractionDuringReheatAutosized() const
  {
    boost::optional<std::string> text = this->getString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumFlowFractionDuringReheat);
    if( text )
    {
      if( istringEqual(text.get(),"Autocalculate") )
      {
        return true;
      }
      else if( istringEqual(text.get(),"AutoSize") )
      {
        return true;
      }
    }

    return false;
  }

  void AirTerminalSingleDuctVAVReheat_Impl::resetMaximumFlowFractionDuringReheat()
  {
    this->setString(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumFlowFractionDuringReheat,"");
  }

  double AirTerminalSingleDuctVAVReheat_Impl::maximumReheatAirTemperature()
  {
    return this->getDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumReheatAirTemperature).get();
  }

  bool AirTerminalSingleDuctVAVReheat_Impl::setMaximumReheatAirTemperature( double value )
  {
    return this->setDouble(OS_AirTerminal_SingleDuct_VAV_ReheatFields::MaximumReheatAirTemperature,value);
  }

  HVACComponent AirTerminalSingleDuctVAVReheat_Impl::reheatCoil() const
  {
    boost::optional<HVACComponent> coil = optionalReheatCoil();
    OS_ASSERT(coil);
    return coil.get();
  }

  bool AirTerminalSingleDuctVAVReheat_Impl::setReheatCoil(HVACComponent & coil)
  {
    bool result = false;

    switch(coil.iddObject().type().value())
    {
    case openstudio::IddObjectType::OS_Coil_Heating_Gas :
      {
        result = true;

        break;
      }
    case openstudio::IddObjectType::OS_Coil_Heating_Water :
      {
        result = true;

        break;
      }
    case openstudio::IddObjectType::OS_Coil_Heating_Electric :
      {
        result = true;

        break;
      }
    default:
      {
        LOG(Warn, "Unsupported or invalid IddObjectType: '" << coil.iddObject().name() << "'");

        result = false;
      }
    }

    if( result )
    {
      result = this->setPointer(OS_AirTerminal_SingleDuct_VAV_ReheatFields::ReheatCoilName,coil.handle());
    }

    return result;
  }

  boost::optional<HVACComponent> AirTerminalSingleDuctVAVReheat_Impl::optionalReheatCoil() const {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_AirTerminal_SingleDuct_VAV_ReheatFields::ReheatCoilName);
  }

  boost::optional<Schedule> AirTerminalSingleDuctVAVReheat_Impl::optionalAvailabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirTerminal_SingleDuct_VAV_ReheatFields::AvailabilityScheduleName);
  }

  boost::optional<ModelObject> AirTerminalSingleDuctVAVReheat_Impl::availabilityScheduleAsModelObject() const {
    OptionalModelObject result = availabilitySchedule();
    return result;
  }

  boost::optional<ModelObject> AirTerminalSingleDuctVAVReheat_Impl::minimumAirFlowFractionScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = minimumAirFlowFractionSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  bool AirTerminalSingleDuctVAVReheat_Impl::setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setAvailabilitySchedule(schedule);
      }
    }
    return false;
  }

  bool AirTerminalSingleDuctVAVReheat_Impl::setMinimumAirFlowFractionScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setMinimumAirFlowFractionSchedule(schedule);
      }
      else {
        return false;
      }
    }
    else {
      resetMinimumAirFlowFractionSchedule();
    }
    return true;
  }

  bool AirTerminalSingleDuctVAVReheat_Impl::controlForOutdoorAir() const
  {
    return getBooleanFieldValue(OS_AirTerminal_SingleDuct_VAV_ReheatFields::ControlForOutdoorAir);
  }

  bool AirTerminalSingleDuctVAVReheat_Impl::setControlForOutdoorAir(bool controlForOutdoorAir)
  {
    return setBooleanFieldValue(OS_AirTerminal_SingleDuct_VAV_ReheatFields::ControlForOutdoorAir,controlForOutdoorAir);;
  }

  AirflowNetworkEquivalentDuct AirTerminalSingleDuctVAVReheat_Impl::getAirflowNetworkEquivalentDuct(double length, double diameter)
  {
    boost::optional<AirflowNetworkEquivalentDuct> opt = airflowNetworkEquivalentDuct();
    if (opt) {
      if (opt->airPathLength() != length){
        opt->setAirPathLength(length);
      }
      if (opt->airPathHydraulicDiameter() != diameter){
        opt->setAirPathHydraulicDiameter(diameter);
      }
    }
    return AirflowNetworkEquivalentDuct(model(), length, diameter, handle());
  }

  boost::optional<AirflowNetworkEquivalentDuct> AirTerminalSingleDuctVAVReheat_Impl::airflowNetworkEquivalentDuct() const
  {
    std::vector<AirflowNetworkEquivalentDuct> myAFN = getObject<ModelObject>().getModelObjectSources<AirflowNetworkEquivalentDuct>(AirflowNetworkEquivalentDuct::iddObjectType());
    auto count = myAFN.size();
    if (count == 1) {
      return myAFN[0];
    } else if (count > 1) {
      LOG(Warn, briefDescription() << " has more than one AirflowNetwork EquivalentDuct attached, returning first.");
      return myAFN[0];
    }
    return boost::none;
  }

  boost::optional<double> AirTerminalSingleDuctVAVReheat_Impl::autosizedMaximumAirFlowRate() const {
    return getAutosizedValue("Design Size Maximum Air Flow Rate", "m3/s");
  }

  boost::optional<double> AirTerminalSingleDuctVAVReheat_Impl::autosizedMaximumHotWaterOrSteamFlowRate() const {
    return getAutosizedValue("Design Size Maximum Reheat Water Flow Rate", "m3/s");
  }

  boost::optional<double> AirTerminalSingleDuctVAVReheat_Impl::autosizedMaximumFlowPerZoneFloorAreaDuringReheat() const {
    return getAutosizedValue("Design Size Maximum Flow per Zone Floor Area during Reheat", "m3/s-m2");
  }

  boost::optional<double> AirTerminalSingleDuctVAVReheat_Impl::autosizedMaximumFlowFractionDuringReheat() const {
    return getAutosizedValue("Design Size Maximum Flow Fraction during Reheat", "typo_in_energyplus");
  }

  void AirTerminalSingleDuctVAVReheat_Impl::autosize() {
    autosizeMaximumAirFlowRate();
    autosizeMaximumHotWaterOrSteamFlowRate();
    autosizeMaximumFlowPerZoneFloorAreaDuringReheat();
    autosizeMaximumFlowFractionDuringReheat();
  }

  void AirTerminalSingleDuctVAVReheat_Impl::applySizingValues() {
    boost::optional<double> val;
    val = autosizedMaximumAirFlowRate();
    if (val) {
      setMaximumAirFlowRate(val.get());
    }

    val = autosizedMaximumHotWaterOrSteamFlowRate();
    if (val) {
      setMaximumHotWaterOrSteamFlowRate(val.get());
    }

    val = autosizedMaximumFlowPerZoneFloorAreaDuringReheat();
    if (val) {
      setMaximumFlowPerZoneFloorAreaDuringReheat(val.get());
    }

    val = autosizedMaximumFlowFractionDuringReheat();
    if (val) {
      setMaximumFlowFractionDuringReheat(val.get());
    }

  }

} // detail

AirTerminalSingleDuctVAVReheat::AirTerminalSingleDuctVAVReheat( const Model& model,
                                                                Schedule & availabilitySchedule,
                                                                HVACComponent & coil )
  : StraightComponent(AirTerminalSingleDuctVAVReheat::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>());

  bool test = setAvailabilitySchedule(availabilitySchedule);
  if (!test) {
    remove();
    LOG_AND_THROW("Unable to construct " << briefDescription() << ", because cannot set its "
        << "availability schedule to " << availabilitySchedule.briefDescription() << ".");
  }

  setReheatCoil(coil);

  autosizeMaximumAirFlowRate();

  setZoneMinimumAirFlowMethod("Constant");

  setConstantMinimumAirFlowFraction(0.3);

  setFixedMinimumAirFlowRate(0.0);

  setMaximumFlowPerZoneFloorAreaDuringReheat(0.0);

  setMinimumHotWaterOrStreamFlowRate(0.0);

  setConvergenceTolerance(0.001);

  setDamperHeatingAction("Normal");

  autosizeMaximumFlowPerZoneFloorAreaDuringReheat();

  autosizeMaximumFlowFractionDuringReheat();

  setMaximumReheatAirTemperature(35.0);

  autosizeMaximumHotWaterOrSteamFlowRate();

  setControlForOutdoorAir(false);
}

Schedule AirTerminalSingleDuctVAVReheat::availabilitySchedule() const
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->availabilitySchedule();
}

bool AirTerminalSingleDuctVAVReheat::setAvailabilitySchedule(Schedule& schedule)
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->setAvailabilitySchedule(schedule);
}

AirTerminalSingleDuctVAVReheat::AirTerminalSingleDuctVAVReheat(std::shared_ptr<detail::AirTerminalSingleDuctVAVReheat_Impl> p)
  : StraightComponent(std::move(p))
{}

IddObjectType AirTerminalSingleDuctVAVReheat::iddObjectType() {
  return IddObjectType(IddObjectType::OS_AirTerminal_SingleDuct_VAV_Reheat);
}

boost::optional<double> AirTerminalSingleDuctVAVReheat::maximumAirFlowRate() const
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->maximumAirFlowRate();
}

bool AirTerminalSingleDuctVAVReheat::setMaximumAirFlowRate( double value )
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->setMaximumAirFlowRate(value);
}

void AirTerminalSingleDuctVAVReheat::autosizeMaximumAirFlowRate()
{
  getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->autosizeMaximumAirFlowRate();
}

bool AirTerminalSingleDuctVAVReheat::isMaximumAirFlowRateAutosized() const
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->isMaximumAirFlowRateAutosized();
}

std::string AirTerminalSingleDuctVAVReheat::zoneMinimumAirFlowMethod()
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->zoneMinimumAirFlowMethod();
}

bool AirTerminalSingleDuctVAVReheat::setZoneMinimumAirFlowMethod( std::string value )
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->setZoneMinimumAirFlowMethod(value);
}

double AirTerminalSingleDuctVAVReheat::constantMinimumAirFlowFraction()
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->constantMinimumAirFlowFraction();
}

bool AirTerminalSingleDuctVAVReheat::setConstantMinimumAirFlowFraction( double value )
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->setConstantMinimumAirFlowFraction(value);
}

double AirTerminalSingleDuctVAVReheat::fixedMinimumAirFlowRate()
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->fixedMinimumAirFlowRate();
}

bool AirTerminalSingleDuctVAVReheat::setFixedMinimumAirFlowRate( double value )
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->setFixedMinimumAirFlowRate(value);
}

boost::optional<Schedule> AirTerminalSingleDuctVAVReheat::minimumAirFlowFractionSchedule() const
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->minimumAirFlowFractionSchedule();
}

bool AirTerminalSingleDuctVAVReheat::setMinimumAirFlowFractionSchedule(Schedule & schedule )
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->setMinimumAirFlowFractionSchedule(schedule);
}

void AirTerminalSingleDuctVAVReheat::resetMinimumAirFlowFractionSchedule() {
  getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->resetMinimumAirFlowFractionSchedule();
}

boost::optional<double> AirTerminalSingleDuctVAVReheat::maximumHotWaterOrSteamFlowRate()
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->maximumHotWaterOrSteamFlowRate();
}

bool AirTerminalSingleDuctVAVReheat::setMaximumHotWaterOrSteamFlowRate( double value )
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->setMaximumHotWaterOrSteamFlowRate(value);
}

void AirTerminalSingleDuctVAVReheat::autosizeMaximumHotWaterOrSteamFlowRate()
{
  getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->autosizeMaximumHotWaterOrSteamFlowRate();
}

bool AirTerminalSingleDuctVAVReheat::isMaximumHotWaterOrSteamFlowRateAutosized() const
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->isMaximumHotWaterOrSteamFlowRateAutosized();
}

double AirTerminalSingleDuctVAVReheat::minimumHotWaterOrSteamFlowRate()
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->minimumHotWaterOrSteamFlowRate();
}

bool AirTerminalSingleDuctVAVReheat::setMinimumHotWaterOrStreamFlowRate( double value )
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->setMinimumHotWaterOrStreamFlowRate(value);
}

double AirTerminalSingleDuctVAVReheat::convergenceTolerance()
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->convergenceTolerance();
}

bool AirTerminalSingleDuctVAVReheat::setConvergenceTolerance( double value )
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->setConvergenceTolerance(value);
}

std::string AirTerminalSingleDuctVAVReheat::damperHeatingAction()
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->damperHeatingAction();
}

bool AirTerminalSingleDuctVAVReheat::setDamperHeatingAction( std::string value )
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->setDamperHeatingAction(value);
}

boost::optional<double> AirTerminalSingleDuctVAVReheat::maximumFlowPerZoneFloorAreaDuringReheat()
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->maximumFlowPerZoneFloorAreaDuringReheat();
}

void AirTerminalSingleDuctVAVReheat::resetMaximumFlowPerZoneFloorAreaDuringReheat()
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->resetMaximumFlowPerZoneFloorAreaDuringReheat();
}

bool AirTerminalSingleDuctVAVReheat::setMaximumFlowPerZoneFloorAreaDuringReheat( double value )
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->setMaximumFlowPerZoneFloorAreaDuringReheat(value);
}

void AirTerminalSingleDuctVAVReheat::autosizeMaximumFlowPerZoneFloorAreaDuringReheat()
{
  getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->autosizeMaximumFlowPerZoneFloorAreaDuringReheat();
}

bool AirTerminalSingleDuctVAVReheat::isMaximumFlowPerZoneFloorAreaDuringReheatAutosized() const
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->isMaximumFlowPerZoneFloorAreaDuringReheatAutosized();
}

boost::optional<double> AirTerminalSingleDuctVAVReheat::maximumFlowFractionDuringReheat()
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->maximumFlowFractionDuringReheat();
}

bool AirTerminalSingleDuctVAVReheat::setMaximumFlowFractionDuringReheat( double value )
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->setMaximumFlowFractionDuringReheat(value);
}

void AirTerminalSingleDuctVAVReheat::autosizeMaximumFlowFractionDuringReheat()
{
  getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->autosizeMaximumFlowFractionDuringReheat();
}

bool AirTerminalSingleDuctVAVReheat::isMaximumFlowFractionDuringReheatAutosized() const
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->isMaximumFlowFractionDuringReheatAutosized();
}

void AirTerminalSingleDuctVAVReheat::resetMaximumFlowFractionDuringReheat()
{
  getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->resetMaximumFlowFractionDuringReheat();
}

double AirTerminalSingleDuctVAVReheat::maximumReheatAirTemperature()
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->maximumReheatAirTemperature();
}

bool AirTerminalSingleDuctVAVReheat::setMaximumReheatAirTemperature( double value )
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->setMaximumReheatAirTemperature(value);
}

HVACComponent AirTerminalSingleDuctVAVReheat::reheatCoil() const
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->reheatCoil();
}

bool AirTerminalSingleDuctVAVReheat::setReheatCoil(HVACComponent & coil)
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->setReheatCoil(coil);
}

bool AirTerminalSingleDuctVAVReheat::controlForOutdoorAir() const
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->controlForOutdoorAir();
}

bool AirTerminalSingleDuctVAVReheat::setControlForOutdoorAir(bool controlForOutdoorAir)
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->setControlForOutdoorAir(controlForOutdoorAir);
}

AirflowNetworkEquivalentDuct AirTerminalSingleDuctVAVReheat::getAirflowNetworkEquivalentDuct(double length, double diameter)
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->getAirflowNetworkEquivalentDuct(length, diameter);
}

boost::optional<AirflowNetworkEquivalentDuct> AirTerminalSingleDuctVAVReheat::airflowNetworkEquivalentDuct() const
{
  return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->airflowNetworkEquivalentDuct();
}

  boost::optional<double> AirTerminalSingleDuctVAVReheat::autosizedMaximumAirFlowRate() const {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->autosizedMaximumAirFlowRate();
  }

  boost::optional<double> AirTerminalSingleDuctVAVReheat::autosizedMaximumHotWaterOrSteamFlowRate() const {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->autosizedMaximumHotWaterOrSteamFlowRate();
  }

  boost::optional<double> AirTerminalSingleDuctVAVReheat::autosizedMaximumFlowPerZoneFloorAreaDuringReheat() const {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->autosizedMaximumFlowPerZoneFloorAreaDuringReheat();
  }

  boost::optional<double> AirTerminalSingleDuctVAVReheat::autosizedMaximumFlowFractionDuringReheat() const {
    return getImpl<detail::AirTerminalSingleDuctVAVReheat_Impl>()->autosizedMaximumFlowFractionDuringReheat();
  }

} // model

} // openstudio
