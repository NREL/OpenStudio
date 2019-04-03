#ifndef UTILITIES_DATA_ENDUSES_I
#define UTILITIES_DATA_ENDUSES_I
%{
  #include <utilities/data/EndUses.hpp>
%}

// create an instantiation of the optional class
%template(OptionalEndUses) boost::optional< openstudio::EndUses >;

// create an instantiation of the vector class
%template(EndUsesVector) std::vector< openstudio::EndUses >;

%include <utilities/data/EndUses.hpp>

#endif //UTILITIES_DATA_ENDUSES_I
