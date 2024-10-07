/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_TABLEMULTIVARIABLELOOKUP_IMPL_HPP
#define MODEL_TABLEMULTIVARIABLELOOKUP_IMPL_HPP

#include "ModelAPI.hpp"
#include "Curve_Impl.hpp"
#include "TableMultiVariableLookup.hpp"

namespace openstudio {
namespace model {

  // Foward declaration
  class TableMultiVariableLookupPoint;

  namespace detail {

    /** TableMultiVariableLookup_Impl is a Curve_Impl that is the implementation class for TableMultiVariableLookup.*/
    class MODEL_API TableMultiVariableLookup_Impl : public Curve_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      TableMultiVariableLookup_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      TableMultiVariableLookup_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      TableMultiVariableLookup_Impl(const TableMultiVariableLookup_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~TableMultiVariableLookup_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      std::string interpolationMethod() const;

      bool isInterpolationMethodDefaulted() const;

      int numberofInterpolationPoints() const;

      bool isNumberofInterpolationPointsDefaulted() const;

      boost::optional<std::string> curveType() const;

      std::string tableDataFormat() const;

      bool isTableDataFormatDefaulted() const;

      boost::optional<std::string> externalFileName() const;

      std::string x1SortOrder() const;

      bool isX1SortOrderDefaulted() const;

      std::string x2SortOrder() const;

      bool isX2SortOrderDefaulted() const;

      boost::optional<double> normalizationReference() const;

      boost::optional<double> minimumValueofX1() const;

      boost::optional<double> maximumValueofX1() const;

      boost::optional<double> minimumValueofX2() const;

      boost::optional<double> maximumValueofX2() const;

      boost::optional<double> minimumValueofX3() const;

      boost::optional<double> maximumValueofX3() const;

      boost::optional<double> minimumValueofX4() const;

      boost::optional<double> maximumValueofX4() const;

      boost::optional<double> minimumValueofX5() const;

      boost::optional<double> maximumValueofX5() const;

      boost::optional<double> minimumTableOutput() const;

      boost::optional<double> maximumTableOutput() const;

      std::string inputUnitTypeforX1() const;

      bool isInputUnitTypeforX1Defaulted() const;

      std::string inputUnitTypeforX2() const;

      bool isInputUnitTypeforX2Defaulted() const;

      std::string inputUnitTypeforX3() const;

      bool isInputUnitTypeforX3Defaulted() const;

      std::string inputUnitTypeforX4() const;

      bool isInputUnitTypeforX4Defaulted() const;

      std::string inputUnitTypeforX5() const;

      bool isInputUnitTypeforX5Defaulted() const;

      std::string outputUnitType() const;

      bool isOutputUnitTypeDefaulted() const;

      int numberofIndependentVariables() const;

      /** Print a fixed-width table of the points, precision is the number of decimals */
      std::string printTable(unsigned int precision) const;

      //@}
      /** @name Setters */
      //@{

      bool setInterpolationMethod(const std::string& interpolationMethod);

      void resetInterpolationMethod();

      bool setNumberofInterpolationPoints(int numberofInterpolationPoints);

      void resetNumberofInterpolationPoints();

      bool setCurveType(boost::optional<std::string> curveType);

      void resetCurveType();

      bool setTableDataFormat(const std::string& tableDataFormat);

      void resetTableDataFormat();

      bool setExternalFileName(boost::optional<std::string> externalFileName);

      void resetExternalFileName();

      bool setX1SortOrder(const std::string& x1SortOrder);

      void resetX1SortOrder();

      bool setX2SortOrder(const std::string& x2SortOrder);

      void resetX2SortOrder();

      bool setNormalizationReference(boost::optional<double> normalizationReference);

      void resetNormalizationReference();

      bool setMinimumValueofX1(boost::optional<double> minimumValueofX1);

      void resetMinimumValueofX1();

      bool setMaximumValueofX1(boost::optional<double> maximumValueofX1);

      void resetMaximumValueofX1();

      bool setMinimumValueofX2(boost::optional<double> minimumValueofX2);

      void resetMinimumValueofX2();

      bool setMaximumValueofX2(boost::optional<double> maximumValueofX2);

      void resetMaximumValueofX2();

      bool setMinimumValueofX3(boost::optional<double> minimumValueofX3);

      void resetMinimumValueofX3();

      bool setMaximumValueofX3(boost::optional<double> maximumValueofX3);

      void resetMaximumValueofX3();

      bool setMinimumValueofX4(boost::optional<double> minimumValueofX4);

      void resetMinimumValueofX4();

      bool setMaximumValueofX4(boost::optional<double> maximumValueofX4);

      void resetMaximumValueofX4();

      bool setMinimumValueofX5(boost::optional<double> minimumValueofX5);

      void resetMinimumValueofX5();

      bool setMaximumValueofX5(boost::optional<double> maximumValueofX5);

      void resetMaximumValueofX5();

      bool setMinimumTableOutput(boost::optional<double> minimumTableOutput);

      void resetMinimumTableOutput();

      bool setMaximumTableOutput(boost::optional<double> maximumTableOutput);

      void resetMaximumTableOutput();

      bool setInputUnitTypeforX1(const std::string& inputUnitTypeforX1);

      void resetInputUnitTypeforX1();

      bool setInputUnitTypeforX2(const std::string& inputUnitTypeforX2);

      void resetInputUnitTypeforX2();

      bool setInputUnitTypeforX3(const std::string& inputUnitTypeforX3);

      void resetInputUnitTypeforX3();

      bool setInputUnitTypeforX4(const std::string& inputUnitTypeforX4);

      void resetInputUnitTypeforX4();

      bool setInputUnitTypeforX5(const std::string& inputUnitTypeforX5);

      void resetInputUnitTypeforX5();

      bool setOutputUnitType(const std::string& outputUnitType);

      void resetOutputUnitType();

      bool setNumberofIndependentVariables(int numberofIndependentVariables);

      //@}
      /** @name Other */
      //@{

      int numVariables() const override;

      /** Return all of the x values for independent variable i
      * in ascending order.
      *
      * The independent variables are indexed from 0.
      */
      std::vector<double> xValues(int xIndex) const;

      /** Return the y value corresponding to xValues.
      * If no value then return boost::none
      */
      boost::optional<double> yValue(const std::vector<double>& xValues) const;

      double evaluate(const std::vector<double>& x) const override;

      // Primary way to add a point
      bool addPoint(const TableMultiVariableLookupPoint& point);

      // Convenience functions
      bool addPoint(const std::vector<double>& t_xValues, double t_yValue);
      bool addPoint(double x1, double yValue);
      bool addPoint(double x1, double x2, double yValue);
      bool addPoint(double x1, double x2, double x3, double yValue);
      bool addPoint(double x1, double x2, double x3, double x4, double yValue);
      bool addPoint(double x1, double x2, double x3, double x4, double x5, double yValue);

      // Directly set the points from a vector, will delete any existing points
      bool setPoints(const std::vector<TableMultiVariableLookupPoint>& points);

      std::vector<TableMultiVariableLookupPoint> points() const;

      static bool xValuesEqual(const std::vector<double>& a, const std::vector<double>& b);

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.TableMultiVariableLookup");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_TABLEMULTIVARIABLELOOKUP_IMPL_HPP
