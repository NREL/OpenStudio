/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_GENERATORFUELCELLAIRSUPPLY_IMPL_HPP
#define MODEL_GENERATORFUELCELLAIRSUPPLY_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class Node;
  class CurveCubic;
  class CurveQuadratic;
  class GeneratorFuelCell;
  class AirSupplyConstituent;

  namespace detail {

    /** GeneratorFuelCellAirSupply_Impl is a ModelObject_Impl that is the implementation class for GeneratorFuelCellAirSupply.*/
    class MODEL_API GeneratorFuelCellAirSupply_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      GeneratorFuelCellAirSupply_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      GeneratorFuelCellAirSupply_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      GeneratorFuelCellAirSupply_Impl(const GeneratorFuelCellAirSupply_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~GeneratorFuelCellAirSupply_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<IddObjectType> allowableChildTypes() const;

      virtual std::vector<ModelObject> children() const;

      //extensible fields.
      bool addConstituent(const AirSupplyConstituent& constituent);
      bool addConstituent(const std::string& name, double molarFraction);

      bool removeConstituent(unsigned groupIndex);

      void removeAllConstituents();

      std::vector<AirSupplyConstituent> constituents() const;

      //@}
      /** @name Getters */
      //@{

      boost::optional<Node> airInletNode() const;

      boost::optional<CurveCubic> blowerPowerCurve() const;

      double blowerHeatLossFactor() const;

      std::string airSupplyRateCalculationMode() const;

      boost::optional<double> stoichiometricRatio() const;

      boost::optional<CurveQuadratic> airRateFunctionofElectricPowerCurve() const;

      boost::optional<double> airRateAirTemperatureCoefficient() const;

      boost::optional<CurveQuadratic> airRateFunctionofFuelRateCurve() const;

      std::string airIntakeHeatRecoveryMode() const;

      std::string airSupplyConstituentMode() const;

      unsigned int numberofUserDefinedConstituents() const;

      // Convenience function to check that it's equal to 1.0 (If no constituents, returns 0 and warns)
      double sumofConstituentsMolarFractions() const;

      // Return optional parent generator
      boost::optional<GeneratorFuelCell> fuelCell() const;

      //@}
      /** @name Setters */
      //@{

      bool setAirInletNode(const Node& connection);

      void resetAirInletNode();

      bool setBlowerPowerCurve(const CurveCubic& cubicCurves);

      void resetBlowerPowerCurve();

      bool setBlowerHeatLossFactor(double blowerHeatLossFactor);

      void resetBlowerHeatLossFactor();

      bool setAirSupplyRateCalculationMode(const std::string& airSupplyRateCalculationMode);

      bool setStoichiometricRatio(double stoichiometricRatio);

      void resetStoichiometricRatio();

      bool setAirRateFunctionofElectricPowerCurve(const CurveQuadratic& quadraticCurves);

      void resetAirRateFunctionofElectricPowerCurve();

      bool setAirRateAirTemperatureCoefficient(double airRateAirTemperatureCoefficient);

      void resetAirRateAirTemperatureCoefficient();

      bool setAirRateFunctionofFuelRateCurve(const CurveQuadratic& quadraticCurves);

      void resetAirRateFunctionofFuelRateCurve();

      bool setAirIntakeHeatRecoveryMode(const std::string& airIntakeHeatRecoveryMode);

      bool setAirSupplyConstituentMode(const std::string& airSupplyConstituentMode);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      // Not to be exposed publicly, handled automatically by add/removeConstituent
      bool setNumberofUserDefinedConstituents(unsigned int numberofUserDefinedConstituents);
      void resetNumberofUserDefinedConstituents();

      REGISTER_LOGGER("openstudio.model.GeneratorFuelCellAirSupply");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GENERATORFUELCELLAIRSUPPLY_IMPL_HPP
