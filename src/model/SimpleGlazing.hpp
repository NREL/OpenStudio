/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SIMPLEGLAZING_HPP
#define MODEL_SIMPLEGLAZING_HPP

#include "ModelAPI.hpp"
#include "Glazing.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class SimpleGlazing_Impl;

  }  // namespace detail

  /** SimpleGlazing is a Glazing that wraps the OpenStudio IDD object 'OS:WindowMaterial:SimpleGlazingSystem'. */
  class MODEL_API SimpleGlazing : public Glazing
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SimpleGlazing(const Model& model, double uFactor = 0.1, double solarHeatGainCoefficient = 0.1);

    virtual ~SimpleGlazing() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SimpleGlazing(const SimpleGlazing& other) = default;
    SimpleGlazing(SimpleGlazing&& other) = default;
    SimpleGlazing& operator=(const SimpleGlazing&) = default;
    SimpleGlazing& operator=(SimpleGlazing&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    double uFactor() const;

    double solarHeatGainCoefficient() const;

    boost::optional<double> visibleTransmittance() const;

    //@}
    /** @name Setters */
    //@{

    bool setUFactor(double uFactor);

    bool setSolarHeatGainCoefficient(double solarHeatGainCoefficient);

    bool setVisibleTransmittance(double visibleTransmittance);

    void resetVisibleTransmittance();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::SimpleGlazing_Impl;

    explicit SimpleGlazing(std::shared_ptr<detail::SimpleGlazing_Impl> impl);

    friend class detail::SimpleGlazing_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SimpleGlazing");
  };

  /** \relates SimpleGlazing*/
  using OptionalSimpleGlazing = boost::optional<SimpleGlazing>;

  /** \relates SimpleGlazing*/
  using SimpleGlazingVector = std::vector<SimpleGlazing>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SIMPLEGLAZING_HPP
