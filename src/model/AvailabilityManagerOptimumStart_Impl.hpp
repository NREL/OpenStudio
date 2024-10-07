/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AVAILABILITYMANAGEROPTIMUMSTART_IMPL_HPP
#define MODEL_AVAILABILITYMANAGEROPTIMUMSTART_IMPL_HPP

#include "ModelAPI.hpp"
#include "AvailabilityManager_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class ThermalZone;

  namespace detail {

    /** AvailabilityManagerOptimumStart_Impl is a AvailabilityManager_Impl that is the implementation class for AvailabilityManagerOptimumStart.*/
    class MODEL_API AvailabilityManagerOptimumStart_Impl : public AvailabilityManager_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AvailabilityManagerOptimumStart_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AvailabilityManagerOptimumStart_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AvailabilityManagerOptimumStart_Impl(const AvailabilityManagerOptimumStart_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AvailabilityManagerOptimumStart_Impl() override = default;

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

      std::string controlType() const;

      boost::optional<ThermalZone> controlZone() const;

      double maximumValueforOptimumStartTime() const;

      std::string controlAlgorithm() const;

      double constantTemperatureGradientduringCooling() const;

      double constantTemperatureGradientduringHeating() const;

      double initialTemperatureGradientduringCooling() const;

      double initialTemperatureGradientduringHeating() const;

      double constantStartTime() const;

      int numberofPreviousDays() const;

      //@}
      /** @name Setters */
      //@{

      bool setApplicabilitySchedule(Schedule& schedule);

      bool setControlType(const std::string& controlType);

      bool setControlZone(const boost::optional<ThermalZone>& thermalZone);

      void resetControlZone();

      bool setMaximumValueforOptimumStartTime(double maximumValueforOptimumStartTime);

      bool setControlAlgorithm(const std::string& controlAlgorithm);

      bool setConstantTemperatureGradientduringCooling(double constantTemperatureGradientduringCooling);

      bool setConstantTemperatureGradientduringHeating(double constantTemperatureGradientduringHeating);

      bool setInitialTemperatureGradientduringCooling(double initialTemperatureGradientduringCooling);

      bool setInitialTemperatureGradientduringHeating(double initialTemperatureGradientduringHeating);

      bool setConstantStartTime(double constantStartTime);

      bool setNumberofPreviousDays(int numberofPreviousDays);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AvailabilityManagerOptimumStart");

      // TODO: Check the return types of these methods.
      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<Schedule> optionalApplicabilitySchedule() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AVAILABILITYMANAGEROPTIMUMSTART_IMPL_HPP
