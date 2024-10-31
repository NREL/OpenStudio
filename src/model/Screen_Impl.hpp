/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SCREEN_IMPL_HPP
#define MODEL_SCREEN_IMPL_HPP

#include "ModelAPI.hpp"
#include "ShadingMaterial_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** Screen_Impl is a ShadingMaterial_Impl that is the implementation class for Screen.*/
    class MODEL_API Screen_Impl : public ShadingMaterial_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      Screen_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      Screen_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      Screen_Impl(const Screen_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~Screen_Impl() override = default;

      //@}

      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      /** Get the thickness of the material. For now is equivalent to screen material diameter.
     *  However, actual thickness of construction should not use this number directly, but should
     *  also factor in the screen to glass distance. */
      virtual double thickness() const override;

      /** The conductivity of the material in W/m*K. */
      double thermalConductivity() const;

      /** The conductance of the material in W/m^2*K. */
      double thermalConductance() const;

      /** The resistivity of the material in m*K/W. */
      double thermalResistivity() const;

      /** The resistance of the material in m^2*K/W. */
      double thermalResistance() const;

      virtual boost::optional<double> getVisibleTransmittance() const override;

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

      /** Set thickness to value (m). For now is equivalent to setting screen material diameter. */
      virtual bool setThickness(double value) override;

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
     private:
      REGISTER_LOGGER("openstudio.model.Screen");

      std::vector<std::string> reflectedBeamTransmittanceAccountingMethodValues() const;
      std::vector<std::string> angleofResolutionforScreenTransmittanceOutputMapValues() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SCREEN_IMPL_HPP
