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

#ifndef MODEL_COILHEATINGGASMULTISTAGESTAGEDATA_IMPL_HPP
#define MODEL_COILHEATINGGASMULTISTAGESTAGEDATA_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"
#include "../model/CoilHeatingGasMultiStage.hpp"

namespace openstudio {
namespace model {

namespace detail {

  /** CoilHeatingGasMultiStageStageData_Impl is a ModelObject_Impl that is the implementation class for CoilHeatingGasMultiStageStageData.*/
  class MODEL_API CoilHeatingGasMultiStageStageData_Impl : public ModelObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    CoilHeatingGasMultiStageStageData_Impl(const IdfObject& idfObject,
                                           Model_Impl* model,
                                           bool keepHandle);

    CoilHeatingGasMultiStageStageData_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle);

    CoilHeatingGasMultiStageStageData_Impl(const CoilHeatingGasMultiStageStageData_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle);

    virtual ~CoilHeatingGasMultiStageStageData_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    //@}
    /** @name Getters */
    //@{

    double gasBurnerEfficiency() const;

    boost::optional<double> nominalCapacity() const;

    bool isNominalCapacityAutosized() const;

    double parasiticElectricLoad() const;

  boost::optional<double> autosizedNominalCapacity() const ;

  void autosize();

  void applySizingValues();

    //@}
    /** @name Setters */
    //@{

    bool setGasBurnerEfficiency(double StageGasBurnerEfficiency);

    bool setNominalCapacity(boost::optional<double> StageNominalCapacity);

    void autosizeNominalCapacity();

    bool setParasiticElectricLoad(double StageParasiticElectricLoad);

    //@}
    /** @name Other */
    //@{

    // Used to determine the index of this performance data in the
    // list of stages in the parent object.
    boost::optional<std::tuple<int, CoilHeatingGasMultiStage>> stageIndexAndParentCoil() const;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingGasMultiStageStageData");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_COILHEATINGGASMULTISTAGESTAGEDATA_IMPL_HPP
