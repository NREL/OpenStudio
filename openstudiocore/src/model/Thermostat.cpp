/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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
    auto zones = model().getConcreteModelObjects<model::ThermalZone>();
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
  : ModelObject(std::move(p))
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

