/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SIZINGPLANT_IMPL_HPP
#define MODEL_SIZINGPLANT_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class PlantLoop;

  namespace detail {

    /** SizingPlant_Impl is a ModelObject_Impl that is the implementation class for SizingPlant.*/
    class MODEL_API SizingPlant_Impl : public ModelObject_Impl
    {

     public:
      SizingPlant_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SizingPlant_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SizingPlant_Impl(const SizingPlant_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SizingPlant_Impl() override = default;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual bool setParent(ParentObject& newParent) override;

      PlantLoop plantLoop() const;

      std::string loopType() const;

      double designLoopExitTemperature() const;

      double loopDesignTemperatureDifference() const;

      std::string sizingOption() const;

      int zoneTimestepsinAveragingWindow() const;

      std::string coincidentSizingFactorMode() const;

      bool setPlantLoop(const PlantLoop& plantLoop);

      bool setLoopType(const std::string& loopType);

      bool setDesignLoopExitTemperature(double designLoopExitTemperature);

      bool setLoopDesignTemperatureDifference(double loopDesignTemperatureDifference);

      bool setSizingOption(const std::string& sizingOption);

      bool setZoneTimestepsinAveragingWindow(int zoneTimestepsinAveragingWindow);

      bool setCoincidentSizingFactorMode(const std::string& coincidentSizingFactorMode);

     protected:
     private:
      REGISTER_LOGGER("openstudio.model.SizingPlant");

      // TODO: Check the return types of these methods.
      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<PlantLoop> optionalPlantLoop() const;

      std::vector<std::string> loopTypeValues() const;
      boost::optional<ModelObject> plantLoopAsModelObject() const;

      bool setPlantLoopAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SIZINGPLANT_IMPL_HPP
