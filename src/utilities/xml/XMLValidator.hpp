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
#include "../core/Optional.hpp"

#include <boost/optional.hpp>

#include <string>

namespace openstudio {

class UTILITIES_API XMLValidator
{
 public:
  /** @name Constructors */
  //@{

  /// Constructor for a new validator
  explicit XMLValidator(const openstudio::path& xsdPath);

  explicit XMLValidator(const std::string& xsdString);

  //@}
  /** @name Destructor */
  //@{

  /// Virtual destructor
  virtual ~XMLValidator() {}

  //@}
  /** @name Getters */
  //@{

  boost::optional<openstudio::path> xsdPath() const;

  boost::optional<std::string> xsdString() const;

  std::vector<std::string> errors() const;

  std::vector<std::string> warnings() const;

  bool isValid() const;

  //@}
  /** @name Setters */
  //@{

  bool validate(const openstudio::path& xmlPath);

  bool validate(const std::string& xmlString);

  //@}
  /** @name Operators */
  //@{

  //@}

 protected:
  void setParser();

 private:
  REGISTER_LOGGER("openstudio.XMLValidator");

  boost::optional<openstudio::path> m_xsdPath;
  boost::optional<std::string> m_xsdString;
  std::vector<std::string> m_errors;
  std::vector<std::string> m_warnings;
};

/// optional XMLValidator
typedef boost::optional<XMLValidator> OptionalXMLValidator;

}  // namespace openstudio

#endif  // UTILITIES_XML_VALIDATOR_HPP
