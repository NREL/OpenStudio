/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_IDF_HANDLE_HPP
#define UTILITIES_IDF_HANDLE_HPP

#include "../core/UUID.hpp"

#include <boost/optional.hpp>

#include <map>
#include <vector>
#include <set>

namespace openstudio {

/// Objects can be identified by Handle.
using Handle = openstudio::UUID;
/// Vector of Handles.
using HandleVector = std::vector<Handle>;
/// Set of Handles. Enforces uniqueness within a list.
using HandleSet = std::set<Handle>;
/// Maps Handles to Handles.
using HandleMap = std::map<Handle, Handle>;
/// Optional Handle.
using OptionalHandle = boost::optional<Handle>;
/// Optional HandleVector.
using OptionalHandleVector = boost::optional<HandleVector>;

/** Returns the handle that corresponds to original, where original is a handleMap key and the
 *  returned value is the corresponding value. If original is not a handleMap key, then the return
 *  value .isNull(). */
UTILITIES_API Handle applyHandleMap(const Handle& original, const HandleMap& handleMap);

/** Returns the HandleVector equivalent to original, where original is a vector of handleMap keys,
 *  and the return vector consists of the corresponding handleMap values. Handles in original that
 *  are not listed in handleMap are discarded (silently--compare .size() if all values should be
 *  preserved). */
UTILITIES_API HandleVector applyHandleMap(const HandleVector& original, const HandleMap& handleMap);

}  // namespace openstudio

#endif  // UTILITIES_IDF_HANDLE_HPP
