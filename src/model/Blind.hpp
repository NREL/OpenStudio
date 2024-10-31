/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_BLIND_HPP
#define MODEL_BLIND_HPP

#include "ModelAPI.hpp"
#include "ShadingMaterial.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class Blind_Impl;

  }  // namespace detail

  /** Blind is a ShadingMaterial that wraps the OpenStudio IDD object 'OS:WindowMaterial:Blind'. */
  class MODEL_API Blind : public ShadingMaterial
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit Blind(const Model& model, double slatWidth = 0.025, double slatSeparation = 0.01875, double frontSideSlatBeamSolarReflectance = 0.5,
                   double backSideSlatBeamSolarReflectance = 0.5, double frontSideSlatDiffuseSolarReflectance = 0.5,
                   double backSideSlatDiffuseSolarReflectance = 0.5, double slatBeamVisibleTransmittance = 0.0);

    virtual ~Blind() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    Blind(const Blind& other) = default;
    Blind(Blind&& other) = default;
    Blind& operator=(const Blind&) = default;
    Blind& operator=(Blind&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> slatOrientationValues();

    /** @name Getters */
    //@{

    std::string slatOrientation() const;

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

    // TODO: has a default, should be double, not optional double
    boost::optional<double> frontSideSlatBeamVisibleReflectance() const;

    // TODO: has a default, should be double, not optional double
    boost::optional<double> backSideSlatBeamVisibleReflectance() const;

    double slatDiffuseVisibleTransmittance() const;

    bool isSlatDiffuseVisibleTransmittanceDefaulted() const;

    // TODO: has a default, should be double, not optional double
    boost::optional<double> frontSideSlatDiffuseVisibleReflectance() const;

    // TODO: has a default, should be double, not optional double
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

    bool setFrontSideSlatBeamVisibleReflectance(double frontSideSlatBeamVisibleReflectance);

    void resetFrontSideSlatBeamVisibleReflectance();

    bool setBackSideSlatBeamVisibleReflectance(double backSideSlatBeamVisibleReflectance);

    void resetBackSideSlatBeamVisibleReflectance();

    bool setSlatDiffuseVisibleTransmittance(double slatDiffuseVisibleTransmittance);

    void resetSlatDiffuseVisibleTransmittance();

    bool setFrontSideSlatDiffuseVisibleReflectance(double frontSideSlatDiffuseVisibleReflectance);

    void resetFrontSideSlatDiffuseVisibleReflectance();

    bool setBackSideSlatDiffuseVisibleReflectance(double backSideSlatDiffuseVisibleReflectance);

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
    /// @cond
    using ImplType = detail::Blind_Impl;

    explicit Blind(std::shared_ptr<detail::Blind_Impl> impl);

    friend class detail::Blind_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.Blind");
  };

  /** \relates Blind*/
  using OptionalBlind = boost::optional<Blind>;

  /** \relates Blind*/
  using BlindVector = std::vector<Blind>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_BLIND_HPP
