########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
#  following conditions are met:
#
#  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
#  disclaimer.
#
#  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
#  disclaimer in the documentation and/or other materials provided with the distribution.
#
#  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
#  derived from this software without specific prior written permission from the respective party.
#
#  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
#  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
#  written permission from Alliance for Sustainable Energy, LLC.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
#  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
#  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
#  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
#  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
########################################################################################################################

######################################################################
# == Synopsis
#
#   Generator for analysis::AnalysisObject classes.
#
######################################################################

require File.dirname(__FILE__) + '/SubProjectClassGenerator.rb'

class AnalysisClassGenerator < SubProjectClassGenerator

  def initialize(className, baseClassName, pImpl, qobject)
    super(className, baseClassName, pImpl, qobject)
  end

  def hppConstructors()
    result = String.new

    result << "  // TODO: Delete if this class is abstract. Otherwise, add other items necessary for construction.\n"
    result << "  // Algorithms have fixed names, and DataPoints have no names by default, so delete the name\n"
    result << "  // argument in those cases.\n"
    result << "  " << @className << "(const std::string& name);\n\n"

    buffer = "   " + " " * @className.size

    result << "  // TODO: Delete if this class is abstract. Otherwise, add other items necessary for construction.\n"
    result << "  /** Constructor provided for deserialization; not for general use. */\n"
    result << "  " << @className << "(const UUID& uuid,\n"
    result << buffer << "const UUID& versionUUID,\n"
    result << buffer << "const std::string& name,\n"
    result << buffer << "const std::string& displayName,\n"
    result << buffer << "const std::string& description);\n\n"

    return result
  end

  def implHppConstructors()
    result = String.new

    result << "    // TODO: Add other items necessary for construction.\n"
    result << "    " << @className << "_Impl(const std::string& name);\n\n"

    buffer = "          " + " " * @className.size

    result << "    // TODO: Add other items necessary for construction.\n"
    result << "    " << @className << "_Impl(const UUID& uuid,\n"
    result << buffer << "const UUID& versionUUID,\n"
    result << buffer << "const std::string& name,\n"
    result << buffer << "const std::string& displayName,\n"
    result << buffer << "const std::string& description);\n\n"

    return result
  end

  def cppConstructors()
    result = String.new

    result << "  " << @className << "_Impl::" << @className << "_Impl(const std::string& name)\n"
    result << "    : " << @baseClassName << "_Impl(name)\n"
    result << "  {}\n\n"

    buffer = "               " + " " * (@className.size * 2)

    result << "  " << @className << "_Impl::" << @className << "_Impl(const UUID& uuid,\n"
    result << buffer << "const UUID& versionUUID,\n"
    result << buffer << "const std::string& name,\n"
    result << buffer << "const std::string& displayName,\n"
    result << buffer << "const std::string& description)\n"
    result << "    : " << @baseClassName << "_Impl(uuid,versionUUID,name,displayName,description)\n"
    result << "  {}\n\n"

    return result
  end

  def cppPublicClassConstructors()
    result = String.new

    result << "// TODO: Delete if this class is abstract.\n"
    result << @className << "::" << @className << "(const std::string& name)\n"
    result << "  : " << @baseClassName << "(std::shared_ptr<detail::" << @className << "_Impl>(\n"
    result << "        " << "new detail::" << @className << "_Impl(name)))\n"
    result << "{}\n\n"

    buffer = "   " + " " * (@className.size * 2)

    result << "// TODO: Delete if this class is abstract.\n"
    result << @className << "::" << @className << "(const UUID& uuid,\n"
    result << buffer << "const UUID& versionUUID,\n"
    result << buffer << "const std::string& name,\n"
    result << buffer << "const std::string& displayName,\n"
    result << buffer << "const std::string& description)\n"

    buffer = "                          " + " " * @className.size

    result << "  : " << @baseClassName << "(std::shared_ptr<detail::" << @className << "_Impl>(\n"
    result << "        " << "new detail::" << @className << "_Impl(uuid,\n"
    result << buffer << "versionUUID,\n"
    result << buffer << "name,\n"
    result << buffer << "displayName,\n"
    result << buffer << "description)\n"
    result << "{}\n\n"

    return result
  end

  def hppPublicMethods()
    result = String.new
    result << "  /** @name Getters and Queries */\n"
    result << "  //@{\n\n"
    result << "  // TODO: Provide getters for all items needed by the deserialization constructor.\n\n"
    result << "  //@}\n"
    result << "  /** @name Actions */\n"
    result << "  //@{\n\n"
    result << "  // TODO: Declare methods used by AnalysisDriver et al. in running analyses.\n\n"
    result << "  //@}\n"
    return result
  end

  def implHppPublicMethods()
    result = String.new

    result << "    /** @name Virtual Methods */\n"
    result << "    //@{\n\n"
    result << "    // TODO: Declare virtual methods that need to be overridden.\n\n"
    result << "    //@}\n"
    result << "    // TODO: Copy, and possibly modify, declarations made in the public class.\n"

    return result
  end

  def hppProtectedFriends()
    result = String.new
    result << "  friend class AnalysisObject;\n"
    result << "  friend class detail::AnalysisObject_Impl;\n"
    return result
  end

  def implHppProtectedMethods()
    result = String.new
    result << "    // TODO: Add protected data members.\n"
    return result
  end

end
