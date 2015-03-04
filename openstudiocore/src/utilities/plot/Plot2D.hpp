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

#ifndef UTILITIES_PLOT_PLOT2D_HPP
#define UTILITIES_PLOT_PLOT2D_HPP

#include "../UtilitiesAPI.hpp"

#include "../core/Path.hpp"
#include "../core/Macro.hpp"
#include "../data/TimeSeries.hpp"

#include <QWidget>
#include <QAction>
#include <QFont>
#include <QColor>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QSpinBox>
#include <QPainter>
#include <QFileDialog>
#include <QBitmap>
#include <QDropEvent>
#include <QPixmap>
#include <qwt/qwt_plot.h>
#include <qwt/qwt_text.h>
#include <qwt/qwt_legend.h>
#include <qwt/qwt_plot_layout.h>
#include <qwt/qwt_plot_panner.h>
#include <qwt/qwt_scale_widget.h>
#include <qwt/qwt_scale_draw.h>
#include <qwt/qwt_plot_curve.h>

#include <QApplication>


namespace openstudio{


/**  Plot2DTimeAxis supports date/time on  axis
*    \deprecated { Qwt drawing widgets are deprecated in favor of Javascript }
*/
class UTILITIES_API Plot2DTimeAxis: public QwtScaleDraw
{
public:
  Plot2DTimeAxis(const DateTime &startDateTime, double duration);

  virtual QwtText label(double fracDays) const;

  const DateTime startDateTime() const;
  void startDateTime(const DateTime &_startDateTime);
  double duration() const;
  void duration(double _duration);

private:
    DateTime m_startDateTime;
    double m_duration;
};


/**  Plot2D is the wrapper around QwtPlot
*    \deprecated { Qwt drawing widgets are deprecated in favor of Javascript }
*/
class UTILITIES_API Plot2D : public QWidget {

  Q_OBJECT

public:

  /// destructor
  virtual ~Plot2D();

  /// background color
  virtual void backgroundColor(QColor color);

  /// graph title
  void plotTitle(const std::string& title);

  /// graph title
  std::string plotTitle() const;

  /// window title
  void windowTitle(const std::string& title);

  /// window title -
  std::string windowTitle() const;

  /// bottom axis title
  void bottomAxisTitle(const std::string& bottomAxisTitle);

  /// bottom axis title
  std::string bottomAxisTitle() const;

  /// top axis title
  void topAxisTitle(const std::string& topAxisTitle);

  /// top axis title
  std::string topAxisTitle() const;

  /// left axis title
  void leftAxisTitle(const std::string& leftAxisTitle);

  /// left axis title
  std::string leftAxisTitle() const;

  /// right axis title
  void rightAxisTitle(const std::string& rightAxisTitle);

  /// right axis title
  std::string rightAxisTitle() const;

  /// tick font size for all axes
  void tickFontSize(unsigned tickFontSize);

  /// tick font size for left axis
  void leftTickFontSize(unsigned tickFontSize);

  /// tick font size for right axis
  void rightTickFontSize(unsigned tickFontSize);

  /// tick font size for bottom axis
  void bottomTickFontSize(unsigned tickFontSize);

  /// tick font size for top axis
  void topTickFontSize(unsigned tickFontSize);

  /// axis title font size for all axes
  void axesFontSize(unsigned axesFontSize);

  /// axis title font size for left axis
  void leftAxisFontSize(const unsigned axisFontSize);

  /// axis title font size for left axis
  unsigned leftAxisFontSize() const;

  /// axis title font size for right axis
  void rightAxisFontSize(const unsigned axisFontSize);

  /// axis title font size for right axis
  unsigned rightAxisFontSize() const;

  /// axis title font size for top axis
  void topAxisFontSize(const unsigned axisFontSize);

  /// axis title font size for top axis
  unsigned topAxisFontSize() const;

  /// axis title font size for bottom axis
  void bottomAxisFontSize(const unsigned axisFontSize);

  /// axis title font size for bottom axis
  unsigned bottomAxisFontSize() const;

  /// title font size
  void titleFontSize(const unsigned titleFontSize);

  /// title font size
  unsigned titleFontSize() const;

  /// title font
  void titleFont(const QFont& titleFont);

  /// title font size
  QFont titleFont() const;

  /// axis title font size for left axis
  void leftAxisFont(const QFont& axisFont);

  /// axis title font size for left axis
  QFont leftAxisFont() const;

  /// axis title font size for right axis
  void rightAxisFont(const QFont& axisFont);

  /// axis title font size for right axis
  QFont rightAxisFont() const;

  /// axis title font size for top axis
  void topAxisFont(const QFont& axisFont);

  /// axis title font size for top axis
  QFont topAxisFont() const;

  /// axis title font size for bottom axis
  void bottomAxisFont(const QFont& axisFont);

  /// axis title font size for bottom axis
  QFont bottomAxisFont() const;

  /// axis titles based on units
  void leftAxisTitleFromUnits(const std::string& units);

  /// axis titles based on units
  void rightAxisTitleFromUnits(const std::string& units);

  /// axis titles based on units
  QString axisTitleFromUnits(const std::string& units);

  /// image generation
  void generateImage(const openstudio::path& file, int w=800, int h=600);

  /// image exporting
  void exportImageToFile(int w, int h);

  /// setup widget items and layout
  void setupUi(QWidget *widget);

  /// update based on center and span
  virtual void xCenterSpan(double center, double span)=0;

protected:

  /// constructor
  Plot2D(QWidget* parent = nullptr, Qt::WindowFlags flags = nullptr);

  // parented by this
  QwtPlot *m_qwtPlot;

private:
  std::string m_windowTitle;

  // send signal closed if external management - e.g. resultsViewer
  void closeEvent(QCloseEvent *evt);

signals: 
  void signalClose(QString& windowTitle);

private slots:
  void exportImageScreenSize();
  void exportImage800x600();
  void exportImage400x300();
};

} // openstudio

#endif // UTILITIES_PLOT_PLOT2D_HPP
