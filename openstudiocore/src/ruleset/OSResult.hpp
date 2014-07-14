/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

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
