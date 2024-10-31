/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SCHEDULECONSTANT_IMPL_HPP
#define MODEL_SCHEDULECONSTANT_IMPL_HPP

#include "Schedule_Impl.hpp"

namespace openstudio {
namespace model {

  class ScheduleConstant;

  namespace detail {

    class MODEL_API ScheduleConstant_Impl : public Schedule_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      // constructor
      ScheduleConstant_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      ScheduleConstant_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ScheduleConstant_Impl(const ScheduleConstant_Impl& other, Model_Impl* model, bool keepHandle);

      // virtual destructor
      virtual ~ScheduleConstant_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      // return the parent object in the hierarchy
      virtual boost::optional<ParentObject> parent() const override;

      // return any children objects in the hierarchy
      virtual std::vector<ModelObject> children() const override;

      // Get all output variable names that could be associated with this object.
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual boost::optional<ScheduleTypeLimits> scheduleTypeLimits() const override;

      virtual std::vector<double> values() const override;

      //@}
      /** @name Getters */
      //@{

      double value() const;

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      //@}
      /** @name Setters */
      //@{

      virtual bool setScheduleTypeLimits(const ScheduleTypeLimits& scheduleTypeLimits) override;

      virtual bool resetScheduleTypeLimits() override;

      bool setValue(double value);

      // ensure that this object does not contain the date 2/29
      virtual void ensureNoLeapDays() override;

      //@}
     private:
      REGISTER_LOGGER("openstudio.model.ScheduleConstant");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SCHEDULECONSTANT_IMPL_HPP
