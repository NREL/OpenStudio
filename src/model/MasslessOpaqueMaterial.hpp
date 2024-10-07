/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

    virtual ~MasslessOpaqueMaterial() override = default;
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
