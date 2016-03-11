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

#ifndef MODEL_PLANTEQUIPMENTOPERATIONSCHEME_HPP
#define MODEL_PLANTEQUIPMENTOPERATIONSCHEME_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

class PlantLoop;

namespace detail{
  class PlantEquipmentOperationScheme_Impl;
}

class MODEL_API PlantEquipmentOperationScheme : public ModelObject {

  public:

  PlantEquipmentOperationScheme(IddObjectType type,const Model& model);

  virtual ~PlantEquipmentOperationScheme() {}

  friend class openstudio::IdfObject;

  boost::optional<PlantLoop> plantLoop() const;

  /// @cond 

  typedef detail::PlantEquipmentOperationScheme_Impl ImplType;

  explicit PlantEquipmentOperationScheme(std::shared_ptr<detail::PlantEquipmentOperationScheme_Impl> impl);

  private:

  REGISTER_LOGGER("openstudio.model.PlantEquipmentOperationScheme");

  /// @endcond 

};

typedef boost::optional<PlantEquipmentOperationScheme> OptionalPlantEquipmentOperationScheme;

typedef std::vector<PlantEquipmentOperationScheme> PlantEquipmentOperationSchemeVector;

} // model
} // openstudio

#endif // MODEL_PLANTEQUIPMENTOPERATIONSCHEME_HPP

