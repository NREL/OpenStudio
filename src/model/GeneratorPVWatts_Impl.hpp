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

#ifndef MODEL_GENERATORPVWATTS_IMPL_HPP
#define MODEL_GENERATORPVWATTS_IMPL_HPP

#include "ModelAPI.hpp"
#include "Generator_Impl.hpp"

namespace openstudio {
namespace model {

  class PlanarSurface;

  namespace detail {

    /** GeneratorPVWatts_Impl is a Generator_Impl that is the implementation class for GeneratorPVWatts.*/
    class MODEL_API GeneratorPVWatts_Impl : public Generator_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      GeneratorPVWatts_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      GeneratorPVWatts_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      GeneratorPVWatts_Impl(const GeneratorPVWatts_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~GeneratorPVWatts_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual std::string generatorObjectType() const override;

      virtual boost::optional<Schedule> availabilitySchedule() const override;

      virtual boost::optional<double> ratedElectricPowerOutput() const override;

      virtual boost::optional<double> ratedThermaltoElectricalPowerRatio() const override;

      //@}
      /** @name Getters */
      //@{

      std::string pvWattsVersion() const;

      double dcSystemCapacity() const;

      std::string moduleType() const;

      bool isModuleTypeDefaulted() const;

      std::string arrayType() const;

      bool isArrayTypeDefaulted() const;

      double systemLosses() const;

      bool isSystemLossesDefaulted() const;

      double tiltAngle() const;

      bool isTiltAngleDefaulted() const;

      double azimuthAngle() const;

      bool isAzimuthAngleDefaulted() const;

      boost::optional<PlanarSurface> surface() const;

      double groundCoverageRatio() const;

      bool isGroundCoverageRatioDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setDCSystemCapacity(double dcSystemCapacity);

      bool setModuleType(const std::string& moduleType);

      void resetModuleType();

      bool setArrayType(const std::string& arrayType);

      void resetArrayType();

      bool setSystemLosses(double systemLosses);

      void resetSystemLosses();

      bool setTiltAngle(double tiltAngle);

      void resetTiltAngle();

      bool setAzimuthAngle(double azimuthAngle);

      void resetAzimuthAngle();

      bool setSurface(const PlanarSurface& surface);

      void resetSurface();

      bool setGroundCoverageRatio(double groundCoverageRatio);

      void resetGroundCoverageRatio();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.GeneratorPVWatts");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GENERATORPVWATTS_IMPL_HPP
