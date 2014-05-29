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

#include "Glazing.hpp"
#include "Glazing_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  Glazing_Impl::Glazing_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : FenestrationMaterial_Impl(idfObject, model, keepHandle)
  {}

  Glazing_Impl::Glazing_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                             Model_Impl* model,
                             bool keepHandle)
    : FenestrationMaterial_Impl(other, model, keepHandle)
  {}
  
  Glazing_Impl::Glazing_Impl(const Glazing_Impl& other,Model_Impl* model,bool keepHandle)
    : FenestrationMaterial_Impl(other, model, keepHandle)
  {}

} // detail

Glazing::Glazing(IddObjectType type,const Model& model)
  : FenestrationMaterial(type,model) 
{
  OS_ASSERT(getImpl<detail::Glazing_Impl>());
}

/// @cond
Glazing::Glazing(std::shared_ptr<detail::Glazing_Impl> impl)
  : FenestrationMaterial(impl)
{}
/// @endcond

} // model
} // openstudio
