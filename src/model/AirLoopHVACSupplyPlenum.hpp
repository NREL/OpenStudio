/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRLOOPHVACSUPPLYPLENUM_HPP
#define MODEL_AIRLOOPHVACSUPPLYPLENUM_HPP

#include "ModelAPI.hpp"
#include "Splitter.hpp"

namespace openstudio {
namespace model {

  class ThermalZone;

  namespace detail {

    class AirLoopHVACSupplyPlenum_Impl;

  }  // namespace detail

  /** AirLoopHVACSupplyPlenum is a Splitter that wraps the OpenStudio IDD object 'OS:AirLoopHVAC:SupplyPlenum'. */
  class MODEL_API AirLoopHVACSupplyPlenum : public Splitter
  {
   public:
    explicit AirLoopHVACSupplyPlenum(const Model& model);

    virtual ~AirLoopHVACSupplyPlenum() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirLoopHVACSupplyPlenum(const AirLoopHVACSupplyPlenum& other) = default;
    AirLoopHVACSupplyPlenum(AirLoopHVACSupplyPlenum&& other) = default;
    AirLoopHVACSupplyPlenum& operator=(const AirLoopHVACSupplyPlenum&) = default;
    AirLoopHVACSupplyPlenum& operator=(AirLoopHVACSupplyPlenum&&) = default;

    static IddObjectType iddObjectType();

    boost::optional<ThermalZone> thermalZone() const;

    /** Establish thermalZone as the plenum zone.
   *  If thermalZone is already conditioned by ThermalZone::equipment(),
   *  then this method will return false.  A ThermalZone cannot be conditioned
   *  by equipment and and be a plenum zone.  Remove any existing ZoneHVACComponent objects,
   *  and associated AirLoopHVAC objects before using this method on thermalZone.
  **/
    bool setThermalZone(const ThermalZone& thermalZone);

    void resetThermalZone();

    unsigned inletPort() const override;

    unsigned outletPort(unsigned branchIndex) const override;

    unsigned nextOutletPort() const override;

    /** Insert this AirLoopHVACSupplyPlenum into HVAC system at node.
   *  Node must be immediatly after AirLoopHVACZoneSplitter and there
   *  cannot be an existing AirLoopHVACSupplyPlenum on the node's branch
   */
    bool addToNode(Node& node);

    bool addBranchForZone(openstudio::model::ThermalZone& thermalZone);

    bool addBranchForZone(openstudio::model::ThermalZone& thermalZone, HVACComponent& terminal);

   protected:
    /// @cond
    using ImplType = detail::AirLoopHVACSupplyPlenum_Impl;

    explicit AirLoopHVACSupplyPlenum(std::shared_ptr<detail::AirLoopHVACSupplyPlenum_Impl> impl);

    friend class detail::AirLoopHVACSupplyPlenum_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirLoopHVACSupplyPlenum");
  };

  /** \relates AirLoopHVACSupplyPlenum*/
  using OptionalAirLoopHVACSupplyPlenum = boost::optional<AirLoopHVACSupplyPlenum>;

  /** \relates AirLoopHVACSupplyPlenum*/
  using AirLoopHVACSupplyPlenumVector = std::vector<AirLoopHVACSupplyPlenum>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRLOOPHVACSUPPLYPLENUM_HPP
