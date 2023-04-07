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

    virtual ~BuildingUnit() = default;
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
