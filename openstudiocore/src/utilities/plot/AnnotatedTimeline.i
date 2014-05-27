#ifndef UTILITIES_PLOT_ANNOTATED_TIMELINE_I
#define UTILITIES_PLOT_ANNOTATED_TIMELINE_I

%{
  #include <utilities/plot/AnnotatedTimeline.hpp>
%}

// create an instantiation of the smart ptr class
%template(AnnotatedTimelinePtr) std::shared_ptr<openstudio::AnnotatedTimeline>;

// create an instantiation of the vector class
%template(AnnotatedTimelineVector) std::vector< std::shared_ptr<openstudio::AnnotatedTimeline> >;

%include <utilities/plot/AnnotatedTimeline.hpp>

#endif //UTILITIES_PLOT_ANNOTATED_TIMELINE_I