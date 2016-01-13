/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

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
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

