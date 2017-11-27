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

#ifndef MODEL_MATERIAL_HPP
#define MODEL_MATERIAL_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

#include "MaterialPropertyMoisturePenetrationDepthSettings.hpp"
#include "MaterialPropertyMoisturePenetrationDepthSettings_Impl.hpp"

namespace openstudio {
namespace model {

  class StandardsInformationMaterial;
  class MaterialPropertyMoisturePenetrationDepthSettings;

namespace detail{
  class Material_Impl;
}

/** A Material is a ResourceObject that serves as a base class for all objects that can be used
 *  in \link LayeredConstruction LayeredConstructions \endlink. It also provides Attributes for 
 *  'thickness', 'getVisibleTransmiattance', 'interiorVisibleAbsorptance', and 
 *  'exteriorVisibleAbsorptance'. */
class MODEL_API Material : public ResourceObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~Material() {}

  //@}
  /** @name Getters */
  //@{

  /** Get the thickness of the material. Virtual implementation. For some materials, 0.0 is always 
   *  returned. 
   *
   *  Attribute Name: 'thickness' */
  double thickness() const;

  /** Get the visible transmittance of the material. Virtual implementation.
   *
   *  Attribute Name: 'getVisibleTransmittance' */
  boost::optional<double> getVisibleTransmittance() const;

  /** Get the interiorVisibleAbsorptance of the material. Virtual implementation. 
   *
   *  Attribute Name: 'interiorVisibleAbsorptance' */
  boost::optional<double> interiorVisibleAbsorptance() const;

  /** Get the exteriorVisibleAbsorptance of the material. Virtual implementation. 
   *
   *  Attribute Name: 'exteriorVisibleAbsorptance' */
  boost::optional<double> exteriorVisibleAbsorptance() const;

  /** Returns this materials's standards information, constructing a new object if necessary. */
  StandardsInformationMaterial standardsInformation() const;

  //@}
  /** @name Setters */
  //@{

  /** Set thickness to value (m). For some materials, false is always returned. 
   *
   *  Attribute Name: 'thickness' */
  bool setThickness(double value);

  // if material property moisture penetration depth settings already exists, do nothing and return nil; creates the material property moisture penetration depth settings if it does not already exist and return it
  boost::optional<MaterialPropertyMoisturePenetrationDepthSettings> createMaterialPropertyMoisturePenetrationDepthSettings(double waterVaporDiffusionResistanceFactor,
                                                                                                                           double moistureEquationCoefficientA,
                                                                                                                           double moistureEquationCoefficientB,
                                                                                                                           double moistureEquationCoefficientC,
                                                                                                                           double moistureEquationCoefficientD,
                                                                                                                           double coatingLayerThickness,
                                                                                                                           double coatingLayerWaterVaporDiffusionResistanceFactor);

  // returns the material property moisture penetration depth settings if set
  boost::optional<MaterialPropertyMoisturePenetrationDepthSettings> materialPropertyMoisturePenetrationDepthSettings() const;

  // resets the material property moisture penetration depth settings
  void resetMaterialPropertyMoisturePenetrationDepthSettings();
  
  //@}
 protected:
  /// @cond
  typedef detail::Material_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  friend class detail::Material_Impl;

  Material(IddObjectType type,const Model& model);

  explicit Material(std::shared_ptr<detail::Material_Impl> impl);  

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.Material");
};

/** \relates Material */
typedef boost::optional<Material> OptionalMaterial;

/** \relates Material */
typedef std::vector<Material> MaterialVector;

} // model
} // openstudio

#endif // MODEL_MATERIAL_HPP
