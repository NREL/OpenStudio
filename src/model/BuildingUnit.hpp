/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_BUILDINGUNIT_HPP
#define MODEL_BUILDINGUNIT_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  class Space;
  class RenderingColor;

  namespace detail {

    class BuildingUnit_Impl;

  }  // namespace detail

  class MODEL_API BuildingUnit : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit BuildingUnit(const Model& model);

    virtual ~BuildingUnit() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    BuildingUnit(const BuildingUnit& other) = default;
    BuildingUnit(BuildingUnit&& other) = default;
    BuildingUnit& operator=(const BuildingUnit&) = default;
    BuildingUnit& operator=(BuildingUnit&&) = default;

    //@}

    /** @name Static Methods */
    //@{

    // List the possible values for building unit type
    static std::vector<std::string> buildingUnitTypeValues();

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    // Gets the rendering color
    boost::optional<RenderingColor> renderingColor() const;

    // Gets the building unit type
    std::string buildingUnitType() const;

    std::vector<Space> spaces() const;

    /// \deprecated { This method is deprecated, use additionalProperties instead }
    std::vector<std::string> featureNames() const;

    /// \deprecated { This method is deprecated, use additionalProperties instead }
    boost::optional<std::string> getFeatureDataType(const std::string& name) const;

    /// \deprecated { This method is deprecated, use additionalProperties instead }
    boost::optional<std::string> getFeatureAsString(const std::string& name) const;

    /// \deprecated { This method is deprecated, use additionalProperties instead }
    boost::optional<double> getFeatureAsDouble(const std::string& name) const;

    /// \deprecated { This method is deprecated, use additionalProperties instead }
    boost::optional<int> getFeatureAsInteger(const std::string& name) const;

    /// \deprecated { This method is deprecated, use additionalProperties instead }
    boost::optional<bool> getFeatureAsBoolean(const std::string& name) const;

    /// \deprecated { This method is deprecated, use additionalProperties instead }
    std::vector<std::string> suggestedFeatures() const;

    double floorArea() const;

    //@}
    /** @name Setters */
    //@{

    bool setRenderingColor(const RenderingColor& renderingColor);

    void resetRenderingColor();

    bool setBuildingUnitType(const std::string& buildingUnitType);

    void resetBuildingUnitType();

    /// \deprecated { This method is deprecated, use additionalProperties instead }
    bool setFeature(const std::string& name, const std::string& value);

    /// \deprecated { This method is deprecated, use additionalProperties instead }
    bool setFeature(const std::string& name, const char* value);

    /// \deprecated { This method is deprecated, use additionalProperties instead }
    bool setFeature(const std::string& name, double value);

    /// \deprecated { This method is deprecated, use additionalProperties instead }
    bool setFeature(const std::string& name, int value);

    /// \deprecated { This method is deprecated, use additionalProperties instead }
    bool setFeature(const std::string& name, bool value);

    /// \deprecated { This method is deprecated, use additionalProperties instead }
    bool resetFeature(const std::string& name);

    //@}

   protected:
    /// @cond

    using ImplType = detail::BuildingUnit_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    explicit BuildingUnit(std::shared_ptr<detail::BuildingUnit_Impl> impl);

    /// @endcond

   private:
    REGISTER_LOGGER("openstudio.model.BuildingUnit")
  };

}  // namespace model

}  // namespace openstudio

#endif  //MODEL_BUILDINGUNIT_HPP
