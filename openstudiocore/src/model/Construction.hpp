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

#ifndef MODEL_CONSTRUCTION_HPP
#define MODEL_CONSTRUCTION_HPP

#include "ModelAPI.hpp"
#include "LayeredConstruction.hpp"

namespace openstudio {
namespace model {

namespace detail{
  class Construction_Impl;
}

/** Construction is a LayeredConstruction that wraps the EnergyPlus IDD object 'Construction'.
 *  A Construction consists of a number of material layers, each of which should belong to the
 *  same base class: OpaqueMaterial, FenestrationMaterial, or ModelPartitionMaterial. In addition
 *  the layers must satisfy some restrictions in order to form a valid EnergyPlus construction,
 *  see the documentation for LayeredConstruction. */
class MODEL_API Construction : public LayeredConstruction {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit Construction(const Model& model);

  explicit Construction(const std::vector<OpaqueMaterial>& opaqueMaterials);

  explicit Construction(const std::vector<FenestrationMaterial>& fenestrationMaterials);

  explicit Construction(const ModelPartitionMaterial& modelPartitionMaterial);

  virtual ~Construction() {}

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  //@}

  /** Searches the model for a layered construction with layers which are the reverse of this one.  Returns that construction
   *  if found, creates one if not. */
  Construction reverseConstruction() const;

 protected:
  /// @cond
  typedef detail::Construction_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  friend class detail::Construction_Impl;

  explicit Construction(std::shared_ptr<detail::Construction_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.Construction");
};

/** \relates Construction */
typedef boost::optional<Construction> OptionalConstruction;

/** \relates Construction */
typedef std::vector<Construction> ConstructionVector;

} // model
} // openstudio

#endif // MODEL_CONSTRUCTION_HPP
