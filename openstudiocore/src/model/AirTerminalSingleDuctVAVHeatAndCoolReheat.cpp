/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "AirTerminalSingleDuctVAVHeatAndCoolReheat.hpp"
#include "AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "PortList.hpp"
#include "PortList_Impl.hpp"
#include "Mixer.hpp"
#include "Mixer_Impl.hpp"
#include "Splitter.hpp"
#include "Splitter_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include <utilities/idd/OS_AirTerminal_SingleDuct_VAV_HeatAndCool_Reheat_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl::AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl(const IdfObject& idfObject,
                                                                                                 Model_Impl* model,
                                                                                                 bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AirTerminalSingleDuctVAVHeatAndCoolReheat::iddObjectType());
  }

  AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl::AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                 Model_Impl* model,
                                                                                                 bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AirTerminalSingleDuctVAVHeatAndCoolReheat::iddObjectType());
  }

  AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl::AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl(const AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl& other,
                                                                                                 Model_Impl* model,
                                                                                                 bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl::iddObjectType() const {
    return AirTerminalSingleDuctVAVHeatAndCoolReheat::iddObjectType();
  }

  std::vector<ScheduleTypeKey> AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::AvailabilitySchedule) != e)
    {
      result.push_back(ScheduleTypeKey("AirTerminalSingleDuctVAVHeatAndCoolReheat","Availability Schedule"));
    }
    return result;
  }

  boost::optional<Schedule> AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl::availabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::AvailabilitySchedule);
  }

  boost::optional<double> AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl::maximumAirFlowRate() const {
    return getDouble(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::MaximumAirFlowRate,true);
  }

  bool AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl::isMaximumAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::MaximumAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl::zoneMinimumAirFlowFraction() const {
    boost::optional<double> value = getDouble(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::ZoneMinimumAirFlowFraction,true);
    OS_ASSERT(value);
    return value.get();
  }

  HVACComponent AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl::reheatCoil() const {
    boost::optional<HVACComponent> value = optionalReheatCoil();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Reheat Coil attached.");
    }
    return value.get();
  }

  boost::optional<double> AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl::maximumHotWaterorSteamFlowRate() const {
    return getDouble(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::MaximumHotWaterorSteamFlowRate,true);
  }

  bool AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl::isMaximumHotWaterorSteamFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::MaximumHotWaterorSteamFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl::minimumHotWaterorSteamFlowRate() const {
    boost::optional<double> value = getDouble(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::MinimumHotWaterorSteamFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  double AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl::convergenceTolerance() const {
    boost::optional<double> value = getDouble(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::ConvergenceTolerance,true);
    OS_ASSERT(value);
    return value.get();
  }

  double AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl::maximumReheatAirTemperature() const {
    boost::optional<double> value = getDouble(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::MaximumReheatAirTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::AvailabilitySchedule,
                              "AirTerminalSingleDuctVAVHeatAndCoolReheat",
                              "Availability Schedule",
                              schedule);
    return result;
  }

  void AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl::resetAvailabilitySchedule() {
    bool result = setString(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::AvailabilitySchedule, "");
    OS_ASSERT(result);
  }

  bool AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl::setMaximumAirFlowRate(boost::optional<double> maximumAirFlowRate) {
    bool result(false);
    if (maximumAirFlowRate) {
      result = setDouble(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::MaximumAirFlowRate, maximumAirFlowRate.get());
    }
    return result;
  }

  void AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl::autosizeMaximumAirFlowRate() {
    bool result = setString(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::MaximumAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl::setZoneMinimumAirFlowFraction(double zoneMinimumAirFlowFraction) {
    bool result = setDouble(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::ZoneMinimumAirFlowFraction, zoneMinimumAirFlowFraction);
    return result;
  }

  bool AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl::setReheatCoil(const HVACComponent& heatingCoil) {
    bool result = setPointer(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::ReheatCoil, heatingCoil.handle());
    return result;
  }

  bool AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl::setMaximumHotWaterorSteamFlowRate(boost::optional<double> maximumHotWaterorSteamFlowRate) {
    bool result(false);
    if (maximumHotWaterorSteamFlowRate) {
      result = setDouble(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::MaximumHotWaterorSteamFlowRate, maximumHotWaterorSteamFlowRate.get());
    }
    return result;
  }

  void AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl::autosizeMaximumHotWaterorSteamFlowRate() {
    bool result = setString(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::MaximumHotWaterorSteamFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl::setMinimumHotWaterorSteamFlowRate(double minimumHotWaterorSteamFlowRate) {
    bool result = setDouble(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::MinimumHotWaterorSteamFlowRate, minimumHotWaterorSteamFlowRate);
    return result;
  }

  void AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl::setAirOutlet(std::string airOutlet) {
    bool result = setString(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::AirOutlet, airOutlet);
    OS_ASSERT(result);
  }

  bool AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl::setConvergenceTolerance(double convergenceTolerance) {
    bool result = setDouble(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::ConvergenceTolerance, convergenceTolerance);
    return result;
  }

  bool AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl::setMaximumReheatAirTemperature(double maximumReheatAirTemperature) {
    bool result = setDouble(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::MaximumReheatAirTemperature, maximumReheatAirTemperature);
    return result;
  }

  boost::optional<HVACComponent> AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl::optionalReheatCoil() const {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::ReheatCoil);
  }

  unsigned AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl::inletPort()
  {
    return OS_AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::AirInlet;
  }

  unsigned AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl::outletPort()
  {
    return OS_AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::AirOutlet;
  }

  bool AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl::addToNode(Node & node)
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
                AirTerminalSingleDuctVAVHeatAndCoolReheat mo = this->getObject<AirTerminalSingleDuctVAVHeatAndCoolReheat>();

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

  std::vector<IdfObject> AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl::remove()
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

  bool AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl::isRemovable() const
  {
    return true;
  }

  std::vector<ModelObject> AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl::children() const
  {
    std::vector<ModelObject> result;
    if (OptionalHVACComponent intermediate = optionalReheatCoil()) {
      result.push_back(*intermediate);
    }
    return result;
  }

  ModelObject AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl::clone(Model model) const
  {
    AirTerminalSingleDuctVAVHeatAndCoolReheat modelObjectClone = StraightComponent_Impl::clone(model).cast<AirTerminalSingleDuctVAVHeatAndCoolReheat>();

    HVACComponent coil = this->reheatCoil();

    HVACComponent coilClone = coil.clone(model).cast<HVACComponent>();

    modelObjectClone.setReheatCoil(coilClone);

    return modelObjectClone;
  }

} // detail

AirTerminalSingleDuctVAVHeatAndCoolReheat::AirTerminalSingleDuctVAVHeatAndCoolReheat(const Model& model, const HVACComponent& reheatCoil)
  : StraightComponent(AirTerminalSingleDuctVAVHeatAndCoolReheat::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl>());

  if( ! setReheatCoil(reheatCoil) ) {
    LOG_AND_THROW(briefDescription() << " was consructed with invalid reheat coil: " << reheatCoil.briefDescription());
  }

  autosizeMaximumAirFlowRate();
  setZoneMinimumAirFlowFraction(0.0);
  autosizeMaximumHotWaterorSteamFlowRate();
  setMinimumHotWaterorSteamFlowRate(0.0);
  setConvergenceTolerance(0.001);
  setMaximumReheatAirTemperature(1000.0);
}

IddObjectType AirTerminalSingleDuctVAVHeatAndCoolReheat::iddObjectType() {
  return IddObjectType(IddObjectType::OS_AirTerminal_SingleDuct_VAV_HeatAndCool_Reheat);
}

boost::optional<Schedule> AirTerminalSingleDuctVAVHeatAndCoolReheat::availabilitySchedule() const {
  return getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl>()->availabilitySchedule();
}

boost::optional<double> AirTerminalSingleDuctVAVHeatAndCoolReheat::maximumAirFlowRate() const {
  return getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl>()->maximumAirFlowRate();
}

bool AirTerminalSingleDuctVAVHeatAndCoolReheat::isMaximumAirFlowRateAutosized() const {
  return getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl>()->isMaximumAirFlowRateAutosized();
}

double AirTerminalSingleDuctVAVHeatAndCoolReheat::zoneMinimumAirFlowFraction() const {
  return getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl>()->zoneMinimumAirFlowFraction();
}

HVACComponent AirTerminalSingleDuctVAVHeatAndCoolReheat::reheatCoil() const {
  return getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl>()->reheatCoil();
}

boost::optional<double> AirTerminalSingleDuctVAVHeatAndCoolReheat::maximumHotWaterorSteamFlowRate() const {
  return getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl>()->maximumHotWaterorSteamFlowRate();
}

bool AirTerminalSingleDuctVAVHeatAndCoolReheat::isMaximumHotWaterorSteamFlowRateAutosized() const {
  return getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl>()->isMaximumHotWaterorSteamFlowRateAutosized();
}

double AirTerminalSingleDuctVAVHeatAndCoolReheat::minimumHotWaterorSteamFlowRate() const {
  return getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl>()->minimumHotWaterorSteamFlowRate();
}

double AirTerminalSingleDuctVAVHeatAndCoolReheat::convergenceTolerance() const {
  return getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl>()->convergenceTolerance();
}

double AirTerminalSingleDuctVAVHeatAndCoolReheat::maximumReheatAirTemperature() const {
  return getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl>()->maximumReheatAirTemperature();
}

bool AirTerminalSingleDuctVAVHeatAndCoolReheat::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl>()->setAvailabilitySchedule(schedule);
}

void AirTerminalSingleDuctVAVHeatAndCoolReheat::resetAvailabilitySchedule() {
  getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl>()->resetAvailabilitySchedule();
}

bool AirTerminalSingleDuctVAVHeatAndCoolReheat::setMaximumAirFlowRate(double maximumAirFlowRate) {
  return getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl>()->setMaximumAirFlowRate(maximumAirFlowRate);
}

void AirTerminalSingleDuctVAVHeatAndCoolReheat::autosizeMaximumAirFlowRate() {
  getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl>()->autosizeMaximumAirFlowRate();
}

bool AirTerminalSingleDuctVAVHeatAndCoolReheat::setZoneMinimumAirFlowFraction(double zoneMinimumAirFlowFraction) {
  return getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl>()->setZoneMinimumAirFlowFraction(zoneMinimumAirFlowFraction);
}

bool AirTerminalSingleDuctVAVHeatAndCoolReheat::setReheatCoil(const HVACComponent& heatingCoil) {
  return getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl>()->setReheatCoil(heatingCoil);
}

bool AirTerminalSingleDuctVAVHeatAndCoolReheat::setMaximumHotWaterorSteamFlowRate(double maximumHotWaterorSteamFlowRate) {
  return getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl>()->setMaximumHotWaterorSteamFlowRate(maximumHotWaterorSteamFlowRate);
}

void AirTerminalSingleDuctVAVHeatAndCoolReheat::autosizeMaximumHotWaterorSteamFlowRate() {
  getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl>()->autosizeMaximumHotWaterorSteamFlowRate();
}

bool AirTerminalSingleDuctVAVHeatAndCoolReheat::setMinimumHotWaterorSteamFlowRate(double minimumHotWaterorSteamFlowRate) {
  return getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl>()->setMinimumHotWaterorSteamFlowRate(minimumHotWaterorSteamFlowRate);
}

bool AirTerminalSingleDuctVAVHeatAndCoolReheat::setConvergenceTolerance(double convergenceTolerance) {
  return getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl>()->setConvergenceTolerance(convergenceTolerance);
}

bool AirTerminalSingleDuctVAVHeatAndCoolReheat::setMaximumReheatAirTemperature(double maximumReheatAirTemperature) {
  return getImpl<detail::AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl>()->setMaximumReheatAirTemperature(maximumReheatAirTemperature);
}

/// @cond
AirTerminalSingleDuctVAVHeatAndCoolReheat::AirTerminalSingleDuctVAVHeatAndCoolReheat(std::shared_ptr<detail::AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl> impl)
  : StraightComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

