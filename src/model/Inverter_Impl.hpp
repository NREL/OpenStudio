/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_INVERTER_IMPL_HPP
#define MODEL_INVERTER_IMPL_HPP

#include "ParentObject_Impl.hpp"
#include "ThermalZone_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class MODEL_API Inverter_Impl : public ParentObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      Inverter_Impl(IddObjectType type, Model_Impl* model);

      Inverter_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      Inverter_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      Inverter_Impl(const Inverter_Impl& other, Model_Impl* model, bool keepHandles);

      virtual ~Inverter_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      //@}
      /** @name Getters */
      //@{

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
      REGISTER_LOGGER("openstudio.model.Inverter");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_INVERTER_IMPL_HPP
