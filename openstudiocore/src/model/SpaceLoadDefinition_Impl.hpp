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

#ifndef MODEL_SPACELOADDEFINITION_IMPL_HPP
#define MODEL_SPACELOADDEFINITION_IMPL_HPP

#include "ResourceObject_Impl.hpp"

#include <QObject>

namespace openstudio {
namespace model {

// forward declaration
class SpaceLoadInstance;
class SpaceLoadDefinition;

namespace detail {

  class MODEL_API SpaceLoadDefinition_Impl : public ResourceObject_Impl
  {
   public:

    // Construct completely new object.
    SpaceLoadDefinition_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    // Construct from existing workspace object (typically when Model is being constructed
    // from Workspace).
    SpaceLoadDefinition_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                        Model_Impl* model,
                        bool keepHandle);

    // Clone copy constructor.
    SpaceLoadDefinition_Impl(const SpaceLoadDefinition_Impl& other, Model_Impl* model, bool keepHandle);

    virtual ~SpaceLoadDefinition_Impl() {}

    /// Removes the definition and all instances.
    virtual std::vector<openstudio::IdfObject> remove();

    /// Returns all instances of this definition.
    std::vector<SpaceLoadInstance> instances() const;

    /** Returns the floor area associated with instances of this space load definition.  
    If two instances are in the same space then that floor area will be double counted.
    If two instances used in the same space type then that floor area will be double counted.
    Space and SpaceLoadInstance multipliers are included in the result. **/
    double floorArea() const;

    /** Returns the number of instances of this space load.  
    Space and SpaceLoadInstance multipliers are included in the result **/
    int quantity() const;

   private:

    REGISTER_LOGGER("openstudio.model.SpaceLoadDefinition");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SPACELOADDEFINITION_IMPL_HPP
