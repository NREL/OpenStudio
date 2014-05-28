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

#ifndef MODEL_MODELPARTITIONMATERIAL_HPP
#define MODEL_MODELPARTITIONMATERIAL_HPP

#include "ModelAPI.hpp"
#include "Material.hpp"

namespace openstudio {
namespace model {

namespace detail{
  class ModelPartitionMaterial_Impl;
}

/** ModelPartitionMaterial is an abstract class derived from Material.
 *
 *  The ModelPartitionMaterial class is the base class for materials that should
 *  exist as the only layer in a Construction.  ModelPartitionMaterial objects 
 *  are typically used for the \link Construction Constructions\endlink of \link Surface Surfaces\endlink 
 *  that are virtual boundaries between two \link Space Spaces\endlink. */
class MODEL_API ModelPartitionMaterial : public Material {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~ModelPartitionMaterial() {}

  //@}
 protected:
  /// @cond 
  typedef detail::ModelPartitionMaterial_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class detail::ModelPartitionMaterial_Impl;

  ModelPartitionMaterial(IddObjectType type, const Model& model);

  explicit ModelPartitionMaterial(std::shared_ptr<detail::ModelPartitionMaterial_Impl> impl);  

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ModelPartitionMaterial");
};

/** \relates ModelPartitionMaterial */
typedef boost::optional<ModelPartitionMaterial> OptionalModelPartitionMaterial;

/** \relates ModelPartitionMaterial */
typedef std::vector<ModelPartitionMaterial> ModelPartitionMaterialVector;

} // model
} // openstudio

#endif // MODEL_MODELPARTITIONMATERIAL_HPP
