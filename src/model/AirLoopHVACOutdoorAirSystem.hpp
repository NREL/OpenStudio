/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
  }  // namespace detail

  class AvailabilityManagerScheduled;
  class ControllerOutdoorAir;
  class Node;
  class AirflowNetworkDistributionNode;
  class AirLoopHVACDedicatedOutdoorAirSystem;

  /** AirLoopHVACOutdoorAirSystem is an HVACComponent that wraps the IDD object
 *  named "OS:AirLoopHVAC:OutdoorAirSystem"
 *
 *  The purpose of this class is to simplify the construction and manipulation
 *  of the EnergyPlus AirLoopHVAC:OutdoorAirSystem object.
 */
  class MODEL_API AirLoopHVACOutdoorAirSystem : public HVACComponent
  {
   public:
    /** Constructs a new AirLoopHVACOutdoorAirSystem object and places it inside the
   *  model.
   */
    explicit AirLoopHVACOutdoorAirSystem(Model& model, const ControllerOutdoorAir& controller);

    virtual ~AirLoopHVACOutdoorAirSystem() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirLoopHVACOutdoorAirSystem(const AirLoopHVACOutdoorAirSystem& other) = default;
    AirLoopHVACOutdoorAirSystem(AirLoopHVACOutdoorAirSystem&& other) = default;
    AirLoopHVACOutdoorAirSystem& operator=(const AirLoopHVACOutdoorAirSystem&) = default;
    AirLoopHVACOutdoorAirSystem& operator=(AirLoopHVACOutdoorAirSystem&&) = default;

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

    virtual bool addToNode(Node& node);

    virtual std::vector<openstudio::IdfObject> remove();

    virtual ModelObject clone(Model model) const;

    /** Returns the ControllerOutdoorAir object associated with the AirLoopHVACOutdoorAirSystem. **/
    ControllerOutdoorAir getControllerOutdoorAir() const;

    /** Sets the ControllerOutdoorAir object associated with the AirLoopHVACOutdoorAirSystem. **/
    bool setControllerOutdoorAir(const ControllerOutdoorAir& controllerOutdoorAir);

    /** Reimplemented from HVACComponent. **/
    boost::optional<AirLoopHVAC> airLoop() const;

    AirflowNetworkDistributionNode getAirflowNetworkDistributionNode();

    boost::optional<AirflowNetworkDistributionNode> airflowNetworkDistributionNode() const;

    static IddObjectType iddObjectType();

    /** Returns the AirLoopHVACDedicatedOutdoorAirSystem, if it exists. */
    boost::optional<AirLoopHVACDedicatedOutdoorAirSystem> airLoopHVACDedicatedOutdoorAirSystem() const;

   protected:
    /// @cond
    using ImplType = detail::AirLoopHVACOutdoorAirSystem_Impl;

    explicit AirLoopHVACOutdoorAirSystem(std::shared_ptr<detail::AirLoopHVACOutdoorAirSystem_Impl> impl);

    friend class detail::AirLoopHVACOutdoorAirSystem_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @cond
   private:
    REGISTER_LOGGER("openstudio.model.AirLoopHVACOutdoorAirSystem");

    /// @endcond
  };

  /** \relates AirLoopHVACOutdoorAirSystem */
  using OptionalAirLoopHVACOutdoorAirSystem = boost::optional<AirLoopHVACOutdoorAirSystem>;

  /** \relates AirLoopHVACOutdoorAirSystem */
  using AirLoopHVACOutdoorAirSystemVector = std::vector<AirLoopHVACOutdoorAirSystem>;

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_AIRLOOPHVACOUTDOORAIRSYSTEM_HPP
