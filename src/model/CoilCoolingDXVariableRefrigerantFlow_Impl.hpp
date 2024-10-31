/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILCOOLINGDXVARIABLEREFRIGERANTFLOW_IMPL_HPP
#define MODEL_COILCOOLINGDXVARIABLEREFRIGERANTFLOW_IMPL_HPP

#include "ModelAPI.hpp"
#include "HVACComponent_Impl.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class Curve;

  namespace detail {

    /** CoilCoolingDXVariableRefrigerantFlow_Impl is a HVACComponent_Impl that is the implementation class for CoilCoolingDXVariableRefrigerantFlow.*/
    class MODEL_API CoilCoolingDXVariableRefrigerantFlow_Impl : public HVACComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilCoolingDXVariableRefrigerantFlow_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilCoolingDXVariableRefrigerantFlow_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilCoolingDXVariableRefrigerantFlow_Impl(const CoilCoolingDXVariableRefrigerantFlow_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilCoolingDXVariableRefrigerantFlow_Impl() override = default;

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

      boost::optional<double> ratedSensibleHeatRatio() const;

      bool isRatedSensibleHeatRatioAutosized() const;

      boost::optional<double> ratedAirFlowRate() const;

      bool isRatedAirFlowRateAutosized() const;

      Curve coolingCapacityRatioModifierFunctionofTemperatureCurve() const;

      Curve coolingCapacityModifierCurveFunctionofFlowFraction() const;

      boost::optional<double> autosizedRatedTotalCoolingCapacity() const;

      boost::optional<double> autosizedRatedSensibleHeatRatio() const;

      boost::optional<double> autosizedRatedAirFlowRate() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setRatedTotalCoolingCapacity(boost::optional<double> ratedTotalCoolingCapacity);

      void autosizeRatedTotalCoolingCapacity();

      bool setRatedSensibleHeatRatio(boost::optional<double> ratedSensibleHeatRatio);

      void autosizeRatedSensibleHeatRatio();

      bool setRatedAirFlowRate(boost::optional<double> ratedAirFlowRate);

      void autosizeRatedAirFlowRate();

      bool setCoolingCapacityRatioModifierFunctionofTemperatureCurve(const Curve& curve);

      bool setCoolingCapacityModifierCurveFunctionofFlowFraction(const Curve& curve);

      //@}
      /** @name Other */
      //@{

      ModelObject clone(Model model) const override;

      std::vector<ModelObject> children() const override;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilCoolingDXVariableRefrigerantFlow");

      boost::optional<Schedule> optionalAvailabilitySchedule() const;
      boost::optional<Curve> optionalCoolingCapacityRatioModifierFunctionofTemperatureCurve() const;
      boost::optional<Curve> optionalCoolingCapacityModifierCurveFunctionofFlowFraction() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGDXVARIABLEREFRIGERANTFLOW_IMPL_HPP
