/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILHEATINGDXVARIABLEREFRIGERANTFLOW_IMPL_HPP
#define MODEL_COILHEATINGDXVARIABLEREFRIGERANTFLOW_IMPL_HPP

#include "ModelAPI.hpp"
#include "HVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class Curve;

  namespace detail {

    /** CoilHeatingDXVariableRefrigerantFlow_Impl is a HVACComponent_Impl that is the implementation class for CoilHeatingDXVariableRefrigerantFlow.*/
    class MODEL_API CoilHeatingDXVariableRefrigerantFlow_Impl : public HVACComponent_Impl
    {

     public:
      CoilHeatingDXVariableRefrigerantFlow_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilHeatingDXVariableRefrigerantFlow_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilHeatingDXVariableRefrigerantFlow_Impl(const CoilHeatingDXVariableRefrigerantFlow_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilHeatingDXVariableRefrigerantFlow_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<ModelObject> children() const override;

      virtual void autosize() override;

      virtual void applySizingValues() override;

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

      boost::optional<double> ratedAirFlowRate() const;

      bool isRatedAirFlowRateAutosized() const;

      Curve heatingCapacityRatioModifierFunctionofTemperatureCurve() const;

      Curve heatingCapacityModifierFunctionofFlowFractionCurve() const;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setRatedTotalHeatingCapacity(boost::optional<double> ratedTotalHeatingCapacity);

      void autosizeRatedTotalHeatingCapacity();

      bool setRatedAirFlowRate(boost::optional<double> ratedAirFlowRate);

      void autosizeRatedAirFlowRate();

      bool setHeatingCapacityRatioModifierFunctionofTemperatureCurve(const Curve& curve);

      bool setHeatingCapacityModifierFunctionofFlowFractionCurve(const Curve& curve);

      //@}
      /** @name Other */
      //@{

      boost::optional<double> autosizedRatedTotalHeatingCapacity() const;

      boost::optional<double> autosizedRatedAirFlowRate() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilHeatingDXVariableRefrigerantFlow");

      boost::optional<Schedule> optionalAvailabilitySchedule() const;
      boost::optional<Curve> optionalHeatingCapacityRatioModifierFunctionofTemperatureCurve() const;
      boost::optional<Curve> optionalHeatingCapacityModifierFunctionofFlowFractionCurve() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGDXVARIABLEREFRIGERANTFLOW_IMPL_HPP
