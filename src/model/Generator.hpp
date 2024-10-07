/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_GENERATOR_HPP
#define MODEL_GENERATOR_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {
namespace model {

  class ElectricLoadCenterDistribution;

  namespace detail {
    class Generator_Impl;
  }

  /** Generator is the base class for generators.  The ratedElectricPowerOutput, availabilitySchedule, and
 *  ratedThermaltoElectricalPowerRatio fields are mapped to fields in the ElectricLoadCenter:Generators object
 *  in EnergyPlus.  The ElectricLoadCenter:Generators object does not exist in OpenStudio.
 */
  class MODEL_API Generator : public ParentObject
  {

   public:
    Generator(IddObjectType type, const Model& model);

    virtual ~Generator() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    Generator(const Generator& other) = default;
    Generator(Generator&& other) = default;
    Generator& operator=(const Generator&) = default;
    Generator& operator=(Generator&&) = default;

    std::string generatorObjectType() const;

    boost::optional<double> ratedElectricPowerOutput() const;

    boost::optional<Schedule> availabilitySchedule() const;

    boost::optional<double> ratedThermaltoElectricalPowerRatio() const;

    boost::optional<ElectricLoadCenterDistribution> electricLoadCenterDistribution() const;

   protected:
    friend class Model;
    friend class openstudio::IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    friend class detail::Generator_Impl;

    /// @cond

    using ImplType = detail::Generator_Impl;

    explicit Generator(std::shared_ptr<detail::Generator_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.Generator");

    /// @endcond
  };

  using OptionalGenerator = boost::optional<Generator>;

  using GeneratorVector = std::vector<Generator>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GENERATOR_HPP
