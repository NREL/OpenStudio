#start the measure
class UtilityMeasure < OpenStudio::Ruleset::UtilityUserScript
  
  #define the name that a user will see, this method may be deprecated as
  #the display name in PAT comes from the name field in measure.xml
  def name
    return "UtilityMeasure"
  end
  
  #define the arguments that the user will input
  def arguments()
    args = OpenStudio::Ruleset::OSArgumentVector.new
    
    #make an argument for your name
    user_name = OpenStudio::Ruleset::OSArgument::makeStringArgument("user_name",true)
    user_name.setDisplayName("What is your name?")
    args << user_name

    #make an argument to print the name or not
    say_hello = OpenStudio::Ruleset::OSArgument::makeBoolArgument("say_hello",true)
    say_hello.setDisplayName("Do you want to say hello?")
    say_hello.setDefaultValue(true)
    args << say_hello
    
    return args
  end #end the arguments method

  #define what happens when the measure is run
  def run(runner, user_arguments)
    super(runner, user_arguments)
    
    #use the built-in error checking 
    if not runner.validateUserArguments(arguments(), user_arguments)
      return false
    end

    #assign the user inputs to variables
    user_name = runner.getStringArgumentValue("user_name",user_arguments)
    say_hello = runner.getBoolArgumentValue("say_hello",user_arguments)
    
    #check the user_name for reasonableness
    puts user_name
    if user_name == ""
      runner.registerError("No Name was Entered.")
      return false
    end
    
    #ruby test to see if first charter of string is uppercase letter
    if not user_name[0..0] =~ /[A-Z]/ 
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

#this allows the measure to be use by the application
UtilityMeasure.new.registerWithApplication