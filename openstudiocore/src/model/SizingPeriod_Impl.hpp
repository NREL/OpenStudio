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

#ifndef MODEL_SIZINGPERIOD_IMPL_HPP
#define MODEL_SIZINGPERIOD_IMPL_HPP

#include "ParentObject_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class MODEL_API SizingPeriod_Impl : public ParentObject_Impl {
   public:

    // constructor
    SizingPeriod_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    // construct from workspace
    SizingPeriod_Impl(const openstudio::detail::WorkspaceObject_Impl& other, 
                   Model_Impl* model, 
                   bool keepHandle);

    // clone copy constructor
    SizingPeriod_Impl(const SizingPeriod_Impl& other,Model_Impl* model,bool keepHandle);

    // virtual destructor
    virtual ~SizingPeriod_Impl(){}

    // ensure that this object does not contain the date 2/29
    virtual void ensureNoLeapDays() = 0;

    // return the parent object in the hierarchy
    virtual boost::optional<ParentObject> parent() const;

    // set the parent, child may have to call methods on the parent
    virtual bool setParent(ParentObject& newParent);

    // return any children objects in the hierarchy
    virtual std::vector<ModelObject> children() const;

    /// get a vector of allowable children types
    virtual std::vector<IddObjectType> allowableChildTypes() const;
  
   private:

    REGISTER_LOGGER("openstudio.model.SizingPeriod");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SIZINGPERIOD_IMPL_HPP
