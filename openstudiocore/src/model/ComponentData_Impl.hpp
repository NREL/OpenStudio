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

#ifndef MODEL_COMPONENTDATA_IMPL_HPP
#define MODEL_COMPONENTDATA_IMPL_HPP


#include "ComponentData.hpp"

#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class MODEL_API ComponentData_Impl : public ModelObject_Impl {
    Q_OBJECT;

    Q_PROPERTY(boost::optional<int> creationTimestamp READ creationTimestamp);
    Q_PROPERTY(boost::optional<int> versionTimestamp READ versionTimestamp);

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> primaryComponentObject READ primaryComponentObjectAsModelObject);
    Q_PROPERTY(std::vector<openstudio::model::ModelObject> componentObjects READ componentObjects);
   public:
    /** @name Constructors and Destructors */
    //@{

    // constructor
    ComponentData_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    // construct from workspace
    ComponentData_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                       Model_Impl* model,
                       bool keepHandle);

    // copy constructor
    ComponentData_Impl(const ComponentData_Impl& other, Model_Impl* model, bool keepHandle);

    // virtual destructor
    virtual ~ComponentData_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    // Get all output variable names that could be associated with this object.
    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
    /** @name Getters */
    //@{

    UUID uuid() const;

    UUID versionUUID() const;

    boost::optional<int> creationTimestamp() const;

    boost::optional<int> versionTimestamp() const;

    /** Equivalent to getComponentObject(0). */
    ModelObject primaryComponentObject() const;

    std::vector<ModelObject> componentObjects() const;

    /** Throws if objectIndex >= numComponentObjects, or if ComponentData has been corrupted. */
    ModelObject getComponentObject(unsigned objectIndex) const;

    //@}
    /** @name Setters */
    //@{

    UUID createVersionUUID();

    /** Pushes a new extensible group containing object's data. Not exposed in the public
     *  interface. Use with care. */
    bool registerObject(const ModelObject& object);

    //@}
    /** @name Queries */
    //@{

    /** Returns the number of objects in the Component described by this object. Must be >= 1. */
    unsigned numComponentObjects() const;

    //@}
   private:
    REGISTER_LOGGER("openstudio.model.ComponentData");

    boost::optional<ModelObject> primaryComponentObjectAsModelObject() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_COMPONENTDATA_IMPL_HPP
