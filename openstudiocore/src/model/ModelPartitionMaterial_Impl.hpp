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

#ifndef MODEL_MODELPARTITIONMATERIAL_IMPL_HPP
#define MODEL_MODELPARTITIONMATERIAL_IMPL_HPP

#include "Material_Impl.hpp"

#include <QObject>

namespace openstudio {
namespace model {

// forward declaration
class ModelPartitionMaterial;

namespace detail {

  class MODEL_API ModelPartitionMaterial_Impl : public Material_Impl
  {
   public:

    /** @name Constructors and Destructors */
    //@{

    // Construct completely new object.
    ModelPartitionMaterial_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    // Construct from existing workspace object (typically when Model is being constructed 
    // from Workspace).
    ModelPartitionMaterial_Impl(const openstudio::detail::WorkspaceObject_Impl& other, 
                                Model_Impl* model, 
                                bool keepHandle);

    // Clone copy constructor.
    ModelPartitionMaterial_Impl(const ModelPartitionMaterial_Impl& other, 
                                Model_Impl* model, 
                                bool keepHandle);

    virtual ~ModelPartitionMaterial_Impl() {}

    //@}
    /** @name Getters */
    //@{

    /** Get the thickness of the material. Always returns 0.0. */
    virtual double thickness() const;

    /** The conductivitiy of the material in W/m*K. */
    virtual double thermalConductivity() const;

    /** The conductance of the material in W/m^2*K. */
    virtual double thermalConductance() const;

    /** The resistivity of the material in m*K/W. */
    virtual double thermalResistivity() const;

    /** The resistance of the material in m^2*K/W. */
    virtual double thermalResistance() const;

    virtual double thermalTransmittance() const;

    virtual double thermalAbsorptance() const;

    virtual double thermalReflectance() const;

    virtual double solarTransmittance() const;

    virtual double solarAbsorptance() const;

    virtual double solarReflectance() const;

    virtual boost::optional<double> getVisibleTransmittance() const;

    virtual double visibleAbsorptance() const;

    virtual double visibleReflectance() const;

    //@}
    /** @name Setters */
    //@{

    /** Set thickness to value (m). Always returns false. */
    virtual bool setThickness(double value);

    /** Sets the conductivity of the material in W/m*K, if possible. */
    virtual bool setThermalConductivity(double value);

    /** Sets the conductance of the material in W/m^2*K, if possible. */
    virtual bool setThermalConductance(double value);

    /** Sets the resistivity of the material in m*K/W, if possible. */
    virtual bool setThermalResistivity(double value);

    /** Sets the resistance of the material in m^2*K/W, if possible. */
    virtual bool setThermalResistance(double value);

    virtual bool setThermalTransmittance(double value);

    virtual bool setThermalAbsorptance(double value);

    virtual bool setThermalReflectance(double value);

    virtual bool setSolarTransmittance(double value);

    virtual bool setSolarAbsorptance(double value);

    virtual bool setSolarReflectance(double value);

    virtual bool setVisibleTransmittance(double value);

    virtual bool setVisibleAbsorptance(double value);

    virtual bool setVisibleReflectance(double value);

    //@}

   private:

    REGISTER_LOGGER("openstudio.model.ModelPartitionMaterial");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_MODELPARTITIONMATERIAL_IMPL_HPP
