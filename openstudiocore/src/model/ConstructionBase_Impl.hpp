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

#ifndef MODEL_CONSTRUCTIONBASE_IMPL_HPP
#define MODEL_CONSTRUCTIONBASE_IMPL_HPP

#include <model/ResourceObject_Impl.hpp>

#include <QObject>

namespace openstudio {
namespace model {

// forward declaration
class StandardsInformationConstruction;
class RenderingColor;

namespace detail {

  class MODEL_API ConstructionBase_Impl : public ResourceObject_Impl
  {
    Q_OBJECT;
    Q_PROPERTY(boost::optional<double> heatCapacity READ heatCapacity);
    // may require simulation results.
    Q_PROPERTY(boost::optional<double> interiorVisibleAbsorbtance READ interiorVisibleAbsorbtance);
    // may require simulation results.
    Q_PROPERTY(boost::optional<double> exteriorVisibleAbsorbtance READ exteriorVisibleAbsorbtance);
    // may require simulation results.
    Q_PROPERTY(boost::optional<double> visibleTransmittance READ visibleTransmittance);
  public:

    /** @name Constructors and Destructors */
    //@{

    // Construct completely new object.
    ConstructionBase_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    // Construct from existing workspace object (typically when Model is being constructed
    // from Workspace).
    ConstructionBase_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                          Model_Impl* model,
                          bool keepHandle);

    // Clone copy constructor.
    ConstructionBase_Impl(const ConstructionBase_Impl& other, Model_Impl* model, bool keepHandle);

    virtual ~ConstructionBase_Impl() {}

    //@}
    /** @name Getters */
    //@{

    /** Returns this construction's standards information, constructing a new object if necessary. */
    StandardsInformationConstruction standardsInformation() const;

    virtual double getNetArea() const;

    // return any children objects in the hierarchy
    virtual std::vector<ModelObject> children() const;

    /// get a vector of allowable children types
    virtual std::vector<IddObjectType> allowableChildTypes() const;

    //@}
    /** @name Setters */
    //@{

    /** Set the u-factor of this surface in W/m^2*K, if possible. value should already include
     *  appropriate film coefficients. By default, assumes still air indoors and 15 mph outdoor air
     *  speed. */
    virtual bool setUFactor(double value);

    /** Set the u-factor of this surface in W/m^2*K, if possible. filmResistance (m^2*K/W) may be
     *  used to convert value to a conductance. */
    virtual bool setUFactor(double value,double filmResistance);

    /** Set the conductance of this surface in W/m^2*K, if possible. value should not include any film
     *  coefficients. */
    virtual bool setConductance(double value);

    /** Set the conductance of this surface in W/m^2*K, if possible. filmResistance (m^2*K/W) may be
     *  used to convert value to a u-factor. */
    virtual bool setConductance(double value,double filmResistance);

    //@}
    /** @name Queries */
    //@{

    virtual bool isOpaque() const=0;

    virtual bool isFenestration() const=0;

    virtual bool isSolarDiffusing() const=0;

    virtual bool isModelPartition() const=0;

    /** Returns true if the construction has RoofVegetation as the outer layer. */
    virtual bool isGreenRoof() const;

    /** Get the u-factor of this construction. Includes film coefficients. */
    virtual boost::optional<double> uFactor() const;

    /** Get the u-factor of this construction. Includes filmResistance. */
    virtual boost::optional<double> uFactor(double filmResistance) const;

    /** Get the thermal conductance of this construction. Does not include film coefficients. */
    virtual boost::optional<double> thermalConductance() const;

    /** Get the thermal conductance of this construction. filmResistance is subtracted out of the
     *  thermalResistance if necessary. */
    virtual boost::optional<double> thermalConductance(double filmResistance) const;

    /** Get the heat capacity of this construction (J/m^2*K). Only works for
     *  \link LayeredConstruction LayeredConstructions \endlink of \link StandardOpaqueMaterial
     *  StandardOpaqueMaterials \endlink. */
    virtual boost::optional<double> heatCapacity() const;

    /// get interior visible absorbtance (unitless)
    virtual boost::optional<double> interiorVisibleAbsorbtance() const;

    /// get exterior visible absorbtance (unitless)
    virtual boost::optional<double> exteriorVisibleAbsorbtance() const;

    /// get visible transmittance (unitless)
    virtual boost::optional<double> visibleTransmittance() const;
    
    /// Returns the rendering color.
    virtual boost::optional<RenderingColor> renderingColor() const;

    /// Sets the rendering color.
    virtual bool setRenderingColor(const RenderingColor& renderingColor);

    /// Resets the rendering color.
    virtual void resetRenderingColor();

    /// Resets the rendering color.
    virtual int renderingColorIndex() const = 0;

    //@}

   private:

    REGISTER_LOGGER("openstudio.model.ConstructionBase");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_CONSTRUCTIONBASE_IMPL_HPP
