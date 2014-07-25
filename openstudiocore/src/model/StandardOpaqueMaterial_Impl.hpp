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

#ifndef MODEL_STANDARDOPAQUEMATERIAL_IMPL_HPP
#define MODEL_STANDARDOPAQUEMATERIAL_IMPL_HPP

#include "ModelAPI.hpp"
#include "OpaqueMaterial_Impl.hpp"

#include <QObject>
#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

namespace openstudio {
namespace model {

// forward declaration
class StandardOpaqueMaterial;

namespace detail {

  /** StandardOpaqueMaterial_Impl is a OpaqueMaterial_Impl that is the implementation class for StandardOpaqueMaterial.*/
  class MODEL_API StandardOpaqueMaterial_Impl : public OpaqueMaterial_Impl {
    Q_OBJECT;

    Q_PROPERTY(double thermalConductivity READ thermalConductivity WRITE setThermalConductivity);
    Q_PROPERTY(double thermalConductance READ thermalConductance WRITE setThermalConductance);
    Q_PROPERTY(double thermalResistivity READ thermalResistivity WRITE setThermalResistivity);
    Q_PROPERTY(double thermalResistance READ thermalResistance WRITE setThermalResistance);

    Q_PROPERTY(boost::optional<double> heatCapacity READ heatCapacity);

    Q_PROPERTY(boost::optional<double> thermalReflectance READ thermalReflectance WRITE setThermalReflectance);
    Q_PROPERTY(boost::optional<double> solarReflectance READ solarReflectance WRITE setSolarReflectance);
    Q_PROPERTY(boost::optional<double> visibleReflectance READ visibleReflectance WRITE setVisibleReflectance);

    Q_PROPERTY(std::string roughness READ roughness WRITE setRoughness);
    Q_PROPERTY(std::vector<std::string> roughnessValues READ roughnessValues);

    Q_PROPERTY(double thickness READ thickness WRITE setThickness);
    Q_PROPERTY(openstudio::Quantity thickness_SI READ thickness_SI WRITE setThickness);
    Q_PROPERTY(openstudio::Quantity thickness_IP READ thickness_IP WRITE setThickness);

    Q_PROPERTY(double conductivity READ conductivity WRITE setConductivity);
    Q_PROPERTY(openstudio::Quantity conductivity_SI READ conductivity_SI WRITE setConductivity);
    Q_PROPERTY(openstudio::Quantity conductivity_IP READ conductivity_IP WRITE setConductivity);

    Q_PROPERTY(double density READ density WRITE setDensity);
    Q_PROPERTY(openstudio::Quantity density_SI READ density_SI WRITE setDensity);
    Q_PROPERTY(openstudio::Quantity density_IP READ density_IP WRITE setDensity);

    Q_PROPERTY(double specificHeat READ specificHeat WRITE setSpecificHeat);
    Q_PROPERTY(openstudio::Quantity specificHeat_SI READ specificHeat_SI WRITE setSpecificHeat);
    Q_PROPERTY(openstudio::Quantity specificHeat_IP READ specificHeat_IP WRITE setSpecificHeat);

    Q_PROPERTY(double thermalAbsorptance READ thermalAbsorptance WRITE setThermalAbsorptance RESET resetThermalAbsorptance);
    Q_PROPERTY(openstudio::Quantity thermalAbsorptance_SI READ thermalAbsorptance_SI WRITE setThermalAbsorptance RESET resetThermalAbsorptance);
    Q_PROPERTY(openstudio::Quantity thermalAbsorptance_IP READ thermalAbsorptance_IP WRITE setThermalAbsorptance RESET resetThermalAbsorptance);
    Q_PROPERTY(bool isThermalAbsorptanceDefaulted READ isThermalAbsorptanceDefaulted);

    Q_PROPERTY(double solarAbsorptance READ solarAbsorptance WRITE setSolarAbsorptance RESET resetSolarAbsorptance);
    Q_PROPERTY(openstudio::Quantity solarAbsorptance_SI READ solarAbsorptance_SI WRITE setSolarAbsorptance RESET resetSolarAbsorptance);
    Q_PROPERTY(openstudio::Quantity solarAbsorptance_IP READ solarAbsorptance_IP WRITE setSolarAbsorptance RESET resetSolarAbsorptance);
    Q_PROPERTY(bool isSolarAbsorptanceDefaulted READ isSolarAbsorptanceDefaulted);

    Q_PROPERTY(double visibleAbsorptance READ visibleAbsorptance WRITE setVisibleAbsorptance RESET resetVisibleAbsorptance);
    Q_PROPERTY(openstudio::Quantity visibleAbsorptance_SI READ visibleAbsorptance_SI WRITE setVisibleAbsorptance RESET resetVisibleAbsorptance);
    Q_PROPERTY(openstudio::Quantity visibleAbsorptance_IP READ visibleAbsorptance_IP WRITE setVisibleAbsorptance RESET resetVisibleAbsorptance);
    Q_PROPERTY(bool isVisibleAbsorptanceDefaulted READ isVisibleAbsorptanceDefaulted);

    // TODO: Add relationships for objects related to this one, but not pointed to by the underlying data.
    //       Such relationships can be generated by the GenerateRelationships.rb script.
   public:
    /** @name Constructors and Destructors */
    //@{

    StandardOpaqueMaterial_Impl(const IdfObject& idfObject,
                  Model_Impl* model,
                  bool keepHandle);

    StandardOpaqueMaterial_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                  Model_Impl* model,
                  bool keepHandle);

    StandardOpaqueMaterial_Impl(const StandardOpaqueMaterial_Impl& other,
                  Model_Impl* model,
                  bool keepHandle);

    virtual ~StandardOpaqueMaterial_Impl() {}

    //@}

    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
    /** @name Getters */
    //@{

    /** The conductivitiy of the material in W/m*K. */
    virtual double thermalConductivity() const;

    /** The conductance of the material in W/m^2*K. */
    virtual double thermalConductance() const;

    /** The resistivity of the material in m*K/W. */
    virtual double thermalResistivity() const;

    /** The resistance of the material in m^2*K/W. */
    virtual double thermalResistance() const;

    virtual boost::optional<double> thermalReflectance() const;

    virtual boost::optional<double> solarReflectance() const;

    virtual boost::optional<double> visibleReflectance() const;

    virtual boost::optional<double> getVisibleTransmittance() const;

    virtual boost::optional<double> interiorVisibleAbsorptance() const;

    virtual boost::optional<double> exteriorVisibleAbsorptance() const;

    virtual std::string roughness() const;

    virtual double thickness() const;

    virtual Quantity getThickness(bool returnIP) const;

    virtual double conductivity() const;

    virtual Quantity getConductivity(bool returnIP) const;

    virtual double density() const;

    virtual Quantity getDensity(bool returnIP) const;

    virtual double specificHeat() const;

    virtual Quantity getSpecificHeat(bool returnIP) const;

    virtual double thermalAbsorptance() const;

    virtual Quantity getThermalAbsorptance(bool returnIP) const;

    virtual double solarAbsorptance() const;

    virtual Quantity getSolarAbsorptance(bool returnIP=false) const;

    virtual bool isSolarAbsorptanceDefaulted() const;

    virtual double visibleAbsorptance() const;

    virtual Quantity getVisibleAbsorptance(bool returnIP=false) const;

    virtual bool isVisibleAbsorptanceDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    /** Sets the conductivity of the material in W/m*K, if possible. */
    virtual bool setThermalConductivity(double value);

    /** Sets the conductance of the material in W/m^2*K, if possible. */
    virtual bool setThermalConductance(double value);

    /** Sets the resistivity of the material in m*K/W, if possible. */
    virtual bool setThermalResistivity(double value);

    /** Sets the resistance of the material in m^2*K/W, if possible. */
    virtual bool setThermalResistance(double value);

    virtual bool setThermalReflectance(boost::optional<double> value);

    virtual bool setSolarAbsorptance(boost::optional<double> value);

    virtual bool setSolarReflectance(boost::optional<double> value);

    virtual bool setVisibleAbsorptance(boost::optional<double> value);

    virtual bool setVisibleReflectance(boost::optional<double> value);

    virtual bool setRoughness(std::string roughness);

    virtual bool setThickness(double thickness);

    virtual bool setThickness(const Quantity& thickness);

    virtual bool setConductivity(double conductivity);

    virtual bool setConductivity(const Quantity& conductivity);

    virtual bool setDensity(double density);

    virtual bool setDensity(const Quantity& density);

    virtual bool setSpecificHeat(double specificHeat);

    virtual bool setSpecificHeat(const Quantity& specificHeat);

    virtual bool setThermalAbsorptance(double thermalAbsorptance);

    virtual bool setThermalAbsorptance(const Quantity& thermalAbsorptance);

    virtual void resetThermalAbsorptance();

    virtual bool isThermalAbsorptanceDefaulted() const;

    virtual bool setSolarAbsorptance(double solarAbsorptance);

    virtual bool setSolarAbsorptance(const Quantity& solarAbsorptance);

    virtual void resetSolarAbsorptance();

    virtual bool setVisibleAbsorptance(double visibleAbsorptance);

    virtual bool setVisibleAbsorptance(const Quantity& visibleAbsorptance);

    virtual void resetVisibleAbsorptance();

    //@}
    /** @name Queries */
    //@{

    /** Energy it takes to raise the temperature of this material 1 K (J/m^2*K). Equal to 
     *  specificHeat() * density() * thickness(). */
    virtual boost::optional<double> heatCapacity() const;

    //@}

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.StandardOpaqueMaterial");

    std::vector<std::string> roughnessValues() const;
    openstudio::Quantity thickness_SI() const;
    openstudio::Quantity thickness_IP() const;
    openstudio::Quantity conductivity_SI() const;
    openstudio::Quantity conductivity_IP() const;
    openstudio::Quantity density_SI() const;
    openstudio::Quantity density_IP() const;
    openstudio::Quantity specificHeat_SI() const;
    openstudio::Quantity specificHeat_IP() const;
    openstudio::Quantity thermalAbsorptance_SI() const;
    openstudio::Quantity thermalAbsorptance_IP() const;
    openstudio::Quantity solarAbsorptance_SI() const;
    openstudio::Quantity solarAbsorptance_IP() const;
    openstudio::Quantity visibleAbsorptance_SI() const;
    openstudio::Quantity visibleAbsorptance_IP() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_STANDARDOPAQUEMATERIAL_IMPL_HPP

