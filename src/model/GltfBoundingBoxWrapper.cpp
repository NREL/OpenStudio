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

#include <tuple>

namespace openstudio {
namespace model {

  void GltfBoundingBoxWrapper::setlookAtR(double value) {
    std::string key = "lookAtR";
    lookAtR = std::make_tuple(key, value);
  }

  void GltfBoundingBoxWrapper::setlookAtX(double value) {
    std::string key = "lookAtX";
    lookAtX = std::make_tuple(key, value);
  }

  void GltfBoundingBoxWrapper::setlookAtY(double value) {
    std::string key = "lookAtY";
    lookAtY = std::make_tuple(key, value);
  }

  void GltfBoundingBoxWrapper::setlookAtZ(double value) {
    std::string key = "lookAtZ";
    lookAtZ = std::make_tuple(key, value);
  }

  void GltfBoundingBoxWrapper::setmaxX(double value) {
    std::string key = "maxX";
    maxX = std::make_tuple(key, value);
  }

  void GltfBoundingBoxWrapper::setmaxY(double value) {
    std::string key = "maxY";
    maxY = std::make_tuple(key, value);
  }

  void GltfBoundingBoxWrapper::setmaxZ(double value) {
    std::string key = "maxZ";
    maxZ = std::make_tuple(key, value);
  }

  void GltfBoundingBoxWrapper::setminX(double value) {
    std::string key = "minX";
    minX = std::make_tuple(key, value);
  }

  void GltfBoundingBoxWrapper::setminY(double value) {
    std::string key = "minY";
    minY = std::make_tuple(key, value);
  }

  void GltfBoundingBoxWrapper::setminZ(double value) {
    std::string key = "minZ";
    minZ = std::make_tuple(key, value);
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

}  // namespace model
}  // namespace openstudio
