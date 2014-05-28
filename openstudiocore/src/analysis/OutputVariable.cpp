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

#include "OutputVariable.hpp"
#include "OutputVariable_Impl.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  OutputVariable_Impl::OutputVariable_Impl(const std::string& name)
    : Variable_Impl(name)
  {}

  OutputVariable_Impl::OutputVariable_Impl(const UUID& uuid,
                                           const UUID& versionUUID,
                                           const std::string& name,
                                           const std::string& displayName,
                                           const std::string& description)
    : Variable_Impl(uuid,versionUUID,name,displayName,description)
  {}

  OutputVariable_Impl::OutputVariable_Impl(const OutputVariable_Impl& other)
    : Variable_Impl(other)
  {}

} // detail

/// @cond
OutputVariable::OutputVariable(std::shared_ptr<detail::OutputVariable_Impl> impl)
  : Variable(impl)
{}
/// @endcond

} // analysis
} // openstudio
