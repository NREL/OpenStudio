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

#ifndef RESULTSVIEWER_PLOTVIEW_HPP
#define RESULTSVIEWER_PLOTVIEW_HPP

#include <QWidget>
#include <QAction>
#include <QFont>
#include <QColor>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QSlider>
#include <QSpinBox>
#include <QMenuBar>
#include <QToolBar>
#include <QPainter>
#include <QFileDialog>
#include <QBitmap>
#include <QDropEvent>
#include <QPixmap>
#include <QToolButton>
#include <QMimeData>

#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_grid.h>
#include <qwt/qwt_text.h>
#include <qwt/qwt_legend.h>
#include <qwt/qwt_array.h>
#include <qwt/qwt_plot_layout.h>
#include <qwt/qwt_plot_panner.h>
#include <qwt/qwt_plot_picker.h>
#include <qwt/qwt_plot_marker.h>
#include <qwt/qwt_plot_zoomer.h>
#include <qwt/qwt_scale_widget.h>
#include <qwt/qwt_scale_draw.h>
#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_color_map.h>
#include <qwt/qwt_plot_spectrogram.h>

#include "../utilities/data/TimeSeries.hpp"
#include "../utilities/time/Date.hpp"
#include "../utilities/plot/FloodPlot.hpp"
#include "../utilities/plot/LinePlot.hpp"

namespace resultsviewer{

  // int or enum
  const int RVPV_LINEPLOT = 0;
  const int RVPV_FLOODPLOT = 1;
  const int RVPV_ILLUMINANCEPLOT = 2;
  enum YValueType { unScaledY, scaledY };
  enum LinePlotStyleType { smoothLinePlot, stairLinePlot, barLinePlot };

  class PlotView;

  /** LinePlotCurve is a line plot curve item to track alias and source
  and hold scaled and unscaled y-values
  and hold smooth and stair x-values
  */
  class LinePlotCurve : public QwtPlotCurve
  {
  public:
    LinePlotCurve(QString& title, openstudio::TimeSeriesLinePlotData& data);
    //  LinePlotCurve(QString title):QwtPlotCurve(title){};

    void setLegend(QString legend) {m_legend=legend;}
    QString legend() {return m_legend;}
    void setAlias(QStringList& alias) {m_alias=alias;}
    QStringList& alias() {return m_alias;}
    void setPlotSource(QStringList& plotSource) {m_plotSource=plotSource;}
    QStringList& plotSource() {return m_plotSource;}

    double yUnscaled(int i) {return m_yUnscaled[i];}
    void setYUnscaled(QwtArray<double>& yUnscaled) {m_yUnscaled = yUnscaled;}
    double yScaled(int i) {return m_yScaled[i];}
    void setYScaled(QwtArray<double>& yScaled) {m_yScaled = yScaled;}
    double xValues(int i) {return m_xValues[i];}
    void setXValues(QwtArray<double>& xValues) {m_xValues = xValues;}

    // assign data and update array members
    void setDataMode(YValueType yType);
    void setLinePlotData(const openstudio::LinePlotData& data);

    YValueType yType() {return m_yType;}
    LinePlotStyleType linePlotStyle() {return m_linePlotStyle;}

    void setLinePlotStyle(LinePlotStyleType lineStyle);

  private:
    QStringList m_alias;
    QStringList m_plotSource;
    QString m_legend;
    QwtArray<double> m_yScaled;
    QwtArray<double> m_yUnscaled;
    QwtArray<double> m_xValues; // mid point
    YValueType m_yType;
    LinePlotStyleType m_linePlotStyle;

  };

  /** PlotLegend is a ui widget to display line plot legends
  */
  class PlotLegend : public QWidget
  {
    Q_OBJECT

  public:

    PlotLegend( PlotView *parent);
    void refreshLegend();

  private:
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void performDrag(QPoint pos);

    void paintEvent(QPaintEvent *evt);
    void addLegendItem(resultsviewer::LinePlotCurve *curve);
    void updateLegend();
    void clearLegend(QLayout *li);

    QPoint m_startDragPos;
    PlotView *m_plotView;
    QVBoxLayout *m_mainLayout;
    int m_x;
    int m_y;
    int m_width;
  };


  /// PlotViewData is a convenience structure for holding plot data
  struct PlotViewData {
    QStringList plotSource; // most likely an eplusout.sql file
    QStringList alias; // shorthand notation for plot source - used to distinguish legend values
    QString legendName; // string that appears in legend - usually variablename - keyname
    QString dbIdentifier; // additional data to retrieve data (e.g. mapname for illuminance maps)
    QString interval; // used for setting x and y axis titles
    QString plotTitle;
    QString windowTitle;
    QString xAxisTitle;
    QString yAxisTitle;
    openstudio::OptionalTimeSeries ts;
  };

  /**  PlotViewMimeData supports dropping plotViewData of drag/drop operations
  */
  class PlotViewMimeData : public QMimeData {
    Q_OBJECT

  public:
    PlotViewMimeData(std::vector<PlotViewData> &_plotViewDataVec);

    std::vector<PlotViewData> plotViewDataVec() const {return m_plotViewDataVec;}
  private:
    std::vector<PlotViewData> m_plotViewDataVec;
  };

  /**  PlotViewTimeAxis is a plot axis that supports date/time values
  */
  class PlotViewTimeAxis: public QwtScaleDraw
  {
  public:
    PlotViewTimeAxis(const int &plotType)
    {
      m_plotType = plotType;
    }

    virtual QwtText label(double fracDays) const
    {
      openstudio::Time timeFromFracDays(fracDays);
      //    openstudio::Date date = openstudio::Date::fromDayOfYear(timeFromFracDays.days()); Date issue with day 366 in year 2009
      //  Fix this - update year outside of Date class
      int dayOfYear = timeFromFracDays.days();
      while (dayOfYear < 1) dayOfYear +=365;
      while (dayOfYear > 365) dayOfYear -=365;
      openstudio::Date date = openstudio::Date::fromDayOfYear(dayOfYear);

      unsigned day = date.dayOfMonth();
      unsigned month = openstudio::month(date.monthOfYear());
      int hour = timeFromFracDays.hours();
      int minutes = timeFromFracDays.minutes();
      int seconds = timeFromFracDays.seconds();

      QString s;

      // zooming
      double currentDuration = scaleDiv().upperBound() - scaleDiv().lowerBound();

      if (m_plotType == RVPV_FLOODPLOT)
      {
        s.sprintf("%02d/%02d", month, day);
      }
      else if (currentDuration < 1.0/24.0) // less than an hour
      {
        s.sprintf("%02d/%02d %02d:%02d:%02d", month, day, hour, minutes, seconds);
      }
      else if (currentDuration < 7.0) // less than a week
      {
        s.sprintf("%02d/%02d %02d:%02d", month, day, hour, minutes);
      }
      else // week or more
      {
        s.sprintf("%02d/%02d", month, day);
      }

      return s;
    }

    const openstudio::DateTime startDateTime() const {return m_startDateTime;}
    void startDateTime(const openstudio::DateTime &_startDate) {m_startDateTime = _startDate;}
    const int plotType() const {return m_plotType;}
    void plotType(const int _plotType) {m_plotType = _plotType;}

  private:
    openstudio::DateTime m_startDateTime;
    int m_plotType;
  };

  /** Zoomer is the control used for rectangular zooming in the plot region
  */
  class Zoomer: public QwtPlotZoomer
  {
  public:
    Zoomer(int xAxis, int yAxis, QwtPlotCanvas *canvas):   QwtPlotZoomer(xAxis, yAxis, canvas)
    {
      setSelectionFlags(QwtPicker::DragSelection | QwtPicker::CornerToCorner);
      setTrackerMode(QwtPicker::AlwaysOff);
      setRubberBand(QwtPicker::NoRubberBand);

      // RightButton: zoom out by 1
      // Ctrl+RightButton: zoom out to full size

      setMousePattern(QwtEventPattern::MouseSelect2, Qt::RightButton, Qt::ControlModifier);

      //    setMousePattern(QwtEventPattern::MouseSelect3, Qt::RightButton);
    }
  };


  /** PlotViewToolbar is the collection of tools for the user to control each plot
  */
  class PlotViewToolbar: public QToolBar
  {
    Q_OBJECT
  public:
    PlotViewToolbar(const QString &title, QWidget *parent = nullptr);

  protected:
    void mouseDoubleClickEvent(QMouseEvent *evt);

signals:
    void signalDoubleClick();
  };


  /** PlotView is a ui container for the plot and all controls and toolbars
  */

  class PlotView : public QWidget
  {
    Q_OBJECT

  public:

    PlotView(int plotType=RVPV_LINEPLOT, QWidget* parent=nullptr);
    PlotView(QString& path, int plotType=RVPV_LINEPLOT, QWidget* parent=nullptr);


    // plot view data handler
    void plotViewData(PlotViewData &_plotViewData, const std::function<bool ()> &t_workCanceled);

    // plot view difference data handler
    void plotViewDataDifference(PlotViewData &_plotViewData1, PlotViewData &_plotViewData2);

    // access to qwtPlot widget
    QwtPlot *plot() {return m_plot;}

    // access to plot type
    int plotType() {return m_plotType;}

    // spectorgram and countour
    bool spectrogramOn() {return m_spectrogramOn;}
    bool contourOn() {return m_contourOn;}
    void showContour(bool on);
    void showSpectrogram(bool on);
    QStringList colorMapList();
    int colorMap() {return (int)m_colorMapType;}
    void setColorMap(openstudio::FloodPlotColorMap::ColorMapList clrMap);
    void setColorMap(QString& clrMap);
    // number of qwtPlotCurves on plot
    int numberOfCurves();

    resultsviewer::PlotLegend *legend() {return m_legend;}

    // alias updating
    void updateAlias(QString& alias, QString& plotSource);

    // access to last image saved path
    QString& lastImageSavedPath() {return m_lastImageSavedPath;}
    void setLastImageSavedPath(QString &path) {m_lastImageSavedPath = path;}

    // access to get and set autoscaling option
    bool yLeftAutoScale() {return m_yLeftAutoScale;}
    void setYLeftAutoScale(bool autoScale) {m_yLeftAutoScale = autoScale;}
    bool yRightAutoScale() {return m_yRightAutoScale;}
    void setYRightAutoScale(bool autoScale) {m_yRightAutoScale = autoScale;}

    // autoscale and min max for flood plots and illuminance maps
    void setDataRange();
    bool floodPlotAutoScale() {return m_floodPlotAutoScale;}
    void setFloodPlotAutoScale(bool autoscale) {m_floodPlotAutoScale=autoscale;}
    double floodPlotMin() {return m_floodPlotMin;}
    void setFloodPlotMin(double minValue) {m_floodPlotMin=minValue;}
    double floodPlotMax() {return m_floodPlotMax;}
    void setFloodPlotMax(double maxValue) {m_floodPlotMax=maxValue;}
    // yearly values - used for illuminance maps
    double floodPlotYearlyMin() {return m_floodPlotYearlyMin;}
    void setFloodPlotYearlyMin(double minValue) {m_floodPlotYearlyMin=minValue;}
    double floodPlotYearlyMax() {return m_floodPlotYearlyMax;}
    void setFloodPlotYearlyMax(double maxValue) {m_floodPlotYearlyMax=maxValue;}
    // properties access
    QwtPlotMarker* illuminanceMapRefPt1() {return m_illuminanceMapRefPt1;}
    QwtPlotMarker* illuminanceMapRefPt2() {return m_illuminanceMapRefPt2;}


  private:
    bool m_floodPlotAutoScale;
    double m_floodPlotMin;
    double m_floodPlotMax;
    double m_floodPlotYearlyMin;
    double m_floodPlotYearlyMax;

    // ref pt markers - used for illuminance map reference points - can generalize
    QwtPlotMarker *m_illuminanceMapRefPt1;
    QwtPlotMarker *m_illuminanceMapRefPt2;

    // initialization of widget - called from constructors
    void init();

    // alias updating
    QString updateLabelString( QString& label, QStringList& alias, QStringList& plotSource);
    QString m_legendName;
    QString m_dbIdentifier;
    QStringList m_alias;
    QStringList m_plotSource;

    // last image saved path
    QString m_lastImageSavedPath;

    // send float or dock signal
    // void mouseDoubleClickEvent(QMouseEvent *evt);
    void closeEvent(QCloseEvent *evt);
    QwtPlot *m_plot;
    // menu and toolbar
    QMenuBar *m_menuBar;
    PlotViewToolbar *m_toolBar;
    // span and center
    QSlider *m_centerSlider;
    QLineEdit *m_centerDate;
    QSlider *m_spanSlider;
    QDoubleSpinBox *m_centerSpinBox;
    QDoubleSpinBox *m_spanSpinBox;
    // line or flood now
    int m_plotType;
    // rubberband zoomer for both left and right axes
    QwtPlotZoomer *m_zoomer[2];
    // pick items on plot - value info
    QwtPlotPicker *m_picker;
    QTextEdit *m_valueInfo;
    QwtPlotMarker *m_valueInfoMarker;
    // pan plot when zoomed
    QwtPlotPanner *m_panner;

    // line plot specific
    void linePlotItem(PlotViewData &_plotViewData, const std::function<bool ()> &t_workCanceled);
    // flood plot specific
    void floodPlotItem(PlotViewData &_plotViewData);
    // illuminance plot specific
    void illuminancePlotItem(PlotViewData &_plotViewData);
    // illuminance plot difference 1-2
    void illuminancePlotItemsDifference(PlotViewData &_plotViewData1, PlotViewData &_plotViewData2);


    void createToolBar();
    void createMenuBar();
    void createLayout();
    void createQwtPlot();

    // cursors for toolbar icons
    QCursor m_selectCursor;
    QCursor m_panCursor;
    QCursor m_valueInfoCursor;
    QCursor m_zoomCursor;
    void setPlotCursor(const QCursor& _cursor);

    // scale curves when multiple with different units
    void scaleCurves(LinePlotCurve *curve, const std::function<bool ()> &t_workCanceled);

    // set curve color
    QPen curvePen(QColor &color);
    QColor curveColor(QColor &lastColor);

    // used with center and span and to set base zoom rect
    double m_xAxisMin;
    double m_xAxisMax;
    double m_yAxisMin;
    double m_yAxisMax;
    // line thickness for multiple curves
    int m_lineThickness;

    // time axis for multiple curves
    openstudio::DateTime m_startDateTime;
    openstudio::DateTime m_endDateTime;
    double m_duration;
    PlotViewTimeAxis *m_plotViewTimeAxis;

    // plot grid - line plots
    QwtPlotGrid *m_grid;

    // unit tracking for multiple curves
    QString m_leftAxisUnits;
    QString m_rightAxisUnits;

    // collection of automated colors
    std::vector<QColor> m_colorVec;
    QColor m_lastColor;

    // legend - may be replaced with custom widget
    // QwtLegend *m_legend;
    resultsviewer::PlotLegend *m_legend;


    void showCurve(QwtPlotItem *item, bool on);

    QwtLinearColorMap m_colorMap;
    QwtPlotSpectrogram* m_spectrogram;
    QwtScaleWidget* m_rightAxis;
    openstudio::FloodPlotData::Ptr m_floodPlotData;
    openstudio::FloodPlotColorMap::ColorMapList m_colorMapType;
    QwtDoubleInterval m_dataRange;
    openstudio::Vector m_colorLevels;
    void initColorMap();
    void initColorBar();
    int m_colorMapLength;

    // Flood Plot
    void contourLevels(openstudio::Vector& contourValues);
    void colorMapRange(double min, double max);
    void colorLevels(openstudio::Vector& colorLevels);
    // spectorgram and countour
    bool m_spectrogramOn;
    bool m_contourOn;


    void xCenterSpan(double center, double span);

    // set base rect for zoomer - reinitialize zoom stack if reset true
    void updateZoomBase(const QwtDoubleRect& base, bool reset);

    // image export functions
    void generateImage(QString& file, int w, int h);
    void exportImageToFile(int w, int h);

    // axes titles from units
    void leftAxisTitleFromUnits(const QString& units);
    void rightAxisTitleFromUnits(const QString& units);
    QString axisTitleFromUnits(const QString& units);

    // value info
    void valueInfoLinePlot(const QPoint& pos);
    void valueInfoFloodPlot(const QPoint& pos);
    void valueInfoIlluminanceMap(const QPoint& pos);
    void valueInfoIlluminanceMap(const double& x, const double& y);

    // auto rescaling for x values trac 346
    void AutoScaleY(double minX, double maxX);
    bool m_yLeftAutoScale;
    bool m_yRightAutoScale;

    // Legend overlay printing - for saving and printing images
    void printLegend(QPainter *p, const QRect &rect);

    // illuminance map hourly report indices
    std::vector< std::pair<int, openstudio::DateTime> > m_illuminanceMapReportIndicesDates;
    std::vector<openstudio::FloodPlotData::Ptr> m_illuminanceMapData;
    // difference index
    std::vector< std::pair<int,int> > m_illuminanceMapDifferenceReportIndices;
    void plotDataAvailable(bool available);

    // hour day arrow buttons
    QSpinBox *m_spinDay;
    QSpinBox *m_spinHour;
    int m_spinDayPreviousValue;
    int m_spinHourPreviousValue;
    void setCenterDayHour(int day, int hour);

    // no data available widget
    QLabel *m_noData;

  protected:
    /// drop target support for drag/drop operations
    void dropEvent(QDropEvent *e);
    /// drop target support for drag/drop operations
    void dragEnterEvent(QDragEnterEvent *e) { e->accept(); };
    /// trac 1383 right mouse click for properties
    void mousePressEvent(QMouseEvent *e);

  signals:
    void signalFloatOrDockMe(resultsviewer::PlotView *plotView);

    // closing in resultsviewer - remove from collection
    void signalClose(QString& windowTitle);

    // update last image saved path
    void signalLastImageSavedPath(QString& path);

    private slots:
      // rubber band zoom mode
      void slotZoomMode(bool on);
      // pan mode
      void slotPanMode(bool on);
      // value info mode
      void slotValueInfoMode(bool on);
      void slotValueInfo(const QPoint& pos);
      // signal if zoomed - hide value info if rect changes
      void slotZoomed(const QwtDoubleRect& rect);

      // zoom in and out in increments
      void slotZoomIn();
      void slotZoomOut();
      // pointer (selection) mode
      void slotPointerMode(bool on);
      // print
      void slotPrint();
      // properties
      void slotProperties();

      // center and span with fractional days
      // spin box holds fractional day (2 decimals) and sliders hold integer values = fractional days * 100
      void slotCenterValue(double center);
      void slotSpanValue(double span);
      void slotCenterSliderToDouble(int center);
      void slotSpanSliderToDouble(int span);
      void slotCenterSpinBoxToInt(double center);
      void slotSpanSpinBoxToInt(double span);


      // illuminance
      void slotCenterIlluminance(int reportIndex);
      void slotIlluminanceDateChanged();
      void slotSpinDayChanged(int i);
      void slotSpinHourChanged(int i);

      // save image functions
      void slotSaveImageScreenSize();
      void slotSaveImage800x600();
      void slotSaveImage400x300();

      // docking
      void slotFloatOrDock();


  };


}; // resultsviewer namespace

#endif // RESULTSVIEWER_PLOTVIEW_HPP
