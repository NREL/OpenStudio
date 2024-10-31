/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRLOOPHVACZONESPLITTER_HPP
#define MODEL_AIRLOOPHVACZONESPLITTER_HPP

#include "ModelAPI.hpp"
#include "Splitter.hpp"

namespace openstudio {

namespace model {

  class ThermalZone;
  class AirflowNetworkDistributionNode;

  namespace detail {
    class AirLoopHVACZoneSplitter_Impl;
  }  // namespace detail

  /** AirLoopHVACZoneSplitter is an interface to the EnergyPlus IDD object named "AirLoopHVAC:ZoneSplitter"
 *
 *  The purpose of this class is to simplify the construction and manipulation
 *  of zone splitter objects in EnergyPlus.  Methods are built around the
 *  acts of getting the inlet and outlet ports to the splitter.  Branch indexes
 *  are used to refer to the many outlet ports of the mixer
 */
  class MODEL_API AirLoopHVACZoneSplitter : public Splitter
  {
   public:
    /** Constructs a new AirLoopHVACZoneSplitter object and places it inside the model. */
    explicit AirLoopHVACZoneSplitter(const Model& model);

    virtual ~AirLoopHVACZoneSplitter() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirLoopHVACZoneSplitter(const AirLoopHVACZoneSplitter& other) = default;
    AirLoopHVACZoneSplitter(AirLoopHVACZoneSplitter&& other) = default;
    AirLoopHVACZoneSplitter& operator=(const AirLoopHVACZoneSplitter&) = default;
    AirLoopHVACZoneSplitter& operator=(AirLoopHVACZoneSplitter&&) = default;

    /// Returns the IddObjectType.
    static IddObjectType iddObjectType();

    /** Returns the inlet port to the zone splitter. */
    unsigned inletPort() const override;

    /** Returns the outlet port for branchIndex.  Branches are sequentially
   *  indexed starting from 0.
   */
    unsigned outletPort(unsigned branchIndex) const override;

    /** Returns the next available outlet port.  This will be the first port
   *  with no connected objects */
    unsigned nextOutletPort() const override;

    /** Returns a vector of all ZoneHVACEquipmentConnections objects connected
   *  to the splitter's outlets.
   */
    std::vector<ThermalZone> thermalZones();

    AirflowNetworkDistributionNode getAirflowNetworkDistributionNode();

    boost::optional<AirflowNetworkDistributionNode> airflowNetworkDistributionNode() const;

   protected:
    /// @cond

    friend class Model;

    friend class openstudio::IdfObject;

    using ImplType = detail::AirLoopHVACZoneSplitter_Impl;

    explicit AirLoopHVACZoneSplitter(std::shared_ptr<detail::AirLoopHVACZoneSplitter_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.AirLoopHVACZoneSplitter");

    /// @endcond
  };

  /** \relates AirLoopHVACZoneSplitter */
  using OptionalAirLoopHVACZoneSplitter = boost::optional<AirLoopHVACZoneSplitter>;

  /** \relates AirLoopHVACZoneSplitter */
  using AirLoopHVACZoneSplitterVector = std::vector<AirLoopHVACZoneSplitter>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRLOOPHVACZONESPLITTER_HPP
