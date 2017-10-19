#see the URL below for information on how to write OpenStudio measures
# http://openstudio.nrel.gov/openstudio-measure-writing-guide

#see the URL below for information on using life cycle cost objects in OpenStudio
# http://openstudio.nrel.gov/openstudio-life-cycle-examples

#see the URL below for access to C++ documentation on model objects (click on "model" in the main window to view model objects)
# http://openstudio.nrel.gov/sites/openstudio.nrel.gov/files/nv_data/cpp_documentation_it/model/html/namespaces.html

#start the measure
class IntersectSpaceGeometry < OpenStudio::Ruleset::ModelUserScript
  
  #define the name that a user will see, this method may be deprecated as
  #the display name in PAT comes from the name field in measure.xml
  def name
    return "Intersect Space Geometry"
  end
  
  #define the arguments that the user will input
  def arguments(model)
    args = OpenStudio::Ruleset::OSArgumentVector.new
    
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

    #reporting initial condition of model
    starting_surfaces = model.getSurfaces
    runner.registerInitialCondition("The building started with #{starting_surfaces.size} base surfaces.")

    if starting_surfaces.size == 0
      runner.registerAsNotApplicable("The building does not have any base surfaces to intersect.")
    end

    spaces = model.getSpaces
    n = spaces.size

    boundingBoxes = []
    (0...n).each do |i|
      boundingBoxes[i] = spaces[i].transformation * spaces[i].boundingBox
    end

    (0...n).each do |i|
      (i+1...n).each do |j|
        next if not boundingBoxes[i].intersects(boundingBoxes[j])
        spaces[i].intersectSurfaces(spaces[j])
        spaces[i].matchSurfaces(spaces[j])
      end #j
    end #i

    #reporting final condition of model
    finishing_surfaces = model.getSurfaces
    runner.registerFinalCondition("The building finished with #{finishing_surfaces.size} base surfaces.")

    if starting_surfaces.size == finishing_surfaces.size
      runner.registerAsNotApplicable("No surface intersections were found.")
    end

    return true
 
  end #end the run method

end #end the measure

#this allows the measure to be use by the application
IntersectSpaceGeometry.new.registerWithApplication