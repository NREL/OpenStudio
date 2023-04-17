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
#ifndef GLTF_GLTFMODELOBJECTMETADATA_HPP
#define GLTF_GLTFMODELOBJECTMETADATA_HPP

#include "GltfAPI.hpp"

#include "../utilities/core/Logger.hpp"

namespace tinygltf {
class Value;
}

namespace openstudio {
namespace model {

  class AirLoopHVAC;
  class BuildingUnit;
  class SpaceType;
  class BuildingStory;
  class ThermalZone;
  class DefaultConstructionSet;
  class Space;
}  // namespace model

namespace gltf {

  class GltfMetaData;

  /** GltfModelObjectMetaData is an Interface class between select ModelObjects and glTF values
   *
   *  It is capable of storing attributes such as name, iddObjectType, color, handle from ModelObjects:
   *  ThermalZone, SpaceType, AirLoopHVAC, BuildingStory, BuildingUnit, DefaultConstructionSet
   **/
  class GLTF_API GltfModelObjectMetaData
  {
   public:
    /** Default constructor **/
    GltfModelObjectMetaData();

    /** @name Interface with ModelObjects */
    //@{
    explicit GltfModelObjectMetaData(const model::AirLoopHVAC& airLoopHVAC);
    explicit GltfModelObjectMetaData(const model::BuildingUnit& buildingUnit);
    explicit GltfModelObjectMetaData(const model::SpaceType& spaceType);
    explicit GltfModelObjectMetaData(const model::BuildingStory& buildingStory);
    explicit GltfModelObjectMetaData(const model::ThermalZone& thermalZone);
    explicit GltfModelObjectMetaData(const model::DefaultConstructionSet& defaultConstructionSet);
    explicit GltfModelObjectMetaData(const model::Space& space);
    //@}

    /** @name Getters and Setters */
    //@{

    std::string color() const;
    void setColor(const std::string& color);

    std::string handle() const;
    void setHandle(const std::string& handle);

    std::string iddObjectType() const;
    void setIddObjectType(const std::string& iddObjectType);

    std::string name() const;
    void setName(const std::string& name);

    bool isOpentoBelow() const;
    void setOpentoBelow(bool opentoBelow);

    double nominalZCoordinate() const;
    void setNominalZCoordinate(double nominalZCoordinate);

    double nominalFloortoCeilingHeight() const;
    void setNominalFloortoCeilingHeight(double nominalFloortoCeilingHeight);

    int multiplier() const;
    void setMultiplier(int multiplier);

    //@}

   protected:
    /** @name Protected */
    //@{

    /** Interface with an existing GLTF file **/
    explicit GltfModelObjectMetaData(const tinygltf::Value& modelObjectMetaDataObj);

    /** Export to tinygltf **/
    std::map<std::string, tinygltf::Value> toExtras() const;

    //@}

    friend class GltfMetaData;

   private:
    REGISTER_LOGGER("openstudio.gltf.GltfModelObjectMetaData");

    std::string m_color;
    std::string m_handle;
    std::string m_iddObjectType;
    std::string m_name;
    bool m_opentoBelow = false;
    double m_nominalZCoordinate = 0.0;
    double m_nominalFloortoCeilingHeight = 0.0;
    int m_multiplier = 0;
  };

  using GltfModelObjectMetaDataVector = std::vector<GltfModelObjectMetaData>;

}  // namespace gltf
}  // namespace openstudio

#endif  // GLTF_GLTFMODELOBJECTMETADATA_HPP
