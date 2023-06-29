/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef GLTF_GLTFBOUNDINGBOX_HPP
#define GLTF_GLTFBOUNDINGBOX_HPP

#include "GltfAPI.hpp"

#include "../utilities/core/Logger.hpp"

#include <vector>

namespace tinygltf {
class Value;
}

namespace openstudio {
namespace model {
  class PlanarSurfaceGroup;
}  // namespace model

namespace gltf {

  class GltfMetaData;

  /** GltfBoundingBox is an Interface between the PlanarSurfaceGroups of a Model and a glTF Scene's extras to set the boundingbox **/
  class GLTF_API GltfBoundingBox
  {
   public:
    GltfBoundingBox();
    GltfBoundingBox(const std::vector<model::PlanarSurfaceGroup>& planarSurfaceGroups);

    double lookAtR() const;
    void setLookAtR(double lookAtR);

    double lookAtX() const;
    void setLookAtX(double lookAtX);

    double lookAtY() const;
    void setLookAtY(double lookAtY);

    double lookAtZ() const;
    void setLookAtZ(double lookAtZ);

    double maxX() const;
    void setMaxX(double maxX);

    double maxY() const;
    void setMaxY(double maxY);

    double maxZ() const;
    void setMaxZ(double maxZ);

    double minX() const;
    void setMinX(double minX);

    double minY() const;
    void setMinY(double minY);

    double minZ() const;
    void setMinZ(double minZ);

   protected:
    std::map<std::string, tinygltf::Value> toExtras() const;
    GltfBoundingBox(const tinygltf::Value& boundingBoxObj);
    friend class GltfMetaData;

   private:
    REGISTER_LOGGER("openstudio.gltf.GltfBoundingBox");

    double m_lookAtR = 0.0;
    double m_lookAtX = 0.0;
    double m_lookAtY = 0.0;
    double m_lookAtZ = 0.0;
    double m_maxX = 0.0;
    double m_maxY = 0.0;
    double m_maxZ = 0.0;
    double m_minX = 0.0;
    double m_minY = 0.0;
    double m_minZ = 0.0;
  };
}  // namespace gltf
}  // namespace openstudio

#endif  //GLTF_GLTFBOUNDINGBOX_HPP
