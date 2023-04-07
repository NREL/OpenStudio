/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef UTILITIES_IDF_OBJECTPOINTER_HPP
#define UTILITIES_IDF_OBJECTPOINTER_HPP

#include "Handle.hpp"

namespace openstudio {

/** Data structure for specifying connections between IdfWorkspace and other object collections.
 *  Other object collections include other IdfWorkspaces, IdfFile, IdfObjectVector. */
template <class T, class U>
struct ObjectPointer
{
  using source_type = T;
  using target_type = U;

  T source;
  unsigned fieldIndex;
  U target;

  ObjectPointer() = default;
  ObjectPointer(const T& s, unsigned fi, const U& t) : source(s), fieldIndex(fi), target(t) {}
};

template <class T>
struct FieldIndexLess
{
  bool operator()(const T& left, const T& right) const {
    return (left.fieldIndex < right.fieldIndex);
  }
};

template <class T>
bool fieldIndexEqualTo(const T& object, unsigned value) {
  return (object.fieldIndex == value);
}

/** ObjectPointer with source as index in IdfObjectVector, target as Handle. For Workspace, source
 *  will be outside of workspace, target will be within workspace. */
using UHPointer = ObjectPointer<unsigned int, Handle>;
/** ObjectPointer with source as Handle, target as index in IdfObjectVector. For Workspace, source
 *  will be within workspace, target will be outside of workspace. */
using HUPointer = ObjectPointer<Handle, unsigned int>;
/** ObjectPointer with source and target as Handles. For Workspace, both objects must be in a
 *  workspace. */
using HHPointer = ObjectPointer<Handle, Handle>;

using UHPointerVector = std::vector<UHPointer>;
using HUPointerVector = std::vector<HUPointer>;
using HHPointerVector = std::vector<HHPointer>;

}  // namespace openstudio

#endif  // UTILITIES_IDF_OBJECTPOINTER_HPP
