/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ELECTRICALSTORAGE_HPP
#define MODEL_ELECTRICALSTORAGE_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"
#include "ThermalZone.hpp"

namespace openstudio {
namespace model {

  class ElectricLoadCenterDistribution;

  namespace detail {
    class ElectricalStorage_Impl;
  }

  /** ElectricStorage is a ParentObject.
It is the Base Class of ElectricLoadCenterStorageSimple, ElectricLoadCenterStorageBattery, ElectricLoadCenterStorageLiIonNMCBattery*/
  class MODEL_API ElectricalStorage : public ParentObject
  {

   public:
    ElectricalStorage(IddObjectType type, const Model& model);

    virtual ~ElectricalStorage() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ElectricalStorage(const ElectricalStorage& other) = default;
    ElectricalStorage(ElectricalStorage&& other) = default;
    ElectricalStorage& operator=(const ElectricalStorage&) = default;
    ElectricalStorage& operator=(ElectricalStorage&&) = default;

    boost::optional<ElectricLoadCenterDistribution> electricLoadCenterDistribution() const;

    virtual boost::optional<ThermalZone> thermalZone() const;

    virtual bool setThermalZone(const ThermalZone& thermalZone);

    virtual void resetThermalZone();

   protected:
    friend class Model;

    friend class openstudio::IdfObject;

    /// @cond

    using ImplType = detail::ElectricalStorage_Impl;

    explicit ElectricalStorage(std::shared_ptr<detail::ElectricalStorage_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.ElectricalStorage");

    /// @endcond
  };

  using OptionalElectricalStorage = boost::optional<ElectricalStorage>;

  using ElectricalStorageVector = std::vector<ElectricalStorage>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ELECTRICALSTORAGE_HPP
