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

#include "LinePlot.hpp"
#include <cfloat>
#include "../core/Application.hpp"
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
  m_boundingRect = QwtDoubleRect(m_minX, m_minY, (m_maxX - m_minX), (m_maxY - m_minY));
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
  m_boundingRect = QwtDoubleRect(m_minX, m_minY, (m_maxX - m_minX), (m_maxY - m_minY));
  m_minValue = m_minY;
  m_maxValue = m_maxY;
  m_units = timeSeries.units();
  m_fracDaysOffset = fracDaysOffset; // note updating in xValue does not affect scaled axis
  m_x = m_timeSeries.daysFromFirstReport();
  m_y = m_timeSeries.values();
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
  m_boundingRect = QwtDoubleRect(m_minX, m_minY, (m_maxX - m_minX), (m_maxY - m_minY));
}

 double VectorLinePlotData::x(size_t pos) const
{
  return m_xVector(pos);
}

double VectorLinePlotData::y(size_t pos) const
{
  return m_yVector(pos);
}


/* --------------------------------------
 * LinePlot Class
 * --------------------------------------
*/

LinePlot::LinePlot(QWidget* parent /*= 0*/, Qt::WindowFlags flags /*= 0*/) : Plot2D(parent, flags)
{
  init();
}

LinePlot::Ptr LinePlot::create(QWidget* parent, Qt::WindowFlags flags)
{
  Application::instance().application();
  return LinePlot::Ptr(new LinePlot(parent, flags));
}

void LinePlot::init()
{
  m_plot2DTimeAxis = nullptr;
  // destroy windows when closed
  setAttribute(Qt::WA_DeleteOnClose);
  m_qwtPlot->plotLayout()->setAlignCanvasToScales(true);
  // legend - subclass qwtLegend to overwrite paint event
  // m_legend = new Plot2DLegend(this);
  m_legend = new QwtLegend(this);
  m_legend->setItemMode(QwtLegend::CheckableItem);
  //  m_legend->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  //  m_legend->setGeometry(120,100,150,150);
  QPalette p(palette());
  p.setColor(QPalette::Background, Qt::white);
  m_legend->setPalette(p);
  // m_qwtPlot->insertLegend(m_legend, QwtPlot::ExternalLegend);
  m_qwtPlot->insertLegend(m_legend, QwtPlot::BottomLegend);
  // m_legend->show();
  // for window menu in treeview application
  setAttribute(Qt::WA_DeleteOnClose);

  /// left and right axes units
  m_leftAxisUnits = "NONE SPECIFIED";
  m_rightAxisUnits = "NONE SPECIFIED";

  m_xAxisMin = DBL_MAX;
  m_xAxisMax = -DBL_MAX;

  // base on data
  this->bottomAxisTitle("Hour of Year");

  /// legend checked event
  connect(m_qwtPlot, SIGNAL(legendChecked(QwtPlotItem *, bool)), SLOT(showCurve(QwtPlotItem *, bool)));

  m_colorVec.push_back(Qt::black);
  m_colorVec.push_back(Qt::blue);
  m_colorVec.push_back(Qt::cyan);
  m_colorVec.push_back(Qt::magenta);
  m_colorVec.push_back(Qt::yellow);
  m_colorVec.push_back(Qt::darkBlue);
  m_colorVec.push_back(Qt::darkCyan);
  m_colorVec.push_back(Qt::darkMagenta);
  m_colorVec.push_back(Qt::darkYellow);


  // plot grid
  m_grid = new QwtPlotGrid();
  QPen gridPen(Qt::gray);
  gridPen.setCosmetic(true);
  gridPen.setWidth(1);
  gridPen.setStyle(Qt::DotLine);
  m_grid->setPen(gridPen);
  m_grid->attach(m_qwtPlot);

  
}



void LinePlot::timeseriesData(TimeSeries tsData, const std::string& name, QColor color)
{
  if (tsData.values().empty()){
    return;
  }

  double offset=0.0;
  if (m_plot2DTimeAxis == nullptr) 
  {
    m_startDateTime = tsData.firstReportDateTime();
    m_endDateTime = tsData.firstReportDateTime() + Time(tsData.daysFromFirstReport(tsData.daysFromFirstReport().size()-1));
    m_duration = (m_endDateTime-m_startDateTime).totalDays();
    m_plot2DTimeAxis = new Plot2DTimeAxis(m_startDateTime, m_duration);
    m_qwtPlot->setAxisTitle(QwtPlot::xBottom, " Simulation Time");
    m_qwtPlot->setAxisScaleDraw(QwtPlot::xBottom, m_plot2DTimeAxis);
    m_xAxisMin = 0.0;
    m_xAxisMax = m_duration;
    m_qwtPlot->setAxisScale(QwtPlot::xBottom, 0, m_duration);
    m_qwtPlot->setAxisLabelRotation(QwtPlot::xBottom, -90.0);
    m_qwtPlot->setAxisLabelAlignment(QwtPlot::xBottom, Qt::AlignLeft | Qt::AlignBottom);
  } 
  else 
  {
    if (tsData.firstReportDateTime() < m_startDateTime) {
      m_xAxisMin = (tsData.firstReportDateTime() - m_startDateTime).totalDays();
      offset = m_xAxisMin;
    } else {
      offset = (tsData.firstReportDateTime() - m_startDateTime).totalDays();
    }

    if ((tsData.firstReportDateTime() + Time(tsData.daysFromFirstReport(tsData.daysFromFirstReport().size()-1))) > m_endDateTime) {
      m_xAxisMax += ((tsData.firstReportDateTime() + Time(tsData.daysFromFirstReport(tsData.daysFromFirstReport().size()-1))) - m_endDateTime).totalDays();
    }
  }

  TimeSeriesLinePlotData::Ptr data = TimeSeriesLinePlotData::create(tsData, offset);
  linePlotData(data, name, color, offset);
}


QColor LinePlot::curveColor(QColor &lastColor)
{
  auto colorIt = std::find(m_colorVec.begin(), m_colorVec.end(), lastColor);
  if ( (colorIt == m_colorVec.end()) ||  (*colorIt == m_colorVec.back()) ) {
    return (m_colorVec.at(0));
  } else {
    ++colorIt;
    return *colorIt;
  }
}

void LinePlot::setLineThickness(const int &width)
{
  const QwtPlotItemList &listPlotItem = m_qwtPlot->itemList();
  QwtPlotItemIterator itPlotItem;
  for (itPlotItem = listPlotItem.begin();itPlotItem!=listPlotItem.end();++itPlotItem)
  {
    QwtPlotItem *plotItem = *itPlotItem;
    if ( plotItem->rtti() == QwtPlotItem::Rtti_PlotCurve)
    {
      QwtPlotCurve *curve = (QwtPlotCurve *) plotItem;
      QPen pen(curve->pen()); 
      pen.setWidth(width);
      curve->setPen(pen);
    }
  }
  m_qwtPlot->replot();

}


void LinePlot::linePlotData(LinePlotData::Ptr data,  const std::string& name, QColor color, double offset )
{
  if (!data) return;


  // set based on data - record overall min and max
  if ( (data->minX() < m_xAxisMin) || (data->maxX() > m_xAxisMax) )
  {
    if  (data->minX() < m_xAxisMin) m_xAxisMin = data->minX();
    if  (data->maxX() > m_xAxisMax) m_xAxisMax = data->maxX();
  }
  if (numberOfCurves() == 0)
  {
    m_lineThickness = 10;
  }


  /// todo - curve collection - shared pointers
  QwtPlotCurve * curve = new QwtPlotCurve(toQString(name));
  if (color == Qt::color0) 
  { // generate new color from color map
    color = curveColor(m_lastColor);
    m_lastColor = color;
  }
  curve->setPen(curvePen(color));
  curve->attach(m_qwtPlot);
  curve->setData(*data);

  // check for number of different units (std::string  based)
  QString curveUnits = toQString(data->units());
  if (m_leftAxisUnits == "NONE SPECIFIED")
  {
    m_leftAxisUnits = curveUnits;
    this->leftAxisTitleFromUnits(toString(m_leftAxisUnits));
    curve->setYAxis(QwtPlot::yLeft);
    m_qwtPlot->enableAxis(QwtPlot::yRight, false);
  }
  else if (m_leftAxisUnits.toUpper() == curveUnits.toUpper())
  {
    curve->setYAxis(QwtPlot::yLeft);
    m_qwtPlot->enableAxis(QwtPlot::yRight, false);
  }
  else if (m_rightAxisUnits == "NONE SPECIFIED")
  {
    m_rightAxisUnits = curveUnits;
    this->rightAxisTitleFromUnits(toString(m_rightAxisUnits));
    curve->setYAxis(QwtPlot::yRight);
    m_qwtPlot->enableAxis(QwtPlot::yRight, true);
  }
  else if (m_rightAxisUnits.toUpper() == curveUnits.toUpper())
  {
    curve->setYAxis(QwtPlot::yRight);
    m_qwtPlot->enableAxis(QwtPlot::yRight, true);
  }
  else // more than 2 units - scale all curves
  {
    scaleCurves(curve);
    m_qwtPlot->enableAxis(QwtPlot::yRight, false);
    this->leftAxisTitle("Scaled");
  }



/// update legend and replot
  showCurve(curve, true);

//  initZoomer();

}

QPen LinePlot::curvePen(QColor &color)
{
// trac ticket 170
  QPen pen(color);
  pen.setCosmetic(true);
  if (m_lineThickness > 2)
    m_lineThickness-=2;
  else
    m_lineThickness=8;
  int numCurves = numberOfCurves();
  if (numCurves < 4)
    pen.setStyle(Qt::SolidLine);
  else if (numCurves < 8)
    pen.setStyle(Qt::DotLine);
  else if (numCurves < 12)
    pen.setStyle(Qt::DashLine);
  else
    pen.setStyle(Qt::DashDotLine);
  pen.setWidth(m_lineThickness);
  return pen;
}


int LinePlot::numberOfCurves()
{
  /// multiple curves based on units
  const QwtPlotItemList &listPlotItem = m_qwtPlot->itemList();
  QwtPlotItemIterator itPlotItem;
  int curveCount = 0;
  for (itPlotItem = listPlotItem.begin();itPlotItem!=listPlotItem.end();++itPlotItem)
  {
    QwtPlotItem *plotItem = *itPlotItem;
    if ( plotItem->rtti() == QwtPlotItem::Rtti_PlotCurve)
    {
      curveCount++;
    }
  }
  return curveCount;
}


void LinePlot::scaleCurves(QwtPlotCurve *curve)
{

  /// multiple curves based on units
  const QwtPlotItemList &listPlotItem = m_qwtPlot->itemList();
  QwtPlotCurve *plotCurve;
  QwtPlotItemIterator itPlotItem;
  int curveCount = numberOfCurves();

  switch (curveCount)
  {
    case 0:
    {
      curve->setYAxis(QwtPlot::yLeft);
      m_qwtPlot->enableAxis(QwtPlot::yRight, false);
      break;
    }
    case 1:
    {
      curve->setYAxis(QwtPlot::yRight);
      m_qwtPlot->enableAxis(QwtPlot::yRight, true);
      break;
    }
    default: //scale
      m_qwtPlot->enableAxis(QwtPlot::yRight, false);
      // find min, max of all curves
      // scale
      int i;
      for ( itPlotItem = listPlotItem.begin();itPlotItem!=listPlotItem.end();++itPlotItem)
      {
        if ( (*itPlotItem)->rtti() == QwtPlotItem::Rtti_PlotCurve)
        {
          plotCurve = (QwtPlotCurve*) (*itPlotItem);

          if ((plotCurve->minYValue() != 0) || (plotCurve->maxYValue() != 1))
          {

            QwtArray<double> xData(plotCurve->dataSize());
            QwtArray<double> yData(plotCurve->dataSize());
            for (i = 0; i < plotCurve->dataSize(); i++)
            {
              xData[i] = plotCurve->x(i);
              yData[i] = (plotCurve->y(i) - plotCurve->minYValue())/ (plotCurve->maxYValue() - plotCurve->minYValue());
            }
            // reset data
            plotCurve->setTitle(plotCurve->title().text() + "[" + QString::number(plotCurve->minYValue()) + ", "  + QString::number(plotCurve->maxYValue()) + "]");
            plotCurve->setData(xData,yData);
          }
        }
      }
      break;
  }

}

void LinePlot::linePlotLeftRightAxesData(LinePlotData::Ptr leftAxisData, LinePlotData::Ptr rightAxisData, const std::string& leftName, const std::string& rightName, QColor leftColor, QColor rightColor)
{
  linePlotData(leftAxisData, leftName, leftColor);
  linePlotData(rightAxisData, rightName, rightColor);
}

void LinePlot::showCurve(QwtPlotItem *item, bool on)
{
  /// update curve visibility
  item->setVisible(on);
  /// update legend item status
  QWidget *legendItem = m_legend->find(item);
  if ( (legendItem) && (legendItem->inherits("QwtLegendItem")) )
  {
    ((QwtLegendItem *)legendItem)->setChecked(on);
  }

  m_qwtPlot->replot();
  m_legend->updateGeometry();
}

void LinePlot::xCenterSpan(double center, double span)
{
  /// min = maximum(center - span, min(x data))
  /// max = minimum(center + span, max(x data))
  double centerX = centerXValue(center);
  double spanX = spanXValue(span);
  double minX = centerX - spanX;
  double maxX = centerX + spanX;
  if (minX < m_xAxisMin)  {
    minX = m_xAxisMin;
    maxX = minX + 2.0 * spanX;
  } else if (maxX > m_xAxisMax) {
    maxX = m_xAxisMax;
    minX = maxX - 2.0 * spanX;
  }
  m_qwtPlot->setAxisScale(QwtPlot::xBottom, minX, maxX);
  m_qwtPlot->replot();
}

double LinePlot::centerXValue(double center)
{
  /// do any translations here
  return center;
}

double LinePlot::spanXValue(double span)
{
  /// do any translations here
  return span;
}


} // openstudio
