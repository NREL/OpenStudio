/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_GASMIXTURE_IMPL_HPP
#define MODEL_GASMIXTURE_IMPL_HPP

#include "ModelAPI.hpp"
#include "GasLayer_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** GasMixture_Impl is a GasLayer_Impl that is the implementation class for GasMixture.*/
    class MODEL_API GasMixture_Impl : public GasLayer_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      GasMixture_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      GasMixture_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      GasMixture_Impl(const GasMixture_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~GasMixture_Impl() override = default;

      //@}

      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      /** Get the thickness of the material. */
      virtual double thickness() const override;

      /** Get the type of gas at index, where index starts at 0. Throws if unavailable. */
      std::string getGasType(unsigned index) const;

      double getGasFraction(unsigned index) const;

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

      int numberofGasesinMixture() const;

      std::string gas1Type() const;

      double gas1Fraction() const;

      std::string gas2Type() const;

      double gas2Fraction() const;

      std::string gas3Type() const;

      boost::optional<double> gas3Fraction() const;

      std::string gas4Type() const;

      boost::optional<double> gas4Fraction() const;

      //@}
      /** @name Setters */
      //@{

      /** Set thickness to value (m). */
      virtual bool setThickness(double value) override;

      /** Set the gas at index, if index <= numGases. */
      bool setGas(unsigned index, const std::string& type, double fraction);

      bool setGasType(unsigned index, const std::string& type);

      bool setGasFraction(unsigned index, double fraction);

      /** Append gas if numGases < 4. */
      bool addGas(const std::string& type, double fraction);

      /** Set the gas fractions. Length of vector should be equal to numGases, and sum should be
     *  openstudio::equal(1.0). If all fractions except the last is < 1.0, will set last fraction
     *  value if necessary (and issue warning). */
      bool setFractions(const std::vector<double>& fractions);

      /** Assign gas fraction at index to 1.0 - sum(other gas fractions), if index < numGases. */
      bool balanceFractions(unsigned index);

      bool setNumberofGasesinMixture(int numberofGasesinMixture);

      bool setGas1Type(const std::string& gas1Type);

      void resetGas1Type();

      bool setGas1Fraction(double gas1Fraction);

      void resetGas1Fraction();

      bool setGas2Type(const std::string& gas2Type);

      void resetGas2Type();

      bool setGas2Fraction(double gas2Fraction);

      void resetGas2Fraction();

      bool setGas3Type(boost::optional<std::string> gas3Type);

      void resetGas3Type();

      bool setGas3Fraction(boost::optional<double> gas3Fraction);

      void resetGas3Fraction();

      bool setGas4Type(boost::optional<std::string> gas4Type);

      void resetGas4Type();

      bool setGas4Fraction(boost::optional<double> gas4Fraction);

      void resetGas4Fraction();

      //@}
      /** @name Queries */
      //@{

      unsigned numGases() const;

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.GasMixture");

      unsigned mf_getGasTypeFieldIndex(unsigned gasIndex) const;

      unsigned mf_getGasFractionFieldIndex(unsigned gasIndex) const;

      std::vector<std::string> gas1TypeValues() const;
      std::vector<std::string> gas2TypeValues() const;
      std::vector<std::string> gas3TypeValues() const;
      std::vector<std::string> gas4TypeValues() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GASMIXTURE_IMPL_HPP
