/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AVAILABILITYMANAGERHYBRIDVENTILATION_IMPL_HPP
#define MODEL_AVAILABILITYMANAGERHYBRIDVENTILATION_IMPL_HPP

#include "ModelAPI.hpp"
#include "AvailabilityManager_Impl.hpp"

namespace openstudio {
namespace model {

  class ThermalZone;
  class Schedule;
  class Curve;
  class ModelObject;

  namespace detail {

    /** AvailabilityManagerHybridVentilation_Impl is a ModelObject_Impl that is the implementation class for AvailabilityManagerHybridVentilation.*/
    class MODEL_API AvailabilityManagerHybridVentilation_Impl : public AvailabilityManager_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AvailabilityManagerHybridVentilation_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AvailabilityManagerHybridVentilation_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AvailabilityManagerHybridVentilation_Impl(const AvailabilityManagerHybridVentilation_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AvailabilityManagerHybridVentilation_Impl() override = default;

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

      boost::optional<ThermalZone> controlledZone() const;

      Schedule ventilationControlModeSchedule() const;

      bool useWeatherFileRainIndicators() const;

      double maximumWindSpeed() const;

      double minimumOutdoorTemperature() const;

      double maximumOutdoorTemperature() const;

      double minimumOutdoorEnthalpy() const;

      double maximumOutdoorEnthalpy() const;

      double minimumOutdoorDewpoint() const;

      double maximumOutdoorDewpoint() const;

      Schedule minimumOutdoorVentilationAirSchedule() const;

      boost::optional<Curve> openingFactorFunctionofWindSpeedCurve() const;

      double minimumHVACOperationTime() const;

      double minimumVentilationTime() const;

      boost::optional<Schedule> airflowNetworkControlTypeSchedule() const;

      boost::optional<Schedule> simpleAirflowControlTypeSchedule() const;

      boost::optional<ModelObject> zoneVentilationObject() const;

      //@}
      /** @name Setters */
      //@{

      bool setControlledZone(const boost::optional<ThermalZone>& thermalZone);

      void resetControlledZone();

      bool setVentilationControlModeSchedule(Schedule& schedule);

      bool setUseWeatherFileRainIndicators(bool useWeatherFileRainIndicators);

      bool setMaximumWindSpeed(double maximumWindSpeed);

      bool setMinimumOutdoorTemperature(double minimumOutdoorTemperature);

      bool setMaximumOutdoorTemperature(double maximumOutdoorTemperature);

      bool setMinimumOutdoorEnthalpy(double minimumOutdoorEnthalpy);

      bool setMaximumOutdoorEnthalpy(double maximumOutdoorEnthalpy);

      bool setMinimumOutdoorDewpoint(double minimumOutdoorDewpoint);

      bool setMaximumOutdoorDewpoint(double maximumOutdoorDewpoint);

      bool setMinimumOutdoorVentilationAirSchedule(Schedule& schedule);

      bool setOpeningFactorFunctionofWindSpeedCurve(const boost::optional<Curve>& curve);

      void resetOpeningFactorFunctionofWindSpeedCurve();

      bool setMinimumHVACOperationTime(double minimumHVACOperationTime);

      bool setMinimumVentilationTime(double minimumVentilationTime);

      bool setAirflowNetworkControlTypeSchedule(Schedule& schedule);
      void resetAirflowNetworkControlTypeSchedule();

      bool setSimpleAirflowControlTypeSchedule(Schedule& schedule);
      void resetSimpleAirflowControlTypeSchedule();

      bool setZoneVentilationObject(const ModelObject& zv);
      void resetZoneVentilationObject();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AvailabilityManagerHybridVentilation");

      // TODO: Check the return types of these methods.
      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<Schedule> optionalVentilationControlModeSchedule() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AVAILABILITYMANAGERHYBRIDVENTILATION_IMPL_HPP
