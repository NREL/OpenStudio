/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_EXTERIORFUELEQUIPMENTDEFINITION_IMPL_HPP
#define MODEL_EXTERIORFUELEQUIPMENTDEFINITION_IMPL_HPP

#include "ModelAPI.hpp"
#include "ExteriorLoadDefinition_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** ExteriorFuelEquipmentDefinition_Impl is a ExteriorLoadDefinition_Impl that is the implementation class for ExteriorFuelEquipmentDefinition.*/
    class MODEL_API ExteriorFuelEquipmentDefinition_Impl : public ExteriorLoadDefinition_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ExteriorFuelEquipmentDefinition_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ExteriorFuelEquipmentDefinition_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ExteriorFuelEquipmentDefinition_Impl(const ExteriorFuelEquipmentDefinition_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ExteriorFuelEquipmentDefinition_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      // TODO: remove (unused)
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      double designLevel() const;

      //@}
      /** @name Setters */
      //@{

      bool setDesignLevel(double designLevel);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ExteriorFuelEquipmentDefinition");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EXTERIORFUELEQUIPMENTDEFINITION_IMPL_HPP
