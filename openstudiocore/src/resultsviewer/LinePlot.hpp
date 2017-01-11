/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef RESULTSVIEWER_LINEPLOT_HPP
#define RESULTSVIEWER_LINEPLOT_HPP

#include "../utilities/data/TimeSeries.hpp"
#include "../utilities/data/Vector.hpp"
#include "../utilities/data/Matrix.hpp"
 
#include <QWidget>
#include <QPushButton>
#include <QAction>
#include <QColor>
 
#include <qwt/qwt_legend.h>
#include <qwt/qwt_plot_grid.h>
#include <qwt/qwt_legend_label.h>
#include <qwt/qwt_text.h>
#include <qwt/qwt_color_map.h>
#include <qwt/qwt_series_data.h>
#include <qwt/qwt_scale_draw.h>
#include <qwt/qwt_plot_zoomer.h>
#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_plot_layout.h>
#include <qwt/qwt_point_data.h>

#include <cmath>

namespace openstudio{

/** LinePlotData is abstract class for data that can be used in a line plot.
 *  Derive from this class to plot your data.
 *  \deprecated { Qwt drawing widgets are deprecated in favor of Javascript }
 */
class  LinePlotData : public QwtSeriesData<QPointF>
{
public:

  /// virtual destructor
  virtual ~LinePlotData() {}

  /// must provide copy
  virtual LinePlotData* copy() const = 0;

  /// get the value at point x
// virtual double value(double x) const = 0;

  /// minX
  virtual double minX() const = 0;

  /// maxX
  virtual double maxX() const = 0;

  /// minY
  virtual double minY() const = 0;

  /// maxY
  virtual double maxY() const = 0;

  /// minValue
  virtual double minValue() const = 0;

  /// maxValue
  virtual double maxValue() const = 0;

  /// sumValue
  virtual double sumValue() const = 0;

  /// meanValue
  virtual double meanValue() const = 0;

  /// stdDevValue
  virtual double stdDevValue() const = 0;

  /// units for plotting on axes or scaling
  virtual void units(const std::string &unit) = 0;

  /// units for plotting on axes or scaling
  virtual std::string units() const = 0;

  virtual size_t size() const = 0;
  
  virtual QPointF sample(size_t i) const = 0;

  virtual QRectF boundingRect() const = 0;

protected:
  LinePlotData() {}
};

/** TimeSeriesLinePlotData converts a time series into Line plot data
*   \deprecated { Qwt drawing widgets are deprecated in favor of Javascript }
*/
class  TimeSeriesLinePlotData: public LinePlotData
{
public:

  /// constructor
  TimeSeriesLinePlotData(TimeSeries timeSeries);
  
  /// constructor
  TimeSeriesLinePlotData(TimeSeries timeSeries, double fracDaysOffset);

  /// virtual destructor
  virtual ~TimeSeriesLinePlotData();

  /// must provide copy
  virtual TimeSeriesLinePlotData* copy() const override;

  /// value at fractional day - used by qwtPlot engine
//  double value(double fractionalDay) const;

  /// minX
  double minX() const override;

  /// maxX
  double maxX() const override;

  /// minY
  double minY() const override;

  /// maxY
  double maxY() const override;

  /// minValue
  double minValue() const override;

  /// maxValue
  double maxValue() const override;

  /// sumValue
  double sumValue() const override;

  /// meanValue
  double meanValue() const override;

  /// stdDevValue
  double stdDevValue() const override;

  /// reimplement bounding rect for speed
  QRectF boundingRect() const override;

  /// reimplement sample
  QPointF sample(size_t i) const override;

  /// reimplement abstract function size
  size_t size(void) const override;

  /// reimplement abstract function x
  double x(size_t pos) const;

  /// reimplement abstract function y
  double y(size_t pos) const;

  /// units for plotting on axes or scaling
  void units(const std::string &unit) override;

  /// units for plotting on axes or scaling
  std::string units() const override;

private:
  TimeSeries m_timeSeries;
  double m_minValue;
  double m_maxValue;
  double m_minX;
  double m_maxX;
  double m_minY;
  double m_maxY;
  size_t m_size;
  QRectF m_boundingRect;
  std::string m_units;
  double m_fracDaysOffset;
  // testing Vector class
  Vector m_x;
  Vector m_y;
};

/** VectorLinePlotData converts two Vectors into Line plot data
*   \deprecated { Qwt drawing widgets are deprecated in favor of Javascript }
*/
class  VectorLinePlotData: public LinePlotData
{
public:

  /// constructor with x and y vectors
  VectorLinePlotData(const Vector& xVector, const Vector& yVector);

  /// virtual destructor
  virtual ~VectorLinePlotData() {}

  /// must provide copy
  virtual VectorLinePlotData* copy() const override;

  /// get the value at point x
//  double value(double x) const;

  /// set the interp method, defaults to Nearest
  void interpMethod(InterpMethod interpMethod);

  /// minX
  double minX() const override;

  /// maxX
  double maxX() const override;

  /// minY
  double minY() const override;

  /// maxY
  double maxY() const override;

  /// minValue
  double minValue() const override;

  /// maxValue
  double maxValue() const override;

  /// sumValue
  double sumValue() const override;

  /// meanValue
  double meanValue() const override;

  /// stdDevValue
  double stdDevValue() const override;

  /// reimplement bounding rect for speed
  QRectF boundingRect() const override;

  /// reimplement sample
  QPointF sample(size_t i) const override;

  /// reimplement abstract function size
  size_t size(void) const override;

  /// reimplement abstract function x
  double x(size_t pos) const;

  /// reimplement abstract function y
  double y(size_t pos) const;

  /// units for plotting on axes or scaling
  void units(const std::string &unit) override;

  /// units for plotting on axes or scaling
  std::string units() const override;

private:

  // set ranges and bounding box
  void init();

  Vector m_xVector;
  Vector m_yVector;
  InterpMethod m_interpMethod;
  double m_minX, m_maxX, m_minY, m_maxY;
  QRectF m_boundingRect;
  size_t m_size;
  std::string m_units;
};
 
} // openstudio

#endif // RESULTSVIEWER_LINEPLOT_HPP
