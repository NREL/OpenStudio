/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_EXTERIORLOADDEFINITION_HPP
#define MODEL_EXTERIORLOADDEFINITION_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

namespace openstudio {
namespace model {

  class ExteriorLoadInstance;

  namespace detail {
    class ExteriorLoadDefinition_Impl;
  }

  /** ExteriorLoadDefinition is an abstract class derived from ResourceObject. \link ExteriorLoadDefinition
 *  ExteriorLoadDefinitions \endlink describe internal loads in a generic way that can be used by
 *  multiple \link ExteriorLoadInstance ExteriorLoadInstances \endlink. */
  class MODEL_API ExteriorLoadDefinition : public ResourceObject
  {
   public:
    virtual ~ExteriorLoadDefinition() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ExteriorLoadDefinition(const ExteriorLoadDefinition& other) = default;
    ExteriorLoadDefinition(ExteriorLoadDefinition&& other) = default;
    ExteriorLoadDefinition& operator=(const ExteriorLoadDefinition&) = default;
    ExteriorLoadDefinition& operator=(ExteriorLoadDefinition&&) = default;

    /** Returns all instances (ExteriorLoad) of this definition. */
    std::vector<ExteriorLoadInstance> instances() const;

    /** Returns the number of instances of this exterior load definition.
  ExteriorLoad multipliers are included in the result **/
    int quantity() const;

   protected:
    /** @name Constructors and Destructors */
    //@{

    /// Constructs a new ExteriorLoadDefinition object in the model.
    ExteriorLoadDefinition(IddObjectType type, const Model& model);

    //@}

    /// @cond

    using ImplType = detail::ExteriorLoadDefinition_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class detail::ExteriorLoadDefinition_Impl;

    explicit ExteriorLoadDefinition(std::shared_ptr<detail::ExteriorLoadDefinition_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.ExteriorLoadDefinition");

    /// @endcond
  };

  /** \relates ExteriorLoadDefinition */
  using OptionalExteriorLoadDefinition = boost::optional<ExteriorLoadDefinition>;

  /** \relates ExteriorLoadDefinition */
  using ExteriorLoadDefinitionVector = std::vector<ExteriorLoadDefinition>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EXTERIORLOADDEFINITION_HPP
