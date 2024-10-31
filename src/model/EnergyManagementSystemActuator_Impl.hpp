/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ENERGYMANAGEMENTSYSTEMACTUATOR_IMPL_HPP
#define MODEL_ENERGYMANAGEMENTSYSTEMACTUATOR_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class ThermalZone;
  class Space;

  namespace detail {

    /** EnergyManagementSystemActuator_Impl is a ModelObject_Impl that is the implementation class for EnergyManagementSystemActuator.*/
    class MODEL_API EnergyManagementSystemActuator_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      EnergyManagementSystemActuator_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      EnergyManagementSystemActuator_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      EnergyManagementSystemActuator_Impl(const EnergyManagementSystemActuator_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~EnergyManagementSystemActuator_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<ModelObject> actuatedComponent() const;

      std::string actuatedComponentControlType() const;

      std::string actuatedComponentType() const;

      boost::optional<ModelObject> zoneOrSpace() const;
      boost::optional<ThermalZone> thermalZone() const;
      boost::optional<Space> space() const;

      //@}
      /** @name Setters */
      //@{

      bool setActuatedComponent(const ModelObject& modelObject);

      bool setActuatedComponentControlType(const std::string& actuatedComponentControlType);

      bool setActuatedComponentType(const std::string& actuatedComponentType);

      bool setThermalZone(const ThermalZone& thermalZone);
      bool setSpace(const Space& space);
      void resetZoneOrSpace();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.EnergyManagementSystemActuator");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ENERGYMANAGEMENTSYSTEMACTUATOR_IMPL_HPP
