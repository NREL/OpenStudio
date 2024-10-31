/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SHADE_HPP
#define MODEL_SHADE_HPP

#include "ModelAPI.hpp"
#include "ShadingMaterial.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class Shade_Impl;

  }  // namespace detail

  /** Shade is a ShadingMaterial that wraps the OpenStudio IDD object 'OS:WindowMaterial:Shade'. */
  class MODEL_API Shade : public ShadingMaterial
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit Shade(const Model& model, double solarTransmittance = 0.4, double solarReflectance = 0.5, double visibleTransmittance = 0.4,
                   double visibleReflectance = 0.5, double thermalHemisphericalEmissivity = 0.9, double thermalTransmittance = 0.0,
                   double thickness = 0.005, double conductivity = 0.1);

    virtual ~Shade() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    Shade(const Shade& other) = default;
    Shade(Shade&& other) = default;
    Shade& operator=(const Shade&) = default;
    Shade& operator=(Shade&&) = default;

    //@}

    static IddObjectType iddObjectType();

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

    double solarTransmittance() const;

    double solarReflectance() const;

    double visibleTransmittance() const;

    double visibleReflectance() const;

    double thermalHemisphericalEmissivity() const;

    double thermalTransmittance() const;

    double solarAbsorptance() const;

    double visibleAbsorptance() const;

    double thickness() const;

    double conductivity() const;

    double shadetoGlassDistance() const;

    bool isShadetoGlassDistanceDefaulted() const;

    double topOpeningMultiplier() const;

    bool isTopOpeningMultiplierDefaulted() const;

    double bottomOpeningMultiplier() const;

    bool isBottomOpeningMultiplierDefaulted() const;

    double leftSideOpeningMultiplier() const;

    bool isLeftSideOpeningMultiplierDefaulted() const;

    double rightSideOpeningMultiplier() const;

    bool isRightSideOpeningMultiplierDefaulted() const;

    double airflowPermeability() const;

    bool isAirflowPermeabilityDefaulted() const;

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

    bool setThermalTransmittance(double thermalTransmittance);

    bool setSolarTransmittance(double solarTransmittance);

    bool setSolarReflectance(double solarReflectance);

    bool setVisibleTransmittance(double visibleTransmittance);

    bool setVisibleReflectance(double visibleReflectance);

    bool setThermalHemisphericalEmissivity(double thermalHemisphericalEmissivity);

    bool setThickness(double thickness);

    bool setConductivity(double conductivity);

    bool setShadetoGlassDistance(double shadetoGlassDistance);

    void resetShadetoGlassDistance();

    bool setTopOpeningMultiplier(double topOpeningMultiplier);

    void resetTopOpeningMultiplier();

    bool setBottomOpeningMultiplier(double bottomOpeningMultiplier);

    void resetBottomOpeningMultiplier();

    bool setLeftSideOpeningMultiplier(double leftSideOpeningMultiplier);

    void resetLeftSideOpeningMultiplier();

    bool setRightSideOpeningMultiplier(double rightSideOpeningMultiplier);

    void resetRightSideOpeningMultiplier();

    bool setAirflowPermeability(double airflowPermeability);

    void resetAirflowPermeability();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::Shade_Impl;

    explicit Shade(std::shared_ptr<detail::Shade_Impl> impl);

    friend class detail::Shade_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.Shade");
  };

  /** \relates Shade*/
  using OptionalShade = boost::optional<Shade>;

  /** \relates Shade*/
  using ShadeVector = std::vector<Shade>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SHADE_HPP
