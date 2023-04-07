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

#ifndef MODEL_SPACELOADINSTANCE_IMPL_HPP
#define MODEL_SPACELOADINSTANCE_IMPL_HPP

#include "SpaceLoad_Impl.hpp"

namespace openstudio {
namespace model {

  // forward declaration
  class SpaceLoadDefinition;
  class SpaceLoadInstance;

  namespace detail {

    class MODEL_API SpaceLoadInstance_Impl : public SpaceLoad_Impl
    {

     public:
      // Construct completely new object.
      SpaceLoadInstance_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // Construct from existing workspace object (typically when Model is being constructed
      // from Workspace).
      SpaceLoadInstance_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // Clone copy constructor.
      SpaceLoadInstance_Impl(const SpaceLoadInstance_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SpaceLoadInstance_Impl() = default;

      /** Returns the definition of this instance. */
      SpaceLoadDefinition definition() const;

      /** Sets the definition of this instance. */
      virtual bool setDefinition(const SpaceLoadDefinition& definition) = 0;

      /** Ensures that no other instances share this instances definition. */
      void makeUnique();

      /** Gets the multiplier for this space instance, defaults to 1. **/
      virtual double multiplier() const = 0;

      /** Returns true if the multiplier is defaulted. **/
      virtual bool isMultiplierDefaulted() const = 0;

      /** Returns the floor area associated with this space load instance.
    If this space load is associated with a single space then that space's floor area is returned.
    If this space load is associated with a space type then the sum of all space floor area in that space type is returned.
    Space and SpaceLoadInstance multipliers are included in the result.**/
      double floorArea() const;

      /** Returns the number of instances this space load instance represents.
    Space and SpaceLoadInstance multipliers are included in the result **/
      int quantity() const;

     protected:
      friend class openstudio::model::SpaceLoadInstance;

      // index of the definition name
      virtual int definitionIndex() const = 0;

     private:
      REGISTER_LOGGER("openstudio.model.SpaceLoadInstance");

      boost::optional<ModelObject> definitionAsModelObject() const;

      bool setDefinitionAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SPACELOADINSTANCE_IMPL_HPP
