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

#ifndef UTILITIES_IDF_WORKSPACEEXTENSIBLEGROUP_HPP
#define UTILITIES_IDF_WORKSPACEEXTENSIBLEGROUP_HPP

#include "../UtilitiesAPI.hpp"
#include "../idd/ExtensibleIndex.hpp"
#include "IdfExtensibleGroup.hpp"

#include "../core/Logger.hpp"
#include "../core/Optional.hpp"
#include "../core/Containers.hpp"

#include <boost/optional.hpp>

namespace openstudio {

namespace detail {
  class WorkspaceObject_Impl;
}
class WorkspaceObject;

/** WorkspaceExtensibleGroup wraps a set of extensible fields in a WorkspaceObject. */
class UTILITIES_API WorkspaceExtensibleGroup : public IdfExtensibleGroup
{
 public:
  virtual ~WorkspaceExtensibleGroup() = default;

  /** @name Getters */
  //@{

  /** Returns the object pointed to by field index, if it exists. */
  boost::optional<WorkspaceObject> getTarget(unsigned fieldIndex) const;

  /** Returns all objects pointed to by this object. */
  std::vector<WorkspaceObject> getTargets() const;

  /** Returns the fieldIndex of the field in this extensible group that points to targetHandle,
   *  if possible. */
  std::vector<unsigned> getSourceFieldIndices(const Handle& targetHandle) const;

  /** Like getString except for reference fields getString will return the
   *  name of the referenced object. This method, getField, will always return the string value
   *  of the field.
   */
  boost::optional<std::string> getField(unsigned index) const;

  //@}
  /** @name Setters */
  //@{

  /** Sets the pointer at fieldIndex to point to target, if possible. The field must be of
   *  object-list type, and target must be valid (in the containing Workspace, and if the strictness
   *  is greater than enums::None, of a proper type). */
  bool setPointer(unsigned fieldIndex, const Handle& targetHandle);

  bool setPointer(unsigned fieldIndex, const Handle& targetHandle, bool checkValidity);

  //@}
  /** @name Queries */
  //@{

  /** Returns true if this ExtensibleGroup points to another object. To qualify, there must be at
   *  least one pointer field holding a non-null pointer. */
  bool isSource() const;

  /** Returns true if this object can point to another object. To qualify, there must be at least
   *  one object-list field. */
  bool canBeSource() const;

  //@}

 protected:
  using ImplType = detail::WorkspaceObject_Impl;

  friend class IdfExtensibleGroup;

  WorkspaceExtensibleGroup(std::shared_ptr<detail::WorkspaceObject_Impl> impl, unsigned index);

 private:
  /** Private default constructor. */
  WorkspaceExtensibleGroup();

  REGISTER_LOGGER("openstudio.WorkspaceExtensibleGroup");
};

}  // namespace openstudio

#endif  // UTILITIES_IDF_WORKSPACEEXTENSIBLEGROUP_HPP
