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

#ifndef MODEL_CURVEEXPONENT_IMPL_HPP
#define MODEL_CURVEEXPONENT_IMPL_HPP

#include "ModelAPI.hpp"
#include "Curve_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  /** CurveExponent_Impl is a Curve_Impl that is the implementation class for CurveExponent.*/
  class MODEL_API CurveExponent_Impl : public Curve_Impl {
    Q_OBJECT;
    Q_PROPERTY(double coefficient1Constant READ coefficient1Constant WRITE setCoefficient1Constant);
    Q_PROPERTY(double coefficient2Constant READ coefficient2Constant WRITE setCoefficient2Constant);
    Q_PROPERTY(double coefficient3Constant READ coefficient3Constant WRITE setCoefficient3Constant);
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

    CurveExponent_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    CurveExponent_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                       Model_Impl* model,
                       bool keepHandle);

    CurveExponent_Impl(const CurveExponent_Impl& other,
                       Model_Impl* model,
                       bool keepHandle);

    virtual ~CurveExponent_Impl() {}

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

    double coefficient2Constant() const;

    double coefficient3Constant() const;

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

    void setCoefficient2Constant(double coefficient2Constant);

    void setCoefficient3Constant(double coefficient3Constant);

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
    REGISTER_LOGGER("openstudio.model.CurveExponent");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_CURVEEXPONENT_IMPL_HPP

