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

#ifndef UTILITIES_WORKSPACEOBJECT_DIFF_HPP
#define UTILITIES_WORKSPACEOBJECT_DIFF_HPP

#include <utilities/idf/IdfObjectDiff.hpp>

#include <utilities/UtilitiesAPI.hpp>

#include <utilities/core/UUID.hpp>

namespace openstudio {
namespace detail {
  class WorkspaceObjectDiff_Impl;
}

/** WorkspaceObjectDiff represents a change to an WorkspaceObjectDiff.
**/
class UTILITIES_API WorkspaceObjectDiff : public IdfObjectDiff {
public:

  WorkspaceObjectDiff(unsigned index, boost::optional<std::string> oldValue, boost::optional<std::string> newValue,
                      boost::optional<UUID> oldHandle, boost::optional<UUID> newHandle);

  /// get the old handle if there was one, use oldValue to determine if field existed or not
  boost::optional<UUID> oldHandle() const;

  /// get the new handle if there is one, use newValue to determine if field exists or not
  boost::optional<UUID> newHandle() const;

private:

  friend class IdfObjectDiff;

  typedef detail::WorkspaceObjectDiff_Impl ImplType;

  WorkspaceObjectDiff(const boost::shared_ptr<detail::WorkspaceObjectDiff_Impl>& impl);

};

} // openstudio

#endif // UTILITIES_WORKSPACEOBJECT_DIFF_HPP
