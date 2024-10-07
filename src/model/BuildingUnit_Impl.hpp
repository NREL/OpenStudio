/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_BUILDINGUNIT_IMPL_HPP
#define MODEL_BUILDINGUNIT_IMPL_HPP

#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class Space;
  class RenderingColor;
  class BuildingUnit;

  namespace detail {

    class MODEL_API BuildingUnit_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      BuildingUnit_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      BuildingUnit_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      BuildingUnit_Impl(const BuildingUnit_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~BuildingUnit_Impl() override = default;

      //@}

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      /** @name Getters */
      //@{

      boost::optional<RenderingColor> renderingColor() const;

      std::string buildingUnitType() const;

      std::vector<Space> spaces() const;

      std::vector<std::string> featureNames() const;

      boost::optional<std::string> getFeatureDataType(const std::string& name) const;

      boost::optional<std::string> getFeatureAsString(const std::string& name) const;

      boost::optional<double> getFeatureAsDouble(const std::string& name) const;

      boost::optional<int> getFeatureAsInteger(const std::string& name) const;

      boost::optional<bool> getFeatureAsBoolean(const std::string& name) const;

      std::vector<std::string> suggestedFeatures() const;

      //@}

      /** @name Setters */
      //@{

      bool setRenderingColor(const RenderingColor& renderingColor);

      void resetRenderingColor();

      bool setBuildingUnitType(const std::string& buildingUnitType);

      void resetBuildingUnitType();

      bool setFeature(const std::string& name, const std::string& value);

      bool setFeature(const std::string& name, const char* value);

      bool setFeature(const std::string& name, double value);

      bool setFeature(const std::string& name, int value);

      bool setFeature(const std::string& name, bool value);

      bool resetFeature(const std::string& name);

      //@}

     protected:
     private:
      REGISTER_LOGGER("openstudio.model.BuildingUnit")
    };

  }  // namespace detail
}  // namespace model
}  // namespace openstudio

#endif  //MODEL_BUILDINGUNIT_IMPL_HPP
