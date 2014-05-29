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

#include "SizingPeriod.hpp"
#include "SizingPeriod_Impl.hpp"
#include "Site.hpp"
#include "Site_Impl.hpp"
#include "SkyTemperature.hpp"
#include "SkyTemperature_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model  {

namespace detail {

  SizingPeriod_Impl::SizingPeriod_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ParentObject_Impl(idfObject, model, keepHandle)
  {}

  SizingPeriod_Impl::SizingPeriod_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                 Model_Impl* model,
                                 bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {}

  SizingPeriod_Impl::SizingPeriod_Impl(const SizingPeriod_Impl& other,Model_Impl* model,bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {}

  // return the parent object in the hierarchy
  boost::optional<ParentObject> SizingPeriod_Impl::parent() const
  {
    boost::optional<ParentObject> result(model().getOptionalUniqueModelObject<Site>());
    return result;
  }

  // set the parent, child may have to call methods on the parent
  bool SizingPeriod_Impl::setParent(ParentObject& newParent)
  {
    if (newParent.optionalCast<Site>() && (newParent.model() == model())) {
      return true;
    }
    return false;
  }

  // return any children objects in the hierarchy
  std::vector<ModelObject> SizingPeriod_Impl::children() const
  {
    std::vector<ModelObject> result;
    SkyTemperatureVector sts = getObject<ModelObject>().getModelObjectSources<SkyTemperature>();
    result.insert(result.end(),sts.begin(),sts.end());
    return result;
  }

  std::vector<IddObjectType> SizingPeriod_Impl::allowableChildTypes() const {
    IddObjectTypeVector result;
    result.push_back(SkyTemperature::iddObjectType());
    return result;
  }

} // detail

/// constructor
SizingPeriod::SizingPeriod(IddObjectType type,const Model& model)
  : ParentObject(type,model) 
{
  OS_ASSERT(getImpl<detail::SizingPeriod_Impl>());
}

// constructor
SizingPeriod::SizingPeriod(std::shared_ptr<detail::SizingPeriod_Impl> impl)
  : ParentObject(impl)
{}

// ensure that this object does not contain the date 2/29
void SizingPeriod::ensureNoLeapDays()
{
  getImpl<detail::SizingPeriod_Impl>()->ensureNoLeapDays();
}

} // model
} // openstudio
