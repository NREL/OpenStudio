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

#ifndef MODEL_MATERIALPROPERTYMOISTUREPENETRATIONDEPTHSETTINGS_HPP
#define MODEL_MATERIALPROPERTYMOISTUREPENETRATIONDEPTHSETTINGS_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class MaterialPropertyMoisturePenetrationDepthSettings_Impl;

} // detail

/** MaterialPropertyMoisturePenetrationDepthSettings is a ResourceObject that wraps the OpenStudio IDD object 'OS:MaterialProperty:MoisturePenetrationDepth:Settings'. */
class MODEL_API MaterialPropertyMoisturePenetrationDepthSettings : public ResourceObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~MaterialPropertyMoisturePenetrationDepthSettings() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  double waterVaporDiffusionResistanceFactor() const;

  double moistureEquationCoefficientA() const;

  double moistureEquationCoefficientB() const;

  double moistureEquationCoefficientC() const;

  double moistureEquationCoefficientD() const;
  
  std::vector<double> moistureEquationCoefficients() const;

  boost::optional<double> surfaceLayerPenetrationDepth();

  bool isSurfaceLayerPenetrationDepthAutocalculated();

  boost::optional<double> deepLayerPenetrationDepth();

  bool isDeepLayerPenetrationDepthAutocalculated();

  double coatingLayerThickness() const;

  double coatingLayerWaterVaporDiffusionResistanceFactor() const;

  //@}
  /** @name Setters */
  //@{

  void setWaterVaporDiffusionResistanceFactor(double waterVaporDiffusionResistanceFactor);

  void setMoistureEquationCoefficientA(double moistureEquationCoefficientA);

  void setMoistureEquationCoefficientB(double moistureEquationCoefficientB);

  void setMoistureEquationCoefficientC(double moistureEquationCoefficientC);

  void setMoistureEquationCoefficientD(double moistureEquationCoefficientD);
  
  void setMoistureEquationCoefficients(double moistureEquationCoefficientA, double moistureEquationCoefficientB, double moistureEquationCoefficientC, double moistureEquationCoefficientD);

  void setSurfaceLayerPenetrationDepth(double value);

  void autocalculateSurfaceLayerPenetrationDepth();

  void setDeepLayerPenetrationDepth(double value);

  void autocalculateDeepLayerPenetrationDepth();

  void setCoatingLayerThickness(double coatingLayerThickness);

  void setCoatingLayerWaterVaporDiffusionResistanceFactor(double coatingLayerWaterVaporDiffusionResistanceFactor);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::MaterialPropertyMoisturePenetrationDepthSettings_Impl ImplType;

  explicit MaterialPropertyMoisturePenetrationDepthSettings(std::shared_ptr<detail::MaterialPropertyMoisturePenetrationDepthSettings_Impl> impl);

  friend class detail::MaterialPropertyMoisturePenetrationDepthSettings_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.MaterialPropertyMoisturePenetrationDepthSettings");
};

/** \relates MaterialPropertyMoisturePenetrationDepthSettings*/
typedef boost::optional<MaterialPropertyMoisturePenetrationDepthSettings> OptionalMaterialPropertyMoisturePenetrationDepthSettings;

/** \relates MaterialPropertyMoisturePenetrationDepthSettings*/
typedef std::vector<MaterialPropertyMoisturePenetrationDepthSettings> MaterialPropertyMoisturePenetrationDepthSettingsVector;

} // model
} // openstudio

#endif // MODEL_MATERIALPROPERTYMOISTUREPENETRATIONDEPTHSETTINGS_HPP