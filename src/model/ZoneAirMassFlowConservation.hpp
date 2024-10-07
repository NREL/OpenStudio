/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEAIRMASSFLOWCONSERVATION_HPP
#define MODEL_ZONEAIRMASSFLOWCONSERVATION_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

#include "../utilities/core/Deprecated.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class ZoneAirMassFlowConservation_Impl;

  }  // namespace detail

  /** ZoneAirMassFlowConservation is a ModelObject that wraps the OpenStudio IDD object 'OS:ZoneAirMassFlowConservation'. */
  class MODEL_API ZoneAirMassFlowConservation : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~ZoneAirMassFlowConservation() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ZoneAirMassFlowConservation(const ZoneAirMassFlowConservation& other) = default;
    ZoneAirMassFlowConservation(ZoneAirMassFlowConservation&& other) = default;
    ZoneAirMassFlowConservation& operator=(const ZoneAirMassFlowConservation&) = default;
    ZoneAirMassFlowConservation& operator=(ZoneAirMassFlowConservation&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> adjustZoneMixingandReturnForAirMassFlowBalanceValues();

    static std::vector<std::string> infiltrationBalancingMethodValues();

    static std::vector<std::string> infiltrationBalancingZonesValues();

    /** @name Getters */
    //@{

    std::string adjustZoneMixingandReturnForAirMassFlowBalance() const;
    // TODO: makes little sense
    OS_DEPRECATED(3, 2, 0) bool adjustZoneMixingForZoneAirMassFlowBalance() const;

    bool isAdjustZoneMixingandReturnForAirMassFlowBalanceDefaulted() const;
    OS_DEPRECATED(3, 2, 0) bool isAdjustZoneMixingForZoneAirMassFlowBalanceDefaulted() const;

    std::string infiltrationBalancingMethod() const;

    bool isInfiltrationBalancingMethodDefaulted() const;

    std::string infiltrationBalancingZones() const;

    bool isInfiltrationBalancingZonesDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setAdjustZoneMixingandReturnForAirMassFlowBalance(const std::string& adjustZoneMixingandReturnForAirMassFlowBalance);
    // TODO: makes little sense too
    OS_DEPRECATED(3, 2, 0) bool setAdjustZoneMixingForZoneAirMassFlowBalance(bool adjustZoneMixingForZoneAirMassFlowBalance);

    void resetAdjustZoneMixingandReturnForAirMassFlowBalance();
    OS_DEPRECATED(3, 2, 0) void resetAdjustZoneMixingForZoneAirMassFlowBalance();

    bool setInfiltrationBalancingMethod(const std::string& infiltrationBalancingMethod);

    void resetInfiltrationBalancingMethod();

    bool setInfiltrationBalancingZones(const std::string& infiltrationBalancingZones);

    void resetInfiltrationBalancingZones();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::ZoneAirMassFlowConservation_Impl;

    explicit ZoneAirMassFlowConservation(std::shared_ptr<detail::ZoneAirMassFlowConservation_Impl> impl);

    friend class detail::ZoneAirMassFlowConservation_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    explicit ZoneAirMassFlowConservation(Model& model);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ZoneAirMassFlowConservation");
  };

  /** \relates ZoneAirMassFlowConservation*/
  using OptionalZoneAirMassFlowConservation = boost::optional<ZoneAirMassFlowConservation>;

  /** \relates ZoneAirMassFlowConservation*/
  using ZoneAirMassFlowConservationVector = std::vector<ZoneAirMassFlowConservation>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEAIRMASSFLOWCONSERVATION_HPP
