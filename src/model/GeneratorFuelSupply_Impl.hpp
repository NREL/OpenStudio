/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_GENERATORFUELSUPPLY_IMPL_HPP
#define MODEL_GENERATORFUELSUPPLY_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class Node;
  class Schedule;
  class CurveCubic;
  class GeneratorFuelCell;
  class FuelSupplyConstituent;

  namespace detail {

    /** GeneratorFuelSupply_Impl is a ModelObject_Impl that is the implementation class for GeneratorFuelSupply.*/
    class MODEL_API GeneratorFuelSupply_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      GeneratorFuelSupply_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      GeneratorFuelSupply_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      GeneratorFuelSupply_Impl(const GeneratorFuelSupply_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~GeneratorFuelSupply_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual std::vector<IddObjectType> allowableChildTypes() const;

      virtual std::vector<ModelObject> children() const;

      //extensible fields
      bool addConstituent(const FuelSupplyConstituent& constituent);
      bool addConstituent(const std::string& name, double molarFraction);

      bool removeConstituent(unsigned groupIndex);
      void removeAllConstituents();

      std::vector<FuelSupplyConstituent> constituents() const;

      //@}
      /** @name Getters */
      //@{

      std::string fuelTemperatureModelingMode() const;

      boost::optional<Node> fuelTemperatureReferenceNode() const;

      boost::optional<Schedule> fuelTemperatureSchedule() const;

      CurveCubic compressorPowerMultiplierFunctionofFuelRateCurve() const;

      double compressorHeatLossFactor() const;

      std::string fuelType() const;

      boost::optional<double> liquidGenericFuelLowerHeatingValue() const;

      boost::optional<double> liquidGenericFuelHigherHeatingValue() const;

      boost::optional<double> liquidGenericFuelMolecularWeight() const;

      boost::optional<double> liquidGenericFuelCO2EmissionFactor() const;

      unsigned int numberofConstituentsinGaseousConstituentFuelSupply() const;

      // Convenience function to check that it's equal to 1.0 (If no constituents, returns 0 and warns)
      double sumofConstituentsMolarFractions() const;

      // Return optional parent generator
      boost::optional<GeneratorFuelCell> fuelCell() const;

      //@}
      /** @name Setters */
      //@{

      bool setFuelTemperatureModelingMode(const std::string& fuelTemperatureModelingMode);

      void resetFuelTemperatureModelingMode();

      bool setFuelTemperatureReferenceNode(const Node& connection);

      void resetFuelTemperatureReferenceNode();

      bool setFuelTemperatureSchedule(Schedule& schedule);

      void resetFuelTemperatureSchedule();

      bool setCompressorPowerMultiplierFunctionofFuelRateCurve(const CurveCubic& cubicCurves);

      bool setCompressorHeatLossFactor(double compressorHeatLossFactor);

      void resetCompressorHeatLossFactor();

      bool setFuelType(const std::string& fuelType);

      void resetFuelType();

      bool setLiquidGenericFuelLowerHeatingValue(double liquidGenericFuelLowerHeatingValue);

      void resetLiquidGenericFuelLowerHeatingValue();

      bool setLiquidGenericFuelHigherHeatingValue(double liquidGenericFuelHigherHeatingValue);

      void resetLiquidGenericFuelHigherHeatingValue();

      bool setLiquidGenericFuelMolecularWeight(double liquidGenericFuelMolecularWeight);

      void resetLiquidGenericFuelMolecularWeight();

      bool setLiquidGenericFuelCO2EmissionFactor(double liquidGenericFuelCO2EmissionFactor);

      void resetLiquidGenericFuelCO2EmissionFactor();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.GeneratorFuelSupply");
      // Not to be exposed publicly, automatically handled by addConstituent, removeConstituent
      bool setNumberofConstituentsinGaseousConstituentFuelSupply(unsigned int numberofConstituentsinGaseousConstituentFuelSupply);
      void resetNumberofConstituentsinGaseousConstituentFuelSupply();
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GENERATORFUELSUPPLY_IMPL_HPP
