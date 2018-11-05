# NCEB has been creating their own JSON files that live in another place
# and are more up to date (including the 2015 that is brand new)
# And they didn't (yet) remove their data from the regular (default)
# standards data
# This script aims to properly collate these two data sources
# Author: Julien Marrec, EffiBEM
# Date: 2018-08-23

require 'json'

default_dir = "/home/julien/Software/Others/openstudio-standards/data/standards/"

# Keys that we actually need for C++ code
# cf openstudiocore/model/SpaceType.cpp and StandardsInformationMaterial.cpp
$json_info = {"space_types" => ["space_type", "building_type"],
              "materials" => ["material_standard",
                              "code_category",
                              "cavity_insulation",
                              "material_standard_source",
                              "code_identifier",
                              "framing_material",
                              "framing_configuration",
                              "framing_depth",
                              "framing_size",
                             ]}
# json_names = json_info.keys

def verify_proper_keys_present(json_keys, json_name)
  missing = $json_info[json_name].reject{|x| json_keys.include?(x)}
  if missing.size > 0
    raise "For #{json_name}, Some keys that are used in the C++ code are NO Longer present in the JSON, action is required!"
  end

end

###############################################################################
#                          S P A C E     T Y P E S
###############################################################################

json_name = "space_types"
puts "\n\n\n"
puts "###############################################################################"
puts "#                          S P A C E     T Y P E S                             "
puts "###############################################################################"

json_path = File.join(default_dir, "OpenStudio_Standards_#{json_name}.json")
file = File.read(json_path);
ori_data = JSON.parse(file);
n_ini = ori_data[json_name].size

if json_name == "space_types"
  # Some keys in the ori have '\n' in them whereas they don't in NECB
  # eg: lpd_fraction\nlinear_fluorescent
  cleaned_data = {}
  cleaned_data[json_name] = []
  ori_data[json_name].each_with_index do |e, i|
    cleaned_data[json_name] << {}
    e.each_key do |k|
      if k.include?("\n")
        cleaned_data[json_name][i][k.gsub("\n", "_")] = ori_data[json_name][i][k]
      else
        cleaned_data[json_name][i][k] = ori_data[json_name][i][k]
      end
    end
  end
else
  cleaned_data = ori_data
end;
ori_keys = cleaned_data[json_name].map{|e| e.keys}.flatten.uniq;
verify_proper_keys_present(ori_keys, json_name)

# Exclude NCEB entries
cleaned_data[json_name] = cleaned_data[json_name].select{|e| !e["template"].downcase.include?("necb")};
n_after_remove = cleaned_data[json_name].size
puts "Removed #{n_ini - n_after_remove} NECB entries"


necb_years = [2011, 2015]

necb_years.each do |necb_year|

  puts "\n\nProcessing NECB #{necb_year}"
  puts "=====================================\n"
  necb_path = "/home/julien/Software/Others/openstudio-standards/lib/openstudio-standards/standards/necb/necb_#{necb_year}/data/#{json_name}.json"
  file = File.read(necb_path);
  necb_data = JSON.parse(file);
  tables = necb_data["tables"].select{|t| t["name"] == json_name};
  if tables.size != 1
    raise "Cannot locate right table"
  end
  n_inter = cleaned_data[json_name].size
  necb_data = tables[0]["table"];
  cleaned_data[json_name] += necb_data

  puts "Added #{cleaned_data[json_name].size - n_inter} NECB entries"

  new_keys = necb_data.map{|e| e.keys}.flatten.uniq;
  verify_proper_keys_present(new_keys, json_name)

  added_keys = new_keys.reject{|x| ori_keys.include?(x)};
  if (added_keys.size > 0)
    puts "Keys that were ADDED in NCEB: #{added_keys}"
  end
  removed_keys = ori_keys.reject{|x| new_keys.include?(x)};
  if (removed_keys.size > 0)
    puts "Keys that were REMOVED in NCEB: #{removed_keys}"
  end

end;

n_final = cleaned_data[json_name].size;
puts "\n\n#{json_name}: Started with #{n_ini} keys, ended with #{n_final} (diff=#{n_final-n_ini})"
save_path = "OpenStudio_Standards_#{json_name}_merged.json"
File.open(save_path,"w") do |f|
  f.write(JSON.pretty_generate(cleaned_data))
end
puts "\nWritten to #{save_path}"


###############################################################################
#                          M A T E R I A L S
###############################################################################

puts "\n\n\n"
puts "###############################################################################"
puts "#                          M A T E R I A L S                                   "
puts "###############################################################################"

json_name = "materials"

json_path = File.join(default_dir, "OpenStudio_Standards_#{json_name}.json")
file = File.read(json_path);
ori_data = JSON.parse(file);
n_ini = ori_data[json_name].size
cleaned_data = ori_data;
ori_keys = cleaned_data[json_name].map{|e| e.keys}.flatten.uniq;
verify_proper_keys_present(ori_keys, json_name)
ori_data_names = ori_data[json_name].map{|e| e["name"]}.uniq;

# THERE IS NO MATERIALS.JSON in 2015
necb_years = [2011]

necb_years.each do |necb_year|

  puts "\n\nProcessing NECB #{necb_year}"
  puts "=====================================\n"
  necb_path = "/home/julien/Software/Others/openstudio-standards/lib/openstudio-standards/standards/necb/necb_#{necb_year}/data/#{json_name}.json"
  file = File.read(necb_path);
  necb_data = JSON.parse(file);
  tables = necb_data["tables"].select{|t| t["name"] == json_name};
  if tables.size != 1
    raise "Cannot locate right table"
  end
  n_inter = cleaned_data[json_name].size
  necb_data = tables[0]["table"];

  # Report any added/removed keys
  new_keys = necb_data.map{|e| e.keys}.flatten.uniq;
  verify_proper_keys_present(new_keys, json_name)

  added_keys = new_keys.reject{|x| ori_keys.include?(x)};
  if (added_keys.size > 0)
    puts "Keys that were ADDED in NCEB: #{added_keys}"
  end
  removed_keys = ori_keys.reject{|x| new_keys.include?(x)};
  if (removed_keys.size > 0)
    puts "Keys that were REMOVED in NCEB: #{removed_keys}"
  end


  # Report any materials (based on "name") that have been added/removed
  new_data_names = necb_data.map{|e| e["name"]}.uniq;
  added_data_names = new_data_names.reject{|x| ori_data_names.include?(x)};
  if (added_data_names.size > 0)
    puts "\n#{added_data_names.size} Names that were ADDED in NCEB: #{added_data_names}"
  end
  removed_data_names = ori_data_names.reject{|x| new_data_names.include?(x)};
  if (removed_data_names.size > 0)
    puts "\n#{removed_data_names.size} Names that were REMOVED in NCEB: #{removed_data_names}"
  end


  # We add the new entries from NCEB, in case where the material is in both
  # array we privilegiate the default one (not NCEB)
  # cleaned_data[json_name] = (cleaned_data[json_name] + necb_data).uniq{|h| h["name"]};
  cleaned_data[json_name] = (cleaned_data[json_name] + necb_data).uniq{|h| h.values_at("name", "code_category")};

  puts "Added #{cleaned_data[json_name].size - n_inter} NECB entries"

end;

n_final = cleaned_data[json_name].size;
puts "\n\n#{json_name}: Started with #{n_ini} keys, ended with #{n_final} (diff=#{n_final-n_ini})"
save_path = "OpenStudio_Standards_#{json_name}_merged.json"
File.open(save_path,"w") do |f|
  f.write(JSON.pretty_generate(cleaned_data))
end
puts "\nWritten to #{save_path}"
