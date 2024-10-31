/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

      virtual ~Blind_Impl() override = default;

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
