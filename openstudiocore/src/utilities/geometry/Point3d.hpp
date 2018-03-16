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

#ifndef UTILITIES_GEOMETRY_POINT3D_HPP
#define UTILITIES_GEOMETRY_POINT3D_HPP

#include "../UtilitiesAPI.hpp"
#include "../data/Vector.hpp"
#include "../core/Logger.hpp"

#include <vector>
#include <boost/optional.hpp>

namespace openstudio{

  // forward declaration
  class Vector3d;

  class UTILITIES_API Point3d{
  public:

    /// default constructor creates point at 0, 0, 0
    Point3d();

    /// constructor with x, y, z
    Point3d(double x, double y, double z);

    /// copy constructor
    Point3d(const Point3d& other);

    /// get x
    double x() const;

    /// get y
    double y() const;

    /// get z
    double z() const;

    /// point plus a vector is a new point
    Point3d operator+(const Vector3d& vec) const;

    /// point plus a vector is a new point
    Point3d& operator+=(const Vector3d& vec);

    /// point minus another point is a vector
    Vector3d operator-(const Point3d& other) const;

    /// check equality
    bool operator==(const Point3d& other) const;

  private:

    REGISTER_LOGGER("utilities.Point3d");
    Vector m_storage;

  };

  /// ostream operator
  UTILITIES_API std::ostream& operator<<(std::ostream& os, const Point3d& point);

  /// ostream operator
  UTILITIES_API std::ostream& operator<<(std::ostream& os, const std::vector<Point3d>& pointVector);

  // optional Point3d
  typedef boost::optional<Point3d> OptionalPoint3d;

  // vector of Point3d
  typedef std::vector<Point3d> Point3dVector;

  // vector of Point3dVector
  typedef std::vector<Point3dVector> Point3dVectorVector;

} // openstudio

#endif //UTILITIES_GEOMETRY_POINT3D_HPP
