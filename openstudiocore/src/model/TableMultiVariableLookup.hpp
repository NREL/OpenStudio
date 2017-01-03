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

#ifndef MODEL_TABLEMULTIVARIABLELOOKUP_HPP
#define MODEL_TABLEMULTIVARIABLELOOKUP_HPP

#include "ModelAPI.hpp"
#include "Curve.hpp"

#include "../utilities/math/FloatCompare.hpp"

namespace openstudio {

namespace model {

namespace detail {

  class TableMultiVariableLookup_Impl;

} // detail

/** TableMultiVariableLookup is a Curve that wraps the OpenStudio IDD object 'OS:Table:MultiVariableLookup'. */
class MODEL_API TableMultiVariableLookup : public Curve {
 public:

  /** @name Constructors and Destructors */
  //@{

  /** Create a TableMultiVariableLookup object with the specified number 
    * of independent variables.
    */
  explicit TableMultiVariableLookup(const Model& model,
                                    const int& numberofIndependentVariables);

  virtual ~TableMultiVariableLookup() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> interpolationMethodValues();

  static std::vector<std::string> curveTypeValues();

  static std::vector<std::string> tableDataFormatValues();

  static std::vector<std::string> x1SortOrderValues();

  static std::vector<std::string> x2SortOrderValues();

  static std::vector<std::string> inputUnitTypeforX1Values();

  static std::vector<std::string> inputUnitTypeforX2Values();

  static std::vector<std::string> inputUnitTypeforX3Values();

  static std::vector<std::string> inputUnitTypeforX4Values();

  static std::vector<std::string> inputUnitTypeforX5Values();

  static std::vector<std::string> outputUnitTypeValues();

  /** @name Getters */
  //@{

  std::string interpolationMethod() const;

  bool isInterpolationMethodDefaulted() const;

  int numberofInterpolationPoints() const;

  bool isNumberofInterpolationPointsDefaulted() const;

  boost::optional<std::string> curveType() const;

  std::string tableDataFormat() const;

  bool isTableDataFormatDefaulted() const;

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

  // TODO: Handle this object's extensible fields.

  //@}
  /** @name Setters */
  //@{

  bool setInterpolationMethod(std::string interpolationMethod);

  void resetInterpolationMethod();

  bool setNumberofInterpolationPoints(int numberofInterpolationPoints);

  void resetNumberofInterpolationPoints();

  bool setCurveType(std::string curveType);

  void resetCurveType();

  bool setTableDataFormat(std::string tableDataFormat);

  void resetTableDataFormat();

  void setNormalizationReference(double normalizationReference);

  void resetNormalizationReference();

  void setMinimumValueofX1(double minimumValueofX1);

  void resetMinimumValueofX1();

  void setMaximumValueofX1(double maximumValueofX1);

  void resetMaximumValueofX1();

  void setMinimumValueofX2(double minimumValueofX2);

  void resetMinimumValueofX2();

  void setMaximumValueofX2(double maximumValueofX2);

  void resetMaximumValueofX2();

  void setMinimumValueofX3(double minimumValueofX3);

  void resetMinimumValueofX3();

  void setMaximumValueofX3(double maximumValueofX3);

  void resetMaximumValueofX3();

  void setMinimumValueofX4(double minimumValueofX4);

  void resetMinimumValueofX4();

  void setMaximumValueofX4(double maximumValueofX4);

  void resetMaximumValueofX4();

  void setMinimumValueofX5(double minimumValueofX5);

  void resetMinimumValueofX5();

  void setMaximumValueofX5(double maximumValueofX5);

  void resetMaximumValueofX5();

  void setMinimumTableOutput(double minimumTableOutput);

  void resetMinimumTableOutput();

  void setMaximumTableOutput(double maximumTableOutput);

  void resetMaximumTableOutput();

  bool setInputUnitTypeforX1(std::string inputUnitTypeforX1);

  void resetInputUnitTypeforX1();

  bool setInputUnitTypeforX2(std::string inputUnitTypeforX2);

  void resetInputUnitTypeforX2();

  bool setInputUnitTypeforX3(std::string inputUnitTypeforX3);

  void resetInputUnitTypeforX3();

  bool setInputUnitTypeforX4(std::string inputUnitTypeforX4);

  void resetInputUnitTypeforX4();

  bool setInputUnitTypeforX5(std::string inputUnitTypeforX5);

  void resetInputUnitTypeforX5();

  bool setOutputUnitType(std::string outputUnitType);

  void resetOutputUnitType();

  //@}
  /** @name Other */
  //@{

  /**
   * Add a y value corresponding to xValues. The size of the XValues vector must be
   * equal to the number of independent variables specified when the table was created.
   * If a y value already exists for a particular coordinate, then the y value
   * will be replaced.
   */
  bool addPoint(const std::vector<double> & xValues, double yValue);

  bool addPoint(double x1, double yValue);

  bool addPoint(double x1, double x2, double yValue);

  bool addPoint(double x1, double x2, double x3, double yValue);

  bool addPoint(double x1, double x2, double x3, double x4, double yValue);

  bool addPoint(double x1, double x2, double x3, double x4, double x5, double yValue);

  // Return a vector of xValues and corresponding yValues, this is the entire set of data points
  std::vector<std::pair<std::vector<double>,double> > points() const;

  boost::optional<double> yValue(const std::vector<double> & xValues) const;

  /** Return all of the x values for independent variable i
    * in ascending order.
    *
    * The independent variables are indexed from 0.
    */
  std::vector<double> xValues(int i) const;

  /**
  * Synonym for numberofIndependentVariables.  This is inherited from the base class.
  */
  int numVariables() const;

  /**
  * Not supported.  This is inherited from the base class.
  */
  double evaluate(const std::vector<double>& x) const;

  //@}
 protected:
  /// @cond
  typedef detail::TableMultiVariableLookup_Impl ImplType;

  explicit TableMultiVariableLookup(std::shared_ptr<detail::TableMultiVariableLookup_Impl> impl);

  friend class detail::TableMultiVariableLookup_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.TableMultiVariableLookup");
};

/** \relates TableMultiVariableLookup*/
typedef boost::optional<TableMultiVariableLookup> OptionalTableMultiVariableLookup;

/** \relates TableMultiVariableLookup*/
typedef std::vector<TableMultiVariableLookup> TableMultiVariableLookupVector;

} // model
} // openstudio

#endif // MODEL_TABLEMULTIVARIABLELOOKUP_HPP

