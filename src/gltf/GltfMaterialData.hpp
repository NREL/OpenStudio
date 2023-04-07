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
#ifndef GLTF_GLTFMATERIALDATA_HPP
#define GLTF_GLTFMATERIALDATA_HPP

#include "GltfAPI.hpp"

#include <array>
#include <string_view>
#include <vector>

namespace tinygltf {
struct Material;
}

namespace openstudio {

namespace model {
  class Model;
  class RenderingColor;
}  // namespace model

namespace gltf {

  class GltfForwardTranslator;

  /** GltfMaterialData is an Interface class between a `RenderingColor` and a `tinygltf::Material` **/
  class GLTF_API GltfMaterialData
  {
   public:
    /** @name Interface with Model and ModelObjects */
    //@{

    /** Standard constructor */
    constexpr GltfMaterialData(std::string_view materialName, int r, int g, int b, double a, bool isDoubleSided = false)
      : m_materialName(materialName), m_r(r), m_g(g), m_b(b), m_a(a), m_isDoubleSided(isDoubleSided){};

    static std::vector<GltfMaterialData> buildMaterials(const model::Model& model);
    //@}

    GltfMaterialData(std::string_view materialName, const model::RenderingColor& color, bool isDoubleSided = false);

    /** @name Getters and Setters */
    //@{
    std::string materialName() const;
    void setMaterialName(const std::string& materialName);

    int r() const;
    void setR(int r);

    int g() const;
    void setG(int g);

    int b() const;
    void setB(int b);

    double a() const;
    void setA(double a);

    bool isDoubleSided() const;
    void setIsDoubleSided(bool isDoubleSided);

   protected:
    /** @name Protected */
    //@{

    /** Creates a GLTF material on the basis of raw Material Values, i.e, R, G, B, A & isDoubleSided */
    tinygltf::Material toGltf() const;
    friend class GltfForwardTranslator;

    //@}

   private:
    std::string_view m_materialName;
    int m_r;     // [0, 255]
    int m_g;     // [0, 255]
    int m_b;     // [0, 255]
    double m_a;  // [0, 1]     -> TODO: make that 0, 255 (int) like RenderingColor does
    bool m_isDoubleSided = false;
  };

  /** \relates GltfMaterialData */
  GLTF_API std::ostream& operator<<(std::ostream& out, const GltfMaterialData& gltfMaterialData);

  using GltfMaterialDataVector = std::vector<GltfMaterialData>;

}  // namespace gltf
}  // namespace openstudio
#endif  // GLTF_GLTFMATERIALDATA_HPP
