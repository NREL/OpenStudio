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

#include "Curve.hpp"
#include "Curve_Impl.hpp"

#include "ParentObject.hpp"
#include "ParentObject_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  Curve_Impl::Curve_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {}

  Curve_Impl::Curve_Impl(const openstudio::detail::WorkspaceObject_Impl& other, 
                         Model_Impl* model, 
                         bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  Curve_Impl::Curve_Impl(const Curve_Impl& other, Model_Impl* model, bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
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
  : ModelObject(type,model) 
{
  OS_ASSERT(getImpl<detail::Curve_Impl>());
}

Curve::Curve(std::shared_ptr<detail::Curve_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

