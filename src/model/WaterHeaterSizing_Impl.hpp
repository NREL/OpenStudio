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

#ifndef MODEL_WATERHEATERSIZING_IMPL_HPP
#define MODEL_WATERHEATERSIZING_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class WaterToWaterComponent;

  namespace detail {

    /** WaterHeaterSizing_Impl is a ModelObject_Impl that is the implementation class for WaterHeaterSizing.*/
    class MODEL_API WaterHeaterSizing_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      WaterHeaterSizing_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      WaterHeaterSizing_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      WaterHeaterSizing_Impl(const WaterHeaterSizing_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~WaterHeaterSizing_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      WaterToWaterComponent waterHeater() const;

      boost::optional<std::string> designMode() const;

      boost::optional<double> timeStorageCanMeetPeakDraw() const;

      boost::optional<double> timeforTankRecovery() const;

      boost::optional<double> nominalTankVolumeforAutosizingPlantConnections() const;

      boost::optional<int> numberofBedrooms() const;

      boost::optional<int> numberofBathrooms() const;

      boost::optional<double> storageCapacityperPerson() const;

      boost::optional<double> recoveryCapacityperPerson() const;

      boost::optional<double> storageCapacityperFloorArea() const;

      boost::optional<double> recoveryCapacityperFloorArea() const;

      boost::optional<double> numberofUnits() const;

      boost::optional<double> storageCapacityperUnit() const;

      boost::optional<double> recoveryCapacityPerUnit() const;

      boost::optional<double> storageCapacityperCollectorArea() const;

      boost::optional<double> heightAspectRatio() const;

      //@}
      /** @name Setters */
      //@{

      bool setWaterHeater(const WaterToWaterComponent& waterHeater);

      bool setDesignMode(const std::string& designMode);

      void resetDesignMode();

      bool setTimeStorageCanMeetPeakDraw(double timeStorageCanMeetPeakDraw);

      void resetTimeStorageCanMeetPeakDraw();

      bool setTimeforTankRecovery(double timeforTankRecovery);

      void resetTimeforTankRecovery();

      bool setNominalTankVolumeforAutosizingPlantConnections(double nominalTankVolumeforAutosizingPlantConnections);

      void resetNominalTankVolumeforAutosizingPlantConnections();

      bool setNumberofBedrooms(int numberofBedrooms);

      void resetNumberofBedrooms();

      bool setNumberofBathrooms(int numberofBathrooms);

      void resetNumberofBathrooms();

      bool setStorageCapacityperPerson(double storageCapacityperPerson);

      void resetStorageCapacityperPerson();

      bool setRecoveryCapacityperPerson(double recoveryCapacityperPerson);

      void resetRecoveryCapacityperPerson();

      bool setStorageCapacityperFloorArea(double storageCapacityperFloorArea);

      void resetStorageCapacityperFloorArea();

      bool setRecoveryCapacityperFloorArea(double recoveryCapacityperFloorArea);

      void resetRecoveryCapacityperFloorArea();

      bool setNumberofUnits(double numberofUnits);

      void resetNumberofUnits();

      bool setStorageCapacityperUnit(double storageCapacityperUnit);

      void resetStorageCapacityperUnit();

      bool setRecoveryCapacityPerUnit(double recoveryCapacityPerUnit);

      void resetRecoveryCapacityPerUnit();

      bool setStorageCapacityperCollectorArea(double storageCapacityperCollectorArea);

      void resetStorageCapacityperCollectorArea();

      bool setHeightAspectRatio(double heightAspectRatio);

      void resetHeightAspectRatio();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.WaterHeaterSizing");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<WaterToWaterComponent> optionalWaterHeater() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_WATERHEATERSIZING_IMPL_HPP
