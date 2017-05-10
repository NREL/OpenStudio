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
#ifndef MODEL_UNITARYSYSTEMPERFORMANCEMULTISPEED_IMPL_HPP
#define MODEL_UNITARYSYSTEMPERFORMANCEMULTISPEED_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {


namespace detail {
  
  /** UnitarySystemPerformanceMultispeed_Impl is a ModelObject_Impl that is the implementation class for UnitarySystemPerformanceMultispeed.*/
  class MODEL_API UnitarySystemPerformanceMultispeed_Impl : public ModelObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{
    UnitarySystemPerformanceMultispeed_Impl(const IdfObject& idfObject,
                                            Model_Impl* model,
                                            bool keepHandle);

    UnitarySystemPerformanceMultispeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                            Model_Impl* model,
                                            bool keepHandle);

    UnitarySystemPerformanceMultispeed_Impl(const UnitarySystemPerformanceMultispeed_Impl& other,
                                            Model_Impl* model,
                                            bool keepHandle);

    virtual ~UnitarySystemPerformanceMultispeed_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{
    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;
    //@}
    /** @name Getters */
    //@{
    bool singleModeOperation() const;

    std::vector<SupplyAirflowRatioField> supplyAirflowRatioFields();

    //@}
    /** @name Setters */
    //@{
    bool setSingleModeOperation(bool singleMode);

    void resetSingleModeOperation();

    bool setSupplyAirflowRatioFields(const std::vector<SupplyAirflowRatioField>& airflowRatioFields);

    bool addSupplyAirflowRatioField(const SupplyAirflowRatioField& airflowRatio);

    bool addSupplyAirflowRatioField(double heatingRatio, double coolingRatio);

    void resetSupplyAirflowRatioFields();

    //@}

   protected:
   private:
    REGISTER_LOGGER("openstudio.model.UnitarySystemPerformanceMultispeed");
    boost::optional<double> readDoubleFieldOrAutosize(unsigned fieldNum, const ModelExtensibleGroup& group);
  };

} //detail

} //model
} //openstudio

#endif // MODEL_UNITARYSYSTEMPERFORMANCEMULTISPEED_IMPL_HPP
