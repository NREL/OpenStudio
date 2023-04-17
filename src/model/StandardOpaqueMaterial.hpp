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

#ifndef MODEL_STANDARDOPAQUEMATERIAL_HPP
#define MODEL_STANDARDOPAQUEMATERIAL_HPP

#include "ModelAPI.hpp"
#include "OpaqueMaterial.hpp"

#include "MaterialPropertyMoisturePenetrationDepthSettings.hpp"
#include "MaterialPropertyMoisturePenetrationDepthSettings_Impl.hpp"
#include "MaterialPropertyPhaseChange.hpp"
#include "MaterialPropertyPhaseChange_Impl.hpp"
#include "MaterialPropertyPhaseChangeHysteresis.hpp"
#include "MaterialPropertyPhaseChangeHysteresis_Impl.hpp"

namespace openstudio {

namespace model {

  class MaterialPropertyMoisturePenetrationDepthSettings;
  class MaterialPropertyPhaseChange;
  class MaterialPropertyPhaseChangeHysteresis;

  namespace detail {

    class StandardOpaqueMaterial_Impl;

  }  // namespace detail

  /** StandardOpaqueMaterial is a OpaqueMaterial that wraps the OpenStudio IDD object 'OS:OpaqueMaterial'. */
  class MODEL_API StandardOpaqueMaterial : public OpaqueMaterial
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit StandardOpaqueMaterial(const Model& model, const std::string& roughness = "Smooth", double thickness = 0.1, double conductivity = 0.1,
                                    double density = 0.1, double specificHeat = 1400);

    virtual ~StandardOpaqueMaterial() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    StandardOpaqueMaterial(const StandardOpaqueMaterial& other) = default;
    StandardOpaqueMaterial(StandardOpaqueMaterial&& other) = default;
    StandardOpaqueMaterial& operator=(const StandardOpaqueMaterial&) = default;
    StandardOpaqueMaterial& operator=(StandardOpaqueMaterial&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> roughnessValues();

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

    // Thermal reflectance of the OpaqueMaterial (dimensionless fraction).
    boost::optional<double> thermalReflectance() const;

    // Solar reflectance of the OpaqueMaterial (dimensionless fraction).
    boost::optional<double> solarReflectance() const;

    // Visible reflectance of the OpaqueMaterial (dimensionless fraction).
    boost::optional<double> visibleReflectance() const;

    std::string roughness() const;

    double thickness() const;

    double conductivity() const;

    double density() const;

    double specificHeat() const;

    double thermalAbsorptance() const;

    bool isThermalAbsorptanceDefaulted() const;

    double solarAbsorptance() const;

    bool isSolarAbsorptanceDefaulted() const;

    double visibleAbsorptance() const;

    bool isVisibleAbsorptanceDefaulted() const;

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
    bool setThermalAbsorptance(boost::optional<double> value);

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

    bool setRoughness(const std::string& roughness);

    bool setThickness(double thickness);

    bool setConductivity(double conductivity);

    bool setDensity(double density);

    bool setSpecificHeat(double specificHeat);

    bool setThermalAbsorptance(double thermalAbsorptance);

    void resetThermalAbsorptance();

    bool setSolarAbsorptance(double solarAbsorptance);

    void resetSolarAbsorptance();

    bool setVisibleAbsorptance(double visibleAbsorptance);

    void resetVisibleAbsorptance();

    // if material property moisture penetration depth settings already exists, do nothing and return nil; creates the material property moisture penetration depth settings if it does not already exist and return it
    boost::optional<MaterialPropertyMoisturePenetrationDepthSettings>
      createMaterialPropertyMoisturePenetrationDepthSettings(double waterVaporDiffusionResistanceFactor, double moistureEquationCoefficientA,
                                                             double moistureEquationCoefficientB, double moistureEquationCoefficientC,
                                                             double moistureEquationCoefficientD, double coatingLayerThickness,
                                                             double coatingLayerWaterVaporDiffusionResistanceFactor);

    // returns the material property moisture penetration depth settings if set
    boost::optional<MaterialPropertyMoisturePenetrationDepthSettings> materialPropertyMoisturePenetrationDepthSettings() const;

    // resets the material property moisture penetration depth settings
    void resetMaterialPropertyMoisturePenetrationDepthSettings();

    // if material property phase change already exists, do nothing and return nil; creates the material property phase change if it does not already exist and return it
    boost::optional<MaterialPropertyPhaseChange> createMaterialPropertyPhaseChange();
    boost::optional<MaterialPropertyPhaseChange> createMaterialPropertyPhaseChange(const std::vector<TemperatureEnthalpy>& temperatureEnthalpys);

    // returns the material property phase change if set
    boost::optional<MaterialPropertyPhaseChange> materialPropertyPhaseChange() const;

    // resets the material property phase change
    void resetMaterialPropertyPhaseChange();

    // if material property phase change hysteresis already exists, do nothing and return nil; creates the material property phase change hysteresis if it does not already exist and return it
    boost::optional<MaterialPropertyPhaseChangeHysteresis> createMaterialPropertyPhaseChangeHysteresis();

    // returns the material property phase change hysteresis if set
    boost::optional<MaterialPropertyPhaseChangeHysteresis> materialPropertyPhaseChangeHysteresis() const;

    // resets the material property phase change hysteresis
    void resetMaterialPropertyPhaseChangeHysteresis();

    //@}
    /** @name Queries */
    //@{

    /** Energy it takes to raise the temperature of this material 1 K (J/m^2*K). Equal to
     *  specificHeat() * density() * thickness(). */
    boost::optional<double> heatCapacity() const;

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::StandardOpaqueMaterial_Impl;

    explicit StandardOpaqueMaterial(std::shared_ptr<detail::StandardOpaqueMaterial_Impl> impl);

    friend class detail::StandardOpaqueMaterial_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.StandardOpaqueMaterial");
  };

  /** \relates StandardOpaqueMaterial*/
  using OptionalStandardOpaqueMaterial = boost::optional<StandardOpaqueMaterial>;

  /** \relates StandardOpaqueMaterial*/
  using StadardOpaqueMaterialVector = std::vector<StandardOpaqueMaterial>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_STANDARDOPAQUEMATERIAL_HPP
