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

#include "WorkspaceExtensibleGroup.hpp"
#include "WorkspaceObject.hpp"
#include "WorkspaceObject_Impl.hpp"

#include "../idd/IddObjectProperties.hpp"
#include "../idd/IddFieldProperties.hpp"

#include <boost/lexical_cast.hpp>

using std::shared_ptr;
using std::dynamic_pointer_cast;

namespace openstudio {

// GETTERS

OptionalWorkspaceObject WorkspaceExtensibleGroup::getTarget(unsigned fieldIndex) const {
  if (!isValid(fieldIndex)) { return boost::none; }
  return getImpl<detail::WorkspaceObject_Impl>()->getTarget(mf_toIndex(fieldIndex));
}

WorkspaceObjectVector WorkspaceExtensibleGroup::getTargets() const {
  WorkspaceObjectVector result;
  if (empty()) { return result; }
  for (unsigned fieldIndex = 0, n = numFields(); fieldIndex < n; ++fieldIndex) {
    OptionalWorkspaceObject owo = getTarget(fieldIndex);
    if (owo) { result.push_back(*owo); }
  }
  return result;
}

std::vector<unsigned> WorkspaceExtensibleGroup::getSourceFieldIndices(const Handle& targetHandle) const {
  UnsignedVector result;
  if (empty()) { return result; }
  UnsignedVector candidates = dynamic_pointer_cast<detail::WorkspaceObject_Impl>(m_impl)->getSourceIndices(targetHandle);
  result = mf_subsetAndToFieldIndices(candidates);
  return result;
}

// SETTERS
bool WorkspaceExtensibleGroup::setPointer(unsigned fieldIndex, const Handle& targetHandle) {
  if (!isValid(fieldIndex)) { return false; }
  return getImpl<detail::WorkspaceObject_Impl>()->setPointer(mf_toIndex(fieldIndex),targetHandle);
}

// QUERIES

bool WorkspaceExtensibleGroup::isSource() const {
  bool result = false;
  if (empty()) { return result; }
  for (unsigned fieldIndex = 0, n = numFields(); fieldIndex < n; ++fieldIndex) {
    result = getImpl<detail::WorkspaceObject_Impl>()->isSource(mf_toIndex(fieldIndex));
    if (result) { break; }
  }
  return result;
}

bool WorkspaceExtensibleGroup::canBeSource() const {
  bool result = false;
  if (empty()) { return result; }
  for (unsigned fieldIndex = 0, n = numFields(); fieldIndex < n; ++fieldIndex) {
    result = getImpl<detail::WorkspaceObject_Impl>()->canBeSource(mf_toIndex(fieldIndex));
    if (result) { break; }
  }
  return result;
}

// PROTECTED
WorkspaceExtensibleGroup::WorkspaceExtensibleGroup(
    std::shared_ptr<detail::WorkspaceObject_Impl> impl,unsigned index)
  : IdfExtensibleGroup(impl,index) {}

} // openstudio

