/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ENERGYMANAGEMENTSYSTEMSUBROUTINE_IMPL_HPP
#define MODEL_ENERGYMANAGEMENTSYSTEMSUBROUTINE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** EnergyManagementSystemSubroutine_Impl is a ModelObject_Impl that is the implementation class for EnergyManagementSystemSubroutine.*/
    class MODEL_API EnergyManagementSystemSubroutine_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      EnergyManagementSystemSubroutine_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      EnergyManagementSystemSubroutine_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      EnergyManagementSystemSubroutine_Impl(const EnergyManagementSystemSubroutine_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~EnergyManagementSystemSubroutine_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      std::string body() const;

      std::vector<std::string> lines() const;

      std::vector<ModelObject> referencedObjects() const;

      std::vector<std::string> invalidReferencedObjects() const;

      //@}
      /** @name Setters */
      //@{

      bool setBody(const std::string& body);

      bool resetBody();

      bool addLine(const std::string& body);

      bool setLines(const std::vector<std::string>& body);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.EnergyManagementSystemSubroutine");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ENERGYMANAGEMENTSYSTEMSUBROUTINE_IMPL_HPP
