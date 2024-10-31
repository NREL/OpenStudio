/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ENERGYMANAGEMENTSYSTEMTRENDVARIABLE_IMPL_HPP
#define MODEL_ENERGYMANAGEMENTSYSTEMTRENDVARIABLE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** EnergyManagementSystemTrendVariable_Impl is a ModelObject_Impl that is the implementation class for EnergyManagementSystemTrendVariable.*/
    class MODEL_API EnergyManagementSystemTrendVariable_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      EnergyManagementSystemTrendVariable_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      EnergyManagementSystemTrendVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      EnergyManagementSystemTrendVariable_Impl(const EnergyManagementSystemTrendVariable_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~EnergyManagementSystemTrendVariable_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      std::string emsVariableName() const;

      int numberOfTimestepsToBeLogged() const;

      boost::optional<ModelObject> emsVariableObject() const;

      boost::optional<EnergyManagementSystemActuator> emsActuator() const;
      boost::optional<EnergyManagementSystemSensor> emsSensor() const;
      boost::optional<EnergyManagementSystemGlobalVariable> emsGlobalVariable() const;
      boost::optional<EnergyManagementSystemTrendVariable> emsTrendVariable() const;
      boost::optional<EnergyManagementSystemInternalVariable> emsInternalVariable() const;
      boost::optional<EnergyManagementSystemCurveOrTableIndexVariable> emsCurveOrTableIndexVariable() const;
      boost::optional<EnergyManagementSystemConstructionIndexVariable> emsConstructionIndexVariable() const;

      //@}
      /** @name Setters */
      //@{

      bool setEMSVariableName(const std::string& eMSVariableName);

      bool setNumberOfTimestepsToBeLogged(int numberofTimestepstobeLogged);

      bool setEMSVariableName(const EnergyManagementSystemActuator& object);
      bool setEMSVariableName(const EnergyManagementSystemSensor& object);
      bool setEMSVariableName(const EnergyManagementSystemGlobalVariable& object);
      bool setEMSVariableName(const EnergyManagementSystemTrendVariable& object);
      bool setEMSVariableName(const EnergyManagementSystemInternalVariable& object);
      bool setEMSVariableName(const EnergyManagementSystemCurveOrTableIndexVariable& object);
      bool setEMSVariableName(const EnergyManagementSystemConstructionIndexVariable& object);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.EnergyManagementSystemTrendVariable");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ENERGYMANAGEMENTSYSTEMTRENDVARIABLE_IMPL_HPP
