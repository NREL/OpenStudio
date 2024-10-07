/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_FENESTRATIONMATERIAL_HPP
#define MODEL_FENESTRATIONMATERIAL_HPP

#include "ModelAPI.hpp"
#include "Material.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class FenestrationMaterial_Impl;
  }

  /** FenestrationMaterial is a Material that serves as a base class for materials that can be
 *  used in fenestration constructions. FenestrationMaterial also serves as a repository for
 *  a number of pieces of fenestration-related data used by EnergyPlus. */
  class MODEL_API FenestrationMaterial : public Material
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~FenestrationMaterial() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    FenestrationMaterial(const FenestrationMaterial& other) = default;
    FenestrationMaterial(FenestrationMaterial&& other) = default;
    FenestrationMaterial& operator=(const FenestrationMaterial&) = default;
    FenestrationMaterial& operator=(FenestrationMaterial&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    // ETH@20110615 Remove const & return values.

    /** Returns the inside air temperature used by EnergyPlus for calculating fenestration
   *  u-factors (K). */
    static double ufactorInsideAirTemperature();

    /** Returns the outside air temperature used by EnergyPlus for calculating fenestration
   *  u-factors (K). */
    static double ufactorOutsideAirTemperature();

    /** Returns the conductivity coefficients for air. */
    static const std::vector<double> airThermalConductivityCoefficients();

    /** Returns the viscosity coefficients for air. */
    static const std::vector<double> airViscosityCoefficients();

    /** Returns the specific heat coefficients for air. */
    static const std::vector<double> airSpecificHeatCoefficients();

    /** The molecular weight of air (u). */
    static double airMolecularWeight();

    /** Returns the conductivity coefficients for argon. */
    static const std::vector<double> argonThermalConductivityCoefficients();

    /** Returns the viscosity coefficients for argon. */
    static const std::vector<double> argonViscosityCoefficients();

    /** Returns the specific heat coefficients for argon. */
    static const std::vector<double> argonSpecificHeatCoefficients();

    /** The molecular weight of argon (u). */
    static double argonMolecularWeight();

    /** Returns the conductivity coefficients for krypton. */
    static const std::vector<double> kryptonThermalConductivityCoefficients();

    /** Returns the viscosity coefficients for krypton. */
    static const std::vector<double> kryptonViscosityCoefficients();

    /** Returns the specific heat coefficients for krypton. */
    static const std::vector<double> kryptonSpecificHeatCoefficients();

    /** The molecular weight of krypton (u). */
    static double kryptonMolecularWeight();

    /** Returns the conductivity coefficients for xenon. */
    static const std::vector<double> xenonThermalConductivityCoefficients();

    /** Returns the viscosity coefficients for xenon. */
    static const std::vector<double> xenonViscosityCoefficients();

    /** Returns the specific heat coefficients for xenon. */
    static const std::vector<double> xenonSpecificHeatCoefficients();

    /** The molecular weight of xenon (u). */
    static double xenonMolecularWeight();

    //@}
   protected:
    /// @cond
    using ImplType = detail::FenestrationMaterial_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class detail::FenestrationMaterial_Impl;

    FenestrationMaterial(IddObjectType type, const Model& model);

    explicit FenestrationMaterial(std::shared_ptr<detail::FenestrationMaterial_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.FenestrationMaterial");
  };

  /** \relates FenestrationMaterial */
  using OptionalFenestrationMaterial = boost::optional<FenestrationMaterial>;

  /** \relates FenestrationMaterial */
  using FenestrationMaterialVector = std::vector<FenestrationMaterial>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_FENESTRATIONMATERIAL_HPP
