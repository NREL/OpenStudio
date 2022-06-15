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

#ifndef UTILITIES_XML_XMLVALIDATOR_HPP
#define UTILITIES_XML_XMLVALIDATOR_HPP

#include "../UtilitiesAPI.hpp"

#include "../core/Path.hpp"
#include "../core/Logger.hpp"
#include "../core/LogMessage.hpp"
#include "../core/Optional.hpp"
#include "../core/StringStreamLogSink.hpp"

#include <boost/optional.hpp>

#include <string>

namespace openstudio {

enum class XMLValidatorType
{
  XSD,
  XSLTSchematron,
  Schematron
};

class UTILITIES_API XMLValidator
{
 public:
  /** @name Constructors */
  //@{

  /// Constructor for a new validator
  explicit XMLValidator(const openstudio::path& schemaPath);

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

  /** Only applicable for Schematron validation **/
  boost::optional<std::string> fullValidationReport() const;

  //@}
  /** @name Setters */
  //@{

  //@}
  /** @name Operators */
  //@{

  //@}

 private:
  // TODO: I'm not sure we need that complexity... perhaps just use std::vector<LogMessage> m_logMessages;
  REGISTER_LOGGER("openstudio.XMLValidator");
  StringStreamLogSink m_logSink;
  openstudio::path m_schemaPath;
  boost::optional<openstudio::path> m_xmlPath;

  XMLValidatorType m_validatorType;

  bool xsdValidate() const;

  // TODO: should we provide a facility to convert from schematron to XLST? or let user deal with that
  bool xsltValidate() const;
  mutable std::string m_fullValidationReport;

  // reset the state of the XMLValidator between translations
  void reset();
};

/// optional XMLValidator
using OptionalXMLValidator = boost::optional<XMLValidator>;

}  // namespace openstudio

#endif  // UTILITIES_XML_VALIDATOR_HPP
