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

#ifndef MODEL_MATERIALPROPERTYGLAZINGSPECTRALDATA_HPP
#define MODEL_MATERIALPROPERTYGLAZINGSPECTRALDATA_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class MaterialPropertyGlazingSpectralData_Impl;

  }  // namespace detail

  class MODEL_API SpectralDataField
  {
   public:
    SpectralDataField(double wavelength, double transmittance, double frontReflectance, double backReflectance);

    double wavelength() const;
    double transmittance() const;
    double frontReflectance() const;
    double backReflectance() const;

   private:
    double m_wavelength;
    double m_transmittance;
    double m_frontReflectance;
    double m_backReflectance;
  };

  /** MaterialPropertyGlazingSpectralData is a ResourceObject that wraps the OpenStudio IDD object 'OS:MaterialProperty:GlazingSpectralData'. */
  class MODEL_API MaterialPropertyGlazingSpectralData : public ResourceObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit MaterialPropertyGlazingSpectralData(const Model& model);

    virtual ~MaterialPropertyGlazingSpectralData() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    MaterialPropertyGlazingSpectralData(const MaterialPropertyGlazingSpectralData& other) = default;
    MaterialPropertyGlazingSpectralData(MaterialPropertyGlazingSpectralData&& other) = default;
    MaterialPropertyGlazingSpectralData& operator=(const MaterialPropertyGlazingSpectralData&) = default;
    MaterialPropertyGlazingSpectralData& operator=(MaterialPropertyGlazingSpectralData&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    std::vector<SpectralDataField> spectralDataFields() const;

    //@}
    /** @name Setters */
    //@{

    bool setSpectralDataFields(const std::vector<SpectralDataField>& spectralDataFields);

    bool addSpectralDataField(const SpectralDataField& spectralDataField);

    bool addSpectralDataField(double wavelength, double transmittance, double frontReflectance, double backReflectance);

    void resetSpectralDataFields();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::MaterialPropertyGlazingSpectralData_Impl;

    explicit MaterialPropertyGlazingSpectralData(std::shared_ptr<detail::MaterialPropertyGlazingSpectralData_Impl> impl);

    friend class detail::MaterialPropertyGlazingSpectralData_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.MaterialPropertyGlazingSpectralData");
  };

  /** \relates MaterialPropertyGlazingSpectralData*/
  using OptionalMaterialPropertyGlazingSpectralData = boost::optional<MaterialPropertyGlazingSpectralData>;

  /** \relates MaterialPropertyGlazingSpectralData*/
  using MaterialPropertyGlazingSpectralDataVector = std::vector<MaterialPropertyGlazingSpectralData>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_MATERIALPROPERTYGLAZINGSPECTRALDATA_HPP
