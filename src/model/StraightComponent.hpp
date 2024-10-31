/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_STRAIGHTCOMPONENT_HPP
#define MODEL_STRAIGHTCOMPONENT_HPP

#include "ModelAPI.hpp"
#include "HVACComponent.hpp"
#include "Connection.hpp"

namespace openstudio {
namespace model {

  class AirLoopHVAC;
  class AirflowNetworkEquivalentDuct;

  namespace detail {
    class StraightComponent_Impl;
  }

  /** StraightComponent is the base class for HVACComponent objects which have precisely one inlet port and one outlet port.
   *
   *  A StraighComponent may appear on either an AirLoopHVAC or a PlantLoop.
   */
  class MODEL_API StraightComponent : public HVACComponent
  {

   public:
    StraightComponent(IddObjectType type, const Model& model);

    virtual ~StraightComponent() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    StraightComponent(const StraightComponent& other) = default;
    StraightComponent(StraightComponent&& other) = default;
    StraightComponent& operator=(const StraightComponent&) = default;
    StraightComponent& operator=(StraightComponent&&) = default;

    bool removeFromLoop();

    /** Returns the inlet port. **/
    unsigned inletPort() const;

    /** Returns the outlet port. **/
    unsigned outletPort() const;

    /** Returns the optional ModelObject connected to the inlet port. **/
    boost::optional<ModelObject> inletModelObject() const;

    /** Returns the optional ModelObject connected to the outlet port. **/
    boost::optional<ModelObject> outletModelObject() const;

    /** Returns the optional AirLoopHVAC object that this AirToAirComponent is attached to.
     *
     *  Reimplemented from HVACComponent.
     */
    boost::optional<AirLoopHVAC> airLoopHVAC() const;  // cppcheck-suppress [duplInheritedMember] for documentation purposes

   protected:
    friend class Model;

    friend class openstudio::IdfObject;

    /// @cond

    using ImplType = detail::StraightComponent_Impl;

    explicit StraightComponent(std::shared_ptr<detail::StraightComponent_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.StraightComponent");

    /// @endcond
  };

  using OptionalStraightComponent = boost::optional<StraightComponent>;

  using StraightComponentVector = std::vector<StraightComponent>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_STRAIGHTCOMPONENT_HPP
