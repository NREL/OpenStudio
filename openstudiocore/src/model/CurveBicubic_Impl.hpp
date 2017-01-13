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

#ifndef MODEL_CURVEBICUBIC_IMPL_HPP
#define MODEL_CURVEBICUBIC_IMPL_HPP

#include "Curve_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  /** CurveBicubic_Impl is a Curve_Impl that is the implementation class for CurveBicubic.*/
  class MODEL_API CurveBicubic_Impl : public Curve_Impl {
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
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

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual int numVariables() const override;

    virtual double evaluate(const std::vector<double>& x) const override;

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

