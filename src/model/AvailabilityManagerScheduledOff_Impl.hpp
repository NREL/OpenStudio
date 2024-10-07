/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AVAILABILITYMANAGERSCHEDULEDOFF_IMPL_HPP
#define MODEL_AVAILABILITYMANAGERSCHEDULEDOFF_IMPL_HPP

#include "ModelAPI.hpp"
#include "AvailabilityManager_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;

  namespace detail {

    /** AvailabilityManagerScheduledOff_Impl is a AvailabilityManager_Impl that is the implementation class for AvailabilityManagerScheduledOff.*/
    class MODEL_API AvailabilityManagerScheduledOff_Impl : public AvailabilityManager_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AvailabilityManagerScheduledOff_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AvailabilityManagerScheduledOff_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AvailabilityManagerScheduledOff_Impl(const AvailabilityManagerScheduledOff_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AvailabilityManagerScheduledOff_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      //@}
      /** @name Getters */
      //@{

      Schedule schedule() const;

      //@}
      /** @name Setters */
      //@{

      bool setSchedule(Schedule& schedule);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AvailabilityManagerScheduledOff");

      boost::optional<Schedule> optionalSchedule() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AVAILABILITYMANAGERSCHEDULEDOFF_IMPL_HPP
