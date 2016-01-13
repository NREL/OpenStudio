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

#include "ExteriorLightsDefinition.hpp"
#include "ExteriorLightsDefinition_Impl.hpp"

#include <utilities/idd/OS_Exterior_Lights_Definition_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ExteriorLightsDefinition_Impl::ExteriorLightsDefinition_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ResourceObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ExteriorLightsDefinition::iddObjectType());
  }

  ExteriorLightsDefinition_Impl::ExteriorLightsDefinition_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                               Model_Impl* model,
                                                               bool keepHandle)
    : ResourceObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ExteriorLightsDefinition::iddObjectType());
  }

  ExteriorLightsDefinition_Impl::ExteriorLightsDefinition_Impl(const ExteriorLightsDefinition_Impl& other,
                                                               Model_Impl* model,
                                                               bool keepHandle)
    : ResourceObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ExteriorLightsDefinition_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ExteriorLightsDefinition_Impl::iddObjectType() const {
    return ExteriorLightsDefinition::iddObjectType();
  }

  double ExteriorLightsDefinition_Impl::designLevel() const {
    boost::optional<double> value = getDouble(OS_Exterior_Lights_DefinitionFields::DesignLevel,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ExteriorLightsDefinition_Impl::setDesignLevel(double designLevel) {
    bool result = setDouble(OS_Exterior_Lights_DefinitionFields::DesignLevel, designLevel);
    return result;
  }

} // detail

ExteriorLightsDefinition::ExteriorLightsDefinition(const Model& model)
  : ResourceObject(ExteriorLightsDefinition::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ExteriorLightsDefinition_Impl>());

  bool ok = setDesignLevel(0.0);
  OS_ASSERT(ok);
}

IddObjectType ExteriorLightsDefinition::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Exterior_Lights_Definition);
  return result;
}

double ExteriorLightsDefinition::designLevel() const {
  return getImpl<detail::ExteriorLightsDefinition_Impl>()->designLevel();
}

bool ExteriorLightsDefinition::setDesignLevel(double designLevel) {
  return getImpl<detail::ExteriorLightsDefinition_Impl>()->setDesignLevel(designLevel);
}

/// @cond
ExteriorLightsDefinition::ExteriorLightsDefinition(std::shared_ptr<detail::ExteriorLightsDefinition_Impl> impl)
  : ResourceObject(impl)
{}
/// @endcond

} // model
} // openstudio

