/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#ifndef MODEL_CURVEDOUBLEEXPONENTIALDECAY_IMPL_HPP
#define MODEL_CURVEDOUBLEEXPONENTIALDECAY_IMPL_HPP

#include "ModelAPI.hpp"
#include "Curve_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  /** CurveDoubleExponentialDecay_Impl is a Curve_Impl that is the implementation class for CurveDoubleExponentialDecay.*/
  class MODEL_API CurveDoubleExponentialDecay_Impl : public Curve_Impl {
    Q_OBJECT;
    Q_PROPERTY(double coefficient1C1 READ coefficient1C1 WRITE setCoefficient1C1);
    Q_PROPERTY(double coefficient2C2 READ coefficient2C2 WRITE setCoefficient2C2);
    Q_PROPERTY(double coefficient3C3 READ coefficient3C3 WRITE setCoefficient3C3);
    Q_PROPERTY(double coefficient3C4 READ coefficient3C4 WRITE setCoefficient3C4);
    Q_PROPERTY(double coefficient3C5 READ coefficient3C5 WRITE setCoefficient3C5);
    Q_PROPERTY(double minimumValueofx READ minimumValueofx WRITE setMinimumValueofx);
    Q_PROPERTY(double maximumValueofx READ maximumValueofx WRITE setMaximumValueofx);
    Q_PROPERTY(boost::optional<double> minimumCurveOutput READ minimumCurveOutput WRITE setMinimumCurveOutput RESET resetMinimumCurveOutput);
    Q_PROPERTY(boost::optional<double> maximumCurveOutput READ maximumCurveOutput WRITE setMaximumCurveOutput RESET resetMaximumCurveOutput);
    Q_PROPERTY(std::string inputUnitTypeforx READ inputUnitTypeforx WRITE setInputUnitTypeforx RESET resetInputUnitTypeforx);
    Q_PROPERTY(bool isInputUnitTypeforxDefaulted READ isInputUnitTypeforxDefaulted);
    Q_PROPERTY(std::string outputUnitType READ outputUnitType WRITE setOutputUnitType RESET resetOutputUnitType);
    Q_PROPERTY(bool isOutputUnitTypeDefaulted READ isOutputUnitTypeDefaulted);
   public:

    /** @name Constructors and Destructors */
    //@{

    CurveDoubleExponentialDecay_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    CurveDoubleExponentialDecay_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle);

    CurveDoubleExponentialDecay_Impl(const CurveDoubleExponentialDecay_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle);

    virtual ~CurveDoubleExponentialDecay_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual int numVariables() const;

    virtual double evaluate(const std::vector<double>& x) const;

    //@}
    /** @name Getters */
    //@{

    double coefficient1C1() const;

    double coefficient2C2() const;

    double coefficient3C3() const;

    double coefficient3C4() const;

    double coefficient3C5() const;

    double minimumValueofx() const;

    double maximumValueofx() const;

    boost::optional<double> minimumCurveOutput() const;

    boost::optional<double> maximumCurveOutput() const;

    std::string inputUnitTypeforx() const;

    bool isInputUnitTypeforxDefaulted() const;

    std::string outputUnitType() const;

    bool isOutputUnitTypeDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    void setCoefficient1C1(double coefficient1C1);

    void setCoefficient2C2(double coefficient2C2);

    void setCoefficient3C3(double coefficient3C3);

    void setCoefficient3C4(double coefficient3C4);

    void setCoefficient3C5(double coefficient3C5);

    void setMinimumValueofx(double minimumValueofx);

    void setMaximumValueofx(double maximumValueofx);

    void setMinimumCurveOutput(boost::optional<double> minimumCurveOutput);

    void resetMinimumCurveOutput();

    void setMaximumCurveOutput(boost::optional<double> maximumCurveOutput);

    void resetMaximumCurveOutput();

    bool setInputUnitTypeforx(std::string inputUnitTypeforx);

    void resetInputUnitTypeforx();

    bool setOutputUnitType(std::string outputUnitType);

    void resetOutputUnitType();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CurveDoubleExponentialDecay");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_CURVEDOUBLEEXPONENTIALDECAY_IMPL_HPP

