#ifndef UTILITIES_CORE_SINGLETON_I
#define UTILITIES_CORE_SINGLETON_I

%{
  #include <utilities/core/Singleton.hpp>
%}

namespace openstudio{

  template<class T>
  class Singleton {
  public:
    static T& instance();
    //static void init(); // never throws
    ~Singleton();

  private:
     Singleton();
  };

} // openstudio

#endif //UTILITIES_CORE_SINGLETON_I
