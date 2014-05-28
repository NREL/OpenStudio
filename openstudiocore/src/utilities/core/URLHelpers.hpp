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

#ifndef UTILITIES_CORE_URLHELPERS_HPP
#define UTILITIES_CORE_URLHELPERS_HPP

#include "../UtilitiesAPI.hpp"
#include "Path.hpp"
#include "Url.hpp"
#include <vector>

namespace openstudio {

/// Locates the given url (if relative) in one of the provided search paths and returns
/// the completed url  
UTILITIES_API Url completeURL(const Url &t_source, const std::vector<Url> &t_searchPaths, bool throwOnFailure = true);

UTILITIES_API Url toURL(const openstudio::path& p);

UTILITIES_API Url toURL(const std::string& s);

UTILITIES_API std::string toString(const Url& url);

} // openstudio

#endif //UTILITIES_CORE_URLHELPERS_HPP

