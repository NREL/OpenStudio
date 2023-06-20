/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef GLTF_GLTFMETADATA_HPP
#define GLTF_GLTFMETADATA_HPP

#include "GltfAPI.hpp"

#include "GltfModelObjectMetaData.hpp"
#include "GltfBoundingBox.hpp"

#include "../utilities/core/Logger.hpp"

namespace tinygltf {
class Value;
}

namespace openstudio {
namespace model {
  class Model;
  class PlanarSurfaceGroup;
  class BuildingStory;

}  // namespace model

namespace gltf {

  /** GltfMetadata is an Interface between an OpenStudio Model and a glTF Scene's `extras` **/
  class GLTF_API GltfMetaData
  {
   public:
    GltfMetaData();  // default ctor

    /** @name Interface with Model */
    //@{
    /** Interface with the Model **/
    GltfMetaData(const model::Model& model);
    //@}

    /** @name Getters and Setters */
    //@{

    std::vector<GltfModelObjectMetaData> glTFModelObjectMetaDataVector() const;
    void setGlTFModelObjectMetaDataVector(const std::vector<GltfModelObjectMetaData>& glTFModelObjectMetaDataVector);

    GltfBoundingBox glTFBoundingBox() const;
    void setGlTFBoundingBox(const GltfBoundingBox& glTFBoundingBox);

    std::vector<std::string> buildingStoryNames() const;
    void setBuildingStoryNames(const std::vector<std::string>& buildingStoryNames);
    // Convenience function. TODO: keep?
    void setBuildingStoryNames(const std::vector<model::BuildingStory>& buildingStories);

    std::string generator() const;
    void setGenerator(const std::string& generator);

    std::string type() const;
    void setType(const std::string& type);

    std::string version() const;
    void setVersion(const std::string& version);

    double northAxis() const;
    void setNorthAxis(double northAxis);
    void setNorthAxis(const model::Model& model);

    // Counting
    // TODO: Remove setters
    int thermalZoneCount() const;
    void setThermalZoneCount(int thermalZoneCount);

    int spaceCount() const;
    void setSpaceCount(int spaceCount);

    int spaceTypeCount() const;
    void setSpaceTypeCount(int spaceTypeCount);

    int constructionSetCount() const;
    void setConstructionSetCount(int constructionSetCount);

    int airLoopCount() const;
    void setAirLoopCount(int airLoopCount);

    int buildingStoryCount() const;
    void setBuildingStoryCount(int buildingStoryCount);

    //@}

   protected:
    /** @name Protected */
    //@{
    std::map<std::string, tinygltf::Value> toExtras() const;
    GltfMetaData(const tinygltf::Value& metaDataObj);

    //@}
    friend class GltfForwardTranslator;

   private:
    REGISTER_LOGGER("openstudio.gltf.GltfMetaData");

    std::vector<GltfModelObjectMetaData> m_glTFModelObjectMetaDataVector;
    GltfBoundingBox m_glTFBoundingBox;
    std::vector<std::string> m_buildingStoryNames;
    std::string m_generator;
    std::string m_type;
    std::string m_version;
    double m_northAxis = 0.0;
    int m_buildingStoryCount = 0;  // TODO: this is pointless, we have m_buildingStoryNames.size()
    int m_thermalZoneCount = 0;
    int m_spaceCount = 0;
    int m_spaceTypeCount = 0;
    int m_constructionSetCount = 0;
    int m_airLoopCount = 0;
    int m_buildingUnitCount = 0;
  };
}  // namespace gltf
}  // namespace openstudio

#endif  //GLTF_GLTFMETADATA_HPP
