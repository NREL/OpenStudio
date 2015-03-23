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

#ifndef RESULTSVIEWER_FLOODPLOT_HPP
#define RESULTSVIEWER_FLOODPLOT_HPP

#include "../utilities/data/TimeSeries.hpp"
#include "../utilities/data/Vector.hpp"
#include "../utilities/data/Matrix.hpp"

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
  *   \deprecated { Qwt drawing widgets are deprecated in favor of Javascript }
  */
  class  FloodPlotColorMap: public QwtLinearColorMap
  {
    public:

      /// available color maps
      enum ColorMapList {Gray, Jet};

      /// constructor
      FloodPlotColorMap(const Vector& colorLevels, ColorMapList colorMap);

      /// virtual destructor
      virtual ~FloodPlotColorMap();

    private:
      // Disabled copy constructor and operator=
      FloodPlotColorMap(const FloodPlotColorMap &);
      FloodPlotColorMap &operator=(const FloodPlotColorMap &);

      Vector m_colorLevels;
      ColorMapList m_colorMapList;
      void init();
  };


  /** FloodPlotData is abstract class for data that can be used in a flood plot.
   *  Derive from this class to plot your data.
   *  \deprecated { Qwt drawing widgets are deprecated in favor of Javascript }
   */
  class  FloodPlotData: public QwtRasterData
  {
    public:

      /// virtual destructor
      virtual ~FloodPlotData();

      /// must provide copy
      virtual FloodPlotData* copy() const = 0;

      /// must provide range of values - colormap range not data range
      //QwtInterval range() const = 0;

      // color map range
      virtual void colorMapRange(QwtInterval colorMapRange) = 0;

      /// color map range
      virtual QwtInterval colorMapRange() = 0;


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

      FloodPlotData();
  };

  /** TimeSeriesFloodPlotData converts a time series into flood plot data
  *   \deprecated { Qwt drawing widgets are deprecated in favor of Javascript }
  */
  class  TimeSeriesFloodPlotData: public FloodPlotData
  {
    public:

      /// constructor
      TimeSeriesFloodPlotData(TimeSeries timeSeries);

      /// constructor with colormap range
      TimeSeriesFloodPlotData(TimeSeries timeSeries, QwtInterval colorMapRange );

      /// virtual destructor
      virtual ~TimeSeriesFloodPlotData();

      /// must provide copy
      virtual TimeSeriesFloodPlotData* copy() const;

      /// must provide range of values - colormap range not data range
      QwtInterval range() const;

      /// provide boundingRect overload for speed - default implementation slow!!!
      QRectF boundingRect() const;

      /// provide size of each pixel
      virtual QRectF pixelHint(const QRectF& area) const;

      ///  value at point fractionalDay and hourOfDay
      double value(double fractionalDay, double hourOfDay) const;

      /// minX
      double minX() const;

      /// maxX
      double maxX() const;

      /// minY
      double minY() const;

      /// maxY
      double maxY() const;

      /// minValue
      double minValue() const;

      /// maxValue
      double maxValue() const;

      /// minValue setter
      void minValue(double min);

      /// maxValue setter
      void maxValue(double max);

      /// sumValue
      double sumValue() const;

      /// meanValue
      double meanValue() const;

      /// stdDevValue
      double stdDevValue() const;

      /// range of values for which to show the colormap
      void colorMapRange(QwtInterval colorMapRange);

      /// range of values for which to show the colormap
      QwtInterval colorMapRange();

      /// units for plotting on axes or scaling
      std::string units() const;

    private:
      TimeSeries m_timeSeries;
      double m_minValue;
      double m_maxValue;
      double m_minX;
      double m_maxX;
      double m_minY;
      double m_maxY;
      double m_startFractionalDay;
      QwtInterval m_colorMapRange;
      std::string m_units;
  };

  /** MatrixFloodPlotData converts a Matrix into flood plot data
  *   \deprecated { Qwt drawing widgets are deprecated in favor of Javascript }
  */
  class  MatrixFloodPlotData: public FloodPlotData
  {
    public:

      /// constructor
      MatrixFloodPlotData(const Matrix& matrix);

      /// constructor and color map range
      MatrixFloodPlotData(const Matrix& matrix, QwtInterval colorMapRange );

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
          QwtInterval colorMapRange );

      /// virtual destructor
      virtual ~MatrixFloodPlotData();

      /// must provide copy
      virtual MatrixFloodPlotData* copy() const;

      /// must provide range of values - colormap range not data range
      QwtInterval range() const;

      /// provide size of each pixel
      virtual QRectF pixelHint(const QRectF& area) const;

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
      double minValue() const;

      /// maxValue
      double maxValue() const;

      /// minValue setter
      void minValue(double min);

      /// maxValue setter
      void maxValue(double max);

      /// sumValue
      double sumValue() const;

      /// meanValue
      double meanValue() const;

      /// stdDevValue
      double stdDevValue() const;

      /// range of values for which to show the colormap
      void colorMapRange(QwtInterval colorMapRange);

      /// range of values for which to show the colormap
      QwtInterval colorMapRange();

      /// units for plotting on axes or scaling
      std::string units() const;

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
      QwtInterval m_colorMapRange;
      Vector m_colorMapScaleValues;
      std::string m_units;
  };

} // openstudio

#endif // RESULTSVIEWER_FLOODPLOT_HPP
