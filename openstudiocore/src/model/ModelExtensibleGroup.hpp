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

#ifndef MODEL_MODELEXTENSIBLEGROUP_HPP
#define MODEL_MODELEXTENSIBLEGROUP_HPP

#include "ModelAPI.hpp"
#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"
#include "ModelObject.hpp"
#include "ModelObject_Impl.hpp"

#include "../utilities/core/Logger.hpp"

#include <boost/optional.hpp>

namespace openstudio {
namespace model {

/** ModelExtensibleGroup is a WorkspaceExtensibleGroup with additional methods added to support
 *  extensible \link ModelObject ModelObjects\endlink. The ExtensibleGroup inheritance hierarchy
 *  roughly follows the IdfObject <- WorkspaceObject <- ModelObject <- etc. hierarchy, except that
 *  all concrete \link ModelObject ModelObjects \endlink with ExtensibleGroup derived classes
 *  derive directly from ModelExtensibleGroup--there are no extensions of ModelExtensibleGroup for
 *  intermediate abstract classes. */
class MODEL_API ModelExtensibleGroup : public WorkspaceExtensibleGroup {
 public:
  virtual ~ModelExtensibleGroup() {}

  /** @name Template Methods */
  //@{

  /** Get the ModelObject of type T pointed to from fieldIndex of this extensible group, if
   *  possible. */
  template <typename T>
  boost::optional<T> getModelObjectTarget(unsigned fieldIndex) const {
    if (!isValid(fieldIndex)) { return boost::none; }
    return ModelObject(getImpl<detail::ModelObject_Impl>()).getModelObjectTarget<T>(mf_toIndex(fieldIndex));
  }

  /** Get all the \link ModelObject ModelObjects \endlink of type T pointed to from this
   *  extensible group. Return value will always be .empty() if this extensible group object is
   *  .empty(). */
  template <typename T>
  std::vector<T> getModelObjectTargets() const {
    std::vector<T> result;
    if (empty()) { return result; }
    for (unsigned fieldIndex = 0, n = numFields(); fieldIndex < n; ++fieldIndex) {
      boost::optional<T> oTarget = getModelObjectTarget<T>(fieldIndex);
      if (oTarget) { result.push_back(*oTarget); }
    }
    return result;
  }

  //@}
 protected:
  /// @cond
  typedef detail::ModelObject_Impl ImplType;

  friend class IdfExtensibleGroup;

  ModelExtensibleGroup(std::shared_ptr<detail::ModelObject_Impl> impl,unsigned index);

  /// @endcond
 private:

  /** Private default constructor. */
  ModelExtensibleGroup();

  REGISTER_LOGGER("openstudio.model.ModelExtensibleGroup");
};

} // model
} // openstudio

#endif // MODEL_MODELEXTENSIBLEGROUP_HPPP
