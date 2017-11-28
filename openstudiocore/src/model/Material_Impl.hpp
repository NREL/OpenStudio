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

#ifndef MODEL_MATERIAL_IMPL_HPP
#define MODEL_MATERIAL_IMPL_HPP


#include "ResourceObject_Impl.hpp"

#include <QObject>

namespace openstudio {
namespace model {

// forward declaration
class Material;
class StandardsInformationMaterial;

namespace detail {

  class MODEL_API Material_Impl : public ResourceObject_Impl {
    

    

    
    
    
   public:

    /** @name Constructors and Destructors */
    //@{

    // Construct completely new object.
    Material_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    // Construct from existing workspace object (typically when Model is being constructed 
    // from Workspace).
    Material_Impl(const openstudio::detail::WorkspaceObject_Impl& other, 
                  Model_Impl* model, 
                  bool keepHandle);

    // Clone copy constructor.
    Material_Impl(const Material_Impl& other, Model_Impl* model, bool keepHandle);

    virtual ~Material_Impl() {}

    //@}
    /** @name Getters */
    //@{

    // return any children objects in the hierarchy
    virtual std::vector<ModelObject> children() const override;

    /** Get the thickness of the material. For some materials, 0.0 is always returned. */
    virtual double thickness() const;

    virtual boost::optional<double> getVisibleTransmittance() const = 0;

    virtual boost::optional<double> interiorVisibleAbsorptance() const;

    virtual boost::optional<double> exteriorVisibleAbsorptance() const;

    StandardsInformationMaterial standardsInformation() const;

    //@}
    /** @name Setters */
    //@{

    /** Set thickness to value (m). For some materials, false is always returned. */
    virtual bool setThickness(double value);

    // if material property moisture penetration depth settings already exists, do nothing and return nil; creates the material property moisture penetration depth settings if it does not already exist and return it;
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

   private:

    REGISTER_LOGGER("openstudio.model.Material");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_MATERIAL_IMPL_HPP
