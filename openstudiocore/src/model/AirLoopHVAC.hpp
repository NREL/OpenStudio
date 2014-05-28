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

#ifndef MODEL_AIRLOOPHVAC_HPP
#define MODEL_AIRLOOPHVAC_HPP

#include "ModelAPI.hpp"
#include "Loop.hpp"

namespace openstudio {

namespace model {

namespace detail {
  class AirLoopHVAC_Impl;
};

class Node;
class AirLoopHVACOutdoorAirSystem;
class AirLoopHVACZoneSplitter;
class AirLoopHVACZoneMixer;
class AirLoopHVACSupplyPlenum;
class AirLoopHVACReturnPlenum;
class StraightComponent;
class ThermalZone;
class SizingSystem;

/** AirLoopHVAC is an interface to the EnergyPlus IDD object named "AirLoopHVAC"
 *
 * The purpose of this class is to simplify the construction and manipulation
 * air loops in energyplus.  Presently, only single zone air loops with one
 * supply branch are supported, however more complicated air loop topologies
 * are planned, including all of those supported by EnergyPlus.
 */
class MODEL_API AirLoopHVAC : public Loop
{
  public:

  /** Constructs a new AirLoopHVAC object and places it inside the
   * model.  The air loop is fully initialized with the companion objects
   * that are needed for a basic air loop in EnergyPlus.  These objects include:
   * supply inlet node, supply outlet node, demand inlet node, demand outlet node,
   * zone splitter, zone mixer, and zone hvac equipment connections.
   */
  explicit AirLoopHVAC(Model& model);

  virtual ~AirLoopHVAC() {}


  boost::optional<double> designSupplyAirFlowRate() const;

  OSOptionalQuantity getDesignSupplyAirFlowRate(bool returnIP=false) const;

  bool isDesignSupplyAirFlowRateAutosized() const;

  void setDesignSupplyAirFlowRate(double designSupplyAirFlowRate);

  bool setDesignSupplyAirFlowRate(const Quantity& designSupplyAirFlowRate);

  void resetDesignSupplyAirFlowRate();

  void autosizeDesignSupplyAirFlowRate();


  /** Returns the supply inlet node. */
  Node supplyInletNode();

  /** Returns the supply outlet nodes.
   * Currently only one supply outlet node is supported, but EnergyPlus allows
   * up to two for dual duct systems.
   */
  std::vector<Node> supplyOutletNodes();

  /** Returns the first supply outlet Node. */
  Node supplyOutletNode();

  /** Returns the demand inlet nodes.
   * Currently only one demand inlet node is supported, but EnergyPlus allows
   * up to two for dual duct systems.
   */
  std::vector<Node> demandInletNodes();

  /** Returns the first demand inlet Node. */
  Node demandInletNode();

  /** Returns the demand outlet node */
  Node demandOutletNode();

  /** Returns the outdoor air node.  This is the outermost node from which
   * outdoor air is introduced into the air loop.  This node only exists if there
   * is an outdoor air system within the air loop.  A freshly constructed
   * AirLoopHVAC object will not have an outdoor air system.
   */
  boost::optional<Node> outdoorAirNode();

  /** Returns the relief air node.  This is the outermost node from which
   * air is relieved from the air loop to the outdoor air.  This node only exists
   * if there is an outdoor air system within the air loop.  A freshly
   * constructed AirLoopHVAC object will not have an outdoor air system.
   */
  boost::optional<Node> reliefAirNode();

  /** Returns the mixed air node.  This is the mixed air node of the outdoor
   * air mixer of the air loop.  This node only exists if there is an outdoor air
   * system within the air loop.  A freshly constructed AirLoopHVAC object
   * will not have an outdoor air system.
   */
  boost::optional<Node> mixedAirNode();

  /** Returns the return air node.  This is the return air node of the outdoor
   * air mixer of the air loop.  This node only exists if there is an outdoor air
   * system within the air loop.  A freshly constructed AirLoopHVAC object
   * will not have an outdoor air system.  In many cases this node will be the
   * same as the supply inlet node, because there is often no hvac equipment
   * before the outdoor air mixer.
   */
  boost::optional<Node> returnAirNode();

  /** Returns the supply side splitter inlet node.  If the system is a dual duct
   * or has a return air bypass then it will have a supply side splitter.
   * Currently these air loop topologies are not suppported so this method
   * will always return a false optional.
   */
  boost::optional<Node> supplySplitterInletNode();

  /** Returns the supply side splitter outlet nodes.  If the system is a dual duct
   * or has a return air bypass then it will have a supply side splitter.
   * Currently these air loop topologies are not suppported so this method
   * will always return a false optional.
   */
  std::vector<Node> supplySplitterOutletNodes();

  /** Returns the supply side mixer inlet nodes.  If the system has a return
   * air bypass then it will have a mixer on the supply side.  Currently,
   * return air bypass topology is not supported so this method will
   * always return a false optional.
   */
  std::vector<Node> supplyMixerInletNodes();

  /** Returns the supply side mixer outlet node.  If the system has a return
   * air bypass then it will have a mixer on the supply side.  Currently,
   * return air bypass topology is not supported so this method will
   * always return a false optional.
   */
  boost::optional<Node> supplyMixerOutletNode();

  /** Returns the zone splitter, if it doesn't exist then it makes one. */
  AirLoopHVACZoneSplitter zoneSplitter();

  /** Returns the zone splitter inlet node. */
  boost::optional<Node> zoneSplitterInletNode(int zoneSplitterIndex);

  /** Returns the zone splitter outlet nodes. */
  std::vector<Node> zoneSplitterOutletNodes(int zoneSplitterIndex);

  /** Returns the zone mixer, if it doesn't exist then it makes one. */
  AirLoopHVACZoneMixer zoneMixer();

  /** Returns the zone mixer inlet nodes. */
  std::vector<Node> zoneMixerInletNodes();

  /** Returns the zone mixer outlet nodes. */
  boost::optional<Node> zoneMixerOutletNode();

  /** Returns all of the components on the outdoor air system including the mixer itself.
   *  If type is given then the results will be limited to the given IddObjectType.
   */
  std::vector<ModelObject> oaComponents(openstudio::IddObjectType type = IddObjectType::Catchall);

  /** Adds a new component to the air loop.  In most cases the newObj will be added immediatly after the
   * targetObj.  The notable exceptions are when the targetObj is either a supply outlet node or a
   * demand outlet node.  This interface is deprecated in favor of HVACComponent::addToNode.
   */
  void addAirLoopComp(openstudio::model::ModelObject targetObj, openstudio::model::ModelObject newObj);

  /** Returns the AirLoopHVACOutdoorAirSystem object associated with the air loop.
   * A freshly constructed AirLoopHVAC object will not have an outdoor air system
   * and this method will return a false optional.
   */
  boost::optional<AirLoopHVACOutdoorAirSystem> airLoopHVACOutdoorAirSystem() const;

  /** Adds a new branch on the demand side of the air loop for a zone labeled zoneLabel
   * and returns true if the operation was successful. The method will return false if the Zone
   * is already connected to an air loop.
   */
  bool addBranchForZone(openstudio::model::ThermalZone & thermalZone,
                        boost::optional<StraightComponent> optAirTerminal);

  /** Overloaded version of addBranchForZone() **/
  bool addBranchForZone(openstudio::model::ThermalZone & thermalZone);

  /** Overloaded version of addBranchForZone() **/
  bool addBranchForZone(ThermalZone & thermalZone, StraightComponent & airTerminal);

  /** Adds a new branch on the demand side of the air loop with the specified airTerminal.
   *  Returns true if the airTerminal was accepted, otherwise false.  The argument, hvacComponent,
   *  can be an air terminal, AirLoopHVACSupplyPlenum, or airLoopHVACReturnPlenum.
   **/
  bool addBranchForHVACComponent(HVACComponent hvacComponent);

  /** Removes the Zone identified by zoneLabel from the air loop and returns true
   * upon successful removal.
   **/
  bool removeBranchForZone(openstudio::model::ThermalZone & thermalZone);

  /** Returns the Sizing:System object associated with this air loop. **/
  SizingSystem sizingSystem() const;

  /** Returns the ThermalZone objects attached to this air loop. **/
  std::vector<ThermalZone> thermalZones();

  /** Returns the availability schedule when this system is allowed to run. **/
  Schedule availabilitySchedule() const;

  /** Set the availability schedule for when this system is allowed to run. **/
  void setAvailabilitySchedule(Schedule & schedule);

  /** Configure the system to night cycle
   *  Valid options are StayOff, CycleOnAny, and CycleOnAnyZoneFansOnly **/
  bool setNightCycleControlType(std::string controlType);

  /** Returns a string indicating if the system is configured to night cycle **/
  std::string nightCycleControlType() const;

  std::vector<openstudio::IdfObject> remove();

  ModelObject clone(Model model) const;

  static IddObjectType iddObjectType();

 protected:

  friend class Model;

  friend class openstudio::IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  /// @cond

  typedef detail::AirLoopHVAC_Impl ImplType;

  explicit AirLoopHVAC(std::shared_ptr<ImplType> impl);

  private:

  REGISTER_LOGGER("openstudio.model.AirLoopHVAC");

  /// @endcond

};

// optional AirLoopHVAC
typedef boost::optional<AirLoopHVAC> OptionalAirLoopHVAC;

// vector of AirLoopHVAC
typedef std::vector<AirLoopHVAC> AirLoopHVACVector;

} // model

} // openstudio

#endif // MODEL_AIRLOOPHVAC_HPP
