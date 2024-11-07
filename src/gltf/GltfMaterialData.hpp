/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef GLTF_GLTFMATERIALDATA_HPP
#define GLTF_GLTFMATERIALDATA_HPP

#include "GltfAPI.hpp"

#include <string>
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
