/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "UnitarySystemPerformanceMultispeed.hpp"
#include "UnitarySystemPerformanceMultispeed_Impl.hpp"
#include "ModelExtensibleGroup.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_UnitarySystemPerformance_Multispeed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    UnitarySystemPerformanceMultispeed_Impl::UnitarySystemPerformanceMultispeed_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == UnitarySystemPerformanceMultispeed::iddObjectType());
    }

    UnitarySystemPerformanceMultispeed_Impl::UnitarySystemPerformanceMultispeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                     Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == UnitarySystemPerformanceMultispeed::iddObjectType());
    }

    UnitarySystemPerformanceMultispeed_Impl::UnitarySystemPerformanceMultispeed_Impl(const UnitarySystemPerformanceMultispeed_Impl& other,
                                                                                     Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& UnitarySystemPerformanceMultispeed_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType UnitarySystemPerformanceMultispeed_Impl::iddObjectType() const {
      return UnitarySystemPerformanceMultispeed::iddObjectType();
    }

    bool UnitarySystemPerformanceMultispeed_Impl::singleModeOperation() const {
      boost::optional<std::string> value = getString(OS_UnitarySystemPerformance_MultispeedFields::SingleModeOperation, true);
      OS_ASSERT(value);
      return openstudio::istringEqual("Yes", value.get());
    }

    bool UnitarySystemPerformanceMultispeed_Impl::setSingleModeOperation(bool singleMode) {
      return setString(OS_UnitarySystemPerformance_MultispeedFields::SingleModeOperation, singleMode ? "Yes" : "No");
    }

    void UnitarySystemPerformanceMultispeed_Impl::resetSingleModeOperation() {
      OS_ASSERT(setString(OS_UnitarySystemPerformance_MultispeedFields::SingleModeOperation, ""));
    }

    double UnitarySystemPerformanceMultispeed_Impl::noLoadSupplyAirflowRateRatio() const {
      boost::optional<double> value = getDouble(OS_UnitarySystemPerformance_MultispeedFields::NoLoadSupplyAirFlowRateRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool UnitarySystemPerformanceMultispeed_Impl::setNoLoadSupplyAirflowRateRatio(double noLoadSupplyAirflowRateRatio) {
      bool result = setDouble(OS_UnitarySystemPerformance_MultispeedFields::NoLoadSupplyAirFlowRateRatio, noLoadSupplyAirflowRateRatio);
      return result;
    }

    boost::optional<double> UnitarySystemPerformanceMultispeed_Impl::readDoubleFieldOrAutosize(unsigned fieldNum,
                                                                                               const ModelExtensibleGroup& group) const {
      boost::optional<double> result = group.getDouble(fieldNum);
      if (!result) {
        boost::optional<std::string> resultString = group.getString(fieldNum);
        if (!resultString || !openstudio::istringEqual(resultString.get(), "autosize")) {
          LOG(Error, "Could not read SupplyAirflowRatioField " << group.groupIndex() << " in " << briefDescription() << ".");
        }
      }
      return result;
    }

    std::vector<SupplyAirflowRatioField> UnitarySystemPerformanceMultispeed_Impl::supplyAirflowRatioFields() const {
      std::vector<SupplyAirflowRatioField> result;

      for (const ModelExtensibleGroup& group : castVector<ModelExtensibleGroup>(extensibleGroups())) {
        boost::optional<double> heatingRatio =
          readDoubleFieldOrAutosize(OS_UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio, group);
        boost::optional<double> coolingRatio =
          readDoubleFieldOrAutosize(OS_UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio, group);
        if (heatingRatio && coolingRatio) {
          result.push_back(SupplyAirflowRatioField(*heatingRatio, *coolingRatio));
        } else if (heatingRatio && !coolingRatio) {
          result.push_back(SupplyAirflowRatioField::fromHeatingRatio(*heatingRatio));
        } else if (coolingRatio && !heatingRatio) {
          result.push_back(SupplyAirflowRatioField::fromCoolingRatio(*coolingRatio));
        } else {
          result.push_back(SupplyAirflowRatioField());
        }
      }
      return result;
    }

    bool UnitarySystemPerformanceMultispeed_Impl::setSupplyAirflowRatioFields(const std::vector<SupplyAirflowRatioField>& airflowRatioFields) {
      std::vector<SupplyAirflowRatioField> originalFields = supplyAirflowRatioFields();

      bool result = true;

      clearExtensibleGroups(false);
      for (const auto& airflowRatioField : airflowRatioFields) {
        if (!(result && addSupplyAirflowRatioField(airflowRatioField))) {
          result = false;
        }
      }

      if (!result) {
        clearExtensibleGroups(false);
        for (const auto& airflowRatioField : originalFields) {
          addSupplyAirflowRatioField(airflowRatioField);
        }
      }

      return result;
    }

    bool UnitarySystemPerformanceMultispeed_Impl::addSupplyAirflowRatioField(const SupplyAirflowRatioField& airflowRatio) {
      std::vector<std::string> values = airflowRatio.getHeatingCoolingRatiosAsStrings();
      auto group = pushExtensibleGroup(values, false).cast<ModelExtensibleGroup>();
      return (!group.empty());
    }

    bool UnitarySystemPerformanceMultispeed_Impl::addSupplyAirflowRatioField(double heatingRatio, double coolingRatio) {
      return addSupplyAirflowRatioField(SupplyAirflowRatioField(heatingRatio, coolingRatio));
    }

    void UnitarySystemPerformanceMultispeed_Impl::resetSupplyAirflowRatioFields() {
      clearExtensibleGroups(false);
    }

  }  // namespace detail

  SupplyAirflowRatioField::SupplyAirflowRatioField() : m_heatingRatio(boost::none), m_coolingRatio(boost::none) {}

  SupplyAirflowRatioField::SupplyAirflowRatioField(double heatingRatio, double coolingRatio)
    : m_heatingRatio(heatingRatio), m_coolingRatio(coolingRatio) {}

  SupplyAirflowRatioField::SupplyAirflowRatioField(bool isHeating, double value) : m_heatingRatio(boost::none), m_coolingRatio(boost::none) {
    if (isHeating) {
      m_heatingRatio = value;
    } else {
      m_coolingRatio = value;
    }
  }

  SupplyAirflowRatioField SupplyAirflowRatioField::fromHeatingRatio(double heatingRatio) {
    return {true, heatingRatio};
  }

  SupplyAirflowRatioField SupplyAirflowRatioField::fromCoolingRatio(double coolingRatio) {
    return {false, coolingRatio};
  }

  boost::optional<double> SupplyAirflowRatioField::heatingRatio() const {
    return m_heatingRatio;
  }

  boost::optional<double> SupplyAirflowRatioField::coolingRatio() const {
    return m_coolingRatio;
  }

  bool SupplyAirflowRatioField::isHeatingRatioAutosized() const {
    return !m_heatingRatio;
  }

  bool SupplyAirflowRatioField::isCoolingRatioAutosized() const {
    return !m_coolingRatio;
  }

  std::vector<std::string> SupplyAirflowRatioField::getHeatingCoolingRatiosAsStrings() const {
    std::vector<std::string> values;
    for (const auto& item : {m_heatingRatio, m_coolingRatio}) {
      if (item) {
        values.push_back(openstudio::toString(*item));
      } else {
        values.push_back("autosize");
      }
    }
    return values;
  }

  UnitarySystemPerformanceMultispeed::UnitarySystemPerformanceMultispeed(const Model& model)
    : ModelObject(UnitarySystemPerformanceMultispeed::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::UnitarySystemPerformanceMultispeed_Impl>());

    bool ok = setNoLoadSupplyAirflowRateRatio(1.0);
    OS_ASSERT(ok);
  }

  IddObjectType UnitarySystemPerformanceMultispeed::iddObjectType() {
    return {IddObjectType::OS_UnitarySystemPerformance_Multispeed};
  }

  bool UnitarySystemPerformanceMultispeed::singleModeOperation() const {
    return getImpl<detail::UnitarySystemPerformanceMultispeed_Impl>()->singleModeOperation();
  }

  bool UnitarySystemPerformanceMultispeed::setSingleModeOperation(bool singleMode) {
    return getImpl<detail::UnitarySystemPerformanceMultispeed_Impl>()->setSingleModeOperation(singleMode);
  }

  void UnitarySystemPerformanceMultispeed::resetSingleModeOperation() {
    getImpl<detail::UnitarySystemPerformanceMultispeed_Impl>()->resetSingleModeOperation();
  }

  double UnitarySystemPerformanceMultispeed::noLoadSupplyAirflowRateRatio() const {
    return getImpl<detail::UnitarySystemPerformanceMultispeed_Impl>()->noLoadSupplyAirflowRateRatio();
  }

  bool UnitarySystemPerformanceMultispeed::setNoLoadSupplyAirflowRateRatio(double noLoadSupplyAirflowRateRatio) {
    return getImpl<detail::UnitarySystemPerformanceMultispeed_Impl>()->setNoLoadSupplyAirflowRateRatio(noLoadSupplyAirflowRateRatio);
  }

  std::vector<SupplyAirflowRatioField> UnitarySystemPerformanceMultispeed::supplyAirflowRatioFields() const {
    return getImpl<detail::UnitarySystemPerformanceMultispeed_Impl>()->supplyAirflowRatioFields();
  }

  bool UnitarySystemPerformanceMultispeed::setSupplyAirflowRatioFields(const std::vector<SupplyAirflowRatioField>& airflowRatioFields) {
    return getImpl<detail::UnitarySystemPerformanceMultispeed_Impl>()->setSupplyAirflowRatioFields(airflowRatioFields);
  }

  bool UnitarySystemPerformanceMultispeed::addSupplyAirflowRatioField(const SupplyAirflowRatioField& airflowRatio) {
    return getImpl<detail::UnitarySystemPerformanceMultispeed_Impl>()->addSupplyAirflowRatioField(airflowRatio);
  }

  bool UnitarySystemPerformanceMultispeed::addSupplyAirflowRatioField(double heatingRatio, double coolingRatio) {
    return getImpl<detail::UnitarySystemPerformanceMultispeed_Impl>()->addSupplyAirflowRatioField(heatingRatio, coolingRatio);
  }

  void UnitarySystemPerformanceMultispeed::resetSupplyAirflowRatioFields() {
    getImpl<detail::UnitarySystemPerformanceMultispeed_Impl>()->resetSupplyAirflowRatioFields();
  }

  /// @cond
  UnitarySystemPerformanceMultispeed::UnitarySystemPerformanceMultispeed(std::shared_ptr<detail::UnitarySystemPerformanceMultispeed_Impl> impl)
    : ModelObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
