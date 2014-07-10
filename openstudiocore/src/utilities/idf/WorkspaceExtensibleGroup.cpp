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

