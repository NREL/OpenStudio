/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "AirflowNetworkDetailedOpening.hpp"
#include "AirflowNetworkDetailedOpening_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_AirflowNetworkDetailedOpening_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"
#include "ModelExtensibleGroup.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  DetailedOpeningFactorData::DetailedOpeningFactorData(double openingFactor, double dischargeCoefficient, double widthFactor, double heightFactor,
                                                       double startHeightFactor)
    : m_openingFactor(openingFactor),
      m_dischargeCoefficient(dischargeCoefficient),
      m_widthFactor(widthFactor),
      m_heightFactor(heightFactor),
      m_startHeightFactor(startHeightFactor) {}

  double DetailedOpeningFactorData::openingFactor() const {
    return m_openingFactor;
  }

  double DetailedOpeningFactorData::dischargeCoefficient() const {
    return m_dischargeCoefficient;
  }

  double DetailedOpeningFactorData::widthFactor() const {
    return m_widthFactor;
  };

  double DetailedOpeningFactorData::heightFactor() const {
    return m_heightFactor;
  }

  double DetailedOpeningFactorData::startHeightFactor() const {
    return m_startHeightFactor;
  }

  bool DetailedOpeningFactorData::setOpeningFactor(double openingFactor) {
    if (openingFactor >= 0.0 && openingFactor <= 1.0) {
      m_openingFactor = openingFactor;
      return true;
    }
    LOG_FREE(Warn, "openstudio.model.AirflowNetworkDetailedOpening",
             "Opening factor must be greater than or equal to 0 and less than or equal to 1, value remains " << m_openingFactor);
    return false;
  }

  bool DetailedOpeningFactorData::setDischargeCoefficient(double dischargeCoefficient) {
    if (dischargeCoefficient > 0.0 && dischargeCoefficient <= 1.0) {
      m_dischargeCoefficient = dischargeCoefficient;
      return true;
    }
    LOG_FREE(Warn, "openstudio.model.AirflowNetworkDetailedOpening",
             "Discharge coefficient must be greater than 0 and less than or equal to 1, value remains " << m_dischargeCoefficient);
    return false;
  }

  bool DetailedOpeningFactorData::setWidthFactor(double widthFactor) {
    if (widthFactor >= 0.0 && widthFactor <= 1.0) {
      m_widthFactor = widthFactor;
      return true;
    }
    LOG_FREE(Warn, "openstudio.model.AirflowNetworkDetailedOpening",
             "Width factor must be greater than or equal to 0 and less than or equal to 1, value remains " << m_widthFactor);
    return false;
  }

  bool DetailedOpeningFactorData::setHeightFactor(double heightFactor) {
    if (heightFactor >= 0.0 && heightFactor <= 1.0) {
      m_heightFactor = heightFactor;
      return true;
    }
    LOG_FREE(Warn, "openstudio.model.AirflowNetworkDetailedOpening",
             "Height factor must be greater than or equal to 0 and less than or equal to 1, value remains " << m_heightFactor);
    return false;
  }

  bool DetailedOpeningFactorData::setStartHeightFactor(double startHeightFactor) {
    if (startHeightFactor >= 0.0 && startHeightFactor <= 1.0) {
      m_startHeightFactor = startHeightFactor;
      return true;
    }
    LOG_FREE(Warn, "openstudio.model.AirflowNetworkDetailedOpening",
             "Start height factor must be greater than or equal to 0 and less than or equal to 1, value remains " << m_startHeightFactor);
    return false;
  }

  namespace detail {

    AirflowNetworkDetailedOpening_Impl::AirflowNetworkDetailedOpening_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : AirflowNetworkComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AirflowNetworkDetailedOpening::iddObjectType());
    }

    AirflowNetworkDetailedOpening_Impl::AirflowNetworkDetailedOpening_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                           bool keepHandle)
      : AirflowNetworkComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AirflowNetworkDetailedOpening::iddObjectType());
    }

    AirflowNetworkDetailedOpening_Impl::AirflowNetworkDetailedOpening_Impl(const AirflowNetworkDetailedOpening_Impl& other, Model_Impl* model,
                                                                           bool keepHandle)
      : AirflowNetworkComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& AirflowNetworkDetailedOpening_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType AirflowNetworkDetailedOpening_Impl::iddObjectType() const {
      return AirflowNetworkDetailedOpening::iddObjectType();
    }

    double AirflowNetworkDetailedOpening_Impl::airMassFlowCoefficientWhenOpeningisClosed() const {
      boost::optional<double> value = getDouble(OS_AirflowNetworkDetailedOpeningFields::AirMassFlowCoefficientWhenOpeningisClosed, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirflowNetworkDetailedOpening_Impl::airMassFlowExponentWhenOpeningisClosed() const {
      boost::optional<double> value = getDouble(OS_AirflowNetworkDetailedOpeningFields::AirMassFlowExponentWhenOpeningisClosed, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirflowNetworkDetailedOpening_Impl::isAirMassFlowExponentWhenOpeningisClosedDefaulted() const {
      return isEmpty(OS_AirflowNetworkDetailedOpeningFields::AirMassFlowExponentWhenOpeningisClosed);
    }

    std::string AirflowNetworkDetailedOpening_Impl::typeofRectangularLargeVerticalOpening() const {
      boost::optional<std::string> value = getString(OS_AirflowNetworkDetailedOpeningFields::TypeofRectangularLargeVerticalOpening, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirflowNetworkDetailedOpening_Impl::isTypeofRectangularLargeVerticalOpeningDefaulted() const {
      return isEmpty(OS_AirflowNetworkDetailedOpeningFields::TypeofRectangularLargeVerticalOpening);
    }

    double AirflowNetworkDetailedOpening_Impl::extraCrackLengthorHeightofPivotingAxis() const {
      boost::optional<double> value = getDouble(OS_AirflowNetworkDetailedOpeningFields::ExtraCrackLengthorHeightofPivotingAxis, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirflowNetworkDetailedOpening_Impl::isExtraCrackLengthorHeightofPivotingAxisDefaulted() const {
      return isEmpty(OS_AirflowNetworkDetailedOpeningFields::ExtraCrackLengthorHeightofPivotingAxis);
    }

    std::vector<DetailedOpeningFactorData> AirflowNetworkDetailedOpening_Impl::openingFactors() const {
      std::vector<DetailedOpeningFactorData> results;
      for (const auto& group : extensibleGroups()) {
        OptionalDouble openingFactor = group.getDouble(0);
        OptionalDouble dischargeCoefficient = group.getDouble(1);
        OptionalDouble widthFactor = group.getDouble(2);
        OptionalDouble heightFactor = group.getDouble(3);
        OptionalDouble startHeightFactor = group.getDouble(4);

        if (openingFactor && dischargeCoefficient && widthFactor && heightFactor && startHeightFactor) {
          results.push_back(DetailedOpeningFactorData(*openingFactor, *dischargeCoefficient, *widthFactor, *heightFactor, *startHeightFactor));
        } else {
          LOG(Error, "Could not read opening factor " << group.groupIndex() << " in " << briefDescription() << ".");
        }
      }
      return results;
    }

    bool AirflowNetworkDetailedOpening_Impl::setAirMassFlowCoefficientWhenOpeningisClosed(double airMassFlowCoefficientWhenOpeningisClosed) {
      bool result =
        setDouble(OS_AirflowNetworkDetailedOpeningFields::AirMassFlowCoefficientWhenOpeningisClosed, airMassFlowCoefficientWhenOpeningisClosed);
      return result;
    }

    bool AirflowNetworkDetailedOpening_Impl::setAirMassFlowExponentWhenOpeningisClosed(double airMassFlowExponentWhenOpeningisClosed) {
      bool result = setDouble(OS_AirflowNetworkDetailedOpeningFields::AirMassFlowExponentWhenOpeningisClosed, airMassFlowExponentWhenOpeningisClosed);
      return result;
    }

    void AirflowNetworkDetailedOpening_Impl::resetAirMassFlowExponentWhenOpeningisClosed() {
      bool result = setString(OS_AirflowNetworkDetailedOpeningFields::AirMassFlowExponentWhenOpeningisClosed, "");
      OS_ASSERT(result);
    }

    bool AirflowNetworkDetailedOpening_Impl::setTypeofRectangularLargeVerticalOpening(const std::string& typeofRectangularLargeVerticalOpening) {
      bool result = setString(OS_AirflowNetworkDetailedOpeningFields::TypeofRectangularLargeVerticalOpening, typeofRectangularLargeVerticalOpening);
      return result;
    }

    void AirflowNetworkDetailedOpening_Impl::resetTypeofRectangularLargeVerticalOpening() {
      bool result = setString(OS_AirflowNetworkDetailedOpeningFields::TypeofRectangularLargeVerticalOpening, "");
      OS_ASSERT(result);
    }

    bool AirflowNetworkDetailedOpening_Impl::setExtraCrackLengthorHeightofPivotingAxis(double extraCrackLengthorHeightofPivotingAxis) {
      bool result = setDouble(OS_AirflowNetworkDetailedOpeningFields::ExtraCrackLengthorHeightofPivotingAxis, extraCrackLengthorHeightofPivotingAxis);
      return result;
    }

    void AirflowNetworkDetailedOpening_Impl::resetExtraCrackLengthorHeightofPivotingAxis() {
      bool result = setString(OS_AirflowNetworkDetailedOpeningFields::ExtraCrackLengthorHeightofPivotingAxis, "");
      OS_ASSERT(result);
    }

    bool AirflowNetworkDetailedOpening_Impl::setOpeningFactors(const std::vector<DetailedOpeningFactorData>& factors) {
      // Number of Sets of Opening Factor Data
      if (factors.size() < 2) {
        LOG(Error, "Insufficient data in opening factors vector, minimum number of factors is 2");
        return false;
      } else if (factors.size() > 4) {
        LOG(Error, "Additional data in opening factors vector, maximum number of factors is 4");
        return false;
      }

      // Opening Factor
      if (factors[0].openingFactor() != 0.0) {
        LOG(Error, "First opening factor must be 0");
        return false;
      }
      if (factors.back().openingFactor() != 1.0) {
        LOG(Error, "Last opening factor must be 1");
        return false;
      }

      clearExtensibleGroups(false);
      for (const auto& factor : factors) {
        // Height Factor and Start Height Factor
        if (factor.heightFactor() + factor.startHeightFactor() > 1.0) {
          LOG(Error, "The sum of the Height Factor and the Start Height Factor cannot be more than 1");
          return false;
        }
        std::vector<std::string> values{
          toString(factor.openingFactor()), toString(factor.dischargeCoefficient()), toString(factor.widthFactor()),
          toString(factor.heightFactor()),  toString(factor.startHeightFactor()),
        };
        auto group = pushExtensibleGroup(values, false);
        OS_ASSERT(!group.empty());
      }
      return true;
    }

  }  // namespace detail

  AirflowNetworkDetailedOpening::AirflowNetworkDetailedOpening(const Model& model, double massFlowCoefficientWhenOpeningisClosed,
                                                               double massFlowExponentWhenOpeningisClosed,
                                                               const std::string& typeofRectangularLargeVerticalOpening,
                                                               double extraCrackLengthorHeightofPivotingAxis,
                                                               const std::vector<DetailedOpeningFactorData>& openingFactors)
    : AirflowNetworkComponent(AirflowNetworkDetailedOpening::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirflowNetworkDetailedOpening_Impl>());

    bool ok = setAirMassFlowCoefficientWhenOpeningisClosed(massFlowCoefficientWhenOpeningisClosed);
    OS_ASSERT(ok);
    ok = setAirMassFlowExponentWhenOpeningisClosed(massFlowExponentWhenOpeningisClosed);
    OS_ASSERT(ok);
    ok = setTypeofRectangularLargeVerticalOpening(typeofRectangularLargeVerticalOpening);
    OS_ASSERT(ok);
    ok = setExtraCrackLengthorHeightofPivotingAxis(extraCrackLengthorHeightofPivotingAxis);
    OS_ASSERT(ok);
    ok = setOpeningFactors(openingFactors);
    OS_ASSERT(ok);
  }

  AirflowNetworkDetailedOpening::AirflowNetworkDetailedOpening(const Model& model, double massFlowCoefficientWhenOpeningisClosed,
                                                               const std::vector<DetailedOpeningFactorData>& openingFactors)
    : AirflowNetworkComponent(AirflowNetworkDetailedOpening::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirflowNetworkDetailedOpening_Impl>());

    bool ok = setAirMassFlowCoefficientWhenOpeningisClosed(massFlowCoefficientWhenOpeningisClosed);
    OS_ASSERT(ok);
    ok = setOpeningFactors(openingFactors);
    OS_ASSERT(ok);
  }

  IddObjectType AirflowNetworkDetailedOpening::iddObjectType() {
    return {IddObjectType::OS_AirflowNetworkDetailedOpening};
  }

  std::vector<std::string> AirflowNetworkDetailedOpening::typeofRectangularLargeVerticalOpeningValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_AirflowNetworkDetailedOpeningFields::TypeofRectangularLargeVerticalOpening);
  }

  double AirflowNetworkDetailedOpening::airMassFlowCoefficientWhenOpeningisClosed() const {
    return getImpl<detail::AirflowNetworkDetailedOpening_Impl>()->airMassFlowCoefficientWhenOpeningisClosed();
  }

  double AirflowNetworkDetailedOpening::airMassFlowExponentWhenOpeningisClosed() const {
    return getImpl<detail::AirflowNetworkDetailedOpening_Impl>()->airMassFlowExponentWhenOpeningisClosed();
  }

  bool AirflowNetworkDetailedOpening::isAirMassFlowExponentWhenOpeningisClosedDefaulted() const {
    return getImpl<detail::AirflowNetworkDetailedOpening_Impl>()->isAirMassFlowExponentWhenOpeningisClosedDefaulted();
  }

  std::string AirflowNetworkDetailedOpening::typeofRectangularLargeVerticalOpening() const {
    return getImpl<detail::AirflowNetworkDetailedOpening_Impl>()->typeofRectangularLargeVerticalOpening();
  }

  bool AirflowNetworkDetailedOpening::isTypeofRectangularLargeVerticalOpeningDefaulted() const {
    return getImpl<detail::AirflowNetworkDetailedOpening_Impl>()->isTypeofRectangularLargeVerticalOpeningDefaulted();
  }

  double AirflowNetworkDetailedOpening::extraCrackLengthorHeightofPivotingAxis() const {
    return getImpl<detail::AirflowNetworkDetailedOpening_Impl>()->extraCrackLengthorHeightofPivotingAxis();
  }

  bool AirflowNetworkDetailedOpening::isExtraCrackLengthorHeightofPivotingAxisDefaulted() const {
    return getImpl<detail::AirflowNetworkDetailedOpening_Impl>()->isExtraCrackLengthorHeightofPivotingAxisDefaulted();
  }

  std::vector<DetailedOpeningFactorData> AirflowNetworkDetailedOpening::openingFactors() const {
    return getImpl<detail::AirflowNetworkDetailedOpening_Impl>()->openingFactors();
  }

  bool AirflowNetworkDetailedOpening::setAirMassFlowCoefficientWhenOpeningisClosed(double airMassFlowCoefficientWhenOpeningisClosed) {
    return getImpl<detail::AirflowNetworkDetailedOpening_Impl>()->setAirMassFlowCoefficientWhenOpeningisClosed(
      airMassFlowCoefficientWhenOpeningisClosed);
  }

  bool AirflowNetworkDetailedOpening::setAirMassFlowExponentWhenOpeningisClosed(double airMassFlowExponentWhenOpeningisClosed) {
    return getImpl<detail::AirflowNetworkDetailedOpening_Impl>()->setAirMassFlowExponentWhenOpeningisClosed(airMassFlowExponentWhenOpeningisClosed);
  }

  void AirflowNetworkDetailedOpening::resetAirMassFlowExponentWhenOpeningisClosed() {
    getImpl<detail::AirflowNetworkDetailedOpening_Impl>()->resetAirMassFlowExponentWhenOpeningisClosed();
  }

  bool AirflowNetworkDetailedOpening::setTypeofRectangularLargeVerticalOpening(const std::string& typeofRectangularLargeVerticalOpening) {
    return getImpl<detail::AirflowNetworkDetailedOpening_Impl>()->setTypeofRectangularLargeVerticalOpening(typeofRectangularLargeVerticalOpening);
  }

  void AirflowNetworkDetailedOpening::resetTypeofRectangularLargeVerticalOpening() {
    getImpl<detail::AirflowNetworkDetailedOpening_Impl>()->resetTypeofRectangularLargeVerticalOpening();
  }

  bool AirflowNetworkDetailedOpening::setExtraCrackLengthorHeightofPivotingAxis(double extraCrackLengthorHeightofPivotingAxis) {
    return getImpl<detail::AirflowNetworkDetailedOpening_Impl>()->setExtraCrackLengthorHeightofPivotingAxis(extraCrackLengthorHeightofPivotingAxis);
  }

  void AirflowNetworkDetailedOpening::resetExtraCrackLengthorHeightofPivotingAxis() {
    getImpl<detail::AirflowNetworkDetailedOpening_Impl>()->resetExtraCrackLengthorHeightofPivotingAxis();
  }

  bool AirflowNetworkDetailedOpening::setOpeningFactors(const std::vector<DetailedOpeningFactorData>& factors) {
    return getImpl<detail::AirflowNetworkDetailedOpening_Impl>()->setOpeningFactors(factors);
  }

  /// @cond
  AirflowNetworkDetailedOpening::AirflowNetworkDetailedOpening(std::shared_ptr<detail::AirflowNetworkDetailedOpening_Impl> impl)
    : AirflowNetworkComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
