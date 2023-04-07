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

    virtual ~Shade() = default;
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
