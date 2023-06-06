/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
