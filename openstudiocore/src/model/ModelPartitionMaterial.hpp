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

#ifndef MODEL_MODELPARTITIONMATERIAL_HPP
#define MODEL_MODELPARTITIONMATERIAL_HPP

#include "ModelAPI.hpp"
#include "Material.hpp"

namespace openstudio {
namespace model {

namespace detail{
  class ModelPartitionMaterial_Impl;
}

/** ModelPartitionMaterial is an abstract class derived from Material.
 *
 *  The ModelPartitionMaterial class is the base class for materials that should
 *  exist as the only layer in a Construction.  ModelPartitionMaterial objects
 *  are typically used for the \link Construction Constructions\endlink of \link Surface Surfaces\endlink
 *  that are virtual boundaries between two \link Space Spaces\endlink. */
class MODEL_API ModelPartitionMaterial : public Material {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~ModelPartitionMaterial() {}

  //@}
 protected:
  /// @cond
  typedef detail::ModelPartitionMaterial_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class detail::ModelPartitionMaterial_Impl;

  ModelPartitionMaterial(IddObjectType type, const Model& model);

  explicit ModelPartitionMaterial(std::shared_ptr<detail::ModelPartitionMaterial_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ModelPartitionMaterial");
};

/** \relates ModelPartitionMaterial */
typedef boost::optional<ModelPartitionMaterial> OptionalModelPartitionMaterial;

/** \relates ModelPartitionMaterial */
typedef std::vector<ModelPartitionMaterial> ModelPartitionMaterialVector;

} // model
} // openstudio

#endif // MODEL_MODELPARTITIONMATERIAL_HPP
