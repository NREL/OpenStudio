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

      virtual ~Screen_Impl() = default;

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
