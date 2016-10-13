/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef MODEL_CURVERECTANGULARHYPERBOLA2_IMPL_HPP
#define MODEL_CURVERECTANGULARHYPERBOLA2_IMPL_HPP

#include "ModelAPI.hpp"
#include "Curve_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  /** CurveRectangularHyperbola2_Impl is a Curve_Impl that is the implementation class for CurveRectangularHyperbola2.*/
  class MODEL_API CurveRectangularHyperbola2_Impl : public Curve_Impl {
    Q_OBJECT;
    Q_PROPERTY(double coefficient1C1 READ coefficient1C1 WRITE setCoefficient1C1);
    Q_PROPERTY(double coefficient2C2 READ coefficient2C2 WRITE setCoefficient2C2);
    Q_PROPERTY(double coefficient3C3 READ coefficient3C3 WRITE setCoefficient3C3);
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

    CurveRectangularHyperbola2_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    CurveRectangularHyperbola2_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                    Model_Impl* model,
                                    bool keepHandle);

    CurveRectangularHyperbola2_Impl(const CurveRectangularHyperbola2_Impl& other,
                                    Model_Impl* model,
                                    bool keepHandle);

    virtual ~CurveRectangularHyperbola2_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual int numVariables() const override;

    virtual double evaluate(const std::vector<double>& x) const override;

    //@}
    /** @name Getters */
    //@{

    double coefficient1C1() const;

    double coefficient2C2() const;

    double coefficient3C3() const;

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
    REGISTER_LOGGER("openstudio.model.CurveRectangularHyperbola2");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_CURVERECTANGULARHYPERBOLA2_IMPL_HPP

