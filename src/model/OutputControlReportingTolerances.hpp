/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_OUTPUTCONTROLREPORTINGTOLERANCES_HPP
#define MODEL_OUTPUTCONTROLREPORTINGTOLERANCES_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class OutputControlReportingTolerances_Impl;

  }  // namespace detail

  /** OutputControlReportingTolerances is a ModelObject that wraps the OpenStudio IDD object 'OS:OutputControl:ReportingTolerances'. */
  class MODEL_API OutputControlReportingTolerances : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~OutputControlReportingTolerances() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    OutputControlReportingTolerances(const OutputControlReportingTolerances& other) = default;
    OutputControlReportingTolerances(OutputControlReportingTolerances&& other) = default;
    OutputControlReportingTolerances& operator=(const OutputControlReportingTolerances&) = default;
    OutputControlReportingTolerances& operator=(OutputControlReportingTolerances&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    double toleranceforTimeHeatingSetpointNotMet() const;

    bool isToleranceforTimeHeatingSetpointNotMetDefaulted() const;

    double toleranceforTimeCoolingSetpointNotMet() const;

    bool isToleranceforTimeCoolingSetpointNotMetDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setToleranceforTimeHeatingSetpointNotMet(double toleranceforTimeHeatingSetpointNotMet);

    void resetToleranceforTimeHeatingSetpointNotMet();

    bool setToleranceforTimeCoolingSetpointNotMet(double toleranceforTimeCoolingSetpointNotMet);

    void resetToleranceforTimeCoolingSetpointNotMet();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::OutputControlReportingTolerances_Impl;

    explicit OutputControlReportingTolerances(std::shared_ptr<detail::OutputControlReportingTolerances_Impl> impl);

    friend class detail::OutputControlReportingTolerances_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    explicit OutputControlReportingTolerances(Model& model);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.OutputControlReportingTolerances");
  };

  /** \relates OutputControlReportingTolerances*/
  using OptionalOutputControlReportingTolerances = boost::optional<OutputControlReportingTolerances>;

  /** \relates OutputControlReportingTolerances*/
  using OutputControlReportingTolerancesVector = std::vector<OutputControlReportingTolerances>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_OUTPUTCONTROLREPORTINGTOLERANCES_HPP
