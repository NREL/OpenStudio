/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ELECTRICALSTORAGE_IMPL_HPP
#define MODEL_ELECTRICALSTORAGE_IMPL_HPP

#include "ParentObject_Impl.hpp"
#include "ThermalZone_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** ElectricStorage_Impl is a ParentObject_Impl that is the implementation class for ElectricStorage_Impl.
  It is the Base Class of ElectricLoadCenterStorageSimple, ElectricLoadCenterStorageBattery, and ElectricLoadCenterStorageLiIonNMCBattery*/
    class MODEL_API ElectricalStorage_Impl : public ParentObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ElectricalStorage_Impl(IddObjectType type, Model_Impl* model);

      ElectricalStorage_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ElectricalStorage_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ElectricalStorage_Impl(const ElectricalStorage_Impl& other, Model_Impl* model, bool keepHandles);

      virtual ~ElectricalStorage_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      //@}
      /** @name Getters */
      //@{

      // Convenience method to return the (optional) electricalLoadCenter on which it's assigned
      boost::optional<ElectricLoadCenterDistribution> electricLoadCenterDistribution() const;

      virtual boost::optional<ThermalZone> thermalZone() const = 0;

      //@}
      /** @name Setters */
      //@{

      virtual bool setThermalZone(const ThermalZone& thermalZone) = 0;

      virtual void resetThermalZone() = 0;

      //@}
      /** @name Other */
      //@{

      //@}
     private:
      REGISTER_LOGGER("openstudio.model.ElectricalStorage");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ELECTRICALSTORAGE_IMPL_HPP
