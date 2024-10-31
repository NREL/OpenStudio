/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_GENERATORFUELCELLEXHAUSTGASTOWATERHEATEXCHANGER_IMPL_HPP
#define MODEL_GENERATORFUELCELLEXHAUSTGASTOWATERHEATEXCHANGER_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {

namespace model {

  class Connection;
  class Node;
  class GeneratorFuelCell;

  namespace detail {

    /** GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl is a ModelObject_Impl that is the implementation class for GeneratorFuelCellExhaustGasToWaterHeatExchanger.*/
    class MODEL_API GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl(const GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl& other, Model_Impl* model,
                                                           bool keepHandle);

      virtual ~GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual bool addToNode(Node& node) override;

      /** For OS App-related reasons, so we can drag this object from library onto a plantLoop
        * This will clone the parent fuelCell (which clones all of his children) and return a reference
        * to that clone's HX */
      virtual ModelObject clone(Model model) const override;

      virtual std::vector<IddObjectType> allowableChildTypes() const override;

      virtual std::vector<ModelObject> children() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      double heatRecoveryWaterMaximumFlowRate() const;

      boost::optional<Node> exhaustOutletAirNode() const;

      std::string heatExchangerCalculationMethod() const;

      boost::optional<double> method1HeatExchangerEffectiveness() const;

      boost::optional<double> method2Parameterhxs0() const;

      boost::optional<double> method2Parameterhxs1() const;

      boost::optional<double> method2Parameterhxs2() const;

      boost::optional<double> method2Parameterhxs3() const;

      boost::optional<double> method2Parameterhxs4() const;

      boost::optional<double> method3h0GasCoefficient() const;

      boost::optional<double> method3NdotGasRefCoefficient() const;

      boost::optional<double> method3nCoefficient() const;

      boost::optional<double> method3GasArea() const;

      boost::optional<double> method3h0WaterCoefficient() const;

      boost::optional<double> method3NdotWaterrefCoefficient() const;

      boost::optional<double> method3mCoefficient() const;

      boost::optional<double> method3WaterArea() const;

      boost::optional<double> method3FAdjustmentFactor() const;

      boost::optional<double> method4hxl1Coefficient() const;

      boost::optional<double> method4hxl2Coefficient() const;

      boost::optional<double> method4CondensationThreshold() const;

      // Return optional parent generator
      boost::optional<GeneratorFuelCell> fuelCell() const;

      //@}
      /** @name Setters */
      //@{

      bool setHeatRecoveryWaterMaximumFlowRate(double heatRecoveryWaterMaximumFlowRate);

      void resetHeatRecoveryWaterMaximumFlowRate();

      bool setExhaustOutletAirNode(const Node& node);

      void resetExhaustOutletAirNode();

      bool setHeatExchangerCalculationMethod(const std::string& heatExchangerCalculationMethod);

      void resetHeatExchangerCalculationMethod();

      bool setMethod1HeatExchangerEffectiveness(double method1HeatExchangerEffectiveness);

      void resetMethod1HeatExchangerEffectiveness();

      bool setMethod2Parameterhxs0(double method2Parameterhxs0);

      void resetMethod2Parameterhxs0();

      bool setMethod2Parameterhxs1(double method2Parameterhxs1);

      void resetMethod2Parameterhxs1();

      bool setMethod2Parameterhxs2(double method2Parameterhxs2);

      void resetMethod2Parameterhxs2();

      bool setMethod2Parameterhxs3(double method2Parameterhxs3);

      void resetMethod2Parameterhxs3();

      bool setMethod2Parameterhxs4(double method2Parameterhxs4);

      void resetMethod2Parameterhxs4();

      bool setMethod3h0GasCoefficient(double method3h0GasCoefficient);

      void resetMethod3h0GasCoefficient();

      bool setMethod3NdotGasRefCoefficient(double method3NdotGasRefCoefficient);

      void resetMethod3NdotGasRefCoefficient();

      bool setMethod3nCoefficient(double method3nCoefficient);

      void resetMethod3nCoefficient();

      bool setMethod3GasArea(double method3GasArea);

      void resetMethod3GasArea();

      bool setMethod3h0WaterCoefficient(double method3h0WaterCoefficient);

      void resetMethod3h0WaterCoefficient();

      bool setMethod3NdotWaterrefCoefficient(double method3NdotWaterrefCoefficient);

      void resetMethod3NdotWaterrefCoefficient();

      bool setMethod3mCoefficient(double method3mCoefficient);

      void resetMethod3mCoefficient();

      bool setMethod3WaterArea(double method3WaterArea);

      void resetMethod3WaterArea();

      bool setMethod3FAdjustmentFactor(double method3FAdjustmentFactor);

      void resetMethod3FAdjustmentFactor();

      bool setMethod4hxl1Coefficient(double method4hxl1Coefficient);

      void resetMethod4hxl1Coefficient();

      bool setMethod4hxl2Coefficient(double method4hxl2Coefficient);

      void resetMethod4hxl2Coefficient();

      bool setMethod4CondensationThreshold(double method4CondensationThreshold);

      void resetMethod4CondensationThreshold();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.GeneratorFuelCellExhaustGasToWaterHeatExchanger");

      boost::optional<Connection> optionalInletNode() const;
      boost::optional<Connection> optionalOutletNode() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GENERATORFUELCELLEXHAUSTGASTOWATERHEATEXCHANGER_IMPL_HPP
