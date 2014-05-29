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

#ifndef MODEL_SHADE_IMPL_HPP
#define MODEL_SHADE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ShadingMaterial_Impl.hpp"

#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

namespace openstudio {
namespace model {

namespace detail {

  /** Shade_Impl is a ShadingMaterial_Impl that is the implementation class for Shade.*/
  class MODEL_API Shade_Impl : public ShadingMaterial_Impl {
    Q_OBJECT;

    Q_PROPERTY(double solarTransmittance READ solarTransmittance WRITE setSolarTransmittance);
    Q_PROPERTY(openstudio::Quantity solarTransmittance_SI READ solarTransmittance_SI WRITE setSolarTransmittance);
    Q_PROPERTY(openstudio::Quantity solarTransmittance_IP READ solarTransmittance_IP WRITE setSolarTransmittance);

    Q_PROPERTY(double solarReflectance READ solarReflectance WRITE setSolarReflectance);
    Q_PROPERTY(openstudio::Quantity solarReflectance_SI READ solarReflectance_SI WRITE setSolarReflectance);
    Q_PROPERTY(openstudio::Quantity solarReflectance_IP READ solarReflectance_IP WRITE setSolarReflectance);

    Q_PROPERTY(double visibleTransmittance READ visibleTransmittance WRITE setVisibleTransmittance);
    Q_PROPERTY(openstudio::Quantity visibleTransmittance_SI READ visibleTransmittance_SI WRITE setVisibleTransmittance);
    Q_PROPERTY(openstudio::Quantity visibleTransmittance_IP READ visibleTransmittance_IP WRITE setVisibleTransmittance);

    Q_PROPERTY(double visibleReflectance READ visibleReflectance WRITE setVisibleReflectance);
    Q_PROPERTY(openstudio::Quantity visibleReflectance_SI READ visibleReflectance_SI WRITE setVisibleReflectance);
    Q_PROPERTY(openstudio::Quantity visibleReflectance_IP READ visibleReflectance_IP WRITE setVisibleReflectance);

    Q_PROPERTY(double thermalHemisphericalEmissivity READ thermalHemisphericalEmissivity WRITE setThermalHemisphericalEmissivity);
    Q_PROPERTY(openstudio::Quantity thermalHemisphericalEmissivity_SI READ thermalHemisphericalEmissivity_SI WRITE setThermalHemisphericalEmissivity);
    Q_PROPERTY(openstudio::Quantity thermalHemisphericalEmissivity_IP READ thermalHemisphericalEmissivity_IP WRITE setThermalHemisphericalEmissivity);

    Q_PROPERTY(double thermalTransmittance READ thermalTransmittance WRITE setThermalTransmittance);
    Q_PROPERTY(openstudio::Quantity thermalTransmittance_SI READ thermalTransmittance_SI WRITE setThermalTransmittance);
    Q_PROPERTY(openstudio::Quantity thermalTransmittance_IP READ thermalTransmittance_IP WRITE setThermalTransmittance);

    Q_PROPERTY(double thickness READ thickness WRITE setThickness);
    Q_PROPERTY(openstudio::Quantity thickness_SI READ thickness_SI WRITE setThickness);
    Q_PROPERTY(openstudio::Quantity thickness_IP READ thickness_IP WRITE setThickness);

    Q_PROPERTY(double conductivity READ conductivity WRITE setConductivity);
    Q_PROPERTY(openstudio::Quantity conductivity_SI READ conductivity_SI WRITE setConductivity);
    Q_PROPERTY(openstudio::Quantity conductivity_IP READ conductivity_IP WRITE setConductivity);

    Q_PROPERTY(double shadetoGlassDistance READ shadetoGlassDistance WRITE setShadetoGlassDistance RESET resetShadetoGlassDistance);
    Q_PROPERTY(openstudio::Quantity shadetoGlassDistance_SI READ shadetoGlassDistance_SI WRITE setShadetoGlassDistance RESET resetShadetoGlassDistance);
    Q_PROPERTY(openstudio::Quantity shadetoGlassDistance_IP READ shadetoGlassDistance_IP WRITE setShadetoGlassDistance RESET resetShadetoGlassDistance);
    Q_PROPERTY(bool isShadetoGlassDistanceDefaulted READ isShadetoGlassDistanceDefaulted);

    Q_PROPERTY(double topOpeningMultiplier READ topOpeningMultiplier WRITE setTopOpeningMultiplier RESET resetTopOpeningMultiplier);
    Q_PROPERTY(openstudio::Quantity topOpeningMultiplier_SI READ topOpeningMultiplier_SI WRITE setTopOpeningMultiplier RESET resetTopOpeningMultiplier);
    Q_PROPERTY(openstudio::Quantity topOpeningMultiplier_IP READ topOpeningMultiplier_IP WRITE setTopOpeningMultiplier RESET resetTopOpeningMultiplier);
    Q_PROPERTY(bool isTopOpeningMultiplierDefaulted READ isTopOpeningMultiplierDefaulted);

    Q_PROPERTY(double bottomOpeningMultiplier READ bottomOpeningMultiplier WRITE setBottomOpeningMultiplier RESET resetBottomOpeningMultiplier);
    Q_PROPERTY(openstudio::Quantity bottomOpeningMultiplier_SI READ bottomOpeningMultiplier_SI WRITE setBottomOpeningMultiplier RESET resetBottomOpeningMultiplier);
    Q_PROPERTY(openstudio::Quantity bottomOpeningMultiplier_IP READ bottomOpeningMultiplier_IP WRITE setBottomOpeningMultiplier RESET resetBottomOpeningMultiplier);
    Q_PROPERTY(bool isBottomOpeningMultiplierDefaulted READ isBottomOpeningMultiplierDefaulted);

    Q_PROPERTY(double leftSideOpeningMultiplier READ leftSideOpeningMultiplier WRITE setLeftSideOpeningMultiplier RESET resetLeftSideOpeningMultiplier);
    Q_PROPERTY(openstudio::Quantity leftSideOpeningMultiplier_SI READ leftSideOpeningMultiplier_SI WRITE setLeftSideOpeningMultiplier RESET resetLeftSideOpeningMultiplier);
    Q_PROPERTY(openstudio::Quantity leftSideOpeningMultiplier_IP READ leftSideOpeningMultiplier_IP WRITE setLeftSideOpeningMultiplier RESET resetLeftSideOpeningMultiplier);
    Q_PROPERTY(bool isLeftSideOpeningMultiplierDefaulted READ isLeftSideOpeningMultiplierDefaulted);

    Q_PROPERTY(double rightSideOpeningMultiplier READ rightSideOpeningMultiplier WRITE setRightSideOpeningMultiplier RESET resetRightSideOpeningMultiplier);
    Q_PROPERTY(openstudio::Quantity rightSideOpeningMultiplier_SI READ rightSideOpeningMultiplier_SI WRITE setRightSideOpeningMultiplier RESET resetRightSideOpeningMultiplier);
    Q_PROPERTY(openstudio::Quantity rightSideOpeningMultiplier_IP READ rightSideOpeningMultiplier_IP WRITE setRightSideOpeningMultiplier RESET resetRightSideOpeningMultiplier);
    Q_PROPERTY(bool isRightSideOpeningMultiplierDefaulted READ isRightSideOpeningMultiplierDefaulted);

    Q_PROPERTY(double airflowPermeability READ airflowPermeability WRITE setAirflowPermeability RESET resetAirflowPermeability);
    Q_PROPERTY(openstudio::Quantity airflowPermeability_SI READ airflowPermeability_SI WRITE setAirflowPermeability RESET resetAirflowPermeability);
    Q_PROPERTY(openstudio::Quantity airflowPermeability_IP READ airflowPermeability_IP WRITE setAirflowPermeability RESET resetAirflowPermeability);
    Q_PROPERTY(bool isAirflowPermeabilityDefaulted READ isAirflowPermeabilityDefaulted);

    // TODO: Add relationships for objects related to this one, but not pointed to by the underlying data.
    //       Such relationships can be generated by the GenerateRelationships.rb script.
   public:
    /** @name Constructors and Destructors */
    //@{

    Shade_Impl(const IdfObject& idfObject,
               Model_Impl* model,
               bool keepHandle);

    Shade_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
               Model_Impl* model,
               bool keepHandle);

    Shade_Impl(const Shade_Impl& other,
               Model_Impl* model,
               bool keepHandle);

    virtual ~Shade_Impl() {}

    //@}

    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
    /** @name Getters */
    //@{

    /** Get the thickness of the material. Note that the construction may need to add additional
     *  thickness for an air gap, depending on context. */
    virtual double thickness() const;

    /** The conductivitiy of the material in W/m*K. */
    double thermalConductivity() const;

    /** The conductance of the material in W/m^2*K. */
    double thermalConductance() const;

    /** The resistivity of the material in m*K/W. */
    double thermalResistivity() const;

    /** The resistance of the material in m^2*K/W. */
    double thermalResistance() const;

    double thermalTransmittance() const;

    double solarTransmittance() const;

    double solarAbsorptance() const;

    double solarReflectance() const;

    double visibleAbsorptance() const;

    virtual boost::optional<double> interiorVisibleAbsorptance() const;

    virtual boost::optional<double> exteriorVisibleAbsorptance() const;

    double visibleReflectance() const;

    Quantity getSolarTransmittance(bool returnIP=false) const;

    Quantity getSolarReflectance(bool returnIP=false) const;

    double visibleTransmittance() const;

    virtual boost::optional<double> getVisibleTransmittance() const;

    Quantity getVisibleTransmittance(bool returnIP) const;

    Quantity getVisibleReflectance(bool returnIP=false) const;

    double thermalHemisphericalEmissivity() const;

    Quantity getThermalHemisphericalEmissivity(bool returnIP=false) const;

    Quantity getThermalTransmittance(bool returnIP=false) const;

    Quantity getThickness(bool returnIP=false) const;

    double conductivity() const;

    Quantity getConductivity(bool returnIP=false) const;

    double shadetoGlassDistance() const;

    Quantity getShadetoGlassDistance(bool returnIP=false) const;

    bool isShadetoGlassDistanceDefaulted() const;

    double topOpeningMultiplier() const;

    Quantity getTopOpeningMultiplier(bool returnIP=false) const;

    bool isTopOpeningMultiplierDefaulted() const;

    double bottomOpeningMultiplier() const;

    Quantity getBottomOpeningMultiplier(bool returnIP=false) const;

    bool isBottomOpeningMultiplierDefaulted() const;

    double leftSideOpeningMultiplier() const;

    Quantity getLeftSideOpeningMultiplier(bool returnIP=false) const;

    bool isLeftSideOpeningMultiplierDefaulted() const;

    double rightSideOpeningMultiplier() const;

    Quantity getRightSideOpeningMultiplier(bool returnIP=false) const;

    bool isRightSideOpeningMultiplierDefaulted() const;

    double airflowPermeability() const;

    Quantity getAirflowPermeability(bool returnIP=false) const;

    bool isAirflowPermeabilityDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    /** Set thickness to value (m). */
    virtual bool setThickness(double value);

    /** Sets the conductivity of the material in W/m*K, if possible. */
    bool setThermalConductivity(double value);

    /** Sets the conductance of the material in W/m^2*K, if possible. */
    bool setThermalConductance(double value);

    /** Sets the resistivity of the material in m*K/W, if possible. */
    bool setThermalResistivity(double value);

    /** Sets the resistance of the material in m^2*K/W, if possible. */
    bool setThermalResistance(double value);

    bool setThermalTransmittance(double value);

    bool setSolarTransmittance(double value);

    bool setSolarReflectance(double value);

    bool setVisibleTransmittance(double value);

    bool setVisibleReflectance(double value);

    bool setSolarTransmittance(const Quantity& solarTransmittance);

    bool setSolarReflectance(const Quantity& solarReflectance);

    bool setVisibleTransmittance(const Quantity& visibleTransmittance);

    bool setVisibleReflectance(const Quantity& visibleReflectance);

    bool setThermalHemisphericalEmissivity(double thermalHemisphericalEmissivity);

    bool setThermalHemisphericalEmissivity(const Quantity& thermalHemisphericalEmissivity);

    bool setThermalTransmittance(const Quantity& thermalTransmittance);

    bool setThickness(const Quantity& thickness);

    bool setConductivity(double conductivity);

    bool setConductivity(const Quantity& conductivity);

    bool setShadetoGlassDistance(double shadetoGlassDistance);

    bool setShadetoGlassDistance(const Quantity& shadetoGlassDistance);

    void resetShadetoGlassDistance();

    bool setTopOpeningMultiplier(double topOpeningMultiplier);

    bool setTopOpeningMultiplier(const Quantity& topOpeningMultiplier);

    void resetTopOpeningMultiplier();

    bool setBottomOpeningMultiplier(double bottomOpeningMultiplier);

    bool setBottomOpeningMultiplier(const Quantity& bottomOpeningMultiplier);

    void resetBottomOpeningMultiplier();

    bool setLeftSideOpeningMultiplier(double leftSideOpeningMultiplier);

    bool setLeftSideOpeningMultiplier(const Quantity& leftSideOpeningMultiplier);

    void resetLeftSideOpeningMultiplier();

    bool setRightSideOpeningMultiplier(double rightSideOpeningMultiplier);

    bool setRightSideOpeningMultiplier(const Quantity& rightSideOpeningMultiplier);

    void resetRightSideOpeningMultiplier();

    bool setAirflowPermeability(double airflowPermeability);

    bool setAirflowPermeability(const Quantity& airflowPermeability);

    void resetAirflowPermeability();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.Shade");

    openstudio::Quantity solarTransmittance_SI() const;
    openstudio::Quantity solarTransmittance_IP() const;
    openstudio::Quantity solarReflectance_SI() const;
    openstudio::Quantity solarReflectance_IP() const;
    openstudio::Quantity visibleTransmittance_SI() const;
    openstudio::Quantity visibleTransmittance_IP() const;
    openstudio::Quantity visibleReflectance_SI() const;
    openstudio::Quantity visibleReflectance_IP() const;
    openstudio::Quantity thermalHemisphericalEmissivity_SI() const;
    openstudio::Quantity thermalHemisphericalEmissivity_IP() const;
    openstudio::Quantity thermalTransmittance_SI() const;
    openstudio::Quantity thermalTransmittance_IP() const;
    openstudio::Quantity thickness_SI() const;
    openstudio::Quantity thickness_IP() const;
    openstudio::Quantity conductivity_SI() const;
    openstudio::Quantity conductivity_IP() const;
    openstudio::Quantity shadetoGlassDistance_SI() const;
    openstudio::Quantity shadetoGlassDistance_IP() const;
    openstudio::Quantity topOpeningMultiplier_SI() const;
    openstudio::Quantity topOpeningMultiplier_IP() const;
    openstudio::Quantity bottomOpeningMultiplier_SI() const;
    openstudio::Quantity bottomOpeningMultiplier_IP() const;
    openstudio::Quantity leftSideOpeningMultiplier_SI() const;
    openstudio::Quantity leftSideOpeningMultiplier_IP() const;
    openstudio::Quantity rightSideOpeningMultiplier_SI() const;
    openstudio::Quantity rightSideOpeningMultiplier_IP() const;
    openstudio::Quantity airflowPermeability_SI() const;
    openstudio::Quantity airflowPermeability_IP() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SHADE_IMPL_HPP

