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

#include "Plot2D.hpp"

using namespace std;
using namespace boost;

namespace openstudio{


Plot2D::Plot2D(QWidget* parent, Qt::WindowFlags flags):QWidget(parent)
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

void Plot2D::backgourndColor(QColor color)
{
  m_qwtPlot->setCanvasBackground(color);
}

void Plot2D::setupUi(QWidget *widget)
{
  QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  sizePolicy.setHorizontalStretch(20);
  sizePolicy.setVerticalStretch(20);
  sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
  widget->setSizePolicy(sizePolicy);

  m_vBoxLayout = new QVBoxLayout(widget);

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
    this->m_qwtPlot->print(&p, rect());
    p.end();
    pixmap.save(toQString(file), nullptr, -1);
  }
  else
  {
    QPixmap pixmap(w, h);
    pixmap.fill(Qt::white);
    QRect r(0,0,w,h);
    QPainter p(&pixmap);
    this->m_qwtPlot->print(&p, r);
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


} // openstudio
