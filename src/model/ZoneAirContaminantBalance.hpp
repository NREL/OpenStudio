/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEAIRCONTAMINANTBALANCE_HPP
#define MODEL_ZONEAIRCONTAMINANTBALANCE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  class Schedule;

  namespace detail {

    class ZoneAirContaminantBalance_Impl;

  }  // namespace detail

  /** ZoneAirContaminantBalance is a ModelObject that wraps the OpenStudio IDD object 'OS:ZoneAirContaminantBalance'. */
  class MODEL_API ZoneAirContaminantBalance : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~ZoneAirContaminantBalance() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ZoneAirContaminantBalance(const ZoneAirContaminantBalance& other) = default;
    ZoneAirContaminantBalance(ZoneAirContaminantBalance&& other) = default;
    ZoneAirContaminantBalance& operator=(const ZoneAirContaminantBalance&) = default;
    ZoneAirContaminantBalance& operator=(ZoneAirContaminantBalance&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    bool carbonDioxideConcentration() const;

    bool isCarbonDioxideConcentrationDefaulted() const;

    boost::optional<Schedule> outdoorCarbonDioxideSchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setCarbonDioxideConcentration(bool carbonDioxideConcentration);

    void setCarbonDioxideConcentrationNoFail(bool carbonDioxideConcentration);

    void resetCarbonDioxideConcentration();

    bool setOutdoorCarbonDioxideSchedule(Schedule& schedule);

    void resetOutdoorCarbonDioxideSchedule();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::ZoneAirContaminantBalance_Impl;

    explicit ZoneAirContaminantBalance(std::shared_ptr<detail::ZoneAirContaminantBalance_Impl> impl);

    friend class detail::ZoneAirContaminantBalance_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    explicit ZoneAirContaminantBalance(Model& model);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ZoneAirContaminantBalance");
  };

  /** \relates ZoneAirContaminantBalance*/
  using OptionalZoneAirContaminantBalance = boost::optional<ZoneAirContaminantBalance>;

  /** \relates ZoneAirContaminantBalance*/
  using ZoneAirContaminantBalanceVector = std::vector<ZoneAirContaminantBalance>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEAIRCONTAMINANTBALANCE_HPP
