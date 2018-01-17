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

#ifndef MODEL_EXTERIORLOADINSTANCE_IMPL_HPP
#define MODEL_EXTERIORLOADINSTANCE_IMPL_HPP

#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

// forward declaration
class ExteriorLoadDefinition;
class ExteriorLoadInstance;

namespace detail {

  class MODEL_API ExteriorLoadInstance_Impl : public ModelObject_Impl {

   public:

    // Construct completely new object.
    ExteriorLoadInstance_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    // Construct from existing workspace object (typically when Model is being constructed
    // from Workspace).
    ExteriorLoadInstance_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                        Model_Impl* model,
                        bool keepHandle);

    // Clone copy constructor.
    ExteriorLoadInstance_Impl(const ExteriorLoadInstance_Impl& other, Model_Impl* model, bool keepHandle);

    virtual ~ExteriorLoadInstance_Impl() {}

    /** Returns the definition of this instance. */
    ExteriorLoadDefinition definition() const;

    /** Sets the definition of this instance. */
    virtual bool setDefinition(const ExteriorLoadDefinition& definition) = 0;

    /** Ensures that no other instances share this instances definition. */
    void makeUnique();

    virtual double multiplier() const = 0;

    virtual bool isMultiplierDefaulted() const = 0;

    int quantity() const;

   protected:

    friend class openstudio::model::ExteriorLoadInstance;

    // index of the definition name
    virtual int definitionIndex() const = 0;

   private:
    REGISTER_LOGGER("openstudio.model.ExteriorLoadInstance");

    boost::optional<ModelObject> definitionAsModelObject() const;

    bool setDefinitionAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_EXTERIORLOADINSTANCE_IMPL_HPP
