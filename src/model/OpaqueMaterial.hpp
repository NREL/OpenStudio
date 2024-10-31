/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_OPAQUEMATERIAL_HPP
#define MODEL_OPAQUEMATERIAL_HPP

#include "ModelAPI.hpp"
#include "Material.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class OpaqueMaterial_Impl;
  }

  /** OpaqueMaterial is a Material that serves as a base class for all \link LayeredConstruction
 *  LayeredConstructions \endlink for which isOpaque. OpaqueMaterial also defines a number of
 *  virtually implemented attributes. */
  class MODEL_API OpaqueMaterial : public Material
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~OpaqueMaterial() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    OpaqueMaterial(const OpaqueMaterial& other) = default;
    OpaqueMaterial(OpaqueMaterial&& other) = default;
    OpaqueMaterial& operator=(const OpaqueMaterial&) = default;
    OpaqueMaterial& operator=(OpaqueMaterial&&) = default;

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

    // Thermal absorptance of the Material (dimensionless fraction).
    double thermalAbsorptance() const;

    // Thermal reflectance of the Material (dimensionless fraction).
    boost::optional<double> thermalReflectance() const;

    // Solar absorptance of the Material (dimensionless fraction).
    double solarAbsorptance() const;

    // Solar reflectance of the Material (dimensionless fraction).
    boost::optional<double> solarReflectance() const;

    // Visible absorptance of the Material (dimensionless fraction).
    double visibleAbsorptance() const;

    // Visible reflectance of the Material (dimensionless fraction).
    boost::optional<double> visibleReflectance() const;

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

    // Set the thermal absorptance (dimensionless fraction).
    bool setThermalAbsorptance(double value);

    // Set the thermal reflectance (dimensionless fraction).
    bool setThermalReflectance(boost::optional<double> value);

    // Set the solar absorptance (dimensionless fraction).
    bool setSolarAbsorptance(boost::optional<double> value);

    // Set the solar reflectance (dimensionless fraction).
    bool setSolarReflectance(boost::optional<double> value);

    // Set the visible absorptance (dimensionless fraction).
    bool setVisibleAbsorptance(boost::optional<double> value);

    // Set the visible reflectance (dimensionless fraction).
    bool setVisibleReflectance(boost::optional<double> value);

    //@}
    /** @name Queries */
    //@{

    /** Energy it takes to raise the temperature of this material 1 K (J/m^2*K). Equal to
    *  specificHeat() * density() * thickness(). */
    boost::optional<double> heatCapacity() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::OpaqueMaterial_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class detail::OpaqueMaterial_Impl;

    OpaqueMaterial(IddObjectType type, const Model& model);

    explicit OpaqueMaterial(std::shared_ptr<detail::OpaqueMaterial_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.OpaqueMaterial");
  };

  /** \relates OpaqueMaterial */
  using OptionalOpaqueMaterial = boost::optional<OpaqueMaterial>;

  /** \relates OpaqueMaterial */
  using OpaqueMaterialVector = std::vector<OpaqueMaterial>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_OPAQUEMATERIAL_HPP
