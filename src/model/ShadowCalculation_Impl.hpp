/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SHADOWCALCULATION_IMPL_HPP
#define MODEL_SHADOWCALCULATION_IMPL_HPP

#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class ThermalZone;
  class ModelObjectList;

  namespace detail {

    class MODEL_API ShadowCalculation_Impl : public ModelObject_Impl
    {

     public:
      // constructor
      ShadowCalculation_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      ShadowCalculation_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // clone copy constructor
      ShadowCalculation_Impl(const ShadowCalculation_Impl& other, Model_Impl* model, bool keepHandle);

      // virtual destructor
      virtual ~ShadowCalculation_Impl() override = default;

      // return the parent object in the hierarchy
      virtual boost::optional<ParentObject> parent() const override;

      // set the parent, child may have to call methods on the parent
      virtual bool setParent(ParentObject& newParent) override;

      // Get all output variable names that could be associated with this object.
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      /** @name Getters */
      //@{

      std::string shadingCalculationMethod() const;

      std::string shadingCalculationUpdateFrequencyMethod() const;
      bool isShadingCalculationUpdateFrequencyMethodDefaulted() const;

      int shadingCalculationUpdateFrequency() const;
      bool isShadingCalculationUpdateFrequencyDefaulted() const;

      int maximumFiguresInShadowOverlapCalculations() const;
      bool isMaximumFiguresInShadowOverlapCalculationsDefaulted() const;

      std::string polygonClippingAlgorithm() const;

      int pixelCountingResolution() const;

      std::string skyDiffuseModelingAlgorithm() const;

      bool outputExternalShadingCalculationResults() const;

      bool disableSelfShadingWithinShadingZoneGroups() const;

      bool disableSelfShadingFromShadingZoneGroupstoOtherZones() const;

      unsigned int numberofShadingZoneGroups() const;
      std::vector<ThermalZone> getShadingZoneGroup(unsigned groupIndex) const;

      //@}
      /** @name Setters */
      //@{

      bool setShadingCalculationMethod(const std::string& shadingCalculationMethod);

      bool setShadingCalculationUpdateFrequencyMethod(const std::string& shadingCalculationUpdateFrequencyMethod);
      void resetShadingCalculationUpdateFrequencyMethod();

      bool setShadingCalculationUpdateFrequency(int shadingCalculationUpdateFrequency);
      void resetShadingCalculationUpdateFrequency();

      bool setMaximumFiguresInShadowOverlapCalculations(int maximumFiguresInShadowOverlapCalculations);
      void resetMaximumFiguresInShadowOverlapCalculations();

      bool setPolygonClippingAlgorithm(const std::string& polygonClippingAlgorithm);
      void resetPolygonClippingAlgorithm();

      bool setPixelCountingResolution(int pixelCountingResolution);

      bool setSkyDiffuseModelingAlgorithm(const std::string& skyDiffuseModelingAlgorithm);
      void resetSkyDiffuseModelingAlgorithm();

      bool setOutputExternalShadingCalculationResults(bool outputExternalShadingCalculationResults);

      bool setDisableSelfShadingWithinShadingZoneGroups(bool disableSelfShadingWithinShadingZoneGroups);

      bool setDisableSelfShadingFromShadingZoneGroupstoOtherZones(bool disableSelfShadingFromShadingZoneGroupstoOtherZones);

      bool addShadingZoneGroup(const std::vector<ThermalZone>& thermalZones);
      bool removeShadingZoneGroup(unsigned groupIndex);
      void removeAllShadingZoneGroups();

      //@}

     private:
      REGISTER_LOGGER("openstudio.model.ShadowCalculation");

      boost::optional<ModelObjectList> getShadingZoneGroupModelObjectList(unsigned groupIndex) const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SHADOWCALCULATION_IMPL_HPP
