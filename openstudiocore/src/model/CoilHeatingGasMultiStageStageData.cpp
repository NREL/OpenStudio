/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "CoilHeatingGasMultiStageStageData.hpp"
#include "CoilHeatingGasMultiStageStageData_Impl.hpp"
#include <utilities/idd/OS_Coil_Heating_Gas_MultiStage_StageData_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoilHeatingGasMultiStageStageData_Impl::CoilHeatingGasMultiStageStageData_Impl(const IdfObject& idfObject,
                                                                                 Model_Impl* model,
                                                                                 bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilHeatingGasMultiStageStageData::iddObjectType());
  }

  CoilHeatingGasMultiStageStageData_Impl::CoilHeatingGasMultiStageStageData_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                 Model_Impl* model,
                                                                                 bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilHeatingGasMultiStageStageData::iddObjectType());
  }

  CoilHeatingGasMultiStageStageData_Impl::CoilHeatingGasMultiStageStageData_Impl(const CoilHeatingGasMultiStageStageData_Impl& other,
                                                                                 Model_Impl* model,
                                                                                 bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoilHeatingGasMultiStageStageData_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CoilHeatingGasMultiStageStageData_Impl::iddObjectType() const {
    return CoilHeatingGasMultiStageStageData::iddObjectType();
  }

  double CoilHeatingGasMultiStageStageData_Impl::gasBurnerEfficiency() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_Gas_MultiStage_StageDataFields::GasBurnerEfficiency,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> CoilHeatingGasMultiStageStageData_Impl::nominalCapacity() const {
    return getDouble(OS_Coil_Heating_Gas_MultiStage_StageDataFields::NominalCapacity,true);
  }

  bool CoilHeatingGasMultiStageStageData_Impl::isNominalCapacityAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Coil_Heating_Gas_MultiStage_StageDataFields::NominalCapacity, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double CoilHeatingGasMultiStageStageData_Impl::parasiticElectricLoad() const {
    boost::optional<double> value = getDouble(OS_Coil_Heating_Gas_MultiStage_StageDataFields::ParasiticElectricLoad,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CoilHeatingGasMultiStageStageData_Impl::setGasBurnerEfficiency(double GasBurnerEfficiency) {
    bool result = setDouble(OS_Coil_Heating_Gas_MultiStage_StageDataFields::GasBurnerEfficiency, GasBurnerEfficiency);
    return result;
  }

  bool CoilHeatingGasMultiStageStageData_Impl::setNominalCapacity(boost::optional<double> NominalCapacity) {
    bool result(false);
    if (NominalCapacity) {
      result = setDouble(OS_Coil_Heating_Gas_MultiStage_StageDataFields::NominalCapacity, NominalCapacity.get());
    }
    return result;
  }

  void CoilHeatingGasMultiStageStageData_Impl::autosizeNominalCapacity() {
    bool result = setString(OS_Coil_Heating_Gas_MultiStage_StageDataFields::NominalCapacity, "autosize");
    OS_ASSERT(result);
  }

  void CoilHeatingGasMultiStageStageData_Impl::setParasiticElectricLoad(double ParasiticElectricLoad) {
    bool result = setDouble(OS_Coil_Heating_Gas_MultiStage_StageDataFields::ParasiticElectricLoad, ParasiticElectricLoad);
    OS_ASSERT(result);
  }

} // detail

CoilHeatingGasMultiStageStageData::CoilHeatingGasMultiStageStageData(const Model& model)
  : ModelObject(CoilHeatingGasMultiStageStageData::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilHeatingGasMultiStageStageData_Impl>());

  setGasBurnerEfficiency(0.80);
  autosizeNominalCapacity();
  setParasiticElectricLoad(0.0);
}

IddObjectType CoilHeatingGasMultiStageStageData::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Coil_Heating_Gas_MultiStage_StageData);
}

double CoilHeatingGasMultiStageStageData::gasBurnerEfficiency() const {
  return getImpl<detail::CoilHeatingGasMultiStageStageData_Impl>()->gasBurnerEfficiency();
}

boost::optional<double> CoilHeatingGasMultiStageStageData::nominalCapacity() const {
  return getImpl<detail::CoilHeatingGasMultiStageStageData_Impl>()->nominalCapacity();
}

bool CoilHeatingGasMultiStageStageData::isNominalCapacityAutosized() const {
  return getImpl<detail::CoilHeatingGasMultiStageStageData_Impl>()->isNominalCapacityAutosized();
}

double CoilHeatingGasMultiStageStageData::parasiticElectricLoad() const {
  return getImpl<detail::CoilHeatingGasMultiStageStageData_Impl>()->parasiticElectricLoad();
}

bool CoilHeatingGasMultiStageStageData::setGasBurnerEfficiency(double GasBurnerEfficiency) {
  return getImpl<detail::CoilHeatingGasMultiStageStageData_Impl>()->setGasBurnerEfficiency(GasBurnerEfficiency);
}

bool CoilHeatingGasMultiStageStageData::setNominalCapacity(double NominalCapacity) {
  return getImpl<detail::CoilHeatingGasMultiStageStageData_Impl>()->setNominalCapacity(NominalCapacity);
}

void CoilHeatingGasMultiStageStageData::autosizeNominalCapacity() {
  getImpl<detail::CoilHeatingGasMultiStageStageData_Impl>()->autosizeNominalCapacity();
}

void CoilHeatingGasMultiStageStageData::setParasiticElectricLoad(double ParasiticElectricLoad) {
  getImpl<detail::CoilHeatingGasMultiStageStageData_Impl>()->setParasiticElectricLoad(ParasiticElectricLoad);
}

/// @cond
CoilHeatingGasMultiStageStageData::CoilHeatingGasMultiStageStageData(std::shared_ptr<detail::CoilHeatingGasMultiStageStageData_Impl> impl)
  : ModelObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

