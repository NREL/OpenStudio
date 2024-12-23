/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_XML_XMLVALIDATOR_HPP
#define UTILITIES_XML_XMLVALIDATOR_HPP

#include "../UtilitiesAPI.hpp"

#include "../core/Compare.hpp"
#include "../core/Logger.hpp"
#include "../core/LogMessage.hpp"
#include "../core/Optional.hpp"
#include "../core/Path.hpp"
#include "../core/StringStreamLogSink.hpp"
#include "../bcl/BCLEnums.hpp"

#include <boost/optional.hpp>

#include <string>

namespace openstudio {

enum class XMLValidatorType
{
  XSD,
  XSLTSchematron,
  Schematron
};

namespace detail {
  struct XMLInitializer;
}

class UTILITIES_API XMLValidator
{
 public:
  /** @name Constructors */
  //@{

  /** Constructor for a new validator
   *
   * The schemaPath extension is used to determine the type of validation to perform:
   * - `*.xsd` => XSD
   * - `*.xml` or `*.sch` => Schematron (convert to XSLT then validate)
   * - `*.xslt` => Schematron that is already transformed to an XSLT stylesheet */
  explicit XMLValidator(const openstudio::path& schemaPath);
  ~XMLValidator();
  XMLValidator(const XMLValidator& other) = default;
  XMLValidator(XMLValidator&& other) noexcept = default;
  XMLValidator& operator=(const XMLValidator& other) = default;
  XMLValidator& operator=(XMLValidator&& other) noexcept = default;

  static XMLValidator gbxmlValidator(const VersionString& schemaVersion = VersionString(7, 3));

  static XMLValidator bclXMLValidator(openstudio::BCLXMLType bclXMLType = openstudio::BCLXMLType::MeasureXML,
                                      const VersionString& schemaVersion = VersionString(3, 1));

  //@}
  /** @name Getters */
  //@{

  openstudio::path schemaPath() const;

  bool validate(const openstudio::path& xmlPath);

  // Below functions are related to the last call to validate

  bool isValid() const;
  boost::optional<openstudio::path> xmlPath() const;

  std::vector<LogMessage> errors() const;

  std::vector<LogMessage> warnings() const;

  // Returns all log messages
  std::vector<LogMessage> logMessages() const;

  /** Only applicable for Schematron validation **/
  boost::optional<std::string> fullValidationReport() const;

  //@}
  /** @name Other */
  //@{

  /** Static helper to convert a Schematron file to an XSLT StyleSheet */
  static openstudio::path schematronToXslt(const openstudio::path& schemaPath, const openstudio::path& outputDir);

  //@}

 private:
  REGISTER_LOGGER("openstudio.XMLValidator");

  // Instead of using a useless log sink (it's not like we'll trigger warnings/errors coming from model/ or whatnot)
  // We just store the log messages directly
  mutable std::vector<LogMessage> m_logMessages;

  // LOG the message (to console) and store it in m_logMessages
  void logAndStore(LogLevel logLevel, const std::string& logMessage) const;

  openstudio::path m_schemaPath;
  boost::optional<openstudio::path> m_xmlPath;

  boost::optional<openstudio::path> m_tempDir;

  XMLValidatorType m_validatorType;

  bool xsdValidate() const;

  bool xsltValidate() const;
  mutable std::string m_fullValidationReport;

  // reset the state of the XMLValidator between translations
  void reset();

  // A static Initializer, as a Meyers Singleton, to know when we need to init/cleanup the libxml2 & libxslt libs
  // This lazily does the initialization, and clean up will be done completey at the end of the program
  static detail::XMLInitializer& xmlInitializerInstance();
};

/// optional XMLValidator
using OptionalXMLValidator = boost::optional<XMLValidator>;

}  // namespace openstudio

#endif  // UTILITIES_XML_VALIDATOR_HPP
