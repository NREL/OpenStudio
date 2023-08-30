/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_DISTRICTHEATING_HPP
#define MODEL_DISTRICTHEATING_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;

  namespace detail {

    class DistrictHeating_Impl;

  }  // namespace detail

  /** DistrictHeating is a StraightComponent that wraps the OpenStudio IDD object 'OS:DistrictHeating'. */

  class MODEL_API DistrictHeating : public StraightComponent
  {

   public:
    /** @name Constructors and Destructors */
    //@{

    explicit DistrictHeating(const Model& model);

    virtual ~DistrictHeating() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    DistrictHeating(const DistrictHeating& other) = default;
    DistrictHeating(DistrictHeating&& other) = default;
    DistrictHeating& operator=(const DistrictHeating&) = default;
    DistrictHeating& operator=(DistrictHeating&&) = default;

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

    boost::optional<Schedule> capacityFractionSchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setNominalCapacity(double nominalCapacity);

    void autosizeNominalCapacity();

    bool setCapacityFractionSchedule(Schedule& schedule);

    void resetCapacityFractionSchedule();

    //@}
    /** @name Other */
    //@{

    //@}

    boost::optional<double> autosizedNominalCapacity() const;

   protected:
    /// @cond
    using ImplType = detail::DistrictHeating_Impl;

    explicit DistrictHeating(std::shared_ptr<detail::DistrictHeating_Impl> impl);

    friend class detail::DistrictHeating_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    /// @endcond

   private:
    REGISTER_LOGGER("openstudio.model.DistrictHeating");
  };

  /** \relates DistrictHeating*/
  using OptionalDistrictHeating = boost::optional<DistrictHeating>;

  /** \relates DistrictHeating*/
  using DistrictHeatingVector = std::vector<DistrictHeating>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_DISTRICTHEATING_HPP
