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
