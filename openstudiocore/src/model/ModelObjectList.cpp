/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <model/ModelObjectList.hpp>
#include <model/ModelObjectList_Impl.hpp>

#include <utilities/idd/OS_ModelObjectList_FieldEnums.hxx>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model {

namespace detail {

  ModelObjectList_Impl::ModelObjectList_Impl(const IdfObject& idfObject,
                                             Model_Impl* model,
                                             bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    BOOST_ASSERT(idfObject.iddObject().type() == ModelObjectList::iddObjectType());
  }

  ModelObjectList_Impl::ModelObjectList_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    BOOST_ASSERT(other.iddObject().type() == ModelObjectList::iddObjectType());
  }

  ModelObjectList_Impl::ModelObjectList_Impl(const ModelObjectList_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ModelObjectList_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ModelObjectList_Impl::iddObjectType() const {
    return ModelObjectList::iddObjectType();
  }

} // detail

ModelObjectList::ModelObjectList(const Model& model)
  : ModelObject(ModelObjectList::iddObjectType(),model)
{
  BOOST_ASSERT(getImpl<detail::ModelObjectList_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  bool ok = true;
  // ok = setHandle();
  BOOST_ASSERT(ok);
}

IddObjectType ModelObjectList::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ModelObjectList);
}

/// @cond
ModelObjectList::ModelObjectList(boost::shared_ptr<detail::ModelObjectList_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

