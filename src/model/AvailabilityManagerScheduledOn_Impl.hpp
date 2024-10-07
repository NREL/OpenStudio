/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AVAILABILITYMANAGERSCHEDULEDON_IMPL_HPP
#define MODEL_AVAILABILITYMANAGERSCHEDULEDON_IMPL_HPP

#include "ModelAPI.hpp"
#include "AvailabilityManager_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;

  namespace detail {

    /** AvailabilityManagerScheduledOn_Impl is a AvailabilityManager_Impl that is the implementation class for AvailabilityManagerScheduledOn.*/
    class MODEL_API AvailabilityManagerScheduledOn_Impl : public AvailabilityManager_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AvailabilityManagerScheduledOn_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AvailabilityManagerScheduledOn_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AvailabilityManagerScheduledOn_Impl(const AvailabilityManagerScheduledOn_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AvailabilityManagerScheduledOn_Impl() override = default;

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
      REGISTER_LOGGER("openstudio.model.AvailabilityManagerScheduledOn");

      boost::optional<Schedule> optionalSchedule() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AVAILABILITYMANAGERSCHEDULEDON_IMPL_HPP
