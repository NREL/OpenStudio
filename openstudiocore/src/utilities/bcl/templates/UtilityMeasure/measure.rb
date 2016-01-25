# see the URL below for information on how to write OpenStudio measures
# http://nrel.github.io/OpenStudio-user-documentation/reference/measure_writing_guide/

#start the measure
class UtilityMeasure < OpenStudio::Ruleset::UtilityUserScript

  # human readable name
  def name
    return "NAME_TEXT"
  end

  # human readable description
  def description
    return "DESCRIPTION_TEXT"
  end

  # human readable description of modeling approach
  def modeler_description
    return "MODELER_DESCRIPTION_TEXT"
  end

  #define the arguments that the user will input
  def arguments()
    args = OpenStudio::Ruleset::OSArgumentVector.new

    #make an argument for your name
    user_name = OpenStudio::Ruleset::OSArgument.makeStringArgument("user_name",true)
    user_name.setDisplayName("What is your name?")
    args << user_name

    #make an argument to print the name or not
    say_hello = OpenStudio::Ruleset::OSArgument.makeBoolArgument("say_hello",true)
    say_hello.setDisplayName("Do you want to say hello?")
    say_hello.setDefaultValue(true)
    args << say_hello

    return args
  end #end the arguments method

  #define what happens when the measure is run
  def run(runner, user_arguments)
    super(runner, user_arguments)

    #use the built-in error checking 
    if !runner.validateUserArguments(arguments(), user_arguments)
      return false
    end

    #assign the user inputs to variables
    user_name = runner.getStringArgumentValue("user_name", user_arguments)
    say_hello = runner.getBoolArgumentValue("say_hello", user_arguments)

    #check the user_name for reasonableness
    puts user_name
    if user_name == ""
      runner.registerError("No Name was Entered.")
      return false
    end

    #ruby test to see if first charter of string is uppercase letter
    if !/[A-Z]/.match(user_name[0..0])
      runner.registerWarning("Your entered name was not capitalized.")    
    end

    #returning the name in an a sentance
    runner.registerInfo("Hello #{user_name}, thanks for entering your name.")

    #reporting initial condition of model
    runner.registerInitialCondition("Hello #{user_name}.")

    # if say hello
    if say_hello == true
      runner.registerInfo("You have chosen to say hello.")
    else
      runner.registerAsNotApplicable("You have chosen not to say hello.")
    end

    #reporting final condition
    runner.registerFinalCondition("Goodbye.")

    return true

  end #end the run method

end #end the measure

# register the measure to be used by the application
UtilityMeasure.new.registerWithApplication
