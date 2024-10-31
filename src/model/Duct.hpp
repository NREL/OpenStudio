/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_DUCT_HPP
#define MODEL_DUCT_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class Duct_Impl;

  }  // namespace detail

  class MODEL_API Duct : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit Duct(const Model& model);

    virtual ~Duct() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    Duct(const Duct& other) = default;
    Duct(Duct&& other) = default;
    Duct& operator=(const Duct&) = default;
    Duct& operator=(Duct&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    //@}
    /** @name Setters */
    //@{

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::Duct_Impl;

    explicit Duct(std::shared_ptr<detail::Duct_Impl> impl);

    friend class detail::Duct_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.Duct");
  };

  /** \relates Duct*/
  using OptionalDuct = boost::optional<Duct>;

  /** \relates Duct*/
  using DuctVector = std::vector<Duct>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_DUCT_HPP
