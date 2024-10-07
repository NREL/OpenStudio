/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_INVERTER_HPP
#define MODEL_INVERTER_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"
#include "ThermalZone.hpp"

namespace openstudio {
namespace model {

  class ElectricLoadCenterDistribution;

  namespace detail {
    class Inverter_Impl;
  }

  /** Inverter is the base class for inverters.
 */
  class MODEL_API Inverter : public ParentObject
  {

   public:
    Inverter(IddObjectType type, const Model& model);

    virtual ~Inverter() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    Inverter(const Inverter& other) = default;
    Inverter(Inverter&& other) = default;
    Inverter& operator=(const Inverter&) = default;
    Inverter& operator=(Inverter&&) = default;

    boost::optional<ElectricLoadCenterDistribution> electricLoadCenterDistribution() const;

    virtual boost::optional<ThermalZone> thermalZone() const;

    virtual bool setThermalZone(const ThermalZone& thermalZone);

    virtual void resetThermalZone();

   protected:
    friend class Model;

    friend class openstudio::IdfObject;

    /// @cond

    using ImplType = detail::Inverter_Impl;

    explicit Inverter(std::shared_ptr<detail::Inverter_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.Inverter");

    /// @endcond
  };

  using OptionalInverter = boost::optional<Inverter>;

  using InverterVector = std::vector<Inverter>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_INVERTER_HPP
