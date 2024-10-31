/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRLOOPHVACRETURNPLENUM_HPP
#define MODEL_AIRLOOPHVACRETURNPLENUM_HPP

#include "ModelAPI.hpp"
#include "Mixer.hpp"

namespace openstudio {
namespace model {

  class ThermalZone;

  namespace detail {

    class AirLoopHVACReturnPlenum_Impl;

  }  // namespace detail

  /** AirLoopHVACReturnPlenum is a Mixer that wraps the OpenStudio IDD object 'OS:AirLoopHVAC:ReturnPlenum'. */
  class MODEL_API AirLoopHVACReturnPlenum : public Mixer
  {
   public:
    explicit AirLoopHVACReturnPlenum(const Model& model);

    virtual ~AirLoopHVACReturnPlenum() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirLoopHVACReturnPlenum(const AirLoopHVACReturnPlenum& other) = default;
    AirLoopHVACReturnPlenum(AirLoopHVACReturnPlenum&& other) = default;
    AirLoopHVACReturnPlenum& operator=(const AirLoopHVACReturnPlenum&) = default;
    AirLoopHVACReturnPlenum& operator=(AirLoopHVACReturnPlenum&&) = default;

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

    unsigned outletPort() const override;

    unsigned inletPort(unsigned branchIndex) const override;

    unsigned nextInletPort() const override;

    bool addToNode(Node& node);

    bool addBranchForZone(openstudio::model::ThermalZone& thermalZone);

    bool addBranchForZone(openstudio::model::ThermalZone& thermalZone, HVACComponent& terminal);

   protected:
    /// @cond
    using ImplType = detail::AirLoopHVACReturnPlenum_Impl;

    explicit AirLoopHVACReturnPlenum(std::shared_ptr<detail::AirLoopHVACReturnPlenum_Impl> impl);

    friend class detail::AirLoopHVACReturnPlenum_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirLoopHVACReturnPlenum");
  };

  /** \relates AirLoopHVACReturnPlenum*/
  using OptionalAirLoopHVACReturnPlenum = boost::optional<AirLoopHVACReturnPlenum>;

  /** \relates AirLoopHVACReturnPlenum*/
  using AirLoopHVACReturnPlenumVector = std::vector<AirLoopHVACReturnPlenum>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRLOOPHVACRETURNPLENUM_HPP
