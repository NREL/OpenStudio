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

#ifndef MODEL_CURVEBICUBIC_IMPL_HPP
#define MODEL_CURVEBICUBIC_IMPL_HPP

#include "Curve_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  /** CurveBicubic_Impl is a Curve_Impl that is the implementation class for CurveBicubic.*/
  class MODEL_API CurveBicubic_Impl : public Curve_Impl {
    Q_OBJECT;
    Q_PROPERTY(double coefficient1Constant READ coefficient1Constant WRITE setCoefficient1Constant);
    Q_PROPERTY(double coefficient2x READ coefficient2x WRITE setCoefficient2x);
    Q_PROPERTY(double coefficient3xPOW2 READ coefficient3xPOW2 WRITE setCoefficient3xPOW2);
    Q_PROPERTY(double coefficient4y READ coefficient4y WRITE setCoefficient4y);
    Q_PROPERTY(double coefficient5yPOW2 READ coefficient5yPOW2 WRITE setCoefficient5yPOW2);
    Q_PROPERTY(double coefficient6xTIMESY READ coefficient6xTIMESY WRITE setCoefficient6xTIMESY);
    Q_PROPERTY(double coefficient7xPOW3 READ coefficient7xPOW3 WRITE setCoefficient7xPOW3);
    Q_PROPERTY(double coefficient8yPOW3 READ coefficient8yPOW3 WRITE setCoefficient8yPOW3);
    Q_PROPERTY(double coefficient9xPOW2TIMESY READ coefficient9xPOW2TIMESY WRITE setCoefficient9xPOW2TIMESY);
    Q_PROPERTY(double coefficient10xTIMESYPOW2 READ coefficient10xTIMESYPOW2 WRITE setCoefficient10xTIMESYPOW2);
    Q_PROPERTY(double minimumValueofx READ minimumValueofx WRITE setMinimumValueofx);
    Q_PROPERTY(double maximumValueofx READ maximumValueofx WRITE setMaximumValueofx);
    Q_PROPERTY(double minimumValueofy READ minimumValueofy WRITE setMinimumValueofy);
    Q_PROPERTY(double maximumValueofy READ maximumValueofy WRITE setMaximumValueofy);
    Q_PROPERTY(boost::optional<double> minimumCurveOutput READ minimumCurveOutput WRITE setMinimumCurveOutput RESET resetMinimumCurveOutput);
    Q_PROPERTY(boost::optional<double> maximumCurveOutput READ maximumCurveOutput WRITE setMaximumCurveOutput RESET resetMaximumCurveOutput);
    Q_PROPERTY(std::string inputUnitTypeforX READ inputUnitTypeforX WRITE setInputUnitTypeforX RESET resetInputUnitTypeforX);
    Q_PROPERTY(bool isInputUnitTypeforXDefaulted READ isInputUnitTypeforXDefaulted);
    Q_PROPERTY(std::string inputUnitTypeforY READ inputUnitTypeforY WRITE setInputUnitTypeforY RESET resetInputUnitTypeforY);
    Q_PROPERTY(bool isInputUnitTypeforYDefaulted READ isInputUnitTypeforYDefaulted);
    Q_PROPERTY(std::string outputUnitType READ outputUnitType WRITE setOutputUnitType RESET resetOutputUnitType);
    Q_PROPERTY(bool isOutputUnitTypeDefaulted READ isOutputUnitTypeDefaulted);
   public:

    /** @name Constructors and Destructors */
    //@{

    CurveBicubic_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    CurveBicubic_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                      Model_Impl* model,
                      bool keepHandle);

    CurveBicubic_Impl(const CurveBicubic_Impl& other,
                      Model_Impl* model,
                      bool keepHandle);

    virtual ~CurveBicubic_Impl() {}

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

    double coefficient1Constant() const;

    double coefficient2x() const;

    double coefficient3xPOW2() const;

    double coefficient4y() const;

    double coefficient5yPOW2() const;

    double coefficient6xTIMESY() const;

    double coefficient7xPOW3() const;

    double coefficient8yPOW3() const;

    double coefficient9xPOW2TIMESY() const;

    double coefficient10xTIMESYPOW2() const;

    double minimumValueofx() const;

    double maximumValueofx() const;

    double minimumValueofy() const;

    double maximumValueofy() const;

    boost::optional<double> minimumCurveOutput() const;

    boost::optional<double> maximumCurveOutput() const;

    std::string inputUnitTypeforX() const;

    bool isInputUnitTypeforXDefaulted() const;

    std::string inputUnitTypeforY() const;

    bool isInputUnitTypeforYDefaulted() const;

    std::string outputUnitType() const;

    bool isOutputUnitTypeDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    void setCoefficient1Constant(double coefficient1Constant);

    void setCoefficient2x(double coefficient2x);

    void setCoefficient3xPOW2(double coefficient3xPOW2);

    void setCoefficient4y(double coefficient4y);

    void setCoefficient5yPOW2(double coefficient5yPOW2);

    void setCoefficient6xTIMESY(double coefficient6xTIMESY);

    void setCoefficient7xPOW3(double coefficient7xPOW3);

    void setCoefficient8yPOW3(double coefficient8yPOW3);

    void setCoefficient9xPOW2TIMESY(double coefficient9xPOW2TIMESY);

    void setCoefficient10xTIMESYPOW2(double coefficient10xTIMESYPOW2);

    void setMinimumValueofx(double minimumValueofx);

    void setMaximumValueofx(double maximumValueofx);

    void setMinimumValueofy(double minimumValueofy);

    void setMaximumValueofy(double maximumValueofy);

    void setMinimumCurveOutput(boost::optional<double> minimumCurveOutput);

    void resetMinimumCurveOutput();

    void setMaximumCurveOutput(boost::optional<double> maximumCurveOutput);

    void resetMaximumCurveOutput();

    bool setInputUnitTypeforX(std::string inputUnitTypeforX);

    void resetInputUnitTypeforX();

    bool setInputUnitTypeforY(std::string inputUnitTypeforY);

    void resetInputUnitTypeforY();

    bool setOutputUnitType(std::string outputUnitType);

    void resetOutputUnitType();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CurveBicubic");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_CURVEBICUBIC_IMPL_HPP

