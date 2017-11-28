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

#include "RefrigerationCondenserWaterCooled.hpp"
#include "RefrigerationCondenserWaterCooled_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "Node.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Refrigeration_Condenser_WaterCooled_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  RefrigerationCondenserWaterCooled_Impl::RefrigerationCondenserWaterCooled_Impl(const IdfObject& idfObject,
                                                                                 Model_Impl* model,
                                                                                 bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == RefrigerationCondenserWaterCooled::iddObjectType());
  }

  RefrigerationCondenserWaterCooled_Impl::RefrigerationCondenserWaterCooled_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                 Model_Impl* model,
                                                                                 bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == RefrigerationCondenserWaterCooled::iddObjectType());
  }

  RefrigerationCondenserWaterCooled_Impl::RefrigerationCondenserWaterCooled_Impl(const RefrigerationCondenserWaterCooled_Impl& other,
                                                                                 Model_Impl* model,
                                                                                 bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  unsigned RefrigerationCondenserWaterCooled_Impl::inletPort()
  {
    return OS_Refrigeration_Condenser_WaterCooledFields::WaterInletNodeName;
  }

  unsigned RefrigerationCondenserWaterCooled_Impl::outletPort()
  {
    return OS_Refrigeration_Condenser_WaterCooledFields::WaterOutletNodeName;
  }

  const std::vector<std::string>& RefrigerationCondenserWaterCooled_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType RefrigerationCondenserWaterCooled_Impl::iddObjectType() const {
    return RefrigerationCondenserWaterCooled::iddObjectType();
  }

  std::vector<ScheduleTypeKey> RefrigerationCondenserWaterCooled_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Refrigeration_Condenser_WaterCooledFields::WaterOutletTemperatureScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("RefrigerationCondenserWaterCooled","Water Outlet Temperature"));
    }
    return result;
  }

  bool RefrigerationCondenserWaterCooled_Impl::addToNode(Node & node)
  {
    if( boost::optional<PlantLoop> plant = node.plantLoop() )
    {
      if( plant->demandComponent(node.handle()) )
      {
        return StraightComponent_Impl::addToNode(node);
      }
    }

    return false;
  }

  boost::optional<double> RefrigerationCondenserWaterCooled_Impl::ratedEffectiveTotalHeatRejectionRate() const {
    return getDouble(OS_Refrigeration_Condenser_WaterCooledFields::RatedEffectiveTotalHeatRejectionRate,true);
  }

  double RefrigerationCondenserWaterCooled_Impl::ratedCondensingTemperature() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_Condenser_WaterCooledFields::RatedCondensingTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double RefrigerationCondenserWaterCooled_Impl::ratedSubcoolingTemperatureDifference() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_Condenser_WaterCooledFields::RatedSubcoolingTemperatureDifference,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCondenserWaterCooled_Impl::isRatedSubcoolingTemperatureDifferenceDefaulted() const {
    return isEmpty(OS_Refrigeration_Condenser_WaterCooledFields::RatedSubcoolingTemperatureDifference);
  }

  double RefrigerationCondenserWaterCooled_Impl::ratedWaterInletTemperature() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_Condenser_WaterCooledFields::RatedWaterInletTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string RefrigerationCondenserWaterCooled_Impl::waterCooledLoopFlowType() const {
    boost::optional<std::string> value = getString(OS_Refrigeration_Condenser_WaterCooledFields::WaterCooledLoopFlowType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCondenserWaterCooled_Impl::isWaterCooledLoopFlowTypeDefaulted() const {
    return isEmpty(OS_Refrigeration_Condenser_WaterCooledFields::WaterCooledLoopFlowType);
  }

  boost::optional<Schedule> RefrigerationCondenserWaterCooled_Impl::waterOutletTemperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Refrigeration_Condenser_WaterCooledFields::WaterOutletTemperatureScheduleName);
  }

  boost::optional<double> RefrigerationCondenserWaterCooled_Impl::waterDesignFlowRate() const {
    return getDouble(OS_Refrigeration_Condenser_WaterCooledFields::WaterDesignFlowRate,true);
  }

  boost::optional<double> RefrigerationCondenserWaterCooled_Impl::waterMaximumFlowRate() const {
    return getDouble(OS_Refrigeration_Condenser_WaterCooledFields::WaterMaximumFlowRate,true);
  }

  double RefrigerationCondenserWaterCooled_Impl::waterMaximumWaterOutletTemperature() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_Condenser_WaterCooledFields::WaterMaximumWaterOutletTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCondenserWaterCooled_Impl::isWaterMaximumWaterOutletTemperatureDefaulted() const {
    return isEmpty(OS_Refrigeration_Condenser_WaterCooledFields::WaterMaximumWaterOutletTemperature);
  }

  double RefrigerationCondenserWaterCooled_Impl::waterMinimumWaterInletTemperature() const {
    boost::optional<double> value = getDouble(OS_Refrigeration_Condenser_WaterCooledFields::WaterMinimumWaterInletTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCondenserWaterCooled_Impl::isWaterMinimumWaterInletTemperatureDefaulted() const {
    return isEmpty(OS_Refrigeration_Condenser_WaterCooledFields::WaterMinimumWaterInletTemperature);
  }

  std::string RefrigerationCondenserWaterCooled_Impl::endUseSubcategory() const {
    boost::optional<std::string> value = getString(OS_Refrigeration_Condenser_WaterCooledFields::EndUseSubcategory,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool RefrigerationCondenserWaterCooled_Impl::isEndUseSubcategoryDefaulted() const {
    return isEmpty(OS_Refrigeration_Condenser_WaterCooledFields::EndUseSubcategory);
  }

  boost::optional<double> RefrigerationCondenserWaterCooled_Impl::condenserRefrigerantOperatingChargeInventory() const {
    return getDouble(OS_Refrigeration_Condenser_WaterCooledFields::CondenserRefrigerantOperatingChargeInventory,true);
  }

  boost::optional<double> RefrigerationCondenserWaterCooled_Impl::condensateReceiverRefrigerantInventory() const {
    return getDouble(OS_Refrigeration_Condenser_WaterCooledFields::CondensateReceiverRefrigerantInventory,true);
  }

  boost::optional<double> RefrigerationCondenserWaterCooled_Impl::condensatePipingRefrigerantInventory() const {
    return getDouble(OS_Refrigeration_Condenser_WaterCooledFields::CondensatePipingRefrigerantInventory,true);
  }

  bool RefrigerationCondenserWaterCooled_Impl::setRatedEffectiveTotalHeatRejectionRate(boost::optional<double> ratedEffectiveTotalHeatRejectionRate) {
    bool result(false);
    if (ratedEffectiveTotalHeatRejectionRate) {
      result = setDouble(OS_Refrigeration_Condenser_WaterCooledFields::RatedEffectiveTotalHeatRejectionRate, ratedEffectiveTotalHeatRejectionRate.get());
    }
    else {
      resetRatedEffectiveTotalHeatRejectionRate();
      result = true;
    }
    return result;
  }

  void RefrigerationCondenserWaterCooled_Impl::resetRatedEffectiveTotalHeatRejectionRate() {
    bool result = setString(OS_Refrigeration_Condenser_WaterCooledFields::RatedEffectiveTotalHeatRejectionRate, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCondenserWaterCooled_Impl::setRatedCondensingTemperature(double ratedCondensingTemperature) {
    bool result = setDouble(OS_Refrigeration_Condenser_WaterCooledFields::RatedCondensingTemperature, ratedCondensingTemperature);
    return result;
  }

  bool RefrigerationCondenserWaterCooled_Impl::setRatedSubcoolingTemperatureDifference(double ratedSubcoolingTemperatureDifference) {
    bool result = setDouble(OS_Refrigeration_Condenser_WaterCooledFields::RatedSubcoolingTemperatureDifference, ratedSubcoolingTemperatureDifference);
    return result;
  }

  void RefrigerationCondenserWaterCooled_Impl::resetRatedSubcoolingTemperatureDifference() {
    bool result = setString(OS_Refrigeration_Condenser_WaterCooledFields::RatedSubcoolingTemperatureDifference, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCondenserWaterCooled_Impl::setRatedWaterInletTemperature(double ratedWaterInletTemperature) {
    bool result = setDouble(OS_Refrigeration_Condenser_WaterCooledFields::RatedWaterInletTemperature, ratedWaterInletTemperature);
    return result;
  }

  bool RefrigerationCondenserWaterCooled_Impl::setWaterCooledLoopFlowType(std::string waterCooledLoopFlowType) {
    bool result = setString(OS_Refrigeration_Condenser_WaterCooledFields::WaterCooledLoopFlowType, waterCooledLoopFlowType);
    return result;
  }

  void RefrigerationCondenserWaterCooled_Impl::resetWaterCooledLoopFlowType() {
    bool result = setString(OS_Refrigeration_Condenser_WaterCooledFields::WaterCooledLoopFlowType, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCondenserWaterCooled_Impl::setWaterOutletTemperatureSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Refrigeration_Condenser_WaterCooledFields::WaterOutletTemperatureScheduleName,
                              "RefrigerationCondenserWaterCooled",
                              "Water Outlet Temperature",
                              schedule);
    return result;
  }

  void RefrigerationCondenserWaterCooled_Impl::resetWaterOutletTemperatureSchedule() {
    bool result = setString(OS_Refrigeration_Condenser_WaterCooledFields::WaterOutletTemperatureScheduleName, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCondenserWaterCooled_Impl::setWaterDesignFlowRate(boost::optional<double> waterDesignFlowRate) {
    bool result(false);
    if (waterDesignFlowRate) {
      result = setDouble(OS_Refrigeration_Condenser_WaterCooledFields::WaterDesignFlowRate, waterDesignFlowRate.get());
    }
    else {
      resetWaterDesignFlowRate();
      result = true;
    }
    return result;
  }

  void RefrigerationCondenserWaterCooled_Impl::resetWaterDesignFlowRate() {
    bool result = setString(OS_Refrigeration_Condenser_WaterCooledFields::WaterDesignFlowRate, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCondenserWaterCooled_Impl::setWaterMaximumFlowRate(boost::optional<double> waterMaximumFlowRate) {
    bool result(false);
    if (waterMaximumFlowRate) {
      result = setDouble(OS_Refrigeration_Condenser_WaterCooledFields::WaterMaximumFlowRate, waterMaximumFlowRate.get());
    }
    else {
      resetWaterMaximumFlowRate();
      result = true;
    }
    return result;
  }

  void RefrigerationCondenserWaterCooled_Impl::resetWaterMaximumFlowRate() {
    bool result = setString(OS_Refrigeration_Condenser_WaterCooledFields::WaterMaximumFlowRate, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCondenserWaterCooled_Impl::setWaterMaximumWaterOutletTemperature(double waterMaximumWaterOutletTemperature) {
    bool result = setDouble(OS_Refrigeration_Condenser_WaterCooledFields::WaterMaximumWaterOutletTemperature, waterMaximumWaterOutletTemperature);
    return result;
  }

  void RefrigerationCondenserWaterCooled_Impl::resetWaterMaximumWaterOutletTemperature() {
    bool result = setString(OS_Refrigeration_Condenser_WaterCooledFields::WaterMaximumWaterOutletTemperature, "");
    OS_ASSERT(result);
  }

  bool RefrigerationCondenserWaterCooled_Impl::setWaterMinimumWaterInletTemperature(double waterMinimumWaterInletTemperature) {
    bool result = setDouble(OS_Refrigeration_Condenser_WaterCooledFields::WaterMinimumWaterInletTemperature, waterMinimumWaterInletTemperature);
    return result;
  }

  void RefrigerationCondenserWaterCooled_Impl::resetWaterMinimumWaterInletTemperature() {
    bool result = setString(OS_Refrigeration_Condenser_WaterCooledFields::WaterMinimumWaterInletTemperature, "");
    OS_ASSERT(result);
  }

  void RefrigerationCondenserWaterCooled_Impl::setEndUseSubcategory(std::string endUseSubcategory) {
    bool result = setString(OS_Refrigeration_Condenser_WaterCooledFields::EndUseSubcategory, endUseSubcategory);
    OS_ASSERT(result);
  }

  void RefrigerationCondenserWaterCooled_Impl::resetEndUseSubcategory() {
    bool result = setString(OS_Refrigeration_Condenser_WaterCooledFields::EndUseSubcategory, "");
    OS_ASSERT(result);
  }

  void RefrigerationCondenserWaterCooled_Impl::setCondenserRefrigerantOperatingChargeInventory(boost::optional<double> condenserRefrigerantOperatingChargeInventory) {
    bool result(false);
    if (condenserRefrigerantOperatingChargeInventory) {
      result = setDouble(OS_Refrigeration_Condenser_WaterCooledFields::CondenserRefrigerantOperatingChargeInventory, condenserRefrigerantOperatingChargeInventory.get());
    }
    else {
      resetCondenserRefrigerantOperatingChargeInventory();
      result = true;
    }
    OS_ASSERT(result);
  }

  void RefrigerationCondenserWaterCooled_Impl::resetCondenserRefrigerantOperatingChargeInventory() {
    bool result = setString(OS_Refrigeration_Condenser_WaterCooledFields::CondenserRefrigerantOperatingChargeInventory, "");
    OS_ASSERT(result);
  }

  void RefrigerationCondenserWaterCooled_Impl::setCondensateReceiverRefrigerantInventory(boost::optional<double> condensateReceiverRefrigerantInventory) {
    bool result(false);
    if (condensateReceiverRefrigerantInventory) {
      result = setDouble(OS_Refrigeration_Condenser_WaterCooledFields::CondensateReceiverRefrigerantInventory, condensateReceiverRefrigerantInventory.get());
    }
    else {
      resetCondensateReceiverRefrigerantInventory();
      result = true;
    }
    OS_ASSERT(result);
  }

  void RefrigerationCondenserWaterCooled_Impl::resetCondensateReceiverRefrigerantInventory() {
    bool result = setString(OS_Refrigeration_Condenser_WaterCooledFields::CondensateReceiverRefrigerantInventory, "");
    OS_ASSERT(result);
  }

  void RefrigerationCondenserWaterCooled_Impl::setCondensatePipingRefrigerantInventory(boost::optional<double> condensatePipingRefrigerantInventory) {
    bool result(false);
    if (condensatePipingRefrigerantInventory) {
      result = setDouble(OS_Refrigeration_Condenser_WaterCooledFields::CondensatePipingRefrigerantInventory, condensatePipingRefrigerantInventory.get());
    }
    else {
      resetCondensatePipingRefrigerantInventory();
      result = true;
    }
    OS_ASSERT(result);
  }

  void RefrigerationCondenserWaterCooled_Impl::resetCondensatePipingRefrigerantInventory() {
    bool result = setString(OS_Refrigeration_Condenser_WaterCooledFields::CondensatePipingRefrigerantInventory, "");
    OS_ASSERT(result);
  }

} // detail

RefrigerationCondenserWaterCooled::RefrigerationCondenserWaterCooled(const Model& model)
  : StraightComponent(RefrigerationCondenserWaterCooled::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::RefrigerationCondenserWaterCooled_Impl>());

  //   58000.,                  !- Rated Effective Total Heat Rejection Rate {W}
  setRatedEffectiveTotalHeatRejectionRate(58000.0);

  //  29.4,                    !- Rated Condensing Temperature {C}
  setRatedCondensingTemperature(29.4);

  //  0.0,                     !- Rated Subcooling Temperature Difference {DeltaC}
  setRatedSubcoolingTemperatureDifference(0.0);

  //  10.0,                    !- Rated Water Inlet Temperature {C}
  setRatedWaterInletTemperature(10.0);

  //  VariableFlow,            !- Water-Cooled Loop Flow Type
  setWaterCooledLoopFlowType("ConstantFlow");

  // Brian provided value for 'ConstantFlow'
  setWaterDesignFlowRate(0.0025);

  //  0.003,                   !- Water Maximum Flow Rate {m3/s}
  setWaterMaximumFlowRate(0.003);

  //  55.,                     !- Water Maximum Water Outlet Temperature {C}
  setWaterMaximumWaterOutletTemperature(55.0);

  //  ,                        !- Water Minimum Water Inlet Temperature {C}
  setWaterMinimumWaterInletTemperature(10.0);
}

IddObjectType RefrigerationCondenserWaterCooled::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Refrigeration_Condenser_WaterCooled);
}

std::vector<std::string> RefrigerationCondenserWaterCooled::waterCooledLoopFlowTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Refrigeration_Condenser_WaterCooledFields::WaterCooledLoopFlowType);
}

boost::optional<double> RefrigerationCondenserWaterCooled::ratedEffectiveTotalHeatRejectionRate() const {
  return getImpl<detail::RefrigerationCondenserWaterCooled_Impl>()->ratedEffectiveTotalHeatRejectionRate();
}

double RefrigerationCondenserWaterCooled::ratedCondensingTemperature() const {
  return getImpl<detail::RefrigerationCondenserWaterCooled_Impl>()->ratedCondensingTemperature();
}

double RefrigerationCondenserWaterCooled::ratedSubcoolingTemperatureDifference() const {
  return getImpl<detail::RefrigerationCondenserWaterCooled_Impl>()->ratedSubcoolingTemperatureDifference();
}

bool RefrigerationCondenserWaterCooled::isRatedSubcoolingTemperatureDifferenceDefaulted() const {
  return getImpl<detail::RefrigerationCondenserWaterCooled_Impl>()->isRatedSubcoolingTemperatureDifferenceDefaulted();
}

double RefrigerationCondenserWaterCooled::ratedWaterInletTemperature() const {
  return getImpl<detail::RefrigerationCondenserWaterCooled_Impl>()->ratedWaterInletTemperature();
}

std::string RefrigerationCondenserWaterCooled::waterCooledLoopFlowType() const {
  return getImpl<detail::RefrigerationCondenserWaterCooled_Impl>()->waterCooledLoopFlowType();
}

bool RefrigerationCondenserWaterCooled::isWaterCooledLoopFlowTypeDefaulted() const {
  return getImpl<detail::RefrigerationCondenserWaterCooled_Impl>()->isWaterCooledLoopFlowTypeDefaulted();
}

boost::optional<Schedule> RefrigerationCondenserWaterCooled::waterOutletTemperatureSchedule() const {
  return getImpl<detail::RefrigerationCondenserWaterCooled_Impl>()->waterOutletTemperatureSchedule();
}

boost::optional<double> RefrigerationCondenserWaterCooled::waterDesignFlowRate() const {
  return getImpl<detail::RefrigerationCondenserWaterCooled_Impl>()->waterDesignFlowRate();
}

boost::optional<double> RefrigerationCondenserWaterCooled::waterMaximumFlowRate() const {
  return getImpl<detail::RefrigerationCondenserWaterCooled_Impl>()->waterMaximumFlowRate();
}

double RefrigerationCondenserWaterCooled::waterMaximumWaterOutletTemperature() const {
  return getImpl<detail::RefrigerationCondenserWaterCooled_Impl>()->waterMaximumWaterOutletTemperature();
}

bool RefrigerationCondenserWaterCooled::isWaterMaximumWaterOutletTemperatureDefaulted() const {
  return getImpl<detail::RefrigerationCondenserWaterCooled_Impl>()->isWaterMaximumWaterOutletTemperatureDefaulted();
}

double RefrigerationCondenserWaterCooled::waterMinimumWaterInletTemperature() const {
  return getImpl<detail::RefrigerationCondenserWaterCooled_Impl>()->waterMinimumWaterInletTemperature();
}

bool RefrigerationCondenserWaterCooled::isWaterMinimumWaterInletTemperatureDefaulted() const {
  return getImpl<detail::RefrigerationCondenserWaterCooled_Impl>()->isWaterMinimumWaterInletTemperatureDefaulted();
}

std::string RefrigerationCondenserWaterCooled::endUseSubcategory() const {
  return getImpl<detail::RefrigerationCondenserWaterCooled_Impl>()->endUseSubcategory();
}

bool RefrigerationCondenserWaterCooled::isEndUseSubcategoryDefaulted() const {
  return getImpl<detail::RefrigerationCondenserWaterCooled_Impl>()->isEndUseSubcategoryDefaulted();
}

boost::optional<double> RefrigerationCondenserWaterCooled::condenserRefrigerantOperatingChargeInventory() const {
  return getImpl<detail::RefrigerationCondenserWaterCooled_Impl>()->condenserRefrigerantOperatingChargeInventory();
}

boost::optional<double> RefrigerationCondenserWaterCooled::condensateReceiverRefrigerantInventory() const {
  return getImpl<detail::RefrigerationCondenserWaterCooled_Impl>()->condensateReceiverRefrigerantInventory();
}

boost::optional<double> RefrigerationCondenserWaterCooled::condensatePipingRefrigerantInventory() const {
  return getImpl<detail::RefrigerationCondenserWaterCooled_Impl>()->condensatePipingRefrigerantInventory();
}

bool RefrigerationCondenserWaterCooled::setRatedEffectiveTotalHeatRejectionRate(double ratedEffectiveTotalHeatRejectionRate) {
  return getImpl<detail::RefrigerationCondenserWaterCooled_Impl>()->setRatedEffectiveTotalHeatRejectionRate(ratedEffectiveTotalHeatRejectionRate);
}

void RefrigerationCondenserWaterCooled::resetRatedEffectiveTotalHeatRejectionRate() {
  getImpl<detail::RefrigerationCondenserWaterCooled_Impl>()->resetRatedEffectiveTotalHeatRejectionRate();
}

bool RefrigerationCondenserWaterCooled::setRatedCondensingTemperature(double ratedCondensingTemperature) {
  return getImpl<detail::RefrigerationCondenserWaterCooled_Impl>()->setRatedCondensingTemperature(ratedCondensingTemperature);
}

bool RefrigerationCondenserWaterCooled::setRatedSubcoolingTemperatureDifference(double ratedSubcoolingTemperatureDifference) {
  return getImpl<detail::RefrigerationCondenserWaterCooled_Impl>()->setRatedSubcoolingTemperatureDifference(ratedSubcoolingTemperatureDifference);
}

void RefrigerationCondenserWaterCooled::resetRatedSubcoolingTemperatureDifference() {
  getImpl<detail::RefrigerationCondenserWaterCooled_Impl>()->resetRatedSubcoolingTemperatureDifference();
}

bool RefrigerationCondenserWaterCooled::setRatedWaterInletTemperature(double ratedWaterInletTemperature) {
  return getImpl<detail::RefrigerationCondenserWaterCooled_Impl>()->setRatedWaterInletTemperature(ratedWaterInletTemperature);
}

bool RefrigerationCondenserWaterCooled::setWaterCooledLoopFlowType(std::string waterCooledLoopFlowType) {
  return getImpl<detail::RefrigerationCondenserWaterCooled_Impl>()->setWaterCooledLoopFlowType(waterCooledLoopFlowType);
}

void RefrigerationCondenserWaterCooled::resetWaterCooledLoopFlowType() {
  getImpl<detail::RefrigerationCondenserWaterCooled_Impl>()->resetWaterCooledLoopFlowType();
}

bool RefrigerationCondenserWaterCooled::setWaterOutletTemperatureSchedule(Schedule& schedule) {
  return getImpl<detail::RefrigerationCondenserWaterCooled_Impl>()->setWaterOutletTemperatureSchedule(schedule);
}

void RefrigerationCondenserWaterCooled::resetWaterOutletTemperatureSchedule() {
  getImpl<detail::RefrigerationCondenserWaterCooled_Impl>()->resetWaterOutletTemperatureSchedule();
}

bool RefrigerationCondenserWaterCooled::setWaterDesignFlowRate(double waterDesignFlowRate) {
  return getImpl<detail::RefrigerationCondenserWaterCooled_Impl>()->setWaterDesignFlowRate(waterDesignFlowRate);
}

void RefrigerationCondenserWaterCooled::resetWaterDesignFlowRate() {
  getImpl<detail::RefrigerationCondenserWaterCooled_Impl>()->resetWaterDesignFlowRate();
}

bool RefrigerationCondenserWaterCooled::setWaterMaximumFlowRate(double waterMaximumFlowRate) {
  return getImpl<detail::RefrigerationCondenserWaterCooled_Impl>()->setWaterMaximumFlowRate(waterMaximumFlowRate);
}

void RefrigerationCondenserWaterCooled::resetWaterMaximumFlowRate() {
  getImpl<detail::RefrigerationCondenserWaterCooled_Impl>()->resetWaterMaximumFlowRate();
}

bool RefrigerationCondenserWaterCooled::setWaterMaximumWaterOutletTemperature(double waterMaximumWaterOutletTemperature) {
  return getImpl<detail::RefrigerationCondenserWaterCooled_Impl>()->setWaterMaximumWaterOutletTemperature(waterMaximumWaterOutletTemperature);
}

void RefrigerationCondenserWaterCooled::resetWaterMaximumWaterOutletTemperature() {
  getImpl<detail::RefrigerationCondenserWaterCooled_Impl>()->resetWaterMaximumWaterOutletTemperature();
}

bool RefrigerationCondenserWaterCooled::setWaterMinimumWaterInletTemperature(double waterMinimumWaterInletTemperature) {
  return getImpl<detail::RefrigerationCondenserWaterCooled_Impl>()->setWaterMinimumWaterInletTemperature(waterMinimumWaterInletTemperature);
}

void RefrigerationCondenserWaterCooled::resetWaterMinimumWaterInletTemperature() {
  getImpl<detail::RefrigerationCondenserWaterCooled_Impl>()->resetWaterMinimumWaterInletTemperature();
}

void RefrigerationCondenserWaterCooled::setEndUseSubcategory(std::string endUseSubcategory) {
  getImpl<detail::RefrigerationCondenserWaterCooled_Impl>()->setEndUseSubcategory(endUseSubcategory);
}

void RefrigerationCondenserWaterCooled::resetEndUseSubcategory() {
  getImpl<detail::RefrigerationCondenserWaterCooled_Impl>()->resetEndUseSubcategory();
}

void RefrigerationCondenserWaterCooled::setCondenserRefrigerantOperatingChargeInventory(double condenserRefrigerantOperatingChargeInventory) {
  getImpl<detail::RefrigerationCondenserWaterCooled_Impl>()->setCondenserRefrigerantOperatingChargeInventory(condenserRefrigerantOperatingChargeInventory);
}

void RefrigerationCondenserWaterCooled::resetCondenserRefrigerantOperatingChargeInventory() {
  getImpl<detail::RefrigerationCondenserWaterCooled_Impl>()->resetCondenserRefrigerantOperatingChargeInventory();
}

void RefrigerationCondenserWaterCooled::setCondensateReceiverRefrigerantInventory(double condensateReceiverRefrigerantInventory) {
  getImpl<detail::RefrigerationCondenserWaterCooled_Impl>()->setCondensateReceiverRefrigerantInventory(condensateReceiverRefrigerantInventory);
}

void RefrigerationCondenserWaterCooled::resetCondensateReceiverRefrigerantInventory() {
  getImpl<detail::RefrigerationCondenserWaterCooled_Impl>()->resetCondensateReceiverRefrigerantInventory();
}

void RefrigerationCondenserWaterCooled::setCondensatePipingRefrigerantInventory(double condensatePipingRefrigerantInventory) {
  getImpl<detail::RefrigerationCondenserWaterCooled_Impl>()->setCondensatePipingRefrigerantInventory(condensatePipingRefrigerantInventory);
}

void RefrigerationCondenserWaterCooled::resetCondensatePipingRefrigerantInventory() {
  getImpl<detail::RefrigerationCondenserWaterCooled_Impl>()->resetCondensatePipingRefrigerantInventory();
}

/// @cond
RefrigerationCondenserWaterCooled::RefrigerationCondenserWaterCooled(std::shared_ptr<detail::RefrigerationCondenserWaterCooled_Impl> impl)
  : StraightComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

