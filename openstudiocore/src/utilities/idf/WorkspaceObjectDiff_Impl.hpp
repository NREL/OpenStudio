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

#ifndef UTILITIES_IDF_WORKSPACEOBJECTDIFF_IMPL_HPP
#define UTILITIES_IDF_WORKSPACEOBJECTDIFF_IMPL_HPP

#include "IdfObjectDiff_Impl.hpp"

#include "../UtilitiesAPI.hpp"

#include "../core/UUID.hpp"

namespace openstudio {
namespace detail {

  class UTILITIES_API WorkspaceObjectDiff_Impl : public IdfObjectDiff_Impl {
  public:

    WorkspaceObjectDiff_Impl(unsigned index, boost::optional<std::string> oldValue, boost::optional<std::string> newValue,
                             boost::optional<UUID> oldHandle, boost::optional<UUID> newHandle);

    /// get the old handle if there was one
    boost::optional<UUID> oldHandle() const;

    /// get the new handle if there is one
    boost::optional<UUID> newHandle() const;

  private:

    boost::optional<UUID> m_oldHandle;
    boost::optional<UUID> m_newHandle;

  };

} // detail
} // openstudio

#endif // UTILITIES_IDF_WORKSPACEOBJECTDIFF_IMPL_HPP
