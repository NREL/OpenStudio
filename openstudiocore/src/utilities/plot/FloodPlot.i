#ifndef UTILITIES_PLOT_FLOODPLOT_I
#define UTILITIES_PLOT_FLOODPLOT_I

%{
  #include <utilities/plot/FloodPlot.hpp>
%}

// create an instantiation of the smart ptr classes
%template(FloodPlotDataPtr) std::shared_ptr<openstudio::FloodPlotData>;
%template(TimeSeriesFloodPlotDataPtr) std::shared_ptr<openstudio::TimeSeriesFloodPlotData>;
%template(MatrixFloodPlotDataPtr) std::shared_ptr<openstudio::MatrixFloodPlotData>;
%template(FloodPlotPtr) std::shared_ptr<openstudio::FloodPlot>;

// create an instantiation of the vector classes
%template(FloodPlotDataVector) std::vector< std::shared_ptr<openstudio::FloodPlotData> >;
%template(TimeSeriesFloodPlotDataVector) std::vector< std::shared_ptr<openstudio::TimeSeriesFloodPlotData> >;
%template(MatrixFloodPlotDataVector) std::vector< std::shared_ptr<openstudio::MatrixFloodPlotData> >;
%template(FloodPlotVector) std::vector< std::shared_ptr<openstudio::FloodPlot> >;

%include <utilities/plot/FloodPlot.hpp>

// DLM@20090803: extremely lame hack because we are not able to automatically convert
// shared_ptr<Derived> to shared_ptr<Base>.  Presumably the SWIG_SHARED_PTR and SWIG_SHARED_PTR_DERIVED
// macros in shared_ptr.i work for this, but they are only implemented for Python, C#, and Java
%extend openstudio::FloodPlot{
    void floodPlotData(std::shared_ptr<openstudio::TimeSeriesFloodPlotData> data){
        self->floodPlotData(std::dynamic_pointer_cast<openstudio::FloodPlotData>(data));
    }
    void floodPlotData(std::shared_ptr<openstudio::MatrixFloodPlotData> data){
        self->floodPlotData(std::dynamic_pointer_cast<openstudio::FloodPlotData>(data));
    }
}

#endif //UTILITIES_PLOT_FLOODPLOT_I