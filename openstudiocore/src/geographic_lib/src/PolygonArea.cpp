/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include <GeographicLib/PolygonArea.hpp>

namespace GeographicLib {

  using namespace std;

  template <class GeodType>
  void PolygonAreaT<GeodType>::AddPoint(real lat, real lon) {
    lat = Math::LatFix(lat);
    lon = Math::AngNormalize(lon);
    if (_num == 0) {
      _lat0 = _lat1 = lat;
      _lon0 = _lon1 = lon;
    } else {
      real s12, S12, t;
      _earth.GenInverse(_lat1, _lon1, lat, lon, _mask, s12, t, t, t, t, t, S12);
      _perimetersum += s12;
      if (!_polyline) {
        _areasum += S12;
        _crossings += transit(_lon1, lon);
      }
      _lat1 = lat; _lon1 = lon;
    }
    ++_num;
  }

  template <class GeodType>
  void PolygonAreaT<GeodType>::AddEdge(real azi, real s) {
    if (_num) {                 // Do nothing if _num is zero
      real lat, lon, S12, t;
      _earth.GenDirect(_lat1, _lon1, azi, false, s, _mask,
                       lat, lon, t, t, t, t, t, S12);
      _perimetersum += s;
      if (!_polyline) {
        _areasum += S12;
        _crossings += transitdirect(_lon1, lon);
        lon = Math::AngNormalize(lon);
      }
      _lat1 = lat; _lon1 = lon;
      ++_num;
    }
  }

  template <class GeodType>
  unsigned PolygonAreaT<GeodType>::Compute(bool reverse, bool sign,
                                           real& perimeter, real& area) const {
    real s12, S12, t;
    if (_num < 2) {
      perimeter = 0;
      if (!_polyline)
        area = 0;
      return _num;
    }
    if (_polyline) {
      perimeter = _perimetersum();
      return _num;
    }
    _earth.GenInverse(_lat1, _lon1, _lat0, _lon0, _mask,
                      s12, t, t, t, t, t, S12);
    perimeter = _perimetersum(s12);
    Accumulator<> tempsum(_areasum);
    tempsum += S12;
    int crossings = _crossings + transit(_lon1, _lon0);
    if (crossings & 1)
      tempsum += (tempsum < 0 ? 1 : -1) * _area0/2;
    // area is with the clockwise sense.  If !reverse convert to
    // counter-clockwise convention.
    if (!reverse)
      tempsum *= -1;
    // If sign put area in (-area0/2, area0/2], else put area in [0, area0)
    if (sign) {
      if (tempsum > _area0/2)
        tempsum -= _area0;
      else if (tempsum <= -_area0/2)
        tempsum += _area0;
    } else {
      if (tempsum >= _area0)
        tempsum -= _area0;
      else if (tempsum < 0)
        tempsum += _area0;
    }
    area = 0 + tempsum();
    return _num;
  }

  template <class GeodType>
  unsigned PolygonAreaT<GeodType>::TestPoint(real lat, real lon,
                                             bool reverse, bool sign,
                                             real& perimeter, real& area) const
  {
    if (_num == 0) {
      perimeter = 0;
      if (!_polyline)
        area = 0;
      return 1;
    }
    perimeter = _perimetersum();
    real tempsum = _polyline ? 0 : _areasum();
    int crossings = _crossings;
    unsigned num = _num + 1;
    for (int i = 0; i < (_polyline ? 1 : 2); ++i) {
      real s12, S12, t;
      _earth.GenInverse(i == 0 ? _lat1 : lat, i == 0 ? _lon1 : lon,
                        i != 0 ? _lat0 : lat, i != 0 ? _lon0 : lon,
                        _mask, s12, t, t, t, t, t, S12);
      perimeter += s12;
      if (!_polyline) {
        tempsum += S12;
        crossings += transit(i == 0 ? _lon1 : lon,
                             i != 0 ? _lon0 : lon);
      }
    }

    if (_polyline)
      return num;

    if (crossings & 1)
      tempsum += (tempsum < 0 ? 1 : -1) * _area0/2;
    // area is with the clockwise sense.  If !reverse convert to
    // counter-clockwise convention.
    if (!reverse)
      tempsum *= -1;
    // If sign put area in (-area0/2, area0/2], else put area in [0, area0)
    if (sign) {
      if (tempsum > _area0/2)
        tempsum -= _area0;
      else if (tempsum <= -_area0/2)
        tempsum += _area0;
    } else {
      if (tempsum >= _area0)
        tempsum -= _area0;
      else if (tempsum < 0)
        tempsum += _area0;
    }
    area = 0 + tempsum;
    return num;
  }

  template <class GeodType>
  unsigned PolygonAreaT<GeodType>::TestEdge(real azi, real s,
                                            bool reverse, bool sign,
                                            real& perimeter, real& area) const {
    if (_num == 0) {            // we don't have a starting point!
      perimeter = Math::NaN();
      if (!_polyline)
        area = Math::NaN();
      return 0;
    }
    unsigned num = _num + 1;
    perimeter = _perimetersum() + s;
    if (_polyline)
      return num;

    real tempsum =  _areasum();
    int crossings = _crossings;
    {
      real lat, lon, s12, S12, t;
      _earth.GenDirect(_lat1, _lon1, azi, false, s, _mask,
                       lat, lon, t, t, t, t, t, S12);
      tempsum += S12;
      crossings += transitdirect(_lon1, lon);
      lon = Math::AngNormalize(lon);
      _earth.GenInverse(lat, lon, _lat0, _lon0, _mask, s12, t, t, t, t, t, S12);
      perimeter += s12;
      tempsum += S12;
      crossings += transit(lon, _lon0);
    }

    if (crossings & 1)
      tempsum += (tempsum < 0 ? 1 : -1) * _area0/2;
    // area is with the clockwise sense.  If !reverse convert to
    // counter-clockwise convention.
    if (!reverse)
      tempsum *= -1;
    // If sign put area in (-area0/2, area0/2], else put area in [0, area0)
    if (sign) {
      if (tempsum > _area0/2)
        tempsum -= _area0;
      else if (tempsum <= -_area0/2)
        tempsum += _area0;
    } else {
      if (tempsum >= _area0)
        tempsum -= _area0;
      else if (tempsum < 0)
        tempsum += _area0;
    }
    area = 0 + tempsum;
    return num;
  }

  template class GEOGRAPHICLIB_EXPORT PolygonAreaT<Geodesic>;
  template class GEOGRAPHICLIB_EXPORT PolygonAreaT<GeodesicExact>;
  template class GEOGRAPHICLIB_EXPORT PolygonAreaT<Rhumb>;

} // namespace GeographicLib
