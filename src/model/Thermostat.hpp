/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_THERMOSTAT_HPP
#define MODEL_THERMOSTAT_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  class ThermalZone;

  namespace detail {
    class Thermostat_Impl;
  };

  /** Thermostat is the base class for all types of thermostats.
 */
  class MODEL_API Thermostat : public ModelObject
  {
   public:
    virtual ~Thermostat() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    Thermostat(const Thermostat& other) = default;
    Thermostat(Thermostat&& other) = default;
    Thermostat& operator=(const Thermostat&) = default;
    Thermostat& operator=(Thermostat&&) = default;

    boost::optional<ThermalZone> thermalZone() const;

   protected:
    Thermostat(IddObjectType type, const Model& model);

    using ImplType = detail::Thermostat_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    explicit Thermostat(std::shared_ptr<ImplType> impl);

   private:
    REGISTER_LOGGER("openstudio.model.Thermostat");
  };

  using OptionalThermostat = boost::optional<Thermostat>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_THERMOSTAT_HPP
