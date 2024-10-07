/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_GAS_IMPL_HPP
#define MODEL_GAS_IMPL_HPP

#include "ModelAPI.hpp"
#include "GasLayer_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** Gas_Impl is a GasLayer_Impl that is the implementation class for Gas.*/
    class MODEL_API Gas_Impl : public GasLayer_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      Gas_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      Gas_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      Gas_Impl(const Gas_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~Gas_Impl() override = default;

      //@}

      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      /** Gets the gas type. Throws if value is not available. */
      std::string gasType() const;

      /** Get the thickness of the material. Will throw if value is not available. */
      virtual double thickness() const override;

      /** The conductivity (W/m*K) at temperature (K). Will throw if necessary values are not
     *  available. */
      double getThermalConductivity(double temperature) const;

      /** The conductance (W/m^2*K) at temperature (K). Will throw if necessary values are not
     *  available. */
      double getThermalConductance(double temperature) const;

      /** The resistivity (m*K/W) at temperature (K). Will throw if necessary values are not
     *  available. */
      double getThermalResistivity(double temperature) const;

      /** The resistance (m^2*K/W) at temperature (K). Will throw if necessary values are not
     *  available. */
      double getThermalResistance(double temperature) const;

      double getViscosity(double temperature) const;

      double getSpecificHeat(double temperature) const;

      boost::optional<double> customConductivityCoefficientA() const;

      boost::optional<double> customConductivityCoefficientB() const;

      boost::optional<double> customConductivityCoefficientC() const;

      boost::optional<double> customViscosityCoefficientA() const;

      boost::optional<double> customViscosityCoefficientB() const;

      boost::optional<double> customViscosityCoefficientC() const;

      boost::optional<double> customSpecificHeatCoefficientA() const;

      boost::optional<double> customSpecificHeatCoefficientB() const;

      boost::optional<double> customSpecificHeatCoefficientC() const;

      boost::optional<double> customMolecularWeight() const;

      boost::optional<double> conductivityCoefficientA() const;

      boost::optional<double> conductivityCoefficientB() const;

      boost::optional<double> conductivityCoefficientC() const;

      boost::optional<double> viscosityCoefficientA() const;

      boost::optional<double> viscosityCoefficientB() const;

      boost::optional<double> viscosityCoefficientC() const;

      boost::optional<double> specificHeatCoefficientA() const;

      boost::optional<double> specificHeatCoefficientB() const;

      boost::optional<double> specificHeatCoefficientC() const;

      boost::optional<double> molecularWeight() const;

      boost::optional<double> specificHeatRatio() const;

      //@}
      /** @name Setters */
      //@{

      virtual bool setThickness(double value) override;

      bool setCustomConductivity(double A, double B, double C);

      bool setCustomViscosity(double A, double B, double C);

      bool setCustomSpecificHeat(double A, double B, double C);

      bool setCustomMolecularWeight(double value);

      bool setGasType(const std::string& gasType);

      bool setConductivityCoefficientA(boost::optional<double> conductivityCoefficientA);

      void resetConductivityCoefficientA();

      bool setConductivityCoefficientB(boost::optional<double> conductivityCoefficientB);

      void resetConductivityCoefficientB();

      bool setConductivityCoefficientC(boost::optional<double> conductivityCoefficientC);

      void resetConductivityCoefficientC();

      bool setViscosityCoefficientA(boost::optional<double> viscosityCoefficientA);

      void resetViscosityCoefficientA();

      bool setViscosityCoefficientB(boost::optional<double> viscosityCoefficientB);

      void resetViscosityCoefficientB();

      bool setViscosityCoefficientC(boost::optional<double> viscosityCoefficientC);

      void resetViscosityCoefficientC();

      bool setSpecificHeatCoefficientA(boost::optional<double> specificHeatCoefficientA);

      void resetSpecificHeatCoefficientA();

      bool setSpecificHeatCoefficientB(boost::optional<double> specificHeatCoefficientB);

      void resetSpecificHeatCoefficientB();

      bool setSpecificHeatCoefficientC(boost::optional<double> specificHeatCoefficientC);

      void resetSpecificHeatCoefficientC();

      bool setMolecularWeight(boost::optional<double> molecularWeight);

      void resetMolecularWeight();

      bool setSpecificHeatRatio(boost::optional<double> specificHeatRatio);

      void resetSpecificHeatRatio();

      //@}
      /** @name Queries */
      //@{

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.Gas");

      std::vector<std::string> gasTypeValues() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GAS_IMPL_HPP
