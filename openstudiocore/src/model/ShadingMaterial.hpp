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

#ifndef MODEL_SHADINGMATERIAL_HPP
#define MODEL_SHADINGMATERIAL_HPP

#include "ModelAPI.hpp"
#include "FenestrationMaterial.hpp"

namespace openstudio {
namespace model {

namespace detail{
  class ShadingMaterial_Impl;
}

/** ShadingMaterial is a FenestrationMaterial that serves as a base class for objects like Blind,
 *  Screen, and Shade. */
class MODEL_API ShadingMaterial : public FenestrationMaterial {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~ShadingMaterial() {}

  //@}
 protected:
  /// @cond
  typedef detail::ShadingMaterial_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class detail::ShadingMaterial_Impl;

  ShadingMaterial(IddObjectType type,const Model& model);

  explicit ShadingMaterial(std::shared_ptr<detail::ShadingMaterial_Impl> impl);  

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ShadingMaterial");
};

/** \relates ShadingMaterial */
typedef boost::optional<ShadingMaterial> OptionalShadingMaterial;

/** \relates ShadingMaterial */
typedef std::vector<ShadingMaterial> ShadingMaterialVector;

} // model
} // openstudio

#endif // MODEL_SHADINGMATERIAL_HPP
