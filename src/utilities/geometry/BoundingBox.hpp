/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2021, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef UTILITIES_GEOMETRY_BOUNDINGBOX_HPP
#define UTILITIES_GEOMETRY_BOUNDINGBOX_HPP

#include "../UtilitiesAPI.hpp"
#include "../core/Logger.hpp"

#include <boost/optional.hpp>

#include <vector>

namespace openstudio {

// forward declaration
class Point3d;

/** BoundingBox is an axis aligned cuboid that can check for intersections with another BoundingBox.
   *  BoundingBox intersections require that both boxes are specified in the same coordinate system.
   */
class UTILITIES_API BoundingBox
{
 public:
  /// default constructor creates empty BoundingBox, will not intersect with anything
  BoundingBox();

  /// add another BoundingBox
  void add(const BoundingBox& other);

  /// add a point to the BoundingBox
  void addPoint(const Point3d& point);

  /// add points to the BoundingBox
  void addPoints(const std::vector<Point3d>& points);

  /// test for intersection. Default tolerance is 1cm
  bool intersects(const BoundingBox& other, double tol = 0.01) const;

  bool isEmpty() const;

  boost::optional<double> minX() const;
  boost::optional<double> minY() const;
  boost::optional<double> minZ() const;

  boost::optional<double> maxX() const;
  boost::optional<double> maxY() const;
  boost::optional<double> maxZ() const;

  std::vector<Point3d> corners() const;

 private:
  REGISTER_LOGGER("utilities.BoundingBox");

  boost::optional<double> m_minX;
  boost::optional<double> m_minY;
  boost::optional<double> m_minZ;

  boost::optional<double> m_maxX;
  boost::optional<double> m_maxY;
  boost::optional<double> m_maxZ;
};

// optional BoundingBox
typedef boost::optional<BoundingBox> OptionalBoundingBox;

// vector of BoundingBox
typedef std::vector<BoundingBox> BoundingBoxVector;

}  // namespace openstudio

#endif  //UTILITIES_GEOMETRY_BOUNDINGBOX_HPP
