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

#ifndef MODEL_OPAQUEMATERIAL_IMPL_HPP
#define MODEL_OPAQUEMATERIAL_IMPL_HPP

#include "Material_Impl.hpp"

#include <QObject>

namespace openstudio {
namespace model {

// forward declaration
class OpaqueMaterial;

namespace detail {

  class MODEL_API OpaqueMaterial_Impl : public Material_Impl {
    Q_OBJECT;

    Q_PROPERTY(double thermalConductivity READ thermalConductivity WRITE setThermalConductivity);
    Q_PROPERTY(double thermalConductance READ thermalConductance WRITE setThermalConductance);
    Q_PROPERTY(double thermalResistivity READ thermalResistivity WRITE setThermalResistivity);
    Q_PROPERTY(double thermalResistance READ thermalResistance WRITE setThermalResistance);

    Q_PROPERTY(boost::optional<double> heatCapacity READ heatCapacity);

    Q_PROPERTY(double thermalAbsorptance READ thermalAbsorptance WRITE setThermalAbsorptance);
    Q_PROPERTY(boost::optional<double> thermalReflectance READ thermalReflectance WRITE setThermalReflectance);
    Q_PROPERTY(double solarAbsorptance READ solarAbsorptance WRITE setSolarAbsorptance);
    Q_PROPERTY(boost::optional<double> solarReflectance READ solarReflectance WRITE setSolarReflectance);
    Q_PROPERTY(double visibleAbsorptance READ visibleAbsorptance WRITE setVisibleAbsorptance);
    Q_PROPERTY(boost::optional<double> visibleReflectance READ visibleReflectance WRITE setVisibleReflectance);

   public:

    // Construct completely new object.
    OpaqueMaterial_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    // Construct from existing workspace object (typically when Model is being constructed 
    // from Workspace).
    OpaqueMaterial_Impl(const openstudio::detail::WorkspaceObject_Impl& other, 
                        Model_Impl* model, 
                        bool keepHandle);

    // Clone copy constructor.
    OpaqueMaterial_Impl(const OpaqueMaterial_Impl& other, Model_Impl* model, bool keepHandle);

    virtual ~OpaqueMaterial_Impl() {}

    /** @name Getters */
    //@{

    /** The conductivitiy of the material in W/m*K. */
    virtual double thermalConductivity() const = 0;

    /** The conductance of the material in W/m^2*K. */
    virtual double thermalConductance() const = 0;

    /** The resistivity of the material in m*K/W. */
    virtual double thermalResistivity() const = 0;

    /** The resistance of the material in m^2*K/W. */
    virtual double thermalResistance() const = 0;

    virtual double thermalAbsorptance() const = 0;

    virtual boost::optional<double> thermalReflectance() const = 0;

    virtual double solarAbsorptance() const = 0;

    virtual boost::optional<double> solarReflectance() const = 0;

    virtual boost::optional<double> getVisibleTransmittance() const;

    virtual double visibleAbsorptance() const = 0;

    virtual boost::optional<double> interiorVisibleAbsorptance() const;

    virtual boost::optional<double> exteriorVisibleAbsorptance() const;

    virtual boost::optional<double> visibleReflectance() const = 0;

    //@}
    /** @name Setters */
    //@{

    /** Sets the conductivity of the material in W/m*K, if possible. */
    virtual bool setThermalConductivity(double value) = 0;

    /** Sets the conductance of the material in W/m^2*K, if possible. */
    virtual bool setThermalConductance(double value) = 0;

    /** Sets the resistivity of the material in m*K/W, if possible. */
    virtual bool setThermalResistivity(double value) = 0;

    /** Sets the resistance of the material in m^2*K/W, if possible. */
    virtual bool setThermalResistance(double value) = 0;

    virtual bool setThermalAbsorptance(double value) = 0;

    virtual bool setThermalReflectance(boost::optional<double> value) = 0;

    virtual bool setSolarAbsorptance(boost::optional<double> value) = 0;

    virtual bool setSolarReflectance(boost::optional<double> value) = 0;

    virtual bool setVisibleAbsorptance(boost::optional<double> value) = 0;

    virtual bool setVisibleReflectance(boost::optional<double> value) = 0;

    //@}
    /** @name Queries */
    //@{

    /** Energy it takes to raise the temperature of this material 1 K (J/m^2*K). Equal to 
     *  specificHeat() * density() * thickness(). */
    virtual boost::optional<double> heatCapacity() const;

    //@}

   private:

    REGISTER_LOGGER("openstudio.model.OpaqueMaterial");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_OPAQUEMATERIAL_IMPL_HPP
