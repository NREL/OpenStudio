#ifndef UTILITIES_PLOT_LINEPLOT_I
#define UTILITIES_PLOT_LINEPLOT_I

%{
  #include <utilities/plot/LinePlot.hpp>
%}

// create an instantiation of the smart ptr classes
%template(LinePlotDataPtr) std::shared_ptr<openstudio::LinePlotData>;
%template(TimeSeriesLinePlotDataPtr) std::shared_ptr<openstudio::TimeSeriesLinePlotData>;
%template(VectorLinePlotDataPtr) std::shared_ptr<openstudio::VectorLinePlotData>;
%template(LinePlotPtr) std::shared_ptr<openstudio::LinePlot>;

// create an instantiation of the vector classes
%template(LinePlotDataVector) std::vector< std::shared_ptr<openstudio::LinePlotData> >;
%template(TimeSeriesLinePlotDataVector) std::vector< std::shared_ptr<openstudio::TimeSeriesLinePlotData> >;
%template(VectorLinePlotDataVector) std::vector< std::shared_ptr<openstudio::VectorLinePlotData> >;
%template(LinePlotVector) std::vector< std::shared_ptr<openstudio::LinePlot> >;

%include <utilities/plot/LinePlot.hpp>

// DLM@20090803: extremely lame hack because we are not able to automatically convert
// shared_ptr<Derived> to shared_ptr<Base>.  Presumably the SWIG_SHARED_PTR and SWIG_SHARED_PTR_DERIVED
// macros in shared_ptr.i work for this, but they are only implemented for Python, C#, and Java
%extend openstudio::LinePlot{
    void linePlotData(std::shared_ptr<openstudio::TimeSeriesLinePlotData> data, const std::string& name, QColor color = Qt::blue){
        self->linePlotData(std::dynamic_pointer_cast<openstudio::TimeSeriesLinePlotData>(data), name, color);
    }
    void linePlotData(std::shared_ptr<openstudio::VectorLinePlotData> data, const std::string& name, QColor color = Qt::blue){
        self->linePlotData(std::dynamic_pointer_cast<openstudio::VectorLinePlotData>(data), name, color);
    }
}

#endif //UTILITIES_PLOT_LINEPLOT_I