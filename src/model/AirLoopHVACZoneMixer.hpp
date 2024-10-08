/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRLOOPHVACZONEMIXER_HPP
#define MODEL_AIRLOOPHVACZONEMIXER_HPP

#include "ModelAPI.hpp"
#include "Mixer.hpp"

namespace openstudio {
namespace model {

  class AirflowNetworkDistributionNode;

  namespace detail {
    class AirLoopHVACZoneMixer_Impl;
  }  // namespace detail

  /** AirLoopHVACZoneMixer is an interface to the EnergyPlus IDD object named "AirLoopHVAC:ZoneMixer"
 *
 *  The purpose of this class is to simplify the construction and manipulation
 *  of zone mixer objects in EnergyPlus.  Methods are built around the
 *  acts of getting the inlet and outlet ports to the mixer.  Branch indexes
 *  are used to refer to the many inlet ports of the mixer
 */
  class MODEL_API AirLoopHVACZoneMixer : public Mixer
  {

   public:
    /** Constructs a new AirLoopHVACZoneMixer object and places it inside the model. */
    explicit AirLoopHVACZoneMixer(const Model& model);

    virtual ~AirLoopHVACZoneMixer() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirLoopHVACZoneMixer(const AirLoopHVACZoneMixer& other) = default;
    AirLoopHVACZoneMixer(AirLoopHVACZoneMixer&& other) = default;
    AirLoopHVACZoneMixer& operator=(const AirLoopHVACZoneMixer&) = default;
    AirLoopHVACZoneMixer& operator=(AirLoopHVACZoneMixer&&) = default;

    unsigned outletPort() const override;

    unsigned inletPort(unsigned branchIndex) const override;

    unsigned nextInletPort() const override;

    AirflowNetworkDistributionNode getAirflowNetworkDistributionNode();

    boost::optional<AirflowNetworkDistributionNode> airflowNetworkDistributionNode() const;

    static IddObjectType iddObjectType();

   protected:
    friend class Model;

    friend class openstudio::IdfObject;

    /// @cond

    using ImplType = detail::AirLoopHVACZoneMixer_Impl;

    explicit AirLoopHVACZoneMixer(std::shared_ptr<detail::AirLoopHVACZoneMixer_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.AirLoopHVACZoneMixer");

    AirLoopHVACZoneMixer(const Handle& handle, const Model& model);

    /// @endcond
  };

  /** \relates AirLoopHVACZoneMixer */
  using OptionalAirLoopHVACZoneMixer = boost::optional<AirLoopHVACZoneMixer>;

  /** \relates AirLoopHVACZoneMixer */
  using AirLoopHVACZoneMixerVector = std::vector<AirLoopHVACZoneMixer>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRLOOPHVACZONEMIXER_HPP
