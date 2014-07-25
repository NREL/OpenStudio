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

#ifndef MODEL_CURVETRIQUADRATIC_IMPL_HPP
#define MODEL_CURVETRIQUADRATIC_IMPL_HPP

#include "ModelAPI.hpp"
#include "Curve_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  /** CurveTriquadratic_Impl is a Curve_Impl that is the implementation class for CurveTriquadratic.*/
  class MODEL_API CurveTriquadratic_Impl : public Curve_Impl {
    Q_OBJECT;
    Q_PROPERTY(boost::optional<double> coefficient1Constant READ coefficient1Constant WRITE setCoefficient1Constant RESET resetCoefficient1Constant);
    Q_PROPERTY(boost::optional<double> coefficient2xPOW2 READ coefficient2xPOW2 WRITE setCoefficient2xPOW2 RESET resetCoefficient2xPOW2);
    Q_PROPERTY(boost::optional<double> coefficient3x READ coefficient3x WRITE setCoefficient3x RESET resetCoefficient3x);
    Q_PROPERTY(boost::optional<double> coefficient4yPOW2 READ coefficient4yPOW2 WRITE setCoefficient4yPOW2 RESET resetCoefficient4yPOW2);
    Q_PROPERTY(boost::optional<double> coefficient5y READ coefficient5y WRITE setCoefficient5y RESET resetCoefficient5y);
    Q_PROPERTY(boost::optional<double> coefficient6zPOW2 READ coefficient6zPOW2 WRITE setCoefficient6zPOW2 RESET resetCoefficient6zPOW2);
    Q_PROPERTY(boost::optional<double> coefficient7z READ coefficient7z WRITE setCoefficient7z RESET resetCoefficient7z);
    Q_PROPERTY(boost::optional<double> coefficient8xPOW2TIMESYPOW2 READ coefficient8xPOW2TIMESYPOW2 WRITE setCoefficient8xPOW2TIMESYPOW2 RESET resetCoefficient8xPOW2TIMESYPOW2);
    Q_PROPERTY(boost::optional<double> coefficient9xTIMESY READ coefficient9xTIMESY WRITE setCoefficient9xTIMESY RESET resetCoefficient9xTIMESY);
    Q_PROPERTY(boost::optional<double> coefficient10xTIMESYPOW2 READ coefficient10xTIMESYPOW2 WRITE setCoefficient10xTIMESYPOW2 RESET resetCoefficient10xTIMESYPOW2);
    Q_PROPERTY(boost::optional<double> coefficient11xPOW2TIMESY READ coefficient11xPOW2TIMESY WRITE setCoefficient11xPOW2TIMESY RESET resetCoefficient11xPOW2TIMESY);
    Q_PROPERTY(boost::optional<double> coefficient12xPOW2TIMESZPOW2 READ coefficient12xPOW2TIMESZPOW2 WRITE setCoefficient12xPOW2TIMESZPOW2 RESET resetCoefficient12xPOW2TIMESZPOW2);
    Q_PROPERTY(boost::optional<double> coefficient13xTIMESZ READ coefficient13xTIMESZ WRITE setCoefficient13xTIMESZ RESET resetCoefficient13xTIMESZ);
    Q_PROPERTY(boost::optional<double> coefficient14xTIMESZPOW2 READ coefficient14xTIMESZPOW2 WRITE setCoefficient14xTIMESZPOW2 RESET resetCoefficient14xTIMESZPOW2);
    Q_PROPERTY(boost::optional<double> coefficient15xPOW2TIMESZ READ coefficient15xPOW2TIMESZ WRITE setCoefficient15xPOW2TIMESZ RESET resetCoefficient15xPOW2TIMESZ);
    Q_PROPERTY(boost::optional<double> coefficient16yPOW2TIMESZPOW2 READ coefficient16yPOW2TIMESZPOW2 WRITE setCoefficient16yPOW2TIMESZPOW2 RESET resetCoefficient16yPOW2TIMESZPOW2);
    Q_PROPERTY(boost::optional<double> coefficient17yTIMESZ READ coefficient17yTIMESZ WRITE setCoefficient17yTIMESZ RESET resetCoefficient17yTIMESZ);
    Q_PROPERTY(boost::optional<double> coefficient18yTIMESZPOW2 READ coefficient18yTIMESZPOW2 WRITE setCoefficient18yTIMESZPOW2 RESET resetCoefficient18yTIMESZPOW2);
    Q_PROPERTY(boost::optional<double> coefficient19yPOW2TIMESZ READ coefficient19yPOW2TIMESZ WRITE setCoefficient19yPOW2TIMESZ RESET resetCoefficient19yPOW2TIMESZ);
    Q_PROPERTY(boost::optional<double> coefficient20xPOW2TIMESYPOW2TIMESZPOW2 READ coefficient20xPOW2TIMESYPOW2TIMESZPOW2 WRITE setCoefficient20xPOW2TIMESYPOW2TIMESZPOW2 RESET resetCoefficient20xPOW2TIMESYPOW2TIMESZPOW2);
    Q_PROPERTY(boost::optional<double> coefficient21xPOW2TIMESYPOW2TIMESZ READ coefficient21xPOW2TIMESYPOW2TIMESZ WRITE setCoefficient21xPOW2TIMESYPOW2TIMESZ RESET resetCoefficient21xPOW2TIMESYPOW2TIMESZ);
    Q_PROPERTY(boost::optional<double> coefficient22xPOW2TIMESYTIMESZPOW2 READ coefficient22xPOW2TIMESYTIMESZPOW2 WRITE setCoefficient22xPOW2TIMESYTIMESZPOW2 RESET resetCoefficient22xPOW2TIMESYTIMESZPOW2);
    Q_PROPERTY(boost::optional<double> coefficient23xTIMESYPOW2TIMESZPOW2 READ coefficient23xTIMESYPOW2TIMESZPOW2 WRITE setCoefficient23xTIMESYPOW2TIMESZPOW2 RESET resetCoefficient23xTIMESYPOW2TIMESZPOW2);
    Q_PROPERTY(boost::optional<double> coefficient24xPOW2TIMESYTIMESZ READ coefficient24xPOW2TIMESYTIMESZ WRITE setCoefficient24xPOW2TIMESYTIMESZ RESET resetCoefficient24xPOW2TIMESYTIMESZ);
    Q_PROPERTY(boost::optional<double> coefficient25xTIMESYPOW2TIMESZ READ coefficient25xTIMESYPOW2TIMESZ WRITE setCoefficient25xTIMESYPOW2TIMESZ RESET resetCoefficient25xTIMESYPOW2TIMESZ);
    Q_PROPERTY(boost::optional<double> coefficient26xTIMESYTIMESZPOW2 READ coefficient26xTIMESYTIMESZPOW2 WRITE setCoefficient26xTIMESYTIMESZPOW2 RESET resetCoefficient26xTIMESYTIMESZPOW2);
    Q_PROPERTY(boost::optional<double> coefficient27xTIMESYTIMESZ READ coefficient27xTIMESYTIMESZ WRITE setCoefficient27xTIMESYTIMESZ RESET resetCoefficient27xTIMESYTIMESZ);
    Q_PROPERTY(boost::optional<double> minimumValueofx READ minimumValueofx WRITE setMinimumValueofx RESET resetMinimumValueofx);
    Q_PROPERTY(boost::optional<double> maximumValueofx READ maximumValueofx WRITE setMaximumValueofx RESET resetMaximumValueofx);
    Q_PROPERTY(boost::optional<double> minimumValueofy READ minimumValueofy WRITE setMinimumValueofy RESET resetMinimumValueofy);
    Q_PROPERTY(boost::optional<double> maximumValueofy READ maximumValueofy WRITE setMaximumValueofy RESET resetMaximumValueofy);
    Q_PROPERTY(boost::optional<double> minimumValueofz READ minimumValueofz WRITE setMinimumValueofz RESET resetMinimumValueofz);
    Q_PROPERTY(boost::optional<double> maximumValueofz READ maximumValueofz WRITE setMaximumValueofz RESET resetMaximumValueofz);
    Q_PROPERTY(boost::optional<double> minimumCurveOutput READ minimumCurveOutput WRITE setMinimumCurveOutput RESET resetMinimumCurveOutput);
    Q_PROPERTY(boost::optional<double> maximumCurveOutput READ maximumCurveOutput WRITE setMaximumCurveOutput RESET resetMaximumCurveOutput);
    Q_PROPERTY(std::string inputUnitTypeforX READ inputUnitTypeforX WRITE setInputUnitTypeforX RESET resetInputUnitTypeforX);
    Q_PROPERTY(bool isInputUnitTypeforXDefaulted READ isInputUnitTypeforXDefaulted);
    Q_PROPERTY(std::string inputUnitTypeforY READ inputUnitTypeforY WRITE setInputUnitTypeforY RESET resetInputUnitTypeforY);
    Q_PROPERTY(bool isInputUnitTypeforYDefaulted READ isInputUnitTypeforYDefaulted);
    Q_PROPERTY(std::string inputUnitTypeforZ READ inputUnitTypeforZ WRITE setInputUnitTypeforZ RESET resetInputUnitTypeforZ);
    Q_PROPERTY(bool isInputUnitTypeforZDefaulted READ isInputUnitTypeforZDefaulted);
    Q_PROPERTY(std::string outputUnitType READ outputUnitType WRITE setOutputUnitType RESET resetOutputUnitType);
    Q_PROPERTY(bool isOutputUnitTypeDefaulted READ isOutputUnitTypeDefaulted);
   public:

    /** @name Constructors and Destructors */
    //@{

    CurveTriquadratic_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    CurveTriquadratic_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                           Model_Impl* model,
                           bool keepHandle);

    CurveTriquadratic_Impl(const CurveTriquadratic_Impl& other,
                           Model_Impl* model,
                           bool keepHandle);

    virtual ~CurveTriquadratic_Impl() {}

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

    boost::optional<double> coefficient1Constant() const;

    boost::optional<double> coefficient2xPOW2() const;

    boost::optional<double> coefficient3x() const;

    boost::optional<double> coefficient4yPOW2() const;

    boost::optional<double> coefficient5y() const;

    boost::optional<double> coefficient6zPOW2() const;

    boost::optional<double> coefficient7z() const;

    boost::optional<double> coefficient8xPOW2TIMESYPOW2() const;

    boost::optional<double> coefficient9xTIMESY() const;

    boost::optional<double> coefficient10xTIMESYPOW2() const;

    boost::optional<double> coefficient11xPOW2TIMESY() const;

    boost::optional<double> coefficient12xPOW2TIMESZPOW2() const;

    boost::optional<double> coefficient13xTIMESZ() const;

    boost::optional<double> coefficient14xTIMESZPOW2() const;

    boost::optional<double> coefficient15xPOW2TIMESZ() const;

    boost::optional<double> coefficient16yPOW2TIMESZPOW2() const;

    boost::optional<double> coefficient17yTIMESZ() const;

    boost::optional<double> coefficient18yTIMESZPOW2() const;

    boost::optional<double> coefficient19yPOW2TIMESZ() const;

    boost::optional<double> coefficient20xPOW2TIMESYPOW2TIMESZPOW2() const;

    boost::optional<double> coefficient21xPOW2TIMESYPOW2TIMESZ() const;

    boost::optional<double> coefficient22xPOW2TIMESYTIMESZPOW2() const;

    boost::optional<double> coefficient23xTIMESYPOW2TIMESZPOW2() const;

    boost::optional<double> coefficient24xPOW2TIMESYTIMESZ() const;

    boost::optional<double> coefficient25xTIMESYPOW2TIMESZ() const;

    boost::optional<double> coefficient26xTIMESYTIMESZPOW2() const;

    boost::optional<double> coefficient27xTIMESYTIMESZ() const;

    boost::optional<double> minimumValueofx() const;

    boost::optional<double> maximumValueofx() const;

    boost::optional<double> minimumValueofy() const;

    boost::optional<double> maximumValueofy() const;

    boost::optional<double> minimumValueofz() const;

    boost::optional<double> maximumValueofz() const;

    boost::optional<double> minimumCurveOutput() const;

    boost::optional<double> maximumCurveOutput() const;

    std::string inputUnitTypeforX() const;

    bool isInputUnitTypeforXDefaulted() const;

    std::string inputUnitTypeforY() const;

    bool isInputUnitTypeforYDefaulted() const;

    std::string inputUnitTypeforZ() const;

    bool isInputUnitTypeforZDefaulted() const;

    std::string outputUnitType() const;

    bool isOutputUnitTypeDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    void setCoefficient1Constant(boost::optional<double> coefficient1Constant);

    void resetCoefficient1Constant();

    void setCoefficient2xPOW2(boost::optional<double> coefficient2xPOW2);

    void resetCoefficient2xPOW2();

    void setCoefficient3x(boost::optional<double> coefficient3x);

    void resetCoefficient3x();

    void setCoefficient4yPOW2(boost::optional<double> coefficient4yPOW2);

    void resetCoefficient4yPOW2();

    void setCoefficient5y(boost::optional<double> coefficient5y);

    void resetCoefficient5y();

    void setCoefficient6zPOW2(boost::optional<double> coefficient6zPOW2);

    void resetCoefficient6zPOW2();

    void setCoefficient7z(boost::optional<double> coefficient7z);

    void resetCoefficient7z();

    void setCoefficient8xPOW2TIMESYPOW2(boost::optional<double> coefficient8xPOW2TIMESYPOW2);

    void resetCoefficient8xPOW2TIMESYPOW2();

    void setCoefficient9xTIMESY(boost::optional<double> coefficient9xTIMESY);

    void resetCoefficient9xTIMESY();

    void setCoefficient10xTIMESYPOW2(boost::optional<double> coefficient10xTIMESYPOW2);

    void resetCoefficient10xTIMESYPOW2();

    void setCoefficient11xPOW2TIMESY(boost::optional<double> coefficient11xPOW2TIMESY);

    void resetCoefficient11xPOW2TIMESY();

    void setCoefficient12xPOW2TIMESZPOW2(boost::optional<double> coefficient12xPOW2TIMESZPOW2);

    void resetCoefficient12xPOW2TIMESZPOW2();

    void setCoefficient13xTIMESZ(boost::optional<double> coefficient13xTIMESZ);

    void resetCoefficient13xTIMESZ();

    void setCoefficient14xTIMESZPOW2(boost::optional<double> coefficient14xTIMESZPOW2);

    void resetCoefficient14xTIMESZPOW2();

    void setCoefficient15xPOW2TIMESZ(boost::optional<double> coefficient15xPOW2TIMESZ);

    void resetCoefficient15xPOW2TIMESZ();

    void setCoefficient16yPOW2TIMESZPOW2(boost::optional<double> coefficient16yPOW2TIMESZPOW2);

    void resetCoefficient16yPOW2TIMESZPOW2();

    void setCoefficient17yTIMESZ(boost::optional<double> coefficient17yTIMESZ);

    void resetCoefficient17yTIMESZ();

    void setCoefficient18yTIMESZPOW2(boost::optional<double> coefficient18yTIMESZPOW2);

    void resetCoefficient18yTIMESZPOW2();

    void setCoefficient19yPOW2TIMESZ(boost::optional<double> coefficient19yPOW2TIMESZ);

    void resetCoefficient19yPOW2TIMESZ();

    void setCoefficient20xPOW2TIMESYPOW2TIMESZPOW2(boost::optional<double> coefficient20xPOW2TIMESYPOW2TIMESZPOW2);

    void resetCoefficient20xPOW2TIMESYPOW2TIMESZPOW2();

    void setCoefficient21xPOW2TIMESYPOW2TIMESZ(boost::optional<double> coefficient21xPOW2TIMESYPOW2TIMESZ);

    void resetCoefficient21xPOW2TIMESYPOW2TIMESZ();

    void setCoefficient22xPOW2TIMESYTIMESZPOW2(boost::optional<double> coefficient22xPOW2TIMESYTIMESZPOW2);

    void resetCoefficient22xPOW2TIMESYTIMESZPOW2();

    void setCoefficient23xTIMESYPOW2TIMESZPOW2(boost::optional<double> coefficient23xTIMESYPOW2TIMESZPOW2);

    void resetCoefficient23xTIMESYPOW2TIMESZPOW2();

    void setCoefficient24xPOW2TIMESYTIMESZ(boost::optional<double> coefficient24xPOW2TIMESYTIMESZ);

    void resetCoefficient24xPOW2TIMESYTIMESZ();

    void setCoefficient25xTIMESYPOW2TIMESZ(boost::optional<double> coefficient25xTIMESYPOW2TIMESZ);

    void resetCoefficient25xTIMESYPOW2TIMESZ();

    void setCoefficient26xTIMESYTIMESZPOW2(boost::optional<double> coefficient26xTIMESYTIMESZPOW2);

    void resetCoefficient26xTIMESYTIMESZPOW2();

    void setCoefficient27xTIMESYTIMESZ(boost::optional<double> coefficient27xTIMESYTIMESZ);

    void resetCoefficient27xTIMESYTIMESZ();

    void setMinimumValueofx(boost::optional<double> minimumValueofx);

    void resetMinimumValueofx();

    void setMaximumValueofx(boost::optional<double> maximumValueofx);

    void resetMaximumValueofx();

    void setMinimumValueofy(boost::optional<double> minimumValueofy);

    void resetMinimumValueofy();

    void setMaximumValueofy(boost::optional<double> maximumValueofy);

    void resetMaximumValueofy();

    void setMinimumValueofz(boost::optional<double> minimumValueofz);

    void resetMinimumValueofz();

    void setMaximumValueofz(boost::optional<double> maximumValueofz);

    void resetMaximumValueofz();

    void setMinimumCurveOutput(boost::optional<double> minimumCurveOutput);

    void resetMinimumCurveOutput();

    void setMaximumCurveOutput(boost::optional<double> maximumCurveOutput);

    void resetMaximumCurveOutput();

    bool setInputUnitTypeforX(std::string inputUnitTypeforX);

    void resetInputUnitTypeforX();

    bool setInputUnitTypeforY(std::string inputUnitTypeforY);

    void resetInputUnitTypeforY();

    bool setInputUnitTypeforZ(std::string inputUnitTypeforZ);

    void resetInputUnitTypeforZ();

    bool setOutputUnitType(std::string outputUnitType);

    void resetOutputUnitType();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CurveTriquadratic");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_CURVETRIQUADRATIC_IMPL_HPP

