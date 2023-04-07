/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

    virtual ~FenestrationMaterial() = default;
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
