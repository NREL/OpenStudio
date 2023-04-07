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
#ifndef GLTF_GLTFUSERDATA_HPP
#define GLTF_GLTFUSERDATA_HPP

#include "GltfAPI.hpp"

#include "../utilities/core/Logger.hpp"

#include <map>
#include <string>
#include <vector>

namespace tinygltf {
class Value;
}

namespace openstudio {
namespace model {

  class PlanarSurface;
}

namespace gltf {
  class GltfForwardTranslator;

  // TODO: rename to GltfPlanarSurfaceData!
  /** GltfUserData is an Interface class between a PlanarSurface and an glTF Node's `extras`.
   *
   *  It stores attributes such as surfaceType, its construction, the Space and SpaceType and ThermalZone it belongs to, etc.
   **/
  class GLTF_API GltfUserData
  {
   public:
    GltfUserData();  // Default constructor (TODO: remove?)

    /** @name Interface with Model and ModelObjects */
    //@{
    GltfUserData(const model::PlanarSurface& planarSurface);
    //@}

    /** @name Getters & Setters */
    //@{

    std::string handle() const;
    void setHandle(const std::string& handle);

    std::string name() const;
    void setName(const std::string& name);

    std::string surfaceType() const;
    void setSurfaceType(const std::string& surfaceType);

    std::string constructionName() const;
    void setConstructionName(const std::string& constructionName);

    std::string constructionHandle() const;
    void setConstructionHandle(const std::string& constructionHandle);

    std::string constructionMaterialName() const;
    void setConstructionMaterialName(const std::string& constructionMaterialName);

    std::string surfaceName() const;
    void setSurfaceName(const std::string& surfaceName);

    std::string surfaceHandle() const;
    void setSurfaceHandle(const std::string& surfaceHandle);

    std::string subSurfaceName() const;
    void setSubSurfaceName(const std::string& subSurfaceName);

    std::string subSurfaceHandle() const;
    void setSubSurfaceHandle(const std::string& subSurfaceHandle);

    std::string spaceName() const;
    void setSpaceName(const std::string& spaceName);

    std::string spaceHandle() const;
    void setSpaceHandle(const std::string& spaceHandle);

    std::string shadingName() const;
    void setShadingName(const std::string& shadingName);

    std::string shadingHandle() const;
    void setShadingHandle(const std::string& shadingHandle);

    std::string thermalZoneName() const;
    void setThermalZoneName(const std::string& thermalZoneName);

    std::string thermalZoneHandle() const;
    void setThermalZoneHandle(const std::string& thermalZoneHandle);

    std::string thermalZoneMaterialName() const;
    void setThermalZoneMaterialName(const std::string& thermalZoneMaterialName);

    std::string spaceTypeName() const;
    void setSpaceTypeName(const std::string& spaceTypeName);

    std::string spaceTypeHandle() const;
    void setSpaceTypeHandle(const std::string& spaceTypeHandle);

    std::string spaceTypeMaterialName() const;
    void setSpaceTypeMaterialName(const std::string& spaceTypeMaterialName);

    std::string buildingStoryName() const;
    void setBuildingStoryName(const std::string& buildingStoryName);

    std::string buildingStoryHandle() const;
    void setBuildingStoryHandle(const std::string& buildingStoryHandle);

    std::string buildingStoryMaterialName() const;
    void setBuildingStoryMaterialName(const std::string& buildingStoryMaterialName);

    std::string buildingUnitName() const;
    void setBuildingUnitName(const std::string& buildingUnitName);

    std::string buildingUnitHandle() const;
    void setBuildingUnitHandle(const std::string& buildingUnitHandle);

    std::string buildingUnitMaterialName() const;
    void setBuildingUnitMaterialName(const std::string& buildingUnitMaterialName);

    std::string constructionSetName() const;
    void setConstructionSetName(const std::string& constructionSetName);

    std::string constructionSetHandle() const;
    void setConstructionSetHandle(const std::string& constructionSetHandle);

    std::string constructionSetMaterialName() const;
    void setConstructionSetMaterialName(const std::string& constructionSetMaterialName);

    std::string outsideBoundaryCondition() const;
    void setOutsideBoundaryCondition(const std::string& outsideBoundaryCondition);

    std::string outsideBoundaryConditionObjectName() const;
    void setOutsideBoundaryConditionObjectName(const std::string& outsideBoundaryConditionObjectName);

    std::string outsideBoundaryConditionObjectHandle() const;
    void setOutsideBoundaryConditionObjectHandle(const std::string& outsideBoundaryConditionObjectHandle);

    bool coincidentWithOutsideObject() const;
    void setCoincidentWithOutsideObject(bool coincidentWithOutsideObject);

    std::string sunExposure() const;
    void setSunExposure(const std::string& sunExposure);

    std::string windExposure() const;
    void setWindExposure(const std::string& windExposure);

    double illuminanceSetpoint() const;
    void setIlluminanceSetpoint(double illuminanceSetpoint);

    bool airWall() const;
    void setAirWall(bool airWall);

    std::vector<std::string> airLoopHVACNames() const;
    void setAirLoopHVACNames(const std::vector<std::string>& airLoopHVACNames);
    void addAirLoopHVACName(std::string airLoopHVACName);

    std::vector<std::string> airLoopHVACHandles() const;
    void setAirLoopHVACHandles(const std::vector<std::string>& airLoopHVACHandles);
    void addAirLoopHVACHandle(std::string airLoopHVACHandle);

    std::vector<std::string> airLoopHVACMaterialNames() const;
    void setAirLoopHVACMaterialNames(const std::vector<std::string>& airLoopHVACMaterialNames);
    void addAirLoopHVACMaterialName(std::string airLoopHVACMaterialName);

    //@}
    /** @name Other */
    //@{

    // // Gets GLTF Material name on the basis of Surface Type
    std::string surfaceTypeMaterialName() const;
    // Gets GLTF Material name on the basis of Surface Outside Boundary Condition, Wind Exposure, Sun Exposure
    std::string boundaryMaterialName() const;

    //@}

   protected:
    /** @name Protected */
    //@{
    /** Interface with an existing GLTF file */
    GltfUserData(const tinygltf::Value& userDataObj);

    // TODO: change to return a tinygltf::Node instead
    std::map<std::string, tinygltf::Value> toExtras() const;
    //@}
    friend class GltfForwardTranslator;

   private:
    REGISTER_LOGGER("openstudio.gltf.GltfUserData");

    std::string m_handle;
    std::string m_name;
    std::string m_surfaceType;
    std::string m_constructionName;
    std::string m_constructionHandle;
    std::string m_constructionMaterialName;
    std::string m_surfaceName;
    std::string m_surfaceHandle;
    std::string m_subSurfaceName;
    std::string m_subSurfaceHandle;
    std::string m_spaceName;
    std::string m_spaceHandle;
    std::string m_shadingName;
    std::string m_shadingHandle;
    std::string m_thermalZoneName;
    std::string m_thermalZoneHandle;
    std::string m_thermalZoneMaterialName;
    std::string m_spaceTypeName;
    std::string m_spaceTypeHandle;
    std::string m_spaceTypeMaterialName;
    std::string m_buildingStoryName;
    std::string m_buildingStoryHandle;
    std::string m_buildingStoryMaterialName;
    std::string m_buildingUnitName;
    std::string m_buildingUnitHandle;
    std::string m_buildingUnitMaterialName;
    std::string m_constructionSetName;
    std::string m_constructionSetHandle;
    std::string m_constructionSetMaterialName;
    std::string m_outsideBoundaryCondition;
    std::string m_outsideBoundaryConditionObjectName;
    std::string m_outsideBoundaryConditionObjectHandle;
    bool m_coincidentWithOutsideObject = false;
    std::string m_sunExposure;
    std::string m_windExposure;
    double m_illuminanceSetpoint = 0.0;
    bool m_airWall = false;

    // TODO: replace with a struct to avoid having to keep indices in line?
    std::vector<std::string> m_airLoopHVACNames;
    std::vector<std::string> m_airLoopHVACHandles;
    std::vector<std::string> m_airLoopHVACMaterialNames;
  };

  using GltfUserDataVector = std::vector<GltfUserData>;

}  // namespace gltf
}  // namespace openstudio

#endif  // GLTF_GLTFUSERDATA_HPP
