/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SCHEDULECOMPACT_IMPL_HPP
#define MODEL_SCHEDULECOMPACT_IMPL_HPP

#include "Schedule_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class MODEL_API ScheduleCompact_Impl : public Schedule_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      // constructor
      ScheduleCompact_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      ScheduleCompact_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ScheduleCompact_Impl(const ScheduleCompact_Impl& other, Model_Impl* model, bool keepHandle);

      // virtual destructor
      virtual ~ScheduleCompact_Impl() override = default;

      //@}
      /** @name Getters */
      //@{

      // return the parent object in the hierarchy
      virtual boost::optional<ParentObject> parent() const override;

      // return any children objects in the hierarchy
      virtual std::vector<ModelObject> children() const override;

      // Get all output variable names that could be associated with this object.
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<double> values() const override;

      virtual boost::optional<ScheduleTypeLimits> scheduleTypeLimits() const override;

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      //@}
      /** @name Setters */
      //@{

      virtual bool setScheduleTypeLimits(const ScheduleTypeLimits& scheduleTypeLimits) override;

      virtual bool resetScheduleTypeLimits() override;

      bool setToConstantValue(double value);

      // ensure that this object does not contain the date 2/29
      virtual void ensureNoLeapDays() override;

      //@}
      /** @name Queries */
      //@{

      bool isConstantValue() const;

      boost::optional<double> constantValue() const;

      //@}
     private:
      REGISTER_LOGGER("openstudio.model.ScheduleCompact");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SCHEDULECOMPACT_IMPL_HPP
