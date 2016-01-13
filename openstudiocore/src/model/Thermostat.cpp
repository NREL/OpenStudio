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

#include "Thermostat.hpp"
#include "Thermostat_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  Thermostat_Impl::Thermostat_Impl(IddObjectType type, Model_Impl* model)
    : ModelObject_Impl(type,model)
  {
  }

  Thermostat_Impl::Thermostat_Impl(const IdfObject& idfObject,
                                         Model_Impl* model,
                                         bool keepHandle)
                                           : ModelObject_Impl(idfObject, model, keepHandle)
  {
  }

  Thermostat_Impl::Thermostat_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other,
      Model_Impl* model,
      bool keepHandle)
        : ModelObject_Impl(other,model,keepHandle)
  {
  }

  Thermostat_Impl::Thermostat_Impl(const Thermostat_Impl& other,
                                         Model_Impl* model,
                                         bool keepHandles)
                                           : ModelObject_Impl(other,model,keepHandles)
  {
  }

  boost::optional<ThermalZone> Thermostat_Impl::thermalZone() const
  {
    Handle h = handle();
    auto zones = model().getModelObjects<model::ThermalZone>();
    for( const auto & zone: zones ) {
      if( auto thermostat = zone.thermostat() ) {
        if( thermostat->handle() == h ) {
          return zone;
        }
      }
    }
    return boost::none;
  }

} // detail

Thermostat::Thermostat(std::shared_ptr<detail::Thermostat_Impl> p)
  : ModelObject(p)
{}

Thermostat::Thermostat(IddObjectType type,const Model& model)
  : ModelObject(type,model)
{
  OS_ASSERT(getImpl<detail::Thermostat_Impl>());
}     

boost::optional<ThermalZone> Thermostat::thermalZone() const
{
  return getImpl<detail::Thermostat_Impl>()->thermalZone();
}

} // model

} // openstudio

