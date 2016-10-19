/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#ifndef MODEL_ENERGYMANAGEMENTSYSTEMCONSTRUCTIONINDEXVARIABLE_HPP
#define MODEL_ENERGYMANAGEMENTSYSTEMCONSTRUCTIONINDEXVARIABLE_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
class Construction;

namespace detail {

  class EnergyManagementSystemConstructionIndexVariable_Impl;

} // detail

/** EnergyManagementSystemConstructionIndexVariable is a ModelObject that wraps the OpenStudio IDD object 'OS:EnergyManagementSystem:ConstructionIndexVariable'. */
class MODEL_API EnergyManagementSystemConstructionIndexVariable : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit EnergyManagementSystemConstructionIndexVariable(const Model& model, const Construction& construction);

  explicit EnergyManagementSystemConstructionIndexVariable(const Model& model);

  virtual ~EnergyManagementSystemConstructionIndexVariable() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  ModelObject constructionObject() const;

  //@}
  /** @name Setters */
  //@{

  bool setConstructionObject(const ModelObject& construction);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::EnergyManagementSystemConstructionIndexVariable_Impl ImplType;

  explicit EnergyManagementSystemConstructionIndexVariable(std::shared_ptr<detail::EnergyManagementSystemConstructionIndexVariable_Impl> impl);

  friend class detail::EnergyManagementSystemConstructionIndexVariable_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.EnergyManagementSystemConstructionIndexVariable");
};

/** \relates EnergyManagementSystemConstructionIndexVariable*/
typedef boost::optional<EnergyManagementSystemConstructionIndexVariable> OptionalEnergyManagementSystemConstructionIndexVariable;

/** \relates EnergyManagementSystemConstructionIndexVariable*/
typedef std::vector<EnergyManagementSystemConstructionIndexVariable> EnergyManagementSystemConstructionIndexVariableVector;

} // model
} // openstudio

#endif // MODEL_ENERGYMANAGEMENTSYSTEMCONSTRUCTIONINDEXVARIABLE_HPP

