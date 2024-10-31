/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_DISTRICTHEATINGWATER_HPP
#define MODEL_DISTRICTHEATINGWATER_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;

  namespace detail {

    class DistrictHeatingWater_Impl;

  }  // namespace detail

  /** DistrictHeatingWater is a StraightComponent that wraps the OpenStudio IDD object 'OS:DistrictHeating:Water'. */

  class MODEL_API DistrictHeatingWater : public StraightComponent
  {

   public:
    /** @name Constructors and Destructors */
    //@{

    // The capacityFractionSchedule is defaulted to alwaysOnContinuousSchedule
    explicit DistrictHeatingWater(const Model& model);

    virtual ~DistrictHeatingWater() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    DistrictHeatingWater(const DistrictHeatingWater& other) = default;
    DistrictHeatingWater(DistrictHeatingWater&& other) = default;
    DistrictHeatingWater& operator=(const DistrictHeatingWater&) = default;
    DistrictHeatingWater& operator=(DistrictHeatingWater&&) = default;

    //@}

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    //@}
    /** @name Getters */
    //@{

    boost::optional<double> nominalCapacity() const;

    bool isNominalCapacityAutosized() const;

    Schedule capacityFractionSchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setNominalCapacity(double nominalCapacity);

    void autosizeNominalCapacity();

    bool setCapacityFractionSchedule(Schedule& schedule);

    //@}
    /** @name Other */
    //@{

    //@}

    boost::optional<double> autosizedNominalCapacity() const;

   protected:
    /// @cond
    using ImplType = detail::DistrictHeatingWater_Impl;

    explicit DistrictHeatingWater(std::shared_ptr<detail::DistrictHeatingWater_Impl> impl);

    friend class detail::DistrictHeatingWater_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    /// @endcond

   private:
    REGISTER_LOGGER("openstudio.model.DistrictHeatingWater");
  };

  /** \relates DistrictHeatingWater*/
  using OptionalDistrictHeatingWater = boost::optional<DistrictHeatingWater>;

  /** \relates DistrictHeatingWater*/
  using DistrictHeatingWaterVector = std::vector<DistrictHeatingWater>;

  // In E+ 23.2.0, DistrictHeating was renamed to DistrictHeating:Water. We typedef for backwards compatibility
  using DistrictHeating [[deprecated("Deprecated at 3.7.0, replaced with DistrictHeatingWater. It will be removed after three releases")]] =
    DistrictHeatingWater;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_DISTRICTHEATINGWATER_HPP
