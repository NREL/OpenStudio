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

#ifndef UTILITIES_IDF_HANDLE_HPP
#define UTILITIES_IDF_HANDLE_HPP

#include "../core/UUID.hpp"

#include <boost/optional.hpp>

#include <map>
#include <vector>
#include <set>

namespace openstudio {

/// Objects can be identified by Handle.
typedef openstudio::UUID Handle;
/// Vector of Handles.
typedef std::vector<Handle> HandleVector;
/// Set of Handles. Enforces uniqueness within a list.
typedef std::set<Handle> HandleSet;
/// Maps Handles to Handles.
typedef std::map<Handle,Handle> HandleMap;
/// Optional Handle.
typedef boost::optional<Handle> OptionalHandle;
/// Optional HandleVector.
typedef boost::optional<HandleVector> OptionalHandleVector;

/** Returns the handle that corresponds to original, where original is a handleMap key and the
 *  returned value is the corresponding value. If original is not a handleMap key, then the return
 *  value .isNull(). */
UTILITIES_API Handle applyHandleMap(const Handle& original, const HandleMap& handleMap);

/** Returns the HandleVector equivalent to original, where original is a vector of handleMap keys, 
 *  and the return vector consists of the corresponding handleMap values. Handles in original that 
 *  are not listed in handleMap are discarded (silently--compare .size() if all values should be 
 *  preserved). */
UTILITIES_API HandleVector applyHandleMap(const HandleVector& original,const HandleMap& handleMap);

} // openstudio

#endif // UTILITIES_IDF_HANDLE_HPP
