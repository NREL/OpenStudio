/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#ifndef MODEL_PLANTEQUIPMENTOPERATIONLOADSCHEME_HPP
#define MODEL_PLANTEQUIPMENTOPERATIONLOADSCHEME_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

class PlantLoop;

namespace detail{
  class PlantEquipmentOperationLoadScheme_Impl;
}

class MODEL_API PlantEquipmentOperationLoadScheme : public ModelObject {

  public:

  PlantEquipmentOperationLoadScheme(IddObjectType type,const Model& model);

  virtual ~PlantEquipmentOperationLoadScheme() {}

  friend class openstudio::IdfObject;

  /// @cond 

  typedef detail::PlantEquipmentOperationLoadScheme_Impl ImplType;

  explicit PlantEquipmentOperationLoadScheme(std::shared_ptr<detail::PlantEquipmentOperationLoadScheme_Impl> impl);

  private:

  REGISTER_LOGGER("openstudio.model.PlantEquipmentOperationLoadScheme");

  /// @endcond 

};

typedef boost::optional<PlantEquipmentOperationLoadScheme> OptionalPlantEquipmentOperationLoadScheme;

typedef std::vector<PlantEquipmentOperationLoadScheme> PlantEquipmentOperationLoadSchemeVector;

} // model
} // openstudio

#endif // MODEL_PLANTEQUIPMENTOPERATIONLOADSCHEME_HPP

