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

#include "GltfBoundingBoxWrapper.hpp"

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

  void GltfBoundingBoxWrapper::setlookAtR(double value) {
    std::string key = "lookAtR";
    lookAtR = make_tuple(key, value);
  }

  void GltfBoundingBoxWrapper::setlookAtX(double value) {
    std::string key = "lookAtX";
    lookAtX = make_tuple(key, value);
  }

  void GltfBoundingBoxWrapper::setlookAtY(double value) {
    std::string key = "lookAtY";
    lookAtY = make_tuple(key, value);
  }

  void GltfBoundingBoxWrapper::setlookAtZ(double value) {
    std::string key = "lookAtZ";
    lookAtZ = make_tuple(key, value);
  }

  void GltfBoundingBoxWrapper::setmaxX(double value) {
    std::string key = "maxX";
    maxX = make_tuple(key, value);
  }

  void GltfBoundingBoxWrapper::setmaxY(double value) {
    std::string key = "maxY";
    maxY = make_tuple(key, value);
  }

  void GltfBoundingBoxWrapper::setmaxZ(double value) {
    std::string key = "maxZ";
    maxZ = make_tuple(key, value);
  }

  void GltfBoundingBoxWrapper::setminX(double value) {
    std::string key = "minX";
    minX = make_tuple(key, value);
  }

  void GltfBoundingBoxWrapper::setminY(double value) {
    std::string key = "minY";
    minY = make_tuple(key, value);
  }

  void GltfBoundingBoxWrapper::setminZ(double value) {
    std::string key = "minZ";
    minZ = make_tuple(key, value);
  }

  // Getters
  double GltfBoundingBoxWrapper::getlookAtR() const {
    return std::get<1>(lookAtR);
  }

  double GltfBoundingBoxWrapper::getlookAtX() const {
    return std::get<1>(lookAtX);
  }

  double GltfBoundingBoxWrapper::getlookAtY() const {
    return std::get<1>(lookAtY);
  }

  double GltfBoundingBoxWrapper::getlookAtZ() const {
    return std::get<1>(lookAtZ);
  }

  double GltfBoundingBoxWrapper::getmaxX() const {
    return std::get<1>(maxX);
  }

  double GltfBoundingBoxWrapper::getmaxY() const {
    return std::get<1>(maxY);
  }

  double GltfBoundingBoxWrapper::getmaxZ() const {
    return std::get<1>(maxZ);
  }

  double GltfBoundingBoxWrapper::getminX() const {
    return std::get<1>(minX);
  }

  double GltfBoundingBoxWrapper::getminY() const {
    return std::get<1>(minY);
  }

  double GltfBoundingBoxWrapper::getminZ() const {
    return std::get<1>(minZ);
  }

  GltfBoundingBoxWrapper::GltfBoundingBoxWrapper() {
    m_logSink.setLogLevel(Warn);
    m_logSink.setThreadId(std::this_thread::get_id());
  }

  std::vector<LogMessage> GltfBoundingBoxWrapper::warnings() const {
    std::vector<LogMessage> result;
    for (LogMessage logMessage : m_logSink.logMessages()) {
      if (logMessage.logLevel() == Warn) {
        result.push_back(logMessage);
      }
    }
    return result;
  }

  std::vector<LogMessage> GltfBoundingBoxWrapper::errors() const {
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
