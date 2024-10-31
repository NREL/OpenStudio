/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_REFRIGERATIONDEFROSTCYCLEPARAMETERS_HPP
#define MODEL_REFRIGERATIONDEFROSTCYCLEPARAMETERS_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class RefrigerationDefrostCycleParameters_Impl;

  }  // namespace detail

  /** RefrigerationDefrostCycleParameters is a ModelObject that wraps the OpenStudio IDD object 'OS:Refrigeration:DefrostCycleParameters'. */
  class MODEL_API RefrigerationDefrostCycleParameters : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit RefrigerationDefrostCycleParameters(const Model& model);

    virtual ~RefrigerationDefrostCycleParameters() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    RefrigerationDefrostCycleParameters(const RefrigerationDefrostCycleParameters& other) = default;
    RefrigerationDefrostCycleParameters(RefrigerationDefrostCycleParameters&& other) = default;
    RefrigerationDefrostCycleParameters& operator=(const RefrigerationDefrostCycleParameters&) = default;
    RefrigerationDefrostCycleParameters& operator=(RefrigerationDefrostCycleParameters&&) = default;

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
    using ImplType = detail::RefrigerationDefrostCycleParameters_Impl;

    explicit RefrigerationDefrostCycleParameters(std::shared_ptr<detail::RefrigerationDefrostCycleParameters_Impl> impl);

    friend class detail::RefrigerationDefrostCycleParameters_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.RefrigerationDefrostCycleParameters");
  };

  /** \relates RefrigerationDefrostCycleParameters*/
  using OptionalRefrigerationDefrostCycleParameters = boost::optional<RefrigerationDefrostCycleParameters>;

  /** \relates RefrigerationDefrostCycleParameters*/
  using RefrigerationDefrostCycleParametersVector = std::vector<RefrigerationDefrostCycleParameters>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_REFRIGERATIONDEFROSTCYCLEPARAMETERS_HPP
