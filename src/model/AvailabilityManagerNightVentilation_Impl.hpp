/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AVAILABILITYMANAGERNIGHTVENTILATION_IMPL_HPP
#define MODEL_AVAILABILITYMANAGERNIGHTVENTILATION_IMPL_HPP

#include "ModelAPI.hpp"
#include "AvailabilityManager_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class ThermalZone;

  namespace detail {

    /** AvailabilityManagerNightVentilation_Impl is a AvailabilityManager_Impl that is the implementation class for AvailabilityManagerNightVentilation.*/
    class MODEL_API AvailabilityManagerNightVentilation_Impl : public AvailabilityManager_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AvailabilityManagerNightVentilation_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AvailabilityManagerNightVentilation_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AvailabilityManagerNightVentilation_Impl(const AvailabilityManagerNightVentilation_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AvailabilityManagerNightVentilation_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      //@}
      /** @name Getters */
      //@{

      Schedule applicabilitySchedule() const;

      boost::optional<Schedule> ventilationTemperatureSchedule() const;

      double ventilationTemperatureDifference() const;

      double ventilationTemperatureLowLimit() const;

      double nightVentingFlowFraction() const;

      boost::optional<ThermalZone> controlZone() const;

      //@}
      /** @name Setters */
      //@{

      bool setApplicabilitySchedule(Schedule& schedule);

      bool setVentilationTemperatureSchedule(Schedule& schedule);

      void resetVentilationTemperatureSchedule();

      bool setVentilationTemperatureDifference(double ventilationTemperatureDifference);

      bool setVentilationTemperatureLowLimit(double ventilationTemperatureLowLimit);

      bool setNightVentingFlowFraction(double nightVentingFlowFraction);

      bool setControlZone(const boost::optional<ThermalZone>& thermalZone);

      void resetControlZone();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AvailabilityManagerNightVentilation");

      // TODO: Check the return types of these methods.
      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<Schedule> optionalApplicabilitySchedule() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AVAILABILITYMANAGERNIGHTVENTILATION_IMPL_HPP
