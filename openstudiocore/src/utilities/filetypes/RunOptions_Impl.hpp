/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef UTILITIES_FILETYPES_RUNOPTIONS_IMPL_HPP
#define UTILITIES_FILETYPES_RUNOPTIONS_IMPL_HPP

#include "../UtilitiesAPI.hpp"

#include "../core/Logger.hpp"
#include "../core/Path.hpp"
#include "../data/Variant.hpp"

#include <jsoncpp/json.h>

#include <nano/nano_signal_slot.hpp> 

namespace openstudio{
namespace detail {

  class UTILITIES_API RunOptions_Impl
  {
  public:

    RunOptions_Impl();

    virtual ~RunOptions_Impl();

    std::string string() const;

    bool debug() const;
    bool setDebug(bool debug);
    void resetDebug();

    bool preserveRunDir() const;
    bool setPreserveRunDir(bool preserve);
    void resetPreserveRunDir();

    bool cleanup() const;
    bool setCleanup(bool cleanup);
    void resetCleanup();

    boost::optional<CustomOutputAdapter> customOutputAdapter() const;
    bool setCustomOutputAdapter(const CustomOutputAdapter& adapter);
    void resetCustomOutputAdapter();

    // Emitted on any change
    Nano::Signal<void()> onChange;

  protected:
    
    void onUpdate();

  private:

    // configure logging
    REGISTER_LOGGER("openstudio.RunOptions");

    bool m_debug;
    bool m_preserveRunDir;
    bool m_cleanup;
    boost::optional<CustomOutputAdapter> m_customOutputAdapter;
  };

} // detail
} // openstudio

#endif //UTILITIES_FILETYPES_RUNOPTIONS_IMPL_HPP
