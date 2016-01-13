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

#include "Inverter.hpp"
#include "Inverter_Impl.hpp"
#include "ParentObject.hpp"
#include "ParentObject_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "ElectricLoadCenterDistribution.hpp"
#include "ElectricLoadCenterDistribution_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {

namespace model {

namespace detail {

Inverter_Impl::Inverter_Impl(IddObjectType type, Model_Impl* model)
  : ParentObject_Impl(type,model)
{
}

Inverter_Impl::Inverter_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
  : ParentObject_Impl(idfObject, model, keepHandle)
{ 
}

Inverter_Impl::Inverter_Impl(
    const openstudio::detail::WorkspaceObject_Impl& other, 
    Model_Impl* model, 
    bool keepHandle)
 : ParentObject_Impl(other, model, keepHandle)
{
}

Inverter_Impl::Inverter_Impl(const Inverter_Impl& other, 
                             Model_Impl* model, 
                             bool keepHandles)
 : ParentObject_Impl(other, model, keepHandles)
{
}

boost::optional<ElectricLoadCenterDistribution> Inverter_Impl::electricLoadCenterDistribution() const
{
  auto elcds = getObject<ModelObject>().getModelObjectSources<ElectricLoadCenterDistribution>(ElectricLoadCenterDistribution::iddObjectType());
  if (elcds.empty()){
    // no error
  } else if (elcds.size() == 1u){
    return elcds[0];
  } else{
    // error
  }

  return boost::none;
}

} // detail

Inverter::Inverter(IddObjectType type,const Model& model)
  : ParentObject(type, model)
{
  OS_ASSERT(getImpl<detail::Inverter_Impl>());
}

Inverter::Inverter(std::shared_ptr<detail::Inverter_Impl> p)
  : ParentObject(p)
{}

boost::optional<ElectricLoadCenterDistribution> Inverter::electricLoadCenterDistribution() const
{
  return getImpl<detail::Inverter_Impl>()->electricLoadCenterDistribution();
}

boost::optional<ThermalZone> Inverter::thermalZone() const {
  return getImpl<detail::Inverter_Impl>()->thermalZone();
}

bool Inverter::setThermalZone(const ThermalZone& thermalZone) {
  return getImpl<detail::Inverter_Impl>()->setThermalZone(thermalZone);
}

void Inverter::resetThermalZone() {
  getImpl<detail::Inverter_Impl>()->resetThermalZone();
}

} // model

} // openstudio

