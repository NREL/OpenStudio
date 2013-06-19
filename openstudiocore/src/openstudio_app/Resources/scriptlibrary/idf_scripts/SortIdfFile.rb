# Opens in.idf, cleans it, sorts and saves it as out.idf.

require 'openstudio'

def save_sorted_idf(idf_model, save_filename)
  # Iterate over all the IDF objects and put into a ruby array.
  # Note that you must strip the object name because for some reason it has
  # trailing characters
  sorted_idf = {}
  verobj = idf_model.versionObject
  if not verobj.empty?
    verobj = verobj.get
    objname = verobj.getString(0).get
    obj = verobj.to_s.gsub("!-", "!")
    sorted_idf["#{verobj.iddObject.name()} #{objname}"] = obj 
  end
  idf_model.objects.each do |object|
    if object.iddObject.type != "CommentOnly".to_IddObjectType
      objname = object.name()
      objname = objname.to_s.strip
      if objname == ""
        # puts "[DEBUG] ObjectName is Blank, using first field"
        objname = object.getString(0).get
      end
      # puts "Class Name: #{object.iddObject.name()}    Object Name: #{objname}      Size: #{object.numFields()}"
      
      # Clean up the comment field. They comments flags change on translation!
      obj = object.to_s.gsub("!-", "!")
      sorted_idf["#{object.iddObject.name()} #{objname}"] = obj 
    end
  end
  
  out = sorted_idf.sort #returns a nested array, 0 is key, 1 is value
  File.open(save_filename, 'w') do |file|
    out.each do |value|
      file << value[1]
    end
  end
end

idf = OpenStudio::IdfFile::load(OpenStudio::Path.new("in.idf"), "EnergyPlus".to_IddFileType).get
  
new_filepath = "in_cleaned.idf"  
 
# save idf back out which is in the same order, but cleaned up with updated
# field tags and spacing
puts "Saving cleaned version of IDF to #{new_filepath}"
File.open(new_filepath, 'w') do |file|
  file << idf.to_s
end
  
# Save the sorted version
new_filepath = "out.idf"
save_sorted_idf(idf, new_filepath)

