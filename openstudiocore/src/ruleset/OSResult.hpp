/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef RULESET_OSRESULT_HPP
#define RULESET_OSRESULT_HPP

#include "RulesetAPI.hpp"

#include "../utilities/core/Enum.hpp"
#include "../utilities/core/Logger.hpp"
#include "../utilities/core/LogMessage.hpp"
#include "../utilities/core/Path.hpp"

#include "../utilities/data/Attribute.hpp"

class QDomDocument;
class QDomElement;

namespace openstudio {
namespace ruleset {

/** \class OSResultValue
 *  \brief Possible outcomes of running a UserScript or a Ruleset.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual
 *  macro call is:
 *  \code
OPENSTUDIO_ENUM( OSResultValue,
  ((NA)(NotApplicable)(-1))
  ((Success)(Pass)(0))
  ((Fail)(Fail)(1))
);
 *  \endcode */
OPENSTUDIO_ENUM( OSResultValue,
  ((NA)(NotApplicable)(-1))
  ((Success)(Pass)(0))
  ((Fail)(Fail)(1))
);

/** Class for documenting the outcome of running a UserScript or a Ruleset. There is an overall
 *  result flag (available from value()), and a number of message types. */
class RULESET_API OSResult {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Default constructor sets value() to OSResultValue::Success. */
  OSResult();

  /** Construct from XML content. Called by OSResult::load. */
  explicit OSResult(const QDomElement& element);

  //@}
  /** @name Getters */
  //@{

  OSResultValue value() const;

  std::vector<LogMessage> errors() const;

  std::vector<LogMessage> warnings() const;

  std::vector<LogMessage> info() const;

  boost::optional<LogMessage> initialCondition() const;

  boost::optional<LogMessage> finalCondition() const;

  std::vector<Attribute> attributes() const;

  //@}
  /** @name Setters 
   *
   *  Typically, these methods are not used directly, but are called by OSRunner methods such as 
   *  OSRunner::registerError, OSRunner::registerAsNotApplicable. */
  //@{

  /** Set the result value. */
  void setValue(const OSResultValue& value);

  void addError(const std::string& channel, const std::string& message);

  void addWarning(const std::string& channel, const std::string& message);

  void addInfo(const std::string& channel, const std::string& message);

  void setInitialCondition(const std::string& channel, const std::string& message);

  void setFinalCondition(const std::string& channel, const std::string& message);

  void appendAttribute(const Attribute& attribute);

  //@}
  /** @name Serialization */
  //@{

  /** Loads OSResult from the XML .ossr file at path p, if possible. Otherwise, returns 
   *  boost::none. */
  static boost::optional<OSResult> load(const openstudio::path& p);

  /** Saves this OSResult to path p (using an XML format and extension .ossr). */
  bool save(const openstudio::path& p, bool overwrite = false) const;

  //@}
 private:
  REGISTER_LOGGER("openstudio.ruleset.OSResult")

  OSResultValue m_value;
  std::vector<LogMessage> m_errors;
  std::vector<LogMessage> m_warnings;
  std::vector<LogMessage> m_info;
  boost::optional<LogMessage> m_initialCondition;
  boost::optional<LogMessage> m_finalCondition;
  std::vector<Attribute> m_attributes;

  LogMessage logMessageFromXML(LogLevel level, const QDomElement& element) const;

  QDomDocument toXml() const;

  void logMessagesToXml(QDomDocument& doc,
                        QDomElement& element,
                        const std::vector<LogMessage>& logMessages) const;
};

/** \relates OSResult */
typedef boost::optional<OSResult> OptionalOSResult;

} // ruleset
} // openstudio

#endif // RULESET_OSRESULT_HPP
