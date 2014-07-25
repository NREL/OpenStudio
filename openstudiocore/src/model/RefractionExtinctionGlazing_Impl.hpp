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

#ifndef MODEL_REFRACTIONEXTINCTIONGLAZING_IMPL_HPP
#define MODEL_REFRACTIONEXTINCTIONGLAZING_IMPL_HPP

#include "ModelAPI.hpp"
#include "Glazing_Impl.hpp"

#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

namespace openstudio {
namespace model {

namespace detail {

  /** RefractionExtinctionGlazing_Impl is a Glazing_Impl that is the implementation class for RefractionExtinctionGlazing.*/
  class MODEL_API RefractionExtinctionGlazing_Impl : public Glazing_Impl {
    Q_OBJECT;

    Q_PROPERTY(double thickness READ thickness WRITE setThickness);
    Q_PROPERTY(openstudio::Quantity thickness_SI READ thickness_SI WRITE setThickness);
    Q_PROPERTY(openstudio::Quantity thickness_IP READ thickness_IP WRITE setThickness);

    Q_PROPERTY(double solarIndexofRefraction READ solarIndexofRefraction WRITE setSolarIndexofRefraction);
    Q_PROPERTY(openstudio::Quantity solarIndexofRefraction_SI READ solarIndexofRefraction_SI WRITE setSolarIndexofRefraction);
    Q_PROPERTY(openstudio::Quantity solarIndexofRefraction_IP READ solarIndexofRefraction_IP WRITE setSolarIndexofRefraction);

    Q_PROPERTY(double solarExtinctionCoefficient READ solarExtinctionCoefficient WRITE setSolarExtinctionCoefficient);
    Q_PROPERTY(openstudio::Quantity solarExtinctionCoefficient_SI READ solarExtinctionCoefficient_SI WRITE setSolarExtinctionCoefficient);
    Q_PROPERTY(openstudio::Quantity solarExtinctionCoefficient_IP READ solarExtinctionCoefficient_IP WRITE setSolarExtinctionCoefficient);

    Q_PROPERTY(double visibleIndexofRefraction READ visibleIndexofRefraction WRITE setVisibleIndexofRefraction);
    Q_PROPERTY(openstudio::Quantity visibleIndexofRefraction_SI READ visibleIndexofRefraction_SI WRITE setVisibleIndexofRefraction);
    Q_PROPERTY(openstudio::Quantity visibleIndexofRefraction_IP READ visibleIndexofRefraction_IP WRITE setVisibleIndexofRefraction);

    Q_PROPERTY(double visibleExtinctionCoefficient READ visibleExtinctionCoefficient WRITE setVisibleExtinctionCoefficient);
    Q_PROPERTY(openstudio::Quantity visibleExtinctionCoefficient_SI READ visibleExtinctionCoefficient_SI WRITE setVisibleExtinctionCoefficient);
    Q_PROPERTY(openstudio::Quantity visibleExtinctionCoefficient_IP READ visibleExtinctionCoefficient_IP WRITE setVisibleExtinctionCoefficient);

    Q_PROPERTY(double infraredTransmittanceatNormalIncidence READ infraredTransmittanceatNormalIncidence WRITE setInfraredTransmittanceatNormalIncidence RESET resetInfraredTransmittanceatNormalIncidence);
    Q_PROPERTY(openstudio::Quantity infraredTransmittanceatNormalIncidence_SI READ infraredTransmittanceatNormalIncidence_SI WRITE setInfraredTransmittanceatNormalIncidence RESET resetInfraredTransmittanceatNormalIncidence);
    Q_PROPERTY(openstudio::Quantity infraredTransmittanceatNormalIncidence_IP READ infraredTransmittanceatNormalIncidence_IP WRITE setInfraredTransmittanceatNormalIncidence RESET resetInfraredTransmittanceatNormalIncidence);
    Q_PROPERTY(bool isInfraredTransmittanceatNormalIncidenceDefaulted READ isInfraredTransmittanceatNormalIncidenceDefaulted);

    Q_PROPERTY(double infraredHemisphericalEmissivity READ infraredHemisphericalEmissivity WRITE setInfraredHemisphericalEmissivity RESET resetInfraredHemisphericalEmissivity);
    Q_PROPERTY(openstudio::Quantity infraredHemisphericalEmissivity_SI READ infraredHemisphericalEmissivity_SI WRITE setInfraredHemisphericalEmissivity RESET resetInfraredHemisphericalEmissivity);
    Q_PROPERTY(openstudio::Quantity infraredHemisphericalEmissivity_IP READ infraredHemisphericalEmissivity_IP WRITE setInfraredHemisphericalEmissivity RESET resetInfraredHemisphericalEmissivity);
    Q_PROPERTY(bool isInfraredHemisphericalEmissivityDefaulted READ isInfraredHemisphericalEmissivityDefaulted);

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

    RefractionExtinctionGlazing_Impl(const IdfObject& idfObject,
                                     Model_Impl* model,
                                     bool keepHandle);

    RefractionExtinctionGlazing_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle);

    RefractionExtinctionGlazing_Impl(const RefractionExtinctionGlazing_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle);

    virtual ~RefractionExtinctionGlazing_Impl() {}

    //@}

    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
    /** @name Getters */
    //@{

    double thickness() const;

    double thermalConductivity() const;

    double thermalConductance() const;

    double thermalResistivity() const;

    double thermalResistance() const;

    double thermalTransmittance() const;

    virtual boost::optional<double> getVisibleTransmittance() const;

    /** This model assumes that the front- and back-side properties are identical. */
    double visibleAbsorptance() const;

    virtual boost::optional<double> interiorVisibleAbsorptance() const;

    virtual boost::optional<double> exteriorVisibleAbsorptance() const;

    Quantity getThickness(bool returnIP=false) const;

    double solarIndexofRefraction() const;

    Quantity getSolarIndexofRefraction(bool returnIP=false) const;

    double solarExtinctionCoefficient() const;

    Quantity getSolarExtinctionCoefficient(bool returnIP=false) const;

    double visibleIndexofRefraction() const;

    Quantity getVisibleIndexofRefraction(bool returnIP=false) const;

    double visibleExtinctionCoefficient() const;

    Quantity getVisibleExtinctionCoefficient(bool returnIP=false) const;

    double infraredTransmittanceatNormalIncidence() const;

    Quantity getInfraredTransmittanceatNormalIncidence(bool returnIP=false) const;

    bool isInfraredTransmittanceatNormalIncidenceDefaulted() const;

    double infraredHemisphericalEmissivity() const;

    Quantity getInfraredHemisphericalEmissivity(bool returnIP=false) const;

    bool isInfraredHemisphericalEmissivityDefaulted() const;

    virtual double conductivity() const;

    virtual Quantity getConductivity(bool returnIP=false) const;

    virtual bool isConductivityDefaulted() const;

    virtual double dirtCorrectionFactorforSolarandVisibleTransmittance() const;

    virtual Quantity getDirtCorrectionFactorforSolarandVisibleTransmittance(bool returnIP=false) const;

    virtual bool isDirtCorrectionFactorforSolarandVisibleTransmittanceDefaulted() const;

    bool solarDiffusing() const;

    bool isSolarDiffusingDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setThermalConductivity(double value);

    bool setThermalConductance(double value);

    bool setThermalResistivity(double value);

    bool setThermalResistance(double value);

    bool setThermalTransmittance(double value);

    bool setThickness(double thickness);

    bool setThickness(const Quantity& thickness);

    bool setSolarIndexofRefraction(double solarIndexofRefraction);

    bool setSolarIndexofRefraction(const Quantity& solarIndexofRefraction);

    bool setSolarExtinctionCoefficient(double solarExtinctionCoefficient);

    bool setSolarExtinctionCoefficient(const Quantity& solarExtinctionCoefficient);

    bool setVisibleIndexofRefraction(double visibleIndexofRefraction);

    bool setVisibleIndexofRefraction(const Quantity& visibleIndexofRefraction);

    bool setVisibleExtinctionCoefficient(double visibleExtinctionCoefficient);

    bool setVisibleExtinctionCoefficient(const Quantity& visibleExtinctionCoefficient);

    bool setInfraredTransmittanceatNormalIncidence(double infraredTransmittanceatNormalIncidence);

    bool setInfraredTransmittanceatNormalIncidence(const Quantity& infraredTransmittanceatNormalIncidence);

    void resetInfraredTransmittanceatNormalIncidence();

    bool setInfraredHemisphericalEmissivity(double infraredHemisphericalEmissivity);

    bool setInfraredHemisphericalEmissivity(const Quantity& infraredHemisphericalEmissivity);

    void resetInfraredHemisphericalEmissivity();

    virtual bool setConductivity(double conductivity);

    virtual bool setConductivity(const Quantity& conductivity);

    virtual void resetConductivity();

    virtual bool setDirtCorrectionFactorforSolarandVisibleTransmittance(double dirtCorrectionFactorforSolarandVisibleTransmittance);

    virtual bool setDirtCorrectionFactorforSolarandVisibleTransmittance(const Quantity& dirtCorrectionFactorforSolarandVisibleTransmittance);

    virtual void resetDirtCorrectionFactorforSolarandVisibleTransmittance();

    void setSolarDiffusing(bool solarDiffusing);

    void resetSolarDiffusing();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.RefractionExtinctionGlazing");

    openstudio::Quantity thickness_SI() const;
    openstudio::Quantity thickness_IP() const;
    openstudio::Quantity solarIndexofRefraction_SI() const;
    openstudio::Quantity solarIndexofRefraction_IP() const;
    openstudio::Quantity solarExtinctionCoefficient_SI() const;
    openstudio::Quantity solarExtinctionCoefficient_IP() const;
    openstudio::Quantity visibleIndexofRefraction_SI() const;
    openstudio::Quantity visibleIndexofRefraction_IP() const;
    openstudio::Quantity visibleExtinctionCoefficient_SI() const;
    openstudio::Quantity visibleExtinctionCoefficient_IP() const;
    openstudio::Quantity infraredTransmittanceatNormalIncidence_SI() const;
    openstudio::Quantity infraredTransmittanceatNormalIncidence_IP() const;
    openstudio::Quantity infraredHemisphericalEmissivity_SI() const;
    openstudio::Quantity infraredHemisphericalEmissivity_IP() const;
    openstudio::Quantity conductivity_SI() const;
    openstudio::Quantity conductivity_IP() const;
    openstudio::Quantity dirtCorrectionFactorforSolarandVisibleTransmittance_SI() const;
    openstudio::Quantity dirtCorrectionFactorforSolarandVisibleTransmittance_IP() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_REFRACTIONEXTINCTIONGLAZING_IMPL_HPP

