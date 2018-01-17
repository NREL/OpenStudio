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

#ifndef MODEL_SIZINGPLANT_IMPL_HPP
#define MODEL_SIZINGPLANT_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"
#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

namespace openstudio {
namespace model {

class PlantLoop;

namespace detail {

  /** SizingPlant_Impl is a ModelObject_Impl that is the implementation class for SizingPlant.*/
  class MODEL_API SizingPlant_Impl : public ModelObject_Impl {















    public:

    SizingPlant_Impl(const IdfObject& idfObject,
                     Model_Impl* model,
                     bool keepHandle);

    SizingPlant_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                     Model_Impl* model,
                     bool keepHandle);

    SizingPlant_Impl(const SizingPlant_Impl& other,
                     Model_Impl* model,
                     bool keepHandle);

    virtual ~SizingPlant_Impl() {}

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    PlantLoop plantLoop() const;

    std::string loopType() const;

    double designLoopExitTemperature() const;

    Quantity getDesignLoopExitTemperature(bool returnIP=false) const;

    double loopDesignTemperatureDifference() const;

    Quantity getLoopDesignTemperatureDifference(bool returnIP=false) const;

    std::string sizingOption() const;

    int zoneTimestepsinAveragingWindow() const;

    std::string coincidentSizingFactorMode() const;

    bool setPlantLoop(const PlantLoop& plantLoop);

    bool setLoopType(std::string loopType);

    bool setDesignLoopExitTemperature(double designLoopExitTemperature);

    bool setDesignLoopExitTemperature(const Quantity& designLoopExitTemperature);

    bool setLoopDesignTemperatureDifference(double loopDesignTemperatureDifference);

    bool setLoopDesignTemperatureDifference(const Quantity& loopDesignTemperatureDifference);

    bool setSizingOption(std::string sizingOption);

    bool setZoneTimestepsinAveragingWindow(int zoneTimestepsinAveragingWindow);

    bool setCoincidentSizingFactorMode(std::string coincidentSizingFactorMode);

    protected:

    private:

    REGISTER_LOGGER("openstudio.model.SizingPlant");

    // TODO: Check the return types of these methods.
    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<PlantLoop> optionalPlantLoop() const;

    std::vector<std::string> loopTypeValues() const;
    openstudio::Quantity designLoopExitTemperature_SI() const;
    openstudio::Quantity designLoopExitTemperature_IP() const;
    openstudio::Quantity loopDesignTemperatureDifference_SI() const;
    openstudio::Quantity loopDesignTemperatureDifference_IP() const;

    boost::optional<ModelObject> plantLoopAsModelObject() const;

    bool setPlantLoopAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SIZINGPLANT_IMPL_HPP
