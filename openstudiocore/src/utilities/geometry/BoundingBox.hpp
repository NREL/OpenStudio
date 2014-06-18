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


#ifndef UTILITIES_GEOMETRY_BOUNDINGBOX_HPP
#define UTILITIES_GEOMETRY_BOUNDINGBOX_HPP

#include "../UtilitiesAPI.hpp"
#include "../core/Logger.hpp"

#include <boost/optional.hpp>

namespace openstudio{

  // forward declaration
  class Point3d;

  /** BoundingBox is an axis aligned cuboid that can check for intersections with another BoundingBox.
   *  BoundingBox intersections require that both boxes are specified in the same coordinate system.
   */
  class UTILITIES_API BoundingBox{
  public:

    /// default constructor creates empty BoundingBox, will not intersect with anything
    BoundingBox();

    /// add another BoundingBox
    void add(const BoundingBox& other);

    /// add a point to the BoundingBox
    void addPoint(const Point3d& point);

    /// add points to the BoundingBox
    void addPoints(const std::vector<Point3d>& points);

    /// test for intersection
    bool intersects(const BoundingBox& other, double tol = 0.001);

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

} // openstudio

#endif //UTILITIES_GEOMETRY_BOUNDINGBOX_HPP
