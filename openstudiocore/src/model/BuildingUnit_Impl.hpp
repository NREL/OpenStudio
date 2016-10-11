/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/


#ifndef OPENSTUDIO_BUILDINGUNIT_IMPL_HPP_HPP
#define OPENSTUDIO_BUILDINGUNIT_IMPL_HPP_HPP

#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

class Space;
class RenderingColor;
class BuildingUnit;

namespace detail {

class MODEL_API BuildingUnit_Impl : public ModelObject_Impl {
 public:

  /** @name Constructors and Destructors */
  //@{

  BuildingUnit_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

  BuildingUnit_Impl(
          const openstudio::detail::WorkspaceObject_Impl& other,
          Model_Impl* model,
          bool keepHandle
  );

  BuildingUnit_Impl(
          const BuildingUnit_Impl& other,
          Model_Impl* model,
          bool keepHandle
  );

  virtual ~BuildingUnit_Impl() {}

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

  boost::optional<ModelExtensibleGroup> getFeatureGroupByName(const std::string& name) const;

  bool setFeatureGroupDataTypeAndValue(const std::string& name, const std::string& dataType, const std::string& value);

  boost::optional<std::string> getFeatureStringAndCheckForType(const std::string& name, const std::string& expectedDataType) const;

  //bool setRenderingColorAsModelObject(boost::optional<ModelObject> modelObject);

};

} // detail
} // model
} // openstudio

#endif //OPENSTUDIO_BUILDINGUNIT_IMPL_HPP_HPP
