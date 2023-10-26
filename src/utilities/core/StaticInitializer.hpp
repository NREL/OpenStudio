/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_CORE_STATICINITIALIZER_HPP
#define UTILITIES_CORE_STATICINITIALIZER_HPP

/// This template should be used as a base class for a class that has static
/// data to initialize. There is no thread safe way in windows to initialize static
/// data except to do it before main runs, or under some other controlled condition.
/// This template ensures that the static member function initialize() of the derived
/// class is called before main begins.
/// This code is not necessary with GCC because GCC puts static initializers in critical sections
template <typename T>
struct StaticInitializer
{
#ifndef __GNUC__
 private:
  struct object_creator
  {
    // This constructor does nothing more than ensure that instance()
    //  is called before main() begins, thus creating the static
    //  T object before multithreading race issues can come up.
    object_creator() {
      StaticInitializer<T>::do_initialize();
    }

    inline void do_nothing() const {}
  };
  static object_creator create_object;

 public:
  StaticInitializer() {
    do_initialize();
  }

  // If, at any point (in user code), Singleton<T>::instance()
  //  is called, then the following function is instantiated.
  static void do_initialize() {
    // call the static function
    T::initialize();

    // The following line does nothing else than force the instantiation
    //  of Singleton<T>::create_object, whose constructor is
    //  called before main() begins.
    create_object.do_nothing();
  }
#endif
};

#ifndef __GNUC__
template <typename T>
typename StaticInitializer<T>::object_creator StaticInitializer<T>::create_object;
#endif

#endif  // UTILITIES_CORE_STATICINITIALIZER_HPP
