/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SCHEDULEINTERVAL_IMPL_HPP
#define MODEL_SCHEDULEINTERVAL_IMPL_HPP

#include "Schedule_Impl.hpp"

namespace openstudio {

class TimeSeries;

namespace model {

  namespace detail {

    class MODEL_API ScheduleInterval_Impl : public Schedule_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      // constructor
      ScheduleInterval_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      ScheduleInterval_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ScheduleInterval_Impl(const ScheduleInterval_Impl& other, Model_Impl* model, bool keepHandle);

      // virtual destructor
      virtual ~ScheduleInterval_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      // return the parent object in the hierarchy
      virtual boost::optional<ParentObject> parent() const override;

      // return any children objects in the hierarchy
      virtual std::vector<ModelObject> children() const override;

      // Get all output variable names that could be associated with this object.
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual std::vector<double> values() const override;

      //@}
      /** @name Getters */
      //@{

      virtual openstudio::TimeSeries timeSeries() const = 0;

      //@}
      /** @name Setters */
      //@{

      virtual bool setTimeSeries(const openstudio::TimeSeries& timeSeries) = 0;

      //@}
     private:
      REGISTER_LOGGER("openstudio.model.ScheduleInterval");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SCHEDULEINTERVAL_IMPL_HPP
