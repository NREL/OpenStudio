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

#include <model/IlluminanceMap.hpp>
#include <model/IlluminanceMap_Impl.hpp>

#include <utilities/idd/OS_IlluminanceMap_FieldEnums.hxx>

#include <utilities/geometry/Transformation.hpp>
#include <utilities/geometry/EulerAngles.hpp>
#include <utilities/geometry/Vector3d.hpp>
#include <utilities/geometry/Point3d.hpp>
#include <utilities/geometry/Geometry.hpp>
#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model {

namespace detail {

  IlluminanceMap_Impl::IlluminanceMap_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : SpaceItem_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == IlluminanceMap::iddObjectType());
  }

  IlluminanceMap_Impl::IlluminanceMap_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : SpaceItem_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == IlluminanceMap::iddObjectType());
  }

  IlluminanceMap_Impl::IlluminanceMap_Impl(const IlluminanceMap_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : SpaceItem_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& IlluminanceMap_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType IlluminanceMap_Impl::iddObjectType() const {
    return IlluminanceMap::iddObjectType();
  }

  double IlluminanceMap_Impl::originXCoordinate() const {
    boost::optional<double> value = getDouble(OS_IlluminanceMapFields::OriginXCoordinate,true);
    OS_ASSERT(value);
    return value.get();
  }

  double IlluminanceMap_Impl::originYCoordinate() const {
    boost::optional<double> value = getDouble(OS_IlluminanceMapFields::OriginYCoordinate,true);
    OS_ASSERT(value);
    return value.get();
  }

  double IlluminanceMap_Impl::originZCoordinate() const {
    boost::optional<double> value = getDouble(OS_IlluminanceMapFields::OriginZCoordinate,true);
    OS_ASSERT(value);
    return value.get();
  }

  double IlluminanceMap_Impl::psiRotationAroundXAxis() const {
    boost::optional<double> value = getDouble(OS_IlluminanceMapFields::PsiRotationAroundXAxis,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool IlluminanceMap_Impl::isPsiRotationAroundXAxisDefaulted() const {
    return isEmpty(OS_IlluminanceMapFields::PsiRotationAroundXAxis);
  }

  double IlluminanceMap_Impl::thetaRotationAroundYAxis() const {
    boost::optional<double> value = getDouble(OS_IlluminanceMapFields::ThetaRotationAroundYAxis,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool IlluminanceMap_Impl::isThetaRotationAroundYAxisDefaulted() const {
    return isEmpty(OS_IlluminanceMapFields::ThetaRotationAroundYAxis);
  }

  double IlluminanceMap_Impl::phiRotationAroundZAxis() const {
    boost::optional<double> value = getDouble(OS_IlluminanceMapFields::PhiRotationAroundZAxis,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool IlluminanceMap_Impl::isPhiRotationAroundZAxisDefaulted() const {
    return isEmpty(OS_IlluminanceMapFields::PhiRotationAroundZAxis);
  }

  double IlluminanceMap_Impl::xLength() const {
    boost::optional<double> value = getDouble(OS_IlluminanceMapFields::XLength,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool IlluminanceMap_Impl::isXLengthDefaulted() const {
    return isEmpty(OS_IlluminanceMapFields::XLength);
  }

  int IlluminanceMap_Impl::numberofXGridPoints() const {
    boost::optional<int> value = getInt(OS_IlluminanceMapFields::NumberofXGridPoints,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool IlluminanceMap_Impl::isNumberofXGridPointsDefaulted() const {
    return isEmpty(OS_IlluminanceMapFields::NumberofXGridPoints);
  }

  double IlluminanceMap_Impl::yLength() const {
    boost::optional<double> value = getDouble(OS_IlluminanceMapFields::YLength,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool IlluminanceMap_Impl::isYLengthDefaulted() const {
    return isEmpty(OS_IlluminanceMapFields::YLength);
  }

  int IlluminanceMap_Impl::numberofYGridPoints() const {
    boost::optional<int> value = getInt(OS_IlluminanceMapFields::NumberofYGridPoints,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool IlluminanceMap_Impl::isNumberofYGridPointsDefaulted() const {
    return isEmpty(OS_IlluminanceMapFields::NumberofYGridPoints);
  }

  void IlluminanceMap_Impl::setOriginXCoordinate(double originXCoordinate) {
    bool result = setDouble(OS_IlluminanceMapFields::OriginXCoordinate, originXCoordinate);
    OS_ASSERT(result);
  }

  void IlluminanceMap_Impl::setOriginYCoordinate(double originYCoordinate) {
    bool result = setDouble(OS_IlluminanceMapFields::OriginYCoordinate, originYCoordinate);
    OS_ASSERT(result);
  }

  void IlluminanceMap_Impl::setOriginZCoordinate(double originZCoordinate) {
    bool result = setDouble(OS_IlluminanceMapFields::OriginZCoordinate, originZCoordinate);
    OS_ASSERT(result);
  }

  bool IlluminanceMap_Impl::setPsiRotationAroundXAxis(double psiRotationAroundXAxis) {
    bool result = setDouble(OS_IlluminanceMapFields::PsiRotationAroundXAxis, psiRotationAroundXAxis);
    return result;
  }

  void IlluminanceMap_Impl::resetPsiRotationAroundXAxis() {
    bool result = setString(OS_IlluminanceMapFields::PsiRotationAroundXAxis, "");
    OS_ASSERT(result);
  }

  bool IlluminanceMap_Impl::setThetaRotationAroundYAxis(double thetaRotationAroundYAxis) {
    bool result = setDouble(OS_IlluminanceMapFields::ThetaRotationAroundYAxis, thetaRotationAroundYAxis);
    return result;
  }

  void IlluminanceMap_Impl::resetThetaRotationAroundYAxis() {
    bool result = setString(OS_IlluminanceMapFields::ThetaRotationAroundYAxis, "");
    OS_ASSERT(result);
  }

  bool IlluminanceMap_Impl::setPhiRotationAroundZAxis(double phiRotationAroundZAxis) {
    bool result = setDouble(OS_IlluminanceMapFields::PhiRotationAroundZAxis, phiRotationAroundZAxis);
    return result;
  }

  void IlluminanceMap_Impl::resetPhiRotationAroundZAxis() {
    bool result = setString(OS_IlluminanceMapFields::PhiRotationAroundZAxis, "");
    OS_ASSERT(result);
  }

  void IlluminanceMap_Impl::setXLength(double xLength) {
    bool result = setDouble(OS_IlluminanceMapFields::XLength, xLength);
    OS_ASSERT(result);
  }

  void IlluminanceMap_Impl::resetXLength() {
    bool result = setString(OS_IlluminanceMapFields::XLength, "");
    OS_ASSERT(result);
  }

  bool IlluminanceMap_Impl::setNumberofXGridPoints(int numberofXGridPoints) {
    bool result = setInt(OS_IlluminanceMapFields::NumberofXGridPoints, numberofXGridPoints);
    return result;
  }

  void IlluminanceMap_Impl::resetNumberofXGridPoints() {
    bool result = setString(OS_IlluminanceMapFields::NumberofXGridPoints, "");
    OS_ASSERT(result);
  }

  void IlluminanceMap_Impl::setYLength(double yLength) {
    bool result = setDouble(OS_IlluminanceMapFields::YLength, yLength);
    OS_ASSERT(result);
  }

  void IlluminanceMap_Impl::resetYLength() {
    bool result = setString(OS_IlluminanceMapFields::YLength, "");
    OS_ASSERT(result);
  }

  bool IlluminanceMap_Impl::setNumberofYGridPoints(int numberofYGridPoints) {
    bool result = setInt(OS_IlluminanceMapFields::NumberofYGridPoints, numberofYGridPoints);
    return result;
  }

  void IlluminanceMap_Impl::resetNumberofYGridPoints() {
    bool result = setString(OS_IlluminanceMapFields::NumberofYGridPoints, "");
    OS_ASSERT(result);
  }

  openstudio::Transformation IlluminanceMap_Impl::transformation() const
  {
    Vector3d origin(this->originXCoordinate(),
                    this->originYCoordinate(),
                    this->originZCoordinate());

    EulerAngles angles(degToRad(this->psiRotationAroundXAxis()),
                       degToRad(this->thetaRotationAroundYAxis()),
                       degToRad(this->phiRotationAroundZAxis()));

    Transformation result = Transformation::translation(origin)*Transformation::rotation(angles);

    return result;
  }

  bool IlluminanceMap_Impl::setTransformation(const openstudio::Transformation& transformation) 
  {
    bool test;

    Vector3d translation = transformation.translation();
    this->setOriginXCoordinate(translation.x());
    this->setOriginYCoordinate(translation.y());
    this->setOriginZCoordinate(translation.z());

    EulerAngles eulerAngles = transformation.eulerAngles();
    test = this->setPsiRotationAroundXAxis(radToDeg(eulerAngles.psi()));
    OS_ASSERT(test);
    test = this->setThetaRotationAroundYAxis(radToDeg(eulerAngles.theta()));
    OS_ASSERT(test);
    test = this->setPhiRotationAroundZAxis(radToDeg(eulerAngles.phi()));
    OS_ASSERT(test);

    return true;
  }

  std::vector<Point3d> IlluminanceMap_Impl::referencePoints() const
  {
    std::vector<Point3d> result;

    BOOST_FOREACH(double y, linspace(0, this->yLength(), this->numberofYGridPoints())){
      BOOST_FOREACH(double x, linspace(0, this->xLength(), this->numberofXGridPoints())){
        result.push_back(Point3d(x, y, 0));
      }
    }

    return result;
  }

  std::vector<Point3d> IlluminanceMap_Impl::corners() const
  {
    std::vector<Point3d> result;
    result.push_back(Point3d(0,this->yLength(),0));
    result.push_back(Point3d(0,0,0));
    result.push_back(Point3d(this->xLength(),0,0));
    result.push_back(Point3d(this->xLength(),this->yLength(),0));
    return result;
  }

  int IlluminanceMap_Impl::spaceIndex() const
  {
    return OS_IlluminanceMapFields::SpaceName;
  }

} // detail

IlluminanceMap::IlluminanceMap(const Model& model)
  : SpaceItem(IlluminanceMap::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::IlluminanceMap_Impl>());
}

IddObjectType IlluminanceMap::iddObjectType() {
  IddObjectType result(IddObjectType::OS_IlluminanceMap);
  return result;
}

double IlluminanceMap::originXCoordinate() const {
  return getImpl<detail::IlluminanceMap_Impl>()->originXCoordinate();
}

double IlluminanceMap::originYCoordinate() const {
  return getImpl<detail::IlluminanceMap_Impl>()->originYCoordinate();
}

double IlluminanceMap::originZCoordinate() const {
  return getImpl<detail::IlluminanceMap_Impl>()->originZCoordinate();
}

double IlluminanceMap::psiRotationAroundXAxis() const {
  return getImpl<detail::IlluminanceMap_Impl>()->psiRotationAroundXAxis();
}

bool IlluminanceMap::isPsiRotationAroundXAxisDefaulted() const {
  return getImpl<detail::IlluminanceMap_Impl>()->isPsiRotationAroundXAxisDefaulted();
}

double IlluminanceMap::thetaRotationAroundYAxis() const {
  return getImpl<detail::IlluminanceMap_Impl>()->thetaRotationAroundYAxis();
}

bool IlluminanceMap::isThetaRotationAroundYAxisDefaulted() const {
  return getImpl<detail::IlluminanceMap_Impl>()->isThetaRotationAroundYAxisDefaulted();
}

double IlluminanceMap::phiRotationAroundZAxis() const {
  return getImpl<detail::IlluminanceMap_Impl>()->phiRotationAroundZAxis();
}

bool IlluminanceMap::isPhiRotationAroundZAxisDefaulted() const {
  return getImpl<detail::IlluminanceMap_Impl>()->isPhiRotationAroundZAxisDefaulted();
}

double IlluminanceMap::xLength() const {
  return getImpl<detail::IlluminanceMap_Impl>()->xLength();
}

bool IlluminanceMap::isXLengthDefaulted() const {
  return getImpl<detail::IlluminanceMap_Impl>()->isXLengthDefaulted();
}

int IlluminanceMap::numberofXGridPoints() const {
  return getImpl<detail::IlluminanceMap_Impl>()->numberofXGridPoints();
}

bool IlluminanceMap::isNumberofXGridPointsDefaulted() const {
  return getImpl<detail::IlluminanceMap_Impl>()->isNumberofXGridPointsDefaulted();
}

double IlluminanceMap::yLength() const {
  return getImpl<detail::IlluminanceMap_Impl>()->yLength();
}

bool IlluminanceMap::isYLengthDefaulted() const {
  return getImpl<detail::IlluminanceMap_Impl>()->isYLengthDefaulted();
}

int IlluminanceMap::numberofYGridPoints() const {
  return getImpl<detail::IlluminanceMap_Impl>()->numberofYGridPoints();
}

bool IlluminanceMap::isNumberofYGridPointsDefaulted() const {
  return getImpl<detail::IlluminanceMap_Impl>()->isNumberofYGridPointsDefaulted();
}

void IlluminanceMap::setOriginXCoordinate(double originXCoordinate) {
  getImpl<detail::IlluminanceMap_Impl>()->setOriginXCoordinate(originXCoordinate);
}

void IlluminanceMap::setOriginYCoordinate(double originYCoordinate) {
  getImpl<detail::IlluminanceMap_Impl>()->setOriginYCoordinate(originYCoordinate);
}

void IlluminanceMap::setOriginZCoordinate(double originZCoordinate) {
  getImpl<detail::IlluminanceMap_Impl>()->setOriginZCoordinate(originZCoordinate);
}

bool IlluminanceMap::setPsiRotationAroundXAxis(double psiRotationAroundXAxis) {
  return getImpl<detail::IlluminanceMap_Impl>()->setPsiRotationAroundXAxis(psiRotationAroundXAxis);
}

void IlluminanceMap::resetPsiRotationAroundXAxis() {
  getImpl<detail::IlluminanceMap_Impl>()->resetPsiRotationAroundXAxis();
}

bool IlluminanceMap::setThetaRotationAroundYAxis(double thetaRotationAroundYAxis) {
  return getImpl<detail::IlluminanceMap_Impl>()->setThetaRotationAroundYAxis(thetaRotationAroundYAxis);
}

void IlluminanceMap::resetThetaRotationAroundYAxis() {
  getImpl<detail::IlluminanceMap_Impl>()->resetThetaRotationAroundYAxis();
}

bool IlluminanceMap::setPhiRotationAroundZAxis(double phiRotationAroundZAxis) {
  return getImpl<detail::IlluminanceMap_Impl>()->setPhiRotationAroundZAxis(phiRotationAroundZAxis);
}

void IlluminanceMap::resetPhiRotationAroundZAxis() {
  getImpl<detail::IlluminanceMap_Impl>()->resetPhiRotationAroundZAxis();
}

void IlluminanceMap::setXLength(double xLength) {
  getImpl<detail::IlluminanceMap_Impl>()->setXLength(xLength);
}

void IlluminanceMap::resetXLength() {
  getImpl<detail::IlluminanceMap_Impl>()->resetXLength();
}

bool IlluminanceMap::setNumberofXGridPoints(int numberofXGridPoints) {
  return getImpl<detail::IlluminanceMap_Impl>()->setNumberofXGridPoints(numberofXGridPoints);
}

void IlluminanceMap::resetNumberofXGridPoints() {
  getImpl<detail::IlluminanceMap_Impl>()->resetNumberofXGridPoints();
}

void IlluminanceMap::setYLength(double yLength) {
  getImpl<detail::IlluminanceMap_Impl>()->setYLength(yLength);
}

void IlluminanceMap::resetYLength() {
  getImpl<detail::IlluminanceMap_Impl>()->resetYLength();
}

bool IlluminanceMap::setNumberofYGridPoints(int numberofYGridPoints) {
  return getImpl<detail::IlluminanceMap_Impl>()->setNumberofYGridPoints(numberofYGridPoints);
}

void IlluminanceMap::resetNumberofYGridPoints() {
  getImpl<detail::IlluminanceMap_Impl>()->resetNumberofYGridPoints();
}


openstudio::Transformation IlluminanceMap::transformation() const
{
  return getImpl<detail::IlluminanceMap_Impl>()->transformation();
}

bool IlluminanceMap::setTransformation(const openstudio::Transformation& transformation)
{
  return getImpl<detail::IlluminanceMap_Impl>()->setTransformation(transformation);
}

std::vector<Point3d> IlluminanceMap::referencePoints() const
{
  return getImpl<detail::IlluminanceMap_Impl>()->referencePoints();
}

std::vector<Point3d> IlluminanceMap::corners() const
{
  return getImpl<detail::IlluminanceMap_Impl>()->corners();
}

/// @cond
IlluminanceMap::IlluminanceMap(boost::shared_ptr<detail::IlluminanceMap_Impl> impl)
  : SpaceItem(impl)
{}
/// @endcond


} // model
} // openstudio

