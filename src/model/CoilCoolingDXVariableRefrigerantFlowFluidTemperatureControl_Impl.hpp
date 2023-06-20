/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILCOOLINGDXVARIABLEREFRIGERANTFLOWFLUIDTEMPERATURECONTROL_IMPL_HPP
#define MODEL_COILCOOLINGDXVARIABLEREFRIGERANTFLOWFLUIDTEMPERATURECONTROL_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "HVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class Curve;

  namespace detail {

    /** CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl is a HVACComponent_Impl that is the implementation class for CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl.*/
    class MODEL_API CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl : public HVACComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                       bool keepHandle);

      CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl(const CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl& other,
                                                                       Model_Impl* model, bool keepHandle);

      virtual ~CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl() {}

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      Schedule availabilitySchedule() const;

      boost::optional<double> ratedTotalCoolingCapacity() const;

      bool isRatedTotalCoolingCapacityAutosized() const;

      boost::optional<double> autosizedRatedTotalCoolingCapacity();

      boost::optional<double> ratedSensibleHeatRatio() const;

      bool isRatedSensibleHeatRatioAutosized() const;

      boost::optional<double> autosizedRatedSensibleHeatRatio();

      double indoorUnitReferenceSuperheating() const;

      Curve indoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve() const;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setRatedTotalCoolingCapacity(double ratedTotalCoolingCapacity);

      void autosizeRatedTotalCoolingCapacity();

      bool setRatedSensibleHeatRatio(double ratedSensibleHeatRatio);

      void autosizeRatedSensibleHeatRatio();

      bool setIndoorUnitReferenceSuperheating(double indoorUnitReferenceSuperheating);

      bool setIndoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve(const Curve& curve);

      virtual void autosize() override;

      virtual void applySizingValues() override;

      //@}
      /** @name Other */
      //@{

      ModelObject clone(Model model) const override;

      std::vector<ModelObject> children() const override;

      boost::optional<double> autosizedRatedAirFlowRate() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl");

      boost::optional<Schedule> optionalAvailabilitySchedule() const;
      boost::optional<Curve> optionalIndoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGDXVARIABLEREFRIGERANTFLOWFLUIDTEMPERATURECONTROL_IMPL_HPP
