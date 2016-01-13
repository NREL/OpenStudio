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
  Q_OBJECT;

  Q_PROPERTY(std::vector<openstudio::model::ModelObject> supplyOutletNodes READ supplyOutletNodesAsModelObjects);
  Q_PROPERTY(std::vector<openstudio::model::ModelObject> demandInletNodes READ demandInletNodesAsModelObjects);
  Q_PROPERTY(boost::optional<openstudio::model::ModelObject> demandOutletNode READ demandOutletNodeAsModelObject);
  Q_PROPERTY(boost::optional<openstudio::model::ModelObject> reliefAirNode READ reliefAirNodeAsModelObject);
  Q_PROPERTY(boost::optional<openstudio::model::ModelObject> zoneMixer READ zoneMixerAsModelObject);
  Q_PROPERTY(boost::optional<openstudio::model::ModelObject> zoneSplitter READ zoneSplitterAsModelObject);

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

  bool removeSupplySplitter();

  bool removeSupplySplitter(HVACComponent & hvacComponent);

  boost::optional<Node> supplySplitterInletNode() const;

  std::vector<Node> supplySplitterOutletNodes() const;

  AirLoopHVACZoneMixer zoneMixer();

  AirLoopHVACZoneSplitter zoneSplitter() const;

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

  bool setNightCycleControlType(std::string nightCycle);

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

