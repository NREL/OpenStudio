/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_OUTPUTTABLESUMMARYREPORTS_IMPL_HPP
#define MODEL_OUTPUTTABLESUMMARYREPORTS_IMPL_HPP

#include <vector>
#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** OutputTableSummaryReports_Impl is a ModelObject_Impl that is the implementation class for OutputTableSummaryReports.*/
    class MODEL_API OutputTableSummaryReports_Impl : public ModelObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      OutputTableSummaryReports_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      OutputTableSummaryReports_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      OutputTableSummaryReports_Impl(const OutputTableSummaryReports_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~OutputTableSummaryReports_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      std::vector<std::string> summaryReports() const;

      unsigned int numberofSummaryReports() const;

      boost::optional<unsigned> summaryReportIndex(const std::string& summaryReport) const;

      boost::optional<std::string> getSummaryReport(unsigned groupIndex) const;

      //@}
      /** @name Setters */
      //@{

      bool addSummaryReport(const std::string& summaryReport);

      bool addSummaryReports(const std::vector<std::string>& summaryReports);

      bool removeSummaryReport(unsigned groupIndex);

      void removeAllSummaryReports();

      // Just a convenience function to add 'AllSummary' as a key since it's so common
      bool enableAllSummaryReport();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.OutputTableSummaryReports");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_OUTPUTTABLESUMMARYREPORTS_IMPL_HPP
