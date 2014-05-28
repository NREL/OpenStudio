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

#ifndef MODEL_CONSTRUCTION_HPP
#define MODEL_CONSTRUCTION_HPP

#include "ModelAPI.hpp"
#include "LayeredConstruction.hpp"

namespace openstudio {
namespace model {

namespace detail{
  class Construction_Impl;
}

/** Construction is a LayeredConstruction that wraps the EnergyPlus IDD object 'Construction'.
 *  A Construction consists of a number of material layers, each of which should belong to the 
 *  same base class: OpaqueMaterial, FenestrationMaterial, or ModelPartitionMaterial. In addition
 *  the layers must satisfy some restrictions in order to form a valid EnergyPlus construction,
 *  see the documentation for LayeredConstruction. */
class MODEL_API Construction : public LayeredConstruction {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit Construction(const Model& model);

  explicit Construction(const std::vector<OpaqueMaterial>& opaqueMaterials);

  explicit Construction(const std::vector<FenestrationMaterial>& fenestrationMaterials);

  explicit Construction(const ModelPartitionMaterial& modelPartitionMaterial);

  virtual ~Construction() {}

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  //@}

  /** Searches the model for a layered construction with layers which are the reverse of this one.  Returns that construction 
   *  if found, creates one if not. */
  Construction reverseConstruction() const;

 protected:
  /// @cond
  typedef detail::Construction_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  friend class detail::Construction_Impl;

  explicit Construction(std::shared_ptr<detail::Construction_Impl> impl);  

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.Construction");
};

/** \relates Construction */
typedef boost::optional<Construction> OptionalConstruction;

/** \relates Construction */
typedef std::vector<Construction> ConstructionVector;

} // model
} // openstudio

#endif // MODEL_CONSTRUCTION_HPP
