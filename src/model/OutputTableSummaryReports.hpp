/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

    virtual ~OutputTableSummaryReports() override = default;
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
