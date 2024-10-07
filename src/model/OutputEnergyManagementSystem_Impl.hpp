/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_OUTPUTENERGYMANAGEMENTSYSTEM_IMPL_HPP
#define MODEL_OUTPUTENERGYMANAGEMENTSYSTEM_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** OutputEnergyManagementSystem_Impl is a ModelObject_Impl that is the implementation class for OutputEnergyManagementSystem.*/
    class MODEL_API OutputEnergyManagementSystem_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      OutputEnergyManagementSystem_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      OutputEnergyManagementSystem_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      OutputEnergyManagementSystem_Impl(const OutputEnergyManagementSystem_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~OutputEnergyManagementSystem_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      std::string actuatorAvailabilityDictionaryReporting() const;

      bool isActuatorAvailabilityDictionaryReportingDefaulted() const;

      std::string internalVariableAvailabilityDictionaryReporting() const;

      bool isInternalVariableAvailabilityDictionaryReportingDefaulted() const;

      std::string eMSRuntimeLanguageDebugOutputLevel() const;

      bool isEMSRuntimeLanguageDebugOutputLevelDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setActuatorAvailabilityDictionaryReporting(const std::string& actuatorAvailabilityDictionaryReporting);

      void resetActuatorAvailabilityDictionaryReporting();

      bool setInternalVariableAvailabilityDictionaryReporting(const std::string& internalVariableAvailabilityDictionaryReporting);

      void resetInternalVariableAvailabilityDictionaryReporting();

      bool setEMSRuntimeLanguageDebugOutputLevel(const std::string& eMSRuntimeLanguageDebugOutputLevel);

      void resetEMSRuntimeLanguageDebugOutputLevel();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.OutputEnergyManagementSystem");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_OUTPUTENERGYMANAGEMENTSYSTEM_IMPL_HPP
