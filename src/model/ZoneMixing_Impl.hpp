/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEMIXING_IMPL_HPP
#define MODEL_ZONEMIXING_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class Space;
  class ThermalZone;

  namespace detail {

    /** ZoneMixing_Impl is a ModelObject_Impl that is the implementation class for ZoneMixing.*/
    class MODEL_API ZoneMixing_Impl : public ModelObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      ZoneMixing_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ZoneMixing_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ZoneMixing_Impl(const ZoneMixing_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ZoneMixing_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual bool setParent(ParentObject& newParent) override;

      bool hardSize();

      //@}
      /** @name Getters */
      //@{

      boost::optional<ThermalZone> zone() const;
      boost::optional<Space> space() const;
      ModelObject zoneOrSpace() const;

      Schedule schedule() const;

      std::string designFlowRateCalculationMethod() const;

      boost::optional<double> designFlowRate() const;

      boost::optional<double> flowRateperFloorArea() const;

      boost::optional<double> flowRateperPerson() const;

      boost::optional<double> airChangesperHour() const;

      boost::optional<ThermalZone> sourceZone() const;
      boost::optional<Space> sourceSpace() const;
      boost::optional<ModelObject> sourceZoneOrSpace() const;

      boost::optional<double> deltaTemperature() const;

      boost::optional<Schedule> deltaTemperatureSchedule() const;

      boost::optional<Schedule> minimumReceivingTemperatureSchedule() const;

      boost::optional<Schedule> maximumReceivingTemperatureSchedule() const;

      boost::optional<Schedule> minimumSourceTemperatureSchedule() const;

      boost::optional<Schedule> maximumSourceTemperatureSchedule() const;

      boost::optional<Schedule> minimumOutdoorTemperatureSchedule() const;

      boost::optional<Schedule> maximumOutdoorTemperatureSchedule() const;

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      //@}
      /** @name Setters */
      //@{

      bool setZone(const ThermalZone& zone);
      bool setSpace(const Space& space);

      bool setSchedule(Schedule& schedule);

      bool setDesignFlowRate(double designFlowRate);

      bool setFlowRateperFloorArea(double flowRateperFloorArea);

      bool setFlowRateperPerson(double flowRateperPerson);

      bool setAirChangesperHour(double airChangesperHour);

      bool setSourceZone(const ThermalZone& zone);
      bool setSourceSpace(const Space& space);
      void resetSourceZoneOrSpace();

      bool setDeltaTemperature(double deltaTemperature);

      void resetDeltaTemperature();

      bool setDeltaTemperatureSchedule(Schedule& schedule);

      void resetDeltaTemperatureSchedule();

      bool setMinimumReceivingTemperatureSchedule(Schedule& schedule);

      void resetMinimumReceivingTemperatureSchedule();

      bool setMaximumReceivingTemperatureSchedule(Schedule& schedule);

      void resetMaximumReceivingTemperatureSchedule();

      bool setMinimumSourceTemperatureSchedule(Schedule& schedule);

      void resetMinimumSourceTemperatureSchedule();

      bool setMaximumSourceTemperatureSchedule(Schedule& schedule);

      void resetMaximumSourceTemperatureSchedule();

      bool setMinimumOutdoorTemperatureSchedule(Schedule& schedule);

      void resetMinimumOutdoorTemperatureSchedule();

      bool setMaximumOutdoorTemperatureSchedule(Schedule& schedule);

      void resetMaximumOutdoorTemperatureSchedule();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ZoneMixing");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEMIXING_IMPL_HPP
