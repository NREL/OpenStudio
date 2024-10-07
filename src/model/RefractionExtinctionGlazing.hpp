/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_REFRACTIONEXTINCTIONGLAZING_HPP
#define MODEL_REFRACTIONEXTINCTIONGLAZING_HPP

#include "ModelAPI.hpp"
#include "Glazing.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class RefractionExtinctionGlazing_Impl;

  }  // namespace detail

  /** RefractionExtinctionGlazing is a Glazing that wraps the OpenStudio IDD object 'OS:WindowMaterial:Glazing:RefractionExtinctionMethod'. */
  class MODEL_API RefractionExtinctionGlazing : public Glazing
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit RefractionExtinctionGlazing(const Model& model, double thickness = 0.1, double solarIndexofRefraction = 1.1,
                                         double solarExtinctionCoefficient = 0.1, double visibleIndexofRefraction = 1.1,
                                         double visibleExtinctionCoefficient = 0.1);
    //double dirtCorrectionFactorforSolarandVisibleTransmittance = 0.1);

    virtual ~RefractionExtinctionGlazing() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    RefractionExtinctionGlazing(const RefractionExtinctionGlazing& other) = default;
    RefractionExtinctionGlazing(RefractionExtinctionGlazing&& other) = default;
    RefractionExtinctionGlazing& operator=(const RefractionExtinctionGlazing&) = default;
    RefractionExtinctionGlazing& operator=(RefractionExtinctionGlazing&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    //@}
    /** @name Getters */
    //@{

    /** The conductivity of the material in W/m*K. */
    double thermalConductivity() const;

    /** The conductance of the material in W/m^2*K. */
    double thermalConductance() const;

    /** The resistivity of the material in m*K/W. */
    double thermalResistivity() const;

    /** The resistance of the material in m^2*K/W. */
    double thermalResistance() const;

    /** The thermal transmittance of the material (dimensionless fraction). */
    double thermalTransmittance() const;

    double thickness() const;

    double solarIndexofRefraction() const;

    double solarExtinctionCoefficient() const;

    double visibleIndexofRefraction() const;

    double visibleExtinctionCoefficient() const;

    double infraredTransmittanceatNormalIncidence() const;

    bool isInfraredTransmittanceatNormalIncidenceDefaulted() const;

    double infraredHemisphericalEmissivity() const;

    bool isInfraredHemisphericalEmissivityDefaulted() const;

    double conductivity() const;

    bool isConductivityDefaulted() const;

    double dirtCorrectionFactorforSolarandVisibleTransmittance() const;

    bool isDirtCorrectionFactorforSolarandVisibleTransmittanceDefaulted() const;

    bool solarDiffusing() const;

    bool isSolarDiffusingDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    /** Sets the conductivity of the material in W/m*K, if possible. */
    bool setThermalConductivity(double value);

    /** Sets the conductance of the material in W/m^2*K, if possible. Sets thickness to achieve conductivity. */
    bool setThermalConductance(double value);

    /** Sets the resistivity of the material in m*K/W, if possible. */
    bool setThermalResistivity(double value);

    /** Sets the resistance of the material in m^2*K/W, if possible. Sets thickness to achieve resistivity. */
    bool setThermalResistance(double value);

    bool setThermalTransmittance(double value);

    bool setThickness(double thickness);

    bool setSolarIndexofRefraction(double solarIndexofRefraction);

    bool setSolarExtinctionCoefficient(double solarExtinctionCoefficient);

    bool setVisibleIndexofRefraction(double visibleIndexofRefraction);

    bool setVisibleExtinctionCoefficient(double visibleExtinctionCoefficient);

    bool setInfraredTransmittanceatNormalIncidence(double infraredTransmittanceatNormalIncidence);

    void resetInfraredTransmittanceatNormalIncidence();

    bool setInfraredHemisphericalEmissivity(double infraredHemisphericalEmissivity);

    void resetInfraredHemisphericalEmissivity();

    bool setConductivity(double conductivity);

    void resetConductivity();

    bool setDirtCorrectionFactorforSolarandVisibleTransmittance(double dirtCorrectionFactorforSolarandVisibleTransmittance);

    void resetDirtCorrectionFactorforSolarandVisibleTransmittance();

    bool setSolarDiffusing(bool solarDiffusing);

    void setSolarDiffusingNoFail(bool solarDiffusing);

    void resetSolarDiffusing();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::RefractionExtinctionGlazing_Impl;

    explicit RefractionExtinctionGlazing(std::shared_ptr<detail::RefractionExtinctionGlazing_Impl> impl);

    friend class detail::RefractionExtinctionGlazing_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.RefractionExtinctionGlazing");
  };

  /** \relates RefractionExtinctionGlazing*/
  using OptionalRefractionExtinctionGlazing = boost::optional<RefractionExtinctionGlazing>;

  /** \relates RefractionExtinctionGlazing*/
  using RefractionExtinctionGlazingVector = std::vector<RefractionExtinctionGlazing>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_REFRACTIONEXTINCTIONGLAZING_HPP
