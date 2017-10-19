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
class AvailabilityManager;

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
   *
   * If dualDuct is true, then there will be a supply spiltter and two supply outlet nodes
   * connected into the system topology.
   *
   * The two paths / ducts / decks of a dual duct style system are identified by indexes
   * 0 and 1.  If dual duct, key methods including supplyOutletNodes(), demandInletNodes(), and zoneSplitters(),
   * will return a vector with exactly two items.  Item 0 in the supplyOutletNodes vector matches to item
   * 0 in the demandInletNodes vector, and the zoneSplitters vector.  In this way the interface avoids
   * declaring the paths as heating, cooling, ventilation, etc.  It is the client's responsibilty to choose
   * what types of equipment to apply to the two branches.  Note that these indexes carry through to the terminals.
   * Dual duct terminals are Mixer objects which have the method inletModelObjects(branchIndex).  The branchIndex 0
   * will be connected to demandInletNodes()[0] path.
   */
  explicit AirLoopHVAC(Model& model, bool dualDuct = false);

  virtual ~AirLoopHVAC() {}


  boost::optional<double> designSupplyAirFlowRate() const;

  OSOptionalQuantity getDesignSupplyAirFlowRate(bool returnIP=false) const;

  bool isDesignSupplyAirFlowRateAutosized() const;

  void setDesignSupplyAirFlowRate(double designSupplyAirFlowRate);

  bool setDesignSupplyAirFlowRate(const Quantity& designSupplyAirFlowRate);

  void resetDesignSupplyAirFlowRate();

  void autosizeDesignSupplyAirFlowRate();


  /** Returns the supply inlet node. */
  Node supplyInletNode() const override;

  /** Returns the supply outlet nodes.
   * Single duct systems have one supply outlet node.
   * Dual duct systems have two supply outlet nodes.
   */
  std::vector<Node> supplyOutletNodes() const override;

  /** Returns the first supply outlet Node. */
  Node supplyOutletNode() const override;

  /** Returns the demand inlet nodes.
   * Single duct systems have one demand inlet node.
   * Dual duct systems have two demand inlet nodes.
   */
  std::vector<Node> demandInletNodes() const override;

  /** Returns the first demand inlet Node. */
  Node demandInletNode() const override;

  /** Returns the demand outlet node */
  Node demandOutletNode() const override;

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

  /** Returns true if the system is "dual duct"
    * ie. there is a supply splitter
    */
  bool isDualDuct() const;

  /** Returns the supply side splitter.
    * If the system is a dual duct then it will have a supply side splitter.
    */
  boost::optional<Splitter> supplySplitter() const;

  /** If this is a dual duct system, remove the supply side splitter.
    * If this is not a dual duct system, there is no supply side splitter and the method will return false.
    *
    * The system will become a single duct. Dual duct terminals may remain on the demand side, and those must be
    * resolved separately by removing the zones served by dual ducts or changing to single duct terminals.
    *
    * The components downstream of the splitter will also be removed.
    */
  bool removeSupplySplitter();

  /** If this is a dual duct system, remove the supply side splitter.
    * If this is not a dual duct system, there is no supply side splitter and the method will return false.
    *
    * The system will become a single duct. Dual duct terminals may remain on the demand side, and those must be
    * resolved separately by removing the zones served by dual ducts or changing to single duct terminals.
    *
    * The dual duct branch containing hvacComponent will be removed.
    * The remaining branch will be integrated into the loop.
    * If hvacComponent is not found on either dual duct branch
    * the method will return false. This will be the case if hvacComponent is not found on the system's supplyComponents(),
    * or upstream of the splitter.
    */
  bool removeSupplySplitter(HVACComponent & hvacComponent);

  /** Returns the supply side splitter inlet node.  If the system is a dual duct
   * or has a return air bypass then it will have a supply side splitter.
   */
  boost::optional<Node> supplySplitterInletNode() const;

  /** Returns the supply side splitter outlet nodes.  If the system is a dual duct
   * or has a return air bypass then it will have a supply side splitter.
   */
  std::vector<Node> supplySplitterOutletNodes() const;

  /** Returns the first zone splitter.
   */
  AirLoopHVACZoneSplitter zoneSplitter() const;

  /** Returns the zone splitters.
   * Single duct systems have one zone splitter, dual duct systems have two.
   */
  std::vector<AirLoopHVACZoneSplitter> zoneSplitters() const;

  /** Returns the zone mixer, if it doesn't exist then it makes one. */
  AirLoopHVACZoneMixer zoneMixer() const;

  /** Returns all of the components on the outdoor air system including the mixer itself.
   *  If type is given then the results will be limited to the given IddObjectType.
   */
  std::vector<ModelObject> oaComponents(openstudio::IddObjectType type = openstudio::IddObjectType("Catchall"));

  /** Returns the AirLoopHVACOutdoorAirSystem object associated with the air loop.
   * A freshly constructed AirLoopHVAC object will not have an outdoor air system
   * and this method will return a false optional.
   */
  boost::optional<AirLoopHVACOutdoorAirSystem> airLoopHVACOutdoorAirSystem() const;

  /** Returns the fan in the mixed air stream (after outdoor air system) of the air system.
   *  If there is no outdoor air system or there are multiple fans in the mixed air stream,
   *  then the fan closest to the supply outlet node will be returned.
   */
  boost::optional<HVACComponent> supplyFan() const;

  /** Returns the fan in the return air stream (before the outdoor air system.
   *  If there is no outdoor air system then this method will return false.
   *  If there are multiple fans, then return the fan nearest the oa system.
   */
  boost::optional<HVACComponent> returnFan() const;

  /** Returns the most outboard fan on the relief air stream of the outdoor air system */
  boost::optional<HVACComponent> reliefFan() const;

  /** Adds a new branch on the demand side of the air loop for a zone labeled zoneLabel
   * and returns true if the operation was successful. The method will return false if the Zone
   * is already connected to an air loop.
   * New code should favor addBranchForZone(ThermalZone &) or addBranchForZone(ThermalZone &, HVACComponent&).
   */
  bool addBranchForZone(openstudio::model::ThermalZone & thermalZone,
                        boost::optional<StraightComponent> optAirTerminal);

  /** Overloaded version of addBranchForZone() **/
  bool addBranchForZone(openstudio::model::ThermalZone & thermalZone);

  /** Overloaded version of addBranchForZone() **/
  bool addBranchForZone(ThermalZone & thermalZone, HVACComponent & airTerminal);

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
  std::vector<ThermalZone> thermalZones() const;

  /** Returns the availability schedule when this system is allowed to run. **/
  Schedule availabilitySchedule() const;

  /** Set the availability schedule for when this system is allowed to run. **/
  void setAvailabilitySchedule(Schedule & schedule);

  /** Configure the system to night cycle
    * This is a convenience for creating and attaching a new AvailabilityManagerNightCycle.
    * Valid options are StayOff, CycleOnAny, and CycleOnAnyZoneFansOnly **/
  bool setNightCycleControlType(std::string const & controlType);

  /** Returns a string indicating if the system is configured to night cycle
    * If there is no AvailabilityManagerNightCycle this method will return StayOff **/
  std::string nightCycleControlType() const;

  // returnAirBypassFlowTemperatureSetpointSchedule is not possible currently in OS, uncomment in future

  /** Returns the temperature setpoint schedule when this system tries to determine the required mass
   *  flow rate through a return air bypass duct. If there is a temperature setpoint schedule,
   *  the forward translator will create a SetpointManager:ReturnAirBypassFlow object. **/
  // boost::optional<Schedule> returnAirBypassFlowTemperatureSetpointSchedule() const;

  /** Set the temperature setpoint schedule for when this system tries to determine the required mass
   *  flow rate through a return air bypass duct. **/
  // bool setReturnAirBypassFlowTemperatureSetpointSchedule(Schedule & temperatureSetpointSchedule);

  /** Reset the temperature setpoint schedule for when this system tries to determine the required mass
   *  flow rate through a return air bypass duct. **/
  // void resetReturnAirBypassFlowTemperatureSetpointSchedule();

  /** AvailabilityManager is used to override the system availabilitySchedule() with one of OpenStudio's
    * supported AvailabilityManager types.
    * Unlike EnergyPlus which supports layering multiple availability managers on an AvailabilityManagerAssignmentList,
    * OpenStudio allows only one AvailabilityManager at a time.
    **/
  boost::optional<AvailabilityManager> availabilityManager() const;

  bool setAvailabilityManager(const AvailabilityManager& availabilityManager);

  void resetAvailabilityManager();

  std::vector<openstudio::IdfObject> remove() override;

  ModelObject clone(Model model) const override;

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
