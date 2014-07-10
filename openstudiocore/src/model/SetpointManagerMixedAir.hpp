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

#ifndef MODEL_SETPOINTMANAGERMIXEDAIR_HPP
#define MODEL_SETPOINTMANAGERMIXEDAIR_HPP

#include "ModelAPI.hpp"
#include "SetpointManager.hpp"
#include <boost/optional.hpp>

namespace openstudio {

namespace model {

namespace detail {

  class SetpointManagerMixedAir_Impl;

}

/** SetpointManagerMixedAir is an interface to the IDD object
 *  named "OS:SetpointManager:MixedAir"
 *
 *  The purpose of this class is to simplify the construction and manipulation
 *  of the EnergyPlus SetpointManager:MixedAir object.
 */
class MODEL_API SetpointManagerMixedAir : public SetpointManager
{
  public:
  virtual ~SetpointManagerMixedAir() {}

  /** Constructs a new SetpointManagerMixedAir object and places it inside the
   *  model.  The loop is fully initialized with all companion objects.
   */
  explicit SetpointManagerMixedAir(const Model& model);

  /** Returns the value of the ControlVariable field. **/
  std::string controlVariable() const;

  /** Sets the value of the ControlVariable field.
   *  Options are Temperature.
   */
  bool setControlVariable( const std::string& controlVariable );

  /** Returns the Node referred to by the referenceSetpointNodeName. **/
  boost::optional<Node> referenceSetpointNode();

  /** Sets the Node referred to by the referenceSetpointNodeName. **/
  void setReferenceSetpointNode( Node & node );

  /** Returns the Node referred to by the fanInletNodeName. **/
  boost::optional<Node> fanInletNode();

  /** Sets the Node referred to by the fanInletNodeName. **/
  void setFanInletNode( Node & node );

  /** Returns the Node referred to by the fanOutletNodeName. **/
  boost::optional<Node> fanOutletNode();

  /** Sets the Node referred to by the fanOutletNodeName. **/
  void setFanOutletNode( Node & node );

  /** Returns the Node this setpoint manager is acting on. **/
  boost::optional<Node> setpointNode() const;

  static IddObjectType iddObjectType();

  static void updateFanInletOutletNodes(AirLoopHVAC & airLoopHVAC);

  protected:

  friend class Model;

  friend class openstudio::IdfObject;

  friend class openstudio::detail::IdfObject_Impl;

  friend class detail::SetpointManagerMixedAir_Impl;

  /// @cond

  typedef detail::SetpointManagerMixedAir_Impl ImplType;

  explicit SetpointManagerMixedAir(std::shared_ptr<detail::SetpointManagerMixedAir_Impl> impl);

  private:

  REGISTER_LOGGER("openstudio.model.SetpointManagerMixedAir");

  /// @endcond

};

typedef boost::optional<SetpointManagerMixedAir> OptionalSetpointManagerMixedAir;

typedef std::vector<SetpointManagerMixedAir> SetpointManagerMixedAirVector;

} // model

} // openstudio

#endif // MODEL_SETPOINTMANAGERMIXEDAIR_HPP

