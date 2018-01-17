/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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
