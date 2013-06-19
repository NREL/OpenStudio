# Opens in.osm, sorts it, and saves it as out.osm

require 'openstudio'

def save_sorted_osm(osm_model, save_filename)
  # Iterate over all the osm objects and put into a ruby array.
  # Note that you must strip the object name because for some reason it has
  # trailing characters
  sorted_osm = {}
  osm_model.objects.each do |object|
    objname = object.name()
    objname = objname.to_s.strip
    if objname == ""
      # puts "[DEBUG] ObjectName is Blank, using first field"
      objname = object.getString(0)
    end
    puts "Class Name: #{object.iddObject.name()}    Object Name: #{objname}      Size: #{object.numFields()}"

    # Clean up the comment field. They comments flags change on translation!
    obj = object.to_s.gsub("!-", "!")
    sorted_osm["#{object.iddObject.name()} #{objname}"] = obj
  end
  
  out = sorted_osm.sort #returns a nested array, 0 is key, 1 is value
  File.open(save_filename, 'w') do |file|
    out.each do |value|
      file << value[1]
    end
  end
end

model = OpenStudio::Model::Model::load(OpenStudio::Path.new("in.osm")).get
save_sorted_osm(model, "out.osm")

