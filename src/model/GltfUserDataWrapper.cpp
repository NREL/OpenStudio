/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "GltfUserDataWrapper.hpp"

#include "RenderingColor.hpp"
#include "ConstructionBase.hpp"
#include "ConstructionBase_Impl.hpp"
#include "ConstructionAirBoundary.hpp"
#include "ConstructionAirBoundary_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "SpaceType.hpp"
#include "SpaceType_Impl.hpp"
#include "Building.hpp"
#include "Building_Impl.hpp"
#include "BuildingStory.hpp"
#include "BuildingStory_Impl.hpp"
#include "BuildingUnit.hpp"
#include "BuildingUnit_Impl.hpp"
#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "SubSurface.hpp"
#include "SubSurface_Impl.hpp"
#include "ShadingSurface.hpp"
#include "ShadingSurface_Impl.hpp"
#include "InteriorPartitionSurface.hpp"
#include "InteriorPartitionSurface_Impl.hpp"
#include "PlanarSurfaceGroup.hpp"
#include "PlanarSurfaceGroup_Impl.hpp"
#include "Space.hpp"
#include "Space_Impl.hpp"
#include "DefaultConstructionSet.hpp"
#include "DefaultConstructionSet_Impl.hpp"
#include "ShadingSurfaceGroup.hpp"
#include "InteriorPartitionSurfaceGroup.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Compare.hpp"
#include "../utilities/geometry/Point3d.hpp"
#include "../utilities/geometry/Plane.hpp"
#include "../utilities/geometry/BoundingBox.hpp"
#include "../utilities/geometry/Transformation.hpp"
#include "../utilities/geometry/Geometry.hpp"
#include "../utilities/geometry/Polygon3d.hpp"

#include <tiny_gltf.h>

#include <algorithm>
#include <cmath>
#include <iterator>
#include <numeric>
#include <stack>
#include <limits.h>
#include <tuple>

namespace openstudio {
namespace model {
  // m_handle;
  std::string handlekey = "handle";
  void GltfUserDataWrapper::setHandle(const std::string& value) {
    handle = make_tuple(handlekey, value);
  }

  // m_name;
  std::string namekey = "name";
  void GltfUserDataWrapper::setName(const std::string& value) {
    name = make_tuple(namekey, value);
  }

  // m_surfaceType;
  std::string surfaceTypekey = "surfaceType";
  void GltfUserDataWrapper::setSurfaceType(const std::string& value) {
    surfaceType = make_tuple(surfaceTypekey, value);
  }

  // m_surfaceTypeMaterialName;
  std::string surfaceTypeMaterailNamekey = "surfaceTypeMaterailName";
  void GltfUserDataWrapper::setSurfaceTypeMaterailName(const std::string& value) {
    surfaceTypeMaterailName = make_tuple(surfaceTypeMaterailNamekey, value);
  }

  // m_constructionName;
  std::string constructionNamekey = "constructionName";
  void GltfUserDataWrapper::setConstructionName(const std::string& value) {
    constructionName = make_tuple(constructionNamekey, value);
  }

  // m_constructionHandle;
  std::string constructionHandlekey = "constructionHandle";
  void GltfUserDataWrapper::setConstructionHandle(const std::string& value) {
    constructionHandle = make_tuple(constructionHandlekey, value);
  }

  // m_constructionMaterialName;
  std::string constructionMaterialNamekey = "constructionMaterialName";
  void GltfUserDataWrapper::setConstructionMaterialName(const std::string& value) {
    constructionMaterialName = make_tuple(constructionMaterialNamekey, value);
  }

  // m_surfaceName;
  std::string surfaceNamekey = "surfaceName";
  void GltfUserDataWrapper::setSurfaceName(const std::string& value) {
    surfaceName = make_tuple(surfaceNamekey, value);
  }

  // m_surfaceHandle;
  std::string surfaceHandlekey = "surfaceHandle";
  void GltfUserDataWrapper::setSurfaceHandle(const std::string& value) {
    surfaceHandle = make_tuple(surfaceHandlekey, value);
  }

  // m_subSurfaceName;
  std::string subSurfaceNamekey = "subSurfaceName";
  void GltfUserDataWrapper::setSubSurfaceName(const std::string& value) {
    subSurfaceName = make_tuple(subSurfaceNamekey, value);
  }

  // m_subSurfaceHandle;
  std::string subSurfaceHandlekey = "subSurfaceHandle";
  void GltfUserDataWrapper::setSubSurfaceHandle(const std::string& value) {
    subSurfaceHandle = make_tuple(subSurfaceHandlekey, value);
  }

  // m_spaceName;
  std::string spaceNamekey = "spaceName";
  void GltfUserDataWrapper::setSpaceName(const std::string& value) {
    spaceName = make_tuple(spaceNamekey, value);
  }

  // m_spaceHandle;
  std::string spaceHandlekey = "spaceHandle";
  void GltfUserDataWrapper::setSpaceHandle(const std::string& value) {
    spaceHandle = make_tuple(spaceHandlekey, value);
  }

  // m_shadingName
  std::string shadingNamekey = "shadingName";
  void GltfUserDataWrapper::setShadingName(const std::string& value) {
    shadingName = make_tuple(shadingNamekey, value);
  }

  // m_shadingHandle;
  std::string shadingHandlekey = "shadingHandle";
  void GltfUserDataWrapper::setShadingHandle(const std::string& value) {
    shadingHandle = make_tuple(shadingHandlekey, value);
  }

  // m_thermalZoneName;
  std::string thermalZoneNamekey = "thermalZoneName";
  void GltfUserDataWrapper::setThermalZoneName(const std::string& value) {
    thermalZoneName = make_tuple(thermalZoneNamekey, value);
  }

  // m_thermalZoneHandle;
  std::string thermalZoneHandlekey = "thermalZoneHandle";
  void GltfUserDataWrapper::setThermalZoneHandle(const std::string& value) {
    thermalZoneHandle = make_tuple(thermalZoneHandlekey, value);
  }

  // m_thermalZoneMaterialName;
  std::string thermalZoneMaterialNamekey = "thermalZoneMaterialName";
  void GltfUserDataWrapper::setThermalZoneMaterialName(const std::string& value) {
    thermalZoneMaterialName = make_tuple(thermalZoneMaterialNamekey, value);
  }

  // m_spaceTypeName;
  std::string spaceTypeNamekey = "spaceTypeName";
  void GltfUserDataWrapper::setSpaceTypeName(const std::string& value) {
    spaceTypeName = make_tuple(spaceTypeNamekey, value);
  }

  // m_spaceTypeHandle;
  std::string spaceTypeHandlekey = "spaceTypeHandle";
  void GltfUserDataWrapper::setSpaceTypeHandle(const std::string& value) {
    spaceTypeHandle = make_tuple(spaceTypeHandlekey, value);
  }

  // m_spaceTypeMaterialName;
  std::string spaceTypeMaterialNamekey = "spaceTypeMaterialName";
  void GltfUserDataWrapper::setSpaceTypeMaterialName(const std::string& value) {
    spaceTypeMaterialName = make_tuple(spaceTypeMaterialNamekey, value);
  }

  // m_buildingStoryName;
  std::string buildingStoryNamekey = "buildingStoryName";
  void GltfUserDataWrapper::setBuildingStoryName(const std::string& value) {
    buildingStoryName = make_tuple(buildingStoryNamekey, value);
  }

  // m_buildingStoryHandle;
  std::string buildingStoryHandlekey = "buildingStoryHandle";
  void GltfUserDataWrapper::setBuildingStoryHandle(const std::string& value) {
    buildingStoryHandle = make_tuple(buildingStoryHandlekey, value);
  }

  // m_buildingStoryMaterialName;
  std::string buildingStoryMaterialNamekey = "buildingStoryMaterialName";
  void GltfUserDataWrapper::setBuildingStoryMaterialName(const std::string& value) {
    buildingStoryMaterialName = make_tuple(buildingStoryMaterialNamekey, value);
  }

  // m_buildingUnitName;
  std::string buildingUnitNamekey = "buildingUnitName";
  void GltfUserDataWrapper::setBuildingUnitName(const std::string& value) {
    buildingUnitName = make_tuple(buildingUnitNamekey, value);
  }

  // m_buildingUnitHandle;
  std::string buildingUnitHandlekey = "buildingUnitHandle";
  void GltfUserDataWrapper::setBuildingUnitHandle(const std::string& value) {
    buildingUnitHandle = make_tuple(buildingUnitHandlekey, value);
  }

  // m_buildingUnitMaterialName;
  std::string buildingUnitMaterialNamekey = "buildingUnitMaterialName";
  void GltfUserDataWrapper::setBuildingUnitMaterialName(const std::string& value) {
    buildingUnitMaterialName = make_tuple(buildingUnitMaterialNamekey, value);
  }

  // m_constructionSetName;
  std::string constructionSetNamekey = "constructionSetName";
  void GltfUserDataWrapper::setConstructionSetName(const std::string& value) {
    constructionSetName = make_tuple(constructionSetNamekey, value);
  }

  // m_constructionSetHandle;
  std::string constructionSetHandlekey = "constructionSetHandle";
  void GltfUserDataWrapper::setConstructionSetHandle(const std::string& value) {
    constructionSetHandle = make_tuple(constructionSetHandlekey, value);
  }

  // m_constructionSetMaterialName;
  std::string constructionSetMaterialNamekey = "constructionSetMaterialName";
  void GltfUserDataWrapper::setConstructionSetMaterialName(const std::string& value) {
    constructionSetMaterialName = make_tuple(constructionSetMaterialNamekey, value);
  }

  // m_outsideBoundaryCondition;
  std::string outsideBoundaryConditionkey = "outsideBoundaryCondition";
  void GltfUserDataWrapper::setOutsideBoundaryCondition(const std::string& value) {
    outsideBoundaryCondition = make_tuple(outsideBoundaryConditionkey, value);
  }

  // m_outsideBoundaryConditionObjectName;
  std::string outsideBoundaryConditionObjectNamekey = "outsideBoundaryConditionObjectName";
  void GltfUserDataWrapper::setOutsideBoundaryConditionObjectName(const std::string& value) {
    outsideBoundaryConditionObjectName = make_tuple(outsideBoundaryConditionObjectNamekey, value);
  }

  // m_outsideBoundaryConditionObjectHandle;
  std::string outsideBoundaryConditionObjectHandlekey = "outsideBoundaryConditionObjectHandle";
  void GltfUserDataWrapper::setOutsideBoundaryConditionObjectHandle(const std::string& value) {
    outsideBoundaryConditionObjectHandle = make_tuple(outsideBoundaryConditionObjectHandlekey, value);
  }

  // m_boundaryMaterialName;
  std::string boundaryMaterialNamekey = "boundaryMaterialName";
  void GltfUserDataWrapper::setBoundaryMaterialName(const std::string& value) {
    boundaryMaterialName = make_tuple(boundaryMaterialNamekey, value);
  }

  // bool m_coincidentWithOutsideObject;
  std::string coincidentWithOutsideObjectkey = "coincidentWithOutsideObject";
  void GltfUserDataWrapper::setCoincidentWithOutsideObject(const bool& value) {
    coincidentWithOutsideObject = make_tuple(coincidentWithOutsideObjectkey, value);
  }

  // m_sunExposure;
  std::string sunExposurekey = "sunExposure";
  void GltfUserDataWrapper::setSunExposure(const std::string& value) {
    sunExposure = make_tuple(sunExposurekey, value);
  }
  // m_windExposure;
  std::string windExposurekey = "windExposure";
  void GltfUserDataWrapper::setWindExposure(const std::string& value) {
    windExposure = make_tuple(windExposurekey, value);
  }

  // double m_illuminanceSetpoint;
  std::string illuminanceSetpointkey = "illuminanceSetpoint";
  void GltfUserDataWrapper::setIlluminanceSetpoint(const double& value) {
    illuminanceSetpoint = make_tuple(illuminanceSetpointkey, value);
  }

  // bool m_airWall;
  std::string airWallkey = "airWall";
  void GltfUserDataWrapper::setAirWall(const bool& value) {
    airWall = make_tuple(airWallkey, value);
  }

  // vector airLoopHVACNames
  void GltfUserDataWrapper::setAirLoopHVACNames(const std::vector<std::string>& value) {
    airLoopHVACNames = value;
  }

  // vector airLoopHVACHandles
  void GltfUserDataWrapper::setAirLoopHVACHandles(const std::vector<std::string>& value) {
    airLoopHVACHandles = value;
  }

  // vector airLoopHVACMaterialNames
  void GltfUserDataWrapper::setAirLoopHVACMaterialNames(const std::vector<std::string>& value) {
    airLoopHVACMaterialNames = value;
  }




  std::string GltfUserDataWrapper::getHandle() const {
    return std::get<1>(handle);
  }
  std::string GltfUserDataWrapper::getName() const {
    return std::get<1>(name);
  }
  std::string GltfUserDataWrapper::getSurfaceType() const {
    return std::get<1>(surfaceType);
  }
  std::string GltfUserDataWrapper::getSurfaceTypeMaterailName() const {
    return std::get<1>(surfaceTypeMaterailName);
  }
  std::string GltfUserDataWrapper::getConstructionName() const {
    return std::get<1>(constructionName);
  }
  std::string GltfUserDataWrapper::getConstructionHandle() const {
    return std::get<1>(constructionHandle);
  }
  std::string GltfUserDataWrapper::getConstructionMaterialName() const {
    return std::get<1>(constructionMaterialName);
  }
  std::string GltfUserDataWrapper::getSurfaceName() const {
    return std::get<1>(surfaceName);
  }
  std::string GltfUserDataWrapper::getSurfaceHandle() const {
    return std::get<1>(surfaceHandle);
  }
  std::string GltfUserDataWrapper::getSubSurfaceName() const {
    return std::get<1>(subSurfaceName);
  }
  std::string GltfUserDataWrapper::getSubSurfaceHandle() const {
    return std::get<1>(subSurfaceHandle);
  }
  std::string GltfUserDataWrapper::getSpaceName() const {
    return std::get<1>(spaceName);
  }
  std::string GltfUserDataWrapper::getSpaceHandle() const {
    return std::get<1>(spaceHandle);
  }
  std::string GltfUserDataWrapper::getShadingName() const {
    return std::get<1>(shadingName);
  }
  std::string GltfUserDataWrapper::getShadingHandle() const {
    return std::get<1>(shadingHandle);
  }
  std::string GltfUserDataWrapper::getThermalZoneName() const {
    return std::get<1>(thermalZoneName);
  }
  std::string GltfUserDataWrapper::getThermalZoneHandle() const {
    return std::get<1>(thermalZoneHandle);
  }
  std::string GltfUserDataWrapper::getThermalZoneMaterialName() const {
    return std::get<1>(thermalZoneMaterialName);
  }
  std::string GltfUserDataWrapper::getSpaceTypeName() const {
    return std::get<1>(spaceTypeName);
  }
  std::string GltfUserDataWrapper::getSpaceTypeHandle() const {
    return std::get<1>(spaceTypeHandle);
  }
  std::string GltfUserDataWrapper::getSpaceTypeMaterialName() const {
    return std::get<1>(spaceTypeMaterialName);
  }
  std::string GltfUserDataWrapper::getBuildingStoryName() const {
    return std::get<1>(buildingStoryName);
  }
  std::string GltfUserDataWrapper::getBuildingStoryHandle() const {
    return std::get<1>(buildingStoryHandle);
  }
  std::string GltfUserDataWrapper::getBuildingStoryMaterialName() const {
    return std::get<1>(buildingStoryMaterialName);
  }
  std::string GltfUserDataWrapper::getBuildingUnitName() const {
    return std::get<1>(buildingUnitName);
  }
  std::string GltfUserDataWrapper::getBuildingUnitHandle() const {
    return std::get<1>(buildingUnitHandle);
  }
  std::string GltfUserDataWrapper::getBuildingUnitMaterialName() const {
    return std::get<1>(buildingUnitMaterialName);
  }
  std::string GltfUserDataWrapper::getConstructionSetName() const {
    return std::get<1>(constructionSetName);
  }
  std::string GltfUserDataWrapper::getConstructionSetHandle() const {
    return std::get<1>(constructionSetHandle);
  }
  std::string GltfUserDataWrapper::getConstructionSetMaterialName() const {
    return std::get<1>(constructionSetMaterialName);
  }
  std::string GltfUserDataWrapper::getOutsideBoundaryCondition() const {
    return std::get<1>(outsideBoundaryCondition);
  }
  std::string GltfUserDataWrapper::getOutsideBoundaryConditionObjectName() const {
    return std::get<1>(outsideBoundaryConditionObjectName);
  }
  std::string GltfUserDataWrapper::getOutsideBoundaryConditionObjectHandle() const {
    return std::get<1>(outsideBoundaryConditionObjectHandle);
  }
  std::string GltfUserDataWrapper::getBoundaryMaterialName() const {
    return std::get<1>(boundaryMaterialName);
  }
  bool GltfUserDataWrapper::getCoincidentWithOutsideObject() const {
    return std::get<1>(coincidentWithOutsideObject);
  }
  std::string GltfUserDataWrapper::getSunExposure() const {
    return std::get<1>(sunExposure);
  }
  std::string GltfUserDataWrapper::getWindExposure() const {
    return std::get<1>(windExposure);
  }
  double GltfUserDataWrapper::getIlluminanceSetpoint() const {
    return std::get<1>(illuminanceSetpoint);
  }
  bool GltfUserDataWrapper::getAirWall() const {
    return std::get<1>(airWall);
  }
  std::vector<std::string> GltfUserDataWrapper::getAirLoopHVACHandles() const {
    return airLoopHVACHandles;
  }
  std::vector<std::string> GltfUserDataWrapper::getAirLoopHVACNames() const {
    return airLoopHVACNames;
  }
  std::vector<std::string> GltfUserDataWrapper::getAirLoopHVACMaterialNames() const {
    return airLoopHVACMaterialNames;
  }

  GltfUserDataWrapper::GltfUserDataWrapper() {
    m_logSink.setLogLevel(Warn);
    m_logSink.setThreadId(std::this_thread::get_id());
  }

  std::vector<LogMessage> GltfUserDataWrapper::warnings() const {
    std::vector<LogMessage> result;
    for (LogMessage logMessage : m_logSink.logMessages()) {
      if (logMessage.logLevel() == Warn) {
        result.push_back(logMessage);
      }
    }
    return result;
  }

  std::vector<LogMessage> GltfUserDataWrapper::errors() const {
    std::vector<LogMessage> result;
    for (LogMessage logMessage : m_logSink.logMessages()) {
      if (logMessage.logLevel() > Warn) {
        result.push_back(logMessage);
      }
    }
    return result;
  }

}  // namespace model
}  // namespace openstudio
