/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRGAP_HPP
#define MODEL_AIRGAP_HPP

#include "ModelAPI.hpp"
#include "OpaqueMaterial.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class AirGap_Impl;

  }  // namespace detail

  /** AirGap is a OpaqueMaterial that wraps the OpenStudio IDD object 'OS:Material:AirGap'. */
  class MODEL_API AirGap : public OpaqueMaterial
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit AirGap(const Model& model, double thermalResistance = 0.1);

    virtual ~AirGap() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirGap(const AirGap& other) = default;
    AirGap(AirGap&& other) = default;
    AirGap& operator=(const AirGap&) = default;
    AirGap& operator=(AirGap&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    double thermalResistance() const;

    //@}
    /** @name Setters */
    //@{

    bool setThermalResistance(double thermalResistance);

    // Resets the thermal resistance to the default of 0.1 m2-K/W
    void resetThermalResistance();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirGap_Impl;

    explicit AirGap(std::shared_ptr<detail::AirGap_Impl> impl);

    friend class detail::AirGap_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirGap");
  };

  /** \relates AirGap*/
  using OptionalAirGap = boost::optional<AirGap>;

  /** \relates AirGap*/
  using AirGapVector = std::vector<AirGap>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRGAP_HPP
