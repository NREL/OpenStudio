#ifndef UTILITIES_DATA_TIMESERIES_I
#define UTILITIES_DATA_TIMESERIES_I

%{
#ifdef _MSC_VER
  #pragma warning( disable : 4190 )
#endif

  #include <utilities/data/TimeSeries.hpp>
%}

%ignore openstudio::detail;

%template(TimeSeriesPtr) std::shared_ptr<openstudio::TimeSeries>;

// create an instantiation of the optional class
%template(OptionalTimeSeries) boost::optional<openstudio::TimeSeries>;

// create an instantiation of the vector class
%template(TimeSeriesPtrVector) std::vector< std::shared_ptr<openstudio::TimeSeries> >;
%template(TimeSeriesVector) std::vector< openstudio::TimeSeries >;

%template(TimeSeriesFromTimeSeriesVectorFunctor) boost::function1<openstudio::TimeSeries, const std::vector<openstudio::TimeSeries>&>;


// include the header into the swig interface directly
%include <utilities/data/TimeSeries.hpp>


#endif //UTILITIES_DATA_TIMESERIES_I 
