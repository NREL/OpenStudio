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

    virtual ~Blind() = default;
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
