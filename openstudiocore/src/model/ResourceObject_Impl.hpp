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

#ifndef MODEL_RESOURCEOBJECT_IMPL_HPP
#define MODEL_RESOURCEOBJECT_IMPL_HPP

#include "ParentObject_Impl.hpp"

#include <QObject>

namespace openstudio {
namespace model {

// forward declaration
class ResourceObject;

namespace detail {

  class MODEL_API ResourceObject_Impl : public ParentObject_Impl
  {
   public:

    // Construct completely new object.
    ResourceObject_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    // Construct from existing workspace object (typically when Model is being constructed 
    // from Workspace).
    ResourceObject_Impl(const openstudio::detail::WorkspaceObject_Impl& other, 
                        Model_Impl* model, 
                        bool keepHandle);

    // Clone copy constructor.
    ResourceObject_Impl(const ResourceObject_Impl& other, Model_Impl* model, bool keepHandle);

    virtual ~ResourceObject_Impl() {}

    /** Returns the number of objects that use this resource directly. All usages count equally. 
     *  For instance, the use of a Material in a LayeredConstruction counts as one use of the 
     *  Material object, and usages of that LayeredConstruction do not add to the Material's use
     *  count. */
    unsigned directUseCount() const;

    /** Returns the number of non-ResourceObjects that use this resource either directly or 
     *  indirectly. Non-ResourceObjects include Building, Zone, Surface, Lights, and AirLoopHVAC,
     *  for instance. An example of indirect use is the use of a Material object through inclusion 
     *  in a Construction. If a particular Material is used in exactly one LayeredConstruction, which
     *  is in turn used by three PlanarSurface objects, then the nonResourceObjectUseCount of both 
     *  the Material and the LayeredConstruction is three. */
    unsigned nonResourceObjectUseCount() const;

   private:

    REGISTER_LOGGER("openstudio.model.ResourceObject");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_RESOURCEOBJECT_IMPL_HPP
