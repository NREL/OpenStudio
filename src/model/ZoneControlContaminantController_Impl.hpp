/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONECONTROLCONTAMINANTCONTROLLER_IMPL_HPP
#define MODEL_ZONECONTROLCONTAMINANTCONTROLLER_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class ThermalZone;
  class Schedule;

  namespace detail {

    /** ZoneControlContaminantController_Impl is a ModelObject_Impl that is the implementation class for ZoneControlContaminantController.*/
    class MODEL_API ZoneControlContaminantController_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ZoneControlContaminantController_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ZoneControlContaminantController_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ZoneControlContaminantController_Impl(const ZoneControlContaminantController_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ZoneControlContaminantController_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual ModelObject clone(Model model) const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<Schedule> carbonDioxideControlAvailabilitySchedule() const;

      boost::optional<Schedule> carbonDioxideSetpointSchedule() const;

      boost::optional<Schedule> minimumCarbonDioxideConcentrationSchedule() const;

      boost::optional<Schedule> genericContaminantControlAvailabilitySchedule() const;

      boost::optional<Schedule> genericContaminantSetpointSchedule() const;

      //@}
      /** @name Setters */
      //@{

      bool setCarbonDioxideControlAvailabilitySchedule(Schedule& schedule);

      void resetCarbonDioxideControlAvailabilitySchedule();

      bool setCarbonDioxideSetpointSchedule(Schedule& schedule);

      void resetCarbonDioxideSetpointSchedule();

      bool setMinimumCarbonDioxideConcentrationSchedule(Schedule& schedule);

      void resetMinimumCarbonDioxideConcentrationSchedule();

      bool setGenericContaminantControlAvailabilitySchedule(Schedule& schedule);

      void resetGenericContaminantControlAvailabilitySchedule();

      bool setGenericContaminantSetpointSchedule(Schedule& schedule);

      void resetGenericContaminantSetpointSchedule();

      //@}
      /** @name Other */
      //@{

      boost::optional<ThermalZone> controlledZone() const;

      bool setControlledZone(ThermalZone& zone);

      void resetControlledZone();

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ZoneControlContaminantController");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONECONTROLCONTAMINANTCONTROLLER_IMPL_HPP
