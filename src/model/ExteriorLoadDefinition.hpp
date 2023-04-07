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

#ifndef MODEL_EXTERIORLOADDEFINITION_HPP
#define MODEL_EXTERIORLOADDEFINITION_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

namespace openstudio {
namespace model {

  class ExteriorLoadInstance;

  namespace detail {
    class ExteriorLoadDefinition_Impl;
  }

  /** ExteriorLoadDefinition is an abstract class derived from ResourceObject. \link ExteriorLoadDefinition
 *  ExteriorLoadDefinitions \endlink describe internal loads in a generic way that can be used by
 *  multiple \link ExteriorLoadInstance ExteriorLoadInstances \endlink. */
  class MODEL_API ExteriorLoadDefinition : public ResourceObject
  {
   public:
    virtual ~ExteriorLoadDefinition() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ExteriorLoadDefinition(const ExteriorLoadDefinition& other) = default;
    ExteriorLoadDefinition(ExteriorLoadDefinition&& other) = default;
    ExteriorLoadDefinition& operator=(const ExteriorLoadDefinition&) = default;
    ExteriorLoadDefinition& operator=(ExteriorLoadDefinition&&) = default;

    /** Returns all instances (ExteriorLoad) of this definition. */
    std::vector<ExteriorLoadInstance> instances() const;

    /** Returns the number of instances of this exterior load definition.
  ExteriorLoad multipliers are included in the result **/
    int quantity() const;

   protected:
    /** @name Constructors and Destructors */
    //@{

    /// Constructs a new ExteriorLoadDefinition object in the model.
    ExteriorLoadDefinition(IddObjectType type, const Model& model);

    //@}

    /// @cond

    using ImplType = detail::ExteriorLoadDefinition_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class detail::ExteriorLoadDefinition_Impl;

    explicit ExteriorLoadDefinition(std::shared_ptr<detail::ExteriorLoadDefinition_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.ExteriorLoadDefinition");

    /// @endcond
  };

  /** \relates ExteriorLoadDefinition */
  using OptionalExteriorLoadDefinition = boost::optional<ExteriorLoadDefinition>;

  /** \relates ExteriorLoadDefinition */
  using ExteriorLoadDefinitionVector = std::vector<ExteriorLoadDefinition>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EXTERIORLOADDEFINITION_HPP
