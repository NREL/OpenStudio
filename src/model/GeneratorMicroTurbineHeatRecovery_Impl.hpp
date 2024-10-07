/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_GENERATORMICROTURBINEHEATRECOVERY_IMPL_HPP
#define MODEL_GENERATORMICROTURBINEHEATRECOVERY_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {

namespace model {

  // TODO: Check the following class names against object getters and setters.
  class Curve;
  class GeneratorMicroTurbine;

  namespace detail {

    class MODEL_API GeneratorMicroTurbineHeatRecovery_Impl : public StraightComponent_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      GeneratorMicroTurbineHeatRecovery_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      GeneratorMicroTurbineHeatRecovery_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      GeneratorMicroTurbineHeatRecovery_Impl(const GeneratorMicroTurbineHeatRecovery_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~GeneratorMicroTurbineHeatRecovery_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual bool addToNode(Node& node) override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      // DLM: consider reimplementing any other virtual methods of StraightComponent or HVACComponent

      //@}
      /** @name Getters */
      //@{

      //boost::optional<Connection> heatRecoveryWaterInletNode() const;

      //boost::optional<Connection> heatRecoveryWaterOutletNode() const;

      // TODO: I might need to make this one default to something that makes sense...
      double referenceThermalEfficiencyUsingLowerHeatValue() const;
      bool isReferenceThermalEfficiencyUsingLowerHeatValueDefaulted() const;

      double referenceInletWaterTemperature() const;

      std::string heatRecoveryWaterFlowOperatingMode() const;
      bool isHeatRecoveryWaterFlowOperatingModeDefaulted() const;

      double referenceHeatRecoveryWaterFlowRate() const;

      // BiquadraticCurves, BiVariateTables.
      boost::optional<Curve> heatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve() const;

      // BicubicBiquadraticCurves, BiVariateTables.
      boost::optional<Curve> thermalEfficiencyFunctionofTemperatureandElevationCurve() const;

      // QuadraticCubicCurves, UniVariateTables.
      boost::optional<Curve> heatRecoveryRateFunctionofPartLoadRatioCurve() const;

      //QuadraticCurves, UniVariateTables.
      boost::optional<Curve> heatRecoveryRateFunctionofInletWaterTemperatureCurve() const;

      // QuadraticCurves, UniVariateTables.
      boost::optional<Curve> heatRecoveryRateFunctionofWaterFlowRateCurve() const;

      double minimumHeatRecoveryWaterFlowRate() const;
      bool isMinimumHeatRecoveryWaterFlowRateDefaulted() const;

      double maximumHeatRecoveryWaterFlowRate() const;
      bool isMaximumHeatRecoveryWaterFlowRateDefaulted() const;

      boost::optional<double> maximumHeatRecoveryWaterTemperature() const;

      double ratedThermaltoElectricalPowerRatio() const;
      bool isRatedThermaltoElectricalPowerRatioDefaulted() const;

      // Return parent generator
      GeneratorMicroTurbine generatorMicroTurbine() const;

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      //@}
      /** @name Setters */
      //@{

      // TODO: Check argument type. From object lists, some candidates are: Connection.
      //bool setHeatRecoveryWaterInletNode(const Connection& connection);
      //void resetHeatRecoveryWaterInletNode();

      // TODO: Check argument type. From object lists, some candidates are: Connection.
      //bool setHeatRecoveryWaterOutletNode(const Connection& connection);
      //void resetHeatRecoveryWaterOutletNode();

      bool setReferenceThermalEfficiencyUsingLowerHeatValue(double referenceThermalEfficiencyUsingLowerHeatValue);
      void resetReferenceThermalEfficiencyUsingLowerHeatValue();

      bool setReferenceInletWaterTemperature(double referenceInletWaterTemperature);

      bool setHeatRecoveryWaterFlowOperatingMode(const std::string& heatRecoveryWaterFlowOperatingMode);
      void resetHeatRecoveryWaterFlowOperatingMode();

      bool setReferenceHeatRecoveryWaterFlowRate(double referenceHeatRecoveryWaterFlowRate);

      // BiquadraticCurves, BiVariateTables.
      bool setHeatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve(const Curve& heatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve);
      void resetHeatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve();

      // BicubicBiquadraticCurves, BiVariateTables.
      bool setThermalEfficiencyFunctionofTemperatureandElevationCurve(const Curve& thermalEfficiencyFunctionofTemperatureandElevationCurve);
      void resetThermalEfficiencyFunctionofTemperatureandElevationCurve();

      // QuadraticCubicCurves, UniVariateTables.
      bool setHeatRecoveryRateFunctionofPartLoadRatioCurve(const Curve& heatRecoveryRateFunctionofPartLoadRatioCurve);
      void resetHeatRecoveryRateFunctionofPartLoadRatioCurve();

      // QuadraticCurves, UniVariateTables.
      bool setHeatRecoveryRateFunctionofInletWaterTemperatureCurve(const Curve& heatRecoveryRateFunctionofInletWaterTemperatureCurve);
      void resetHeatRecoveryRateFunctionofInletWaterTemperatureCurve();

      // QuadraticCurves, UniVariateTables.
      bool setHeatRecoveryRateFunctionofWaterFlowRateCurve(const Curve& heatRecoveryRateFunctionofWaterFlowRateCurve);
      void resetHeatRecoveryRateFunctionofWaterFlowRateCurve();

      bool setMinimumHeatRecoveryWaterFlowRate(double minimumHeatRecoveryWaterFlowRate);
      void resetMinimumHeatRecoveryWaterFlowRate();

      bool setMaximumHeatRecoveryWaterFlowRate(double maximumHeatRecoveryWaterFlowRate);
      void resetMaximumHeatRecoveryWaterFlowRate();

      bool setMaximumHeatRecoveryWaterTemperature(double maximumHeatRecoveryWaterTemperature);
      void resetMaximumHeatRecoveryWaterTemperature();

      bool setRatedThermaltoElectricalPowerRatio(double ratedThermaltoElectricalPowerRatio);
      void resetRatedThermaltoElectricalPowerRatio();

      //@}
      /** @name Other */
      //@{

      ModelObject clone(Model model) const override;

      std::vector<IddObjectType> allowableChildTypes() const override;

      std::vector<ModelObject> children() const override;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.GeneratorMicroTurbineHeatRecovery");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GENERATORMICROTURBINEHEATRECOVERY_IMPL_HPP
