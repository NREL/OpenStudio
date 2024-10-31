/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_OUTPUTDEBUGGINGDATA_HPP
#define MODEL_OUTPUTDEBUGGINGDATA_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class OutputDebuggingData_Impl;

  }  // namespace detail

  /** OutputDebuggingData is a ModelObject that wraps the OpenStudio IDD object 'OS:Output:DebuggingData'. */
  class MODEL_API OutputDebuggingData : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~OutputDebuggingData() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    OutputDebuggingData(const OutputDebuggingData& other) = default;
    OutputDebuggingData(OutputDebuggingData&& other) = default;
    OutputDebuggingData& operator=(const OutputDebuggingData&) = default;
    OutputDebuggingData& operator=(OutputDebuggingData&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    bool reportDebuggingData() const;

    bool reportDuringWarmup() const;

    //@}
    /** @name Setters */
    //@{

    bool setReportDebuggingData(bool reportDebuggingData);

    bool setReportDuringWarmup(bool reportDuringWarmup);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::OutputDebuggingData_Impl;

    explicit OutputDebuggingData(std::shared_ptr<detail::OutputDebuggingData_Impl> impl);

    friend class detail::OutputDebuggingData_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    explicit OutputDebuggingData(Model& model);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.OutputDebuggingData");
  };

  /** \relates OutputDebuggingData*/
  using OptionalOutputDebuggingData = boost::optional<OutputDebuggingData>;

  /** \relates OutputDebuggingData*/
  using OutputDebuggingDataVector = std::vector<OutputDebuggingData>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_OUTPUTDEBUGGINGDATA_HPP
