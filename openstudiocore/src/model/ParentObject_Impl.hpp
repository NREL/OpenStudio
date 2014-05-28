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
#ifndef MODEL_PARENTOBJECT_IMPL_HPP
#define MODEL_PARENTOBJECT_IMPL_HPP

#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class MODEL_API ParentObject_Impl : public ModelObject_Impl {
    Q_OBJECT;

    Q_PROPERTY(std::vector<openstudio::model::ModelObject> children READ children);
   public:

    // constructor
    ParentObject_Impl(IddObjectType type, Model_Impl* model);

    // constructor
    ParentObject_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    // construct from workspace
    ParentObject_Impl(const openstudio::detail::WorkspaceObject_Impl& other, 
                      Model_Impl* model, 
                      bool keepHandle);

    // clone copy constructor
    ParentObject_Impl(const ParentObject_Impl& other, Model_Impl* model, bool keepHandle);

    virtual ~ParentObject_Impl() {}

    /// return direct child objects in the hierarchy
    virtual std::vector<ModelObject> children() const;

    /// remove the object from the model's workspace
    /// return a new workspace containing any removed object(s)
    virtual std::vector<openstudio::IdfObject> remove();

    /// get a vector of allowable children types
    virtual std::vector<IddObjectType> allowableChildTypes() const;

    virtual ModelObject clone(Model model) const;

   private:

    REGISTER_LOGGER("openstudio.model.ParentObject");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_PARENTOBJECT_IMPL_HPP
