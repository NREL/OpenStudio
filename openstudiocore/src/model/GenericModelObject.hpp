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

#ifndef MODEL_GENERICMODELOBJECT_HPP
#define MODEL_GENERICMODELOBJECT_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {
namespace detail {
  class GenericModelObject_Impl;
} // detail

/** GenericModelObject is a generic interface for \link ModelObject ModelObjects\endlink
 *  that do not yet have a customized interface. */

/** GenericModelObject derives from ModelObject and is a generic interface to any OpenStudio IDD object..
 *
 *  GenericModelObject is different from all other \link ModelObject ModelObjects \endlink in that
 *  it does not wrap a specific OpenStudio IDD object.  Instead it is a generic wrapper that can hold
 *  OpenStudio IDD objects of any type.  When objects are added to Model, a large switch statement is
 *  used to construct wrappers of the correct type for each new object.  If the type of a newly added
 *  object has not yet been wrapped by a specific ModelObject then a GenericModelObject is used to hold
 *  a reference to the data.  GenericModelObject is planned to be deprecated once all OpenStudio IDD types
 *  are wrapped by specific \link ModelObject ModelObjects \endlink, this class only exists in the interim
 *  so translation to and from EnergyPlus via the implicit translator does not lose data objects.
 *
 */
class MODEL_API GenericModelObject : public ModelObject {
 public:
  virtual ~GenericModelObject() {}

 protected:

  /// @cond

  typedef detail::GenericModelObject_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  // constructor
  explicit GenericModelObject(std::shared_ptr<detail::GenericModelObject_Impl> impl);

 private:

  REGISTER_LOGGER("openstudio.model.GenericModelObject");

  /// @endcond

};

// optional GenericModelObject
typedef boost::optional<GenericModelObject> OptionalGenericModelObject;

// vector of GenericModelObject
typedef std::vector<GenericModelObject> GenericModelObjectVector;

} // model
} // openstudio

#endif //MODEL_GENERICMODELOBJECT_HPP
