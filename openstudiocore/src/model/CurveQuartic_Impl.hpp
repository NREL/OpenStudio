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

#ifndef MODEL_CURVEQUARTIC_IMPL_HPP
#define MODEL_CURVEQUARTIC_IMPL_HPP

#include "ModelAPI.hpp"
#include "Curve_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  /** CurveQuartic_Impl is a Curve_Impl that is the implementation class for CurveQuartic.*/
  class MODEL_API CurveQuartic_Impl : public Curve_Impl {
    Q_OBJECT;
    Q_PROPERTY(double coefficient1Constant READ coefficient1Constant WRITE setCoefficient1Constant);
    Q_PROPERTY(double coefficient2x READ coefficient2x WRITE setCoefficient2x);
    Q_PROPERTY(double coefficient3xPOW2 READ coefficient3xPOW2 WRITE setCoefficient3xPOW2);
    Q_PROPERTY(double coefficient4xPOW3 READ coefficient4xPOW3 WRITE setCoefficient4xPOW3);
    Q_PROPERTY(double coefficient5xPOW4 READ coefficient5xPOW4 WRITE setCoefficient5xPOW4);
    Q_PROPERTY(double minimumValueofx READ minimumValueofx WRITE setMinimumValueofx);
    Q_PROPERTY(double maximumValueofx READ maximumValueofx WRITE setMaximumValueofx);
    Q_PROPERTY(boost::optional<double> minimumCurveOutput READ minimumCurveOutput WRITE setMinimumCurveOutput RESET resetMinimumCurveOutput);
    Q_PROPERTY(boost::optional<double> maximumCurveOutput READ maximumCurveOutput WRITE setMaximumCurveOutput RESET resetMaximumCurveOutput);
    Q_PROPERTY(std::string inputUnitTypeforX READ inputUnitTypeforX WRITE setInputUnitTypeforX RESET resetInputUnitTypeforX);
    Q_PROPERTY(bool isInputUnitTypeforXDefaulted READ isInputUnitTypeforXDefaulted);
    Q_PROPERTY(std::string outputUnitType READ outputUnitType WRITE setOutputUnitType RESET resetOutputUnitType);
    Q_PROPERTY(bool isOutputUnitTypeDefaulted READ isOutputUnitTypeDefaulted);
   public:

    /** @name Constructors and Destructors */
    //@{

    CurveQuartic_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    CurveQuartic_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                      Model_Impl* model,
                      bool keepHandle);

    CurveQuartic_Impl(const CurveQuartic_Impl& other,
                      Model_Impl* model,
                      bool keepHandle);

    virtual ~CurveQuartic_Impl() {}

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

    double coefficient4xPOW3() const;

    double coefficient5xPOW4() const;

    double minimumValueofx() const;

    double maximumValueofx() const;

    boost::optional<double> minimumCurveOutput() const;

    boost::optional<double> maximumCurveOutput() const;

    std::string inputUnitTypeforX() const;

    bool isInputUnitTypeforXDefaulted() const;

    std::string outputUnitType() const;

    bool isOutputUnitTypeDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    void setCoefficient1Constant(double coefficient1Constant);

    void setCoefficient2x(double coefficient2x);

    void setCoefficient3xPOW2(double coefficient3xPOW2);

    void setCoefficient4xPOW3(double coefficient4xPOW3);

    void setCoefficient5xPOW4(double coefficient5xPOW4);

    void setMinimumValueofx(double minimumValueofx);

    void setMaximumValueofx(double maximumValueofx);

    void setMinimumCurveOutput(boost::optional<double> minimumCurveOutput);

    void resetMinimumCurveOutput();

    void setMaximumCurveOutput(boost::optional<double> maximumCurveOutput);

    void resetMaximumCurveOutput();

    bool setInputUnitTypeforX(std::string inputUnitTypeforX);

    void resetInputUnitTypeforX();

    bool setOutputUnitType(std::string outputUnitType);

    void resetOutputUnitType();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CurveQuartic");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_CURVEQUARTIC_IMPL_HPP

