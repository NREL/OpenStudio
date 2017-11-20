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
  : ParentObject(std::move(impl))
{}

// ensure that this object does not contain the date 2/29
void SizingPeriod::ensureNoLeapDays()
{
  getImpl<detail::SizingPeriod_Impl>()->ensureNoLeapDays();
}

} // model
} // openstudio
