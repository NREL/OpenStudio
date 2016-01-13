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

#include "SkyTemperature.hpp"
#include "SkyTemperature_Impl.hpp"
#include "Site.hpp"
#include "Site_Impl.hpp"
#include "RunPeriod.hpp"
#include "RunPeriod_Impl.hpp"
#include "SizingPeriod.hpp"
#include "SizingPeriod_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/OS_WeatherProperty_SkyTemperature_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model  {

namespace detail {

  SkyTemperature_Impl::SkyTemperature_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ModelObject_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SkyTemperature::iddObjectType());
  }

  SkyTemperature_Impl::SkyTemperature_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SkyTemperature::iddObjectType());
  }

  SkyTemperature_Impl::SkyTemperature_Impl(const SkyTemperature_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
  }

  // return the parent object in the hierarchy
  boost::optional<ParentObject> SkyTemperature_Impl::parent() const
  {
    // if Name field not blank, that is the parent
    OptionalParentObject result = getObject<ModelObject>().getModelObjectTarget<ParentObject>(
        OS_WeatherProperty_SkyTemperatureFields::Name);
    // if it is blank, parent is Site
    if (!result) {
      result = OptionalParentObject(model().getOptionalUniqueModelObject<Site>());
    }
    return result;
  }

  // set the parent, child may have to call methods on the parent
  bool SkyTemperature_Impl::setParent(ParentObject& newParent)
  {
    if (!(newParent.model() == model())) { return false; }

    if (newParent.optionalCast<Site>()) {
      // nullify the field
      return setPointer(OS_WeatherProperty_SkyTemperatureFields::Name,Handle());
    }
    else if (newParent.optionalCast<RunPeriod>() || newParent.optionalCast<SizingPeriod>()) {
      return setPointer(OS_WeatherProperty_SkyTemperatureFields::Name,newParent.handle());
    }

    return false;
  }

  // Get all output variable names that could be associated with this object.
  const std::vector<std::string>& SkyTemperature_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SkyTemperature_Impl::iddObjectType() const {
    return SkyTemperature::iddObjectType();
  }

} // detail

/// constructor
SkyTemperature::SkyTemperature(const Model& model)
  : ModelObject(SkyTemperature::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SkyTemperature_Impl>());
}

// constructor
SkyTemperature::SkyTemperature(std::shared_ptr<detail::SkyTemperature_Impl> impl)
  : ModelObject(impl)
{}

IddObjectType SkyTemperature::iddObjectType() {
  IddObjectType result(IddObjectType::OS_WeatherProperty_SkyTemperature);
  return result;
}

} // model
} // openstudio
