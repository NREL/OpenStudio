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

#include "OSOutput.hpp"

#include "../utilities/core/Assert.hpp"

#include <sstream>

namespace openstudio {
namespace measure {

  OSOutput OSOutput::makeBoolOutput(const std::string& name, bool modelDependent) {
    OSOutput result(name, OSOutputType::Boolean, modelDependent);
    return result;
  }

  OSOutput OSOutput::makeDoubleOutput(const std::string& name, bool modelDependent) {
    OSOutput result(name, OSOutputType::Double, modelDependent);
    return result;
  }

  OSOutput OSOutput::makeIntegerOutput(const std::string& name, bool modelDependent) {
    OSOutput result(name, OSOutputType::Integer, modelDependent);
    return result;
  }

  OSOutput OSOutput::makeStringOutput(const std::string& name, bool modelDependent) {
    OSOutput result(name, OSOutputType::String, modelDependent);
    return result;
  }

  std::string OSOutput::name() const {
    return m_name;
  }

  std::string OSOutput::displayName() const {
    return m_displayName;
  }

  std::string OSOutput::shortName() const {
    return m_shortName;
  }

  boost::optional<std::string> OSOutput::description() const {
    return m_description;
  }

  OSOutputType OSOutput::type() const {
    return m_type;
  }

  boost::optional<std::string> OSOutput::units() const {
    return m_units;
  }

  bool OSOutput::modelDependent() const {
    return m_modelDependent;
  }

  void OSOutput::setDisplayName(const std::string& displayName) {
    m_displayName = displayName;
  }

  void OSOutput::setShortName(const std::string& shortName) {
    m_shortName = shortName;
  }

  void OSOutput::setDescription(const std::string& description) {
    m_description = description;
  }

  void OSOutput::setUnits(const std::string& units) {
    m_units = units;
  }

  std::string OSOutput::print() const {
    std::stringstream ss;

    // name
    ss << name();
    if (!displayName().empty()) {
      ss << " (" << displayName() << ")";
    }
    ss << '\n';

    // type and required
    ss << type().valueName() << '\n';

    return ss.str();
  }

  OSOutput::OSOutput() = default;

  OSOutput::OSOutput(const std::string& name, const OSOutputType& type, bool modelDependent)
    : m_name(name), m_displayName(name), m_shortName(name), m_type(type), m_modelDependent(modelDependent) {}

  std::ostream& operator<<(std::ostream& os, const OSOutput& output) {
    os << output.print();
    return os;
  }

}  // namespace measure
}  // namespace openstudio
