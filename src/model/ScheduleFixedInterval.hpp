/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SCHEDULEFIXEDINTERVAL_HPP
#define MODEL_SCHEDULEFIXEDINTERVAL_HPP

#include "ModelAPI.hpp"
#include "ScheduleInterval.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class ScheduleFixedInterval_Impl;

  }  // namespace detail

  /** ScheduleFixedInterval is a ScheduleInterval that wraps the OpenStudio IDD object
 *  'OS_Schedule_FixedInterval'. */
  class MODEL_API ScheduleFixedInterval : public ScheduleInterval
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ScheduleFixedInterval(const Model& model);

    virtual ~ScheduleFixedInterval() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ScheduleFixedInterval(const ScheduleFixedInterval& other) = default;
    ScheduleFixedInterval(ScheduleFixedInterval&& other) = default;
    ScheduleFixedInterval& operator=(const ScheduleFixedInterval&) = default;
    ScheduleFixedInterval& operator=(ScheduleFixedInterval&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    bool interpolatetoTimestep() const;

    bool isInterpolatetoTimestepDefaulted() const;

    bool translatetoScheduleFile() const;

    bool isTranslatetoScheduleFileDefaulted() const;

    int intervalLength() const;

    double outOfRangeValue() const;

    bool isOutOfRangeValueDefaulted() const;

    int startMonth() const;

    int startDay() const;

    //@}
    /** @name Setters */
    //@{

    bool setInterpolatetoTimestep(bool interpolatetoTimestep);

    void resetInterpolatetoTimestep();

    bool setTranslatetoScheduleFile(bool translatetoScheduleFile);

    void resetTranslatetoScheduleFile();

    bool setIntervalLength(int intervalLength);

    bool setOutOfRangeValue(double outOfRangeValue);

    void resetOutOfRangeValue();

    bool setStartMonth(int startMonth);

    bool setStartDay(int startDay);

    //@}
   protected:
    /// @cond
    using ImplType = detail::ScheduleFixedInterval_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class detail::ScheduleFixedInterval_Impl;

    explicit ScheduleFixedInterval(std::shared_ptr<detail::ScheduleFixedInterval_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ScheduleFixedInterval");
  };

  /** \relates ScheduleFixedInterval*/
  using OptionalScheduleFixedInterval = boost::optional<ScheduleFixedInterval>;

  /** \relates ScheduleFixedInterval*/
  using ScheduleFixedIntervalVector = std::vector<ScheduleFixedInterval>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SCHEDULEFIXEDINTERVAL_HPP
