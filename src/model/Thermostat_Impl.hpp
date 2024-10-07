/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_THERMOSTAT_IMPL_HPP
#define MODEL_THERMOSTAT_IMPL_HPP

#include "ModelObject_Impl.hpp"

namespace openstudio {

namespace model {

  class ThermalZone;

  namespace detail {

    class MODEL_API Thermostat_Impl : public ModelObject_Impl
    {

     public:
      Thermostat_Impl(IddObjectType type, Model_Impl* model);

      Thermostat_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      Thermostat_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      Thermostat_Impl(const Thermostat_Impl& other, Model_Impl* model, bool keepHandles);

      boost::optional<ThermalZone> thermalZone() const;

      virtual ~Thermostat_Impl() override = default;

     protected:
      friend class Model_Impl;

     private:
      REGISTER_LOGGER("openstudio.model.Thermostat");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif
