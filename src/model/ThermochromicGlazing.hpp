/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_THERMOCHROMICGLAZING_HPP
#define MODEL_THERMOCHROMICGLAZING_HPP

#include "ModelAPI.hpp"
#include "Glazing.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class ThermochromicGlazing_Impl;

  }  // namespace detail

  /** ThermochromicGlazing is a Glazing that wraps the OpenStudio IDD object 'OS:WindowMaterial:GlazingGroup:Thermochromic'. */
  class MODEL_API ThermochromicGlazing : public Glazing
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ThermochromicGlazing(const Model& model, double opticalDataTemperature = 80.0);

    virtual ~ThermochromicGlazing() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ThermochromicGlazing(const ThermochromicGlazing& other) = default;
    ThermochromicGlazing(ThermochromicGlazing&& other) = default;
    ThermochromicGlazing& operator=(const ThermochromicGlazing&) = default;
    ThermochromicGlazing& operator=(ThermochromicGlazing&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    double opticalDataTemperature() const;

    // TODO: Handle this object's extensible fields.

    //@}
    /** @name Setters */
    //@{

    bool setOpticalDataTemperature(double value);

    // TODO: Handle this object's extensible fields.

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::ThermochromicGlazing_Impl;

    explicit ThermochromicGlazing(std::shared_ptr<detail::ThermochromicGlazing_Impl> impl);

    friend class detail::ThermochromicGlazing_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ThermochromicGlazing");
  };

  /** \relates ThermochromicGlazing*/
  using OptionalThermochromicGlazing = boost::optional<ThermochromicGlazing>;

  /** \relates ThermochromicGlazing*/
  using ThermochromicGlazingVector = std::vector<ThermochromicGlazing>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_THERMOCHROMICGLAZING_HPP
