/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILHEATINGDXVARIABLEREFRIGERANTFLOWFLUIDTEMPERATURECONTROL_IMPL_HPP
#define MODEL_COILHEATINGDXVARIABLEREFRIGERANTFLOWFLUIDTEMPERATURECONTROL_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "HVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class Curve;

  namespace detail {

    /** CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl is a HVACComponent_Impl that is the implementation class for CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl.*/
    class MODEL_API CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl : public HVACComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                       bool keepHandle);

      CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl(const CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl& other,
                                                                       Model_Impl* model, bool keepHandle);

      virtual ~CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl() {}

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

      boost::optional<double> ratedTotalHeatingCapacity() const;

      bool isRatedTotalHeatingCapacityAutosized() const;

      boost::optional<double> autosizedRatedTotalHeatingCapacity();

      double indoorUnitReferenceSubcooling() const;

      Curve indoorUnitCondensingTemperatureFunctionofSubcoolingCurve() const;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setRatedTotalHeatingCapacity(double ratedTotalHeatingCapacity);

      void autosizeRatedTotalHeatingCapacity();

      bool setIndoorUnitReferenceSubcooling(double indoorUnitReferenceSubcooling);

      bool setIndoorUnitCondensingTemperatureFunctionofSubcoolingCurve(const Curve& curve);

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
      REGISTER_LOGGER("openstudio.model.CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl");

      boost::optional<Schedule> optionalAvailabilitySchedule() const;
      boost::optional<Curve> optionalIndoorUnitCondensingTemperatureFunctionofSubcoolingCurve() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGDXVARIABLEREFRIGERANTFLOWFLUIDTEMPERATURECONTROL_IMPL_HPP
