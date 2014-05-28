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

#include "GenericModelObject.hpp"
#include "GenericModelObject_Impl.hpp"


namespace openstudio {
namespace model {

namespace detail{

// constructor
GenericModelObject_Impl::GenericModelObject_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
  : ModelObject_Impl(idfObject, model, keepHandle)
{}

GenericModelObject_Impl::GenericModelObject_Impl(
  const openstudio::detail::WorkspaceObject_Impl& other,Model_Impl* model,bool keepHandle)
  : ModelObject_Impl(other,model,keepHandle)
{}

GenericModelObject_Impl::GenericModelObject_Impl(const GenericModelObject_Impl& other,
                                                 Model_Impl* model,
                                                 bool keepHandle)
  : ModelObject_Impl(other,model,keepHandle)
{}

const std::vector<std::string>& GenericModelObject_Impl::outputVariableNames() const
{
  static std::vector<std::string> result;
  if (result.empty()){
  }
  return result;
}


IddObjectType GenericModelObject_Impl::iddObjectType() const
{
  IddObjectType retVal(iddObject().type());
  return retVal;
}

}// detail

GenericModelObject::GenericModelObject(std::shared_ptr<detail::GenericModelObject_Impl> p)
  :  ModelObject(p)
{}

} // model
} // openstudio
