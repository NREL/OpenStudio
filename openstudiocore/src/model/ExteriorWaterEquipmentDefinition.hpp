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

#ifndef MODEL_EXTERIORWATEREQUIPMENTDEFINITION_HPP
#define MODEL_EXTERIORWATEREQUIPMENTDEFINITION_HPP

#include "ModelAPI.hpp"
#include "ExteriorLoadDefinition.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class ExteriorWaterEquipmentDefinition_Impl;

} // detail

/** ExteriorWaterEquipmentDefinition is a ExteriorLoadDefinition that wraps the OpenStudio IDD object
 *  'OS:Exterior:WaterEquipment:Definition'. */
class MODEL_API ExteriorWaterEquipmentDefinition : public ExteriorLoadDefinition {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Defaults design level to 0.0 W. */
  explicit ExteriorWaterEquipmentDefinition(const Model& model);

  virtual ~ExteriorWaterEquipmentDefinition() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  double designLevel() const;

  //@}
  /** @name Setters */
  //@{

  bool setDesignLevel(double designLevel);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::ExteriorWaterEquipmentDefinition_Impl ImplType;

  explicit ExteriorWaterEquipmentDefinition(std::shared_ptr<detail::ExteriorWaterEquipmentDefinition_Impl> impl);

  friend class detail::ExteriorWaterEquipmentDefinition_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ExteriorWaterEquipmentDefinition");
};

/** \relates ExteriorWaterEquipmentDefinition*/
typedef boost::optional<ExteriorWaterEquipmentDefinition> OptionalExteriorWaterEquipmentDefinition;

/** \relates ExteriorWaterEquipmentDefinition*/
typedef std::vector<ExteriorWaterEquipmentDefinition> ExteriorWaterEquipmentDefinitionVector;

} // model
} // openstudio

#endif // MODEL_EXTERIORWATEREQUIPMENTDEFINITION_HPP

