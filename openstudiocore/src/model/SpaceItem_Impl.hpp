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

#ifndef MODEL_SPACEITEM_IMPL_HPP
#define MODEL_SPACEITEM_IMPL_HPP

#include "ModelObject_Impl.hpp"

#include <QObject>

namespace openstudio {
namespace model {

// forward declaration
class Space;
class SpaceItem;

namespace detail {

  class MODEL_API SpaceItem_Impl : public ModelObject_Impl {
    Q_OBJECT;

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> space READ spaceAsModelObject WRITE setSpaceAsModelObject RESET resetSpace);
   public:

    // Construct completely new object.
    SpaceItem_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    // Construct from existing workspace object (typically when Model is being constructed
    // from Workspace).
    SpaceItem_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                        Model_Impl* model,
                        bool keepHandle);

    // Clone copy constructor.
    SpaceItem_Impl(const SpaceItem_Impl& other, Model_Impl* model, bool keepHandle);

    virtual ~SpaceItem_Impl() {}

    // return the parent object in the hierarchy
    virtual boost::optional<ParentObject> parent() const;

    /// set the parent, child may have to call methods on the parent
    virtual bool setParent(ParentObject& newParent);

    /// Returns the parent Space.
    boost::optional<Space> space() const;

    /// Sets the parent Space.
    bool setSpace(const Space& space);

    void resetSpace();

   protected:

    // index of the space name
    virtual int spaceIndex() const = 0;

   private:

    REGISTER_LOGGER("openstudio.model.SpaceItem");

    boost::optional<ModelObject> spaceAsModelObject() const;

    bool setSpaceAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SPACEITEM_IMPL_HPP
