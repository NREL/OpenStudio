/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_OUTPUTENVIRONMENTALIMPACTFACTORS_HPP
#define MODEL_OUTPUTENVIRONMENTALIMPACTFACTORS_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class OutputEnvironmentalImpactFactors_Impl;

  }  // namespace detail

  /** OutputEnvironmentalImpactFactors is a ModelObject that wraps the OpenStudio IDD object 'OS:Output:EnvironmentalImpactFactors'. */
  class MODEL_API OutputEnvironmentalImpactFactors : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit OutputEnvironmentalImpactFactors(const Model& model);

    virtual ~OutputEnvironmentalImpactFactors() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    OutputEnvironmentalImpactFactors(const OutputEnvironmentalImpactFactors& other) = default;
    OutputEnvironmentalImpactFactors(OutputEnvironmentalImpactFactors&& other) = default;
    OutputEnvironmentalImpactFactors& operator=(const OutputEnvironmentalImpactFactors&) = default;
    OutputEnvironmentalImpactFactors& operator=(OutputEnvironmentalImpactFactors&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> reportingFrequencyValues();

    /** @name Getters */
    //@{

    std::string reportingFrequency() const;

    //@}
    /** @name Setters */
    //@{

    bool setReportingFrequency(const std::string& reportingFrequency);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::OutputEnvironmentalImpactFactors_Impl;

    explicit OutputEnvironmentalImpactFactors(std::shared_ptr<detail::OutputEnvironmentalImpactFactors_Impl> impl);

    friend class detail::OutputEnvironmentalImpactFactors_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.OutputEnvironmentalImpactFactors");
  };

  /** \relates OutputEnvironmentalImpactFactors*/
  using OptionalOutputEnvironmentalImpactFactors = boost::optional<OutputEnvironmentalImpactFactors>;

  /** \relates OutputEnvironmentalImpactFactors*/
  using OutputEnvironmentalImpactFactorsVector = std::vector<OutputEnvironmentalImpactFactors>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_OUTPUTENVIRONMENTALIMPACTFACTORS_HPP
