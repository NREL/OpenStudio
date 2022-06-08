/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#ifndef MODEL_GLTFUSERDATAWRAPPER_HPP
#define MODEL_GLTFUSERDATAWRAPPER_HPP

#include "ModelAPI.hpp"
#include "Model.hpp"

namespace openstudio {
//class GltfMetaDta;
namespace model {

  class MODEL_API GltfUserDataWrapper
  {
   public:
    void setHandle(const std::string& value);
    void setName(const std::string& value);
    void setSurfaceType(const std::string& value);
    void setSurfaceTypeMaterailName(const std::string& value);
    void setConstructionName(const std::string& value);
    void setConstructionHandle(const std::string& value);
    void setConstructionMaterialName(const std::string& value);
    void setSurfaceName(const std::string& value);
    void setSurfaceHandle(const std::string& value);
    void setSubSurfaceName(const std::string& value);
    void setSubSurfaceHandle(const std::string& value);
    void setSpaceName(const std::string& value);
    void setSpaceHandle(const std::string& value);
    void setShadingName(const std::string& value);
    void setShadingHandle(const std::string& value);
    void setThermalZoneName(const std::string& value);
    void setThermalZoneHandle(const std::string& value);
    void setThermalZoneMaterialName(const std::string& value);
    void setSpaceTypeName(const std::string& value);
    void setSpaceTypeHandle(const std::string& value);
    void setSpaceTypeMaterialName(const std::string& value);
    void setBuildingStoryName(const std::string& value);
    void setBuildingStoryHandle(const std::string& value);
    void setBuildingStoryMaterialName(const std::string& value);
    void setBuildingUnitName(const std::string& value);
    void setBuildingUnitHandle(const std::string& value);
    void setBuildingUnitMaterialName(const std::string& value);
    void setConstructionSetName(const std::string& value);
    void setConstructionSetHandle(const std::string& value);
    void setConstructionSetMaterialName(const std::string& value);
    void setOutsideBoundaryCondition(const std::string& value);
    void setOutsideBoundaryConditionObjectName(const std::string& value);
    void setOutsideBoundaryConditionObjectHandle(const std::string& value);
    void setBoundaryMaterialName(const std::string& value);
    void setCoincidentWithOutsideObject(const bool& value);
    void setSunExposure(const std::string& value);
    void setWindExposure(const std::string& value);
    void setIlluminanceSetpoint(const double& value);
    void setAirWall(const bool& value);
    void setAirLoopHVACHandles(const std::vector<std::string>& airLoopHvacHandles);
    void setAirLoopHVACNames(const std::vector<std::string>& airLoopHvacNames);
    void setAirLoopHVACMaterialNames(const std::vector<std::string>& airLoopHvacMaterialNames);

    std::string getHandle() const;
    std::string getName() const;
    std::string getSurfaceType() const;
    std::string getSurfaceTypeMaterailName() const;
    std::string getConstructionName() const;
    std::string getConstructionHandle() const;
    std::string getConstructionMaterialName() const;
    std::string getSurfaceName() const;
    std::string getSurfaceHandle() const;
    std::string getSubSurfaceName() const;
    std::string getSubSurfaceHandle() const;
    std::string getSpaceName() const;
    std::string getSpaceHandle() const;
    std::string getShadingName() const;
    std::string getShadingHandle() const;
    std::string getThermalZoneName() const;
    std::string getThermalZoneHandle() const;
    std::string getThermalZoneMaterialName() const;
    std::string getSpaceTypeName() const;
    std::string getSpaceTypeHandle() const;
    std::string getSpaceTypeMaterialName() const;
    std::string getBuildingStoryName() const;
    std::string getBuildingStoryHandle() const;
    std::string getBuildingStoryMaterialName() const;
    std::string getBuildingUnitName() const;
    std::string getBuildingUnitHandle() const;
    std::string getBuildingUnitMaterialName() const;
    std::string getConstructionSetName() const;
    std::string getConstructionSetHandle() const;
    std::string getConstructionSetMaterialName() const;
    std::string getOutsideBoundaryCondition() const;
    std::string getOutsideBoundaryConditionObjectName() const;
    std::string getOutsideBoundaryConditionObjectHandle() const;
    std::string getBoundaryMaterialName() const;
    bool getCoincidentWithOutsideObject() const;
    std::string getSunExposure() const;
    std::string getWindExposure() const;
    double getIlluminanceSetpoint() const;
    bool getAirWall() const;
    std::vector<std::string> getAirLoopHVACHandles() const;
    std::vector<std::string> getAirLoopHVACNames() const;
    std::vector<std::string> getAirLoopHVACMaterialNames() const;

   private:
    std::tuple<std::string, std::string> handle;
    std::tuple<std::string, std::string> name;
    std::tuple<std::string, std::string> surfaceType;
    std::tuple<std::string, std::string> surfaceTypeMaterailName;
    std::tuple<std::string, std::string> constructionName;
    std::tuple<std::string, std::string> constructionHandle;
    std::tuple<std::string, std::string> constructionMaterialName;
    std::tuple<std::string, std::string> surfaceName;
    std::tuple<std::string, std::string> surfaceHandle;
    std::tuple<std::string, std::string> subSurfaceName;
    std::tuple<std::string, std::string> subSurfaceHandle;
    std::tuple<std::string, std::string> spaceName;
    std::tuple<std::string, std::string> spaceHandle;
    std::tuple<std::string, std::string> shadingName;
    std::tuple<std::string, std::string> shadingHandle;
    std::tuple<std::string, std::string> thermalZoneName;
    std::tuple<std::string, std::string> thermalZoneHandle;
    std::tuple<std::string, std::string> thermalZoneMaterialName;
    std::tuple<std::string, std::string> spaceTypeName;
    std::tuple<std::string, std::string> spaceTypeHandle;
    std::tuple<std::string, std::string> spaceTypeMaterialName;
    std::tuple<std::string, std::string> buildingStoryName;
    std::tuple<std::string, std::string> buildingStoryHandle;
    std::tuple<std::string, std::string> buildingStoryMaterialName;
    std::tuple<std::string, std::string> buildingUnitName;
    std::tuple<std::string, std::string> buildingUnitHandle;
    std::tuple<std::string, std::string> buildingUnitMaterialName;
    std::tuple<std::string, std::string> constructionSetName;
    std::tuple<std::string, std::string> constructionSetHandle;
    std::tuple<std::string, std::string> constructionSetMaterialName;
    std::tuple<std::string, std::string> outsideBoundaryCondition;
    std::tuple<std::string, std::string> outsideBoundaryConditionObjectName;
    std::tuple<std::string, std::string> outsideBoundaryConditionObjectHandle;
    std::tuple<std::string, std::string> boundaryMaterialName;
    std::tuple<std::string, bool> coincidentWithOutsideObject;
    std::tuple<std::string, std::string> sunExposure;
    std::tuple<std::string, std::string> windExposure;
    std::tuple<std::string, double> illuminanceSetpoint;
    std::tuple<std::string, bool> airWall;
    std::vector<std::string> airLoopHVACNames;
    std::vector<std::string> airLoopHVACHandles;
    std::vector<std::string> airLoopHVACMaterialNames;
  };

}  // namespace model
}  // namespace openstudio

#endif  //MODEL_GLTFUSERDATAWRAPPER_HPP
