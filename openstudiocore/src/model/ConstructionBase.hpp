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

#ifndef MODEL_CONSTRUCTIONBASE_HPP
#define MODEL_CONSTRUCTIONBASE_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

namespace openstudio {
namespace model {

class StandardsInformationConstruction;
class RenderingColor;

namespace detail{
  class ConstructionBase_Impl;
}

/** ConstructionBase is a ResourceObject that serves as a base class for several objects that can
 *  be used to specify a construction for a PlanarSurface. */
class MODEL_API ConstructionBase : public ResourceObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~ConstructionBase() {}

  //@}
  /** @name Getters */
  //@{

  /** Returns this construction's standards information, constructing a new object if necessary. */
  StandardsInformationConstruction standardsInformation() const;

  //@}

  /** Returns the sum of the surface area of all planer surfaces that use this construction (m^2). 
   *  If this construction is used by a Surface that contains one or more \link SubSurface 
   *  SubSurfaces \endlink, than the SubSurface areas are subtracted out of the total. */
  double getNetArea() const;

  /** Set the u-factor of this surface in W/m^2*K, if possible. value should already include
   *  appropriate film coefficients. By default, assumes still air indoors and 15 mph outdoor air
   *  speed. */
  bool setUFactor(double value);

  /** Set the u-factor of this surface in W/m^2*K, if possible. filmResistance (m^2*K/W) may be
   *  used to convert value to a conductance. */
  bool setUFactor(double value,double filmResistance);

  /** Set the conductance of this surface in W/m^2*K, if possible. value should not include any film
   *  coefficients. */
  bool setConductance(double value);

  /** Set the conductance of this surface in W/m^2*K, if possible. filmResistance (m^2*K/W) may be
   *  used to convert value to a u-factor. */
  bool setConductance(double value,double filmResistance);

  bool isOpaque() const;

  bool isFenestration() const;

  /** Returns true if isFenestration and any material layers are solar diffusing. */
  bool isSolarDiffusing() const;

  bool isModelPartition() const;

  /** Returns true if the construction has RoofVegetation as the outer layer. */
  bool isGreenRoof() const;

  /** Get the u-factor of this construction (W/m^2*K). Includes film coefficients. */
  boost::optional<double> uFactor() const;

  /** Get the u-factor of this construction (W/m^2*K). Includes filmResistance. */
  boost::optional<double> uFactor(double filmResistance) const;

  /** Get the thermal conductance of this construction (W/m^2*K). Does not include film 
   *  coefficients. */
  boost::optional<double> thermalConductance() const;

  /** Get the thermal conductance of this construction (W/m^2*K). filmResistance is subtracted out 
   *  of the thermalResistance if necessary. */
  boost::optional<double> thermalConductance(double filmResistance) const;

  /** Get the heat capacity of this construction (J/m^2*K). Only works for
   *  \link LayeredConstruction LayeredConstructions \endlink of \link StandardOpaqueMaterial
   *  StandardOpaqueMaterials \endlink. 
   *
   *  Attribute Name: 'heatCapacity' */
  boost::optional<double> heatCapacity() const;

  /** Get the visible absorptance of the interior side of this construction (dimensionless). May 
   *  require simulation ('EnvelopeSummary' table). 
   *
   *  Attribute Name: 'interiorVisibleAbsorptance' */
  boost::optional<double> interiorVisibleAbsorptance() const;

  /** \deprecated 
   *   This will be removed after 1.4.0 */
  boost::optional<double> interiorVisibleAbsorbtance() const;

  /** Get the visible absorptance of the exterior side of this construction (dimensionless). May 
   *  require simulation ('EnvelopeSummary' table). 
   *
   *  Attribute Name: 'exteriorVisibleAbsorptance' */
  boost::optional<double> exteriorVisibleAbsorptance() const;

  /** \deprecated 
   *   This will be removed after 1.4.0 */
  boost::optional<double> exteriorVisibleAbsorbtance() const;

  /** Get the visible transmittance of this construction (dimensionless). May require simulation 
   *  ('EnvelopeSummary' table). 
   *
   *  Attribute Name: 'visibleTransmittance' */
  boost::optional<double> visibleTransmittance() const;

  /// Returns the rendering color.
  boost::optional<RenderingColor> renderingColor() const;

  /// Sets the rendering color.
  bool setRenderingColor(const RenderingColor& renderingColor);

  /// Resets the rendering color.
  void resetRenderingColor();

 protected:
  /// @cond
  typedef detail::ConstructionBase_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  friend class detail::ConstructionBase_Impl;

  ConstructionBase(IddObjectType type,const Model& model);

  explicit ConstructionBase(std::shared_ptr<detail::ConstructionBase_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ConstructionBase");
};

/** \relates ConstructionBase */
typedef boost::optional<ConstructionBase> OptionalConstructionBase;

/** \relates ConstructionBase */
typedef std::vector<ConstructionBase> ConstructionBaseVector;

} // model
} // openstudio

#endif // MODEL_CONSTRUCTIONBASE_HPP
