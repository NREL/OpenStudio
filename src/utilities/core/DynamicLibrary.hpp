#if defined(_MSC_VER)
#  include "DynamicLibraryWindows.hpp"
#elif defined(HAVE_UNISTD_H)
#  include "DynamicLibraryPOSIX.hpp"
#endif
