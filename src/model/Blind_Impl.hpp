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

#ifndef MODEL_BLIND_IMPL_HPP
#define MODEL_BLIND_IMPL_HPP

#include "ModelAPI.hpp"
#include "ShadingMaterial_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** Blind_Impl is a ShadingMaterial_Impl that is the implementation class for Blind.*/
    class MODEL_API Blind_Impl : public ShadingMaterial_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      Blind_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      Blind_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      Blind_Impl(const Blind_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~Blind_Impl() = default;

      //@}

      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      /** Get the thickness of the material. For now is equivalent to slat thickness. */
      virtual double thickness() const override;

      std::string slatOrientation() const;
      /** Throws because blind has beam and diffuse visible transmittance. */
      virtual boost::optional<double> getVisibleTransmittance() const override;

      bool isSlatOrientationDefaulted() const;

      double slatWidth() const;

      double slatSeparation() const;

      double slatThickness() const;

      bool isSlatThicknessDefaulted() const;

      double slatAngle() const;

      bool isSlatAngleDefaulted() const;

      double slatConductivity() const;

      bool isSlatConductivityDefaulted() const;

      double slatBeamSolarTransmittance() const;

      bool isSlatBeamSolarTransmittanceDefaulted() const;

      double frontSideSlatBeamSolarReflectance() const;

      double backSideSlatBeamSolarReflectance() const;

      double slatDiffuseSolarTransmittance() const;

      bool isSlatDiffuseSolarTransmittanceDefaulted() const;

      double frontSideSlatDiffuseSolarReflectance() const;

      double backSideSlatDiffuseSolarReflectance() const;

      double slatBeamVisibleTransmittance() const;

      boost::optional<double> frontSideSlatBeamVisibleReflectance() const;

      boost::optional<double> backSideSlatBeamVisibleReflectance() const;

      double slatDiffuseVisibleTransmittance() const;

      bool isSlatDiffuseVisibleTransmittanceDefaulted() const;

      boost::optional<double> frontSideSlatDiffuseVisibleReflectance() const;

      boost::optional<double> backSideSlatDiffuseVisibleReflectance() const;

      double slatInfraredHemisphericalTransmittance() const;

      bool isSlatInfraredHemisphericalTransmittanceDefaulted() const;

      double frontSideSlatInfraredHemisphericalEmissivity() const;

      bool isFrontSideSlatInfraredHemisphericalEmissivityDefaulted() const;

      double backSideSlatInfraredHemisphericalEmissivity() const;

      bool isBackSideSlatInfraredHemisphericalEmissivityDefaulted() const;

      double blindtoGlassDistance() const;

      bool isBlindtoGlassDistanceDefaulted() const;

      double blindTopOpeningMultiplier() const;

      bool isBlindTopOpeningMultiplierDefaulted() const;

      double blindBottomOpeningMultiplier() const;

      bool isBlindBottomOpeningMultiplierDefaulted() const;

      double blindLeftSideOpeningMultiplier() const;

      bool isBlindLeftSideOpeningMultiplierDefaulted() const;

      double blindRightSideOpeningMultiplier() const;

      bool isBlindRightSideOpeningMultiplierDefaulted() const;

      double minimumSlatAngle() const;

      bool isMinimumSlatAngleDefaulted() const;

      double maximumSlatAngle() const;

      bool isMaximumSlatAngleDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      /** Set thickness to value (m). For now is equivalent to setting slat thickness. */
      virtual bool setThickness(double value) override;

      bool setSlatOrientation(const std::string& slatOrientation);

      void resetSlatOrientation();

      bool setSlatWidth(double slatWidth);

      bool setSlatSeparation(double slatSeparation);

      bool setSlatThickness(double slatThickness);

      void resetSlatThickness();

      bool setSlatAngle(double slatAngle);

      void resetSlatAngle();

      bool setSlatConductivity(double slatConductivity);

      void resetSlatConductivity();

      bool setSlatBeamSolarTransmittance(double slatBeamSolarTransmittance);

      void resetSlatBeamSolarTransmittance();

      bool setFrontSideSlatBeamSolarReflectance(double frontSideSlatBeamSolarReflectance);

      bool setBackSideSlatBeamSolarReflectance(double backSideSlatBeamSolarReflectance);

      bool setSlatDiffuseSolarTransmittance(double slatDiffuseSolarTransmittance);

      void resetSlatDiffuseSolarTransmittance();

      bool setFrontSideSlatDiffuseSolarReflectance(double frontSideSlatDiffuseSolarReflectance);

      bool setBackSideSlatDiffuseSolarReflectance(double backSideSlatDiffuseSolarReflectance);

      bool setSlatBeamVisibleTransmittance(double slatBeamVisibleTransmittance);

      bool setFrontSideSlatBeamVisibleReflectance(boost::optional<double> frontSideSlatBeamVisibleReflectance);

      void resetFrontSideSlatBeamVisibleReflectance();

      bool setBackSideSlatBeamVisibleReflectance(boost::optional<double> backSideSlatBeamVisibleReflectance);

      void resetBackSideSlatBeamVisibleReflectance();

      bool setSlatDiffuseVisibleTransmittance(double slatDiffuseVisibleTransmittance);

      void resetSlatDiffuseVisibleTransmittance();

      bool setFrontSideSlatDiffuseVisibleReflectance(boost::optional<double> frontSideSlatDiffuseVisibleReflectance);

      void resetFrontSideSlatDiffuseVisibleReflectance();

      bool setBackSideSlatDiffuseVisibleReflectance(boost::optional<double> backSideSlatDiffuseVisibleReflectance);

      void resetBackSideSlatDiffuseVisibleReflectance();

      bool setSlatInfraredHemisphericalTransmittance(double slatInfraredHemisphericalTransmittance);

      void resetSlatInfraredHemisphericalTransmittance();

      bool setFrontSideSlatInfraredHemisphericalEmissivity(double frontSideSlatInfraredHemisphericalEmissivity);

      void resetFrontSideSlatInfraredHemisphericalEmissivity();

      bool setBackSideSlatInfraredHemisphericalEmissivity(double backSideSlatInfraredHemisphericalEmissivity);

      void resetBackSideSlatInfraredHemisphericalEmissivity();

      bool setBlindtoGlassDistance(double blindtoGlassDistance);

      void resetBlindtoGlassDistance();

      bool setBlindTopOpeningMultiplier(double blindTopOpeningMultiplier);

      void resetBlindTopOpeningMultiplier();

      bool setBlindBottomOpeningMultiplier(double blindBottomOpeningMultiplier);

      void resetBlindBottomOpeningMultiplier();

      bool setBlindLeftSideOpeningMultiplier(double blindLeftSideOpeningMultiplier);

      void resetBlindLeftSideOpeningMultiplier();

      bool setBlindRightSideOpeningMultiplier(double blindRightSideOpeningMultiplier);

      void resetBlindRightSideOpeningMultiplier();

      bool setMinimumSlatAngle(double minimumSlatAngle);

      void resetMinimumSlatAngle();

      bool setMaximumSlatAngle(double maximumSlatAngle);

      void resetMaximumSlatAngle();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.Blind");

      std::vector<std::string> slatOrientationValues() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_BLIND_IMPL_HPP
