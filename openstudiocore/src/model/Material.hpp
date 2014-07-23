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

#ifndef MODEL_MATERIAL_HPP
#define MODEL_MATERIAL_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

namespace openstudio {
namespace model {

namespace detail{
  class Material_Impl;
}

/** A Material is a ResourceObject that serves as a base class for all objects that can be used
 *  in \link LayeredConstruction LayeredConstructions \endlink. It also provides Attributes for 
 *  'thickness', 'getVisibleTransmiattance', 'interiorVisibleAbsorptance', and 
 *  'exteriorVisibleAbsorptance'. */
class MODEL_API Material : public ResourceObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~Material() {}

  //@}
  /** @name Getters */
  //@{

  /** Get the thickness of the material. Virtual implementation. For some materials, 0.0 is always 
   *  returned. 
   *
   *  Attribute Name: 'thickness' */
  double thickness() const;

  /** Get the visible transmittance of the material. Virtual implementation.
   *
   *  Attribute Name: 'getVisibleTransmittance' */
  boost::optional<double> getVisibleTransmittance() const;

  /** Get the interiorVisibleAbsorptance of the material. Virtual implementation. 
   *
   *  Attribute Name: 'interiorVisibleAbsorptance' */
  boost::optional<double> interiorVisibleAbsorptance() const;

  /** Get the exteriorVisibleAbsorptance of the material. Virtual implementation. 
   *
   *  Attribute Name: 'exteriorVisibleAbsorptance' */
  boost::optional<double> exteriorVisibleAbsorptance() const;

  //@}
  /** @name Setters */
  //@{

  /** Set thickness to value (m). For some materials, false is always returned. 
   *
   *  Attribute Name: 'thickness' */
  bool setThickness(double value);

  //@}
 protected:
  /// @cond
  typedef detail::Material_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class detail::Material_Impl;

  Material(IddObjectType type,const Model& model);

  explicit Material(std::shared_ptr<detail::Material_Impl> impl);  

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.Material");
};

/** \relates Material */
typedef boost::optional<Material> OptionalMaterial;

/** \relates Material */
typedef std::vector<Material> MaterialVector;

} // model
} // openstudio

#endif // MODEL_MATERIAL_HPP
