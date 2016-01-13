/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.  
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

#include "PhotovoltaicPerformance.hpp"
#include "PhotovoltaicPerformance_Impl.hpp"
#include "GeneratorPhotovoltaic.hpp"
#include "ModelObject.hpp"
#include "ModelObject_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {

namespace model {

namespace detail {

PhotovoltaicPerformance_Impl::PhotovoltaicPerformance_Impl(IddObjectType type, Model_Impl* model)
  : ModelObject_Impl(type, model)
{
}

PhotovoltaicPerformance_Impl::PhotovoltaicPerformance_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
  : ModelObject_Impl(idfObject, model, keepHandle)
{ 
}

PhotovoltaicPerformance_Impl::PhotovoltaicPerformance_Impl(
    const openstudio::detail::WorkspaceObject_Impl& other, 
    Model_Impl* model, 
    bool keepHandle)
  : ModelObject_Impl(other, model, keepHandle)
{
}

PhotovoltaicPerformance_Impl::PhotovoltaicPerformance_Impl(const PhotovoltaicPerformance_Impl& other,
                                                           Model_Impl* model, 
                                                           bool keepHandles)
  : ModelObject_Impl(other, model, keepHandles)
{
}

std::vector<openstudio::IdfObject> PhotovoltaicPerformance_Impl::remove()
{
  std::vector<openstudio::IdfObject> result;

  if (getSources(GeneratorPhotovoltaic::iddObjectType()).empty()){
    result = ModelObject_Impl::remove();
  }

  return result;
}

} // detail

PhotovoltaicPerformance::PhotovoltaicPerformance(IddObjectType type, const Model& model)
  : ModelObject(type,model)
{
  OS_ASSERT(getImpl<detail::PhotovoltaicPerformance_Impl>());
}     

PhotovoltaicPerformance::PhotovoltaicPerformance(std::shared_ptr<detail::PhotovoltaicPerformance_Impl> p)
  : ModelObject(p)
{}


} // model

} // openstudio

