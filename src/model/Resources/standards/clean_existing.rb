require 'json'

# Keys that we actually need for C++ code
# cf openstudiocore/model/SpaceType.cpp and StandardsInformationMaterial.cpp
$json_info = {"space_types" => ["template", "space_type", "building_type"],
              "materials" => ["name",
                              "material_standard",
                              "code_category",
                              "cavity_insulation",
                              "material_standard_source",
                              "code_identifier",
                              "framing_material",
                              "framing_configuration",
                              "framing_depth",
                              "framing_size",
                             ]}

def sort_and_remove_unused_keys(cleaned_data, json_name)
  if json_name == 'space_types'
    cleaned_data[json_name] = cleaned_data[json_name].sort_by{|x| [x['template'],x['building_type'], x['space_type']]}
  elsif json_name == 'materials'
    cleaned_data[json_name] = cleaned_data[json_name].sort_by{|x| x['name']}
  else
    raise "Unknown json_name=#{json_name}"
  end

  cleaned_data[json_name].each { |e| e.delete_if{|k| !$json_info[json_name].include?(k)} }
end


["space_types", "materials"].each do |json_name|
  save_path = "OpenStudio_Standards_#{json_name}_merged.json"
  cleaned_data = JSON.parse(File.read(save_path))
  sort_and_remove_unused_keys(cleaned_data, json_name)

  File.open(save_path,"w") do |f|
    f.write(JSON.pretty_generate(cleaned_data))
  end
end
