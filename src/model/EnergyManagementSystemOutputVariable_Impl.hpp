/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ENERGYMANAGEMENTSYSTEMOUTPUTVARIABLE_IMPL_HPP
#define MODEL_ENERGYMANAGEMENTSYSTEMOUTPUTVARIABLE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** EnergyManagementSystemOutputVariable_Impl is a ModelObject_Impl that is the implementation class for EnergyManagementSystemOutputVariable.*/
    class MODEL_API EnergyManagementSystemOutputVariable_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      EnergyManagementSystemOutputVariable_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      EnergyManagementSystemOutputVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      EnergyManagementSystemOutputVariable_Impl(const EnergyManagementSystemOutputVariable_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~EnergyManagementSystemOutputVariable_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      std::string emsVariableName() const;

      std::string typeOfDataInVariable() const;

      std::string updateFrequency() const;

      std::string emsProgramOrSubroutineName() const;

      boost::optional<EnergyManagementSystemProgram> emsProgram() const;

      boost::optional<EnergyManagementSystemSubroutine> emsSubroutine() const;

      std::string units() const;

      boost::optional<ModelObject> emsVariableObject() const;

      boost::optional<EnergyManagementSystemActuator> emsActuator() const;
      boost::optional<EnergyManagementSystemSensor> emsSensor() const;
      boost::optional<EnergyManagementSystemGlobalVariable> emsGlobalVariable() const;
      boost::optional<EnergyManagementSystemTrendVariable> emsTrendVariable() const;
      boost::optional<EnergyManagementSystemInternalVariable> emsInternalVariable() const;
      boost::optional<EnergyManagementSystemCurveOrTableIndexVariable> emsCurveOrTableIndexVariable() const;
      boost::optional<EnergyManagementSystemConstructionIndexVariable> emsConstructionIndexVariable() const;

      bool exportToBCVTB() const;

      bool isExportToBCVTBDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setEMSVariableName(const std::string& eMSVariableName);

      bool setTypeOfDataInVariable(const std::string& typeofDatainVariable);

      bool setUpdateFrequency(const std::string& updateFrequency);

      bool setEMSProgramOrSubroutineName(const EnergyManagementSystemProgram& program);

      bool setEMSProgramOrSubroutineName(const EnergyManagementSystemSubroutine& subroutine);

      void resetEMSProgramOrSubroutineName();

      bool setUnits(const std::string& units);

      void resetUnits();

      bool setEMSVariableName(const EnergyManagementSystemActuator& object);
      bool setEMSVariableName(const EnergyManagementSystemSensor& object);
      bool setEMSVariableName(const EnergyManagementSystemGlobalVariable& object);
      bool setEMSVariableName(const EnergyManagementSystemTrendVariable& object);
      bool setEMSVariableName(const EnergyManagementSystemInternalVariable& object);
      bool setEMSVariableName(const EnergyManagementSystemCurveOrTableIndexVariable& object);
      bool setEMSVariableName(const EnergyManagementSystemConstructionIndexVariable& object);

      bool setExportToBCVTB(bool exportToBCVTB);

      void resetExportToBCVTB();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.EnergyManagementSystemOutputVariable");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ENERGYMANAGEMENTSYSTEMOUTPUTVARIABLE_IMPL_HPP
