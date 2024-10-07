/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_GENERATORFUELCELLSTACKCOOLER_IMPL_HPP
#define MODEL_GENERATORFUELCELLSTACKCOOLER_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Connection;
  class GeneratorFuelCell;

  namespace detail {

    /** GeneratorFuelCellStackCooler_Impl is a ModelObject_Impl that is the implementation class for GeneratorFuelCellStackCooler.*/
    class MODEL_API GeneratorFuelCellStackCooler_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      GeneratorFuelCellStackCooler_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      GeneratorFuelCellStackCooler_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      GeneratorFuelCellStackCooler_Impl(const GeneratorFuelCellStackCooler_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~GeneratorFuelCellStackCooler_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      double nominalStackTemperature() const;

      double actualStackTemperature() const;

      double coefficientr0() const;

      double coefficientr1() const;

      double coefficientr2() const;

      double coefficientr3() const;

      double stackCoolantFlowRate() const;

      double stackCoolerUFactorTimesAreaValue() const;

      double fscogenAdjustmentFactor() const;

      double stackCogenerationExchangerArea() const;

      double stackCogenerationExchangerNominalFlowRate() const;

      double stackCogenerationExchangerNominalHeatTransferCoefficient() const;

      double stackCogenerationExchangerNominalHeatTransferCoefficientExponent() const;

      double stackCoolerPumpPower() const;

      double stackCoolerPumpHeatLossFraction() const;

      double stackAirCoolerFanCoefficientf0() const;

      double stackAirCoolerFanCoefficientf1() const;

      double stackAirCoolerFanCoefficientf2() const;

      // Return optional parent generator
      GeneratorFuelCell fuelCell() const;

      //@}
      /** @name Setters */
      //@{

      bool setNominalStackTemperature(double nominalStackTemperature);

      void resetNominalStackTemperature();

      bool setActualStackTemperature(double actualStackTemperature);

      void resetActualStackTemperature();

      bool setCoefficientr0(double coefficientr0);

      void resetCoefficientr0();

      bool setCoefficientr1(double coefficientr1);

      void resetCoefficientr1();

      bool setCoefficientr2(double coefficientr2);

      void resetCoefficientr2();

      bool setCoefficientr3(double coefficientr3);

      void resetCoefficientr3();

      bool setStackCoolantFlowRate(double stackCoolantFlowRate);

      void resetStackCoolantFlowRate();

      bool setStackCoolerUFactorTimesAreaValue(double stackCoolerUFactorTimesAreaValue);

      void resetStackCoolerUFactorTimesAreaValue();

      bool setFscogenAdjustmentFactor(double fscogenAdjustmentFactor);

      void resetFscogenAdjustmentFactor();

      bool setStackCogenerationExchangerArea(double stackCogenerationExchangerArea);

      void resetStackCogenerationExchangerArea();

      bool setStackCogenerationExchangerNominalFlowRate(double stackCogenerationExchangerNominalFlowRate);

      void resetStackCogenerationExchangerNominalFlowRate();

      bool setStackCogenerationExchangerNominalHeatTransferCoefficient(double stackCogenerationExchangerNominalHeatTransferCoefficient);

      void resetStackCogenerationExchangerNominalHeatTransferCoefficient();

      bool
        setStackCogenerationExchangerNominalHeatTransferCoefficientExponent(double stackCogenerationExchangerNominalHeatTransferCoefficientExponent);

      void resetStackCogenerationExchangerNominalHeatTransferCoefficientExponent();

      bool setStackCoolerPumpPower(double stackCoolerPumpPower);

      void resetStackCoolerPumpPower();

      bool setStackCoolerPumpHeatLossFraction(double stackCoolerPumpHeatLossFraction);

      void resetStackCoolerPumpHeatLossFraction();

      bool setStackAirCoolerFanCoefficientf0(double stackAirCoolerFanCoefficientf0);

      void resetStackAirCoolerFanCoefficientf0();

      bool setStackAirCoolerFanCoefficientf1(double stackAirCoolerFanCoefficientf1);

      void resetStackAirCoolerFanCoefficientf1();

      bool setStackAirCoolerFanCoefficientf2(double stackAirCoolerFanCoefficientf2);

      void resetStackAirCoolerFanCoefficientf2();

      //@}
      /** @name Other */
      //@{
      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      bool addToNode(Node& node) override;

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<IddObjectType> allowableChildTypes() const override;

      virtual std::vector<ModelObject> children() const override;
      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.GeneratorFuelCellStackCooler");

      boost::optional<Connection> optionalInletNode() const;
      boost::optional<Connection> optionalOutletNode() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GENERATORFUELCELLSTACKCOOLER_IMPL_HPP
