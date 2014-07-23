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

#ifndef MODEL_AIRLOOPHVACOUTDOORAIRSYSTEM_HPP
#define MODEL_AIRLOOPHVACOUTDOORAIRSYSTEM_HPP

#include "ModelAPI.hpp"

#include "HVACComponent.hpp"
#include "StraightComponent.hpp"
#include "Connection.hpp"

namespace openstudio {
namespace model {

namespace detail {
  class AirLoopHVACOutdoorAirSystem_Impl;
} // detail

class AvailabilityManagerScheduled;
class ControllerOutdoorAir;
class Node;

/** AirLoopHVACOutdoorAirSystem is an HVACComponent that wraps the IDD object
 *  named "OS:AirLoopHVAC:OutdoorAirSystem"
 *
 *  The purpose of this class is to simplify the construction and manipulation
 *  of the EnergyPlus AirLoopHVAC:OutdoorAirSystem object.  
 */
class MODEL_API AirLoopHVACOutdoorAirSystem : public HVACComponent {
  public:

  /** Constructs a new AirLoopHVACOutdoorAirSystem object and places it inside the 
   *  model.     
   */
  explicit AirLoopHVACOutdoorAirSystem(Model& model, const ControllerOutdoorAir & controller);

  virtual ~AirLoopHVACOutdoorAirSystem() {}

  /** Returns the return air port of the outdoor air system. */
  unsigned returnAirPort() const;

  /** Returns the outdoor air port of the outdoor air mixer.
   *  This is not the most outboard air node of the outdoor air system,
   *  rather it is the port directly attached to the mixer.  There can be
   *  additional components between this port and the most outboard port.
   **/
  unsigned outdoorAirPort() const;

  /** Returns the outdoor air port of the outdoor air system.
   *  This is not the most outboard air node of the outdoor air system,
   *  rather it is the port directly attached to the mixer.  There can be
   *  additional components between this port and the most outboard port.
   **/
  unsigned reliefAirPort() const;

  /** Returns the mixed air port. **/
  unsigned mixedAirPort() const;

  /** Returns the optional ModelObject attached to the return air port. **/
  boost::optional<ModelObject> returnAirModelObject();

  /** Returns the optional ModelObject attached to the outdoor air port. **/
  boost::optional<ModelObject> outdoorAirModelObject();

  /** Returns the optional ModelObject attached to the relief air port. **/
  boost::optional<ModelObject> reliefAirModelObject();

  /** Returns the optional ModelObject attached to the mixer air port. **/
  boost::optional<ModelObject> mixedAirModelObject();

  /** Returns the most outboard outdoor air Node. **/
  boost::optional<Node> outboardOANode() const;

  /** Returns the most outboard relief air Node. **/
  boost::optional<Node> outboardReliefNode() const;

  /** Returns a vector of model objects that are on the path of the incoming outdoor air stream. **/
  std::vector<ModelObject> oaComponents() const;

  /** Returns a vector of model objects that are on the path of the outgoing relief air stream. **/
  std::vector<ModelObject> reliefComponents() const;
  
  /** Returns a vector that is the concatenation of oaComponents() and reliefComponents(). **/
  std::vector<ModelObject> components() const;
  
  /** Returns the optional ModelObject with the Handle given.  The optional
   *  will be false if the given handle does not correspond to the a ModelObject
   *  that is not part of the outdoor air system.
   **/
  boost::optional<ModelObject> component(openstudio::Handle handle);
  
  /** Returns the optional ModelObject with the Handle given.  The optional
   *  will be false if the given handle does not correspond to the a ModelObject
   *  that is not part of the supply side of the outdoor air system.
   **/
  boost::optional<ModelObject> oaComponent(openstudio::Handle handle);
  
  /** Returns the optional ModelObject with the Handle given.  The optional
   *  will be false if the given handle does not correspond to the a ModelObject
   *  that is not part of the supply side of the outdoor air system.
   **/
  boost::optional<ModelObject> reliefComponent(openstudio::Handle handle);
  
  virtual bool addToNode(Node & node);

  virtual std::vector<openstudio::IdfObject> remove();

  virtual ModelObject clone(Model model) const;

  /** Returns the ControllerOutdoorAir object associated with the AirLoopHVACOutdoorAirSystem. **/
  ControllerOutdoorAir getControllerOutdoorAir() const;

  /** Sets the ControllerOutdoorAir object associated with the AirLoopHVACOutdoorAirSystem. **/
  void setControllerOutdoorAir(const ControllerOutdoorAir & controllerOutdoorAir );

  /** Reimplemented from HVACComponent. **/
  boost::optional<AirLoopHVAC> airLoop() const;

  static IddObjectType iddObjectType();

 protected:

  friend class Model;

  friend class openstudio::IdfObject;

  /// @cond 
  
  typedef detail::AirLoopHVACOutdoorAirSystem_Impl ImplType;

  explicit AirLoopHVACOutdoorAirSystem(
      std::shared_ptr<detail::AirLoopHVACOutdoorAirSystem_Impl> impl);

 private:

  REGISTER_LOGGER("openstudio.model.AirLoopHVACOutdoorAirSystem");

  /// @endcond

};

/** \relates AirLoopHVACOutdoorAirSystem */
typedef boost::optional<AirLoopHVACOutdoorAirSystem> OptionalAirLoopHVACOutdoorAirSystem;

/** \relates AirLoopHVACOutdoorAirSystem */
typedef std::vector<AirLoopHVACOutdoorAirSystem> AirLoopHVACOutdoorAirSystemVector;

} // model

} // openstudio

#endif // MODEL_AIRLOOPHVACOUTDOORAIRSYSTEM_HPP
