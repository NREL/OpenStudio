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

#ifndef MODEL_BLIND_IMPL_HPP
#define MODEL_BLIND_IMPL_HPP

#include "ModelAPI.hpp"
#include "ShadingMaterial_Impl.hpp"

#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

namespace openstudio {
namespace model {

namespace detail {

  /** Blind_Impl is a ShadingMaterial_Impl that is the implementation class for Blind.*/
  class MODEL_API Blind_Impl : public ShadingMaterial_Impl {
    Q_OBJECT;

    Q_PROPERTY(std::string slatOrientation READ slatOrientation WRITE setSlatOrientation RESET resetSlatOrientation);
    Q_PROPERTY(bool isSlatOrientationDefaulted READ isSlatOrientationDefaulted);
    Q_PROPERTY(std::vector<std::string> slatOrientationValues READ slatOrientationValues);

    Q_PROPERTY(double slatWidth READ slatWidth WRITE setSlatWidth);
    Q_PROPERTY(openstudio::Quantity slatWidth_SI READ slatWidth_SI WRITE setSlatWidth);
    Q_PROPERTY(openstudio::Quantity slatWidth_IP READ slatWidth_IP WRITE setSlatWidth);

    Q_PROPERTY(double slatSeparation READ slatSeparation WRITE setSlatSeparation);
    Q_PROPERTY(openstudio::Quantity slatSeparation_SI READ slatSeparation_SI WRITE setSlatSeparation);
    Q_PROPERTY(openstudio::Quantity slatSeparation_IP READ slatSeparation_IP WRITE setSlatSeparation);

    Q_PROPERTY(double slatThickness READ slatThickness WRITE setSlatThickness RESET resetSlatThickness);
    Q_PROPERTY(openstudio::Quantity slatThickness_SI READ slatThickness_SI WRITE setSlatThickness RESET resetSlatThickness);
    Q_PROPERTY(openstudio::Quantity slatThickness_IP READ slatThickness_IP WRITE setSlatThickness RESET resetSlatThickness);
    Q_PROPERTY(bool isSlatThicknessDefaulted READ isSlatThicknessDefaulted);

    Q_PROPERTY(double slatAngle READ slatAngle WRITE setSlatAngle RESET resetSlatAngle);
    Q_PROPERTY(openstudio::Quantity slatAngle_SI READ slatAngle_SI WRITE setSlatAngle RESET resetSlatAngle);
    Q_PROPERTY(openstudio::Quantity slatAngle_IP READ slatAngle_IP WRITE setSlatAngle RESET resetSlatAngle);
    Q_PROPERTY(bool isSlatAngleDefaulted READ isSlatAngleDefaulted);

    Q_PROPERTY(double slatConductivity READ slatConductivity WRITE setSlatConductivity RESET resetSlatConductivity);
    Q_PROPERTY(openstudio::Quantity slatConductivity_SI READ slatConductivity_SI WRITE setSlatConductivity RESET resetSlatConductivity);
    Q_PROPERTY(openstudio::Quantity slatConductivity_IP READ slatConductivity_IP WRITE setSlatConductivity RESET resetSlatConductivity);
    Q_PROPERTY(bool isSlatConductivityDefaulted READ isSlatConductivityDefaulted);

    Q_PROPERTY(double slatBeamSolarTransmittance READ slatBeamSolarTransmittance WRITE setSlatBeamSolarTransmittance RESET resetSlatBeamSolarTransmittance);
    Q_PROPERTY(openstudio::Quantity slatBeamSolarTransmittance_SI READ slatBeamSolarTransmittance_SI WRITE setSlatBeamSolarTransmittance RESET resetSlatBeamSolarTransmittance);
    Q_PROPERTY(openstudio::Quantity slatBeamSolarTransmittance_IP READ slatBeamSolarTransmittance_IP WRITE setSlatBeamSolarTransmittance RESET resetSlatBeamSolarTransmittance);
    Q_PROPERTY(bool isSlatBeamSolarTransmittanceDefaulted READ isSlatBeamSolarTransmittanceDefaulted);

    Q_PROPERTY(double frontSideSlatBeamSolarReflectance READ frontSideSlatBeamSolarReflectance WRITE setFrontSideSlatBeamSolarReflectance);
    Q_PROPERTY(openstudio::Quantity frontSideSlatBeamSolarReflectance_SI READ frontSideSlatBeamSolarReflectance_SI WRITE setFrontSideSlatBeamSolarReflectance);
    Q_PROPERTY(openstudio::Quantity frontSideSlatBeamSolarReflectance_IP READ frontSideSlatBeamSolarReflectance_IP WRITE setFrontSideSlatBeamSolarReflectance);

    Q_PROPERTY(double backSideSlatBeamSolarReflectance READ backSideSlatBeamSolarReflectance WRITE setBackSideSlatBeamSolarReflectance);
    Q_PROPERTY(openstudio::Quantity backSideSlatBeamSolarReflectance_SI READ backSideSlatBeamSolarReflectance_SI WRITE setBackSideSlatBeamSolarReflectance);
    Q_PROPERTY(openstudio::Quantity backSideSlatBeamSolarReflectance_IP READ backSideSlatBeamSolarReflectance_IP WRITE setBackSideSlatBeamSolarReflectance);

    Q_PROPERTY(double slatDiffuseSolarTransmittance READ slatDiffuseSolarTransmittance WRITE setSlatDiffuseSolarTransmittance RESET resetSlatDiffuseSolarTransmittance);
    Q_PROPERTY(openstudio::Quantity slatDiffuseSolarTransmittance_SI READ slatDiffuseSolarTransmittance_SI WRITE setSlatDiffuseSolarTransmittance RESET resetSlatDiffuseSolarTransmittance);
    Q_PROPERTY(openstudio::Quantity slatDiffuseSolarTransmittance_IP READ slatDiffuseSolarTransmittance_IP WRITE setSlatDiffuseSolarTransmittance RESET resetSlatDiffuseSolarTransmittance);
    Q_PROPERTY(bool isSlatDiffuseSolarTransmittanceDefaulted READ isSlatDiffuseSolarTransmittanceDefaulted);

    Q_PROPERTY(double frontSideSlatDiffuseSolarReflectance READ frontSideSlatDiffuseSolarReflectance WRITE setFrontSideSlatDiffuseSolarReflectance);
    Q_PROPERTY(openstudio::Quantity frontSideSlatDiffuseSolarReflectance_SI READ frontSideSlatDiffuseSolarReflectance_SI WRITE setFrontSideSlatDiffuseSolarReflectance);
    Q_PROPERTY(openstudio::Quantity frontSideSlatDiffuseSolarReflectance_IP READ frontSideSlatDiffuseSolarReflectance_IP WRITE setFrontSideSlatDiffuseSolarReflectance);

    Q_PROPERTY(double backSideSlatDiffuseSolarReflectance READ backSideSlatDiffuseSolarReflectance WRITE setBackSideSlatDiffuseSolarReflectance);
    Q_PROPERTY(openstudio::Quantity backSideSlatDiffuseSolarReflectance_SI READ backSideSlatDiffuseSolarReflectance_SI WRITE setBackSideSlatDiffuseSolarReflectance);
    Q_PROPERTY(openstudio::Quantity backSideSlatDiffuseSolarReflectance_IP READ backSideSlatDiffuseSolarReflectance_IP WRITE setBackSideSlatDiffuseSolarReflectance);

    Q_PROPERTY(double slatBeamVisibleTransmittance READ slatBeamVisibleTransmittance WRITE setSlatBeamVisibleTransmittance);
    Q_PROPERTY(openstudio::Quantity slatBeamVisibleTransmittance_SI READ slatBeamVisibleTransmittance_SI WRITE setSlatBeamVisibleTransmittance);
    Q_PROPERTY(openstudio::Quantity slatBeamVisibleTransmittance_IP READ slatBeamVisibleTransmittance_IP WRITE setSlatBeamVisibleTransmittance);

    Q_PROPERTY(boost::optional<double> frontSideSlatBeamVisibleReflectance READ frontSideSlatBeamVisibleReflectance WRITE setFrontSideSlatBeamVisibleReflectance RESET resetFrontSideSlatBeamVisibleReflectance);
    Q_PROPERTY(openstudio::OSOptionalQuantity frontSideSlatBeamVisibleReflectance_SI READ frontSideSlatBeamVisibleReflectance_SI WRITE setFrontSideSlatBeamVisibleReflectance RESET resetFrontSideSlatBeamVisibleReflectance);
    Q_PROPERTY(openstudio::OSOptionalQuantity frontSideSlatBeamVisibleReflectance_IP READ frontSideSlatBeamVisibleReflectance_IP WRITE setFrontSideSlatBeamVisibleReflectance RESET resetFrontSideSlatBeamVisibleReflectance);

    Q_PROPERTY(boost::optional<double> backSideSlatBeamVisibleReflectance READ backSideSlatBeamVisibleReflectance WRITE setBackSideSlatBeamVisibleReflectance RESET resetBackSideSlatBeamVisibleReflectance);
    Q_PROPERTY(openstudio::OSOptionalQuantity backSideSlatBeamVisibleReflectance_SI READ backSideSlatBeamVisibleReflectance_SI WRITE setBackSideSlatBeamVisibleReflectance RESET resetBackSideSlatBeamVisibleReflectance);
    Q_PROPERTY(openstudio::OSOptionalQuantity backSideSlatBeamVisibleReflectance_IP READ backSideSlatBeamVisibleReflectance_IP WRITE setBackSideSlatBeamVisibleReflectance RESET resetBackSideSlatBeamVisibleReflectance);

    Q_PROPERTY(double slatDiffuseVisibleTransmittance READ slatDiffuseVisibleTransmittance WRITE setSlatDiffuseVisibleTransmittance RESET resetSlatDiffuseVisibleTransmittance);
    Q_PROPERTY(openstudio::Quantity slatDiffuseVisibleTransmittance_SI READ slatDiffuseVisibleTransmittance_SI WRITE setSlatDiffuseVisibleTransmittance RESET resetSlatDiffuseVisibleTransmittance);
    Q_PROPERTY(openstudio::Quantity slatDiffuseVisibleTransmittance_IP READ slatDiffuseVisibleTransmittance_IP WRITE setSlatDiffuseVisibleTransmittance RESET resetSlatDiffuseVisibleTransmittance);
    Q_PROPERTY(bool isSlatDiffuseVisibleTransmittanceDefaulted READ isSlatDiffuseVisibleTransmittanceDefaulted);

    Q_PROPERTY(boost::optional<double> frontSideSlatDiffuseVisibleReflectance READ frontSideSlatDiffuseVisibleReflectance WRITE setFrontSideSlatDiffuseVisibleReflectance RESET resetFrontSideSlatDiffuseVisibleReflectance);
    Q_PROPERTY(openstudio::OSOptionalQuantity frontSideSlatDiffuseVisibleReflectance_SI READ frontSideSlatDiffuseVisibleReflectance_SI WRITE setFrontSideSlatDiffuseVisibleReflectance RESET resetFrontSideSlatDiffuseVisibleReflectance);
    Q_PROPERTY(openstudio::OSOptionalQuantity frontSideSlatDiffuseVisibleReflectance_IP READ frontSideSlatDiffuseVisibleReflectance_IP WRITE setFrontSideSlatDiffuseVisibleReflectance RESET resetFrontSideSlatDiffuseVisibleReflectance);

    Q_PROPERTY(boost::optional<double> backSideSlatDiffuseVisibleReflectance READ backSideSlatDiffuseVisibleReflectance WRITE setBackSideSlatDiffuseVisibleReflectance RESET resetBackSideSlatDiffuseVisibleReflectance);
    Q_PROPERTY(openstudio::OSOptionalQuantity backSideSlatDiffuseVisibleReflectance_SI READ backSideSlatDiffuseVisibleReflectance_SI WRITE setBackSideSlatDiffuseVisibleReflectance RESET resetBackSideSlatDiffuseVisibleReflectance);
    Q_PROPERTY(openstudio::OSOptionalQuantity backSideSlatDiffuseVisibleReflectance_IP READ backSideSlatDiffuseVisibleReflectance_IP WRITE setBackSideSlatDiffuseVisibleReflectance RESET resetBackSideSlatDiffuseVisibleReflectance);

    Q_PROPERTY(double slatInfraredHemisphericalTransmittance READ slatInfraredHemisphericalTransmittance WRITE setSlatInfraredHemisphericalTransmittance RESET resetSlatInfraredHemisphericalTransmittance);
    Q_PROPERTY(openstudio::Quantity slatInfraredHemisphericalTransmittance_SI READ slatInfraredHemisphericalTransmittance_SI WRITE setSlatInfraredHemisphericalTransmittance RESET resetSlatInfraredHemisphericalTransmittance);
    Q_PROPERTY(openstudio::Quantity slatInfraredHemisphericalTransmittance_IP READ slatInfraredHemisphericalTransmittance_IP WRITE setSlatInfraredHemisphericalTransmittance RESET resetSlatInfraredHemisphericalTransmittance);
    Q_PROPERTY(bool isSlatInfraredHemisphericalTransmittanceDefaulted READ isSlatInfraredHemisphericalTransmittanceDefaulted);

    Q_PROPERTY(double frontSideSlatInfraredHemisphericalEmissivity READ frontSideSlatInfraredHemisphericalEmissivity WRITE setFrontSideSlatInfraredHemisphericalEmissivity RESET resetFrontSideSlatInfraredHemisphericalEmissivity);
    Q_PROPERTY(openstudio::Quantity frontSideSlatInfraredHemisphericalEmissivity_SI READ frontSideSlatInfraredHemisphericalEmissivity_SI WRITE setFrontSideSlatInfraredHemisphericalEmissivity RESET resetFrontSideSlatInfraredHemisphericalEmissivity);
    Q_PROPERTY(openstudio::Quantity frontSideSlatInfraredHemisphericalEmissivity_IP READ frontSideSlatInfraredHemisphericalEmissivity_IP WRITE setFrontSideSlatInfraredHemisphericalEmissivity RESET resetFrontSideSlatInfraredHemisphericalEmissivity);
    Q_PROPERTY(bool isFrontSideSlatInfraredHemisphericalEmissivityDefaulted READ isFrontSideSlatInfraredHemisphericalEmissivityDefaulted);

    Q_PROPERTY(double backSideSlatInfraredHemisphericalEmissivity READ backSideSlatInfraredHemisphericalEmissivity WRITE setBackSideSlatInfraredHemisphericalEmissivity RESET resetBackSideSlatInfraredHemisphericalEmissivity);
    Q_PROPERTY(openstudio::Quantity backSideSlatInfraredHemisphericalEmissivity_SI READ backSideSlatInfraredHemisphericalEmissivity_SI WRITE setBackSideSlatInfraredHemisphericalEmissivity RESET resetBackSideSlatInfraredHemisphericalEmissivity);
    Q_PROPERTY(openstudio::Quantity backSideSlatInfraredHemisphericalEmissivity_IP READ backSideSlatInfraredHemisphericalEmissivity_IP WRITE setBackSideSlatInfraredHemisphericalEmissivity RESET resetBackSideSlatInfraredHemisphericalEmissivity);
    Q_PROPERTY(bool isBackSideSlatInfraredHemisphericalEmissivityDefaulted READ isBackSideSlatInfraredHemisphericalEmissivityDefaulted);

    Q_PROPERTY(double blindtoGlassDistance READ blindtoGlassDistance WRITE setBlindtoGlassDistance RESET resetBlindtoGlassDistance);
    Q_PROPERTY(openstudio::Quantity blindtoGlassDistance_SI READ blindtoGlassDistance_SI WRITE setBlindtoGlassDistance RESET resetBlindtoGlassDistance);
    Q_PROPERTY(openstudio::Quantity blindtoGlassDistance_IP READ blindtoGlassDistance_IP WRITE setBlindtoGlassDistance RESET resetBlindtoGlassDistance);
    Q_PROPERTY(bool isBlindtoGlassDistanceDefaulted READ isBlindtoGlassDistanceDefaulted);

    Q_PROPERTY(double blindTopOpeningMultiplier READ blindTopOpeningMultiplier WRITE setBlindTopOpeningMultiplier RESET resetBlindTopOpeningMultiplier);
    Q_PROPERTY(openstudio::Quantity blindTopOpeningMultiplier_SI READ blindTopOpeningMultiplier_SI WRITE setBlindTopOpeningMultiplier RESET resetBlindTopOpeningMultiplier);
    Q_PROPERTY(openstudio::Quantity blindTopOpeningMultiplier_IP READ blindTopOpeningMultiplier_IP WRITE setBlindTopOpeningMultiplier RESET resetBlindTopOpeningMultiplier);
    Q_PROPERTY(bool isBlindTopOpeningMultiplierDefaulted READ isBlindTopOpeningMultiplierDefaulted);

    Q_PROPERTY(double blindBottomOpeningMultiplier READ blindBottomOpeningMultiplier WRITE setBlindBottomOpeningMultiplier RESET resetBlindBottomOpeningMultiplier);
    Q_PROPERTY(openstudio::Quantity blindBottomOpeningMultiplier_SI READ blindBottomOpeningMultiplier_SI WRITE setBlindBottomOpeningMultiplier RESET resetBlindBottomOpeningMultiplier);
    Q_PROPERTY(openstudio::Quantity blindBottomOpeningMultiplier_IP READ blindBottomOpeningMultiplier_IP WRITE setBlindBottomOpeningMultiplier RESET resetBlindBottomOpeningMultiplier);
    Q_PROPERTY(bool isBlindBottomOpeningMultiplierDefaulted READ isBlindBottomOpeningMultiplierDefaulted);

    Q_PROPERTY(double blindLeftSideOpeningMultiplier READ blindLeftSideOpeningMultiplier WRITE setBlindLeftSideOpeningMultiplier RESET resetBlindLeftSideOpeningMultiplier);
    Q_PROPERTY(openstudio::Quantity blindLeftSideOpeningMultiplier_SI READ blindLeftSideOpeningMultiplier_SI WRITE setBlindLeftSideOpeningMultiplier RESET resetBlindLeftSideOpeningMultiplier);
    Q_PROPERTY(openstudio::Quantity blindLeftSideOpeningMultiplier_IP READ blindLeftSideOpeningMultiplier_IP WRITE setBlindLeftSideOpeningMultiplier RESET resetBlindLeftSideOpeningMultiplier);
    Q_PROPERTY(bool isBlindLeftSideOpeningMultiplierDefaulted READ isBlindLeftSideOpeningMultiplierDefaulted);

    Q_PROPERTY(double blindRightSideOpeningMultiplier READ blindRightSideOpeningMultiplier WRITE setBlindRightSideOpeningMultiplier RESET resetBlindRightSideOpeningMultiplier);
    Q_PROPERTY(openstudio::Quantity blindRightSideOpeningMultiplier_SI READ blindRightSideOpeningMultiplier_SI WRITE setBlindRightSideOpeningMultiplier RESET resetBlindRightSideOpeningMultiplier);
    Q_PROPERTY(openstudio::Quantity blindRightSideOpeningMultiplier_IP READ blindRightSideOpeningMultiplier_IP WRITE setBlindRightSideOpeningMultiplier RESET resetBlindRightSideOpeningMultiplier);
    Q_PROPERTY(bool isBlindRightSideOpeningMultiplierDefaulted READ isBlindRightSideOpeningMultiplierDefaulted);

    Q_PROPERTY(double minimumSlatAngle READ minimumSlatAngle WRITE setMinimumSlatAngle RESET resetMinimumSlatAngle);
    Q_PROPERTY(openstudio::Quantity minimumSlatAngle_SI READ minimumSlatAngle_SI WRITE setMinimumSlatAngle RESET resetMinimumSlatAngle);
    Q_PROPERTY(openstudio::Quantity minimumSlatAngle_IP READ minimumSlatAngle_IP WRITE setMinimumSlatAngle RESET resetMinimumSlatAngle);
    Q_PROPERTY(bool isMinimumSlatAngleDefaulted READ isMinimumSlatAngleDefaulted);

    Q_PROPERTY(double maximumSlatAngle READ maximumSlatAngle WRITE setMaximumSlatAngle RESET resetMaximumSlatAngle);
    Q_PROPERTY(openstudio::Quantity maximumSlatAngle_SI READ maximumSlatAngle_SI WRITE setMaximumSlatAngle RESET resetMaximumSlatAngle);
    Q_PROPERTY(openstudio::Quantity maximumSlatAngle_IP READ maximumSlatAngle_IP WRITE setMaximumSlatAngle RESET resetMaximumSlatAngle);
    Q_PROPERTY(bool isMaximumSlatAngleDefaulted READ isMaximumSlatAngleDefaulted);

    // TODO: Add relationships for objects related to this one, but not pointed to by the underlying data.
    //       Such relationships can be generated by the GenerateRelationships.rb script.
   public:
    /** @name Constructors and Destructors */
    //@{

    Blind_Impl(const IdfObject& idfObject,
               Model_Impl* model,
               bool keepHandle);

    Blind_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
               Model_Impl* model,
               bool keepHandle);

    Blind_Impl(const Blind_Impl& other,
               Model_Impl* model,
               bool keepHandle);

    virtual ~Blind_Impl() {}

    //@}

    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
    /** @name Getters */
    //@{

    /** Get the thickness of the material. For now is equivalent to slat thickness. */
    virtual double thickness() const;

    std::string slatOrientation() const;
    /** Throws because blind has beam and diffuse visible transmittance. */
    virtual boost::optional<double> getVisibleTransmittance() const;

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

    /** Set thickness to value (m). For now is equivalent to setting slat thickness. */
    virtual bool setThickness(double value);

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

    bool setFrontSideSlatBeamVisibleReflectance(boost::optional<double> frontSideSlatBeamVisibleReflectance);

    bool setFrontSideSlatBeamVisibleReflectance(const OSOptionalQuantity& frontSideSlatBeamVisibleReflectance);

    void resetFrontSideSlatBeamVisibleReflectance();

    bool setBackSideSlatBeamVisibleReflectance(boost::optional<double> backSideSlatBeamVisibleReflectance);

    bool setBackSideSlatBeamVisibleReflectance(const OSOptionalQuantity& backSideSlatBeamVisibleReflectance);

    void resetBackSideSlatBeamVisibleReflectance();

    bool setSlatDiffuseVisibleTransmittance(double slatDiffuseVisibleTransmittance);

    bool setSlatDiffuseVisibleTransmittance(const Quantity& slatDiffuseVisibleTransmittance);

    void resetSlatDiffuseVisibleTransmittance();

    bool setFrontSideSlatDiffuseVisibleReflectance(boost::optional<double> frontSideSlatDiffuseVisibleReflectance);

    bool setFrontSideSlatDiffuseVisibleReflectance(const OSOptionalQuantity& frontSideSlatDiffuseVisibleReflectance);

    void resetFrontSideSlatDiffuseVisibleReflectance();

    bool setBackSideSlatDiffuseVisibleReflectance(boost::optional<double> backSideSlatDiffuseVisibleReflectance);

    bool setBackSideSlatDiffuseVisibleReflectance(const OSOptionalQuantity& backSideSlatDiffuseVisibleReflectance);

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
   private:
    REGISTER_LOGGER("openstudio.model.Blind");

    std::vector<std::string> slatOrientationValues() const;
    openstudio::Quantity slatWidth_SI() const;
    openstudio::Quantity slatWidth_IP() const;
    openstudio::Quantity slatSeparation_SI() const;
    openstudio::Quantity slatSeparation_IP() const;
    openstudio::Quantity slatThickness_SI() const;
    openstudio::Quantity slatThickness_IP() const;
    openstudio::Quantity slatAngle_SI() const;
    openstudio::Quantity slatAngle_IP() const;
    openstudio::Quantity slatConductivity_SI() const;
    openstudio::Quantity slatConductivity_IP() const;
    openstudio::Quantity slatBeamSolarTransmittance_SI() const;
    openstudio::Quantity slatBeamSolarTransmittance_IP() const;
    openstudio::Quantity frontSideSlatBeamSolarReflectance_SI() const;
    openstudio::Quantity frontSideSlatBeamSolarReflectance_IP() const;
    openstudio::Quantity backSideSlatBeamSolarReflectance_SI() const;
    openstudio::Quantity backSideSlatBeamSolarReflectance_IP() const;
    openstudio::Quantity slatDiffuseSolarTransmittance_SI() const;
    openstudio::Quantity slatDiffuseSolarTransmittance_IP() const;
    openstudio::Quantity frontSideSlatDiffuseSolarReflectance_SI() const;
    openstudio::Quantity frontSideSlatDiffuseSolarReflectance_IP() const;
    openstudio::Quantity backSideSlatDiffuseSolarReflectance_SI() const;
    openstudio::Quantity backSideSlatDiffuseSolarReflectance_IP() const;
    openstudio::Quantity slatBeamVisibleTransmittance_SI() const;
    openstudio::Quantity slatBeamVisibleTransmittance_IP() const;
    openstudio::OSOptionalQuantity frontSideSlatBeamVisibleReflectance_SI() const;
    openstudio::OSOptionalQuantity frontSideSlatBeamVisibleReflectance_IP() const;
    openstudio::OSOptionalQuantity backSideSlatBeamVisibleReflectance_SI() const;
    openstudio::OSOptionalQuantity backSideSlatBeamVisibleReflectance_IP() const;
    openstudio::Quantity slatDiffuseVisibleTransmittance_SI() const;
    openstudio::Quantity slatDiffuseVisibleTransmittance_IP() const;
    openstudio::OSOptionalQuantity frontSideSlatDiffuseVisibleReflectance_SI() const;
    openstudio::OSOptionalQuantity frontSideSlatDiffuseVisibleReflectance_IP() const;
    openstudio::OSOptionalQuantity backSideSlatDiffuseVisibleReflectance_SI() const;
    openstudio::OSOptionalQuantity backSideSlatDiffuseVisibleReflectance_IP() const;
    openstudio::Quantity slatInfraredHemisphericalTransmittance_SI() const;
    openstudio::Quantity slatInfraredHemisphericalTransmittance_IP() const;
    openstudio::Quantity frontSideSlatInfraredHemisphericalEmissivity_SI() const;
    openstudio::Quantity frontSideSlatInfraredHemisphericalEmissivity_IP() const;
    openstudio::Quantity backSideSlatInfraredHemisphericalEmissivity_SI() const;
    openstudio::Quantity backSideSlatInfraredHemisphericalEmissivity_IP() const;
    openstudio::Quantity blindtoGlassDistance_SI() const;
    openstudio::Quantity blindtoGlassDistance_IP() const;
    openstudio::Quantity blindTopOpeningMultiplier_SI() const;
    openstudio::Quantity blindTopOpeningMultiplier_IP() const;
    openstudio::Quantity blindBottomOpeningMultiplier_SI() const;
    openstudio::Quantity blindBottomOpeningMultiplier_IP() const;
    openstudio::Quantity blindLeftSideOpeningMultiplier_SI() const;
    openstudio::Quantity blindLeftSideOpeningMultiplier_IP() const;
    openstudio::Quantity blindRightSideOpeningMultiplier_SI() const;
    openstudio::Quantity blindRightSideOpeningMultiplier_IP() const;
    openstudio::Quantity minimumSlatAngle_SI() const;
    openstudio::Quantity minimumSlatAngle_IP() const;
    openstudio::Quantity maximumSlatAngle_SI() const;
    openstudio::Quantity maximumSlatAngle_IP() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_BLIND_IMPL_HPP

