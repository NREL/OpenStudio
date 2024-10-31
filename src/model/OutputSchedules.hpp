/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_OUTPUTSCHEDULES_HPP
#define MODEL_OUTPUTSCHEDULES_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class OutputSchedules_Impl;

  }  // namespace detail

  /** OutputSchedules is a ModelObject that wraps the OpenStudio IDD object 'OS:Output:Schedules'. */
  class MODEL_API OutputSchedules : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~OutputSchedules() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    OutputSchedules(const OutputSchedules& other) = default;
    OutputSchedules(OutputSchedules&& other) = default;
    OutputSchedules& operator=(const OutputSchedules&) = default;
    OutputSchedules& operator=(OutputSchedules&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> keyFieldValues();
    static std::vector<std::string> validKeyFieldValues();

    /** @name Getters */
    //@{

    std::string keyField() const;

    //@}
    /** @name Setters */
    //@{

    bool setKeyField(const std::string& keyField);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::OutputSchedules_Impl;

    explicit OutputSchedules(std::shared_ptr<detail::OutputSchedules_Impl> impl);

    friend class detail::OutputSchedules_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    explicit OutputSchedules(Model& model);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.OutputSchedules");
  };

  /** \relates OutputSchedules*/
  using OptionalOutputSchedules = boost::optional<OutputSchedules>;

  /** \relates OutputSchedules*/
  using OutputSchedulesVector = std::vector<OutputSchedules>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_OUTPUTSCHEDULES_HPP
