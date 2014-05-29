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

#include <gtest/gtest.h>

#include "../FloodPlot.hpp"

#include "../../data/Matrix.hpp"
#include "../../core/Application.hpp"
#include "../../data/Vector.hpp"

using namespace std;
using namespace boost;
using namespace openstudio;

TEST(FloodPlot, MatrixFloodPlotData)
{
  Matrix matrix(10,10);
  for (unsigned i = 0; i < 10; ++i){
    for (unsigned j = 0; j < 10; ++j){
      matrix(i,j) = 10*i+j;
    }
  }

  MatrixFloodPlotData::Ptr data = MatrixFloodPlotData::create(matrix);

  FloodPlot::Ptr fp = FloodPlot::create();
  fp->floodPlotData(data);
  fp->generateImage(toPath("testMatrixFloodPlotData.png"));

  // passes the visual test
  EXPECT_TRUE(true);
};

TEST(FloodPlot, MatrixFloodPlotDataCountourLevels)
{
  Matrix matrix(10,10);
  for (unsigned i = 0; i < 10; ++i){
    for (unsigned j = 0; j < 10; ++j){
      matrix(i,j) = 10*i+j;
    }
  }
  Vector contourLevels(5);
  contourLevels(0) = 0;
  contourLevels(1) = 25;
  contourLevels(2) = 50;
  contourLevels(3) = 75;
  contourLevels(4) = 100;

  MatrixFloodPlotData::Ptr data = MatrixFloodPlotData::create(matrix);
  data->interpMethod(LinearInterp);

  FloodPlot::Ptr fp = FloodPlot::create();
  fp->floodPlotData(data);
  fp->contourLevels(contourLevels);
  fp->colorMapRange(45,75);
  fp->showContour(true);
  
  fp->generateImage(toPath("testMatrixFloodPlotDataCountourLevels.png"));

  // passes the visual test
  EXPECT_TRUE(true);
};


TEST(FloodPlot, MatrixFloodPlotData2)
{
  Matrix matrix(10,10);
  for (unsigned i = 0; i < 10; ++i){
    for (unsigned j = 0; j < 10; ++j){
      matrix(i,j) = 10*i+j;
    }
  }

  Vector xVector(linspace(-100, 0, 10));
  Vector yVector(linspace(0, 100, 10));
  MatrixFloodPlotData::Ptr data = MatrixFloodPlotData::create(xVector, yVector, matrix);
  data->interpMethod(LinearInterp);

  FloodPlot::Ptr fp = FloodPlot::create();
  fp->floodPlotData(data);
  fp->bottomAxisTitle("X axis");
  fp->leftAxisTitle("Y axis");
  fp->generateImage(toPath("testMatrixFloodPlotData2.png"));

  // passes the visual test
  EXPECT_TRUE(true);
};

TEST(FloodPlot, EricDataPlot)
{
  Vector x(3); x(0) = 3; x(1) = 4; x(2) = 5;
  Vector y(3); y(0) = 0.25; y(1) = 0.5; y(2) = 0.75;
  
  Matrix m(3,3);
  m(0,0) = 1.01328e+011; m(0,1) = 1.25153e+011; m(0,2) = 1.48116e+011;
  m(1,0) = 1.06746e+011; m(1,1) = 1.32534e+011; m(1,2) = 1.52106e+011;
  m(2,0) = 1.11509e+011; m(2,1) = 1.39306e+011; m(2,2) = 1.5545e+011;
  
  MatrixFloodPlotData::Ptr data = MatrixFloodPlotData::create(x,y,m);
  //MatrixFloodPlotData::Ptr data = MatrixFloodPlotData::create(m); // this one works (still has weird solid pixel in upper right)
  data->interpMethod(LinearInterp);

  FloodPlot::Ptr fp = FloodPlot::create();
  fp->floodPlotData(data);
  fp->generateImage(toPath("testEricData.png"));

  // does not pass the visual test
  EXPECT_TRUE(true);
};


class MandelbrotFloodPlotData: public FloodPlotData
{
public:

  COMMON_PTR_TYPEDEFS(MandelbrotFloodPlotData)

  /// constructor
  MandelbrotFloodPlotData(unsigned maxN)
  : m_maxN(maxN),
    m_minValue(0),
    m_maxValue(maxN)
  {
    m_colorMapRange = QwtDoubleInterval(0,maxN);
  }

  /// create
  static Ptr create(unsigned maxN)
  {return Ptr(new MandelbrotFloodPlotData(maxN));}

  /// must provide copy
  virtual MandelbrotFloodPlotData* copy() const 
  {
    MandelbrotFloodPlotData* result = new MandelbrotFloodPlotData(m_maxN);
    result->colorMapRange(m_colorMapRange);
    return result;
  }

  /// get the value at point x, y
  double value(double x, double y) const
  {
    double x0 = x;
    double y0 = y;
    unsigned N = 0;
    while ( (x*x + y*y <= (2*2))  &&  (N < m_maxN) ) 
    {
      double xtemp = x*x - y*y + x0;
      y = 2*x*y + y0;
      x = xtemp;
      ++N;
    }

    if (N == m_maxN){
      N = 0;
    }

    return N;
  }

  /// minX
  double minX() const {return -2;};

  /// maxX
  double maxX() const {return 2;};

  /// minY
  double minY() const {return -2;};

  /// maxY
  double maxY() const {return 2;};

  /// minValue
  double minValue() const {return 0;};

  /// maxValue
  double maxValue() const {return m_maxN;};

  /// minValue setter
  virtual void minValue(double min) {m_minValue = min;};

  /// maxValue setter
  virtual void maxValue(double max) {m_maxValue = max;};

//  QwtDoubleInterval range() const {return QwtDoubleInterval(m_minValue, m_maxValue);}
  QwtDoubleInterval range() const {return m_colorMapRange;}
   
  /// sumValue
  double sumValue() const {return 0;}

  /// meanValue
  double meanValue() const {return 0;}

  /// stdDevValue
  double stdDevValue() const {return 0;}

  /// color map range 
  void colorMapRange(QwtDoubleInterval colorMapRange) {m_colorMapRange = colorMapRange;}

  /// color map range 
   QwtDoubleInterval colorMapRange() {return m_colorMapRange;}

   /// units 
   std::string units() const {return m_units;}

private:

  unsigned m_maxN;
  double m_minValue;
  double m_maxValue;
  QwtDoubleInterval m_colorMapRange;
  std::string m_units;
};


TEST(FloodPlot, MandelbrotFloodPlotData)
{
  MandelbrotFloodPlotData::Ptr data = MandelbrotFloodPlotData::create(16);
  FloodPlot::Ptr fp = FloodPlot::create();
  fp->floodPlotData(data);
  fp->generateImage(toPath("testMandelbrotFloodPlotData16.png"));

  data = MandelbrotFloodPlotData::create(160);
  fp->floodPlotData(data);
  fp->generateImage(toPath("testMandelbrotFloodPlotData160.png"));

  // passes the visual test
  EXPECT_TRUE(true);
};

TEST(FloodPlot, EricDataPlot1)
{
  Vector x(3); x(0) = 3; x(1) = 4; x(2) = 5;
  Vector y(3); y(0) = 0.25; y(1) = 0.5; y(2) = 0.75;
  
  Matrix m(3,3);
  m(0,0) = 1.01328e+011; m(0,1) = 1.25153e+011; m(0,2) = 1.48116e+011;
  m(1,0) = 1.06746e+011; m(1,1) = 1.32534e+011; m(1,2) = 1.52106e+011;
  m(2,0) = 1.11509e+011; m(2,1) = 1.39306e+011; m(2,2) = 1.5545e+011;
  
  MatrixFloodPlotData::Ptr data = MatrixFloodPlotData::create(m); 
  data->interpMethod(LinearInterp);

  FloodPlot::Ptr fp = FloodPlot::create();
  fp->floodPlotData(data);
  fp->generateImage(toPath("testEricData1.png"));

  // passes the visual test
  EXPECT_TRUE(true);
};

TEST(FloodPlot, EricDataPlot2)
{
  Vector x(3); x(0) = 3; x(1) = 4; x(2) = 5;
  Vector y(3); y(0) = 0.25; y(1) = 0.5; y(2) = 0.75;
  
  Matrix m(3,3);
  m(0,0) = 1.01328e+011; m(0,1) = 1.25153e+011; m(0,2) = 1.48116e+011;
  m(1,0) = 1.06746e+011; m(1,1) = 1.32534e+011; m(1,2) = 1.52106e+011;
  m(2,0) = 1.11509e+011; m(2,1) = 1.39306e+011; m(2,2) = 1.5545e+011;
 
  EXPECT_DOUBLE_EQ(1.5545e+011, interp(x, y, m, 5.5, 0.8, LinearInterp, NearestExtrap));
  
  MatrixFloodPlotData::Ptr data = MatrixFloodPlotData::create(x,y,m);
  data->interpMethod(LinearInterp);

  FloodPlot::Ptr fp = FloodPlot::create();
  fp->floodPlotData(data);
  fp->generateImage(toPath("testEricData2.png"));

  // passes the visual test
  EXPECT_TRUE(true);
};

TEST(FloodPlot, MatrixConstructor)
{
  Matrix matrix(10,10);
  for (unsigned i = 0; i < 10; ++i){
    for (unsigned j = 0; j < 10; ++j){
      matrix(i,j) = 10*i+j;
    }
  }

  FloodPlot::Ptr fp = FloodPlot::create(matrix);
  fp->generateImage(toPath("testMatrixConstructor.png"));

  // passes the visual test
  EXPECT_TRUE(true);
};

TEST(FloodPlot, LinearScaleColorLevels)
{
  Matrix matrix(10,10);
  for (unsigned i = 0; i < 10; ++i){
    for (unsigned j = 0; j < 10; ++j){
      matrix(i,j) = 10*i+j;
    }
  }

  FloodPlot::Ptr fp = FloodPlot::create(matrix);
  fp->colorMapRange(20,60);
  Vector ls;
  ls = linspace( 20,60,20 );
  fp->colorLevels(ls);
  fp->generateImage(toPath("testLinearScaleColorLevels.png"));

  // fails the visual test - color map not restricted to correct range
  EXPECT_TRUE(true);
};

TEST(FloodPlot, LinearScaleColorLevelsContourLevels)
{
  Matrix matrix(10,10);
  for (unsigned i = 0; i < 10; ++i){
    for (unsigned j = 0; j < 10; ++j){
      matrix(i,j) = 10*i+j;
    }
  }

  FloodPlot::Ptr fp = FloodPlot::create(matrix);
  fp->colorMapRange(20,60);

  Vector ls;
  ls = linspace( 20,60,20 );
  fp->contourLevels(ls);
  fp->colorLevels(ls);
  fp->showContour(true);
  fp->generateImage(toPath("testLinearScaleColorLevelsContourLevels.png"));

  // fails the visual test - color map not restricted to correct range - contours correct color incorrect
  EXPECT_TRUE(true);
};

TEST(FloodPlot, LogScaleColorLevels)
{
  Matrix matrix(10,10);
  for (unsigned i = 0; i < 10; ++i){
    for (unsigned j = 0; j < 10; ++j){
      matrix(i,j) = 10*i+j;
    }
  }

  FloodPlot::Ptr fp = FloodPlot::create(matrix);
  Vector ls;
  ls = logspace(0,2,20);
  fp->colorLevels(ls);
  fp->generateImage(toPath("testLogScaleColorLevels.png"));

  // passes the visual test
  EXPECT_TRUE(true);
};

TEST(FloodPlot, LogScaleColorLevelsContourLevels)
{
  Matrix matrix(10,10);
  for (unsigned i = 0; i < 10; ++i){
    for (unsigned j = 0; j < 10; ++j){
      matrix(i,j) = 10*i+j;
    }
  }

  FloodPlot::Ptr fp = FloodPlot::create(matrix);
  fp->colorMapRange(1,100);
  Vector ls;
  ls = logspace(0,2,20);
  fp->colorLevels(ls);
  fp->contourLevels(ls);
  fp->showContour(true);
  fp->generateImage(toPath("testLogScaleColorLevelsContourLevels.png"));

  // passes the visual test
  EXPECT_TRUE(true);
};


TEST(FloodPlot, TimeSeriesFloodPlot_Detailed_Year)
{
  // Jan, 1, 2009 - day 1
  int startHour = 1;
  int startDay = 1;
  DateTimeVector dateTimes(8760);
  Vector values(8760);
  Date startDate(MonthOfYear::Jan, startDay);
  DateTime startDateTime(startDate, Time(0,startHour,0,0));
  for(unsigned i = 0; i < 8760; ++i){
    // every day is the same, it should be 0 until hour 1, 1 until hour 2, 2 until hour 3, ..., 23 until hour 24
    dateTimes[i] = startDateTime + Time(0,i,0,0);
    values[i] = i % 24;
  }
  
  TimeSeries ts(dateTimes, values, "");
  TimeSeriesFloodPlotData::Ptr data = TimeSeriesFloodPlotData::create(ts);
  
  double epsilon=1.5/3600.0; // 1.5 seconds in hours
  EXPECT_EQ(1, startDate.dayOfYear());
  EXPECT_DOUBLE_EQ(1, data->minX());
  EXPECT_DOUBLE_EQ(1+365, data->maxX()); // days
  EXPECT_DOUBLE_EQ(0.0, data->minY());
  EXPECT_DOUBLE_EQ(24.0, data->maxY()); // hours
  EXPECT_DOUBLE_EQ(0.0, data->minValue());
  EXPECT_DOUBLE_EQ(23.0, data->maxValue()); // values

  EXPECT_DOUBLE_EQ(0.0, data->value(1,0.0));
  EXPECT_DOUBLE_EQ(0.0, data->value(1,0.0+epsilon));

  EXPECT_DOUBLE_EQ(0.0, data->value(1,1.0-epsilon));
  EXPECT_DOUBLE_EQ(0.0, data->value(1,1.0));
  EXPECT_DOUBLE_EQ(1.0, data->value(1,1.0+epsilon));

  EXPECT_DOUBLE_EQ(1.0, data->value(1,2.0-epsilon));
  EXPECT_DOUBLE_EQ(1.0, data->value(1,2.0));
  EXPECT_DOUBLE_EQ(2.0, data->value(1,2.0+epsilon));

  EXPECT_DOUBLE_EQ(4.0, data->value(1,5.0-epsilon));
  EXPECT_DOUBLE_EQ(4.0, data->value(1,5.0));
  EXPECT_DOUBLE_EQ(5.0, data->value(1,5.0+epsilon));

  EXPECT_DOUBLE_EQ(11.0, data->value(212,12.0-epsilon));
  EXPECT_DOUBLE_EQ(11.0, data->value(212,12.0));
  EXPECT_DOUBLE_EQ(12.0, data->value(212,12.0+epsilon));

  EXPECT_DOUBLE_EQ(21.0, data->value(215.0,22.0-epsilon)); 
  EXPECT_DOUBLE_EQ(21.0, data->value(215.0,22.0)); 
  EXPECT_DOUBLE_EQ(22.0, data->value(215.0,22.0+epsilon)); 

  EXPECT_DOUBLE_EQ(22.0, data->value(216.0,23.0-epsilon)); 
  EXPECT_DOUBLE_EQ(22.0, data->value(216.0,23.0)); 
  EXPECT_DOUBLE_EQ(23.0, data->value(216.0,23.0+epsilon)); 

  EXPECT_DOUBLE_EQ(22.0, data->value(2.0,23.0-epsilon));
  EXPECT_DOUBLE_EQ(22.0, data->value(2.0,23.0));
  EXPECT_DOUBLE_EQ(23.0, data->value(2.0,23.0+epsilon));

  EXPECT_DOUBLE_EQ(0.0, data->value(1,25.0-epsilon)); // hour 1
  EXPECT_DOUBLE_EQ(0.0, data->value(1,25.0));  // hour 1
  EXPECT_DOUBLE_EQ(1.0, data->value(1,25.0+epsilon)); // hour 1
 
  EXPECT_DOUBLE_EQ(23.0, data->value(1,24.0-epsilon)); // hour 24
  EXPECT_DOUBLE_EQ(23.0, data->value(1,24.0)); // hour 24
  EXPECT_DOUBLE_EQ(0.0, data->value(1,24.0+epsilon)); // hour 24

  EXPECT_DOUBLE_EQ(22.0, data->value(364.0,23.0-epsilon));
  EXPECT_DOUBLE_EQ(22.0, data->value(364.0,23.0));
  EXPECT_DOUBLE_EQ(23.0, data->value(364.0,23.0+epsilon));

  EXPECT_DOUBLE_EQ(23.0, data->value(364.0,24.0-epsilon)); // hour 24
  EXPECT_DOUBLE_EQ(23.0, data->value(364.0,24.0)); // hour 24
  EXPECT_DOUBLE_EQ(0.0, data->value(364.0,24.0+epsilon)); // hour 24

  FloodPlot::Ptr fp = FloodPlot::create();
  std::string name = "TimeSeries Detailed Year Flood Plot Test";
  fp->floodPlotData(data);
  fp->axesFontSize(12);
  fp->tickFontSize(10);
  fp->bottomAxisTitle("Date");
  fp->leftAxisTitle("Hour");
  fp->generateImage(toPath("testTimeSeriesFloodPlot_DetailedYear.png"));
// need diff on images for regression testing
}


TEST(FloodPlot, TimeSeriesFloodPlot_Detailed_Year_212plus10)
{
  // July, 31, 2009 - day 212
  int startHour = 10;
  int startDay = 31;
  DateTimeVector dateTimes(8760);
  Vector values(8760);
  Date startDate(MonthOfYear::Jul, startDay);
  DateTime startDateTime(startDate, Time(0,startHour,0,0));
  for(unsigned i = 0; i < 8760; ++i){
    dateTimes[i] = startDateTime + Time(0,i,0,0);
    values[i] = i % 24;
  }
  
  TimeSeries ts(dateTimes, values, "");
  ts.setOutOfRangeValue(-99);
  TimeSeriesFloodPlotData::Ptr data = TimeSeriesFloodPlotData::create(ts);
  
  double epsilon=1.5/3600.0; // 1.5 seconds in hours
  EXPECT_EQ(212, startDate.dayOfYear());
  EXPECT_DOUBLE_EQ(212, data->minX());
  EXPECT_DOUBLE_EQ(212+366, data->maxX());
  EXPECT_DOUBLE_EQ(0.0, data->minY());
  EXPECT_DOUBLE_EQ(24.0, data->maxY());
  EXPECT_DOUBLE_EQ(0.0, data->minValue());
  EXPECT_DOUBLE_EQ(23.0, data->maxValue());

  EXPECT_DOUBLE_EQ(-99, data->value(1.0, 1.0)); 

  EXPECT_DOUBLE_EQ(-99, data->value(212,5.0));

  EXPECT_DOUBLE_EQ(-99, data->value(212,startHour-epsilon));
  EXPECT_DOUBLE_EQ(0.0, data->value(212,startHour));
  EXPECT_DOUBLE_EQ(1.0, data->value(212,startHour+epsilon));

  EXPECT_DOUBLE_EQ(1.0, data->value(212,10.5));

  EXPECT_DOUBLE_EQ(2.0, data->value(212,12.0-epsilon));
  EXPECT_DOUBLE_EQ(2.0, data->value(212,12.0));
  EXPECT_DOUBLE_EQ(3.0, data->value(212,12.0+epsilon));

  EXPECT_DOUBLE_EQ(12.0, data->value(215.0,22.0-epsilon)); 
  EXPECT_DOUBLE_EQ(12.0, data->value(215.0,22.0)); 
  EXPECT_DOUBLE_EQ(13.0, data->value(215.0,22.0+epsilon)); 

  EXPECT_DOUBLE_EQ(13.0, data->value(216.0,23.0-epsilon)); 
  EXPECT_DOUBLE_EQ(13.0, data->value(216.0,23.0)); 
  EXPECT_DOUBLE_EQ(14.0, data->value(216.0,23.0+epsilon)); 

  EXPECT_DOUBLE_EQ(13.0, data->value(365.0,-1.0-epsilon));
  EXPECT_DOUBLE_EQ(13.0, data->value(365.0,-1.0));
  EXPECT_DOUBLE_EQ(14.0, data->value(365.0,-1.0+epsilon));

  EXPECT_DOUBLE_EQ(13.0, data->value(364.0,23.0-epsilon));
  EXPECT_DOUBLE_EQ(13.0, data->value(364.0,23.0));
  EXPECT_DOUBLE_EQ(14.0, data->value(364.0,23.0+epsilon));

  EXPECT_DOUBLE_EQ(14.0, data->value(364.0,24.0-epsilon));
  EXPECT_DOUBLE_EQ(14.0, data->value(364.0,24.0));
  EXPECT_DOUBLE_EQ(15.0, data->value(364.0,24.0+epsilon));

  EXPECT_DOUBLE_EQ(15.0, data->value(365.0,-23.0-epsilon)); 
  EXPECT_DOUBLE_EQ(15.0, data->value(365.0,-23.0));
  EXPECT_DOUBLE_EQ(16.0, data->value(365.0,-23.0+epsilon));

  EXPECT_DOUBLE_EQ(15.0, data->value(364.0,1.0-epsilon)); 
  EXPECT_DOUBLE_EQ(15.0, data->value(364.0,1.0)); 
  EXPECT_DOUBLE_EQ(16.0, data->value(364.0,1.0+epsilon)); 

  FloodPlot::Ptr fp = FloodPlot::create();
  std::string name = "TimeSeries Detailed Year 212 plus 10 Flood Plot Test";
  fp->floodPlotData(data);
  fp->axesFontSize(12);
  fp->tickFontSize(10);
  fp->bottomAxisTitle("Date");
  fp->leftAxisTitle("Hour");
  fp->generateImage(toPath("testTimeSeriesFloodPlot_DetailedYear212plus10.png"));
// need diff on images for regression testing
}

TEST(FloodPlot, TimeSeriesFloodPlot_Detailed_Day)
{
  int startHour = 10;
  int startDay = 1;
  DateTimeVector dateTimes(24);
  Vector values(24);
  Date startDate(MonthOfYear::Jun, startDay);
  DateTime startDateTime(startDate, Time(0,startHour,0,0));
  for(unsigned i = 0; i < 24; ++i){
    dateTimes[i] = startDateTime + Time(0,i,0,0);
    values[i] = i % 24;
  }
  
  TimeSeries ts(dateTimes, values, "");
  TimeSeriesFloodPlotData::Ptr data = TimeSeriesFloodPlotData::create(ts);
  
  double epsilon=1.5/3600.0; // 1.5 seconds in hours
  EXPECT_EQ(152, startDate.dayOfYear());
  EXPECT_DOUBLE_EQ(152, data->minX());
  EXPECT_DOUBLE_EQ(154, data->maxX());
  EXPECT_DOUBLE_EQ(0.0, data->minY());
  EXPECT_DOUBLE_EQ(24.0, data->maxY());
  EXPECT_DOUBLE_EQ(0.0, data->minValue());
  EXPECT_DOUBLE_EQ(23.0, data->maxValue());

  EXPECT_DOUBLE_EQ(0.0, data->value(startDate.dayOfYear(),startHour-epsilon));
  EXPECT_DOUBLE_EQ(0.0, data->value(startDate.dayOfYear(),startHour));
  EXPECT_DOUBLE_EQ(1.0, data->value(startDate.dayOfYear(),startHour+epsilon));

  EXPECT_DOUBLE_EQ(1.0, data->value(startDate.dayOfYear(),startHour+1-epsilon));
  EXPECT_DOUBLE_EQ(1.0, data->value(startDate.dayOfYear(),startHour+1));
  EXPECT_DOUBLE_EQ(2.0, data->value(startDate.dayOfYear(),startHour+1+epsilon));

  EXPECT_DOUBLE_EQ(14.0, data->value(153,0.0-epsilon));
  EXPECT_DOUBLE_EQ(14.0, data->value(153,0.0));
  EXPECT_DOUBLE_EQ(15.0, data->value(153,0.0+epsilon));

  EXPECT_DOUBLE_EQ(15.0, data->value(153,0.5));

  EXPECT_DOUBLE_EQ(15.0, data->value(153,1.0-epsilon));
  EXPECT_DOUBLE_EQ(15.0, data->value(153,1.0));
  EXPECT_DOUBLE_EQ(16.0, data->value(153,1.0+epsilon));

  EXPECT_DOUBLE_EQ(16.0, data->value(153,2.0-epsilon));
  EXPECT_DOUBLE_EQ(16.0, data->value(153,2.0));
  EXPECT_DOUBLE_EQ(17.0, data->value(153,2.0+epsilon));

  EXPECT_DOUBLE_EQ(17.0, data->value(153,2.5));

  EXPECT_DOUBLE_EQ(14.0, data->value(152,23.5));

  EXPECT_DOUBLE_EQ(15.0, data->value(152, 25.0-epsilon));

  FloodPlot::Ptr fp = FloodPlot::create();
  std::string name = "TimeSeries Detailed Day Flood Plot Test";
  fp->floodPlotData(data);
  fp->axesFontSize(12);
  fp->tickFontSize(10);
  fp->bottomAxisTitle("Date");
  fp->leftAxisTitle("Hour");
  fp->generateImage(toPath("testTimeSeriesFloodPlot_DetailedDay.png"));
// need diff on images for regression testing
}
