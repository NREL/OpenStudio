/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_GASMIXTURE_HPP
#define MODEL_GASMIXTURE_HPP

#include "ModelAPI.hpp"
#include "GasLayer.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class GasMixture_Impl;

  }  // namespace detail

  /** GasMixture is a GasLayer that wraps the OpenStudio IDD object 'OS:WindowMaterial:GasMixture'. */
  class MODEL_API GasMixture : public GasLayer
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit GasMixture(const Model& model, int numberofGasesinMixture = 4, double thickness = 0.003,
                        // Valid gas types: Air, Argon, Krypton, Xenon
                        std::string gas1Type = "Air", double gas1Fraction = .97, std::string gas2Type = "Argon", double gas2Fraction = 0.01,
                        std::string gas3Type = "Krypton", double gas3Fraction = 0.01, std::string gas4Type = "Xenon", double gas4Fraction = 0.01);

    virtual ~GasMixture() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    GasMixture(const GasMixture& other) = default;
    GasMixture(GasMixture&& other) = default;
    GasMixture& operator=(const GasMixture&) = default;
    GasMixture& operator=(GasMixture&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static std::vector<std::string> validGasTypes();

    static IddObjectType iddObjectType();

    static std::vector<std::string> gas1TypeValues();

    static std::vector<std::string> gas2TypeValues();

    static std::vector<std::string> gas3TypeValues();

    static std::vector<std::string> gas4TypeValues();

    //@}
    /** @name Getters */
    //@{

    /** Get the type of gas at index, where index starts at 0. Throws if unavailable. */
    std::string getGasType(unsigned index) const;

    /** Get the mixture fraction of the gas at index, where index starts at 0. Throws if
   *  unavailable. */
    double getGasFraction(unsigned index) const;

    /** The conductivity (W/m*K) at temperature (K). */
    double getThermalConductivity(double temperature) const;

    /** The conductance (W/m^2*K) at temperature (K). */
    double getThermalConductance(double temperature) const;

    /** The resistivity (m*K/W) at temperature (K). */
    double getThermalResistivity(double temperature) const;

    /** The resistance (m^2*K/W) at temperature (K). */
    double getThermalResistance(double temperature) const;

    /** The viscosity (kg/m*s) of the mixture at temperature (K). */
    double getViscosity(double temperature) const;

    /** The specific heat (J/kg*K) of the mixture at temperature (K). */
    double getSpecificHeat(double temperature) const;

    double thickness() const;

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

    /** Set the gas at index, if index <= numGases and index < 4. \sa validGasTypes */
    bool setGas(unsigned index, const std::string& type, double fraction);

    /** Set the type of gas at index, if index < numGases. \sa validGasTypes */
    bool setGasType(unsigned index, const std::string& type);

    /** Set the fraction of gas at index, if index < numGases. */
    bool setGasFraction(unsigned index, double fraction);

    /** Append gas if numGases < 4. \sa validGasTypes */
    bool addGas(const std::string& type, double fraction);

    /** Set the gas fractions. Length of vector should be equal to numGases, and sum should be
   *  openstudio::equal(1.0). If all fractions except the last is < 1.0, will set last fraction
   *  value if necessary (and issue warning). */
    bool setFractions(const std::vector<double>& fractions);

    /** Assign gas fraction at index to 1.0 - sum(other gas fractions), if index < numGases. */
    bool balanceFractions(unsigned index);

    bool setThickness(double thickness);

    bool setNumberofGasesinMixture(int numberofGasesinMixture);

    bool setGas1Type(const std::string& gas1Type);

    void resetGas1Type();

    bool setGas1Fraction(double gas1Fraction);

    void resetGas1Fraction();

    bool setGas2Type(const std::string& gas2Type);

    void resetGas2Type();

    bool setGas2Fraction(double gas2Fraction);

    void resetGas2Fraction();

    bool setGas3Type(const std::string& gas3Type);

    void resetGas3Type();

    bool setGas3Fraction(double gas3Fraction);

    void resetGas3Fraction();

    bool setGas4Type(const std::string& gas4Type);

    void resetGas4Type();

    bool setGas4Fraction(double gas4Fraction);

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
    /// @cond
    using ImplType = detail::GasMixture_Impl;

    explicit GasMixture(std::shared_ptr<detail::GasMixture_Impl> impl);

    friend class detail::GasMixture_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.GasMixture");
  };

  /** \relates GasMixture*/
  using OptionalGasMixture = boost::optional<GasMixture>;

  /** \relates GasMixture*/
  using GasMixtureVector = std::vector<GasMixture>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GASMIXTURE_HPP
