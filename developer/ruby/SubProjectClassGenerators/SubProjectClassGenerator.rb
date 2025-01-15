########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
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
