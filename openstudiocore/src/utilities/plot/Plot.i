#ifndef UTILITIES_PLOT_PLOT_I
#define UTILITIES_PLOT_PLOT_I

// dummy classes because you can't ignore base classes
class QwtScaleDraw{};
class QwtLinearColorMap{};
class QwtRasterData{
  public:
  virtual double value(double x, double y) const = 0;
  protected:
  QwtRasterData();
};
class QwtData{
  public:
  virtual QwtData *copy() const = 0;
  protected:
  QwtData();
};

%include <utilities/plot/Plot2D.i>
%include <utilities/plot/FloodPlot.i>
%include <utilities/plot/LinePlot.i>
%include <utilities/plot/AnnotatedTimeline.i>
%include <utilities/plot/ProgressBar.i>

#endif //UTILITIES_PLOT_PLOT_I
