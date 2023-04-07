/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef MODEL_COILCOOLINGDXCURVEFITPERFORMANCE_IMPL_HPP
#define MODEL_COILCOOLINGDXCURVEFITPERFORMANCE_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "ResourceObject_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class CoilCoolingDX;
  class CoilCoolingDXCurveFitOperatingMode;

  namespace detail {

    /** CoilCoolingDXCurveFitPerformance_Impl is a ResourceObject_Impl that is the implementation class for CoilCoolingDXCurveFitPerformance.*/
    class MODEL_API CoilCoolingDXCurveFitPerformance_Impl : public ResourceObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilCoolingDXCurveFitPerformance_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilCoolingDXCurveFitPerformance_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilCoolingDXCurveFitPerformance_Impl(const CoilCoolingDXCurveFitPerformance_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilCoolingDXCurveFitPerformance_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<ModelObject> children() const override;

      // If this object is still used by at least one CoilCoolingDX, refuse to remove to avoid putting the CoilCoolingDX(s) in a broken state
      virtual std::vector<IdfObject> remove() override;

      //@}
      /** @name Getters */
      //@{

      double crankcaseHeaterCapacity() const;

      double minimumOutdoorDryBulbTemperatureforCompressorOperation() const;

      double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const;

      double unitInternalStaticAirPressure() const;

      std::string capacityControlMethod() const;

      double evaporativeCondenserBasinHeaterCapacity() const;

      double evaporativeCondenserBasinHeaterSetpointTemperature() const;

      Schedule evaporativeCondenserBasinHeaterOperatingSchedule() const;

      std::string compressorFuelType() const;

      CoilCoolingDXCurveFitOperatingMode baseOperatingMode() const;

      boost::optional<CoilCoolingDXCurveFitOperatingMode> alternativeOperatingMode1() const;

      boost::optional<CoilCoolingDXCurveFitOperatingMode> alternativeOperatingMode2() const;

      std::vector<CoilCoolingDX> coilCoolingDXs() const;

      //@}
      /** @name Setters */
      //@{

      bool setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity);

      bool setMinimumOutdoorDryBulbTemperatureforCompressorOperation(double minimumOutdoorDryBulbTemperatureforCompressorOperation);

      bool setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);

      bool setUnitInternalStaticAirPressure(double unitInternalStaticAirPressure);

      bool setCapacityControlMethod(const std::string& capacityControlMethod);

      bool setEvaporativeCondenserBasinHeaterCapacity(double evaporativeCondenserBasinHeaterCapacity);

      bool setEvaporativeCondenserBasinHeaterSetpointTemperature(double evaporativeCondenserBasinHeaterSetpointTemperature);

      bool setEvaporativeCondenserBasinHeaterOperatingSchedule(Schedule& schedule);

      bool setCompressorFuelType(const std::string& compressorFuelType);

      bool setBaseOperatingMode(const CoilCoolingDXCurveFitOperatingMode& baseOperatingMode);

      bool setAlternativeOperatingMode1(const CoilCoolingDXCurveFitOperatingMode& alternativeOperatingMode1);

      void resetAlternativeOperatingMode1();

      bool setAlternativeOperatingMode2(const CoilCoolingDXCurveFitOperatingMode& alternativeOperatingMode2);

      void resetAlternativeOperatingMode2();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilCoolingDXCurveFitPerformance");

      boost::optional<CoilCoolingDXCurveFitOperatingMode> optionalBaseOperatingMode() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGDXCURVEFITPERFORMANCE_IMPL_HPP
