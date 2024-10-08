/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable : 4996)
#elif (defined(__GNUC__))
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

#include "TableMultiVariableLookup.hpp"
#include "TableMultiVariableLookup_Impl.hpp"

#include "../utilities/idf/IdfExtensibleGroup.hpp"
#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Table_MultiVariableLookup_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/math/FloatCompare.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/DeprecatedHelpers.hpp"

#include <algorithm>
#include <iomanip>

namespace openstudio {
namespace model {

  TableMultiVariableLookupPoint::TableMultiVariableLookupPoint(const std::vector<double>& x, double y) : m_x(x), m_y(y){};

  TableMultiVariableLookupPoint::TableMultiVariableLookupPoint(double x1, double yValue) : m_x(std::vector<double>{x1}), m_y(yValue){};

  TableMultiVariableLookupPoint::TableMultiVariableLookupPoint(double x1, double x2, double yValue) : m_x(std::vector<double>{x1, x2}), m_y(yValue){};
  TableMultiVariableLookupPoint::TableMultiVariableLookupPoint(double x1, double x2, double x3, double yValue)
    : m_x(std::vector<double>{x1, x2, x3}), m_y(yValue){};
  TableMultiVariableLookupPoint::TableMultiVariableLookupPoint(double x1, double x2, double x3, double x4, double yValue)
    : m_x(std::vector<double>{x1, x2, x3, x4}), m_y(yValue){};
  TableMultiVariableLookupPoint::TableMultiVariableLookupPoint(double x1, double x2, double x3, double x4, double x5, double yValue)
    : m_x(std::vector<double>{x1, x2, x3, x4, x5}), m_y(yValue){};

  std::vector<double> TableMultiVariableLookupPoint::x() const {
    return m_x;
  }

  double TableMultiVariableLookupPoint::y() const {
    return m_y;
  }

  bool TableMultiVariableLookupPoint::operator<(const TableMultiVariableLookupPoint& other) const {
    auto n = m_x.size();
    auto other_x = other.x();
    if (n != other_x.size()) {
      LOG_FREE_AND_THROW("TableMultiVariableLookupPoint", "Cannot compare points of different size " << n << " and " << other_x.size());
    }

    for (size_t i = 0; i < n; ++i) {
      if (m_x[i] < other_x[i]) {
        return true;
      } else if (m_x[i] > other_x[i]) {
        return false;
      }
    }

    return false;
  }

  std::ostream& operator<<(std::ostream& out, const openstudio::model::TableMultiVariableLookupPoint& point) {
    std::vector<double> xValues = point.x();
    std::stringstream ss_left;
    std::stringstream ss_right;
    int i = 1;
    ss_left << "(";
    ss_right << "(";

    for (const double& x : xValues) {
      ss_left << "x" << i << ", ";
      ss_right << x << ", ";
      ++i;
    }

    ss_left << "y)";
    ss_right << point.y() << ")";

    out << ss_left.str() << " = " << ss_right.str();
    return out;
  }

  namespace detail {

    bool TableMultiVariableLookup_Impl::xValuesEqual(const std::vector<double>& a, const std::vector<double>& b) {
      bool result = true;

      if (a.size() != b.size()) {
        result = false;
      } else {
        for (auto it1 = a.begin(), it2 = b.begin(); it1 != a.end() && it2 != b.end(); ++it1, ++it2) {
          if (!equal(*it1, *it2)) {
            result = false;
          }
        }
      }

      return result;
    }

    TableMultiVariableLookup_Impl::TableMultiVariableLookup_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : Curve_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == TableMultiVariableLookup::iddObjectType());
    }

    TableMultiVariableLookup_Impl::TableMultiVariableLookup_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                 bool keepHandle)
      : Curve_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == TableMultiVariableLookup::iddObjectType());
    }

    TableMultiVariableLookup_Impl::TableMultiVariableLookup_Impl(const TableMultiVariableLookup_Impl& other, Model_Impl* model, bool keepHandle)
      : Curve_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& TableMultiVariableLookup_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType TableMultiVariableLookup_Impl::iddObjectType() const {
      return TableMultiVariableLookup::iddObjectType();
    }

    std::string TableMultiVariableLookup_Impl::interpolationMethod() const {
      boost::optional<std::string> value = getString(OS_Table_MultiVariableLookupFields::InterpolationMethod, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool TableMultiVariableLookup_Impl::isInterpolationMethodDefaulted() const {
      return isEmpty(OS_Table_MultiVariableLookupFields::InterpolationMethod);
    }

    int TableMultiVariableLookup_Impl::numberofInterpolationPoints() const {
      boost::optional<int> value = getInt(OS_Table_MultiVariableLookupFields::NumberofInterpolationPoints, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool TableMultiVariableLookup_Impl::isNumberofInterpolationPointsDefaulted() const {
      return isEmpty(OS_Table_MultiVariableLookupFields::NumberofInterpolationPoints);
    }

    boost::optional<std::string> TableMultiVariableLookup_Impl::curveType() const {
      return getString(OS_Table_MultiVariableLookupFields::CurveType, true);
    }

    std::string TableMultiVariableLookup_Impl::tableDataFormat() const {
      boost::optional<std::string> value = getString(OS_Table_MultiVariableLookupFields::TableDataFormat, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool TableMultiVariableLookup_Impl::isTableDataFormatDefaulted() const {
      return isEmpty(OS_Table_MultiVariableLookupFields::TableDataFormat);
    }

    boost::optional<std::string> TableMultiVariableLookup_Impl::externalFileName() const {
      return getString(OS_Table_MultiVariableLookupFields::ExternalFileName, true);
    }

    std::string TableMultiVariableLookup_Impl::x1SortOrder() const {
      boost::optional<std::string> value = getString(OS_Table_MultiVariableLookupFields::X1SortOrder, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool TableMultiVariableLookup_Impl::isX1SortOrderDefaulted() const {
      return isEmpty(OS_Table_MultiVariableLookupFields::X1SortOrder);
    }

    std::string TableMultiVariableLookup_Impl::x2SortOrder() const {
      boost::optional<std::string> value = getString(OS_Table_MultiVariableLookupFields::X2SortOrder, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool TableMultiVariableLookup_Impl::isX2SortOrderDefaulted() const {
      return isEmpty(OS_Table_MultiVariableLookupFields::X2SortOrder);
    }

    boost::optional<double> TableMultiVariableLookup_Impl::normalizationReference() const {
      return getDouble(OS_Table_MultiVariableLookupFields::NormalizationReference, true);
    }

    boost::optional<double> TableMultiVariableLookup_Impl::minimumValueofX1() const {
      return getDouble(OS_Table_MultiVariableLookupFields::MinimumValueofX1, true);
    }

    boost::optional<double> TableMultiVariableLookup_Impl::maximumValueofX1() const {
      return getDouble(OS_Table_MultiVariableLookupFields::MaximumValueofX1, true);
    }

    boost::optional<double> TableMultiVariableLookup_Impl::minimumValueofX2() const {
      return getDouble(OS_Table_MultiVariableLookupFields::MinimumValueofX2, true);
    }

    boost::optional<double> TableMultiVariableLookup_Impl::maximumValueofX2() const {
      return getDouble(OS_Table_MultiVariableLookupFields::MaximumValueofX2, true);
    }

    boost::optional<double> TableMultiVariableLookup_Impl::minimumValueofX3() const {
      return getDouble(OS_Table_MultiVariableLookupFields::MinimumValueofX3, true);
    }

    boost::optional<double> TableMultiVariableLookup_Impl::maximumValueofX3() const {
      return getDouble(OS_Table_MultiVariableLookupFields::MaximumValueofX3, true);
    }

    boost::optional<double> TableMultiVariableLookup_Impl::minimumValueofX4() const {
      return getDouble(OS_Table_MultiVariableLookupFields::MinimumValueofX4, true);
    }

    boost::optional<double> TableMultiVariableLookup_Impl::maximumValueofX4() const {
      return getDouble(OS_Table_MultiVariableLookupFields::MaximumValueofX4, true);
    }

    boost::optional<double> TableMultiVariableLookup_Impl::minimumValueofX5() const {
      return getDouble(OS_Table_MultiVariableLookupFields::MinimumValueofX5, true);
    }

    boost::optional<double> TableMultiVariableLookup_Impl::maximumValueofX5() const {
      return getDouble(OS_Table_MultiVariableLookupFields::MaximumValueofX5, true);
    }

    boost::optional<double> TableMultiVariableLookup_Impl::minimumTableOutput() const {
      return getDouble(OS_Table_MultiVariableLookupFields::MinimumTableOutput, true);
    }

    boost::optional<double> TableMultiVariableLookup_Impl::maximumTableOutput() const {
      return getDouble(OS_Table_MultiVariableLookupFields::MaximumTableOutput, true);
    }

    std::string TableMultiVariableLookup_Impl::inputUnitTypeforX1() const {
      boost::optional<std::string> value = getString(OS_Table_MultiVariableLookupFields::InputUnitTypeforX1, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool TableMultiVariableLookup_Impl::isInputUnitTypeforX1Defaulted() const {
      return isEmpty(OS_Table_MultiVariableLookupFields::InputUnitTypeforX1);
    }

    std::string TableMultiVariableLookup_Impl::inputUnitTypeforX2() const {
      boost::optional<std::string> value = getString(OS_Table_MultiVariableLookupFields::InputUnitTypeforX2, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool TableMultiVariableLookup_Impl::isInputUnitTypeforX2Defaulted() const {
      return isEmpty(OS_Table_MultiVariableLookupFields::InputUnitTypeforX2);
    }

    std::string TableMultiVariableLookup_Impl::inputUnitTypeforX3() const {
      boost::optional<std::string> value = getString(OS_Table_MultiVariableLookupFields::InputUnitTypeforX3, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool TableMultiVariableLookup_Impl::isInputUnitTypeforX3Defaulted() const {
      return isEmpty(OS_Table_MultiVariableLookupFields::InputUnitTypeforX3);
    }

    std::string TableMultiVariableLookup_Impl::inputUnitTypeforX4() const {
      boost::optional<std::string> value = getString(OS_Table_MultiVariableLookupFields::InputUnitTypeforX4, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool TableMultiVariableLookup_Impl::isInputUnitTypeforX4Defaulted() const {
      return isEmpty(OS_Table_MultiVariableLookupFields::InputUnitTypeforX4);
    }

    std::string TableMultiVariableLookup_Impl::inputUnitTypeforX5() const {
      boost::optional<std::string> value = getString(OS_Table_MultiVariableLookupFields::InputUnitTypeforX5, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool TableMultiVariableLookup_Impl::isInputUnitTypeforX5Defaulted() const {
      return isEmpty(OS_Table_MultiVariableLookupFields::InputUnitTypeforX5);
    }

    std::string TableMultiVariableLookup_Impl::outputUnitType() const {
      boost::optional<std::string> value = getString(OS_Table_MultiVariableLookupFields::OutputUnitType, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool TableMultiVariableLookup_Impl::isOutputUnitTypeDefaulted() const {
      return isEmpty(OS_Table_MultiVariableLookupFields::OutputUnitType);
    }

    int TableMultiVariableLookup_Impl::numberofIndependentVariables() const {
      boost::optional<int> value = getInt(OS_Table_MultiVariableLookupFields::NumberofIndependentVariables, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool TableMultiVariableLookup_Impl::setInterpolationMethod(const std::string& interpolationMethod) {
      bool result = setString(OS_Table_MultiVariableLookupFields::InterpolationMethod, interpolationMethod);
      return result;
    }

    void TableMultiVariableLookup_Impl::resetInterpolationMethod() {
      bool result = setString(OS_Table_MultiVariableLookupFields::InterpolationMethod, "");
      OS_ASSERT(result);
    }

    bool TableMultiVariableLookup_Impl::setNumberofInterpolationPoints(int numberofInterpolationPoints) {
      bool result = setInt(OS_Table_MultiVariableLookupFields::NumberofInterpolationPoints, numberofInterpolationPoints);
      return result;
    }

    void TableMultiVariableLookup_Impl::resetNumberofInterpolationPoints() {
      bool result = setString(OS_Table_MultiVariableLookupFields::NumberofInterpolationPoints, "");
      OS_ASSERT(result);
    }

    bool TableMultiVariableLookup_Impl::setCurveType(boost::optional<std::string> curveType) {
      bool result(false);
      if (curveType) {
        result = setString(OS_Table_MultiVariableLookupFields::CurveType, curveType.get());
      } else {
        resetCurveType();
        result = true;
      }
      return result;
    }

    void TableMultiVariableLookup_Impl::resetCurveType() {
      bool result = setString(OS_Table_MultiVariableLookupFields::CurveType, "");
      OS_ASSERT(result);
    }

    bool TableMultiVariableLookup_Impl::setTableDataFormat(const std::string& tableDataFormat) {
      bool result = setString(OS_Table_MultiVariableLookupFields::TableDataFormat, tableDataFormat);
      return result;
    }

    void TableMultiVariableLookup_Impl::resetTableDataFormat() {
      bool result = setString(OS_Table_MultiVariableLookupFields::TableDataFormat, "");
      OS_ASSERT(result);
    }

    bool TableMultiVariableLookup_Impl::setExternalFileName(boost::optional<std::string> externalFileName) {
      bool result(false);
      if (externalFileName) {
        result = setString(OS_Table_MultiVariableLookupFields::ExternalFileName, externalFileName.get());
      } else {
        resetExternalFileName();
        result = true;
      }
      OS_ASSERT(result);
      return result;
    }

    void TableMultiVariableLookup_Impl::resetExternalFileName() {
      bool result = setString(OS_Table_MultiVariableLookupFields::ExternalFileName, "");
      OS_ASSERT(result);
    }

    bool TableMultiVariableLookup_Impl::setX1SortOrder(const std::string& x1SortOrder) {
      bool result = setString(OS_Table_MultiVariableLookupFields::X1SortOrder, x1SortOrder);
      return result;
    }

    void TableMultiVariableLookup_Impl::resetX1SortOrder() {
      bool result = setString(OS_Table_MultiVariableLookupFields::X1SortOrder, "");
      OS_ASSERT(result);
    }

    bool TableMultiVariableLookup_Impl::setX2SortOrder(const std::string& x2SortOrder) {
      bool result = setString(OS_Table_MultiVariableLookupFields::X2SortOrder, x2SortOrder);
      return result;
    }

    void TableMultiVariableLookup_Impl::resetX2SortOrder() {
      bool result = setString(OS_Table_MultiVariableLookupFields::X2SortOrder, "");
      OS_ASSERT(result);
    }

    bool TableMultiVariableLookup_Impl::setNormalizationReference(boost::optional<double> normalizationReference) {
      bool result(false);
      if (normalizationReference) {
        result = setDouble(OS_Table_MultiVariableLookupFields::NormalizationReference, normalizationReference.get());
      } else {
        resetNormalizationReference();
        result = true;
      }
      OS_ASSERT(result);
      return result;
    }

    void TableMultiVariableLookup_Impl::resetNormalizationReference() {
      bool result = setString(OS_Table_MultiVariableLookupFields::NormalizationReference, "");
      OS_ASSERT(result);
    }

    bool TableMultiVariableLookup_Impl::setMinimumValueofX1(boost::optional<double> minimumValueofX1) {
      bool result(false);
      if (minimumValueofX1) {
        result = setDouble(OS_Table_MultiVariableLookupFields::MinimumValueofX1, minimumValueofX1.get());
      } else {
        resetMinimumValueofX1();
        result = true;
      }
      OS_ASSERT(result);
      return result;
    }

    void TableMultiVariableLookup_Impl::resetMinimumValueofX1() {
      bool result = setString(OS_Table_MultiVariableLookupFields::MinimumValueofX1, "");
      OS_ASSERT(result);
    }

    bool TableMultiVariableLookup_Impl::setMaximumValueofX1(boost::optional<double> maximumValueofX1) {
      bool result(false);
      if (maximumValueofX1) {
        result = setDouble(OS_Table_MultiVariableLookupFields::MaximumValueofX1, maximumValueofX1.get());
      } else {
        resetMaximumValueofX1();
        result = true;
      }
      OS_ASSERT(result);
      return result;
    }

    void TableMultiVariableLookup_Impl::resetMaximumValueofX1() {
      bool result = setString(OS_Table_MultiVariableLookupFields::MaximumValueofX1, "");
      OS_ASSERT(result);
    }

    bool TableMultiVariableLookup_Impl::setMinimumValueofX2(boost::optional<double> minimumValueofX2) {
      bool result(false);
      if (minimumValueofX2) {
        result = setDouble(OS_Table_MultiVariableLookupFields::MinimumValueofX2, minimumValueofX2.get());
      } else {
        resetMinimumValueofX2();
        result = true;
      }
      OS_ASSERT(result);
      return result;
    }

    void TableMultiVariableLookup_Impl::resetMinimumValueofX2() {
      bool result = setString(OS_Table_MultiVariableLookupFields::MinimumValueofX2, "");
      OS_ASSERT(result);
    }

    bool TableMultiVariableLookup_Impl::setMaximumValueofX2(boost::optional<double> maximumValueofX2) {
      bool result(false);
      if (maximumValueofX2) {
        result = setDouble(OS_Table_MultiVariableLookupFields::MaximumValueofX2, maximumValueofX2.get());
      } else {
        resetMaximumValueofX2();
        result = true;
      }
      OS_ASSERT(result);
      return result;
    }

    void TableMultiVariableLookup_Impl::resetMaximumValueofX2() {
      bool result = setString(OS_Table_MultiVariableLookupFields::MaximumValueofX2, "");
      OS_ASSERT(result);
    }

    bool TableMultiVariableLookup_Impl::setMinimumValueofX3(boost::optional<double> minimumValueofX3) {
      bool result(false);
      if (minimumValueofX3) {
        result = setDouble(OS_Table_MultiVariableLookupFields::MinimumValueofX3, minimumValueofX3.get());
      } else {
        resetMinimumValueofX3();
        result = true;
      }
      OS_ASSERT(result);
      return result;
    }

    void TableMultiVariableLookup_Impl::resetMinimumValueofX3() {
      bool result = setString(OS_Table_MultiVariableLookupFields::MinimumValueofX3, "");
      OS_ASSERT(result);
    }

    bool TableMultiVariableLookup_Impl::setMaximumValueofX3(boost::optional<double> maximumValueofX3) {
      bool result(false);
      if (maximumValueofX3) {
        result = setDouble(OS_Table_MultiVariableLookupFields::MaximumValueofX3, maximumValueofX3.get());
      } else {
        resetMaximumValueofX3();
        result = true;
      }
      OS_ASSERT(result);
      return result;
    }

    void TableMultiVariableLookup_Impl::resetMaximumValueofX3() {
      bool result = setString(OS_Table_MultiVariableLookupFields::MaximumValueofX3, "");
      OS_ASSERT(result);
    }

    bool TableMultiVariableLookup_Impl::setMinimumValueofX4(boost::optional<double> minimumValueofX4) {
      bool result(false);
      if (minimumValueofX4) {
        result = setDouble(OS_Table_MultiVariableLookupFields::MinimumValueofX4, minimumValueofX4.get());
      } else {
        resetMinimumValueofX4();
        result = true;
      }
      OS_ASSERT(result);
      return result;
    }

    void TableMultiVariableLookup_Impl::resetMinimumValueofX4() {
      bool result = setString(OS_Table_MultiVariableLookupFields::MinimumValueofX4, "");
      OS_ASSERT(result);
    }

    bool TableMultiVariableLookup_Impl::setMaximumValueofX4(boost::optional<double> maximumValueofX4) {
      bool result(false);
      if (maximumValueofX4) {
        result = setDouble(OS_Table_MultiVariableLookupFields::MaximumValueofX4, maximumValueofX4.get());
      } else {
        resetMaximumValueofX4();
        result = true;
      }
      OS_ASSERT(result);
      return result;
    }

    void TableMultiVariableLookup_Impl::resetMaximumValueofX4() {
      bool result = setString(OS_Table_MultiVariableLookupFields::MaximumValueofX4, "");
      OS_ASSERT(result);
    }

    bool TableMultiVariableLookup_Impl::setMinimumValueofX5(boost::optional<double> minimumValueofX5) {
      bool result(false);
      if (minimumValueofX5) {
        result = setDouble(OS_Table_MultiVariableLookupFields::MinimumValueofX5, minimumValueofX5.get());
      } else {
        resetMinimumValueofX5();
        result = true;
      }
      OS_ASSERT(result);
      return result;
    }

    void TableMultiVariableLookup_Impl::resetMinimumValueofX5() {
      bool result = setString(OS_Table_MultiVariableLookupFields::MinimumValueofX5, "");
      OS_ASSERT(result);
    }

    bool TableMultiVariableLookup_Impl::setMaximumValueofX5(boost::optional<double> maximumValueofX5) {
      bool result(false);
      if (maximumValueofX5) {
        result = setDouble(OS_Table_MultiVariableLookupFields::MaximumValueofX5, maximumValueofX5.get());
      } else {
        resetMaximumValueofX5();
        result = true;
      }
      OS_ASSERT(result);
      return result;
    }

    void TableMultiVariableLookup_Impl::resetMaximumValueofX5() {
      bool result = setString(OS_Table_MultiVariableLookupFields::MaximumValueofX5, "");
      OS_ASSERT(result);
    }

    bool TableMultiVariableLookup_Impl::setMinimumTableOutput(boost::optional<double> minimumTableOutput) {
      bool result(false);
      if (minimumTableOutput) {
        result = setDouble(OS_Table_MultiVariableLookupFields::MinimumTableOutput, minimumTableOutput.get());
      } else {
        resetMinimumTableOutput();
        result = true;
      }
      OS_ASSERT(result);
      return result;
    }

    void TableMultiVariableLookup_Impl::resetMinimumTableOutput() {
      bool result = setString(OS_Table_MultiVariableLookupFields::MinimumTableOutput, "");
      OS_ASSERT(result);
    }

    bool TableMultiVariableLookup_Impl::setMaximumTableOutput(boost::optional<double> maximumTableOutput) {
      bool result(false);
      if (maximumTableOutput) {
        result = setDouble(OS_Table_MultiVariableLookupFields::MaximumTableOutput, maximumTableOutput.get());
      } else {
        resetMaximumTableOutput();
        result = true;
      }
      OS_ASSERT(result);
      return result;
    }

    void TableMultiVariableLookup_Impl::resetMaximumTableOutput() {
      bool result = setString(OS_Table_MultiVariableLookupFields::MaximumTableOutput, "");
      OS_ASSERT(result);
    }

    bool TableMultiVariableLookup_Impl::setInputUnitTypeforX1(const std::string& inputUnitTypeforX1) {
      bool result = setString(OS_Table_MultiVariableLookupFields::InputUnitTypeforX1, inputUnitTypeforX1);
      return result;
    }

    void TableMultiVariableLookup_Impl::resetInputUnitTypeforX1() {
      bool result = setString(OS_Table_MultiVariableLookupFields::InputUnitTypeforX1, "");
      OS_ASSERT(result);
    }

    bool TableMultiVariableLookup_Impl::setInputUnitTypeforX2(const std::string& inputUnitTypeforX2) {
      bool result = setString(OS_Table_MultiVariableLookupFields::InputUnitTypeforX2, inputUnitTypeforX2);
      return result;
    }

    void TableMultiVariableLookup_Impl::resetInputUnitTypeforX2() {
      bool result = setString(OS_Table_MultiVariableLookupFields::InputUnitTypeforX2, "");
      OS_ASSERT(result);
    }

    bool TableMultiVariableLookup_Impl::setInputUnitTypeforX3(const std::string& inputUnitTypeforX3) {
      bool result = setString(OS_Table_MultiVariableLookupFields::InputUnitTypeforX3, inputUnitTypeforX3);
      return result;
    }

    void TableMultiVariableLookup_Impl::resetInputUnitTypeforX3() {
      bool result = setString(OS_Table_MultiVariableLookupFields::InputUnitTypeforX3, "");
      OS_ASSERT(result);
    }

    bool TableMultiVariableLookup_Impl::setInputUnitTypeforX4(const std::string& inputUnitTypeforX4) {
      bool result = setString(OS_Table_MultiVariableLookupFields::InputUnitTypeforX4, inputUnitTypeforX4);
      return result;
    }

    void TableMultiVariableLookup_Impl::resetInputUnitTypeforX4() {
      bool result = setString(OS_Table_MultiVariableLookupFields::InputUnitTypeforX4, "");
      OS_ASSERT(result);
    }

    bool TableMultiVariableLookup_Impl::setInputUnitTypeforX5(const std::string& inputUnitTypeforX5) {
      bool result = setString(OS_Table_MultiVariableLookupFields::InputUnitTypeforX5, inputUnitTypeforX5);
      return result;
    }

    void TableMultiVariableLookup_Impl::resetInputUnitTypeforX5() {
      bool result = setString(OS_Table_MultiVariableLookupFields::InputUnitTypeforX5, "");
      OS_ASSERT(result);
    }

    bool TableMultiVariableLookup_Impl::setOutputUnitType(const std::string& outputUnitType) {
      bool result = setString(OS_Table_MultiVariableLookupFields::OutputUnitType, outputUnitType);
      return result;
    }

    void TableMultiVariableLookup_Impl::resetOutputUnitType() {
      bool result = setString(OS_Table_MultiVariableLookupFields::OutputUnitType, "");
      OS_ASSERT(result);
    }

    bool TableMultiVariableLookup_Impl::setNumberofIndependentVariables(int numberofIndependentVariables) {
      // This object only accept between 1 and 5 independent variables
      // This is enforced by the IDD \minimum and \maximum, so no need to explicitly check here:
      // setInt will return false if outside that range
      bool result = setInt(OS_Table_MultiVariableLookupFields::NumberofIndependentVariables, numberofIndependentVariables);

      return result;
    }

    int TableMultiVariableLookup_Impl::numVariables() const {
      return numberofIndependentVariables();
    }

    double TableMultiVariableLookup_Impl::evaluate(const std::vector<double>& /*x*/) const {
      LOG(Warn, "Curve evaluation isn't implemented for TableMultiVariableLookup");
      return -9999.0;
    }

    bool TableMultiVariableLookup_Impl::addPoint(const std::vector<double>& t_xValues, double t_yValue) {
      if (t_xValues.size() != (unsigned)numberofIndependentVariables()) {
        return false;
      } else if (yValue(t_xValues)) {
        return false;
      } else {
        for (auto it = t_xValues.begin(); it != t_xValues.end(); ++it) {
          IdfExtensibleGroup eg = getObject<model::TableMultiVariableLookup>().pushExtensibleGroup();
          eg.setDouble(0, *it);
        }

        IdfExtensibleGroup eg = getObject<model::TableMultiVariableLookup>().pushExtensibleGroup();
        eg.setDouble(0, t_yValue);
      }

      return true;
    }

    std::vector<TableMultiVariableLookupPoint> TableMultiVariableLookup_Impl::points() const {
      std::vector<TableMultiVariableLookupPoint> result;

      int t_numberofIndependentVariables = numberofIndependentVariables();

      std::vector<IdfExtensibleGroup> groups = extensibleGroups();
      int numberOfPoints = groups.size() / (t_numberofIndependentVariables + 1);

      for (int iPoint = 0; iPoint != numberOfPoints; ++iPoint) {
        std::vector<double> xValues(t_numberofIndependentVariables);
        for (int i = 0; i != t_numberofIndependentVariables; ++i) {
          IdfExtensibleGroup g = groups[iPoint * (t_numberofIndependentVariables + 1) + i];
          boost::optional<double> d = g.getDouble(OS_Table_MultiVariableLookupExtensibleFields::Data);
          OS_ASSERT(d);
          xValues[i] = d.get();
        }

        IdfExtensibleGroup g = groups[iPoint * (t_numberofIndependentVariables + 1) + t_numberofIndependentVariables];
        boost::optional<double> d = g.getDouble(OS_Table_MultiVariableLookupExtensibleFields::Data);
        OS_ASSERT(d);
        double yValue = d.get();

        TableMultiVariableLookupPoint point(xValues, yValue);
        result.push_back(point);
      }

      return result;
    }

    // Helper function for printTable
    std::string centered(const std::string& original, int targetSize) {
      // assert( targetSize >= 0 );
      int padding = targetSize - (int)(original.size());
      return padding > 0 ? std::string(padding / 2, ' ') + original + std::string(padding - (padding / 2), ' ') : original;
    }

    // Helper function for printTable
    std::string centered(double val, int targetSize, unsigned int precision) {
      std::stringstream ss;
      ss << std::fixed << std::setprecision(precision) << val;
      return centered(ss.str(), targetSize);
    }

    // Print a table (for console output in particular)
    std::string TableMultiVariableLookup_Impl::printTable(unsigned int precision) const {
      std::vector<TableMultiVariableLookupPoint> points = this->points();
      std::stringstream ss;
      int size = points[0].x().size();

      ss << "|" << centered("n", 5) << "|";
      for (int i = 1; i <= size; ++i) {
        std::stringstream tempss;
        tempss << "x" << i;
        ss << centered(tempss.str(), 15) << "|";
      }
      ss << centered("y", 15) << "|";
      ss << "\n"
         << "|-----+";
      for (int i = 1; i <= size; ++i) {
        ss << std::string(15, '-') << "+";
      }
      ss << std::string(15, '-') << "|\n";

      int i = 1;
      for (const TableMultiVariableLookupPoint& pt : points) {
        ss << "|" << centered(std::to_string(i), 5);
        for (const double& x : pt.x()) {
          ss << "|" << centered(x, 15, precision);
        }
        ss << "|" << centered(pt.y(), 15, precision) << "|\n";
        ++i;
      }

      return ss.str();
    }

    std::vector<double> TableMultiVariableLookup_Impl::xValues(int xIndex) const {
      std::vector<double> result;

      if (xIndex >= numberofIndependentVariables()) {
        return result;
      }

      std::vector<TableMultiVariableLookupPoint> t_points = points();
      for (const TableMultiVariableLookupPoint& pt : t_points) {
        std::vector<double> xValues = pt.x();
        result.push_back(xValues[xIndex]);
      }

      std::sort(result.begin(), result.end());
      result.erase(std::unique(result.begin(), result.end()), result.end());

      return result;
    }

    boost::optional<double> TableMultiVariableLookup_Impl::yValue(const std::vector<double>& xValues) const {
      boost::optional<double> result;

      std::vector<TableMultiVariableLookupPoint> t_points = points();
      for (const TableMultiVariableLookupPoint& pt : t_points) {
        std::vector<double> t_c = pt.x();
        if (xValuesEqual(t_c, xValues)) {
          result = pt.y();
          break;
        }
      }

      return result;
    }

    bool TableMultiVariableLookup_Impl::setPoints(const std::vector<TableMultiVariableLookupPoint>& points) {
      // We first check that ALL points have the right number of independent variables, before we do anything destructive such as clearing the points
      int n_independent = numberofIndependentVariables();
      for (const TableMultiVariableLookupPoint& pt : points) {
        if (pt.x().size() != (unsigned)n_independent) {
          LOG(Warn,
              "Cannot set points because all points must have exactly " << n_independent << " independent variable(s), for " << briefDescription());
          return false;
        }
      }

      // Ok, now we clear
      this->clearExtensibleGroups();

      // And we add all points
      bool result = true;
      for (const TableMultiVariableLookupPoint& pt : points) {
        result &= addPoint(pt);
      }

      return result;
    }

    bool TableMultiVariableLookup_Impl::addPoint(const TableMultiVariableLookupPoint& point) {
      return addPoint(point.x(), point.y());
    }

    bool TableMultiVariableLookup_Impl::addPoint(double x1, double yValue) {
      std::vector<double> xValues(1);
      xValues[0] = x1;
      return addPoint(xValues, yValue);
    }

    bool TableMultiVariableLookup_Impl::addPoint(double x1, double x2, double yValue) {
      std::vector<double> xValues(2);
      xValues[0] = x1;
      xValues[1] = x2;
      return addPoint(xValues, yValue);
    }

    bool TableMultiVariableLookup_Impl::addPoint(double x1, double x2, double x3, double yValue) {
      std::vector<double> xValues(3);
      xValues[0] = x1;
      xValues[1] = x2;
      xValues[2] = x3;
      return addPoint(xValues, yValue);
    }

    bool TableMultiVariableLookup_Impl::addPoint(double x1, double x2, double x3, double x4, double yValue) {
      std::vector<double> xValues(4);
      xValues[0] = x1;
      xValues[1] = x2;
      xValues[2] = x3;
      xValues[3] = x4;
      return addPoint(xValues, yValue);
    }

    bool TableMultiVariableLookup_Impl::addPoint(double x1, double x2, double x3, double x4, double x5, double yValue) {
      std::vector<double> xValues(5);
      xValues[0] = x1;
      xValues[1] = x2;
      xValues[2] = x3;
      xValues[3] = x4;
      xValues[4] = x5;
      return addPoint(xValues, yValue);
    }

  }  // namespace detail

  TableMultiVariableLookup::TableMultiVariableLookup(const Model& model, int numberofIndependentVariables)
    : Curve(TableMultiVariableLookup::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::TableMultiVariableLookup_Impl>());

    DEPRECATED_AT_MSG(3, 5, 0, "TableMultiVariableLookup is deprecated. Use TableLookup instead.");

    // Check if numberofIndependentVariables between 1 and 5 included, otherwise THROW
    bool ok = getImpl<detail::TableMultiVariableLookup_Impl>()->setNumberofIndependentVariables(numberofIndependentVariables);
    if (!ok) {
      remove();
      LOG_AND_THROW("TableMultiVariableLookup only accepts between 1 and 5 independent variables (included).");
    }
  }

  IddObjectType TableMultiVariableLookup::iddObjectType() {
    return {IddObjectType::OS_Table_MultiVariableLookup};
  }

  std::vector<std::string> TableMultiVariableLookup::interpolationMethodValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Table_MultiVariableLookupFields::InterpolationMethod);
  }

  std::vector<std::string> TableMultiVariableLookup::curveTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Table_MultiVariableLookupFields::CurveType);
  }

  std::vector<std::string> TableMultiVariableLookup::tableDataFormatValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Table_MultiVariableLookupFields::TableDataFormat);
  }

  std::vector<std::string> TableMultiVariableLookup::x1SortOrderValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Table_MultiVariableLookupFields::X1SortOrder);
  }

  std::vector<std::string> TableMultiVariableLookup::x2SortOrderValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Table_MultiVariableLookupFields::X2SortOrder);
  }

  std::vector<std::string> TableMultiVariableLookup::inputUnitTypeforX1Values() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Table_MultiVariableLookupFields::InputUnitTypeforX1);
  }

  std::vector<std::string> TableMultiVariableLookup::inputUnitTypeforX2Values() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Table_MultiVariableLookupFields::InputUnitTypeforX2);
  }

  std::vector<std::string> TableMultiVariableLookup::inputUnitTypeforX3Values() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Table_MultiVariableLookupFields::InputUnitTypeforX3);
  }

  std::vector<std::string> TableMultiVariableLookup::inputUnitTypeforX4Values() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Table_MultiVariableLookupFields::InputUnitTypeforX4);
  }

  std::vector<std::string> TableMultiVariableLookup::inputUnitTypeforX5Values() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Table_MultiVariableLookupFields::InputUnitTypeforX5);
  }

  std::vector<std::string> TableMultiVariableLookup::outputUnitTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Table_MultiVariableLookupFields::OutputUnitType);
  }

  std::string TableMultiVariableLookup::interpolationMethod() const {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->interpolationMethod();
  }

  bool TableMultiVariableLookup::isInterpolationMethodDefaulted() const {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->isInterpolationMethodDefaulted();
  }

  int TableMultiVariableLookup::numberofInterpolationPoints() const {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->numberofInterpolationPoints();
  }

  bool TableMultiVariableLookup::isNumberofInterpolationPointsDefaulted() const {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->isNumberofInterpolationPointsDefaulted();
  }

  boost::optional<std::string> TableMultiVariableLookup::curveType() const {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->curveType();
  }

  std::string TableMultiVariableLookup::tableDataFormat() const {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->tableDataFormat();
  }

  bool TableMultiVariableLookup::isTableDataFormatDefaulted() const {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->isTableDataFormatDefaulted();
  }

  boost::optional<double> TableMultiVariableLookup::normalizationReference() const {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->normalizationReference();
  }

  boost::optional<double> TableMultiVariableLookup::minimumValueofX1() const {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->minimumValueofX1();
  }

  boost::optional<double> TableMultiVariableLookup::maximumValueofX1() const {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->maximumValueofX1();
  }

  boost::optional<double> TableMultiVariableLookup::minimumValueofX2() const {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->minimumValueofX2();
  }

  boost::optional<double> TableMultiVariableLookup::maximumValueofX2() const {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->maximumValueofX2();
  }

  boost::optional<double> TableMultiVariableLookup::minimumValueofX3() const {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->minimumValueofX3();
  }

  boost::optional<double> TableMultiVariableLookup::maximumValueofX3() const {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->maximumValueofX3();
  }

  boost::optional<double> TableMultiVariableLookup::minimumValueofX4() const {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->minimumValueofX4();
  }

  boost::optional<double> TableMultiVariableLookup::maximumValueofX4() const {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->maximumValueofX4();
  }

  boost::optional<double> TableMultiVariableLookup::minimumValueofX5() const {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->minimumValueofX5();
  }

  boost::optional<double> TableMultiVariableLookup::maximumValueofX5() const {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->maximumValueofX5();
  }

  boost::optional<double> TableMultiVariableLookup::minimumTableOutput() const {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->minimumTableOutput();
  }

  boost::optional<double> TableMultiVariableLookup::maximumTableOutput() const {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->maximumTableOutput();
  }

  std::string TableMultiVariableLookup::inputUnitTypeforX1() const {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->inputUnitTypeforX1();
  }

  bool TableMultiVariableLookup::isInputUnitTypeforX1Defaulted() const {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->isInputUnitTypeforX1Defaulted();
  }

  std::string TableMultiVariableLookup::inputUnitTypeforX2() const {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->inputUnitTypeforX2();
  }

  bool TableMultiVariableLookup::isInputUnitTypeforX2Defaulted() const {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->isInputUnitTypeforX2Defaulted();
  }

  std::string TableMultiVariableLookup::inputUnitTypeforX3() const {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->inputUnitTypeforX3();
  }

  bool TableMultiVariableLookup::isInputUnitTypeforX3Defaulted() const {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->isInputUnitTypeforX3Defaulted();
  }

  std::string TableMultiVariableLookup::inputUnitTypeforX4() const {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->inputUnitTypeforX4();
  }

  bool TableMultiVariableLookup::isInputUnitTypeforX4Defaulted() const {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->isInputUnitTypeforX4Defaulted();
  }

  std::string TableMultiVariableLookup::inputUnitTypeforX5() const {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->inputUnitTypeforX5();
  }

  bool TableMultiVariableLookup::isInputUnitTypeforX5Defaulted() const {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->isInputUnitTypeforX5Defaulted();
  }

  std::string TableMultiVariableLookup::outputUnitType() const {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->outputUnitType();
  }

  bool TableMultiVariableLookup::isOutputUnitTypeDefaulted() const {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->isOutputUnitTypeDefaulted();
  }

  int TableMultiVariableLookup::numberofIndependentVariables() const {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->numberofIndependentVariables();
  }

  bool TableMultiVariableLookup::setInterpolationMethod(const std::string& interpolationMethod) {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->setInterpolationMethod(interpolationMethod);
  }

  void TableMultiVariableLookup::resetInterpolationMethod() {
    getImpl<detail::TableMultiVariableLookup_Impl>()->resetInterpolationMethod();
  }

  bool TableMultiVariableLookup::setNumberofInterpolationPoints(int numberofInterpolationPoints) {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->setNumberofInterpolationPoints(numberofInterpolationPoints);
  }

  void TableMultiVariableLookup::resetNumberofInterpolationPoints() {
    getImpl<detail::TableMultiVariableLookup_Impl>()->resetNumberofInterpolationPoints();
  }

  bool TableMultiVariableLookup::setCurveType(const std::string& curveType) {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->setCurveType(curveType);
  }

  void TableMultiVariableLookup::resetCurveType() {
    getImpl<detail::TableMultiVariableLookup_Impl>()->resetCurveType();
  }

  bool TableMultiVariableLookup::setTableDataFormat(const std::string& tableDataFormat) {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->setTableDataFormat(tableDataFormat);
  }

  void TableMultiVariableLookup::resetTableDataFormat() {
    getImpl<detail::TableMultiVariableLookup_Impl>()->resetTableDataFormat();
  }

  bool TableMultiVariableLookup::setNormalizationReference(double normalizationReference) {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->setNormalizationReference(normalizationReference);
  }

  void TableMultiVariableLookup::resetNormalizationReference() {
    getImpl<detail::TableMultiVariableLookup_Impl>()->resetNormalizationReference();
  }

  bool TableMultiVariableLookup::setMinimumValueofX1(double minimumValueofX1) {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->setMinimumValueofX1(minimumValueofX1);
  }

  void TableMultiVariableLookup::resetMinimumValueofX1() {
    getImpl<detail::TableMultiVariableLookup_Impl>()->resetMinimumValueofX1();
  }

  bool TableMultiVariableLookup::setMaximumValueofX1(double maximumValueofX1) {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->setMaximumValueofX1(maximumValueofX1);
  }

  void TableMultiVariableLookup::resetMaximumValueofX1() {
    getImpl<detail::TableMultiVariableLookup_Impl>()->resetMaximumValueofX1();
  }

  bool TableMultiVariableLookup::setMinimumValueofX2(double minimumValueofX2) {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->setMinimumValueofX2(minimumValueofX2);
  }

  void TableMultiVariableLookup::resetMinimumValueofX2() {
    getImpl<detail::TableMultiVariableLookup_Impl>()->resetMinimumValueofX2();
  }

  bool TableMultiVariableLookup::setMaximumValueofX2(double maximumValueofX2) {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->setMaximumValueofX2(maximumValueofX2);
  }

  void TableMultiVariableLookup::resetMaximumValueofX2() {
    getImpl<detail::TableMultiVariableLookup_Impl>()->resetMaximumValueofX2();
  }

  bool TableMultiVariableLookup::setMinimumValueofX3(double minimumValueofX3) {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->setMinimumValueofX3(minimumValueofX3);
  }

  void TableMultiVariableLookup::resetMinimumValueofX3() {
    getImpl<detail::TableMultiVariableLookup_Impl>()->resetMinimumValueofX3();
  }

  bool TableMultiVariableLookup::setMaximumValueofX3(double maximumValueofX3) {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->setMaximumValueofX3(maximumValueofX3);
  }

  void TableMultiVariableLookup::resetMaximumValueofX3() {
    getImpl<detail::TableMultiVariableLookup_Impl>()->resetMaximumValueofX3();
  }

  bool TableMultiVariableLookup::setMinimumValueofX4(double minimumValueofX4) {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->setMinimumValueofX4(minimumValueofX4);
  }

  void TableMultiVariableLookup::resetMinimumValueofX4() {
    getImpl<detail::TableMultiVariableLookup_Impl>()->resetMinimumValueofX4();
  }

  bool TableMultiVariableLookup::setMaximumValueofX4(double maximumValueofX4) {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->setMaximumValueofX4(maximumValueofX4);
  }

  void TableMultiVariableLookup::resetMaximumValueofX4() {
    getImpl<detail::TableMultiVariableLookup_Impl>()->resetMaximumValueofX4();
  }

  bool TableMultiVariableLookup::setMinimumValueofX5(double minimumValueofX5) {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->setMinimumValueofX5(minimumValueofX5);
  }

  void TableMultiVariableLookup::resetMinimumValueofX5() {
    getImpl<detail::TableMultiVariableLookup_Impl>()->resetMinimumValueofX5();
  }

  bool TableMultiVariableLookup::setMaximumValueofX5(double maximumValueofX5) {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->setMaximumValueofX5(maximumValueofX5);
  }

  void TableMultiVariableLookup::resetMaximumValueofX5() {
    getImpl<detail::TableMultiVariableLookup_Impl>()->resetMaximumValueofX5();
  }

  bool TableMultiVariableLookup::setMinimumTableOutput(double minimumTableOutput) {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->setMinimumTableOutput(minimumTableOutput);
  }

  void TableMultiVariableLookup::resetMinimumTableOutput() {
    getImpl<detail::TableMultiVariableLookup_Impl>()->resetMinimumTableOutput();
  }

  bool TableMultiVariableLookup::setMaximumTableOutput(double maximumTableOutput) {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->setMaximumTableOutput(maximumTableOutput);
  }

  void TableMultiVariableLookup::resetMaximumTableOutput() {
    getImpl<detail::TableMultiVariableLookup_Impl>()->resetMaximumTableOutput();
  }

  bool TableMultiVariableLookup::setInputUnitTypeforX1(const std::string& inputUnitTypeforX1) {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->setInputUnitTypeforX1(inputUnitTypeforX1);
  }

  void TableMultiVariableLookup::resetInputUnitTypeforX1() {
    getImpl<detail::TableMultiVariableLookup_Impl>()->resetInputUnitTypeforX1();
  }

  bool TableMultiVariableLookup::setInputUnitTypeforX2(const std::string& inputUnitTypeforX2) {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->setInputUnitTypeforX2(inputUnitTypeforX2);
  }

  void TableMultiVariableLookup::resetInputUnitTypeforX2() {
    getImpl<detail::TableMultiVariableLookup_Impl>()->resetInputUnitTypeforX2();
  }

  bool TableMultiVariableLookup::setInputUnitTypeforX3(const std::string& inputUnitTypeforX3) {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->setInputUnitTypeforX3(inputUnitTypeforX3);
  }

  void TableMultiVariableLookup::resetInputUnitTypeforX3() {
    getImpl<detail::TableMultiVariableLookup_Impl>()->resetInputUnitTypeforX3();
  }

  bool TableMultiVariableLookup::setInputUnitTypeforX4(const std::string& inputUnitTypeforX4) {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->setInputUnitTypeforX4(inputUnitTypeforX4);
  }

  void TableMultiVariableLookup::resetInputUnitTypeforX4() {
    getImpl<detail::TableMultiVariableLookup_Impl>()->resetInputUnitTypeforX4();
  }

  bool TableMultiVariableLookup::setInputUnitTypeforX5(const std::string& inputUnitTypeforX5) {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->setInputUnitTypeforX5(inputUnitTypeforX5);
  }

  void TableMultiVariableLookup::resetInputUnitTypeforX5() {
    getImpl<detail::TableMultiVariableLookup_Impl>()->resetInputUnitTypeforX5();
  }

  bool TableMultiVariableLookup::setOutputUnitType(const std::string& outputUnitType) {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->setOutputUnitType(outputUnitType);
  }

  void TableMultiVariableLookup::resetOutputUnitType() {
    getImpl<detail::TableMultiVariableLookup_Impl>()->resetOutputUnitType();
  }

  // cppcheck-suppress [duplInheritedMember] this is overriden for documentation purposes
  int TableMultiVariableLookup::numVariables() const {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->numVariables();
  }

  // cppcheck-suppress [duplInheritedMember] this is overriden for documentation purposes
  double TableMultiVariableLookup::evaluate(const std::vector<double>& x) const {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->evaluate(x);
  }

  bool TableMultiVariableLookup::addPoint(const TableMultiVariableLookupPoint& point) {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->addPoint(point);
  }

  bool TableMultiVariableLookup::addPoint(const std::vector<double>& xValues, double yValue) {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->addPoint(xValues, yValue);
  }

  bool TableMultiVariableLookup::addPoint(double x1, double yValue) {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->addPoint(x1, yValue);
  }

  bool TableMultiVariableLookup::addPoint(double x1, double x2, double yValue) {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->addPoint(x1, x2, yValue);
  }

  bool TableMultiVariableLookup::addPoint(double x1, double x2, double x3, double yValue) {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->addPoint(x1, x2, x3, yValue);
  }

  bool TableMultiVariableLookup::addPoint(double x1, double x2, double x3, double x4, double yValue) {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->addPoint(x1, x2, x3, x4, yValue);
  }

  bool TableMultiVariableLookup::addPoint(double x1, double x2, double x3, double x4, double x5, double yValue) {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->addPoint(x1, x2, x3, x4, x5, yValue);
  }

  bool TableMultiVariableLookup::setPoints(const std::vector<TableMultiVariableLookupPoint>& points) {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->setPoints(points);
  }

  std::vector<TableMultiVariableLookupPoint> TableMultiVariableLookup::points() const {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->points();
  }

  std::string TableMultiVariableLookup::printTable(unsigned int precision) const {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->printTable(precision);
  }

  boost::optional<double> TableMultiVariableLookup::yValue(const std::vector<double>& xValues) const {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->yValue(xValues);
  }

  std::vector<double> TableMultiVariableLookup::xValues(int i) const {
    return getImpl<detail::TableMultiVariableLookup_Impl>()->xValues(i);
  }

  /// @cond
  TableMultiVariableLookup::TableMultiVariableLookup(std::shared_ptr<detail::TableMultiVariableLookup_Impl> impl)
    : Curve(std::dynamic_pointer_cast<detail::Curve_Impl>(std::move(impl))) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio

#if defined(_MSC_VER)
#  pragma warning(pop)
#elif (defined(__GNUC__))
#  pragma GCC diagnostic pop
#endif
