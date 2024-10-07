/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_DISTRICTCOOLING_HPP
#define MODEL_DISTRICTCOOLING_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;

  namespace detail {

    class DistrictCooling_Impl;

  }  // namespace detail

  /** DistrictCooling is a StraightComponent that wraps the OpenStudio IDD object 'OS:DistrictCooling'. */

  class MODEL_API DistrictCooling : public StraightComponent
  {

   public:
    /** @name Constructors and Destructors */
    //@{

    // The capacityFractionSchedule is defaulted to alwaysOnContinuousSchedule
    explicit DistrictCooling(const Model& model);

    virtual ~DistrictCooling() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    DistrictCooling(const DistrictCooling& other) = default;
    DistrictCooling(DistrictCooling&& other) = default;
    DistrictCooling& operator=(const DistrictCooling&) = default;
    DistrictCooling& operator=(DistrictCooling&&) = default;

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
    using ImplType = detail::DistrictCooling_Impl;

    explicit DistrictCooling(std::shared_ptr<detail::DistrictCooling_Impl> impl);

    friend class detail::DistrictCooling_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    /// @endcond

   private:
    REGISTER_LOGGER("openstudio.model.DistrictCooling");
  };

  /** \relates DistrictCooling*/
  using OptionalDistrictCooling = boost::optional<DistrictCooling>;

  /** \relates DistrictCooling*/
  using DistrictCoolingVector = std::vector<DistrictCooling>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_DISTRICTCOOLING_HPP
