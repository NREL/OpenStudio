/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef MODEL_OUTPUTTABLESUMMARYREPORTS_HPP
#define MODEL_OUTPUTTABLESUMMARYREPORTS_HPP

#include <vector>
#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class OutputTableSummaryReports_Impl;

  }  // namespace detail

  /** OutputTableSummaryReports is a ModelObject that wraps the OpenStudio IDD object 'OS:Output:Table:SummaryReports'. */
  class MODEL_API OutputTableSummaryReports : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~OutputTableSummaryReports() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    OutputTableSummaryReports(const OutputTableSummaryReports& other) = default;
    OutputTableSummaryReports(OutputTableSummaryReports&& other) = default;
    OutputTableSummaryReports& operator=(const OutputTableSummaryReports&) = default;
    OutputTableSummaryReports& operator=(OutputTableSummaryReports&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> summaryReportValues();
    static std::vector<std::string> validSummaryReportValues();

    /** @name Getters */
    //@{

    std::vector<std::string> summaryReports() const;

    unsigned int numberofSummaryReports() const;

    /** If a summaryReport group is already present (cf `summaryReportIndex()`), it will Warn */
    boost::optional<unsigned> summaryReportIndex(const std::string& summaryReport) const;

    boost::optional<std::string> getSummaryReport(unsigned groupIndex) const;

    //@}
    /** @name Setters */
    //@{

    bool addSummaryReport(const std::string& summaryReport);

    bool addSummaryReports(const std::vector<std::string>& summaryReports);

    bool removeSummaryReport(int groupIndex);

    void removeAllSummaryReports();

    // Just a convenience function to **add** 'AllSummary' as a Report Name since it's so common
    bool enableAllSummaryReport();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    explicit OutputTableSummaryReports(Model& model);

    /// @cond
    using ImplType = detail::OutputTableSummaryReports_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class detail::OutputTableSummaryReports_Impl;

    explicit OutputTableSummaryReports(std::shared_ptr<detail::OutputTableSummaryReports_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.OutputTableSummaryReports");
  };

  /** \relates OutputTableSummaryReports*/
  using OptionalOutputTableSummaryReports = boost::optional<OutputTableSummaryReports>;

  /** \relates OutputTableSummaryReports*/
  using OutputTableSummaryReportsVector = std::vector<OutputTableSummaryReports>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_OUTPUTTABLESUMMARYREPORTS_HPP
