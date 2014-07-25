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
class AvailabilityManagerAssignmentList;

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

  Node supplyInletNode() const;

  Node supplyOutletNode() const;

  std::vector<Node> supplyOutletNodes() const;

  Node demandInletNode() const;

  std::vector<Node> demandInletNodes() const;

  Node demandOutletNode() const;

  boost::optional<Node> reliefAirNode() const;

  boost::optional<Node> mixedAirNode() const;

  std::vector<ModelObject> oaComponents(openstudio::IddObjectType type = IddObjectType::Catchall);

  std::vector<ModelObject> components(openstudio::IddObjectType type = IddObjectType::Catchall);

  void addAirLoopComp(ModelObject targetObj, ModelObject newComp);

  void removeAirLoopComp(ModelObject targetObj);

  void replaceAirLoopSupplyComp(ModelObject targetObj, ModelObject newObj);

  boost::optional<AirLoopHVACOutdoorAirSystem> airLoopHVACOutdoorAirSystem() const;

  AirLoopHVACZoneMixer zoneMixer();

  AirLoopHVACZoneSplitter zoneSplitter();

  bool removeBranchForZone(openstudio::model::ThermalZone & thermalZone);

  virtual std::vector<openstudio::IdfObject> remove();

  virtual ModelObject clone(Model model) const;

  virtual const std::vector<std::string>& outputVariableNames() const;

  virtual IddObjectType iddObjectType() const;

  Splitter demandSplitter();

  Mixer demandMixer();

  bool addBranchForZone(openstudio::model::ThermalZone & thermalZone);

  bool addBranchForZone(ThermalZone & thermalZone, StraightComponent & airTerminal);

  bool addBranchForZone(ThermalZone & thermalZone, 
                        Splitter & splitter,
                        Mixer & mixer,
                        StraightComponent & airTerminal);

  bool addBranchForZone(ThermalZone & thermalZone, 
                        Splitter & splitter,
                        Mixer & mixer);

  bool addBranchForZoneImpl(openstudio::model::ThermalZone & thermalZone, 
                            boost::optional<StraightComponent> optAirTerminal);

  bool moveBranchForZone(ThermalZone & thermalZone,
                         Splitter & newSplitter);

  bool moveBranchForZone(ThermalZone & thermalZone,
                         Mixer & newMixer);

  bool addBranchForHVACComponent(HVACComponent airTerminal);

  SizingSystem sizingSystem() const;

  std::vector<ThermalZone> thermalZones();

  std::vector<ModelObject> children() const;

  Schedule availabilitySchedule() const;

  void setAvailabilitySchedule(Schedule & schedule);

  bool setNightCycleControlType(std::string nightCycle);

  std::string nightCycleControlType() const;

  static bool addBranchForZoneImpl(ThermalZone & thermalZone, 
                                   AirLoopHVAC & airLoopHVAC,
                                   Splitter & splitter,
                                   Mixer & mixer,
                                   boost::optional<StraightComponent> & optAirTerminal);

  static boost::optional<ThermalZone> zoneForLastBranch(Mixer & mixer);

  static boost::optional<StraightComponent> terminalForLastBranch(Mixer & mixer);

  static boost::optional<PlantLoop> plantForAirTerminal( HVACComponent & airTerminal );

  static void setPlantForAirTerminal( HVACComponent & airTerminal, PlantLoop & plantLoop );
  private:

  AvailabilityManagerAssignmentList availabilityManagerAssignmentList() const;

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

