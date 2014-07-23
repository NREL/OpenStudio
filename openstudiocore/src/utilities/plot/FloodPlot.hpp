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

#ifndef UTILITIES_PLOT_FLOODPLOT_HPP
#define UTILITIES_PLOT_FLOODPLOT_HPP

#include "../UtilitiesAPI.hpp"

#include "Plot2D.hpp"
#include "../data/TimeSeries.hpp"
#include "../data/Vector.hpp"
#include "../data/Matrix.hpp"

#include <QWidget>
#include <QPushButton>
#include <QAction>

#include <qwt/qwt_plot.h>
#include <qwt/qwt_text.h>
#include <qwt/qwt_color_map.h>
#include <qwt/qwt_plot_spectrogram.h>
#include <qwt/qwt_scale_draw.h>
#include <qwt/qwt_plot_zoomer.h>
#include <qwt/qwt_plot_layout.h>

namespace openstudio{

  /** FloodPlotColorMap is class for colormap that can be used in a flood plot.
  */
  class UTILITIES_API FloodPlotColorMap: public QwtLinearColorMap
  {
    public:

      COMMON_PTR_TYPEDEFS(FloodPlotColorMap);

      /// available color maps
      enum ColorMapList {Gray, Jet};

      /// constructor
      FloodPlotColorMap(Vector& colorLevels, ColorMapList colorMap);

      /// virtual destructor
      virtual ~FloodPlotColorMap() {}

    private:
      Vector& m_colorLevels;
      ColorMapList m_colorMap;
      void init();
  };


  /** FloodPlotData is abstract class for data that can be used in a flood plot.
   *  Derive from this class to plot your data.
   */
  class UTILITIES_API FloodPlotData: public QwtRasterData
  {
    public:

      COMMON_PTR_TYPEDEFS(FloodPlotData);

      /// virtual destructor
      virtual ~FloodPlotData() {}

      /// must provide copy
      virtual FloodPlotData* copy() const = 0;

      /// must provide range of values - colormap range not data range
      QwtDoubleInterval range() const = 0;

      // color map range
      virtual void colorMapRange(QwtDoubleInterval colorMapRange) = 0;

      /// color map range
      virtual QwtDoubleInterval colorMapRange() = 0;


      /// get the value at point x, y
      virtual double value(double x, double y) const = 0;

      /// minX
      virtual double minX() const = 0;

      /// maxX
      virtual double maxX() const = 0;

      /// minY
      virtual double minY() const = 0;

      /// maxY
      virtual double maxY() const = 0;

      /// minValue getter
      virtual double minValue() const = 0;

      /// maxValue getter
      virtual double maxValue() const = 0;

      /// minValue setter
      virtual void minValue(double min) = 0;

      /// maxValue setter
      virtual void maxValue(double max) = 0;

      /// sumValue
      virtual double sumValue() const = 0;

      /// meanValue
      virtual double meanValue() const = 0;

      /// stdDevValue
      virtual double stdDevValue() const = 0;

      /// units for plotting on axes or scaling
      virtual std::string units() const = 0;

    protected:
      FloodPlotData() {}
  };

  /** TimeSeriesFloodPlotData converts a time series into flood plot data
  */
  class UTILITIES_API TimeSeriesFloodPlotData: public FloodPlotData
  {
    public:

      COMMON_PTR_TYPEDEFS(TimeSeriesFloodPlotData);

      /// constructor
      TimeSeriesFloodPlotData(TimeSeries timeSeries);

      /// constructor with colormap range
      TimeSeriesFloodPlotData(TimeSeries timeSeries, QwtDoubleInterval colorMapRange );

      /// create
      static Ptr create(TimeSeries timeSeries)
      {return Ptr(new TimeSeriesFloodPlotData(timeSeries));}

      static Ptr create(TimeSeries timeSeries, QwtDoubleInterval colorMapRange )
      {return Ptr(new TimeSeriesFloodPlotData(timeSeries, colorMapRange));}

      /// virtual destructor
      virtual ~TimeSeriesFloodPlotData() {}

      /// must provide copy
      virtual TimeSeriesFloodPlotData* copy() const;

      /// must provide range of values - colormap range not data range
      QwtDoubleInterval range() const {return m_colorMapRange;}

      /// provide boundingRect overload for speed - default implementation slow!!!
      QwtDoubleRect boundingRect() const;

      ///  value at point fractionalDay and hourOfDay
      double value(double fractionalDay, double hourOfDay) const;

      /// minX
      double minX() const {return m_minX;};

      /// maxX
      double maxX() const {return m_maxX;};

      /// minY
      double minY() const {return m_minY;};

      /// maxY
      double maxY() const {return m_maxY;};

      /// minValue
      double minValue() const {return m_minValue;};

      /// maxValue
      double maxValue() const {return m_maxValue;};

      /// minValue setter
      void minValue(double min) {m_minValue = min;};

      /// maxValue setter
      void maxValue(double max) {m_maxValue = max;};

      /// sumValue
      double sumValue() const;

      /// meanValue
      double meanValue() const;

      /// stdDevValue
      double stdDevValue() const;

      /// range of values for which to show the colormap
      void colorMapRange(QwtDoubleInterval colorMapRange) {m_colorMapRange = colorMapRange;}

      /// range of values for which to show the colormap
      QwtDoubleInterval colorMapRange() {return m_colorMapRange;}

      /// units for plotting on axes or scaling
      std::string units() const {return m_units;};

    private:
      TimeSeries m_timeSeries;
      double m_minValue;
      double m_maxValue;
      double m_minX;
      double m_maxX;
      double m_minY;
      double m_maxY;
      double m_startFractionalDay;
      QwtDoubleInterval m_colorMapRange;
      std::string m_units;
  };

  /** MatrixFloodPlotData converts a Matrix into flood plot data
  */
  class UTILITIES_API MatrixFloodPlotData: public FloodPlotData
  {
    public:

      COMMON_PTR_TYPEDEFS(MatrixFloodPlotData);

      /// constructor
      MatrixFloodPlotData(const Matrix& matrix);

      /// constructor and color map range
      MatrixFloodPlotData(const Matrix& matrix, QwtDoubleInterval colorMapRange );

      /// constructor with x and y vectors
      MatrixFloodPlotData(const Vector& xVector,
          const Vector& yVector,
          const Matrix& matrix);

      /// constructor with x and y vectors and interpolation method
      MatrixFloodPlotData(const Vector& xVector,
          const Vector& yVector,
          const Matrix& matrix,
          const InterpMethod interp);

      /// constructor with x and y vectors
      MatrixFloodPlotData(const std::vector<double>& xVector,
          const std::vector<double>& yVector,
          const std::vector<double>& matrix);

      /// constructor with x and y vectors and interpolation method
      MatrixFloodPlotData(const std::vector<double>& xVector,
          const std::vector<double>& yVector,
          const std::vector<double>& matrix,
          const InterpMethod interp);


      /// constructor with x and y vectors and color map range
      MatrixFloodPlotData(const Vector& xVector,
          const Vector& yVector,
          const Matrix& matrix,
          QwtDoubleInterval colorMapRange );

      /// create
      static Ptr create(const Matrix& matrix)
      {return Ptr(new MatrixFloodPlotData(matrix));}

      static Ptr create(const Matrix& matrix, QwtDoubleInterval& colorMapRange)
      {return Ptr(new MatrixFloodPlotData(matrix, colorMapRange));}

      /// create with x and y vectors
      static Ptr create(const Vector& xVector,
          const Vector& yVector,
          const Matrix& matrix)
      {return Ptr(new MatrixFloodPlotData(xVector, yVector, matrix));}

      static Ptr create(const Vector& xVector,
          const Vector& yVector,
          const Matrix& matrix,
          const InterpMethod interp)
      {return Ptr(new MatrixFloodPlotData(xVector, yVector, matrix,interp));}

      static Ptr create(const Vector& xVector,
          const Vector& yVector,
          const Matrix& matrix,
          QwtDoubleInterval& colorMapRange)
      {return Ptr(new MatrixFloodPlotData(xVector, yVector, matrix, colorMapRange));}

      /// create with std::vectors
      static Ptr create(const std::vector<double>& xVector,
          const std::vector<double>& yVector,
          const std::vector<double>& matrix)
      {return Ptr(new MatrixFloodPlotData(xVector, yVector, matrix));}

      static Ptr create(const std::vector<double>& xVector,
          const std::vector<double>& yVector,
          const std::vector<double>& matrix,
          const InterpMethod interp)
      {return Ptr(new MatrixFloodPlotData(xVector, yVector, matrix,interp));}

      /// virtual destructor
      virtual ~MatrixFloodPlotData() {}

      /// must provide copy
      virtual MatrixFloodPlotData* copy() const;

      /// must provide range of values - colormap range not data range
      QwtDoubleInterval range() const {return m_colorMapRange;}

      /// get the value at point x, y
      double value(double x, double y) const;

      /// set the interp method, defaults to Nearest
      void interpMethod(InterpMethod interpMethod);

      /// minX
      double minX() const;

      /// maxX
      double maxX() const;

      /// minY
      double minY() const;

      /// maxY
      double maxY() const;

      /// minValue
      double minValue() const {return m_minValue;};

      /// maxValue
      double maxValue() const {return m_maxValue;};

      /// minValue setter
      void minValue(double min) {m_minValue = min;};

      /// maxValue setter
      void maxValue(double max) {m_maxValue = max;};

      /// sumValue
      double sumValue() const;

      /// meanValue
      double meanValue() const;

      /// stdDevValue
      double stdDevValue() const;

      /// range of values for which to show the colormap
      void colorMapRange(QwtDoubleInterval colorMapRange) {m_colorMapRange = colorMapRange;}

      /// range of values for which to show the colormap
      QwtDoubleInterval colorMapRange() {return m_colorMapRange;}

      /// units for plotting on axes or scaling
      std::string units() const {return m_units;};

    private:

      // set ranges and bounding box
      void init();
      double m_minValue;
      double m_maxValue;
      Vector m_xVector;
      Vector m_yVector;
      Matrix m_matrix;
      InterpMethod m_interpMethod;
      double m_minX, m_maxX, m_minY, m_maxY;
      QwtDoubleInterval m_colorMapRange;
      Vector m_colorMapScaleValues;
      std::string m_units;
  };


  /** FloodPlot plots data in a nice image
  */
  class UTILITIES_API FloodPlot : public Plot2D
  {
    Q_OBJECT

    public:

      COMMON_PTR_TYPEDEFS(FloodPlot);

      /// default constructor
      FloodPlot(QWidget* parent = nullptr, Qt::WindowFlags flags = nullptr);

      /// matrix constructor
      FloodPlot(const Matrix& m);

      /// create
      static FloodPlot::Ptr create(QWidget* parent = nullptr, Qt::WindowFlags flags = nullptr);

      static FloodPlot::Ptr create(const Matrix& m);

      /// virtual destructor
      virtual ~FloodPlot() {}

      QAction *colorMapSelection;

      QAction *contourPlot;

      /// show the contour map
      void showContour(bool on);

      /// show the spectrogram
      void showSpectrogram(bool on);

      /// data autoscaling
      void dataAutoRange();

      /// set the color map
      void colorMap(FloodPlotColorMap::ColorMapList clrMap);

      /// set the data
      void floodPlotData(FloodPlotData::Ptr data);

      /// convenience wrapper for timeseries data
      void timeseriesData(TimeSeries tsData);

      /// contour level limits
      void contourLevels(Vector& contourValues);

      /// color map range min and max values
      void colorMapRange(double min, double max);

      /// specific color map color stop values
      void colorLevels(Vector& colorLevels);

      /// update based on center and span
      void xCenterSpan(double center, double span);

    protected:
      /// drop target support for drag/drop operations
      void dragEnterEvent(QDragEnterEvent *e) { e->accept(); }

    private:
      QwtLinearColorMap m_colorMap;
      QwtPlotSpectrogram* m_spectrogram;
      QwtScaleWidget* m_rightAxis;
      FloodPlotData::Ptr m_floodPlotData;
      FloodPlotColorMap::ColorMapList m_colorMapType;
      QwtDoubleInterval m_dataRange;
      Vector m_colorLevels;
      void initColorMap();
      void initColorBar();
      int m_colorMapLength;
      double centerXValue(double center);
      double spanXValue(double span);
      void init();
      double m_xAxisMin;
      double m_xAxisMax;
      Plot2DTimeAxis *m_plot2DTimeAxis;
      double m_duration;
      openstudio::DateTime m_startDateTime;
      openstudio::DateTime m_endDateTime;
      std::string m_plotSource;
      std::string m_alias;
      std::string m_legendName;
      void updatePlotTitle();


      private slots:
        void selectColorMap();
      void showContourPlot();
  };

} // openstudio

#endif // UTILITIES_PLOT_FLOODPLOT_HPP
