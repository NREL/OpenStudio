/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#ifndef MODEL_BLIND_HPP
#define MODEL_BLIND_HPP

#include "ModelAPI.hpp"
#include "ShadingMaterial.hpp"

namespace openstudio {

class Quantity;
class OSOptionalQuantity;

namespace model {

namespace detail {

  class Blind_Impl;

} // detail

/** Blind is a ShadingMaterial that wraps the OpenStudio IDD object 'OS:WindowMaterial:Blind'. */
class MODEL_API Blind : public ShadingMaterial {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit Blind(const Model& model,
                 double slatWidth = 0.025,
                 double slatSeparation = 0.01875,
                 double frontSideSlatBeamSolarReflectance = 0.5,
                 double backSideSlatBeamSolarReflectance = 0.5,
                 double frontSideSlatDiffuseSolarReflectance = 0.5,
                 double backSideSlatDiffuseSolarReflectance = 0.5,
                 double slatBeamVisibleTransmittance = 0.0);

  virtual ~Blind() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> slatOrientationValues();

  /** @name Getters */
  //@{

  std::string slatOrientation() const;

  bool isSlatOrientationDefaulted() const;

  double slatWidth() const;

  Quantity getSlatWidth(bool returnIP=false) const;

  double slatSeparation() const;

  Quantity getSlatSeparation(bool returnIP=false) const;

  double slatThickness() const;

  Quantity getSlatThickness(bool returnIP=false) const;

  bool isSlatThicknessDefaulted() const;

  double slatAngle() const;

  Quantity getSlatAngle(bool returnIP=false) const;

  bool isSlatAngleDefaulted() const;

  double slatConductivity() const;

  Quantity getSlatConductivity(bool returnIP=false) const;

  bool isSlatConductivityDefaulted() const;

  double slatBeamSolarTransmittance() const;

  Quantity getSlatBeamSolarTransmittance(bool returnIP=false) const;

  bool isSlatBeamSolarTransmittanceDefaulted() const;

  double frontSideSlatBeamSolarReflectance() const;

  Quantity getFrontSideSlatBeamSolarReflectance(bool returnIP=false) const;

  double backSideSlatBeamSolarReflectance() const;

  Quantity getBackSideSlatBeamSolarReflectance(bool returnIP=false) const;

  double slatDiffuseSolarTransmittance() const;

  Quantity getSlatDiffuseSolarTransmittance(bool returnIP=false) const;

  bool isSlatDiffuseSolarTransmittanceDefaulted() const;

  double frontSideSlatDiffuseSolarReflectance() const;

  Quantity getFrontSideSlatDiffuseSolarReflectance(bool returnIP=false) const;

  double backSideSlatDiffuseSolarReflectance() const;

  Quantity getBackSideSlatDiffuseSolarReflectance(bool returnIP=false) const;

  double slatBeamVisibleTransmittance() const;

  Quantity getSlatBeamVisibleTransmittance(bool returnIP=false) const;

  boost::optional<double> frontSideSlatBeamVisibleReflectance() const;

  OSOptionalQuantity getFrontSideSlatBeamVisibleReflectance(bool returnIP=false) const;

  boost::optional<double> backSideSlatBeamVisibleReflectance() const;

  OSOptionalQuantity getBackSideSlatBeamVisibleReflectance(bool returnIP=false) const;

  double slatDiffuseVisibleTransmittance() const;

  Quantity getSlatDiffuseVisibleTransmittance(bool returnIP=false) const;

  bool isSlatDiffuseVisibleTransmittanceDefaulted() const;

  boost::optional<double> frontSideSlatDiffuseVisibleReflectance() const;

  OSOptionalQuantity getFrontSideSlatDiffuseVisibleReflectance(bool returnIP=false) const;

  boost::optional<double> backSideSlatDiffuseVisibleReflectance() const;

  OSOptionalQuantity getBackSideSlatDiffuseVisibleReflectance(bool returnIP=false) const;

  double slatInfraredHemisphericalTransmittance() const;

  Quantity getSlatInfraredHemisphericalTransmittance(bool returnIP=false) const;

  bool isSlatInfraredHemisphericalTransmittanceDefaulted() const;

  double frontSideSlatInfraredHemisphericalEmissivity() const;

  Quantity getFrontSideSlatInfraredHemisphericalEmissivity(bool returnIP=false) const;

  bool isFrontSideSlatInfraredHemisphericalEmissivityDefaulted() const;

  double backSideSlatInfraredHemisphericalEmissivity() const;

  Quantity getBackSideSlatInfraredHemisphericalEmissivity(bool returnIP=false) const;

  bool isBackSideSlatInfraredHemisphericalEmissivityDefaulted() const;

  double blindtoGlassDistance() const;

  Quantity getBlindtoGlassDistance(bool returnIP=false) const;

  bool isBlindtoGlassDistanceDefaulted() const;

  double blindTopOpeningMultiplier() const;

  Quantity getBlindTopOpeningMultiplier(bool returnIP=false) const;

  bool isBlindTopOpeningMultiplierDefaulted() const;

  double blindBottomOpeningMultiplier() const;

  Quantity getBlindBottomOpeningMultiplier(bool returnIP=false) const;

  bool isBlindBottomOpeningMultiplierDefaulted() const;

  double blindLeftSideOpeningMultiplier() const;

  Quantity getBlindLeftSideOpeningMultiplier(bool returnIP=false) const;

  bool isBlindLeftSideOpeningMultiplierDefaulted() const;

  double blindRightSideOpeningMultiplier() const;

  Quantity getBlindRightSideOpeningMultiplier(bool returnIP=false) const;

  bool isBlindRightSideOpeningMultiplierDefaulted() const;

  double minimumSlatAngle() const;

  Quantity getMinimumSlatAngle(bool returnIP=false) const;

  bool isMinimumSlatAngleDefaulted() const;

  double maximumSlatAngle() const;

  Quantity getMaximumSlatAngle(bool returnIP=false) const;

  bool isMaximumSlatAngleDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  bool setSlatOrientation(std::string slatOrientation);

  void resetSlatOrientation();

  bool setSlatWidth(double slatWidth);

  bool setSlatWidth(const Quantity& slatWidth);

  bool setSlatSeparation(double slatSeparation);

  bool setSlatSeparation(const Quantity& slatSeparation);

  bool setSlatThickness(double slatThickness);

  bool setSlatThickness(const Quantity& slatThickness);

  void resetSlatThickness();

  bool setSlatAngle(double slatAngle);

  bool setSlatAngle(const Quantity& slatAngle);

  void resetSlatAngle();

  bool setSlatConductivity(double slatConductivity);

  bool setSlatConductivity(const Quantity& slatConductivity);

  void resetSlatConductivity();

  bool setSlatBeamSolarTransmittance(double slatBeamSolarTransmittance);

  bool setSlatBeamSolarTransmittance(const Quantity& slatBeamSolarTransmittance);

  void resetSlatBeamSolarTransmittance();

  bool setFrontSideSlatBeamSolarReflectance(double frontSideSlatBeamSolarReflectance);

  bool setFrontSideSlatBeamSolarReflectance(const Quantity& frontSideSlatBeamSolarReflectance);

  bool setBackSideSlatBeamSolarReflectance(double backSideSlatBeamSolarReflectance);

  bool setBackSideSlatBeamSolarReflectance(const Quantity& backSideSlatBeamSolarReflectance);

  bool setSlatDiffuseSolarTransmittance(double slatDiffuseSolarTransmittance);

  bool setSlatDiffuseSolarTransmittance(const Quantity& slatDiffuseSolarTransmittance);

  void resetSlatDiffuseSolarTransmittance();

  bool setFrontSideSlatDiffuseSolarReflectance(double frontSideSlatDiffuseSolarReflectance);

  bool setFrontSideSlatDiffuseSolarReflectance(const Quantity& frontSideSlatDiffuseSolarReflectance);

  bool setBackSideSlatDiffuseSolarReflectance(double backSideSlatDiffuseSolarReflectance);

  bool setBackSideSlatDiffuseSolarReflectance(const Quantity& backSideSlatDiffuseSolarReflectance);

  bool setSlatBeamVisibleTransmittance(double slatBeamVisibleTransmittance);

  bool setSlatBeamVisibleTransmittance(const Quantity& slatBeamVisibleTransmittance);

  bool setFrontSideSlatBeamVisibleReflectance(double frontSideSlatBeamVisibleReflectance);

  bool setFrontSideSlatBeamVisibleReflectance(const Quantity& frontSideSlatBeamVisibleReflectance);

  void resetFrontSideSlatBeamVisibleReflectance();

  bool setBackSideSlatBeamVisibleReflectance(double backSideSlatBeamVisibleReflectance);

  bool setBackSideSlatBeamVisibleReflectance(const Quantity& backSideSlatBeamVisibleReflectance);

  void resetBackSideSlatBeamVisibleReflectance();

  bool setSlatDiffuseVisibleTransmittance(double slatDiffuseVisibleTransmittance);

  bool setSlatDiffuseVisibleTransmittance(const Quantity& slatDiffuseVisibleTransmittance);

  void resetSlatDiffuseVisibleTransmittance();

  bool setFrontSideSlatDiffuseVisibleReflectance(double frontSideSlatDiffuseVisibleReflectance);

  bool setFrontSideSlatDiffuseVisibleReflectance(const Quantity& frontSideSlatDiffuseVisibleReflectance);

  void resetFrontSideSlatDiffuseVisibleReflectance();

  bool setBackSideSlatDiffuseVisibleReflectance(double backSideSlatDiffuseVisibleReflectance);

  bool setBackSideSlatDiffuseVisibleReflectance(const Quantity& backSideSlatDiffuseVisibleReflectance);

  void resetBackSideSlatDiffuseVisibleReflectance();

  bool setSlatInfraredHemisphericalTransmittance(double slatInfraredHemisphericalTransmittance);

  bool setSlatInfraredHemisphericalTransmittance(const Quantity& slatInfraredHemisphericalTransmittance);

  void resetSlatInfraredHemisphericalTransmittance();

  bool setFrontSideSlatInfraredHemisphericalEmissivity(double frontSideSlatInfraredHemisphericalEmissivity);

  bool setFrontSideSlatInfraredHemisphericalEmissivity(const Quantity& frontSideSlatInfraredHemisphericalEmissivity);

  void resetFrontSideSlatInfraredHemisphericalEmissivity();

  bool setBackSideSlatInfraredHemisphericalEmissivity(double backSideSlatInfraredHemisphericalEmissivity);

  bool setBackSideSlatInfraredHemisphericalEmissivity(const Quantity& backSideSlatInfraredHemisphericalEmissivity);

  void resetBackSideSlatInfraredHemisphericalEmissivity();

  bool setBlindtoGlassDistance(double blindtoGlassDistance);

  bool setBlindtoGlassDistance(const Quantity& blindtoGlassDistance);

  void resetBlindtoGlassDistance();

  bool setBlindTopOpeningMultiplier(double blindTopOpeningMultiplier);

  bool setBlindTopOpeningMultiplier(const Quantity& blindTopOpeningMultiplier);

  void resetBlindTopOpeningMultiplier();

  bool setBlindBottomOpeningMultiplier(double blindBottomOpeningMultiplier);

  bool setBlindBottomOpeningMultiplier(const Quantity& blindBottomOpeningMultiplier);

  void resetBlindBottomOpeningMultiplier();

  bool setBlindLeftSideOpeningMultiplier(double blindLeftSideOpeningMultiplier);

  bool setBlindLeftSideOpeningMultiplier(const Quantity& blindLeftSideOpeningMultiplier);

  void resetBlindLeftSideOpeningMultiplier();

  bool setBlindRightSideOpeningMultiplier(double blindRightSideOpeningMultiplier);

  bool setBlindRightSideOpeningMultiplier(const Quantity& blindRightSideOpeningMultiplier);

  void resetBlindRightSideOpeningMultiplier();

  bool setMinimumSlatAngle(double minimumSlatAngle);

  bool setMinimumSlatAngle(const Quantity& minimumSlatAngle);

  void resetMinimumSlatAngle();

  bool setMaximumSlatAngle(double maximumSlatAngle);

  bool setMaximumSlatAngle(const Quantity& maximumSlatAngle);

  void resetMaximumSlatAngle();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::Blind_Impl ImplType;

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
typedef boost::optional<Blind> OptionalBlind;

/** \relates Blind*/
typedef std::vector<Blind> BlindVector;

} // model
} // openstudio

#endif // MODEL_BLIND_HPP

