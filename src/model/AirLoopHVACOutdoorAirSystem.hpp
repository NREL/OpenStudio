/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

    /** A default ControllerOutdoorAir will be created for you */
    explicit AirLoopHVACOutdoorAirSystem(Model& model);

    virtual ~AirLoopHVACOutdoorAirSystem() override = default;
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

    /** Returns the optional ModelObject attached to the outdoor air port.
     *  This is NOT the same as the outboardOANode (unless there isn't nothing on the outdoor air stream)
     *  It is the oa node closest to the OA system **/
    boost::optional<ModelObject> outdoorAirModelObject();

    /** Returns the optional ModelObject attached to the relief air port.
     *  This is NOT the same as the outboardReliefNode (unless there isn't nothing on the relief stream)
     *  It is the relief node closest to the OA system **/
    boost::optional<ModelObject> reliefAirModelObject();

    /** Returns the optional ModelObject attached to the mixer air port. **/
    boost::optional<ModelObject> mixedAirModelObject();

    /** Returns the most outboard outdoor air Node. **/
    boost::optional<Node> outboardOANode() const;  // TODO: shouldn't be optional

    /** Returns the most outboard relief air Node. **/
    boost::optional<Node> outboardReliefNode() const;  // TODO: shouldn't be optional

    /** Returns a vector of model objects that are on the path of the incoming outdoor air stream.
     * This is orderded like the airflow: from the outdoorOANode (OA Intake) towards the OASystem itself **/
    std::vector<ModelObject> oaComponents(openstudio::IddObjectType type = openstudio::IddObjectType("Catchall")) const;

    /** Returns a vector of model objects that are on the path of the outgoing relief air stream.
      * This is orderded like the airflow: from the OASystem itself towards the outboardReliefNode (Relief to Outside) **/
    std::vector<ModelObject> reliefComponents(openstudio::IddObjectType type = openstudio::IddObjectType("Catchall")) const;

    /** Returns a vector that is the concatenation of oaComponents() and reliefComponents(). **/
    std::vector<ModelObject> components(openstudio::IddObjectType type = openstudio::IddObjectType("Catchall")) const;

    /** Returns the optional ModelObject with the Handle given.  The optional
   *  will be false if the given handle does not correspond to the a ModelObject
   *  that is not part of the outdoor air system.
   **/
    boost::optional<ModelObject> component(openstudio::Handle handle) const;

    /** Returns the optional ModelObject with the Handle given.  The optional
   *  will be false if the given handle does not correspond to the a ModelObject
   *  that is not part of the supply side of the outdoor air system.
   **/
    boost::optional<ModelObject> oaComponent(openstudio::Handle handle) const;

    /** Returns the optional ModelObject with the Handle given.  The optional
   *  will be false if the given handle does not correspond to the a ModelObject
   *  that is not part of the supply side of the outdoor air system.
   **/
    boost::optional<ModelObject> reliefComponent(openstudio::Handle handle) const;

    /** Returns the ControllerOutdoorAir object associated with the AirLoopHVACOutdoorAirSystem. **/
    ControllerOutdoorAir getControllerOutdoorAir() const;

    /** Sets the ControllerOutdoorAir object associated with the AirLoopHVACOutdoorAirSystem. **/
    bool setControllerOutdoorAir(const ControllerOutdoorAir& controllerOutdoorAir);

    /** Reimplemented from HVACComponent. **/
    boost::optional<AirLoopHVAC> airLoop() const;  // TODO: this shouldn't exist!!!

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
