/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SCREEN_HPP
#define MODEL_SCREEN_HPP

#include "ModelAPI.hpp"
#include "ShadingMaterial.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class Screen_Impl;

  }  // namespace detail

  /** Screen is a ShadingMaterial that wraps the OpenStudio IDD object 'OS:WindowMaterial:Screen'. */
  class MODEL_API Screen : public ShadingMaterial
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit Screen(const Model& model, double diffuseSolarReflectance = 0.08, double diffuseVisibleReflectance = 0.08,
                    double screenMaterialSpacing = 0.00157, double screenMaterialDiameter = 0.000381);

    virtual ~Screen() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    Screen(const Screen& other) = default;
    Screen(Screen&& other) = default;
    Screen& operator=(const Screen&) = default;
    Screen& operator=(Screen&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> reflectedBeamTransmittanceAccountingMethodValues();

    static std::vector<std::string> angleofResolutionforScreenTransmittanceOutputMapValues();

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

    std::string reflectedBeamTransmittanceAccountingMethod() const;

    bool isReflectedBeamTransmittanceAccountingMethodDefaulted() const;

    double diffuseSolarReflectance() const;

    double diffuseVisibleReflectance() const;

    double thermalHemisphericalEmissivity() const;

    bool isThermalHemisphericalEmissivityDefaulted() const;

    double conductivity() const;

    bool isConductivityDefaulted() const;

    double screenMaterialSpacing() const;

    double screenMaterialDiameter() const;

    double screentoGlassDistance() const;

    bool isScreentoGlassDistanceDefaulted() const;

    double topOpeningMultiplier() const;

    bool isTopOpeningMultiplierDefaulted() const;

    double bottomOpeningMultiplier() const;

    bool isBottomOpeningMultiplierDefaulted() const;

    double leftSideOpeningMultiplier() const;

    bool isLeftSideOpeningMultiplierDefaulted() const;

    double rightSideOpeningMultiplier() const;

    bool isRightSideOpeningMultiplierDefaulted() const;

    std::string angleofResolutionforScreenTransmittanceOutputMap() const;

    bool isAngleofResolutionforScreenTransmittanceOutputMapDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    /** Sets the conductivity of the material in W/m*K, if possible. */
    bool setThermalConductivity(double value);

    /** Sets the resistivity of the material in m*K/W, if possible. */
    bool setThermalResistivity(double value);

    bool setReflectedBeamTransmittanceAccountingMethod(const std::string& reflectedBeamTransmittanceAccountingMethod);

    void resetReflectedBeamTransmittanceAccountingMethod();

    bool setDiffuseSolarReflectance(double diffuseSolarReflectance);

    bool setDiffuseVisibleReflectance(double diffuseVisibleReflectance);

    bool setThermalHemisphericalEmissivity(double thermalHemisphericalEmissivity);

    void resetThermalHemisphericalEmissivity();

    bool setConductivity(double conductivity);

    void resetConductivity();

    bool setScreenMaterialSpacing(double screenMaterialSpacing);

    bool setScreenMaterialDiameter(double screenMaterialDiameter);

    bool setScreentoGlassDistance(double screentoGlassDistance);

    void resetScreentoGlassDistance();

    bool setTopOpeningMultiplier(double topOpeningMultiplier);

    void resetTopOpeningMultiplier();

    bool setBottomOpeningMultiplier(double bottomOpeningMultiplier);

    void resetBottomOpeningMultiplier();

    bool setLeftSideOpeningMultiplier(double leftSideOpeningMultiplier);

    void resetLeftSideOpeningMultiplier();

    bool setRightSideOpeningMultiplier(double rightSideOpeningMultiplier);

    void resetRightSideOpeningMultiplier();

    bool setAngleofResolutionforScreenTransmittanceOutputMap(const std::string& angleofResolutionforScreenTransmittanceOutputMap);

    void resetAngleofResolutionforScreenTransmittanceOutputMap();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::Screen_Impl;

    explicit Screen(std::shared_ptr<detail::Screen_Impl> impl);

    friend class detail::Screen_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.Screen");
  };

  /** \relates Screen*/
  using OptionalScreen = boost::optional<Screen>;

  /** \relates Screen*/
  using ScreenVector = std::vector<Screen>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SCREEN_HPP
