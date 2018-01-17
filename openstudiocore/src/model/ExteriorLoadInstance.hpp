/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef MODEL_EXTERIORLOADINSTANCE_HPP
#define MODEL_EXTERIORLOADINSTANCE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

class ExteriorLoadDefinition;

namespace detail{
  class ExteriorLoadInstance_Impl;
}

/** ExteriorLoadInstance is an abstract class derived from ModelObject. \link ExteriorLoadInstance
 *  ExteriorLoadInstances \endlink instantiate a given ExteriorLoadDefinition in the facility
 *  by defining a multiplier and a schedule.
 */
class MODEL_API ExteriorLoadInstance : public ModelObject {
 public:
  virtual ~ExteriorLoadInstance() {}

  /** Returns the definition of this instance. **/
  ExteriorLoadDefinition definition() const;

  /** Sets the definition of this instance. **/
  bool setDefinition(const ExteriorLoadDefinition& definition);

  /** Ensures that no other instances share this instances definition. **/
  void makeUnique();

  /** Gets the multiplier for this space instance, defaults to 1. **/
  double multiplier() const;

  /** Returns true if the multiplier is defaulted. **/
  bool isMultiplierDefaulted() const;

  /** Returns the number of instances this space load instance represents.
  This just forwards to multiplier() here but is included for consistency with SpaceLoadInstance**/
  int quantity() const;

 protected:

  /** @name Constructors and Destructors */
  //@{

  /// Constructs a new ExteriorLoadInstance object in the model.
  ExteriorLoadInstance(IddObjectType type, const ExteriorLoadDefinition& definition);


  //@}

  /// @cond

  typedef detail::ExteriorLoadInstance_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class detail::ExteriorLoadInstance_Impl;

  explicit ExteriorLoadInstance(std::shared_ptr<detail::ExteriorLoadInstance_Impl> impl);

 private:

  REGISTER_LOGGER("openstudio.model.ExteriorLoadInstance");

  /// @endcond

};

/** \relates ExteriorLoadInstance */
typedef boost::optional<ExteriorLoadInstance> OptionalExteriorLoadInstance;

/** \relates ExteriorLoadInstance */
typedef std::vector<ExteriorLoadInstance> ExteriorLoadInstanceVector;

} // model
} // openstudio

#endif // MODEL_EXTERIORLOADINSTANCE_HPP
