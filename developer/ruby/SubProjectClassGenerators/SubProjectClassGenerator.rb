########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#   Base class for sub-project method generators.
#
######################################################################

class SubProjectClassGenerator
  attr_accessor :className, :baseClassName, :pImpl, :qobject

  def initialize(className, baseClassName, pImpl, qobject)
    @className = className
    @baseClassName = baseClassName
    @pImpl = pImpl
    @qobject = qobject
  end

  # INCLUDES

  def hppIncludes()
    result = String.new
    return result
  end

  def implHppIncludes()
    result = String.new
    return result
  end

  def cppIncludes()
    result = String.new
    return result
  end

  # FORWARD DECLARATIONS

  def hppExternalForwardDeclarations
    result = String.new
    return result
  end

  def implHppExternalForwardDeclarations
    result = String.new
    return result
  end

  def hppOSForwardDeclarations
    result = String.new
    return result
  end

  def implHppOSForwardDeclarations
    result = String.new
    return result
  end

  def hppSubProjectForwardDeclarations
    result = String.new
    return result
  end

  def implHppSubProjectForwardDeclarations
    result = String.new
    return result
  end

  # PRE-CLASS

  def hppPreClass()
    result = String.new
    if not baseClassName.empty?
      result << "  /** " << className << " is a " << baseClassName << ". */\n"
    end
    return result
  end

  def implHppPreClass()
    result = String.new
    if @pImpl
      result << "  /** " << className << "_Impl is "
      if not baseClassName.empty?
        result << "a " << baseClassName << "_Impl that is "
      end
      result << "the implementation class for " << className << ".*/\n"
    end
    return result
  end

  def cppPreClass()
    result = String.new
    return result
  end

  def cppPublicClassPreClass()
    result = String.new
    return result
  end

  # QMACROS

  def hppQMacros()
    result = String.new
    return result
  end

  def implHppQMacros()
    result = String.new
    return result
  end

  # PUBLIC TYPEDEFS

  def hppPublicTypedefs()
    result = String.new
    return result
  end

  def implHppPublicTypedefs()
    result = String.new
    return result
  end

  # CLASS CONSTRUCTORS

  def hppConstructors()
    result = String.new
    return result
  end

  def implHppConstructors()
    result = String.new
    return result
  end

  def cppConstructors()
    result = String.new
    return result
  end

  def cppPublicClassConstructors()
    result = String.new
    return result
  end

  # CLASS PUBLIC METHODS

  def hppPublicMethods()
    result = String.new
    return result
  end

  def implHppPublicMethods()
    result = String.new
    return result
  end

  def cppPublicMethods()
    result = String.new
    return result
  end

  def cppPublicClassPublicMethods()
    result = String.new
    return result
  end

  # PROTECTED IMPLEMENTATION AND FRIENDS

  def hppProtectedImpl()
    result = String.new

    if pImpl
      result << "    using ImplType = detail::" << className << "_Impl;\n\n"
      result << "    explicit " << className << "(std::shared_ptr<detail::" << className << "_Impl> impl);\n\n"
      result << "    friend class detail::" << @className << "_Impl;\n"
    end

    return result
  end

  def hppProtectedFriends()
    result = String.new
    return result
  end

  def cppPublicClassProtectedImpl()
    result = String.new

    if @pImpl
      result << "  " << className << "::" << className << "(\n"
      result << "    std::shared_ptr<detail::" << className << "_Impl> impl)\n"
      if @baseClassName.empty?
        result << "    : m_impl(impl) {}\n"
      else
        result << "    : " << baseClassName << "(std::move(impl)) {}\n"
      end
    end

    return result
  end

  # CLASS PROTECTED METHODS

  def hppProtectedMethods()
    result = String.new
    return result
  end

  def implHppProtectedMethods()
    result = String.new
    return result
  end

  def cppProtectedMethods()
    result = String.new
    return result
  end

  def cppPublicClassProtectedMethods()
    result = String.new
    return result
  end

  # CLASS PRIVATE METHODS

  def hppPrivateMethods()
    result = String.new
    return result
  end

  def implHppPrivateMethods()
    result = String.new
    return result
  end

  def cppPrivateMethods()
    result = String.new
    return result
  end

  def cppPublicClassPrivateMethods()
    result = String.new
    return result
  end

  # POST-CLASS

  def hppPostClass()
    result = String.new
    result << "  /** \\relates " << className << "*/\n"
    result << "  using Optional" << className << " = boost::optional<" << className << ">;\n\n"
    result << "  /** \\relates " << className << "*/\n"
    result << "  using " << className << "Vector = std::vector<" << className << ">;\n\n"
    return result
  end

  def implHppPostClass()
    result = String.new
    return result
  end

  def cppPostClass()
    result = String.new
    return result
  end

  def cppPublicClassPostClass()
    result = String.new
    return result
  end

  def gtest()
    result = String.new
    return result
  end

  def aux()
    result = String.new
    return result
  end

end
