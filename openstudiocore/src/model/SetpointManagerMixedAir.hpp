/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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
  bool setReferenceSetpointNode( Node & node );

  /** Returns the Node referred to by the fanInletNodeName. **/
  boost::optional<Node> fanInletNode();

  /** Sets the Node referred to by the fanInletNodeName. **/
  bool setFanInletNode( Node & node );

  /** Returns the Node referred to by the fanOutletNodeName. **/
  boost::optional<Node> fanOutletNode();

  /** Sets the Node referred to by the fanOutletNodeName. **/
  bool setFanOutletNode( Node & node );

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

