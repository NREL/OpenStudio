/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_GLAZING_HPP
#define MODEL_GLAZING_HPP

#include "ModelAPI.hpp"
#include "FenestrationMaterial.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class Glazing_Impl;
  }

  /** Glazing is a FenestrationMaterial that serves as a base class for several types of glass
 *  layers in fenestration \link LayeredConstruction LayeredConstructions \endlink. */
  class MODEL_API Glazing : public FenestrationMaterial
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~Glazing() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    Glazing(const Glazing& other) = default;
    Glazing(Glazing&& other) = default;
    Glazing& operator=(const Glazing&) = default;
    Glazing& operator=(Glazing&&) = default;

    //@}
   protected:
    /// @cond
    using ImplType = detail::Glazing_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class detail::Glazing_Impl;

    Glazing(IddObjectType type, const Model& model);

    explicit Glazing(std::shared_ptr<detail::Glazing_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.Glazing");
  };

  /** \relates Glazing */
  using OptionalGlazing = boost::optional<Glazing>;

  /** \relates Glazing */
  using GlazingVector = std::vector<Glazing>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GLAZING_HPP
