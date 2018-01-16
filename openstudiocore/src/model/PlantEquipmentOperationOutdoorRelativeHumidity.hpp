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

#ifndef MODEL_PLANTEQUIPMENTOPERATIONOUTDOORRELATIVEHUMIDITY_HPP
#define MODEL_PLANTEQUIPMENTOPERATIONOUTDOORRELATIVEHUMIDITY_HPP

#include "ModelAPI.hpp"
#include "PlantEquipmentOperationRangeBasedScheme.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class PlantEquipmentOperationOutdoorRelativeHumidity_Impl;

} // detail

/** PlantEquipmentOperationOutdoorRelativeHumidity is a PlantEquipmentOperationRangeBasedScheme that wraps the OpenStudio IDD object 'OS:PlantEquipmentOperation:OutdoorRelativeHumidity'. */
class MODEL_API PlantEquipmentOperationOutdoorRelativeHumidity : public PlantEquipmentOperationRangeBasedScheme {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit PlantEquipmentOperationOutdoorRelativeHumidity(const Model& model);

  virtual ~PlantEquipmentOperationOutdoorRelativeHumidity() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{


  //@}
  /** @name Setters */
  //@{


  //@}
  /** @name Other */
  //@{


  //@}
 protected:
  /// @cond
  typedef detail::PlantEquipmentOperationOutdoorRelativeHumidity_Impl ImplType;

  explicit PlantEquipmentOperationOutdoorRelativeHumidity(std::shared_ptr<detail::PlantEquipmentOperationOutdoorRelativeHumidity_Impl> impl);

  friend class detail::PlantEquipmentOperationOutdoorRelativeHumidity_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.PlantEquipmentOperationOutdoorRelativeHumidity");
};

/** \relates PlantEquipmentOperationOutdoorRelativeHumidity*/
typedef boost::optional<PlantEquipmentOperationOutdoorRelativeHumidity> OptionalPlantEquipmentOperationOutdoorRelativeHumidity;

/** \relates PlantEquipmentOperationOutdoorRelativeHumidity*/
typedef std::vector<PlantEquipmentOperationOutdoorRelativeHumidity> PlantEquipmentOperationOutdoorRelativeHumidityVector;

} // model
} // openstudio

#endif // MODEL_PLANTEQUIPMENTOPERATIONOUTDOORRELATIVEHUMIDITY_HPP

