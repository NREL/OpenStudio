/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "PointLatLon.hpp"
#include "Point3d.hpp"

#include <GeographicLib/Geodesic.hpp>
#include <GeographicLib/LocalCartesian.hpp>
#include <GeographicLib/UTMUPS.hpp>

namespace openstudio{

  /// default constructor creates point at 0, 0, 0
  PointLatLon::PointLatLon()
    : m_storage(3, 0.0)
  {}

  /// constructor with lat, lon, and height
  PointLatLon::PointLatLon(double lat, double lon, double height)
    : m_storage(3)
  {
    m_storage[0] = lat;
    m_storage[1] = lon;
    m_storage[2] = height;
  }

  /// copy constructor
  PointLatLon::PointLatLon(const PointLatLon& other)
    : m_storage(other.m_storage)
  {}

  PointLatLon::~PointLatLon()
  {}

  /// get lat
  double PointLatLon::lat() const
  {
    return m_storage[0];
  }

  /// get lon
  double PointLatLon::lon() const
  {
    return m_storage[1];
  }

  /// get height
  double PointLatLon::height() const
  {
    return m_storage[2];
  }

  PointLatLon& PointLatLon::operator=(const openstudio::PointLatLon& other)
  {
    m_storage = other.m_storage;
    m_localCartesianConverter.reset();
    return *this;
  }

  /// check equality
  bool PointLatLon::operator==(const PointLatLon& other) const
  {
    return (m_storage == other.m_storage);
  }

  double PointLatLon::operator-(const PointLatLon& other) const
  {
    double s12;
    const GeographicLib::Geodesic& geod = GeographicLib::Geodesic::WGS84();
    geod.Inverse(lat(), lon(), other.lat(), other.lon(), s12);
    return s12;
  }

  Point3d PointLatLon::toLocalCartesian(const PointLatLon& point) const
  {
    initLocalCartesianConverter();
    double x, y, z;
    m_localCartesianConverter->Forward(point.lat(), point.lon(), point.height(), x, y, z);
    return Point3d(x, y, z);
  }

  std::vector<Point3d> PointLatLon::toLocalCartesian(const std::vector<PointLatLon>& points) const
  {
    std::vector<Point3d> result;
    for (const auto& point : points){
      result.push_back(toLocalCartesian(point));
    }
    return result;
  }

  PointLatLon PointLatLon::fromLocalCartesian(const Point3d& point) const
  {
    initLocalCartesianConverter();
    double lat, lon, h;
    m_localCartesianConverter->Reverse(point.x(), point.y(), point.z(), lat, lon, h);
    return PointLatLon(lat, lon, h);
  }

  std::vector<PointLatLon> PointLatLon::fromLocalCartesian(const std::vector<Point3d>& points) const
  {
    std::vector<PointLatLon> result;
    for (const auto& point : points){
      result.push_back(fromLocalCartesian(point));
    }
    return result;
  }

  int PointLatLon::utmZone() const
  {
    return GeographicLib::UTMUPS::StandardZone(lat(), lon());
  }

  Point3d PointLatLon::toUTM(const PointLatLon& point) const
  {
    int zone;
    bool northp;
    double x, y, gamma, k;
    GeographicLib::UTMUPS::Forward(point.lat(), point.lon(), zone, northp, x, y, gamma, k, utmZone());
    return Point3d(x, y, point.height());
  }

  std::vector<Point3d> PointLatLon::toUTM(const std::vector<PointLatLon>& points) const
  {
    std::vector<Point3d> result;
    for (const auto& point : points){
      result.push_back(toUTM(point));
    }
    return result;
  }

  PointLatLon PointLatLon::fromUTM(const Point3d& point) const
  {
    int zone;
    bool northp;
    {
      double x, y, gamma, k;
      GeographicLib::UTMUPS::Forward(lat(), lon(), zone, northp, x, y, gamma, k);
    }

    double lat, lon, gamma, k;
    GeographicLib::UTMUPS::Reverse(zone, northp, point.x(), point.y(), lat, lon, gamma, k);
    return PointLatLon(lat, lon, point.z());
  }

  std::vector<PointLatLon> PointLatLon::fromUTM(const std::vector<Point3d>& points) const
  {
    std::vector<PointLatLon> result;
    for (const auto& point : points){
      result.push_back(fromUTM(point));
    }
    return result;
  }

  void PointLatLon::initLocalCartesianConverter() const
  {
    if (!m_localCartesianConverter){
      m_localCartesianConverter = std::unique_ptr<GeographicLib::LocalCartesian>(new GeographicLib::LocalCartesian(lat(), lon(), height()));
    }
  }

  /// ostream operator
  std::ostream& operator<<(std::ostream& os, const PointLatLon& point)
  {
    os << "[" << point.lat() << ", " << point.lon() << ", " << point.height() << "]";
    return os;
  }

  /// ostream operator
  std::ostream& operator<<(std::ostream& os, const std::vector<PointLatLon>& pointVector)
  {
    os << "[";
    for (unsigned i = 0; i < pointVector.size(); ++i){
      os << pointVector[i];
      if (i < pointVector.size()-1){
        os << ", ";
      }
    }
    os << "]";
    return os;
  }

  double getDistanceLatLon(double lat1, double lon1, double lat2, double lon2)
  {
    PointLatLon p1(lat1, lon1);
    PointLatLon p2(lat2, lon2);
    return (p1 - p2);
  }

} // openstudio
