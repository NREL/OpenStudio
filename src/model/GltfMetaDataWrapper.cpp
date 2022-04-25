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

#include "GltfMetaDataWrapper.hpp"

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

  std::vector<GltfModelObjectMetadataWrapper> glTFModelObjectMetadataWrapperVector;
  GltfBoundingBoxWrapper glTFBoundingBox;
  std::vector<std::string> buildingStoryNames;
  // string
  std::tuple<std::string, std::string> generator;
  std::tuple<std::string, std::string> type;
  std::tuple<std::string, std::string> version;
  // double
  std::tuple<std::string, double> northAxis;
  // int
  int storyCount;
  int thermalZoneCount;
  int spaceCount;
  int spaceTypeCount;
  int constructionSetCount;
  int airLoopCount;
  /* int m_storyCount;
    int m_thermalZoneCount;
    int m_spaceCount;
    int m_spaceTypeCount;
    int m_constructionSetCount;
    int m_airLoopCount;*/


  void resetModelObjectMetaDataCount() {
    /*m_storyCount = 0;
    m_thermalZoneCount = 0;
    m_spaceCount = 0;
    m_spaceTypeCount = 0;
    m_constructionSetCount = 0;
    m_airLoopCount = 0;*/
    storyCount = 0;
    thermalZoneCount = 0;
    spaceCount = 0;
    spaceTypeCount = 0;
    constructionSetCount = 0;
    airLoopCount = 0;
  }

  GltfMetaDataWrapper::GltfMetaDataWrapper() {
    resetModelObjectMetaDataCount();
    m_logSink.setLogLevel(Warn);
    m_logSink.setThreadId(std::this_thread::get_id());
  }

  double getBoundingBoxlookAtR() {
    return glTFBoundingBox.getlookAtR();
  }

  double getBoundingBoxlookAtX() {
    return glTFBoundingBox.getlookAtX();
  }

  double getBoundingBoxlookAtY() {
    return glTFBoundingBox.getlookAtY();
  }

  double getBoundingBoxlookAtZ() {
    return glTFBoundingBox.getlookAtZ();
  }

  double getBoundingBoxmaxX() {
    return glTFBoundingBox.getmaxX();
  }

  double getBoundingBoxmaxY() {
    return glTFBoundingBox.getmaxY();
  }

  double getBoundingBoxmaxZ() {
    return glTFBoundingBox.getmaxZ();
  }

  double getBoundingBoxminZ() {
    return glTFBoundingBox.getminZ();
  }

  double getBoundingBoxminX() {
    return glTFBoundingBox.getminX();
  }

  double getBoundingBoxminY() {
    return glTFBoundingBox.getminY();
  }

  std::vector<GltfModelObjectMetadataWrapper> getModelObjectMetaDataCollection() {
    return glTFModelObjectMetadataWrapperVector;
  }


  void GltfMetaDataWrapper::setBuildingStoryNames(const std::vector<std::string>& storyNames) {
    buildingStoryNames = storyNames;
    std::sort(buildingStoryNames.begin(), buildingStoryNames.end(), IstringCompare());
  }

  void GltfMetaDataWrapper::setGenerator(const std::string& value) {
    std::string key = "generator";
    generator = make_tuple(key, value);
  }

  void GltfMetaDataWrapper::setType(const std::string& value) {
    std::string key = "type";
    type = make_tuple(key, value);
  }

  void GltfMetaDataWrapper::setVersion(const std::string& value) {
    std::string key = "version";
    version = make_tuple(key, value);
  }

  void GltfMetaDataWrapper::setNorthAxis(const double& value) {
    std::string key = "northAxis";
    northAxis = make_tuple(key, value);
  }

  std::vector<std::string> GltfMetaDataWrapper::getBuildingStoryNames() const {
    return buildingStoryNames;
  }

  std::string GltfMetaDataWrapper::getGenerator() const {
    return std::get<1>(generator);
  }

  std::string GltfMetaDataWrapper::getType() const {
    return std::get<1>(type);
  }

  std::string GltfMetaDataWrapper::getVersion() const {
    return std::get<1>(version);
  }

  double GltfMetaDataWrapper::getNorthAxis() const {
    return std::get<1>(northAxis);
  }

  void GltfMetaDataWrapper::storyCountIncrement() {
    storyCount++;
  }

  void GltfMetaDataWrapper::thermalZoneCountIncrement() {
    thermalZoneCount++;
  }

  void GltfMetaDataWrapper::spaceCountIncrement() {
    spaceCount++;
  }

  void GltfMetaDataWrapper::spaceTypeCountIncrement() {
    spaceTypeCount++;
  }

  void GltfMetaDataWrapper::constructionSetCountIncrement() {
    constructionSetCount++;
  }

  void GltfMetaDataWrapper::airLoopCountIncrement() {
    airLoopCount++;
  }

  int GltfMetaDataWrapper::getStoryCount() const {
    return storyCount;
  }

  int GltfMetaDataWrapper::getThermalZoneCount() const {
    return thermalZoneCount;
  }

  int GltfMetaDataWrapper::getSpaceCount() const {
    return spaceCount;
  }

  int GltfMetaDataWrapper::getSpaceTypeCount() const {
    return spaceTypeCount;
  }

  int GltfMetaDataWrapper::getConstructionSetCount() const {
    return constructionSetCount;
  }

  int GltfMetaDataWrapper::getAirLoopCount() const {
    return airLoopCount;
  }

  void GltfMetaDataWrapper::setglTFBoundingBoxWrapper(const GltfBoundingBoxWrapper& gbx) {
    glTFBoundingBox = gbx;
  }

  GltfBoundingBoxWrapper GltfMetaDataWrapper::getglTFBoundingBoxWrapper() {
    return glTFBoundingBox;
  }

  void GltfMetaDataWrapper::addModelObjectMetadata(GltfModelObjectMetadataWrapper& gbmw) {
    glTFModelObjectMetadataWrapperVector.push_back(gbmw);
  }

  std::vector<GltfModelObjectMetadataWrapper> GltfMetaDataWrapper::getglTFModelObjectMetadataWrapper() {
    return glTFModelObjectMetadataWrapperVector;
  }

  std::vector<LogMessage> GltfMetaDataWrapper::warnings() const {
    std::vector<LogMessage> result;
    for (LogMessage logMessage : m_logSink.logMessages()) {
      if (logMessage.logLevel() == Warn) {
        result.push_back(logMessage);
      }
    }
    return result;
  }

  std::vector<LogMessage> GltfMetaDataWrapper::errors() const {
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
