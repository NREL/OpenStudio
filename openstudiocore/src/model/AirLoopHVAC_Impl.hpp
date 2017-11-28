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

#ifndef MODEL_AIRLOOPHVAC_IMPL_HPP
#define MODEL_AIRLOOPHVAC_IMPL_HPP

#include "Loop_Impl.hpp"

namespace openstudio {
namespace model {

class AirLoopHVAC;
class Node;
class AirLoopHVACOutdoorAirSystem;
class AirLoopHVACZoneSplitter;
class AirLoopHVACZoneMixer;
class AirLoopHVACSupplyPlenum;
class AirLoopHVACReturnPlenum;
class ThermalZone;
class PlantLoop;
class SizingSystem;
class StraightComponent;
class AvailabilityManagerScheduled;
class AvailabilityManager;

namespace detail {

class Model_Impl;

class MODEL_API AirLoopHVAC_Impl : public Loop_Impl {

 public:

  AirLoopHVAC_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

  AirLoopHVAC_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                   Model_Impl* model,
                   bool keepHandle);

  AirLoopHVAC_Impl(const AirLoopHVAC_Impl& other, Model_Impl* model, bool keepHandle);

  virtual ~AirLoopHVAC_Impl() {}

  boost::optional<double> designSupplyAirFlowRate() const;

  OSOptionalQuantity getDesignSupplyAirFlowRate(bool returnIP=false) const;

  bool isDesignSupplyAirFlowRateAutosized() const;

  void setDesignSupplyAirFlowRate(boost::optional<double> designSupplyAirFlowRate);

  bool setDesignSupplyAirFlowRate(const OSOptionalQuantity& designSupplyAirFlowRate);

  void resetDesignSupplyAirFlowRate();

  void autosizeDesignSupplyAirFlowRate();

  Node supplyInletNode() const override;

  Node supplyOutletNode() const override;

  std::vector<Node> supplyOutletNodes() const override;

  Node demandInletNode() const override;

  std::vector<Node> demandInletNodes() const override;

  Node demandOutletNode() const override;

  boost::optional<Node> reliefAirNode() const;

  boost::optional<Node> mixedAirNode() const;

  std::vector<ModelObject> oaComponents(openstudio::IddObjectType type = openstudio::IddObjectType("Catchall"));

  std::vector<ModelObject> components(openstudio::IddObjectType type = openstudio::IddObjectType("Catchall")) override;

  boost::optional<AirLoopHVACOutdoorAirSystem> airLoopHVACOutdoorAirSystem() const;

  boost::optional<Splitter> supplySplitter() const;

  bool setSupplySplitter(Splitter const & splitter);

  void resetSupplySplitter();

  bool removeSupplySplitter();

  bool removeSupplySplitter(HVACComponent & hvacComponent);

  boost::optional<Node> supplySplitterInletNode() const;

  std::vector<Node> supplySplitterOutletNodes() const;

  AirLoopHVACZoneMixer zoneMixer() const;

  bool setZoneMixer(Mixer const & mixer);

  AirLoopHVACZoneSplitter zoneSplitter() const;

  bool setZoneSplitter(Splitter const & splitter, int path);

  void resetZoneSplitter(int path);

  std::vector<AirLoopHVACZoneSplitter> zoneSplitters() const;

  bool removeBranchForZone(openstudio::model::ThermalZone & thermalZone);

  virtual std::vector<openstudio::IdfObject> remove() override;

  virtual ModelObject clone(Model model) const override;

  virtual const std::vector<std::string>& outputVariableNames() const override;

  virtual IddObjectType iddObjectType() const override;

  virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

  Splitter demandSplitter() override;

  Mixer demandMixer() override;

  boost::optional<HVACComponent> supplyFan() const;

  boost::optional<HVACComponent> returnFan() const;

  boost::optional<HVACComponent> reliefFan() const;

  bool addBranchForZone(openstudio::model::ThermalZone & thermalZone);

  bool addBranchForZone(ThermalZone & thermalZone, HVACComponent & airTerminal);

  bool addBranchForZone(ThermalZone & thermalZone,
                        Splitter & splitter,
                        Mixer & mixer,
                        HVACComponent & airTerminal);

  bool addBranchForZone(ThermalZone & thermalZone,
                        Splitter & splitter,
                        Mixer & mixer);

  bool addBranchForZoneImpl(openstudio::model::ThermalZone & thermalZone,
                            boost::optional<StraightComponent> & optAirTerminal);

  bool addBranchForZoneImpl(openstudio::model::ThermalZone & thermalZone,
                            boost::optional<HVACComponent> & optAirTerminal);

  bool moveBranchForZone(ThermalZone & thermalZone,
                         Splitter & newSplitter);

  bool moveBranchForZone(ThermalZone & thermalZone,
                         Mixer & newMixer);

  bool addBranchForHVACComponent(HVACComponent airTerminal);

  SizingSystem sizingSystem() const;

  std::vector<ThermalZone> thermalZones() const;

  std::vector<ModelObject> children() const override;

  Schedule availabilitySchedule() const;

  void setAvailabilitySchedule(Schedule & schedule);

  bool setNightCycleControlType(std::string const & nightCycle);

  std::string nightCycleControlType() const;

  // boost::optional<Schedule> returnAirBypassFlowTemperatureSetpointSchedule() const;

  // bool setReturnAirBypassFlowTemperatureSetpointSchedule(Schedule & temperatureSetpointSchedule);

  // void resetReturnAirBypassFlowTemperatureSetpointSchedule();

  static bool addBranchForZoneImpl(ThermalZone & thermalZone,
                                   AirLoopHVAC & airLoopHVAC,
                                   Splitter & splitter,
                                   Mixer & mixer,
                                   boost::optional<HVACComponent> & optAirTerminal);

  static boost::optional<ThermalZone> zoneForLastBranch(Mixer & mixer);

  static boost::optional<HVACComponent> terminalForLastBranch(Mixer & mixer);

  static boost::optional<PlantLoop> plantForAirTerminal( HVACComponent & airTerminal );

  static void setPlantForAirTerminal( HVACComponent & airTerminal, PlantLoop & plantLoop );

  unsigned supplyOutletPortA() const;

  unsigned supplyOutletPortB() const;

  unsigned supplyInletPort() const;

  unsigned demandInletPortA() const;

  unsigned demandInletPortB() const;

  unsigned demandOutletPort() const;

  static bool addDualDuctTerminalToNode(HVACComponent & terminal, const unsigned inletPortA, const unsigned inletPortB, const unsigned outletPort, Node & node);

  static bool removeDualDuctTerminalFromAirLoopHVAC(HVACComponent & terminal, const unsigned inletPortA, const unsigned inletPortB, const unsigned outletPort);

  bool isDualDuct() const;

  boost::optional<AvailabilityManager> availabilityManager() const;

  bool setAvailabilityManager(const AvailabilityManager & availabilityManager);

  void resetAvailabilityManager();

  private:

  REGISTER_LOGGER("openstudio.model.AirLoopHVAC");

  // TODO: Make these const.
  std::vector<ModelObject> supplyOutletNodesAsModelObjects();
  std::vector<ModelObject> demandInletNodesAsModelObjects();
  boost::optional<ModelObject> demandOutletNodeAsModelObject();
  boost::optional<ModelObject> reliefAirNodeAsModelObject();
  boost::optional<ModelObject> zoneMixerAsModelObject();
  boost::optional<ModelObject> zoneSplitterAsModelObject();
};

} // detail

} // model

} // openstudio

#endif

