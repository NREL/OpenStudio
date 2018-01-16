/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef MODEL_FFACTORGROUNDFLOORCONSTRUCTION_IMPL_HPP
#define MODEL_FFACTORGROUNDFLOORCONSTRUCTION_IMPL_HPP

#include "ConstructionBase_Impl.hpp"

#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

namespace openstudio {
namespace model {
namespace detail {

  class MODEL_API FFactorGroundFloorConstruction_Impl : public ConstructionBase_Impl
  {














  public:

    // constructor
    FFactorGroundFloorConstruction_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    // construct from workspace
    FFactorGroundFloorConstruction_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                        Model_Impl* model,
                                        bool keepHandle);

    FFactorGroundFloorConstruction_Impl(const FFactorGroundFloorConstruction_Impl& other,
                                        Model_Impl* model,
                                        bool keepHandle);

    // virtual destructor
    virtual ~FFactorGroundFloorConstruction_Impl(){}

    /** Get all output variables names that could be associated with this object. These variables
     *  may or may not be available for each simulation, need to check report variable dictionary
     *  to see if the variable is available. Each concrete class should override this method.*/
    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    /** @name Queries */
    //@{

    virtual bool isOpaque() const override;

    virtual bool isFenestration() const override;

    virtual bool isSolarDiffusing() const override;

    virtual bool isModelPartition() const override;

    virtual int renderingColorIndex() const override;

    /// Returns fFactor.
    virtual double fFactor() const;
    virtual Quantity getFFactor(bool returnIP = false) const;
    virtual bool setFFactor(double fFactor);
    virtual bool setFFactor(const Quantity& fFactor);

    /// Returns area.
    virtual double area() const;
    virtual Quantity getArea(bool returnIP = false) const;
    virtual bool setArea(double area);
    virtual bool setArea(const Quantity& area);

    /// Returns perimeterExposed.
    virtual double perimeterExposed() const;
    virtual Quantity getPerimeterExposed(bool returnIP = false) const;
    virtual bool setPerimeterExposed(double perimeterExposed);
    virtual bool setPerimeterExposed(const Quantity& perimeterExposed);

    //@}

  private:
    openstudio::Quantity fFactor_SI() const;
    openstudio::Quantity fFactor_IP() const;

    openstudio::Quantity area_SI() const;
    openstudio::Quantity area_IP() const;

    openstudio::Quantity perimeterExposed_SI() const;
    openstudio::Quantity perimeterExposed_IP() const;

    REGISTER_LOGGER("openstudio.model.FFactorGroundFloorConstruction");
  };

} // detail
} // model
} // openstudio

#endif // MODEL_FFACTORGROUNDFLOORCONSTRUCTION_IMPL_HPP
