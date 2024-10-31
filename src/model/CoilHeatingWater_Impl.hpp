/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILHEATINGWATER_IMPL_HPP
#define MODEL_COILHEATINGWATER_IMPL_HPP

#include "WaterToAirComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;

  namespace detail {

    class MODEL_API CoilHeatingWater_Impl : public WaterToAirComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilHeatingWater_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilHeatingWater_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilHeatingWater_Impl(const CoilHeatingWater_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilHeatingWater_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual std::vector<openstudio::IdfObject> remove() override;

      virtual bool removeFromPlantLoop() override;

      virtual ModelObject clone(Model model) const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ModelObject> children() const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual bool addToNode(Node& node) override;

      virtual unsigned airInletPort() const override;

      virtual unsigned airOutletPort() const override;

      virtual unsigned waterInletPort() const override;

      virtual unsigned waterOutletPort() const override;

      virtual boost::optional<HVACComponent> containingHVACComponent() const override;

      virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters and Setters */
      //@{

      Schedule availabilitySchedule() const;

      bool setAvailabilitySchedule(Schedule& schedule);

      boost::optional<double> uFactorTimesAreaValue();

      bool setUFactorTimesAreaValue(double value);

      bool isUFactorTimesAreaValueAutosized();

      void autosizeUFactorTimesAreaValue();

      boost::optional<double> maximumWaterFlowRate();

      bool setMaximumWaterFlowRate(double value);

      bool isMaximumWaterFlowRateAutosized();

      void autosizeMaximumWaterFlowRate();

      std::string performanceInputMethod();

      bool setPerformanceInputMethod(const std::string& value);

      boost::optional<double> ratedCapacity();

      bool setRatedCapacity(double value);

      bool isRatedCapacityAutosized();

      void autosizeRatedCapacity();

      double ratedInletWaterTemperature();

      bool setRatedInletWaterTemperature(double value);

      double ratedInletAirTemperature();

      bool setRatedInletAirTemperature(double value);

      double ratedOutletWaterTemperature();

      bool setRatedOutletWaterTemperature(double value);

      double ratedOutletAirTemperature();

      bool setRatedOutletAirTemperature(double value);

      double ratedRatioForAirAndWaterConvection();

      bool setRatedRatioForAirAndWaterConvection(double value);

      boost::optional<double> autosizedUFactorTimesAreaValue() const;

      boost::optional<double> autosizedMaximumWaterFlowRate() const;

      boost::optional<double> autosizedRatedCapacity() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      //@}
      /** @name Other */
      //@{

      AirflowNetworkEquivalentDuct getAirflowNetworkEquivalentDuct(double length, double diameter);
      boost::optional<AirflowNetworkEquivalentDuct> airflowNetworkEquivalentDuct() const;

      //@}

     private:
      REGISTER_LOGGER("openstudio.model.CoilHeatingWater");

      boost::optional<ModelObject> availabilityScheduleAsModelObject() const;

      bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGWATER_IMPL_HPP
