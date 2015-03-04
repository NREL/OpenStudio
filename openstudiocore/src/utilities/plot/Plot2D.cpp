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

#include "Plot2D.hpp"

#include "../core/Application.hpp"

using namespace std;
using namespace boost;

namespace openstudio{

Plot2DTimeAxis::Plot2DTimeAxis(const DateTime &startDateTime, double duration)
  : m_startDateTime(startDateTime), m_duration(duration)
{
}

QwtText Plot2DTimeAxis::label(double fracDays) const
{
  Time timeFromFracDays(fracDays - m_startDateTime.date().dayOfYear() - m_startDateTime.time().totalDays());
  DateTime dateTime(m_startDateTime + timeFromFracDays);
  Date date = dateTime.date();
  Time time = dateTime.time();
  unsigned day = date.dayOfMonth();
  unsigned month = openstudio::month(date.monthOfYear());
  int hour = time.hours();
  int minutes = time.minutes();
  int seconds = time.seconds();

  QString s;

  // zooming 
  double currentDuration = scaleDiv().upperBound() - scaleDiv().lowerBound();

  if (currentDuration < 1.0 / 24.0) // less than an hour
  {
    s.sprintf("%02d/%02d %02d:%02d:%02d", month, day, hour, minutes, seconds);
  } else if (currentDuration < 7.0) // less than a week
  {
    s.sprintf("%02d/%02d %02d:%02d", month, day, hour, minutes);
  } else // week or more
  {
    s.sprintf("%02d/%02d", month, day);
  }

  return s;
}

const DateTime Plot2DTimeAxis::startDateTime() const 
{ 
  return m_startDateTime; 
}

void Plot2DTimeAxis::startDateTime(const DateTime &_startDateTime) 
{
  m_startDateTime = _startDateTime; 
}

double Plot2DTimeAxis::duration() const 
{ 
  return m_duration; 
}

void Plot2DTimeAxis::duration(double _duration) 
{ 
  m_duration = _duration; 
}


Plot2D::Plot2D(QWidget* parent, Qt::WindowFlags flags)
  : QWidget(parent)
{
  setupUi(this);

  // accept drops
  setAcceptDrops( true );

  // default font sizes - meeting 8/27/09
  this->titleFontSize(16);
  this->axesFontSize(14);
  this->tickFontSize(12);

  setWindowTitle("[*]");
  setAttribute(Qt::WA_DeleteOnClose);
}

Plot2D::~Plot2D()
{}

void Plot2D::plotTitle(const std::string& title)
{
  m_qwtPlot->setTitle(toQString(title));
}

string Plot2D::plotTitle() const
{
  return toString(m_qwtPlot->title().text());
}

void Plot2D::bottomAxisTitle(const std::string& bottomAxisTitle)
{
  m_qwtPlot->setAxisTitle(QwtPlot::xBottom, toQString(bottomAxisTitle));
}

string Plot2D::bottomAxisTitle() const
{
  return toString(m_qwtPlot->axisTitle(QwtPlot::xBottom).text());
}

void Plot2D::topAxisTitle(const std::string& topAxisTitle)
{
  m_qwtPlot->setAxisTitle(QwtPlot::xTop, toQString(topAxisTitle));
}

void Plot2D::leftAxisTitleFromUnits(const std::string& units)
{
  m_qwtPlot->setAxisTitle(QwtPlot::yLeft, axisTitleFromUnits(units));
}

void Plot2D::rightAxisTitleFromUnits(const std::string& units)
{
  m_qwtPlot->setAxisTitle(QwtPlot::yRight, axisTitleFromUnits(units));
}

QString Plot2D::axisTitleFromUnits(const std::string& units)
{
  QString axisTitle = toQString(units);

  if (axisTitle.toUpper() == "W")
  {
    axisTitle = "Power (W)";
  }
  else if (axisTitle.toUpper() == "J")
  {
    axisTitle = "Energy (J)";
  }
  else if (axisTitle.toUpper() == "C")
  {
    axisTitle = "Temperature (C)";
  }

  return axisTitle;
}

string Plot2D::topAxisTitle() const
{
  return toString(m_qwtPlot->axisTitle(QwtPlot::xTop).text());
}


void Plot2D::leftAxisTitle(const std::string& leftAxisTitle)
{
  m_qwtPlot->setAxisTitle(QwtPlot::yLeft, toQString(leftAxisTitle));
}

string Plot2D::leftAxisTitle() const
{
  return toString(m_qwtPlot->axisTitle(QwtPlot::yLeft).text());
}


void Plot2D::rightAxisTitle(const std::string& rightAxisTitle)
{
  m_qwtPlot->setAxisTitle(QwtPlot::yRight, toQString(rightAxisTitle));
}

string Plot2D::rightAxisTitle() const
{
  return toString(m_qwtPlot->axisTitle(QwtPlot::yRight).text());
}


void Plot2D::tickFontSize(unsigned tickFontSize)
{
  leftTickFontSize(tickFontSize);
  rightTickFontSize(tickFontSize);
  topTickFontSize(tickFontSize);
  bottomTickFontSize(tickFontSize);
}

void Plot2D::leftTickFontSize(unsigned tickFontSize)
{
  QFont font = m_qwtPlot->axisFont(QwtPlot::yLeft);
  font.setPointSize(tickFontSize);
  m_qwtPlot->setAxisFont(QwtPlot::yLeft, font);
}

void Plot2D::rightTickFontSize(unsigned tickFontSize)
{
  QFont font = m_qwtPlot->axisFont(QwtPlot::yRight);
  font.setPointSize(tickFontSize);
  m_qwtPlot->setAxisFont(QwtPlot::yRight, font);
}

void Plot2D::bottomTickFontSize(unsigned tickFontSize)
{
  QFont font = m_qwtPlot->axisFont(QwtPlot::xBottom);
  font.setPointSize(tickFontSize);
  m_qwtPlot->setAxisFont(QwtPlot::xBottom, font);
}

void Plot2D::topTickFontSize(unsigned tickFontSize)
{
  QFont font = m_qwtPlot->axisFont(QwtPlot::xTop);
  font.setPointSize(tickFontSize);
  m_qwtPlot->setAxisFont(QwtPlot::xTop, font);
}

void Plot2D::titleFontSize(const unsigned titleFontSize)
{
  QwtText title = m_qwtPlot->title();
  QFont font = title.font();
  font.setPointSize(titleFontSize);
  title.setFont(font);
  m_qwtPlot->setTitle(title);
}

unsigned Plot2D::titleFontSize() const
{
  QwtText title = m_qwtPlot->title();
  QFont font = title.font();
  return font.pointSize();
}

void Plot2D::axesFontSize(const unsigned axesFontSize)
{
  leftAxisFontSize(axesFontSize);
  rightAxisFontSize(axesFontSize);
  bottomAxisFontSize(axesFontSize);
  topAxisFontSize(axesFontSize);
}

void Plot2D::leftAxisFontSize(const unsigned axisFontSize)
{
  QwtText title = m_qwtPlot->axisTitle(QwtPlot::yLeft);
  QFont font = title.font();
  font.setPointSize(axisFontSize);
  title.setFont(font);
  m_qwtPlot->setAxisTitle(QwtPlot::yLeft, title);
}

unsigned Plot2D::leftAxisFontSize() const
{
  QwtText title = m_qwtPlot->axisTitle(QwtPlot::yLeft);
  QFont font = title.font();
  return font.pointSize();
}


void Plot2D::rightAxisFontSize(const unsigned axisFontSize)
{
  QwtText title = m_qwtPlot->axisTitle(QwtPlot::yRight);
  QFont font = title.font();
  font.setPointSize(axisFontSize);
  title.setFont(font);
  m_qwtPlot->setAxisTitle(QwtPlot::yRight, title);
}

unsigned Plot2D::rightAxisFontSize() const
{
  QwtText title = m_qwtPlot->axisTitle(QwtPlot::yRight);
  QFont font = title.font();
  return font.pointSize();
}

void Plot2D::bottomAxisFontSize(const unsigned axisFontSize)
{
  QwtText title = m_qwtPlot->axisTitle(QwtPlot::xBottom);
  QFont font = title.font();
  font.setPointSize(axisFontSize);
  title.setFont(font);
  m_qwtPlot->setAxisTitle(QwtPlot::xBottom, title);
}

unsigned Plot2D::bottomAxisFontSize() const
{
  QwtText title = m_qwtPlot->axisTitle(QwtPlot::xBottom);
  QFont font = title.font();
  return font.pointSize();
}


void Plot2D::topAxisFontSize(const unsigned axisFontSize)
{
  QwtText title = m_qwtPlot->axisTitle(QwtPlot::xTop);
  QFont font = title.font();
  font.setPointSize(axisFontSize);
  title.setFont(font);
  m_qwtPlot->setAxisTitle(QwtPlot::xTop, title);
}

unsigned Plot2D::topAxisFontSize() const
{
  QwtText title = m_qwtPlot->axisTitle(QwtPlot::xTop);
  QFont font = title.font();
  return font.pointSize();
}

void Plot2D::titleFont(const QFont &titleFont)
{
  QwtText title = m_qwtPlot->title();
  title.setFont(titleFont);
  m_qwtPlot->setTitle(title);
}

QFont Plot2D::titleFont() const
{
  QwtText title = m_qwtPlot->title();
  return title.font();
}

void Plot2D::leftAxisFont(const QFont &axisFont)
{
  m_qwtPlot->setAxisFont(QwtPlot::yLeft, axisFont);
}

QFont Plot2D::leftAxisFont() const
{
  return m_qwtPlot->axisFont(QwtPlot::yLeft);
}

void Plot2D::rightAxisFont(const QFont &axisFont)
{
  m_qwtPlot->setAxisFont(QwtPlot::yRight, axisFont);
}

QFont Plot2D::rightAxisFont() const
{
  return m_qwtPlot->axisFont(QwtPlot::yRight);
}

void Plot2D::bottomAxisFont(const QFont &axisFont)
{
  m_qwtPlot->setAxisFont(QwtPlot::xBottom, axisFont);
}

QFont Plot2D::bottomAxisFont() const
{
  return m_qwtPlot->axisFont(QwtPlot::xBottom);
}

void Plot2D::topAxisFont(const QFont &axisFont)
{
  m_qwtPlot->setAxisFont(QwtPlot::xTop, axisFont);
}

QFont Plot2D::topAxisFont() const
{
  return m_qwtPlot->axisFont(QwtPlot::xTop);
}

void Plot2D::backgroundColor(QColor color)
{
  m_qwtPlot->setCanvasBackground(color);
}

void Plot2D::setupUi(QWidget *widget)
{
  //Application::instance().application(true);

  QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  sizePolicy.setHorizontalStretch(20);
  sizePolicy.setVerticalStretch(20);
  sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
  widget->setSizePolicy(sizePolicy);

  m_qwtPlot = new QwtPlot(this);

  QFont font;
  font.setPointSize(8);

  QwtText xAxis;
  xAxis.setFont(font);
  m_qwtPlot->setAxisTitle(QwtPlot::xBottom, xAxis);

  QwtText yAxis;
  yAxis.setFont(font);
  m_qwtPlot->setAxisTitle(QwtPlot::yLeft, yAxis);

}


void Plot2D::closeEvent(QCloseEvent *evt)
{
  QString windowTitle = openstudio::toQString(m_windowTitle);
  emit(signalClose(windowTitle));
}


void Plot2D::generateImage(const openstudio::path& file, int w, int h)
{
  if ((w*h)==0)
  {
    QPixmap pixmap(this->size());
    pixmap.fill(Qt::white);
    QPainter p(&pixmap);

    QwtScaleMap maps[QwtPlot::axisCnt];
    for (int axisId = 0; axisId < QwtPlot::axisCnt; axisId++){
      maps[axisId] = m_qwtPlot->canvasMap(axisId);
    }

    m_qwtPlot->drawItems(&p, rect(), maps);
    p.end();
    pixmap.save(toQString(file), nullptr, -1);
  }
  else
  {
    QPixmap pixmap(w, h);
    pixmap.fill(Qt::white);
    QRect r(0,0,w,h);
    QPainter p(&pixmap);

    QwtScaleMap maps[QwtPlot::axisCnt];
    for (int axisId = 0; axisId < QwtPlot::axisCnt; axisId++){
      maps[axisId] = m_qwtPlot->canvasMap(axisId);
    }

    m_qwtPlot->drawItems(&p, r, maps);
    p.end();
    pixmap.save(toQString(file), nullptr, -1);
  }
}

void Plot2D::exportImageToFile(int w, int h)
{
  QString filename = QFileDialog::getSaveFileName(this,
    tr("Export Image"), // caption
    ".",        // folder
    tr(  "JPEG (*.jpeg)\n" // filters
    "PNG (*.png)\n"
    "Bitmap (*.bmp)\n"));
  if (!filename.isEmpty())
  {
    generateImage(toPath(filename), w, h);
  }
}

void Plot2D::exportImageScreenSize()
{
  exportImageToFile(0,0);
}

void Plot2D::exportImage800x600()
{
  exportImageToFile(800,600);
}

void Plot2D::exportImage400x300()
{
  exportImageToFile(400,300);
}

void Plot2D::windowTitle(const std::string& title)
{
  m_windowTitle = title;
  setWindowTitle(toQString(title));
}

std::string Plot2D::windowTitle() const
{
  return m_windowTitle;
}

} // openstudio
