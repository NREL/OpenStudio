/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "StandardsJSON.hpp"
#include "StandardsJSON_Impl.hpp"

#include "../core/Json.hpp"
#include <json/json.h>

namespace openstudio {
namespace detail {

  StandardsJSON_Impl::StandardsJSON_Impl() {}

  StandardsJSON_Impl::StandardsJSON_Impl(const std::string& s) {
    std::istringstream ss(s);

    Json::CharReaderBuilder rbuilder;
    rbuilder["collectComments"] = false;
    std::string formattedErrors;

    bool parsingSuccessful = Json::parseFromStream(rbuilder, ss, &m_standardsRoot, &formattedErrors);
    if (!parsingSuccessful) {
      LOG_AND_THROW("StandardsJSON cannot be processed, " << formattedErrors);
    }

    if (!isValidStandardsJSON()) {
      LOG_AND_THROW("StandardsJSON isn't valid");
    }
  }

  bool StandardsJSON_Impl::isValidStandardsJSON() const {

    if (!m_standardsRoot.isObject()) {
      LOG(Warn, "Root isn't an object");
      return false;
    }

    if (m_standardsRoot.getMemberNames().size() < 1) {
      LOG(Warn, "Root doesn't have at least one entry");
      return false;
    }

    std::string primaryKey = m_standardsRoot.getMemberNames()[0];
    if (!(openstudio::checkType(m_standardsRoot, primaryKey, Json::arrayValue))) {
      LOG(Warn, "First entry should be an array");
      return false;
    }

    const Json::Value m_standardsArr = m_standardsRoot.get(primaryKey, Json::arrayValue);
    if (m_standardsArr.size() < 1) {
      LOG(Warn, "First entry should be an array with at least one element");
      return false;
    }
    const Json::Value firstArrEntry = m_standardsArr[0];
    if (!firstArrEntry.isObject()) {
      LOG(Warn, "Second level entry should be an object");
      return false;
    }
    // There shouldn't be any nested levels below that
    for (auto& v : firstArrEntry) {
      if (v.isArray() || v.isObject()) {
        LOG(Warn, "There shouldn't be any nested objects");
        return false;
      }
    }

    return true;
  }

  std::string StandardsJSON_Impl::string() const {
    Json::StreamWriterBuilder wbuilder;
    const std::string output = Json::writeString(wbuilder, m_standardsRoot);
    return output;
  }

  StandardsJSON StandardsJSON_Impl::clone() const {
    StandardsJSON result(this->string());
    return result;
  }

  boost::optional<Json::Value> StandardsJSON_Impl::getPrimaryKey(const std::string& primaryKey) const {

    boost::optional<Json::Value> result;

    // Check that primary key is indeed in there, and that the resulting object is an array
    if (openstudio::checkKeyAndType(m_standardsRoot, primaryKey, Json::arrayValue)) {
      result = m_standardsRoot.get(primaryKey, Json::arrayValue);
    } else {
      LOG(Warn, "Cannot find the primary key '" << primaryKey << "' in the StandardsJSON");
    }

    return result;
  }

}  // namespace detail

StandardsJSON::StandardsJSON() : m_impl(std::shared_ptr<detail::StandardsJSON_Impl>(new detail::StandardsJSON_Impl())) {}

StandardsJSON::StandardsJSON(const std::string& s) : m_impl(std::shared_ptr<detail::StandardsJSON_Impl>(new detail::StandardsJSON_Impl(s))) {}

StandardsJSON::StandardsJSON(std::shared_ptr<detail::StandardsJSON_Impl> impl) : m_impl(impl) {}

boost::optional<StandardsJSON> StandardsJSON::load(const std::string& s) {
  boost::optional<StandardsJSON> result;
  try {
    result = StandardsJSON(s);
  } catch (const std::exception&) {
  }
  return result;
}

std::string StandardsJSON::string() const {
  return getImpl<detail::StandardsJSON_Impl>()->string();
}

StandardsJSON StandardsJSON::clone() const {
  return getImpl<detail::StandardsJSON_Impl>()->clone();
}

boost::optional<Json::Value> StandardsJSON::getPrimaryKey(const std::string& primaryKey) const {
  return getImpl<detail::StandardsJSON_Impl>()->getPrimaryKey(primaryKey);
}

// ostream operator
std::ostream& operator<<(std::ostream& os, const StandardsJSON& standardsJSON) {
  os << standardsJSON.string();
  return os;
}

}  // namespace openstudio
