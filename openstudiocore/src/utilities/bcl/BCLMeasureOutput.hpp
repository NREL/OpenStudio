/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef UTILITIES_BCL_BCLMEASUREOUTPUT_HPP
#define UTILITIES_BCL_BCLMEASUREOUTPUT_HPP

#include "../core/Optional.hpp"
#include "../UtilitiesAPI.hpp"

#include <vector>

class QDomDocument;
class QDomElement;

namespace openstudio{

  /** BCLMeasureOutput is a class representing an output of a measure.  This class does not hold the particular
  *  value of any output, it simply declares that the BCLMeasure has this output.**/
  class UTILITIES_API BCLMeasureOutput {
  public:

    // constructor from xml, throws if required arguments are missing
    BCLMeasureOutput(const QDomElement& element);

    // constructor from xml, throws if required arguments are missing
    BCLMeasureOutput(const std::string& name, const std::string& displayName,
                     const boost::optional<std::string>& shortName,
                     const boost::optional<std::string>& description,
                     const std::string& type, const boost::optional<std::string>& units,
                     bool modelDependent);

    std::string name() const;
    std::string displayName() const;
    boost::optional<std::string> shortName() const;
    boost::optional<std::string> description() const;
    std::string type() const;
    boost::optional<std::string> units() const;
    bool modelDependent() const;

    void writeValues(QDomDocument& doc, QDomElement& element) const;

    bool operator==(const BCLMeasureOutput& other) const;

  private:
    std::string m_name;
    std::string m_displayName;
    boost::optional<std::string> m_shortName;
    boost::optional<std::string> m_description;
    std::string m_type;
    boost::optional<std::string> m_units;
    bool m_modelDependent;
  };

  /** Prints BCLMeasureOutput to os. \relates BCLMeasureOutput */
  UTILITIES_API std::ostream& operator<<(std::ostream& os, const BCLMeasureOutput& output);

} // openstudio

#endif // UTILITIES_BCL_BCLMEASUREOUTPUT_HPP
