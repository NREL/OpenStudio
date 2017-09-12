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

#include "Curve.hpp"
#include "Curve_Impl.hpp"

#include "ParentObject.hpp"
#include "ParentObject_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  Curve_Impl::Curve_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ResourceObject_Impl(idfObject,model,keepHandle)
  {}

  Curve_Impl::Curve_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                         Model_Impl* model,
                         bool keepHandle)
    : ResourceObject_Impl(other,model,keepHandle)
  {}

  Curve_Impl::Curve_Impl(const Curve_Impl& other, Model_Impl* model, bool keepHandle)
    : ResourceObject_Impl(other,model,keepHandle)
  {}

  boost::optional<ParentObject> Curve_Impl::parent() const {
    ParentObjectVector parents = getObject<Curve>().getModelObjectSources<ParentObject>();
    if (parents.size() > 1u) {
      LOG(Error,"This Curve, " << briefDescription() << " has multiple parents. Returning the first.");
    }
    if (!parents.empty()) {
      return parents[0];
    }
    return boost::none;
  }

  double Curve_Impl::evaluate(double x) const {
    return evaluate(DoubleVector(1u,x));
  }

  double Curve_Impl::evaluate(double x, double y) const {
    DoubleVector values(2u);
    values[0] = x;
    values[1] = y;
    return evaluate(values);
  }

  double Curve_Impl::evaluate(double x, double y, double z) const {
    DoubleVector values(3u);
    values[0] = x;
    values[1] = y;
    values[2] = z;
    return evaluate(values);
  }

} // detail

int Curve::numVariables() const {
  return getImpl<detail::Curve_Impl>()->numVariables();
}

double Curve::evaluate(const std::vector<double>& x) const {
  return getImpl<detail::Curve_Impl>()->evaluate(x);
}

double Curve::evaluate(double x) const {
  return getImpl<detail::Curve_Impl>()->evaluate(x);
}

double Curve::evaluate(double x, double y) const {
  return getImpl<detail::Curve_Impl>()->evaluate(x,y);
}

double Curve::evaluate(double x, double y, double z) const {
  return getImpl<detail::Curve_Impl>()->evaluate(x,y,z);
}

/// @cond
Curve::Curve(IddObjectType type, const Model& model)
  : ResourceObject(type,model)
{
  OS_ASSERT(getImpl<detail::Curve_Impl>());
}

Curve::Curve(std::shared_ptr<detail::Curve_Impl> impl)
  : ResourceObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

