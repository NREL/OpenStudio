#ifndef OPENSTUDIO_TEMPORARYDIRECTORY_
#define OPENSTUDIO_TEMPORARYDIRECTORY_

#include <utilities/UtilitiesAPI.hpp>
#include "Path.hpp"
#include "Logger.hpp"

namespace openstudio
{
  class UTILITIES_API TemporaryDirectory
  {
    public:
      TemporaryDirectory();
      ~TemporaryDirectory();


      openstudio::path path();

    private:
      openstudio::path m_path;

      REGISTER_LOGGER("openstudio.utilities.TemporaryDirectory");
  };
}

#endif
