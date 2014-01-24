/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#include <model/GlareSensor.hpp>
#include <model/GlareSensor_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_Glare_Sensor_FieldEnums.hxx>

#include <utilities/geometry/Transformation.hpp>
#include <utilities/geometry/EulerAngles.hpp>
#include <utilities/geometry/Vector3d.hpp>
#include <utilities/geometry/Point3d.hpp>
#include <utilities/geometry/Geometry.hpp>
#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model {

namespace detail {

  GlareSensor_Impl::GlareSensor_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : SpaceItem_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == GlareSensor::iddObjectType());
  }

  GlareSensor_Impl::GlareSensor_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                   Model_Impl* model,
                                                   bool keepHandle)
    : SpaceItem_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == GlareSensor::iddObjectType());
  }

  GlareSensor_Impl::GlareSensor_Impl(const GlareSensor_Impl& other,
                                                   Model_Impl* model,
                                                   bool keepHandle)
    : SpaceItem_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& GlareSensor_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType GlareSensor_Impl::iddObjectType() const {
    return GlareSensor::iddObjectType();
  }

  double GlareSensor_Impl::positionXCoordinate() const {
    boost::optional<double> value = getDouble(OS_Glare_SensorFields::PositionXCoordinate,true);
    OS_ASSERT(value);
    return value.get();
  }

  double GlareSensor_Impl::positionYCoordinate() const {
    boost::optional<double> value = getDouble(OS_Glare_SensorFields::PositionYCoordinate,true);
    OS_ASSERT(value);
    return value.get();
  }

  double GlareSensor_Impl::positionZCoordinate() const {
    boost::optional<double> value = getDouble(OS_Glare_SensorFields::PositionZCoordinate,true);
    OS_ASSERT(value);
    return value.get();
  }

  double GlareSensor_Impl::psiRotationAroundXAxis() const {
    boost::optional<double> value = getDouble(OS_Glare_SensorFields::PsiRotationAroundXAxis,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool GlareSensor_Impl::isPsiRotationAroundXAxisDefaulted() const {
    return isEmpty(OS_Glare_SensorFields::PsiRotationAroundXAxis);
  }

  double GlareSensor_Impl::thetaRotationAroundYAxis() const {
    boost::optional<double> value = getDouble(OS_Glare_SensorFields::ThetaRotationAroundYAxis,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool GlareSensor_Impl::isThetaRotationAroundYAxisDefaulted() const {
    return isEmpty(OS_Glare_SensorFields::ThetaRotationAroundYAxis);
  }

  double GlareSensor_Impl::phiRotationAroundZAxis() const {
    boost::optional<double> value = getDouble(OS_Glare_SensorFields::PhiRotationAroundZAxis,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool GlareSensor_Impl::isPhiRotationAroundZAxisDefaulted() const {
    return isEmpty(OS_Glare_SensorFields::PhiRotationAroundZAxis);
  }

  int GlareSensor_Impl::numberofGlareViewVectors() const {
    boost::optional<int> value = getInt(OS_Glare_SensorFields::NumberofGlareViewVectors,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> GlareSensor_Impl::maximumAllowableDaylightGlareProbability() const {
    return getDouble(OS_Glare_SensorFields::MaximumAllowableDaylightGlareProbability,true);
  }

  void GlareSensor_Impl::setPositionXCoordinate(double positionXCoordinate) {
    bool result = setDouble(OS_Glare_SensorFields::PositionXCoordinate, positionXCoordinate);
    OS_ASSERT(result);
  }

  void GlareSensor_Impl::setPositionYCoordinate(double positionYCoordinate) {
    bool result = setDouble(OS_Glare_SensorFields::PositionYCoordinate, positionYCoordinate);
    OS_ASSERT(result);
  }

  void GlareSensor_Impl::setPositionZCoordinate(double positionZCoordinate) {
    bool result = setDouble(OS_Glare_SensorFields::PositionZCoordinate, positionZCoordinate);
    OS_ASSERT(result);
  }

  void GlareSensor_Impl::setPsiRotationAroundXAxis(double psiRotationAroundXAxis) {
    bool result = setDouble(OS_Glare_SensorFields::PsiRotationAroundXAxis, psiRotationAroundXAxis);
    OS_ASSERT(result);
  }

  void GlareSensor_Impl::resetPsiRotationAroundXAxis() {
    bool result = setString(OS_Glare_SensorFields::PsiRotationAroundXAxis, "");
    OS_ASSERT(result);
  }

  void GlareSensor_Impl::setThetaRotationAroundYAxis(double thetaRotationAroundYAxis) {
    bool result = setDouble(OS_Glare_SensorFields::ThetaRotationAroundYAxis, thetaRotationAroundYAxis);
    OS_ASSERT(result);
  }

  void GlareSensor_Impl::resetThetaRotationAroundYAxis() {
    bool result = setString(OS_Glare_SensorFields::ThetaRotationAroundYAxis, "");
    OS_ASSERT(result);
  }

  void GlareSensor_Impl::setPhiRotationAroundZAxis(double phiRotationAroundZAxis) {
    bool result = setDouble(OS_Glare_SensorFields::PhiRotationAroundZAxis, phiRotationAroundZAxis);
    OS_ASSERT(result);
  }

  void GlareSensor_Impl::resetPhiRotationAroundZAxis() {
    bool result = setString(OS_Glare_SensorFields::PhiRotationAroundZAxis, "");
    OS_ASSERT(result);
  }

  bool GlareSensor_Impl::setNumberofGlareViewVectors(int numberofGlareViewVectors) {
    bool result = setInt(OS_Glare_SensorFields::NumberofGlareViewVectors, numberofGlareViewVectors);
    return result;
  }

  void GlareSensor_Impl::resetNumberofGlareViewVectors() {
    bool result = setString(OS_Glare_SensorFields::NumberofGlareViewVectors, "");
    OS_ASSERT(result);
  }

  bool GlareSensor_Impl::setMaximumAllowableDaylightGlareProbability(boost::optional<double> maximumAllowableDaylightGlareProbability) {
    bool result = false;
    if (maximumAllowableDaylightGlareProbability) {
      result = setDouble(OS_Glare_SensorFields::MaximumAllowableDaylightGlareProbability, maximumAllowableDaylightGlareProbability.get());
    } else {
      result = setString(OS_Glare_SensorFields::MaximumAllowableDaylightGlareProbability, "");
    }
    return result;
  }

  void GlareSensor_Impl::resetMaximumAllowableDaylightGlareProbability() {
    bool result = setString(OS_Glare_SensorFields::MaximumAllowableDaylightGlareProbability, "");
    OS_ASSERT(result);
  }

  Point3d GlareSensor_Impl::position() const
  {
    Point3d result(this->positionXCoordinate(),
                   this->positionYCoordinate(),
                   this->positionZCoordinate());
    return result;
  }

  bool GlareSensor_Impl::setPosition(const Point3d& position)
  {
    this->setPositionXCoordinate(position.x());
    this->setPositionYCoordinate(position.y());
    this->setPositionZCoordinate(position.z());
    return true;
  }

  openstudio::Transformation GlareSensor_Impl::transformation() const
  {
    Vector3d origin(this->positionXCoordinate(),
                    this->positionYCoordinate(),
                    this->positionZCoordinate());

    EulerAngles angles(degToRad(this->psiRotationAroundXAxis()),
                       degToRad(this->thetaRotationAroundYAxis()),
                       degToRad(this->phiRotationAroundZAxis()));

    Transformation result = Transformation::translation(origin)*Transformation::rotation(angles);

    return result;
  }

  bool GlareSensor_Impl::setTransformation(const openstudio::Transformation& transformation) 
  {
    Vector3d translation = transformation.translation();
    this->setPositionXCoordinate(translation.x());
    this->setPositionYCoordinate(translation.y());
    this->setPositionZCoordinate(translation.z());

    EulerAngles eulerAngles = transformation.eulerAngles();
    setPsiRotationAroundXAxis(radToDeg(eulerAngles.psi()));
    setThetaRotationAroundYAxis(radToDeg(eulerAngles.theta()));
    setPhiRotationAroundZAxis(radToDeg(eulerAngles.phi()));
    
    return true;  
  }

  //bool GlareSensor_Impl::isPrimaryGlareSensor() const
  //{
  //  bool result = false;
  //  std::vector<WorkspaceObject> workspaceObjects = this->getSources(IddObjectType::OS_ThermalZone);
  //  if (!workspaceObjects.empty()){
  //    OS_ASSERT(workspaceObjects.size() == 1);
  //    boost::optional<GlareSensor> control = workspaceObjects[0].cast<ThermalZone>().primaryGlareSensor();
  //    if (control){
  //      result = (this->handle() == control->handle());
  //    }
  //  }
  //  return result;
  //}
  //
  //bool GlareSensor_Impl::isSecondaryGlareSensor() const
  //{
  //  bool result = false;
  //  std::vector<WorkspaceObject> workspaceObjects = this->getSources(IddObjectType::OS_ThermalZone);
  //  if (!workspaceObjects.empty()){
  //    OS_ASSERT(workspaceObjects.size() == 1);
  //    boost::optional<GlareSensor> control = workspaceObjects[0].cast<ThermalZone>().secondaryGlareSensor();
  //    if (control){
  //      result = (this->handle() == control->handle());
  //    }
  //  }
  //  return result;  }

  int GlareSensor_Impl::spaceIndex() const
  {
    return OS_Glare_SensorFields::SpaceName;
  }

} // detail

GlareSensor::GlareSensor(const Model& model)
  : SpaceItem(GlareSensor::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::GlareSensor_Impl>());
}

const IddObjectType& GlareSensor::iddObjectType() {
  static IddObjectType result(IddObjectType::OS_Glare_Sensor);
  return result;
}

double GlareSensor::positionXCoordinate() const {
  return getImpl<detail::GlareSensor_Impl>()->positionXCoordinate();
}

double GlareSensor::positionYCoordinate() const {
  return getImpl<detail::GlareSensor_Impl>()->positionYCoordinate();
}

double GlareSensor::positionZCoordinate() const {
  return getImpl<detail::GlareSensor_Impl>()->positionZCoordinate();
}

double GlareSensor::psiRotationAroundXAxis() const {
  return getImpl<detail::GlareSensor_Impl>()->psiRotationAroundXAxis();
}

bool GlareSensor::isPsiRotationAroundXAxisDefaulted() const {
  return getImpl<detail::GlareSensor_Impl>()->isPsiRotationAroundXAxisDefaulted();
}

double GlareSensor::thetaRotationAroundYAxis() const {
  return getImpl<detail::GlareSensor_Impl>()->thetaRotationAroundYAxis();
}

bool GlareSensor::isThetaRotationAroundYAxisDefaulted() const {
  return getImpl<detail::GlareSensor_Impl>()->isThetaRotationAroundYAxisDefaulted();
}

double GlareSensor::phiRotationAroundZAxis() const {
  return getImpl<detail::GlareSensor_Impl>()->phiRotationAroundZAxis();
}

bool GlareSensor::isPhiRotationAroundZAxisDefaulted() const {
  return getImpl<detail::GlareSensor_Impl>()->isPhiRotationAroundZAxisDefaulted();
}

int GlareSensor::numberofGlareViewVectors() const {
  return getImpl<detail::GlareSensor_Impl>()->numberofGlareViewVectors();
}

boost::optional<double> GlareSensor::maximumAllowableDaylightGlareProbability() const {
  return getImpl<detail::GlareSensor_Impl>()->maximumAllowableDaylightGlareProbability();
}

void GlareSensor::setPositionXCoordinate(double positionXCoordinate) {
  getImpl<detail::GlareSensor_Impl>()->setPositionXCoordinate(positionXCoordinate);
}

void GlareSensor::setPositionYCoordinate(double positionYCoordinate) {
  getImpl<detail::GlareSensor_Impl>()->setPositionYCoordinate(positionYCoordinate);
}

void GlareSensor::setPositionZCoordinate(double positionZCoordinate) {
  getImpl<detail::GlareSensor_Impl>()->setPositionZCoordinate(positionZCoordinate);
}

void GlareSensor::setPsiRotationAroundXAxis(double psiRotationAroundXAxis) {
  return getImpl<detail::GlareSensor_Impl>()->setPsiRotationAroundXAxis(psiRotationAroundXAxis);
}

void GlareSensor::resetPsiRotationAroundXAxis() {
  getImpl<detail::GlareSensor_Impl>()->resetPsiRotationAroundXAxis();
}

void GlareSensor::setThetaRotationAroundYAxis(double thetaRotationAroundYAxis) {
  return getImpl<detail::GlareSensor_Impl>()->setThetaRotationAroundYAxis(thetaRotationAroundYAxis);
}

void GlareSensor::resetThetaRotationAroundYAxis() {
  getImpl<detail::GlareSensor_Impl>()->resetThetaRotationAroundYAxis();
}

void GlareSensor::setPhiRotationAroundZAxis(double phiRotationAroundZAxis) {
  return getImpl<detail::GlareSensor_Impl>()->setPhiRotationAroundZAxis(phiRotationAroundZAxis);
}

void GlareSensor::resetPhiRotationAroundZAxis() {
  getImpl<detail::GlareSensor_Impl>()->resetPhiRotationAroundZAxis();
}

bool GlareSensor::setNumberofGlareViewVectors(int numberofGlareViewVectors) {
  return getImpl<detail::GlareSensor_Impl>()->setNumberofGlareViewVectors(numberofGlareViewVectors);
}

void GlareSensor::resetNumberofGlareViewVectors() {
  getImpl<detail::GlareSensor_Impl>()->resetNumberofGlareViewVectors();
}

bool GlareSensor::setMaximumAllowableDaylightGlareProbability(double maximumAllowableDaylightGlareProbability) {
  return getImpl<detail::GlareSensor_Impl>()->setMaximumAllowableDaylightGlareProbability(maximumAllowableDaylightGlareProbability);
}

void GlareSensor::resetMaximumAllowableDaylightGlareProbability() {
  getImpl<detail::GlareSensor_Impl>()->resetMaximumAllowableDaylightGlareProbability();
}

Point3d GlareSensor::position() const
{
  return getImpl<detail::GlareSensor_Impl>()->position();
}

bool GlareSensor::setPosition(const Point3d& position)
{
  return getImpl<detail::GlareSensor_Impl>()->setPosition(position);
}

openstudio::Transformation GlareSensor::transformation() const {
  return getImpl<detail::GlareSensor_Impl>()->transformation();
}

bool GlareSensor::setTransformation(const openstudio::Transformation& transformation)
{
  return getImpl<detail::GlareSensor_Impl>()->setTransformation(transformation);
}

/// @cond
GlareSensor::GlareSensor(boost::shared_ptr<detail::GlareSensor_Impl> impl)
  : SpaceItem(impl)
{}
/// @endcond


} // model
} // openstudio

