/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SCHEDULECONSTANT_HPP
#define MODEL_SCHEDULECONSTANT_HPP

#include "ModelAPI.hpp"
#include "Schedule.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class ScheduleConstant_Impl;
  }  // namespace detail

  /** ScheduleConstant is a Schedule that wraps the OpenStudio IDD object 'OS:Schedule:Constant'. */
  class MODEL_API ScheduleConstant : public Schedule
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Creates new ScheduleConstant in model and sets its value to 0.0. */
    explicit ScheduleConstant(const Model& model);

    virtual ~ScheduleConstant() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ScheduleConstant(const ScheduleConstant& other) = default;
    ScheduleConstant(ScheduleConstant&& other) = default;
    ScheduleConstant& operator=(const ScheduleConstant&) = default;
    ScheduleConstant& operator=(ScheduleConstant&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    double value() const;

    //@}
    /** @name Setters */
    //@{

    bool setValue(double value);

    /** Sets this ScheduleConstant's value if scheduleTypeLimits() is set and value can be properly
   *  converted to the 'SI' units so implied. */

    //@}
   protected:
    using ImplType = detail::ScheduleConstant_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    // constructor
    explicit ScheduleConstant(std::shared_ptr<detail::ScheduleConstant_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.ScheduleConstant");
  };

  /** \relates ScheduleConstant */
  using OptionalScheduleConstant = boost::optional<ScheduleConstant>;

  /** \relates ScheduleConstant */
  using ScheduleConstantVector = std::vector<ScheduleConstant>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SCHEDULECONSTANT_HPP
