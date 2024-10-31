/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_EXTERIORLIGHTSDEFINITION_HPP
#define MODEL_EXTERIORLIGHTSDEFINITION_HPP

#include "ModelAPI.hpp"
#include "ExteriorLoadDefinition.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class ExteriorLightsDefinition_Impl;

  }  // namespace detail

  /** ExteriorLightsDefinition is a ExteriorLoadDefinition that wraps the OpenStudio IDD object
 *  'OS:Exterior:Lights:Definition'. */
  class MODEL_API ExteriorLightsDefinition : public ExteriorLoadDefinition
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Defaults design level to 0.0 W. */
    explicit ExteriorLightsDefinition(const Model& model);

    virtual ~ExteriorLightsDefinition() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ExteriorLightsDefinition(const ExteriorLightsDefinition& other) = default;
    ExteriorLightsDefinition(ExteriorLightsDefinition&& other) = default;
    ExteriorLightsDefinition& operator=(const ExteriorLightsDefinition&) = default;
    ExteriorLightsDefinition& operator=(ExteriorLightsDefinition&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    double designLevel() const;

    //@}
    /** @name Setters */
    //@{

    bool setDesignLevel(double designLevel);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::ExteriorLightsDefinition_Impl;

    explicit ExteriorLightsDefinition(std::shared_ptr<detail::ExteriorLightsDefinition_Impl> impl);

    friend class detail::ExteriorLightsDefinition_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ExteriorLightsDefinition");
  };

  /** \relates ExteriorLightsDefinition*/
  using OptionalExteriorLightsDefinition = boost::optional<ExteriorLightsDefinition>;

  /** \relates ExteriorLightsDefinition*/
  using ExteriorLightsDefinitionVector = std::vector<ExteriorLightsDefinition>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EXTERIORLIGHTSDEFINITION_HPP
