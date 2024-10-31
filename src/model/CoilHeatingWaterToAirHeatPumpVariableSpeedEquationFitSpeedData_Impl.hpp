/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILHEATINGWATERTOAIRHEATPUMPVARIABLESPEEDEQUATIONFITSPEEDDATA_IMPL_HPP
#define MODEL_COILHEATINGWATERTOAIRHEATPUMPVARIABLESPEEDEQUATIONFITSPEEDDATA_IMPL_HPP

#include "ModelAPI.hpp"
#include "ParentObject_Impl.hpp"

namespace openstudio {
namespace model {

  class Curve;

  namespace detail {

    /** CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl is a ParentObject_Impl that is the implementation class for CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData.*/
    class MODEL_API CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl : public ParentObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                          bool keepHandle);

      CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl(
        const CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<ModelObject> children() const override;

      //@}
      /** @name Getters */
      //@{

      double referenceUnitGrossRatedHeatingCapacity() const;

      double referenceUnitGrossRatedHeatingCOP() const;

      double referenceUnitRatedAirFlow() const;

      double referenceUnitRatedWaterFlowRate() const;

      Curve heatingCapacityFunctionofTemperatureCurve() const;

      Curve totalHeatingCapacityFunctionofAirFlowFractionCurve() const;

      Curve heatingCapacityFunctionofWaterFlowFractionCurve() const;

      Curve energyInputRatioFunctionofTemperatureCurve() const;

      Curve energyInputRatioFunctionofAirFlowFractionCurve() const;

      Curve energyInputRatioFunctionofWaterFlowFractionCurve() const;

      double referenceUnitWasteHeatFractionofInputPowerAtRatedConditions() const;

      Curve wasteHeatFunctionofTemperatureCurve() const;

      //@}
      /** @name Setters */
      //@{

      bool setReferenceUnitGrossRatedHeatingCapacity(double referenceUnitGrossRatedHeatingCapacity);

      bool setReferenceUnitGrossRatedHeatingCOP(double referenceUnitGrossRatedHeatingCOP);

      bool setReferenceUnitRatedAirFlow(double referenceUnitRatedAirFlow);

      bool setReferenceUnitRatedWaterFlowRate(double referenceUnitRatedWaterFlowRate);

      bool setHeatingCapacityFunctionofTemperatureCurve(const Curve& curve);

      bool setTotalHeatingCapacityFunctionofAirFlowFractionCurve(const Curve& curve);

      bool setHeatingCapacityFunctionofWaterFlowFractionCurve(const Curve& curve);

      bool setEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve);

      bool setEnergyInputRatioFunctionofAirFlowFractionCurve(const Curve& curve);

      bool setEnergyInputRatioFunctionofWaterFlowFractionCurve(const Curve& curve);

      bool setReferenceUnitWasteHeatFractionofInputPowerAtRatedConditions(double referenceUnitWasteHeatFractionofInputPowerAtRatedConditions);

      bool setWasteHeatFunctionofTemperatureCurve(const Curve& curve);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<Curve> optionalHeatingCapacityFunctionofTemperatureCurve() const;
      boost::optional<Curve> optionalTotalHeatingCapacityFunctionofAirFlowFractionCurve() const;
      boost::optional<Curve> optionalHeatingCapacityFunctionofWaterFlowFractionCurve() const;
      boost::optional<Curve> optionalEnergyInputRatioFunctionofTemperatureCurve() const;
      boost::optional<Curve> optionalEnergyInputRatioFunctionofAirFlowFractionCurve() const;
      boost::optional<Curve> optionalEnergyInputRatioFunctionofWaterFlowFractionCurve() const;
      boost::optional<Curve> optionalWasteHeatFunctionofTemperatureCurve() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGWATERTOAIRHEATPUMPVARIABLESPEEDEQUATIONFITSPEEDDATA_IMPL_HPP
