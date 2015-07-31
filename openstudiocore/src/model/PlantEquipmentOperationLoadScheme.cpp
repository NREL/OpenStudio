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

#include "PlantEquipmentOperationLoadScheme.hpp"
#include "PlantEquipmentOperationLoadScheme_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {

namespace model {

namespace detail {

PlantEquipmentOperationLoadScheme_Impl::PlantEquipmentOperationLoadScheme_Impl(IddObjectType type, Model_Impl* model)
  : ModelObject_Impl(type,model)
{
}

PlantEquipmentOperationLoadScheme_Impl::PlantEquipmentOperationLoadScheme_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
  : ModelObject_Impl(idfObject, model, keepHandle)
{ 
}

PlantEquipmentOperationLoadScheme_Impl::PlantEquipmentOperationLoadScheme_Impl(
    const openstudio::detail::WorkspaceObject_Impl& other, 
    Model_Impl* model, 
    bool keepHandle)
  : ModelObject_Impl(other,model,keepHandle)
{
}

PlantEquipmentOperationLoadScheme_Impl::PlantEquipmentOperationLoadScheme_Impl(const PlantEquipmentOperationLoadScheme_Impl& other, 
                                               Model_Impl* model, 
                                               bool keepHandles)
  : ModelObject_Impl(other,model,keepHandles)
{
}

} // detail

PlantEquipmentOperationLoadScheme::PlantEquipmentOperationLoadScheme(IddObjectType type,const Model& model)
  : ModelObject(type,model)
{
  OS_ASSERT(getImpl<detail::PlantEquipmentOperationLoadScheme_Impl>());
}     

PlantEquipmentOperationLoadScheme::PlantEquipmentOperationLoadScheme(std::shared_ptr<detail::PlantEquipmentOperationLoadScheme_Impl> p)
  : ModelObject(p)
{}


} // model

} // openstudio

