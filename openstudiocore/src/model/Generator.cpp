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

#include "Generator.hpp"
#include "Generator_Impl.hpp"
#include "ParentObject.hpp"
#include "ParentObject_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {

namespace model {

namespace detail {

Generator_Impl::Generator_Impl(IddObjectType type, Model_Impl* model)
  : ParentObject_Impl(type, model)
{
}

Generator_Impl::Generator_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
  : ParentObject_Impl(idfObject, model, keepHandle)
{ 
}

Generator_Impl::Generator_Impl(
    const openstudio::detail::WorkspaceObject_Impl& other, 
    Model_Impl* model, 
    bool keepHandle)
  : ParentObject_Impl(other, model, keepHandle)
{
}

Generator_Impl::Generator_Impl(const Generator_Impl& other, 
                               Model_Impl* model, 
                               bool keepHandles)
  : ParentObject_Impl(other, model, keepHandles)
{
}

} // detail

Generator::Generator(IddObjectType type,const Model& model)
  : ParentObject(type,model)
{
  OS_ASSERT(getImpl<detail::Generator_Impl>());
}     

Generator::Generator(std::shared_ptr<detail::Generator_Impl> p)
  : ParentObject(p)
{}


} // model

} // openstudio

