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

#ifndef MODEL_STANDARDGLAZING_IMPL_HPP
#define MODEL_STANDARDGLAZING_IMPL_HPP

#include "ModelAPI.hpp"
#include "Glazing_Impl.hpp"

#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

namespace openstudio {
namespace model {

namespace detail {

  /** StandardGlazing_Impl is a Glazing_Impl that is the implementation class for StandardGlazing.*/
  class MODEL_API StandardGlazing_Impl : public Glazing_Impl {
    Q_OBJECT;

    Q_PROPERTY(std::string opticalDataType READ opticalDataType WRITE setOpticalDataType);
    Q_PROPERTY(std::vector<std::string> opticalDataTypeValues READ opticalDataTypeValues);

    Q_PROPERTY(boost::optional<std::string> windowGlassSpectralDataSetName READ windowGlassSpectralDataSetName WRITE setWindowGlassSpectralDataSetName RESET resetWindowGlassSpectralDataSetName);
    Q_PROPERTY(double solarTransmittance READ solarTransmittance WRITE setSolarTransmittance RESET resetSolarTransmittanceatNormalIncidence);

    Q_PROPERTY(double thickness READ thickness WRITE setThickness);
    Q_PROPERTY(openstudio::Quantity thickness_SI READ thickness_SI WRITE setThickness);
    Q_PROPERTY(openstudio::Quantity thickness_IP READ thickness_IP WRITE setThickness);

    Q_PROPERTY(boost::optional<double> solarTransmittanceatNormalIncidence READ solarTransmittanceatNormalIncidence WRITE setSolarTransmittanceatNormalIncidence RESET resetSolarTransmittanceatNormalIncidence);
    Q_PROPERTY(openstudio::OSOptionalQuantity solarTransmittanceatNormalIncidence_SI READ solarTransmittanceatNormalIncidence_SI WRITE setSolarTransmittanceatNormalIncidence RESET resetSolarTransmittanceatNormalIncidence);
    Q_PROPERTY(openstudio::OSOptionalQuantity solarTransmittanceatNormalIncidence_IP READ solarTransmittanceatNormalIncidence_IP WRITE setSolarTransmittanceatNormalIncidence RESET resetSolarTransmittanceatNormalIncidence);

    Q_PROPERTY(boost::optional<double> frontSideSolarReflectanceatNormalIncidence READ frontSideSolarReflectanceatNormalIncidence WRITE setFrontSideSolarReflectanceatNormalIncidence RESET resetFrontSideSolarReflectanceatNormalIncidence);
    Q_PROPERTY(openstudio::OSOptionalQuantity frontSideSolarReflectanceatNormalIncidence_SI READ frontSideSolarReflectanceatNormalIncidence_SI WRITE setFrontSideSolarReflectanceatNormalIncidence RESET resetFrontSideSolarReflectanceatNormalIncidence);
    Q_PROPERTY(openstudio::OSOptionalQuantity frontSideSolarReflectanceatNormalIncidence_IP READ frontSideSolarReflectanceatNormalIncidence_IP WRITE setFrontSideSolarReflectanceatNormalIncidence RESET resetFrontSideSolarReflectanceatNormalIncidence);

    Q_PROPERTY(boost::optional<double> backSideSolarReflectanceatNormalIncidence READ backSideSolarReflectanceatNormalIncidence WRITE setBackSideSolarReflectanceatNormalIncidence RESET resetBackSideSolarReflectanceatNormalIncidence);
    Q_PROPERTY(openstudio::OSOptionalQuantity backSideSolarReflectanceatNormalIncidence_SI READ backSideSolarReflectanceatNormalIncidence_SI WRITE setBackSideSolarReflectanceatNormalIncidence RESET resetBackSideSolarReflectanceatNormalIncidence);
    Q_PROPERTY(openstudio::OSOptionalQuantity backSideSolarReflectanceatNormalIncidence_IP READ backSideSolarReflectanceatNormalIncidence_IP WRITE setBackSideSolarReflectanceatNormalIncidence RESET resetBackSideSolarReflectanceatNormalIncidence);

    Q_PROPERTY(boost::optional<double> visibleTransmittanceatNormalIncidence READ visibleTransmittanceatNormalIncidence WRITE setVisibleTransmittanceatNormalIncidence RESET resetVisibleTransmittanceatNormalIncidence);
    Q_PROPERTY(openstudio::OSOptionalQuantity visibleTransmittanceatNormalIncidence_SI READ visibleTransmittanceatNormalIncidence_SI WRITE setVisibleTransmittanceatNormalIncidence RESET resetVisibleTransmittanceatNormalIncidence);
    Q_PROPERTY(openstudio::OSOptionalQuantity visibleTransmittanceatNormalIncidence_IP READ visibleTransmittanceatNormalIncidence_IP WRITE setVisibleTransmittanceatNormalIncidence RESET resetVisibleTransmittanceatNormalIncidence);

    Q_PROPERTY(boost::optional<double> frontSideVisibleReflectanceatNormalIncidence READ frontSideVisibleReflectanceatNormalIncidence WRITE setFrontSideVisibleReflectanceatNormalIncidence RESET resetFrontSideVisibleReflectanceatNormalIncidence);
    Q_PROPERTY(openstudio::OSOptionalQuantity frontSideVisibleReflectanceatNormalIncidence_SI READ frontSideVisibleReflectanceatNormalIncidence_SI WRITE setFrontSideVisibleReflectanceatNormalIncidence RESET resetFrontSideVisibleReflectanceatNormalIncidence);
    Q_PROPERTY(openstudio::OSOptionalQuantity frontSideVisibleReflectanceatNormalIncidence_IP READ frontSideVisibleReflectanceatNormalIncidence_IP WRITE setFrontSideVisibleReflectanceatNormalIncidence RESET resetFrontSideVisibleReflectanceatNormalIncidence);

    Q_PROPERTY(boost::optional<double> backSideVisibleReflectanceatNormalIncidence READ backSideVisibleReflectanceatNormalIncidence WRITE setBackSideVisibleReflectanceatNormalIncidence RESET resetBackSideVisibleReflectanceatNormalIncidence);
    Q_PROPERTY(openstudio::OSOptionalQuantity backSideVisibleReflectanceatNormalIncidence_SI READ backSideVisibleReflectanceatNormalIncidence_SI WRITE setBackSideVisibleReflectanceatNormalIncidence RESET resetBackSideVisibleReflectanceatNormalIncidence);
    Q_PROPERTY(openstudio::OSOptionalQuantity backSideVisibleReflectanceatNormalIncidence_IP READ backSideVisibleReflectanceatNormalIncidence_IP WRITE setBackSideVisibleReflectanceatNormalIncidence RESET resetBackSideVisibleReflectanceatNormalIncidence);

    Q_PROPERTY(double infraredTransmittanceatNormalIncidence READ infraredTransmittanceatNormalIncidence WRITE setInfraredTransmittanceatNormalIncidence RESET resetInfraredTransmittanceatNormalIncidence);
    Q_PROPERTY(openstudio::Quantity infraredTransmittanceatNormalIncidence_SI READ infraredTransmittanceatNormalIncidence_SI WRITE setInfraredTransmittanceatNormalIncidence RESET resetInfraredTransmittanceatNormalIncidence);
    Q_PROPERTY(openstudio::Quantity infraredTransmittanceatNormalIncidence_IP READ infraredTransmittanceatNormalIncidence_IP WRITE setInfraredTransmittanceatNormalIncidence RESET resetInfraredTransmittanceatNormalIncidence);
    Q_PROPERTY(bool isInfraredTransmittanceatNormalIncidenceDefaulted READ isInfraredTransmittanceatNormalIncidenceDefaulted);

    Q_PROPERTY(double frontSideInfraredHemisphericalEmissivity READ frontSideInfraredHemisphericalEmissivity WRITE setFrontSideInfraredHemisphericalEmissivity RESET resetFrontSideInfraredHemisphericalEmissivity);
    Q_PROPERTY(openstudio::Quantity frontSideInfraredHemisphericalEmissivity_SI READ frontSideInfraredHemisphericalEmissivity_SI WRITE setFrontSideInfraredHemisphericalEmissivity RESET resetFrontSideInfraredHemisphericalEmissivity);
    Q_PROPERTY(openstudio::Quantity frontSideInfraredHemisphericalEmissivity_IP READ frontSideInfraredHemisphericalEmissivity_IP WRITE setFrontSideInfraredHemisphericalEmissivity RESET resetFrontSideInfraredHemisphericalEmissivity);
    Q_PROPERTY(bool isFrontSideInfraredHemisphericalEmissivityDefaulted READ isFrontSideInfraredHemisphericalEmissivityDefaulted);

    Q_PROPERTY(double backSideInfraredHemisphericalEmissivity READ backSideInfraredHemisphericalEmissivity WRITE setBackSideInfraredHemisphericalEmissivity RESET resetBackSideInfraredHemisphericalEmissivity);
    Q_PROPERTY(openstudio::Quantity backSideInfraredHemisphericalEmissivity_SI READ backSideInfraredHemisphericalEmissivity_SI WRITE setBackSideInfraredHemisphericalEmissivity RESET resetBackSideInfraredHemisphericalEmissivity);
    Q_PROPERTY(openstudio::Quantity backSideInfraredHemisphericalEmissivity_IP READ backSideInfraredHemisphericalEmissivity_IP WRITE setBackSideInfraredHemisphericalEmissivity RESET resetBackSideInfraredHemisphericalEmissivity);
    Q_PROPERTY(bool isBackSideInfraredHemisphericalEmissivityDefaulted READ isBackSideInfraredHemisphericalEmissivityDefaulted);

    Q_PROPERTY(double conductivity READ conductivity WRITE setConductivity RESET resetConductivity);
    Q_PROPERTY(openstudio::Quantity conductivity_SI READ conductivity_SI WRITE setConductivity RESET resetConductivity);
    Q_PROPERTY(openstudio::Quantity conductivity_IP READ conductivity_IP WRITE setConductivity RESET resetConductivity);
    Q_PROPERTY(bool isConductivityDefaulted READ isConductivityDefaulted);

    Q_PROPERTY(double dirtCorrectionFactorforSolarandVisibleTransmittance READ dirtCorrectionFactorforSolarandVisibleTransmittance WRITE setDirtCorrectionFactorforSolarandVisibleTransmittance RESET resetDirtCorrectionFactorforSolarandVisibleTransmittance);
    Q_PROPERTY(openstudio::Quantity dirtCorrectionFactorforSolarandVisibleTransmittance_SI READ dirtCorrectionFactorforSolarandVisibleTransmittance_SI WRITE setDirtCorrectionFactorforSolarandVisibleTransmittance RESET resetDirtCorrectionFactorforSolarandVisibleTransmittance);
    Q_PROPERTY(openstudio::Quantity dirtCorrectionFactorforSolarandVisibleTransmittance_IP READ dirtCorrectionFactorforSolarandVisibleTransmittance_IP WRITE setDirtCorrectionFactorforSolarandVisibleTransmittance RESET resetDirtCorrectionFactorforSolarandVisibleTransmittance);
    Q_PROPERTY(bool isDirtCorrectionFactorforSolarandVisibleTransmittanceDefaulted READ isDirtCorrectionFactorforSolarandVisibleTransmittanceDefaulted);

    Q_PROPERTY(bool solarDiffusing READ solarDiffusing WRITE setSolarDiffusing RESET resetSolarDiffusing);
    Q_PROPERTY(bool isSolarDiffusingDefaulted READ isSolarDiffusingDefaulted);

    // TODO: Add relationships for objects related to this one, but not pointed to by the underlying data.
    //       Such relationships can be generated by the GenerateRelationships.rb script.
   public:
    /** @name Constructors and Destructors */
    //@{

    StandardGlazing_Impl(const IdfObject& idfObject,
                         Model_Impl* model,
                         bool keepHandle);

    StandardGlazing_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                         Model_Impl* model,
                         bool keepHandle);

    StandardGlazing_Impl(const StandardGlazing_Impl& other,
                         Model_Impl* model,
                         bool keepHandle);

    virtual ~StandardGlazing_Impl() {}

    //@}

    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
    /** @name Getters */
    //@{

    std::string opticalDataType() const;

    boost::optional<std::string> windowGlassSpectralDataSetName() const;

    /** Get the thickness of the material. */
    virtual double thickness() const;

    double solarTransmittance() const;

    Quantity getThickness(bool returnIP=false) const;

    boost::optional<double> solarTransmittanceatNormalIncidence() const;

    OSOptionalQuantity getSolarTransmittanceatNormalIncidence(bool returnIP=false) const;

    boost::optional<double> frontSideSolarReflectanceatNormalIncidence() const;

    OSOptionalQuantity getFrontSideSolarReflectanceatNormalIncidence(bool returnIP=false) const;

    boost::optional<double> backSideSolarReflectanceatNormalIncidence() const;

    OSOptionalQuantity getBackSideSolarReflectanceatNormalIncidence(bool returnIP=false) const;

    virtual boost::optional<double> getVisibleTransmittance() const;

    boost::optional<double> visibleTransmittanceatNormalIncidence() const;

    OSOptionalQuantity getVisibleTransmittanceatNormalIncidence(bool returnIP=false) const;

    boost::optional<double> frontSideVisibleReflectanceatNormalIncidence() const;

    OSOptionalQuantity getFrontSideVisibleReflectanceatNormalIncidence(bool returnIP=false) const;

    boost::optional<double> backSideVisibleReflectanceatNormalIncidence() const;

    OSOptionalQuantity getBackSideVisibleReflectanceatNormalIncidence(bool returnIP=false) const;

    double infraredTransmittance() const;

    double infraredTransmittanceatNormalIncidence() const;

    Quantity getInfraredTransmittanceatNormalIncidence(bool returnIP=false) const;

    bool isInfraredTransmittanceatNormalIncidenceDefaulted() const;

    double frontSideInfraredHemisphericalEmissivity() const;

    Quantity getFrontSideInfraredHemisphericalEmissivity(bool returnIP=false) const;

    bool isFrontSideInfraredHemisphericalEmissivityDefaulted() const;

    double backSideInfraredHemisphericalEmissivity() const;

    Quantity getBackSideInfraredHemisphericalEmissivity(bool returnIP=false) const;

    bool isBackSideInfraredHemisphericalEmissivityDefaulted() const;

    /** The conductivitiy of the material in W/m*K. */
    double thermalConductivity() const;
    
    double conductivity() const;

    Quantity getConductivity(bool returnIP=false) const;

    bool isConductivityDefaulted() const;

    double dirtCorrectionFactorforSolarandVisibleTransmittance() const;

    Quantity getDirtCorrectionFactorforSolarandVisibleTransmittance(bool returnIP=false) const;

    bool isDirtCorrectionFactorforSolarandVisibleTransmittanceDefaulted() const;

    bool solarDiffusing() const;

    bool isSolarDiffusingDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setOpticalDataType(std::string opticalDataType);

    void setWindowGlassSpectralDataSetName(boost::optional<std::string> windowGlassSpectralDataSetName);

    void resetWindowGlassSpectralDataSetName();

    /** Set thickness to value (m). */
    virtual bool setThickness(double value);

    bool setThickness(const Quantity& thickness);

    bool setSolarTransmittance(double value);

    bool setSolarTransmittanceatNormalIncidence(boost::optional<double> solarTransmittanceatNormalIncidence);

    bool setSolarTransmittanceatNormalIncidence(const OSOptionalQuantity& solarTransmittanceatNormalIncidence);

    void resetSolarTransmittanceatNormalIncidence();

    bool setFrontSideSolarReflectanceatNormalIncidence(boost::optional<double> frontSideSolarReflectanceatNormalIncidence);

    bool setFrontSideSolarReflectanceatNormalIncidence(const OSOptionalQuantity& frontSideSolarReflectanceatNormalIncidence);

    void resetFrontSideSolarReflectanceatNormalIncidence();

    bool setBackSideSolarReflectanceatNormalIncidence(boost::optional<double> backSideSolarReflectanceatNormalIncidence);

    bool setBackSideSolarReflectanceatNormalIncidence(const OSOptionalQuantity& backSideSolarReflectanceatNormalIncidence);

    void resetBackSideSolarReflectanceatNormalIncidence();

    bool setVisibleTransmittance(double value);

    bool setVisibleTransmittanceatNormalIncidence(boost::optional<double> visibleTransmittanceatNormalIncidence);

    bool setVisibleTransmittanceatNormalIncidence(const OSOptionalQuantity& visibleTransmittanceatNormalIncidence);

    void resetVisibleTransmittanceatNormalIncidence();

    bool setFrontSideVisibleReflectanceatNormalIncidence(boost::optional<double> frontSideVisibleReflectanceatNormalIncidence);

    bool setFrontSideVisibleReflectanceatNormalIncidence(const OSOptionalQuantity& frontSideVisibleReflectanceatNormalIncidence);

    void resetFrontSideVisibleReflectanceatNormalIncidence();

    bool setBackSideVisibleReflectanceatNormalIncidence(boost::optional<double> backSideVisibleReflectanceatNormalIncidence);

    bool setBackSideVisibleReflectanceatNormalIncidence(const OSOptionalQuantity& backSideVisibleReflectanceatNormalIncidence);

    void resetBackSideVisibleReflectanceatNormalIncidence();

    bool setInfraredTransmittance(double value);

    bool setInfraredTransmittanceatNormalIncidence(double infraredTransmittanceatNormalIncidence);

    bool setInfraredTransmittanceatNormalIncidence(const Quantity& infraredTransmittanceatNormalIncidence);

    void resetInfraredTransmittanceatNormalIncidence();

    bool setFrontSideInfraredHemisphericalEmissivity(double frontSideInfraredHemisphericalEmissivity);

    bool setFrontSideInfraredHemisphericalEmissivity(const Quantity& frontSideInfraredHemisphericalEmissivity);

    void resetFrontSideInfraredHemisphericalEmissivity();

    bool setBackSideInfraredHemisphericalEmissivity(double backSideInfraredHemisphericalEmissivity);

    bool setBackSideInfraredHemisphericalEmissivity(const Quantity& backSideInfraredHemisphericalEmissivity);

    void resetBackSideInfraredHemisphericalEmissivity();

    /** Sets the conductivity of the material in W/m*K, if possible. */
    bool setThermalConductivity(double value);

    bool setConductivity(double conductivity);

    bool setConductivity(const Quantity& conductivity);

    void resetConductivity();

    bool setDirtCorrectionFactorforSolarandVisibleTransmittance(double dirtCorrectionFactorforSolarandVisibleTransmittance);

    bool setDirtCorrectionFactorforSolarandVisibleTransmittance(const Quantity& dirtCorrectionFactorforSolarandVisibleTransmittance);

    void resetDirtCorrectionFactorforSolarandVisibleTransmittance();

    void setSolarDiffusing(bool solarDiffusing);

    void resetSolarDiffusing();

    //@}
    /** @name Other */
    //@{

    /** The conductance of the material in W/m^2*K. */
    double thermalConductance() const;

    /** The resistivity of the material in m*K/W. */
    double thermalResistivity() const;

    /** The resistance of the material in m^2*K/W. */
    double thermalResistance() const;

    double interiorVisibleReflectance() const;

    double exteriorVisibleReflectance() const;

    virtual boost::optional<double> interiorVisibleAbsorptance() const;

    virtual boost::optional<double> exteriorVisibleAbsorptance() const;

    /** Sets the conductance of the material in W/m^2*K, if possible. */
    bool setThermalConductance(double value);

    /** Sets the resistivity of the material in m*K/W, if possible. */
    bool setThermalResistivity(double value);

    /** Sets the resistance of the material in m^2*K/W, if possible. */
    bool setThermalResistance(double value);

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.StandardGlazing");

    std::vector<std::string> opticalDataTypeValues() const;
    openstudio::Quantity thickness_SI() const;
    openstudio::Quantity thickness_IP() const;
    openstudio::OSOptionalQuantity solarTransmittanceatNormalIncidence_SI() const;
    openstudio::OSOptionalQuantity solarTransmittanceatNormalIncidence_IP() const;
    openstudio::OSOptionalQuantity frontSideSolarReflectanceatNormalIncidence_SI() const;
    openstudio::OSOptionalQuantity frontSideSolarReflectanceatNormalIncidence_IP() const;
    openstudio::OSOptionalQuantity backSideSolarReflectanceatNormalIncidence_SI() const;
    openstudio::OSOptionalQuantity backSideSolarReflectanceatNormalIncidence_IP() const;
    openstudio::OSOptionalQuantity visibleTransmittanceatNormalIncidence_SI() const;
    openstudio::OSOptionalQuantity visibleTransmittanceatNormalIncidence_IP() const;
    openstudio::OSOptionalQuantity frontSideVisibleReflectanceatNormalIncidence_SI() const;
    openstudio::OSOptionalQuantity frontSideVisibleReflectanceatNormalIncidence_IP() const;
    openstudio::OSOptionalQuantity backSideVisibleReflectanceatNormalIncidence_SI() const;
    openstudio::OSOptionalQuantity backSideVisibleReflectanceatNormalIncidence_IP() const;
    openstudio::Quantity infraredTransmittanceatNormalIncidence_SI() const;
    openstudio::Quantity infraredTransmittanceatNormalIncidence_IP() const;
    openstudio::Quantity frontSideInfraredHemisphericalEmissivity_SI() const;
    openstudio::Quantity frontSideInfraredHemisphericalEmissivity_IP() const;
    openstudio::Quantity backSideInfraredHemisphericalEmissivity_SI() const;
    openstudio::Quantity backSideInfraredHemisphericalEmissivity_IP() const;
    openstudio::Quantity conductivity_SI() const;
    openstudio::Quantity conductivity_IP() const;
    openstudio::Quantity dirtCorrectionFactorforSolarandVisibleTransmittance_SI() const;
    openstudio::Quantity dirtCorrectionFactorforSolarandVisibleTransmittance_IP() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_STANDARDGLAZING_IMPL_HPP

