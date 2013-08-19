#see the URL below for information on how to write OpenStudio measures
# http://openstudio.nrel.gov/openstudio-measure-writing-guide

#see the URL below for information on using life cycle cost objects in OpenStudio
# http://openstudio.nrel.gov/openstudio-life-cycle-examples

#see the URL below for access to C++ documentation on model objects (click on "model" in the main window to view model objects)
# http://openstudio.nrel.gov/sites/openstudio.nrel.gov/files/nv_data/cpp_documentation_it/model/html/namespaces.html

#start the measure
class ModelMeasure < OpenStudio::Ruleset::ModelUserScript
  
  #define the name that a user will see, this method may be deprecated as
  #the display name in PAT comes from the name field in measure.xml
  def name
    return "ModelMeasure"
  end
  
  #define the arguments that the user will input
  def arguments(model)
    args = OpenStudio::Ruleset::OSArgumentVector.new
    
    #make an argument for your name
    user_name = OpenStudio::Ruleset::OSArgument::makeStringArgument("user_name",true)
    user_name.setDisplayName("What is your name?")
    args << user_name

    #make an argument to add new space true/false
    add_space = OpenStudio::Ruleset::OSArgument::makeBoolArgument("add_space",true)
    add_space.setDisplayName("Add a space to your model?")
    add_space.setDefaultValue(true)
    args << add_space
    
    return args
  end #end the arguments method

  #define what happens when the measure is run
  def run(model, runner, user_arguments)
    super(model, runner, user_arguments)
    
    #use the built-in error checking 
    if not runner.validateUserArguments(arguments(model), user_arguments)
      return false
    end

    #assign the user inputs to variables
    user_name = runner.getStringArgumentValue("user_name",user_arguments)
    add_space = runner.getBoolArgumentValue("add_space",user_arguments)

    #check the user_name for reasonableness
    if user_name == ""
      runner.registerError("No Name was Entered.")
      return false
    end 
    
    #ruby test to see if first charter of string is uppercase letter
    if not user_name[0..0] =~ /[A-Z]/ 
      runner.registerWarning("Your entered name was not capitalized.")    
    end
    
    #returning the name in an a sentence
    runner.registerInfo("Hello #{user_name}, thanks for entering your name.")

    #reporting initial condition of model
    starting_spaces = model.getSpaces
    runner.registerInitialCondition("The building started with #{starting_spaces.size} spaces.")

    #adding a new space to the model
    if add_space == true
      new_space = OpenStudio::Model::Space.new(model)
      new_space.setName("Hello_World test space")
      runner.registerInfo("Space #{new_space.name} was added.")
      
    else
      runner.registerAsNotApplicable("You have chosen not to add a space. Your model will not be altered.")
      return true
    end

    #reporting final condition of model
    finishing_spaces = model.getSpaces
    runner.registerFinalCondition("The building finished with #{finishing_spaces.size} spaces.")
    
    return true
 
  end #end the run method

end #end the measure

#this allows the measure to be use by the application
ModelMeasure.new.registerWithApplication