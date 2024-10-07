/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

    virtual ~StandardOpaqueMaterial() override = default;
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
