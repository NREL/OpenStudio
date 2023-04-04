# NCEB has been creating their own JSON files that live in another place
# and are more up to date (including the 2015 that is brand new)
# And they didn't (yet) remove their data from the regular (default)
# standards data
# This script aims to properly collate these two data sources
# Author: Julien Marrec, EffiBEM
# Date: 2018-08-23
# Modified: 2023-04-04:
#   * All standards data are in individual files now, not just NECB

require 'json'
require 'pathname'

verbose = false
$clean_unused_keys = true

default_dir = Pathname.new("/home/julien/Software/Others/openstudio_gems/openstudio-standards/lib/openstudio-standards/standards/")

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
# json_names = json_info.keys

def verify_proper_keys_present(json_keys, json_name)
  missing = $json_info[json_name].reject{|x| json_keys.include?(x)}
  if missing.size > 0
    raise "For #{json_name}, Some keys that are used in the C++ code are NO Longer present in the JSON, action is required!"
  end
end

def sort_and_remove_unused_keys(cleaned_data, json_name, clean_unused_keys)
  if json_name == 'space_types'
    cleaned_data[json_name] = cleaned_data[json_name].sort_by{|x| [x['template'],x['building_type'], x['space_type']]}
  elsif json_name == 'materials'
    cleaned_data[json_name] = cleaned_data[json_name].sort_by{|x| x['name']}
  else
    raise "Unknown json_name=#{json_name}"
  end

  if clean_unused_keys
    puts "Cleaning the unused keys for reduced size"
    cleaned_data[json_name].each { |e| e.delete_if{|k| !$json_info[json_name].include?(k)} }
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

files = default_dir.glob(File.join('**', '*spc_typ.json')).reject{|p| p.to_s.include?('comstock') || p.to_s.include?('nrel_zne') || p.to_s.include?('aedg') }.sort;
raise "Can't find the spc_typ.json" if files.empty?
cleaned_data = nil

files.each_with_index do |json_path, i|
  puts "===================================================================="
  puts "  P R O C E S S I N G   #{json_path.basename}"
  file = File.read(json_path);
  data = JSON.parse(file);

  # Some keys in the ori used to have '\n' in them whereas they don't in NECB
  # eg: lpd_fraction\nlinear_fluorescent
  data[json_name].each_with_index do |e, i|
    e.each_key do |k|
      if k.include?("\n")
        raise k
        data[json_name][i][k.gsub("\n", "_")] = data[json_name][i][k]
      end
    end
  end

  keys = data[json_name].map{|e| e.keys}.flatten.uniq;
  verify_proper_keys_present(keys, json_name)
  if i == 0
    cleaned_data = data;
    puts "Added #{cleaned_data[json_name].size} entries from #{json_path.basename}"
    next
  end

  n_ini = cleaned_data[json_name].size
  ori_keys = cleaned_data[json_name].map{|e| e.keys}.flatten.uniq;
  ori_data_names = cleaned_data[json_name].map{|e| e["name"]}.uniq;

  # Report any added/removed keys
  added_keys = keys.reject{|x| ori_keys.include?(x)};
  if (added_keys.size > 0)
    puts "Keys that were ADDED in #{json_path.basename}: #{added_keys}"
  end
  removed_keys = ori_keys.reject{|x| keys.include?(x)};
  if (removed_keys.size > 0)
    puts "Keys that were REMOVED in #{json_path.basename}: #{removed_keys}"
  end
  # Report any materials (based on "name") that have been added/removed
  new_data_names = data[json_name].map{|e| e["name"]}.uniq;
  added_data_names = new_data_names.reject{|x| ori_data_names.include?(x)};
  if (added_data_names.size > 0)
    puts "\n#{added_data_names.size} Names that were ADDED in #{json_path.basename}"
    if verbose
      puts added_data_names
    end
  end
  removed_data_names = ori_data_names.reject{|x| new_data_names.include?(x)};
  if (removed_data_names.size > 0)
    puts "\n#{removed_data_names.size} Names that were REMOVED in #{json_path.basename}"
    if verbose
      puts removed_data_names
    end
  end


  cleaned_data[json_name] += data[json_name]

  puts "Added #{cleaned_data[json_name].size - n_ini} entries from #{json_path.basename}"
end

# Exclude NCEB entries
n_ini = cleaned_data[json_name].size
cleaned_data[json_name] = cleaned_data[json_name].select{|e| !e["template"].downcase.include?("necb")};
n_after_remove = cleaned_data[json_name].size
puts "Removed #{n_ini - n_after_remove} NECB entries"

puts "\nBefore NECB, #{cleaned_data[json_name].size} Space Types\n"

necb_years = [2011, 2015, 2017, 2020]

necb_years.each do |necb_year|

  puts "\n\nProcessing NECB #{necb_year}"
  puts "=====================================\n"
  template_name = "NECB#{necb_year}"
  json_path = default_dir.join('necb', template_name, 'data', "#{json_name}.json")
  raise "File #{json_path} does not exist" if !json_path.exist?
  puts json_path
  file = File.read(json_path);
  necb_data = JSON.parse(file);

  tables = necb_data["tables"]
  if tables.nil?
    raise "Cannot locate right 'tables' key"
  end
  entry = tables[json_name]
  if entry.nil?
    raise "Cannot locate tables > #{json_name}"
  end
  necb_data = entry['table']

  # Append template where missing (which is everywhere)
  count_templates = necb_data.select{|e| e.include?('template')}.size
  puts "#{template_name} has #{count_templates} entries with 'template' key out of #{necb_data.size}"

  necb_data.each do |e|
    if !e.include?('template')
      e["template"] = template_name
    end
  end

  # Report any added/removed keys
  keys = necb_data.map{|e| e.keys}.flatten.uniq;
  verify_proper_keys_present(keys, json_name)

  n_ini = cleaned_data[json_name].size
  ori_keys = cleaned_data[json_name].map{|e| e.keys}.flatten.uniq;
  ori_data_names = cleaned_data[json_name].map{|e| e["name"]}.uniq;

  # Report any added/removed keys
  added_keys = keys.reject{|x| ori_keys.include?(x)};
  if (added_keys.size > 0)
    puts "Keys that were ADDED in #{json_path.basename}: #{added_keys}"
  end
  removed_keys = ori_keys.reject{|x| keys.include?(x)};
  if (removed_keys.size > 0)
    puts "Keys that were REMOVED in #{json_path.basename}: #{removed_keys}"
  end
  # Report any materials (based on "name") that have been added/removed
  new_data_names = necb_data.map{|e| e["name"]}.uniq;
  added_data_names = new_data_names.reject{|x| ori_data_names.include?(x)};
  if (added_data_names.size > 0)
    puts "\n#{added_data_names.size} Names that were ADDED in #{json_path.basename}"
    if verbose
      puts added_data_names
    end
  end
  removed_data_names = ori_data_names.reject{|x| new_data_names.include?(x)};
  if (removed_data_names.size > 0)
    puts "\n#{removed_data_names.size} Names that were REMOVED in #{json_path.basename}"
    if verbose
      puts removed_data_names
    end
  end

  cleaned_data[json_name] += necb_data

  puts "Added #{cleaned_data[json_name].size - n_ini} entries from #{json_path.basename}"

end;

puts "\nFinal: #{cleaned_data[json_name].size} Space Types\n"

# Sort and remove unused keys
sort_and_remove_unused_keys(cleaned_data, json_name, $clean_unused_keys)

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

files = default_dir.glob(File.join('**', '*materials.json')).reject{|p| p.to_s.include?('_prm') || p.to_s.include?('NECB')}.sort
raise "Can't find the materials.json" if files.empty?
cleaned_data = nil

files.each_with_index do |json_path, i|
  puts "===================================================================="
  puts "  P R O C E S S I N G   #{json_path.basename}"
  file = File.read(json_path);
  data = JSON.parse(file);
  keys = data[json_name].map{|e| e.keys}.flatten.uniq;
  verify_proper_keys_present(keys, json_name)
  if i == 0
    cleaned_data = data;
    puts "Added #{cleaned_data[json_name].size} entries from #{json_path.basename}"
    next
  end

  n_ini = cleaned_data[json_name].size
  ori_keys = cleaned_data[json_name].map{|e| e.keys}.flatten.uniq;
  ori_data_names = cleaned_data[json_name].map{|e| e["name"]}.uniq;

  # Report any added/removed keys
  added_keys = keys.reject{|x| ori_keys.include?(x)};
  if (added_keys.size > 0)
    puts "Keys that were ADDED in #{json_path.basename}: #{added_keys}"
  end
  removed_keys = ori_keys.reject{|x| keys.include?(x)};
  if (removed_keys.size > 0)
    puts "Keys that were REMOVED in #{json_path.basename}: #{removed_keys}"
  end
  # Report any materials (based on "name") that have been added/removed
  new_data_names = data[json_name].map{|e| e["name"]}.uniq;
  added_data_names = new_data_names.reject{|x| ori_data_names.include?(x)};
  if (added_data_names.size > 0)
    puts "\n#{added_data_names.size} Names that were ADDED in #{json_path.basename}"
    if verbose
      puts added_data_names
    end
  end
  removed_data_names = ori_data_names.reject{|x| new_data_names.include?(x)};
  if (removed_data_names.size > 0)
    puts "\n#{removed_data_names.size} Names that were REMOVED in #{json_path.basename}"
    if verbose
      puts removed_data_names
    end
  end


  # We add the new entries from NCEB, in case where the material is in both
  # array we privilegiate the default one (not NCEB)
  # cleaned_data[json_name] = (cleaned_data[json_name] + necb_data).uniq{|h| h["name"]};
  cleaned_data[json_name] = (cleaned_data[json_name] + data[json_name]).uniq{|h| h.values_at("name", "code_category")};

  puts "Added #{cleaned_data[json_name].size - n_ini} entries from #{json_path.basename}"
end

puts "\nBefore NECB, #{cleaned_data[json_name].size} Materials\n"

# THERE IS NO MATERIALS.JSON in other years than 2011
necb_years = [2011]

necb_years.each do |necb_year|

  puts "\n\nProcessing NECB #{necb_year}"
  puts "=====================================\n"
  json_path = default_dir.join('necb', "NECB#{necb_year}", 'data', "#{json_name}.json")
  raise "File #{json_path} does not exist" if !json_path.exist?
  puts json_path
  file = File.read(json_path);
  necb_data = JSON.parse(file);

  tables = necb_data["tables"]
  if tables.nil?
    raise "Cannot locate right 'tables' key"
  end
  entry = tables[json_name]
  if entry.nil?
    raise "Cannot locate tables > #{json_name}"
  end
  necb_data = entry['table']

  n_ini = cleaned_data[json_name].size
  ori_keys = cleaned_data[json_name].map{|e| e.keys}.flatten.uniq;
  ori_data_names = cleaned_data[json_name].map{|e| e["name"]}.uniq;

  # Report any added/removed keys
  keys = necb_data.map{|e| e.keys}.flatten.uniq;
  verify_proper_keys_present(keys, json_name)

  added_keys = keys.reject{|x| ori_keys.include?(x)};
  if (added_keys.size > 0)
    puts "Keys that were ADDED in NCEB: #{added_keys}"
  end
  removed_keys = ori_keys.reject{|x| keys.include?(x)};
  if (removed_keys.size > 0)
    puts "Keys that were REMOVED in NCEB: #{removed_keys}"
  end

  # Report any materials (based on "name") that have been added/removed
  new_data_names = necb_data.map{|e| e["name"]}.uniq;
  added_data_names = new_data_names.reject{|x| ori_data_names.include?(x)};
  if (added_data_names.size > 0)
    puts "\n#{added_data_names.size} Names that were ADDED in NCEB"
    if verbose
      puts added_data_names
    end
  end
  removed_data_names = ori_data_names.reject{|x| new_data_names.include?(x)};
  if (removed_data_names.size > 0)
    puts "\n#{removed_data_names.size} Names that were REMOVED in NCEB"
    if verbose
      puts removed_data_names
    end
  end


  # We add the new entries from NCEB, in case where the material is in both
  # array we privilegiate the default one (not NCEB)
  # cleaned_data[json_name] = (cleaned_data[json_name] + necb_data).uniq{|h| h["name"]};
  cleaned_data[json_name] = (cleaned_data[json_name] + necb_data).uniq{|h| h.values_at("name", "code_category")};

  puts "Added #{cleaned_data[json_name].size - n_ini} NECB entries"

end;

puts "\nFinal: #{cleaned_data[json_name].size} Materials\n"
save_path = "OpenStudio_Standards_#{json_name}_merged.json"

# Sort
sort_and_remove_unused_keys(cleaned_data, json_name, $clean_unused_keys)

File.open(save_path,"w") do |f|
  f.write(JSON.pretty_generate(cleaned_data))
end
puts "\nWritten to #{save_path}"
