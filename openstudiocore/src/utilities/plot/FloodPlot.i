#ifndef UTILITIES_PLOT_FLOODPLOT_I
#define UTILITIES_PLOT_FLOODPLOT_I

%{
  #include <utilities/plot/FloodPlot.hpp>
%}

// create an instantiation of the smart ptr classes
%template(FloodPlotDataPtr) boost::shared_ptr<openstudio::FloodPlotData>;
%template(TimeSeriesFloodPlotDataPtr) boost::shared_ptr<openstudio::TimeSeriesFloodPlotData>;
%template(MatrixFloodPlotDataPtr) boost::shared_ptr<openstudio::MatrixFloodPlotData>;
%template(FloodPlotPtr) boost::shared_ptr<openstudio::FloodPlot>;

// create an instantiation of the vector classes
%template(FloodPlotDataVector) std::vector< boost::shared_ptr<openstudio::FloodPlotData> >;
%template(TimeSeriesFloodPlotDataVector) std::vector< boost::shared_ptr<openstudio::TimeSeriesFloodPlotData> >;
%template(MatrixFloodPlotDataVector) std::vector< boost::shared_ptr<openstudio::MatrixFloodPlotData> >;
%template(FloodPlotVector) std::vector< boost::shared_ptr<openstudio::FloodPlot> >;

%include <utilities/plot/FloodPlot.hpp>

// DLM@20090803: extremely lame hack because we are not able to automatically convert
// shared_ptr<Derived> to shared_ptr<Base>.  Presumably the SWIG_SHARED_PTR and SWIG_SHARED_PTR_DERIVED
// macros in shared_ptr.i work for this, but they are only implemented for Python, C#, and Java
%extend openstudio::FloodPlot{
    void floodPlotData(boost::shared_ptr<openstudio::TimeSeriesFloodPlotData> data){
        self->floodPlotData(boost::dynamic_pointer_cast<openstudio::FloodPlotData>(data));
    }
    void floodPlotData(boost::shared_ptr<openstudio::MatrixFloodPlotData> data){
        self->floodPlotData(boost::dynamic_pointer_cast<openstudio::FloodPlotData>(data));
    }
}

#endif //UTILITIES_PLOT_FLOODPLOT_I