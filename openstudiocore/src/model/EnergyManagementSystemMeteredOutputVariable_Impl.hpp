/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef MODEL_ENERGYMANAGEMENTSYSTEMMETEREDOUTPUTVARIABLE_IMPL_HPP
#define MODEL_ENERGYMANAGEMENTSYSTEMMETEREDOUTPUTVARIABLE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"
#include "EnergyManagementSystemProgram_Impl.hpp"
#include "EnergyManagementSystemSubroutine_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  /** EnergyManagementSystemMeteredOutputVariable_Impl is a ModelObject_Impl that is the implementation class for EnergyManagementSystemMeteredOutputVariable.*/
  class MODEL_API EnergyManagementSystemMeteredOutputVariable_Impl : public ModelObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    EnergyManagementSystemMeteredOutputVariable_Impl(const IdfObject& idfObject,
                                                     Model_Impl* model,
                                                     bool keepHandle);

    EnergyManagementSystemMeteredOutputVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                     Model_Impl* model,
                                                     bool keepHandle);

    EnergyManagementSystemMeteredOutputVariable_Impl(const EnergyManagementSystemMeteredOutputVariable_Impl& other,
                                                     Model_Impl* model,
                                                     bool keepHandle);

    virtual ~EnergyManagementSystemMeteredOutputVariable_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    //@}
    /** @name Getters */
    //@{

    std::string emsVariableName() const;

    std::string updateFrequency() const;

    std::string emsProgramOrSubroutineName() const;

    boost::optional<EnergyManagementSystemProgram> emsProgram() const;

    boost::optional<EnergyManagementSystemSubroutine> emsSubroutine() const;

    std::string resourceType() const;

    std::string groupType() const;

    std::string endUseCategory() const;

    std::string endUseSubcategory() const;

    std::string units() const;

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

    bool setUpdateFrequency(const std::string& updateFrequency);

    bool setEMSProgramOrSubroutineName(const EnergyManagementSystemProgram& program);

    bool setEMSProgramOrSubroutineName(const EnergyManagementSystemSubroutine& subroutine);

    void resetEMSProgramOrSubroutineName();

    bool setResourceType(const std::string& resourceType);

    bool setGroupType(const std::string& groupType);

    bool setEndUseCategory(const std::string& endUseCategory);

    bool setEndUseSubcategory(const std::string& endUseSubcategory);

    void resetEndUseSubcategory();

    bool setUnits(const std::string& units);

    void resetUnits();

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
    REGISTER_LOGGER("openstudio.model.EnergyManagementSystemMeteredOutputVariable");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_ENERGYMANAGEMENTSYSTEMMETEREDOUTPUTVARIABLE_IMPL_HPP

