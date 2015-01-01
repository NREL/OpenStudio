######################################################################
#  Copyright (c) 2008-2015, Alliance for Sustainable Energy.  
#  All rights reserved.
#  
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License, or (at your option) any later version.
#  
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#  
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
######################################################################

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
      result << "/** " << className << " is a " << baseClassName << ". */\n"
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
      result << "  typedef detail::" << className << "_Impl ImplType;\n\n"
      result << "  explicit " << className << "(boost::shared_ptr<detail::" << className << "_Impl> impl);\n\n"
      result << "  friend class detail::" << @className << "_Impl;\n"
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
      result << className << "::" << className << "(boost::shared_ptr<detail::" << className << "_Impl> impl)\n"
      if @baseClassName.empty?
        result << "  : m_impl(impl)\n"
      else
        result << "  : " << baseClassName << "(impl)\n"
      end
      result << "{}\n"
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
    result << "/** \\relates " << className << "*/\n"
    result << "typedef boost::optional<" << className << "> Optional" << className << ";\n\n"
    result << "/** \\relates " << className << "*/\n"
    result << "typedef std::vector<" << className << "> " << className << "Vector;\n\n"
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
