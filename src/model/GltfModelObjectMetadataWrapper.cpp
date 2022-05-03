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

#include "GltfModelObjectMetadataWrapper.hpp"

#include <tiny_gltf.h>

#include <algorithm>
#include <cmath>
#include <iterator>
#include <numeric>
#include <tuple>

namespace openstudio {
namespace model {

  void GltfModelObjectMetadataWrapper::setColor(const std::string& value) {
    std::string key = "color";
    color = make_tuple(key, value);
  }

  void GltfModelObjectMetadataWrapper::setHandle(const std::string& value) {
    std::string key = "handle";
    handle = make_tuple(key, value);
  }

  void GltfModelObjectMetadataWrapper::setIddObjectType(const std::string& value) {
    std::string key = "iddObjectType";
    iddObjectType = make_tuple(key, value);
  }

  void GltfModelObjectMetadataWrapper::setName(const std::string& value) {
    std::string key = "name";
    name = make_tuple(key, value);
  }

  void GltfModelObjectMetadataWrapper::setOpen_to_below(const bool& value) {
    std::string key = "open_to_below";
    open_to_below = make_tuple(key, value);
  }

  void GltfModelObjectMetadataWrapper::setNominal_z_coordinate(const double& value) {
    std::string key = "nominal_z_coordinate";
    nominal_z_coordinate = make_tuple(key, value);
  }

  void GltfModelObjectMetadataWrapper::setNominal_floorCeiling_Height(const double& value) {
    std::string key = "nominal_floorCeiling_Height";
    nominal_floorCeiling_Height = make_tuple(key, value);
  }

  void GltfModelObjectMetadataWrapper::setMultiplier(const int& value) {
    std::string key = "multiplier";
    multiplier = make_tuple(key, value);
  }

  // getters
  std::string GltfModelObjectMetadataWrapper::getColor() const {
    return std::get<1>(color);
  }

  std::string GltfModelObjectMetadataWrapper::getHandle() const {
    return std::get<1>(handle);
  }

  std::string GltfModelObjectMetadataWrapper::getIddObjectType() const {
    return std::get<1>(iddObjectType);
  }

  std::string GltfModelObjectMetadataWrapper::getName() const {
    return std::get<1>(name);
  }

  bool GltfModelObjectMetadataWrapper::getOpen_to_below() const {
    return std::get<1>(open_to_below);
  }

  double GltfModelObjectMetadataWrapper::getNominal_z_coordinate() const {
    return std::get<1>(nominal_z_coordinate);
  }

  double GltfModelObjectMetadataWrapper::getNominal_floorCeiling_Height() const {
    return std::get<1>(nominal_floorCeiling_Height);
  }

  int GltfModelObjectMetadataWrapper::getMultiplier() const {
    return std::get<1>(multiplier);
  }

  GltfModelObjectMetadataWrapper::GltfModelObjectMetadataWrapper() {
    m_logSink.setLogLevel(Warn);
    m_logSink.setThreadId(std::this_thread::get_id());
  }

  std::vector<LogMessage> GltfModelObjectMetadataWrapper::warnings() const {
    std::vector<LogMessage> result;
    for (LogMessage logMessage : m_logSink.logMessages()) {
      if (logMessage.logLevel() == Warn) {
        result.push_back(logMessage);
      }
    }
    return result;
  }

  std::vector<LogMessage> GltfModelObjectMetadataWrapper::errors() const {
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
