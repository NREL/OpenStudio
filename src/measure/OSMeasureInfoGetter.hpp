/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef MEASURE_OSMEASUREINFOGETTER_HPP
#define MEASURE_OSMEASUREINFOGETTER_HPP

#include "MeasureAPI.hpp"

#include "OSArgument.hpp"
#include "OSOutput.hpp"

#include "../utilities/core/Optional.hpp"
#include "../utilities/bcl/BCLMeasure.hpp"

#include <vector>
#include <string>

namespace openstudio {

class Workspace;

namespace model {
  class Model;
}

namespace measure {

  /** OSMeasureInfo contains information that can be extracted from an OSMeasure by a parser.
 */
  class MEASURE_API OSMeasureInfo
  {
   public:
    /// Constructor used when Ruby interpreter fails to get information about the script
    OSMeasureInfo(const std::string& error);

    /// Constructor used when Ruby interpreter succeeds to get information about the script
    OSMeasureInfo(const MeasureType& measureType, const std::string& className, const std::string& name, const std::string& description,
                  const std::string& taxonomy, const std::string& modelerDescription, const std::vector<OSArgument>& arguments,
                  const std::vector<OSOutput>& outputs);

    /// will be empty if information was successfully extracted from the script, otherwise includes error information
    boost::optional<std::string> error() const;

    /// type of measure if information was successfully extracted from the script, otherwise undefined
    MeasureType measureType() const;

    /// name of measure class if information was successfully extracted from the script, otherwise empty
    std::string className() const;

    /// result of measure's name method if information was successfully extracted from the script, otherwise empty
    std::string name() const;

    /// result of measure's description method if information was successfully extracted from the script, otherwise empty
    std::string description() const;

    /// result of measure's taxonomy method if information was successfully extracted from the script, otherwise empty
    std::string taxonomy() const;

    /// result of measure's modeler_description method if information was successfully extracted from the script, otherwise empty
    std::string modelerDescription() const;

    /// result of measure's arguments method called with empty model if information was successfully extracted from the script, otherwise empty
    std::vector<OSArgument> arguments() const;

    /// result of measure's outputs method called with empty model if information was successfully extracted from the script, otherwise empty
    std::vector<OSOutput> outputs() const;

    /// Compares the measure's information from the xml with information extracted from the ruby measure
    /// Updates the measure if needed, returns true if an update was performed
    bool update(BCLMeasure& measure) const;

   private:
    REGISTER_LOGGER("openstudio.measure.OSMeasureInfo");

    boost::optional<std::string> m_error;
    MeasureType m_measureType;
    std::string m_className;
    std::string m_name;
    std::string m_description;
    std::string m_taxonomy;
    std::string m_modelerDescription;
    std::vector<OSArgument> m_arguments;
    std::vector<OSOutput> m_outputs;
  };

  /** Interface class for using embedded Ruby to extract arguments from an OSMeasure.
 *  See src/measure/test/EmbeddedRuby_GTest.cpp for this use case. If your application is written in
 *  Ruby, this functionality is available through non-member helper function
 *  Opudio::Measure::infoExtractorRubyFunction(BCLMeasure), see
 *  ruby/openstudio/measure/test/OSMeasureInfoGetter_Test.rb for an example. */
  class MEASURE_API OSMeasureInfoGetter
  {
   public:
    virtual ~OSMeasureInfoGetter() = default;

    virtual OSMeasureInfo getInfo(const BCLMeasure& measure) = 0;

    /** Use this method to pass in a (EnergyPlus) Workspace or a (OpenStudio) Model. */
    virtual OSMeasureInfo getInfo(const BCLMeasure& measure, const Workspace& workspace) = 0;

    virtual OSMeasureInfo getInfo(const BCLMeasure& measure, const model::Model& model, const Workspace& workspace) = 0;

    OSMeasureInfo getInfo(const BCLMeasure& measure, const boost::optional<model::Model>& model, const boost::optional<Workspace>& workspace);
  };

  /** Non-member function that returns Ruby code text for retrieving info from a BCLMeasure. This
 *  function is used by both the embedded Ruby OSMeasureInfoGetter and native Ruby
 *  (OpenStudio::Measure::getInfo) implementations of the functionality, such that either usage
 *  pathway should produce identical results. \relates OSMeasureInfoGetter */
  MEASURE_API std::string infoExtractorRubyFunction();

}  // namespace measure
}  // namespace openstudio

#endif  // MEASURE_OSMEASUREINFOGETTER_HPP
