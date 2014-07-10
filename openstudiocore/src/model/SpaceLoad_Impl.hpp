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

#ifndef MODEL_SPACELOAD_IMPL_HPP
#define MODEL_SPACELOAD_IMPL_HPP

#include "SpaceItem_Impl.hpp"

#include <QObject>

namespace openstudio {
namespace model {

// forward declaration
class SpaceType;
class SpaceLoad;

namespace detail {

  class MODEL_API SpaceLoad_Impl : public SpaceItem_Impl {
    Q_OBJECT;

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> spaceType READ spaceTypeAsModelObject WRITE setSpaceTypeAsModelObject RESET resetSpaceType);
   public:

    // Construct completely new object.
    SpaceLoad_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    // Construct from existing workspace object (typically when Model is being constructed
    // from Workspace).
    SpaceLoad_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                        Model_Impl* model,
                        bool keepHandle);

    // Clone copy constructor.
    SpaceLoad_Impl(const SpaceLoad_Impl& other, Model_Impl* model, bool keepHandle);

    virtual ~SpaceLoad_Impl() {}

    // return the parent object in the hierarchy
    virtual boost::optional<ParentObject> parent() const;

    /// set the parent, child may have to call methods on the parent
    virtual bool setParent(ParentObject& newParent);

    /// Returns the parent SpaceType.
    boost::optional<SpaceType> spaceType() const;

    /// Sets the parent SpaceType.
    bool setSpaceType(const SpaceType& spaceType);

    void resetSpaceType();

    /// Converts space load levels to abosolute values rather than by per area, etc.
    /// Also hard sizes any costs associated with this load.
    /// Only possible if this space load is parented by a space, returns false if 
    /// parented by a space type.
    virtual bool hardSize() = 0;

    /// Directly applies default schedules to this space load if found.
    virtual bool hardApplySchedules() = 0;

   private:
    REGISTER_LOGGER("openstudio.model.SpaceLoad");

    boost::optional<ModelObject> spaceTypeAsModelObject() const;

    bool setSpaceTypeAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SPACELOAD_IMPL_HPP
