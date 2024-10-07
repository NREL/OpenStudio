/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ENERGYMANAGEMENTSYSTEMPROGRAMCALLINGMANAGER_IMPL_HPP
#define MODEL_ENERGYMANAGEMENTSYSTEMPROGRAMCALLINGMANAGER_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"
#include "EnergyManagementSystemProgram_Impl.hpp"

namespace openstudio {
namespace model {
  // Forward declaration
  class EnergyManagementSystemProgram;

  namespace detail {

    /** EnergyManagementSystemProgramCallingManager_Impl is a ModelObject_Impl that is the implementation class for EnergyManagementSystemProgramCallingManager.*/
    class MODEL_API EnergyManagementSystemProgramCallingManager_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      EnergyManagementSystemProgramCallingManager_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      EnergyManagementSystemProgramCallingManager_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      EnergyManagementSystemProgramCallingManager_Impl(const EnergyManagementSystemProgramCallingManager_Impl& other, Model_Impl* model,
                                                       bool keepHandle);

      virtual ~EnergyManagementSystemProgramCallingManager_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      std::string callingPoint() const;

      std::vector<EnergyManagementSystemProgram> programs() const;

      boost::optional<EnergyManagementSystemProgram> getProgram(unsigned index) const;

      //@}
      /** @name Setters */
      //@{

      bool setCallingPoint(const std::string& callingPoint);

      bool eraseProgram(unsigned index);

      void erasePrograms();

      bool addProgram(const EnergyManagementSystemProgram& program);

      bool setProgram(const EnergyManagementSystemProgram& program, unsigned index);

      bool setPrograms(const std::vector<EnergyManagementSystemProgram>& programs);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.EnergyManagementSystemProgramCallingManager");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ENERGYMANAGEMENTSYSTEMPROGRAMCALLINGMANAGER_IMPL_HPP
