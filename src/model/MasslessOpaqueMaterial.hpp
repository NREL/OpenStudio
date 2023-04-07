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

#ifndef MODEL_MASSLESSOPAQUEMATERIAL_HPP
#define MODEL_MASSLESSOPAQUEMATERIAL_HPP

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

    class MasslessOpaqueMaterial_Impl;

  }  // namespace detail

  /** MasslessOpaqueMaterial is a OpaqueMaterial that wraps the OpenStudio IDD object 'OS:Material:NoMass'. */
  class MODEL_API MasslessOpaqueMaterial : public OpaqueMaterial
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit MasslessOpaqueMaterial(const Model& model, const std::string& roughness = "Smooth", double thermalResistance = 0.1);

    virtual ~MasslessOpaqueMaterial() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    MasslessOpaqueMaterial(const MasslessOpaqueMaterial& other) = default;
    MasslessOpaqueMaterial(MasslessOpaqueMaterial&& other) = default;
    MasslessOpaqueMaterial& operator=(const MasslessOpaqueMaterial&) = default;
    MasslessOpaqueMaterial& operator=(MasslessOpaqueMaterial&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> roughnessValues();

    /** @name Getters */
    //@{

    std::string roughness() const;

    double thermalResistance() const;

    boost::optional<double> thermalAbsorptance() const;

    bool isThermalAbsorptanceDefaulted() const;

    boost::optional<double> solarAbsorptance() const;

    bool isSolarAbsorptanceDefaulted() const;

    boost::optional<double> visibleAbsorptance() const;

    bool isVisibleAbsorptanceDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setRoughness(const std::string& roughness);

    bool setThermalResistance(double thermalResistance);

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
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::MasslessOpaqueMaterial_Impl;

    explicit MasslessOpaqueMaterial(std::shared_ptr<detail::MasslessOpaqueMaterial_Impl> impl);

    friend class detail::MasslessOpaqueMaterial_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.MasslessOpaqueMaterial");
  };

  /** \relates MasslessOpaqueMaterial*/
  using OptionalMasslessOpaqueMaterial = boost::optional<MasslessOpaqueMaterial>;

  /** \relates MasslessOpaqueMaterial*/
  using MasslessOpaqueMaterialVector = std::vector<MasslessOpaqueMaterial>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_MASSLESSOPAQUEMATERIAL_HPP
