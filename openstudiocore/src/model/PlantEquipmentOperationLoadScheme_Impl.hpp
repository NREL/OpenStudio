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

#ifndef MODEL_PlantEquipmentOperationLoadScheme_IMPL_HPP
#define MODEL_PlantEquipmentOperationLoadScheme_IMPL_HPP

#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

class PlantLoop;

namespace detail {

  class MODEL_API PlantEquipmentOperationLoadScheme_Impl : public ModelObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    PlantEquipmentOperationLoadScheme_Impl(IddObjectType type, Model_Impl* model);

    PlantEquipmentOperationLoadScheme_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    PlantEquipmentOperationLoadScheme_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                           Model_Impl* model,
                           bool keepHandle);

    PlantEquipmentOperationLoadScheme_Impl(const PlantEquipmentOperationLoadScheme_Impl& other, Model_Impl* model, bool keepHandles);

    virtual ~PlantEquipmentOperationLoadScheme_Impl() {}

   private:

    REGISTER_LOGGER("openstudio.model.PlantEquipmentOperationLoadScheme");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_PlantEquipmentOperationLoadScheme_IMPL_HPP
