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

#include "CurveTriquadratic.hpp"
#include "CurveTriquadratic_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Curve_Triquadratic_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

#include <cmath>

using namespace std;

namespace openstudio {
namespace model {

namespace detail {

  CurveTriquadratic_Impl::CurveTriquadratic_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : Curve_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CurveTriquadratic::iddObjectType());
  }

  CurveTriquadratic_Impl::CurveTriquadratic_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                 Model_Impl* model,
                                                 bool keepHandle)
    : Curve_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CurveTriquadratic::iddObjectType());
  }

  CurveTriquadratic_Impl::CurveTriquadratic_Impl(const CurveTriquadratic_Impl& other,
                                                 Model_Impl* model,
                                                 bool keepHandle)
    : Curve_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CurveTriquadratic_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CurveTriquadratic_Impl::iddObjectType() const {
    return CurveTriquadratic::iddObjectType();
  }

  int CurveTriquadratic_Impl::numVariables() const {
    return 3;
  }

  double CurveTriquadratic_Impl::evaluate(const std::vector<double>& x) const {
    OS_ASSERT(x.size() == 3u);
    double x2 = pow(x[0],2);
    double y2 = pow(x[1],2);
    double z2 = pow(x[2],2);
    double result = 0.0;
    if (OptionalDouble coeff = coefficient1Constant()) {
      result += coeff.get() * x2;
    }
    if (OptionalDouble coeff = coefficient3x()) {
      result += coeff.get() * x[0];
    }
    if (OptionalDouble coeff = coefficient4yPOW2()) {
      result += coeff.get() * y2;
    }
    if (OptionalDouble coeff = coefficient5y()) {
      result += coeff.get() * x[1];
    }
    if (OptionalDouble coeff = coefficient6zPOW2()) {
      result += coeff.get() * z2;
    }
    if (OptionalDouble coeff = coefficient7z()) {
      result += coeff.get() * x[2];
    }
    if (OptionalDouble coeff = coefficient8xPOW2TIMESYPOW2()) {
      result += coeff.get() * x2 * y2;
    }
    if (OptionalDouble coeff = coefficient9xTIMESY()) {
      result += coeff.get() * x[0] * x[1];
    }
    if (OptionalDouble coeff = coefficient10xTIMESYPOW2()) {
      result += coeff.get() * x[0] * y2;
    }
    if (OptionalDouble coeff = coefficient11xPOW2TIMESY()) {
      result += coeff.get() * x2 * x[1];
    }
    if (OptionalDouble coeff = coefficient12xPOW2TIMESZPOW2()) {
      result += coeff.get() * x2 * z2;
    }
    if (OptionalDouble coeff = coefficient13xTIMESZ()) {
      result += coeff.get() * x[0] * x[2];
    }
    if (OptionalDouble coeff = coefficient14xTIMESZPOW2()) {
      result += coeff.get() * x[0] * z2;
    }
    if (OptionalDouble coeff = coefficient15xPOW2TIMESZ()) {
      result += coeff.get() * x2 * x[2];
    }
    if (OptionalDouble coeff = coefficient16yPOW2TIMESZPOW2()) {
      result += coeff.get() * y2 * z2;
    }
    if (OptionalDouble coeff = coefficient17yTIMESZ()) {
      result += coeff.get() * x[1] * x[2];
    }
    if (OptionalDouble coeff = coefficient18yTIMESZPOW2()) {
      result += coeff.get() * x[1] * z2;
    }
    if (OptionalDouble coeff = coefficient19yPOW2TIMESZ()) {
      result += coeff.get() * y2 * x[2];
    }
    if (OptionalDouble coeff = coefficient20xPOW2TIMESYPOW2TIMESZPOW2()) {
      result += coeff.get() * x2 * y2 * z2;
    }
    if (OptionalDouble coeff = coefficient21xPOW2TIMESYPOW2TIMESZ()) {
      result += coeff.get() * x2 * y2 * x[2];
    }
    if (OptionalDouble coeff = coefficient22xPOW2TIMESYTIMESZPOW2()) {
      result += coeff.get() * x2 * x[1] * z2;
    }
    if (OptionalDouble coeff = coefficient23xTIMESYPOW2TIMESZPOW2()) {
      result += coeff.get() * x[0] * y2 * z2;
    }
    if (OptionalDouble coeff = coefficient24xPOW2TIMESYTIMESZ()) {
      result += coeff.get() * x2 * x[1] * x[2];
    }
    if (OptionalDouble coeff = coefficient25xTIMESYPOW2TIMESZ()) {
      result += coeff.get() * x[0] * y2 * x[2];
    }
    if (OptionalDouble coeff = coefficient26xTIMESYTIMESZPOW2()) {
      result += coeff.get() * x[0] * x[1] * z2;
    }
    if (OptionalDouble coeff = coefficient27xTIMESYTIMESZ()) {
      result += coeff.get() * x[0] * x[1] * x[2];
    }
    return result;
  }

  boost::optional<double> CurveTriquadratic_Impl::coefficient1Constant() const {
    return getDouble(OS_Curve_TriquadraticFields::Coefficient1Constant,true);
  }

  boost::optional<double> CurveTriquadratic_Impl::coefficient2xPOW2() const {
    return getDouble(OS_Curve_TriquadraticFields::Coefficient2x_POW_2,true);
  }

  boost::optional<double> CurveTriquadratic_Impl::coefficient3x() const {
    return getDouble(OS_Curve_TriquadraticFields::Coefficient3x,true);
  }

  boost::optional<double> CurveTriquadratic_Impl::coefficient4yPOW2() const {
    return getDouble(OS_Curve_TriquadraticFields::Coefficient4y_POW_2,true);
  }

  boost::optional<double> CurveTriquadratic_Impl::coefficient5y() const {
    return getDouble(OS_Curve_TriquadraticFields::Coefficient5y,true);
  }

  boost::optional<double> CurveTriquadratic_Impl::coefficient6zPOW2() const {
    return getDouble(OS_Curve_TriquadraticFields::Coefficient6z_POW_2,true);
  }

  boost::optional<double> CurveTriquadratic_Impl::coefficient7z() const {
    return getDouble(OS_Curve_TriquadraticFields::Coefficient7z,true);
  }

  boost::optional<double> CurveTriquadratic_Impl::coefficient8xPOW2TIMESYPOW2() const {
    return getDouble(OS_Curve_TriquadraticFields::Coefficient8x_POW_2_TIMES_y_POW_2,true);
  }

  boost::optional<double> CurveTriquadratic_Impl::coefficient9xTIMESY() const {
    return getDouble(OS_Curve_TriquadraticFields::Coefficient9x_TIMES_y,true);
  }

  boost::optional<double> CurveTriquadratic_Impl::coefficient10xTIMESYPOW2() const {
    return getDouble(OS_Curve_TriquadraticFields::Coefficient10x_TIMES_y_POW_2,true);
  }

  boost::optional<double> CurveTriquadratic_Impl::coefficient11xPOW2TIMESY() const {
    return getDouble(OS_Curve_TriquadraticFields::Coefficient11x_POW_2_TIMES_y,true);
  }

  boost::optional<double> CurveTriquadratic_Impl::coefficient12xPOW2TIMESZPOW2() const {
    return getDouble(OS_Curve_TriquadraticFields::Coefficient12x_POW_2_TIMES_z_POW_2,true);
  }

  boost::optional<double> CurveTriquadratic_Impl::coefficient13xTIMESZ() const {
    return getDouble(OS_Curve_TriquadraticFields::Coefficient13x_TIMES_z,true);
  }

  boost::optional<double> CurveTriquadratic_Impl::coefficient14xTIMESZPOW2() const {
    return getDouble(OS_Curve_TriquadraticFields::Coefficient14x_TIMES_z_POW_2,true);
  }

  boost::optional<double> CurveTriquadratic_Impl::coefficient15xPOW2TIMESZ() const {
    return getDouble(OS_Curve_TriquadraticFields::Coefficient15x_POW_2_TIMES_z,true);
  }

  boost::optional<double> CurveTriquadratic_Impl::coefficient16yPOW2TIMESZPOW2() const {
    return getDouble(OS_Curve_TriquadraticFields::Coefficient16y_POW_2_TIMES_z_POW_2,true);
  }

  boost::optional<double> CurveTriquadratic_Impl::coefficient17yTIMESZ() const {
    return getDouble(OS_Curve_TriquadraticFields::Coefficient17y_TIMES_z,true);
  }

  boost::optional<double> CurveTriquadratic_Impl::coefficient18yTIMESZPOW2() const {
    return getDouble(OS_Curve_TriquadraticFields::Coefficient18y_TIMES_z_POW_2,true);
  }

  boost::optional<double> CurveTriquadratic_Impl::coefficient19yPOW2TIMESZ() const {
    return getDouble(OS_Curve_TriquadraticFields::Coefficient19y_POW_2_TIMES_z,true);
  }

  boost::optional<double> CurveTriquadratic_Impl::coefficient20xPOW2TIMESYPOW2TIMESZPOW2() const {
    return getDouble(OS_Curve_TriquadraticFields::Coefficient20x_POW_2_TIMES_y_POW_2_TIMES_z_POW_2,true);
  }

  boost::optional<double> CurveTriquadratic_Impl::coefficient21xPOW2TIMESYPOW2TIMESZ() const {
    return getDouble(OS_Curve_TriquadraticFields::Coefficient21x_POW_2_TIMES_y_POW_2_TIMES_z,true);
  }

  boost::optional<double> CurveTriquadratic_Impl::coefficient22xPOW2TIMESYTIMESZPOW2() const {
    return getDouble(OS_Curve_TriquadraticFields::Coefficient22x_POW_2_TIMES_y_TIMES_z_POW_2,true);
  }

  boost::optional<double> CurveTriquadratic_Impl::coefficient23xTIMESYPOW2TIMESZPOW2() const {
    return getDouble(OS_Curve_TriquadraticFields::Coefficient23x_TIMES_y_POW_2_TIMES_z_POW_2,true);
  }

  boost::optional<double> CurveTriquadratic_Impl::coefficient24xPOW2TIMESYTIMESZ() const {
    return getDouble(OS_Curve_TriquadraticFields::Coefficient24x_POW_2_TIMES_y_TIMES_z,true);
  }

  boost::optional<double> CurveTriquadratic_Impl::coefficient25xTIMESYPOW2TIMESZ() const {
    return getDouble(OS_Curve_TriquadraticFields::Coefficient25x_TIMES_y_POW_2_TIMES_z,true);
  }

  boost::optional<double> CurveTriquadratic_Impl::coefficient26xTIMESYTIMESZPOW2() const {
    return getDouble(OS_Curve_TriquadraticFields::Coefficient26x_TIMES_y_TIMES_z_POW_2,true);
  }

  boost::optional<double> CurveTriquadratic_Impl::coefficient27xTIMESYTIMESZ() const {
    return getDouble(OS_Curve_TriquadraticFields::Coefficient27x_TIMES_y_TIMES_z,true);
  }

  boost::optional<double> CurveTriquadratic_Impl::minimumValueofx() const {
    return getDouble(OS_Curve_TriquadraticFields::MinimumValueofx,true);
  }

  boost::optional<double> CurveTriquadratic_Impl::maximumValueofx() const {
    return getDouble(OS_Curve_TriquadraticFields::MaximumValueofx,true);
  }

  boost::optional<double> CurveTriquadratic_Impl::minimumValueofy() const {
    return getDouble(OS_Curve_TriquadraticFields::MinimumValueofy,true);
  }

  boost::optional<double> CurveTriquadratic_Impl::maximumValueofy() const {
    return getDouble(OS_Curve_TriquadraticFields::MaximumValueofy,true);
  }

  boost::optional<double> CurveTriquadratic_Impl::minimumValueofz() const {
    return getDouble(OS_Curve_TriquadraticFields::MinimumValueofz,true);
  }

  boost::optional<double> CurveTriquadratic_Impl::maximumValueofz() const {
    return getDouble(OS_Curve_TriquadraticFields::MaximumValueofz,true);
  }

  boost::optional<double> CurveTriquadratic_Impl::minimumCurveOutput() const {
    return getDouble(OS_Curve_TriquadraticFields::MinimumCurveOutput,true);
  }

  boost::optional<double> CurveTriquadratic_Impl::maximumCurveOutput() const {
    return getDouble(OS_Curve_TriquadraticFields::MaximumCurveOutput,true);
  }

  std::string CurveTriquadratic_Impl::inputUnitTypeforX() const {
    boost::optional<std::string> value = getString(OS_Curve_TriquadraticFields::InputUnitTypeforX,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CurveTriquadratic_Impl::isInputUnitTypeforXDefaulted() const {
    return isEmpty(OS_Curve_TriquadraticFields::InputUnitTypeforX);
  }

  std::string CurveTriquadratic_Impl::inputUnitTypeforY() const {
    boost::optional<std::string> value = getString(OS_Curve_TriquadraticFields::InputUnitTypeforY,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CurveTriquadratic_Impl::isInputUnitTypeforYDefaulted() const {
    return isEmpty(OS_Curve_TriquadraticFields::InputUnitTypeforY);
  }

  std::string CurveTriquadratic_Impl::inputUnitTypeforZ() const {
    boost::optional<std::string> value = getString(OS_Curve_TriquadraticFields::InputUnitTypeforZ,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CurveTriquadratic_Impl::isInputUnitTypeforZDefaulted() const {
    return isEmpty(OS_Curve_TriquadraticFields::InputUnitTypeforZ);
  }

  std::string CurveTriquadratic_Impl::outputUnitType() const {
    boost::optional<std::string> value = getString(OS_Curve_TriquadraticFields::OutputUnitType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool CurveTriquadratic_Impl::isOutputUnitTypeDefaulted() const {
    return isEmpty(OS_Curve_TriquadraticFields::OutputUnitType);
  }

  void CurveTriquadratic_Impl::setCoefficient1Constant(boost::optional<double> coefficient1Constant) {
    bool result = false;
    if (coefficient1Constant) {
      result = setDouble(OS_Curve_TriquadraticFields::Coefficient1Constant, coefficient1Constant.get());
    } else {
      result = setString(OS_Curve_TriquadraticFields::Coefficient1Constant, "");
    }
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::resetCoefficient1Constant() {
    bool result = setString(OS_Curve_TriquadraticFields::Coefficient1Constant, "");
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::setCoefficient2xPOW2(boost::optional<double> coefficient2xPOW2) {
    bool result = false;
    if (coefficient2xPOW2) {
      result = setDouble(OS_Curve_TriquadraticFields::Coefficient2x_POW_2, coefficient2xPOW2.get());
    } else {
      result = setString(OS_Curve_TriquadraticFields::Coefficient2x_POW_2, "");
    }
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::resetCoefficient2xPOW2() {
    bool result = setString(OS_Curve_TriquadraticFields::Coefficient2x_POW_2, "");
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::setCoefficient3x(boost::optional<double> coefficient3x) {
    bool result = false;
    if (coefficient3x) {
      result = setDouble(OS_Curve_TriquadraticFields::Coefficient3x, coefficient3x.get());
    } else {
      result = setString(OS_Curve_TriquadraticFields::Coefficient3x, "");
    }
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::resetCoefficient3x() {
    bool result = setString(OS_Curve_TriquadraticFields::Coefficient3x, "");
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::setCoefficient4yPOW2(boost::optional<double> coefficient4yPOW2) {
    bool result = false;
    if (coefficient4yPOW2) {
      result = setDouble(OS_Curve_TriquadraticFields::Coefficient4y_POW_2, coefficient4yPOW2.get());
    } else {
      result = setString(OS_Curve_TriquadraticFields::Coefficient4y_POW_2, "");
    }
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::resetCoefficient4yPOW2() {
    bool result = setString(OS_Curve_TriquadraticFields::Coefficient4y_POW_2, "");
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::setCoefficient5y(boost::optional<double> coefficient5y) {
    bool result = false;
    if (coefficient5y) {
      result = setDouble(OS_Curve_TriquadraticFields::Coefficient5y, coefficient5y.get());
    } else {
      result = setString(OS_Curve_TriquadraticFields::Coefficient5y, "");
    }
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::resetCoefficient5y() {
    bool result = setString(OS_Curve_TriquadraticFields::Coefficient5y, "");
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::setCoefficient6zPOW2(boost::optional<double> coefficient6zPOW2) {
    bool result = false;
    if (coefficient6zPOW2) {
      result = setDouble(OS_Curve_TriquadraticFields::Coefficient6z_POW_2, coefficient6zPOW2.get());
    } else {
      result = setString(OS_Curve_TriquadraticFields::Coefficient6z_POW_2, "");
    }
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::resetCoefficient6zPOW2() {
    bool result = setString(OS_Curve_TriquadraticFields::Coefficient6z_POW_2, "");
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::setCoefficient7z(boost::optional<double> coefficient7z) {
    bool result = false;
    if (coefficient7z) {
      result = setDouble(OS_Curve_TriquadraticFields::Coefficient7z, coefficient7z.get());
    } else {
      result = setString(OS_Curve_TriquadraticFields::Coefficient7z, "");
    }
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::resetCoefficient7z() {
    bool result = setString(OS_Curve_TriquadraticFields::Coefficient7z, "");
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::setCoefficient8xPOW2TIMESYPOW2(boost::optional<double> coefficient8xPOW2TIMESYPOW2) {
    bool result = false;
    if (coefficient8xPOW2TIMESYPOW2) {
      result = setDouble(OS_Curve_TriquadraticFields::Coefficient8x_POW_2_TIMES_y_POW_2, coefficient8xPOW2TIMESYPOW2.get());
    } else {
      result = setString(OS_Curve_TriquadraticFields::Coefficient8x_POW_2_TIMES_y_POW_2, "");
    }
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::resetCoefficient8xPOW2TIMESYPOW2() {
    bool result = setString(OS_Curve_TriquadraticFields::Coefficient8x_POW_2_TIMES_y_POW_2, "");
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::setCoefficient9xTIMESY(boost::optional<double> coefficient9xTIMESY) {
    bool result = false;
    if (coefficient9xTIMESY) {
      result = setDouble(OS_Curve_TriquadraticFields::Coefficient9x_TIMES_y, coefficient9xTIMESY.get());
    } else {
      result = setString(OS_Curve_TriquadraticFields::Coefficient9x_TIMES_y, "");
    }
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::resetCoefficient9xTIMESY() {
    bool result = setString(OS_Curve_TriquadraticFields::Coefficient9x_TIMES_y, "");
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::setCoefficient10xTIMESYPOW2(boost::optional<double> coefficient10xTIMESYPOW2) {
    bool result = false;
    if (coefficient10xTIMESYPOW2) {
      result = setDouble(OS_Curve_TriquadraticFields::Coefficient10x_TIMES_y_POW_2, coefficient10xTIMESYPOW2.get());
    } else {
      result = setString(OS_Curve_TriquadraticFields::Coefficient10x_TIMES_y_POW_2, "");
    }
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::resetCoefficient10xTIMESYPOW2() {
    bool result = setString(OS_Curve_TriquadraticFields::Coefficient10x_TIMES_y_POW_2, "");
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::setCoefficient11xPOW2TIMESY(boost::optional<double> coefficient11xPOW2TIMESY) {
    bool result = false;
    if (coefficient11xPOW2TIMESY) {
      result = setDouble(OS_Curve_TriquadraticFields::Coefficient11x_POW_2_TIMES_y, coefficient11xPOW2TIMESY.get());
    } else {
      result = setString(OS_Curve_TriquadraticFields::Coefficient11x_POW_2_TIMES_y, "");
    }
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::resetCoefficient11xPOW2TIMESY() {
    bool result = setString(OS_Curve_TriquadraticFields::Coefficient11x_POW_2_TIMES_y, "");
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::setCoefficient12xPOW2TIMESZPOW2(boost::optional<double> coefficient12xPOW2TIMESZPOW2) {
    bool result = false;
    if (coefficient12xPOW2TIMESZPOW2) {
      result = setDouble(OS_Curve_TriquadraticFields::Coefficient12x_POW_2_TIMES_z_POW_2, coefficient12xPOW2TIMESZPOW2.get());
    } else {
      result = setString(OS_Curve_TriquadraticFields::Coefficient12x_POW_2_TIMES_z_POW_2, "");
    }
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::resetCoefficient12xPOW2TIMESZPOW2() {
    bool result = setString(OS_Curve_TriquadraticFields::Coefficient12x_POW_2_TIMES_z_POW_2, "");
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::setCoefficient13xTIMESZ(boost::optional<double> coefficient13xTIMESZ) {
    bool result = false;
    if (coefficient13xTIMESZ) {
      result = setDouble(OS_Curve_TriquadraticFields::Coefficient13x_TIMES_z, coefficient13xTIMESZ.get());
    } else {
      result = setString(OS_Curve_TriquadraticFields::Coefficient13x_TIMES_z, "");
    }
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::resetCoefficient13xTIMESZ() {
    bool result = setString(OS_Curve_TriquadraticFields::Coefficient13x_TIMES_z, "");
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::setCoefficient14xTIMESZPOW2(boost::optional<double> coefficient14xTIMESZPOW2) {
    bool result = false;
    if (coefficient14xTIMESZPOW2) {
      result = setDouble(OS_Curve_TriquadraticFields::Coefficient14x_TIMES_z_POW_2, coefficient14xTIMESZPOW2.get());
    } else {
      result = setString(OS_Curve_TriquadraticFields::Coefficient14x_TIMES_z_POW_2, "");
    }
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::resetCoefficient14xTIMESZPOW2() {
    bool result = setString(OS_Curve_TriquadraticFields::Coefficient14x_TIMES_z_POW_2, "");
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::setCoefficient15xPOW2TIMESZ(boost::optional<double> coefficient15xPOW2TIMESZ) {
    bool result = false;
    if (coefficient15xPOW2TIMESZ) {
      result = setDouble(OS_Curve_TriquadraticFields::Coefficient15x_POW_2_TIMES_z, coefficient15xPOW2TIMESZ.get());
    } else {
      result = setString(OS_Curve_TriquadraticFields::Coefficient15x_POW_2_TIMES_z, "");
    }
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::resetCoefficient15xPOW2TIMESZ() {
    bool result = setString(OS_Curve_TriquadraticFields::Coefficient15x_POW_2_TIMES_z, "");
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::setCoefficient16yPOW2TIMESZPOW2(boost::optional<double> coefficient16yPOW2TIMESZPOW2) {
    bool result = false;
    if (coefficient16yPOW2TIMESZPOW2) {
      result = setDouble(OS_Curve_TriquadraticFields::Coefficient16y_POW_2_TIMES_z_POW_2, coefficient16yPOW2TIMESZPOW2.get());
    } else {
      result = setString(OS_Curve_TriquadraticFields::Coefficient16y_POW_2_TIMES_z_POW_2, "");
    }
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::resetCoefficient16yPOW2TIMESZPOW2() {
    bool result = setString(OS_Curve_TriquadraticFields::Coefficient16y_POW_2_TIMES_z_POW_2, "");
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::setCoefficient17yTIMESZ(boost::optional<double> coefficient17yTIMESZ) {
    bool result = false;
    if (coefficient17yTIMESZ) {
      result = setDouble(OS_Curve_TriquadraticFields::Coefficient17y_TIMES_z, coefficient17yTIMESZ.get());
    } else {
      result = setString(OS_Curve_TriquadraticFields::Coefficient17y_TIMES_z, "");
    }
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::resetCoefficient17yTIMESZ() {
    bool result = setString(OS_Curve_TriquadraticFields::Coefficient17y_TIMES_z, "");
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::setCoefficient18yTIMESZPOW2(boost::optional<double> coefficient18yTIMESZPOW2) {
    bool result = false;
    if (coefficient18yTIMESZPOW2) {
      result = setDouble(OS_Curve_TriquadraticFields::Coefficient18y_TIMES_z_POW_2, coefficient18yTIMESZPOW2.get());
    } else {
      result = setString(OS_Curve_TriquadraticFields::Coefficient18y_TIMES_z_POW_2, "");
    }
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::resetCoefficient18yTIMESZPOW2() {
    bool result = setString(OS_Curve_TriquadraticFields::Coefficient18y_TIMES_z_POW_2, "");
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::setCoefficient19yPOW2TIMESZ(boost::optional<double> coefficient19yPOW2TIMESZ) {
    bool result = false;
    if (coefficient19yPOW2TIMESZ) {
      result = setDouble(OS_Curve_TriquadraticFields::Coefficient19y_POW_2_TIMES_z, coefficient19yPOW2TIMESZ.get());
    } else {
      result = setString(OS_Curve_TriquadraticFields::Coefficient19y_POW_2_TIMES_z, "");
    }
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::resetCoefficient19yPOW2TIMESZ() {
    bool result = setString(OS_Curve_TriquadraticFields::Coefficient19y_POW_2_TIMES_z, "");
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::setCoefficient20xPOW2TIMESYPOW2TIMESZPOW2(boost::optional<double> coefficient20xPOW2TIMESYPOW2TIMESZPOW2) {
    bool result = false;
    if (coefficient20xPOW2TIMESYPOW2TIMESZPOW2) {
      result = setDouble(OS_Curve_TriquadraticFields::Coefficient20x_POW_2_TIMES_y_POW_2_TIMES_z_POW_2, coefficient20xPOW2TIMESYPOW2TIMESZPOW2.get());
    } else {
      result = setString(OS_Curve_TriquadraticFields::Coefficient20x_POW_2_TIMES_y_POW_2_TIMES_z_POW_2, "");
    }
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::resetCoefficient20xPOW2TIMESYPOW2TIMESZPOW2() {
    bool result = setString(OS_Curve_TriquadraticFields::Coefficient20x_POW_2_TIMES_y_POW_2_TIMES_z_POW_2, "");
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::setCoefficient21xPOW2TIMESYPOW2TIMESZ(boost::optional<double> coefficient21xPOW2TIMESYPOW2TIMESZ) {
    bool result = false;
    if (coefficient21xPOW2TIMESYPOW2TIMESZ) {
      result = setDouble(OS_Curve_TriquadraticFields::Coefficient21x_POW_2_TIMES_y_POW_2_TIMES_z, coefficient21xPOW2TIMESYPOW2TIMESZ.get());
    } else {
      result = setString(OS_Curve_TriquadraticFields::Coefficient21x_POW_2_TIMES_y_POW_2_TIMES_z, "");
    }
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::resetCoefficient21xPOW2TIMESYPOW2TIMESZ() {
    bool result = setString(OS_Curve_TriquadraticFields::Coefficient21x_POW_2_TIMES_y_POW_2_TIMES_z, "");
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::setCoefficient22xPOW2TIMESYTIMESZPOW2(boost::optional<double> coefficient22xPOW2TIMESYTIMESZPOW2) {
    bool result = false;
    if (coefficient22xPOW2TIMESYTIMESZPOW2) {
      result = setDouble(OS_Curve_TriquadraticFields::Coefficient22x_POW_2_TIMES_y_TIMES_z_POW_2, coefficient22xPOW2TIMESYTIMESZPOW2.get());
    } else {
      result = setString(OS_Curve_TriquadraticFields::Coefficient22x_POW_2_TIMES_y_TIMES_z_POW_2, "");
    }
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::resetCoefficient22xPOW2TIMESYTIMESZPOW2() {
    bool result = setString(OS_Curve_TriquadraticFields::Coefficient22x_POW_2_TIMES_y_TIMES_z_POW_2, "");
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::setCoefficient23xTIMESYPOW2TIMESZPOW2(boost::optional<double> coefficient23xTIMESYPOW2TIMESZPOW2) {
    bool result = false;
    if (coefficient23xTIMESYPOW2TIMESZPOW2) {
      result = setDouble(OS_Curve_TriquadraticFields::Coefficient23x_TIMES_y_POW_2_TIMES_z_POW_2, coefficient23xTIMESYPOW2TIMESZPOW2.get());
    } else {
      result = setString(OS_Curve_TriquadraticFields::Coefficient23x_TIMES_y_POW_2_TIMES_z_POW_2, "");
    }
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::resetCoefficient23xTIMESYPOW2TIMESZPOW2() {
    bool result = setString(OS_Curve_TriquadraticFields::Coefficient23x_TIMES_y_POW_2_TIMES_z_POW_2, "");
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::setCoefficient24xPOW2TIMESYTIMESZ(boost::optional<double> coefficient24xPOW2TIMESYTIMESZ) {
    bool result = false;
    if (coefficient24xPOW2TIMESYTIMESZ) {
      result = setDouble(OS_Curve_TriquadraticFields::Coefficient24x_POW_2_TIMES_y_TIMES_z, coefficient24xPOW2TIMESYTIMESZ.get());
    } else {
      result = setString(OS_Curve_TriquadraticFields::Coefficient24x_POW_2_TIMES_y_TIMES_z, "");
    }
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::resetCoefficient24xPOW2TIMESYTIMESZ() {
    bool result = setString(OS_Curve_TriquadraticFields::Coefficient24x_POW_2_TIMES_y_TIMES_z, "");
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::setCoefficient25xTIMESYPOW2TIMESZ(boost::optional<double> coefficient25xTIMESYPOW2TIMESZ) {
    bool result = false;
    if (coefficient25xTIMESYPOW2TIMESZ) {
      result = setDouble(OS_Curve_TriquadraticFields::Coefficient25x_TIMES_y_POW_2_TIMES_z, coefficient25xTIMESYPOW2TIMESZ.get());
    } else {
      result = setString(OS_Curve_TriquadraticFields::Coefficient25x_TIMES_y_POW_2_TIMES_z, "");
    }
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::resetCoefficient25xTIMESYPOW2TIMESZ() {
    bool result = setString(OS_Curve_TriquadraticFields::Coefficient25x_TIMES_y_POW_2_TIMES_z, "");
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::setCoefficient26xTIMESYTIMESZPOW2(boost::optional<double> coefficient26xTIMESYTIMESZPOW2) {
    bool result = false;
    if (coefficient26xTIMESYTIMESZPOW2) {
      result = setDouble(OS_Curve_TriquadraticFields::Coefficient26x_TIMES_y_TIMES_z_POW_2, coefficient26xTIMESYTIMESZPOW2.get());
    } else {
      result = setString(OS_Curve_TriquadraticFields::Coefficient26x_TIMES_y_TIMES_z_POW_2, "");
    }
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::resetCoefficient26xTIMESYTIMESZPOW2() {
    bool result = setString(OS_Curve_TriquadraticFields::Coefficient26x_TIMES_y_TIMES_z_POW_2, "");
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::setCoefficient27xTIMESYTIMESZ(boost::optional<double> coefficient27xTIMESYTIMESZ) {
    bool result = false;
    if (coefficient27xTIMESYTIMESZ) {
      result = setDouble(OS_Curve_TriquadraticFields::Coefficient27x_TIMES_y_TIMES_z, coefficient27xTIMESYTIMESZ.get());
    } else {
      result = setString(OS_Curve_TriquadraticFields::Coefficient27x_TIMES_y_TIMES_z, "");
    }
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::resetCoefficient27xTIMESYTIMESZ() {
    bool result = setString(OS_Curve_TriquadraticFields::Coefficient27x_TIMES_y_TIMES_z, "");
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::setMinimumValueofx(boost::optional<double> minimumValueofx) {
    bool result = false;
    if (minimumValueofx) {
      result = setDouble(OS_Curve_TriquadraticFields::MinimumValueofx, minimumValueofx.get());
    } else {
      result = setString(OS_Curve_TriquadraticFields::MinimumValueofx, "");
    }
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::resetMinimumValueofx() {
    bool result = setString(OS_Curve_TriquadraticFields::MinimumValueofx, "");
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::setMaximumValueofx(boost::optional<double> maximumValueofx) {
    bool result = false;
    if (maximumValueofx) {
      result = setDouble(OS_Curve_TriquadraticFields::MaximumValueofx, maximumValueofx.get());
    } else {
      result = setString(OS_Curve_TriquadraticFields::MaximumValueofx, "");
    }
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::resetMaximumValueofx() {
    bool result = setString(OS_Curve_TriquadraticFields::MaximumValueofx, "");
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::setMinimumValueofy(boost::optional<double> minimumValueofy) {
    bool result = false;
    if (minimumValueofy) {
      result = setDouble(OS_Curve_TriquadraticFields::MinimumValueofy, minimumValueofy.get());
    } else {
      result = setString(OS_Curve_TriquadraticFields::MinimumValueofy, "");
    }
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::resetMinimumValueofy() {
    bool result = setString(OS_Curve_TriquadraticFields::MinimumValueofy, "");
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::setMaximumValueofy(boost::optional<double> maximumValueofy) {
    bool result = false;
    if (maximumValueofy) {
      result = setDouble(OS_Curve_TriquadraticFields::MaximumValueofy, maximumValueofy.get());
    } else {
      result = setString(OS_Curve_TriquadraticFields::MaximumValueofy, "");
    }
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::resetMaximumValueofy() {
    bool result = setString(OS_Curve_TriquadraticFields::MaximumValueofy, "");
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::setMinimumValueofz(boost::optional<double> minimumValueofz) {
    bool result = false;
    if (minimumValueofz) {
      result = setDouble(OS_Curve_TriquadraticFields::MinimumValueofz, minimumValueofz.get());
    } else {
      result = setString(OS_Curve_TriquadraticFields::MinimumValueofz, "");
    }
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::resetMinimumValueofz() {
    bool result = setString(OS_Curve_TriquadraticFields::MinimumValueofz, "");
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::setMaximumValueofz(boost::optional<double> maximumValueofz) {
    bool result = false;
    if (maximumValueofz) {
      result = setDouble(OS_Curve_TriquadraticFields::MaximumValueofz, maximumValueofz.get());
    } else {
      result = setString(OS_Curve_TriquadraticFields::MaximumValueofz, "");
    }
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::resetMaximumValueofz() {
    bool result = setString(OS_Curve_TriquadraticFields::MaximumValueofz, "");
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::setMinimumCurveOutput(boost::optional<double> minimumCurveOutput) {
    bool result = false;
    if (minimumCurveOutput) {
      result = setDouble(OS_Curve_TriquadraticFields::MinimumCurveOutput, minimumCurveOutput.get());
    } else {
      result = setString(OS_Curve_TriquadraticFields::MinimumCurveOutput, "");
    }
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::resetMinimumCurveOutput() {
    bool result = setString(OS_Curve_TriquadraticFields::MinimumCurveOutput, "");
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::setMaximumCurveOutput(boost::optional<double> maximumCurveOutput) {
    bool result = false;
    if (maximumCurveOutput) {
      result = setDouble(OS_Curve_TriquadraticFields::MaximumCurveOutput, maximumCurveOutput.get());
    } else {
      result = setString(OS_Curve_TriquadraticFields::MaximumCurveOutput, "");
    }
    OS_ASSERT(result);
  }

  void CurveTriquadratic_Impl::resetMaximumCurveOutput() {
    bool result = setString(OS_Curve_TriquadraticFields::MaximumCurveOutput, "");
    OS_ASSERT(result);
  }

  bool CurveTriquadratic_Impl::setInputUnitTypeforX(std::string inputUnitTypeforX) {
    bool result = setString(OS_Curve_TriquadraticFields::InputUnitTypeforX, inputUnitTypeforX);
    return result;
  }

  void CurveTriquadratic_Impl::resetInputUnitTypeforX() {
    bool result = setString(OS_Curve_TriquadraticFields::InputUnitTypeforX, "");
    OS_ASSERT(result);
  }

  bool CurveTriquadratic_Impl::setInputUnitTypeforY(std::string inputUnitTypeforY) {
    bool result = setString(OS_Curve_TriquadraticFields::InputUnitTypeforY, inputUnitTypeforY);
    return result;
  }

  void CurveTriquadratic_Impl::resetInputUnitTypeforY() {
    bool result = setString(OS_Curve_TriquadraticFields::InputUnitTypeforY, "");
    OS_ASSERT(result);
  }

  bool CurveTriquadratic_Impl::setInputUnitTypeforZ(std::string inputUnitTypeforZ) {
    bool result = setString(OS_Curve_TriquadraticFields::InputUnitTypeforZ, inputUnitTypeforZ);
    return result;
  }

  void CurveTriquadratic_Impl::resetInputUnitTypeforZ() {
    bool result = setString(OS_Curve_TriquadraticFields::InputUnitTypeforZ, "");
    OS_ASSERT(result);
  }

  bool CurveTriquadratic_Impl::setOutputUnitType(std::string outputUnitType) {
    bool result = setString(OS_Curve_TriquadraticFields::OutputUnitType, outputUnitType);
    return result;
  }

  void CurveTriquadratic_Impl::resetOutputUnitType() {
    bool result = setString(OS_Curve_TriquadraticFields::OutputUnitType, "");
    OS_ASSERT(result);
  }

} // detail

CurveTriquadratic::CurveTriquadratic(const Model& model)
  : Curve(CurveTriquadratic::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CurveTriquadratic_Impl>());
}

IddObjectType CurveTriquadratic::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Curve_Triquadratic);
  return result;
}

std::vector<std::string> CurveTriquadratic::validInputUnitTypeforXValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Curve_TriquadraticFields::InputUnitTypeforX);
}

std::vector<std::string> CurveTriquadratic::validInputUnitTypeforYValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Curve_TriquadraticFields::InputUnitTypeforY);
}

std::vector<std::string> CurveTriquadratic::validInputUnitTypeforZValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Curve_TriquadraticFields::InputUnitTypeforZ);
}

std::vector<std::string> CurveTriquadratic::validOutputUnitTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Curve_TriquadraticFields::OutputUnitType);
}

boost::optional<double> CurveTriquadratic::coefficient1Constant() const {
  return getImpl<detail::CurveTriquadratic_Impl>()->coefficient1Constant();
}

boost::optional<double> CurveTriquadratic::coefficient2xPOW2() const {
  return getImpl<detail::CurveTriquadratic_Impl>()->coefficient2xPOW2();
}

boost::optional<double> CurveTriquadratic::coefficient3x() const {
  return getImpl<detail::CurveTriquadratic_Impl>()->coefficient3x();
}

boost::optional<double> CurveTriquadratic::coefficient4yPOW2() const {
  return getImpl<detail::CurveTriquadratic_Impl>()->coefficient4yPOW2();
}

boost::optional<double> CurveTriquadratic::coefficient5y() const {
  return getImpl<detail::CurveTriquadratic_Impl>()->coefficient5y();
}

boost::optional<double> CurveTriquadratic::coefficient6zPOW2() const {
  return getImpl<detail::CurveTriquadratic_Impl>()->coefficient6zPOW2();
}

boost::optional<double> CurveTriquadratic::coefficient7z() const {
  return getImpl<detail::CurveTriquadratic_Impl>()->coefficient7z();
}

boost::optional<double> CurveTriquadratic::coefficient8xPOW2TIMESYPOW2() const {
  return getImpl<detail::CurveTriquadratic_Impl>()->coefficient8xPOW2TIMESYPOW2();
}

boost::optional<double> CurveTriquadratic::coefficient9xTIMESY() const {
  return getImpl<detail::CurveTriquadratic_Impl>()->coefficient9xTIMESY();
}

boost::optional<double> CurveTriquadratic::coefficient10xTIMESYPOW2() const {
  return getImpl<detail::CurveTriquadratic_Impl>()->coefficient10xTIMESYPOW2();
}

boost::optional<double> CurveTriquadratic::coefficient11xPOW2TIMESY() const {
  return getImpl<detail::CurveTriquadratic_Impl>()->coefficient11xPOW2TIMESY();
}

boost::optional<double> CurveTriquadratic::coefficient12xPOW2TIMESZPOW2() const {
  return getImpl<detail::CurveTriquadratic_Impl>()->coefficient12xPOW2TIMESZPOW2();
}

boost::optional<double> CurveTriquadratic::coefficient13xTIMESZ() const {
  return getImpl<detail::CurveTriquadratic_Impl>()->coefficient13xTIMESZ();
}

boost::optional<double> CurveTriquadratic::coefficient14xTIMESZPOW2() const {
  return getImpl<detail::CurveTriquadratic_Impl>()->coefficient14xTIMESZPOW2();
}

boost::optional<double> CurveTriquadratic::coefficient15xPOW2TIMESZ() const {
  return getImpl<detail::CurveTriquadratic_Impl>()->coefficient15xPOW2TIMESZ();
}

boost::optional<double> CurveTriquadratic::coefficient16yPOW2TIMESZPOW2() const {
  return getImpl<detail::CurveTriquadratic_Impl>()->coefficient16yPOW2TIMESZPOW2();
}

boost::optional<double> CurveTriquadratic::coefficient17yTIMESZ() const {
  return getImpl<detail::CurveTriquadratic_Impl>()->coefficient17yTIMESZ();
}

boost::optional<double> CurveTriquadratic::coefficient18yTIMESZPOW2() const {
  return getImpl<detail::CurveTriquadratic_Impl>()->coefficient18yTIMESZPOW2();
}

boost::optional<double> CurveTriquadratic::coefficient19yPOW2TIMESZ() const {
  return getImpl<detail::CurveTriquadratic_Impl>()->coefficient19yPOW2TIMESZ();
}

boost::optional<double> CurveTriquadratic::coefficient20xPOW2TIMESYPOW2TIMESZPOW2() const {
  return getImpl<detail::CurveTriquadratic_Impl>()->coefficient20xPOW2TIMESYPOW2TIMESZPOW2();
}

boost::optional<double> CurveTriquadratic::coefficient21xPOW2TIMESYPOW2TIMESZ() const {
  return getImpl<detail::CurveTriquadratic_Impl>()->coefficient21xPOW2TIMESYPOW2TIMESZ();
}

boost::optional<double> CurveTriquadratic::coefficient22xPOW2TIMESYTIMESZPOW2() const {
  return getImpl<detail::CurveTriquadratic_Impl>()->coefficient22xPOW2TIMESYTIMESZPOW2();
}

boost::optional<double> CurveTriquadratic::coefficient23xTIMESYPOW2TIMESZPOW2() const {
  return getImpl<detail::CurveTriquadratic_Impl>()->coefficient23xTIMESYPOW2TIMESZPOW2();
}

boost::optional<double> CurveTriquadratic::coefficient24xPOW2TIMESYTIMESZ() const {
  return getImpl<detail::CurveTriquadratic_Impl>()->coefficient24xPOW2TIMESYTIMESZ();
}

boost::optional<double> CurveTriquadratic::coefficient25xTIMESYPOW2TIMESZ() const {
  return getImpl<detail::CurveTriquadratic_Impl>()->coefficient25xTIMESYPOW2TIMESZ();
}

boost::optional<double> CurveTriquadratic::coefficient26xTIMESYTIMESZPOW2() const {
  return getImpl<detail::CurveTriquadratic_Impl>()->coefficient26xTIMESYTIMESZPOW2();
}

boost::optional<double> CurveTriquadratic::coefficient27xTIMESYTIMESZ() const {
  return getImpl<detail::CurveTriquadratic_Impl>()->coefficient27xTIMESYTIMESZ();
}

boost::optional<double> CurveTriquadratic::minimumValueofx() const {
  return getImpl<detail::CurveTriquadratic_Impl>()->minimumValueofx();
}

boost::optional<double> CurveTriquadratic::maximumValueofx() const {
  return getImpl<detail::CurveTriquadratic_Impl>()->maximumValueofx();
}

boost::optional<double> CurveTriquadratic::minimumValueofy() const {
  return getImpl<detail::CurveTriquadratic_Impl>()->minimumValueofy();
}

boost::optional<double> CurveTriquadratic::maximumValueofy() const {
  return getImpl<detail::CurveTriquadratic_Impl>()->maximumValueofy();
}

boost::optional<double> CurveTriquadratic::minimumValueofz() const {
  return getImpl<detail::CurveTriquadratic_Impl>()->minimumValueofz();
}

boost::optional<double> CurveTriquadratic::maximumValueofz() const {
  return getImpl<detail::CurveTriquadratic_Impl>()->maximumValueofz();
}

boost::optional<double> CurveTriquadratic::minimumCurveOutput() const {
  return getImpl<detail::CurveTriquadratic_Impl>()->minimumCurveOutput();
}

boost::optional<double> CurveTriquadratic::maximumCurveOutput() const {
  return getImpl<detail::CurveTriquadratic_Impl>()->maximumCurveOutput();
}

std::string CurveTriquadratic::inputUnitTypeforX() const {
  return getImpl<detail::CurveTriquadratic_Impl>()->inputUnitTypeforX();
}

bool CurveTriquadratic::isInputUnitTypeforXDefaulted() const {
  return getImpl<detail::CurveTriquadratic_Impl>()->isInputUnitTypeforXDefaulted();
}

std::string CurveTriquadratic::inputUnitTypeforY() const {
  return getImpl<detail::CurveTriquadratic_Impl>()->inputUnitTypeforY();
}

bool CurveTriquadratic::isInputUnitTypeforYDefaulted() const {
  return getImpl<detail::CurveTriquadratic_Impl>()->isInputUnitTypeforYDefaulted();
}

std::string CurveTriquadratic::inputUnitTypeforZ() const {
  return getImpl<detail::CurveTriquadratic_Impl>()->inputUnitTypeforZ();
}

bool CurveTriquadratic::isInputUnitTypeforZDefaulted() const {
  return getImpl<detail::CurveTriquadratic_Impl>()->isInputUnitTypeforZDefaulted();
}

std::string CurveTriquadratic::outputUnitType() const {
  return getImpl<detail::CurveTriquadratic_Impl>()->outputUnitType();
}

bool CurveTriquadratic::isOutputUnitTypeDefaulted() const {
  return getImpl<detail::CurveTriquadratic_Impl>()->isOutputUnitTypeDefaulted();
}

void CurveTriquadratic::setCoefficient1Constant(double coefficient1Constant) {
  getImpl<detail::CurveTriquadratic_Impl>()->setCoefficient1Constant(coefficient1Constant);
}

void CurveTriquadratic::resetCoefficient1Constant() {
  getImpl<detail::CurveTriquadratic_Impl>()->resetCoefficient1Constant();
}

void CurveTriquadratic::setCoefficient2xPOW2(double coefficient2xPOW2) {
  getImpl<detail::CurveTriquadratic_Impl>()->setCoefficient2xPOW2(coefficient2xPOW2);
}

void CurveTriquadratic::resetCoefficient2xPOW2() {
  getImpl<detail::CurveTriquadratic_Impl>()->resetCoefficient2xPOW2();
}

void CurveTriquadratic::setCoefficient3x(double coefficient3x) {
  getImpl<detail::CurveTriquadratic_Impl>()->setCoefficient3x(coefficient3x);
}

void CurveTriquadratic::resetCoefficient3x() {
  getImpl<detail::CurveTriquadratic_Impl>()->resetCoefficient3x();
}

void CurveTriquadratic::setCoefficient4yPOW2(double coefficient4yPOW2) {
  getImpl<detail::CurveTriquadratic_Impl>()->setCoefficient4yPOW2(coefficient4yPOW2);
}

void CurveTriquadratic::resetCoefficient4yPOW2() {
  getImpl<detail::CurveTriquadratic_Impl>()->resetCoefficient4yPOW2();
}

void CurveTriquadratic::setCoefficient5y(double coefficient5y) {
  getImpl<detail::CurveTriquadratic_Impl>()->setCoefficient5y(coefficient5y);
}

void CurveTriquadratic::resetCoefficient5y() {
  getImpl<detail::CurveTriquadratic_Impl>()->resetCoefficient5y();
}

void CurveTriquadratic::setCoefficient6zPOW2(double coefficient6zPOW2) {
  getImpl<detail::CurveTriquadratic_Impl>()->setCoefficient6zPOW2(coefficient6zPOW2);
}

void CurveTriquadratic::resetCoefficient6zPOW2() {
  getImpl<detail::CurveTriquadratic_Impl>()->resetCoefficient6zPOW2();
}

void CurveTriquadratic::setCoefficient7z(double coefficient7z) {
  getImpl<detail::CurveTriquadratic_Impl>()->setCoefficient7z(coefficient7z);
}

void CurveTriquadratic::resetCoefficient7z() {
  getImpl<detail::CurveTriquadratic_Impl>()->resetCoefficient7z();
}

void CurveTriquadratic::setCoefficient8xPOW2TIMESYPOW2(double coefficient8xPOW2TIMESYPOW2) {
  getImpl<detail::CurveTriquadratic_Impl>()->setCoefficient8xPOW2TIMESYPOW2(coefficient8xPOW2TIMESYPOW2);
}

void CurveTriquadratic::resetCoefficient8xPOW2TIMESYPOW2() {
  getImpl<detail::CurveTriquadratic_Impl>()->resetCoefficient8xPOW2TIMESYPOW2();
}

void CurveTriquadratic::setCoefficient9xTIMESY(double coefficient9xTIMESY) {
  getImpl<detail::CurveTriquadratic_Impl>()->setCoefficient9xTIMESY(coefficient9xTIMESY);
}

void CurveTriquadratic::resetCoefficient9xTIMESY() {
  getImpl<detail::CurveTriquadratic_Impl>()->resetCoefficient9xTIMESY();
}

void CurveTriquadratic::setCoefficient10xTIMESYPOW2(double coefficient10xTIMESYPOW2) {
  getImpl<detail::CurveTriquadratic_Impl>()->setCoefficient10xTIMESYPOW2(coefficient10xTIMESYPOW2);
}

void CurveTriquadratic::resetCoefficient10xTIMESYPOW2() {
  getImpl<detail::CurveTriquadratic_Impl>()->resetCoefficient10xTIMESYPOW2();
}

void CurveTriquadratic::setCoefficient11xPOW2TIMESY(double coefficient11xPOW2TIMESY) {
  getImpl<detail::CurveTriquadratic_Impl>()->setCoefficient11xPOW2TIMESY(coefficient11xPOW2TIMESY);
}

void CurveTriquadratic::resetCoefficient11xPOW2TIMESY() {
  getImpl<detail::CurveTriquadratic_Impl>()->resetCoefficient11xPOW2TIMESY();
}

void CurveTriquadratic::setCoefficient12xPOW2TIMESZPOW2(double coefficient12xPOW2TIMESZPOW2) {
  getImpl<detail::CurveTriquadratic_Impl>()->setCoefficient12xPOW2TIMESZPOW2(coefficient12xPOW2TIMESZPOW2);
}

void CurveTriquadratic::resetCoefficient12xPOW2TIMESZPOW2() {
  getImpl<detail::CurveTriquadratic_Impl>()->resetCoefficient12xPOW2TIMESZPOW2();
}

void CurveTriquadratic::setCoefficient13xTIMESZ(double coefficient13xTIMESZ) {
  getImpl<detail::CurveTriquadratic_Impl>()->setCoefficient13xTIMESZ(coefficient13xTIMESZ);
}

void CurveTriquadratic::resetCoefficient13xTIMESZ() {
  getImpl<detail::CurveTriquadratic_Impl>()->resetCoefficient13xTIMESZ();
}

void CurveTriquadratic::setCoefficient14xTIMESZPOW2(double coefficient14xTIMESZPOW2) {
  getImpl<detail::CurveTriquadratic_Impl>()->setCoefficient14xTIMESZPOW2(coefficient14xTIMESZPOW2);
}

void CurveTriquadratic::resetCoefficient14xTIMESZPOW2() {
  getImpl<detail::CurveTriquadratic_Impl>()->resetCoefficient14xTIMESZPOW2();
}

void CurveTriquadratic::setCoefficient15xPOW2TIMESZ(double coefficient15xPOW2TIMESZ) {
  getImpl<detail::CurveTriquadratic_Impl>()->setCoefficient15xPOW2TIMESZ(coefficient15xPOW2TIMESZ);
}

void CurveTriquadratic::resetCoefficient15xPOW2TIMESZ() {
  getImpl<detail::CurveTriquadratic_Impl>()->resetCoefficient15xPOW2TIMESZ();
}

void CurveTriquadratic::setCoefficient16yPOW2TIMESZPOW2(double coefficient16yPOW2TIMESZPOW2) {
  getImpl<detail::CurveTriquadratic_Impl>()->setCoefficient16yPOW2TIMESZPOW2(coefficient16yPOW2TIMESZPOW2);
}

void CurveTriquadratic::resetCoefficient16yPOW2TIMESZPOW2() {
  getImpl<detail::CurveTriquadratic_Impl>()->resetCoefficient16yPOW2TIMESZPOW2();
}

void CurveTriquadratic::setCoefficient17yTIMESZ(double coefficient17yTIMESZ) {
  getImpl<detail::CurveTriquadratic_Impl>()->setCoefficient17yTIMESZ(coefficient17yTIMESZ);
}

void CurveTriquadratic::resetCoefficient17yTIMESZ() {
  getImpl<detail::CurveTriquadratic_Impl>()->resetCoefficient17yTIMESZ();
}

void CurveTriquadratic::setCoefficient18yTIMESZPOW2(double coefficient18yTIMESZPOW2) {
  getImpl<detail::CurveTriquadratic_Impl>()->setCoefficient18yTIMESZPOW2(coefficient18yTIMESZPOW2);
}

void CurveTriquadratic::resetCoefficient18yTIMESZPOW2() {
  getImpl<detail::CurveTriquadratic_Impl>()->resetCoefficient18yTIMESZPOW2();
}

void CurveTriquadratic::setCoefficient19yPOW2TIMESZ(double coefficient19yPOW2TIMESZ) {
  getImpl<detail::CurveTriquadratic_Impl>()->setCoefficient19yPOW2TIMESZ(coefficient19yPOW2TIMESZ);
}

void CurveTriquadratic::resetCoefficient19yPOW2TIMESZ() {
  getImpl<detail::CurveTriquadratic_Impl>()->resetCoefficient19yPOW2TIMESZ();
}

void CurveTriquadratic::setCoefficient20xPOW2TIMESYPOW2TIMESZPOW2(double coefficient20xPOW2TIMESYPOW2TIMESZPOW2) {
  getImpl<detail::CurveTriquadratic_Impl>()->setCoefficient20xPOW2TIMESYPOW2TIMESZPOW2(coefficient20xPOW2TIMESYPOW2TIMESZPOW2);
}

void CurveTriquadratic::resetCoefficient20xPOW2TIMESYPOW2TIMESZPOW2() {
  getImpl<detail::CurveTriquadratic_Impl>()->resetCoefficient20xPOW2TIMESYPOW2TIMESZPOW2();
}

void CurveTriquadratic::setCoefficient21xPOW2TIMESYPOW2TIMESZ(double coefficient21xPOW2TIMESYPOW2TIMESZ) {
  getImpl<detail::CurveTriquadratic_Impl>()->setCoefficient21xPOW2TIMESYPOW2TIMESZ(coefficient21xPOW2TIMESYPOW2TIMESZ);
}

void CurveTriquadratic::resetCoefficient21xPOW2TIMESYPOW2TIMESZ() {
  getImpl<detail::CurveTriquadratic_Impl>()->resetCoefficient21xPOW2TIMESYPOW2TIMESZ();
}

void CurveTriquadratic::setCoefficient22xPOW2TIMESYTIMESZPOW2(double coefficient22xPOW2TIMESYTIMESZPOW2) {
  getImpl<detail::CurveTriquadratic_Impl>()->setCoefficient22xPOW2TIMESYTIMESZPOW2(coefficient22xPOW2TIMESYTIMESZPOW2);
}

void CurveTriquadratic::resetCoefficient22xPOW2TIMESYTIMESZPOW2() {
  getImpl<detail::CurveTriquadratic_Impl>()->resetCoefficient22xPOW2TIMESYTIMESZPOW2();
}

void CurveTriquadratic::setCoefficient23xTIMESYPOW2TIMESZPOW2(double coefficient23xTIMESYPOW2TIMESZPOW2) {
  getImpl<detail::CurveTriquadratic_Impl>()->setCoefficient23xTIMESYPOW2TIMESZPOW2(coefficient23xTIMESYPOW2TIMESZPOW2);
}

void CurveTriquadratic::resetCoefficient23xTIMESYPOW2TIMESZPOW2() {
  getImpl<detail::CurveTriquadratic_Impl>()->resetCoefficient23xTIMESYPOW2TIMESZPOW2();
}

void CurveTriquadratic::setCoefficient24xPOW2TIMESYTIMESZ(double coefficient24xPOW2TIMESYTIMESZ) {
  getImpl<detail::CurveTriquadratic_Impl>()->setCoefficient24xPOW2TIMESYTIMESZ(coefficient24xPOW2TIMESYTIMESZ);
}

void CurveTriquadratic::resetCoefficient24xPOW2TIMESYTIMESZ() {
  getImpl<detail::CurveTriquadratic_Impl>()->resetCoefficient24xPOW2TIMESYTIMESZ();
}

void CurveTriquadratic::setCoefficient25xTIMESYPOW2TIMESZ(double coefficient25xTIMESYPOW2TIMESZ) {
  getImpl<detail::CurveTriquadratic_Impl>()->setCoefficient25xTIMESYPOW2TIMESZ(coefficient25xTIMESYPOW2TIMESZ);
}

void CurveTriquadratic::resetCoefficient25xTIMESYPOW2TIMESZ() {
  getImpl<detail::CurveTriquadratic_Impl>()->resetCoefficient25xTIMESYPOW2TIMESZ();
}

void CurveTriquadratic::setCoefficient26xTIMESYTIMESZPOW2(double coefficient26xTIMESYTIMESZPOW2) {
  getImpl<detail::CurveTriquadratic_Impl>()->setCoefficient26xTIMESYTIMESZPOW2(coefficient26xTIMESYTIMESZPOW2);
}

void CurveTriquadratic::resetCoefficient26xTIMESYTIMESZPOW2() {
  getImpl<detail::CurveTriquadratic_Impl>()->resetCoefficient26xTIMESYTIMESZPOW2();
}

void CurveTriquadratic::setCoefficient27xTIMESYTIMESZ(double coefficient27xTIMESYTIMESZ) {
  getImpl<detail::CurveTriquadratic_Impl>()->setCoefficient27xTIMESYTIMESZ(coefficient27xTIMESYTIMESZ);
}

void CurveTriquadratic::resetCoefficient27xTIMESYTIMESZ() {
  getImpl<detail::CurveTriquadratic_Impl>()->resetCoefficient27xTIMESYTIMESZ();
}

void CurveTriquadratic::setMinimumValueofx(double minimumValueofx) {
  getImpl<detail::CurveTriquadratic_Impl>()->setMinimumValueofx(minimumValueofx);
}

void CurveTriquadratic::resetMinimumValueofx() {
  getImpl<detail::CurveTriquadratic_Impl>()->resetMinimumValueofx();
}

void CurveTriquadratic::setMaximumValueofx(double maximumValueofx) {
  getImpl<detail::CurveTriquadratic_Impl>()->setMaximumValueofx(maximumValueofx);
}

void CurveTriquadratic::resetMaximumValueofx() {
  getImpl<detail::CurveTriquadratic_Impl>()->resetMaximumValueofx();
}

void CurveTriquadratic::setMinimumValueofy(double minimumValueofy) {
  getImpl<detail::CurveTriquadratic_Impl>()->setMinimumValueofy(minimumValueofy);
}

void CurveTriquadratic::resetMinimumValueofy() {
  getImpl<detail::CurveTriquadratic_Impl>()->resetMinimumValueofy();
}

void CurveTriquadratic::setMaximumValueofy(double maximumValueofy) {
  getImpl<detail::CurveTriquadratic_Impl>()->setMaximumValueofy(maximumValueofy);
}

void CurveTriquadratic::resetMaximumValueofy() {
  getImpl<detail::CurveTriquadratic_Impl>()->resetMaximumValueofy();
}

void CurveTriquadratic::setMinimumValueofz(double minimumValueofz) {
  getImpl<detail::CurveTriquadratic_Impl>()->setMinimumValueofz(minimumValueofz);
}

void CurveTriquadratic::resetMinimumValueofz() {
  getImpl<detail::CurveTriquadratic_Impl>()->resetMinimumValueofz();
}

void CurveTriquadratic::setMaximumValueofz(double maximumValueofz) {
  getImpl<detail::CurveTriquadratic_Impl>()->setMaximumValueofz(maximumValueofz);
}

void CurveTriquadratic::resetMaximumValueofz() {
  getImpl<detail::CurveTriquadratic_Impl>()->resetMaximumValueofz();
}

void CurveTriquadratic::setMinimumCurveOutput(double minimumCurveOutput) {
  getImpl<detail::CurveTriquadratic_Impl>()->setMinimumCurveOutput(minimumCurveOutput);
}

void CurveTriquadratic::resetMinimumCurveOutput() {
  getImpl<detail::CurveTriquadratic_Impl>()->resetMinimumCurveOutput();
}

void CurveTriquadratic::setMaximumCurveOutput(double maximumCurveOutput) {
  getImpl<detail::CurveTriquadratic_Impl>()->setMaximumCurveOutput(maximumCurveOutput);
}

void CurveTriquadratic::resetMaximumCurveOutput() {
  getImpl<detail::CurveTriquadratic_Impl>()->resetMaximumCurveOutput();
}

bool CurveTriquadratic::setInputUnitTypeforX(std::string inputUnitTypeforX) {
  return getImpl<detail::CurveTriquadratic_Impl>()->setInputUnitTypeforX(inputUnitTypeforX);
}

void CurveTriquadratic::resetInputUnitTypeforX() {
  getImpl<detail::CurveTriquadratic_Impl>()->resetInputUnitTypeforX();
}

bool CurveTriquadratic::setInputUnitTypeforY(std::string inputUnitTypeforY) {
  return getImpl<detail::CurveTriquadratic_Impl>()->setInputUnitTypeforY(inputUnitTypeforY);
}

void CurveTriquadratic::resetInputUnitTypeforY() {
  getImpl<detail::CurveTriquadratic_Impl>()->resetInputUnitTypeforY();
}

bool CurveTriquadratic::setInputUnitTypeforZ(std::string inputUnitTypeforZ) {
  return getImpl<detail::CurveTriquadratic_Impl>()->setInputUnitTypeforZ(inputUnitTypeforZ);
}

void CurveTriquadratic::resetInputUnitTypeforZ() {
  getImpl<detail::CurveTriquadratic_Impl>()->resetInputUnitTypeforZ();
}

bool CurveTriquadratic::setOutputUnitType(std::string outputUnitType) {
  return getImpl<detail::CurveTriquadratic_Impl>()->setOutputUnitType(outputUnitType);
}

void CurveTriquadratic::resetOutputUnitType() {
  getImpl<detail::CurveTriquadratic_Impl>()->resetOutputUnitType();
}

/// @cond
CurveTriquadratic::CurveTriquadratic(std::shared_ptr<detail::CurveTriquadratic_Impl> impl)
  : Curve(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

