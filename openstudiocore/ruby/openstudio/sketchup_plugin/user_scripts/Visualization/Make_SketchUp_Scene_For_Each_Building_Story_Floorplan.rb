########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
#  following conditions are met:
#
#  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
#  disclaimer.
#
#  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
#  following disclaimer in the documentation and/or other materials provided with the distribution.
#
#  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
#  products derived from this software without specific prior written permission from the respective party.
#
#  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
#  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
#  specific prior written permission from Alliance for Sustainable Energy, LLC.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
#  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
#  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
########################################################################################################################

# Each user script is implemented within a class that derives from OpenStudio::Ruleset::UserScript
class MakeScenesFromStories < OpenStudio::Ruleset::ModelUserScript

  # override name to return the name of your script
  def name
    return "Make SketchUp Scene For Each Building Story Floorplan"
  end
  
  # returns a vector of arguments, the runner will present these arguments to the user
  # then pass in the results on run
  def arguments(model)
    result = OpenStudio::Ruleset::OSArgumentVector.new
    return result
  end
  
  def makeSceneFromStorySpaces(skp_model, story, all_spaces)
    
    pages = skp_model.pages
    story_name = story.name.get
    
    # hide all spaces
    all_spaces.each do |space|
      if drawing_interface = space.drawing_interface
        if entity = drawing_interface.entity
          entity.visible = false 
        end
      end
    end

    # unhide spaces in story
    story.spaces.each do |space|
      if drawing_interface = space.drawing_interface
        if entity = drawing_interface.entity
          entity.visible = true 
        end
      end
    end

    # delete existing page
    pages.each do |page|
      if page.name.to_s == "OS - #{story_name}"
        skp_model.pages.erase(page)
        break
      end
    end
    
    # create scene saving current visibility state
    story_page = pages.add("OS - #{story_name}")
    story_page.use_camera = false
    story_page.use_hidden= true
    story_page.transition_time = 0
    story_page.delay_time = 0
    story_page.use_shadow_info=false
    #pages.selected_page = story_page # not updated correctly if extra stories after this
  end
  
  def makeSceneFromSpacesWithNoStory(skp_model, all_spaces)
    pages = skp_model.pages
    story_name = "<no story>"
    
    # hide all spaces
    all_spaces.each do |space|
      if drawing_interface = space.drawing_interface
        if entity = drawing_interface.entity
          entity.visible = false 
        end
      end
    end

    # unhide spaces with no story
    all_spaces.each do |space|
      if space.buildingStory.empty?
        if drawing_interface = space.drawing_interface
          if entity = drawing_interface.entity
            entity.visible = true 
          end
        end
      end
    end

    # delete existing page
    pages.each do |page|
      if page.name.to_s == "OS - #{story_name}"
        skp_model.pages.erase(page)
        break
      end
    end
    
    # create scene saving current visibility state
    story_page = pages.add("OS - #{story_name}")
    story_page.use_camera = false
    story_page.use_hidden= true
    story_page.transition_time = 0
    story_page.delay_time = 0
    story_page.use_shadow_info=false
    #pages.selected_page = story_page # not updated correctly if extra stories after this
  end
    
  # override run to implement the functionality of your script
  # model is an OpenStudio::Model::Model, runner is a OpenStudio::Ruleset::UserScriptRunner
  def run(model, runner, user_arguments)   
    super(model, runner, user_arguments)
    
    # this is not a portable script as it uses the SketchUp API

    # unhide all spaces
    spaces = model.getSpaces
    spaces.each do |space|
      if drawing_interface = space.drawing_interface
        if entity = drawing_interface.entity
          entity.visible = true 
        end
      end
    end
    
    # get the active SketchUp model
    skp_model = Sketchup.active_model
    pages = skp_model.pages
    
    # page with all stories
    all_stories_page = nil
    pages.each do |page| 
      if page.name.to_s == "OS - All Stories" 
        
        # update hidden geometry
        page.update(16)
        
        all_stories_page = page
        break
      end
    end
  
    if not all_stories_page
      # create scene saving current visibility state
      all_stories_page = pages.add("OS - All Stories")
      all_stories_page.use_camera = false
      all_stories_page.use_hidden= true
      all_stories_page.transition_time = 0
      all_stories_page.delay_time = 0
      all_stories_page.use_shadow_info = false
    end

    # create scene saving current camera state
    working_camera_page = nil
    pages.each do |page| 
      if page.name.to_s == "OS - Working Camera"  
        working_camera_page = page
        break
      end
    end

    if not working_camera_page
      # create scene saving current visibility state
      working_camera_page = pages.add("OS - Working Camera")
      working_camera_page.use_camera = true
      working_camera_page.use_hidden= false
      working_camera_page.transition_time = 0
      working_camera_page.delay_time = 0
      working_camera_page.use_shadow_info = false
    end

    # set camera to plan and zoom extents before making next scene
    camera = Sketchup::Camera.new
    eye = camera.eye
    target = [0,0,-1]
    up = [0,1,0]
    ortho = camera.perspective = false
    skp_model.active_view.camera.set(eye, target, up, ortho)
    skp_model.active_view.zoom_extents

    # set camera to plan and zoom extents before make next scene
    plan_camera_page = nil
    pages.each do |page| 
      if page.name.to_s == "OS - Plan Camera" 
        
        # update Camera Location
        page.update(1)
        
        plan_camera_page = page 
        break
      end
    end
    
    if not plan_camera_page
      # create scene saving plan zoom extents
      plan_camera_page = pages.add("OS - Plan Camera")
      plan_camera_page.use_camera = true
      plan_camera_page.use_hidden= false
      plan_camera_page.transition_time = 0
      plan_camera_page.delay_time = 0
      plan_camera_page.use_shadow_info=false
    end
    
    # get all the stories sorted by z height
    stories = model.getBuildingStorys.sort do |x, y|
      xz = x.nominalZCoordinate
      yz = y.nominalZCoordinate
      if xz.empty? and yz.empty?
        0
      elsif xz.empty?
        1
      elsif yz.empty?
        -1
      else
        xz.get <=> yz.get
      end
    end

    # make scene for spaces with no story
    makeSceneFromSpacesWithNoStory(skp_model, spaces)

    # make scene for each story
    stories.each do |story|
      makeSceneFromStorySpaces(skp_model, story, spaces)
    end
    
    return true
  end

end

# this call registers your script with the OpenStudio SketchUp plug-in
MakeScenesFromStories.new.registerWithApplication
