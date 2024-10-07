/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_FANZONEEXHAUST_IMPL_HPP
#define MODEL_FANZONEEXHAUST_IMPL_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;

  namespace detail {

    /** FanZoneExhaust_Impl is a ZoneHVACComponent_Impl that is the implementation class for FanZoneExhaust.*/
    class MODEL_API FanZoneExhaust_Impl : public ZoneHVACComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      FanZoneExhaust_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      FanZoneExhaust_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      FanZoneExhaust_Impl(const FanZoneExhaust_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~FanZoneExhaust_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ModelObject> children() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual boost::optional<ThermalZone> thermalZone() const override;

      virtual bool addToThermalZone(ThermalZone& thermalZone) override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<Schedule> availabilitySchedule() const;

      double fanTotalEfficiency() const;

      double pressureRise() const;

      boost::optional<double> maximumFlowRate() const;

      std::string endUseSubcategory() const;

      boost::optional<Schedule> flowFractionSchedule() const;

      std::string systemAvailabilityManagerCouplingMode() const;

      boost::optional<Schedule> minimumZoneTemperatureLimitSchedule() const;

      boost::optional<Schedule> balancedExhaustFractionSchedule() const;

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      void resetAvailabilitySchedule();

      bool setFanTotalEfficiency(double fanTotalEfficiency);

      bool setPressureRise(double pressureRise);

      bool setMaximumFlowRate(boost::optional<double> maximumFlowRate);

      void resetMaximumFlowRate();

      bool setEndUseSubcategory(const std::string& endUseSubcategory);

      bool setFlowFractionSchedule(Schedule& schedule);

      void resetFlowFractionSchedule();

      bool setSystemAvailabilityManagerCouplingMode(const std::string& systemAvailabilityManagerCouplingMode);

      bool setMinimumZoneTemperatureLimitSchedule(Schedule& schedule);

      void resetMinimumZoneTemperatureLimitSchedule();

      bool setBalancedExhaustFractionSchedule(Schedule& schedule);

      void resetBalancedExhaustFractionSchedule();

      //@}
      /** @name Other */
      //@{

      AirflowNetworkZoneExhaustFan getAirflowNetworkZoneExhaustFan(const AirflowNetworkCrack& crack);
      boost::optional<AirflowNetworkZoneExhaustFan> airflowNetworkZoneExhaustFan() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.FanZoneExhaust");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_FANZONEEXHAUST_IMPL_HPP
