/**********************************************************************
*  Copyright (c) 2008-2015, Alliance for Sustainable Energy.  
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

#include "LinePlot.hpp"
#include <cfloat>
#include <qwt/qwt_painter.h>


using namespace std;
using namespace boost;

namespace openstudio{

TimeSeriesLinePlotData::TimeSeriesLinePlotData(TimeSeries timeSeries)
: m_timeSeries(timeSeries),
  m_minX(timeSeries.firstReportDateTime().date().dayOfYear()+timeSeries.firstReportDateTime().time().totalDays()),
  m_maxX(timeSeries.daysFromFirstReport()[timeSeries.daysFromFirstReport().size()-1]+timeSeries.firstReportDateTime().date().dayOfYear()+timeSeries.firstReportDateTime().time().totalDays()), // end day
  m_minY(minimum(timeSeries.values())),
  m_maxY(maximum(timeSeries.values())),
  m_size(timeSeries.values().size())
{
  m_boundingRect = QRectF(m_minX, m_minY, (m_maxX - m_minX), (m_maxY - m_minY));
  m_minValue = m_minY;
  m_maxValue = m_maxY;
  m_units = timeSeries.units();
  m_fracDaysOffset = 0.0;
  m_x = m_timeSeries.daysFromFirstReport();
  m_y = m_timeSeries.values();
}

TimeSeriesLinePlotData::TimeSeriesLinePlotData(TimeSeries timeSeries, double fracDaysOffset)
: m_timeSeries(timeSeries),
  m_minX(timeSeries.firstReportDateTime().date().dayOfYear()+timeSeries.firstReportDateTime().time().totalDays()),
  m_maxX(timeSeries.daysFromFirstReport()[timeSeries.daysFromFirstReport().size()-1]+timeSeries.firstReportDateTime().date().dayOfYear()+timeSeries.firstReportDateTime().time().totalDays()), // end day
  m_minY(minimum(timeSeries.values())),
  m_maxY(maximum(timeSeries.values())),
  m_size(timeSeries.values().size())
{
  m_boundingRect = QRectF(m_minX, m_minY, (m_maxX - m_minX), (m_maxY - m_minY));
  m_minValue = m_minY;
  m_maxValue = m_maxY;
  m_units = timeSeries.units();
  m_fracDaysOffset = fracDaysOffset; // note updating in xValue does not affect scaled axis
  m_x = m_timeSeries.daysFromFirstReport();
  m_y = m_timeSeries.values();
}

TimeSeriesLinePlotData::~TimeSeriesLinePlotData()
{

}

TimeSeriesLinePlotData* TimeSeriesLinePlotData::copy() const
{
  return (new TimeSeriesLinePlotData(m_timeSeries, m_fracDaysOffset));
}

double TimeSeriesLinePlotData::x(size_t pos) const
{
  return m_x(pos) + m_fracDaysOffset + m_minX; // hourly
}

double TimeSeriesLinePlotData::y(size_t pos) const
{
  return m_y(pos);
}

/// units for plotting on axes or scaling
void TimeSeriesLinePlotData::units(const std::string &unit) { m_units = unit; }

/// units for plotting on axes or scaling
std::string TimeSeriesLinePlotData::units() const { return m_units; }

/// minX
double TimeSeriesLinePlotData::minX() const { return m_minX; }

/// maxX
double TimeSeriesLinePlotData::maxX() const { return m_maxX; }

/// minY
double TimeSeriesLinePlotData::minY() const { return m_minY; }

/// maxY
double TimeSeriesLinePlotData::maxY() const { return m_maxY; }

/// minValue
double TimeSeriesLinePlotData::minValue() const { return m_minValue; }

/// maxValue
double TimeSeriesLinePlotData::maxValue() const { return m_maxValue; }

/// sumValue
double TimeSeriesLinePlotData::sumValue() const
{
  return sum(m_timeSeries.values());
}

/// meanValue
double TimeSeriesLinePlotData::meanValue() const
{
  return mean(m_timeSeries.values());
}

/// stdDevValue
double TimeSeriesLinePlotData::stdDevValue() const
{
  return stdDev(m_timeSeries.values());
}

/// reimplement bounding rect for speed
QRectF TimeSeriesLinePlotData::boundingRect() const 
{ 
  return m_boundingRect; 
}

/// reimplement sample
QPointF TimeSeriesLinePlotData::sample(size_t i) const
{ 
  return QPointF(x(i), y(i)); 
}

/// reimplement abstract function size
size_t TimeSeriesLinePlotData::size(void) const
{ 
  return m_size; 
}

VectorLinePlotData::VectorLinePlotData(const Vector& xVector,
                                       const Vector& yVector)
: m_xVector(xVector),
  m_yVector(yVector),
  m_interpMethod(NearestInterp)
{
  init();
}

VectorLinePlotData* VectorLinePlotData::copy() const
{
  auto result = new VectorLinePlotData(m_xVector, m_yVector);
  result->interpMethod(m_interpMethod);
  return result;
}

/// set the interp method, defaults to Nearest
void VectorLinePlotData::interpMethod(InterpMethod interpMethod)
{
  m_interpMethod = interpMethod;
}

/// minX
double VectorLinePlotData::minX() const
{
  return m_minX;
}

/// maxX
double VectorLinePlotData::maxX() const
{
  return m_maxX;
}

/// minY
double VectorLinePlotData::minY() const
{
  return m_minY;
}

/// maxY
double VectorLinePlotData::maxY() const
{
  return m_maxY;
}

/// minValue
double VectorLinePlotData::minValue() const
{
  return minimum(m_yVector);
}

/// maxValue
double VectorLinePlotData::maxValue() const
{
  return maximum(m_yVector);
}

/// sumValue
double VectorLinePlotData::sumValue() const
{
  return sum(m_yVector);
}

/// meanValue
double VectorLinePlotData::meanValue() const
{
  return mean(m_yVector);
}

/// stdDevValue
double VectorLinePlotData::stdDevValue() const
{
  return 0;
}

QRectF VectorLinePlotData::boundingRect() const
{ 
  return m_boundingRect; 
}

QPointF VectorLinePlotData::sample(size_t i) const
{ 
  return QPointF(x(i), y(i)); 
}

size_t VectorLinePlotData::size(void) const
{ 
  return m_size; 
}

// set ranges and bounding box
void VectorLinePlotData::init(){

  unsigned M = m_xVector.size();
  unsigned N = m_yVector.size();

  if ((M <= 1) || (N <= 1) ||  (M != N)){
    throw std::runtime_error("Incorrectly sized matrix or vector for VectorLinePlotData");
  }

  m_size = N;
  m_minX = minimum(m_xVector);
  m_maxX = maximum(m_xVector);
  m_minY = minimum(m_yVector);
  m_maxY = maximum(m_yVector);

  // set the bounding box
  m_boundingRect = QRectF(m_minX, m_minY, (m_maxX - m_minX), (m_maxY - m_minY));
}

 double VectorLinePlotData::x(size_t pos) const
{
  return m_xVector(pos);
}

double VectorLinePlotData::y(size_t pos) const
{
  return m_yVector(pos);
}

/// units for plotting on axes or scaling
void VectorLinePlotData::units(const std::string &unit) { m_units = unit; };

/// units for plotting on axes or scaling
std::string VectorLinePlotData::units() const { return m_units; };

} // openstudio
