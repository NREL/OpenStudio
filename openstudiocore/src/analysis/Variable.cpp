/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <analysis/Variable.hpp>
#include <analysis/Variable_Impl.hpp>

namespace openstudio {
namespace analysis {

namespace detail {

  Variable_Impl::Variable_Impl(const std::string& name)
    : AnalysisObject_Impl(name)
  {}

  Variable_Impl::Variable_Impl(const UUID& uuid,
                               const UUID& versionUUID,
                               const std::string& name,
                               const std::string& displayName,
                               const std::string& description)
    : AnalysisObject_Impl(uuid,versionUUID,name,displayName,description)
  {}

  Variable_Impl::Variable_Impl(const Variable_Impl &other)
    : AnalysisObject_Impl(other)
  {}

} // detail

double Variable::getValue(const DataPoint& dataPoint) const {
  return getImpl<detail::Variable_Impl>()->getValue(dataPoint);
}

/// @cond
Variable::Variable(boost::shared_ptr<detail::Variable_Impl> impl)
  : AnalysisObject(impl)
{}
/// @endcond

} // analysis
} // openstudio

