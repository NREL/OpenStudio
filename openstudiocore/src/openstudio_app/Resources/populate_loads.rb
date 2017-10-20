require 'openstudio'
# Helper to load a model in one line

def osload(path)
  translator = OpenStudio::OSVersion::VersionTranslator.new
  ospath = OpenStudio::Path.new(path)
  model = translator.loadModel(ospath)
  if model.empty?
      raise "Path '#{path}' is not a valid path to an OpenStudio Model"
  else
      model = model.get
  end
  return model
end

files = Dir.glob("**/*.osm")

files.each do |f|
  m = osload(f)

  puts "\nProcessing #{f}"

  # ElectricEquipment
  n_elec = 0
  m.getElectricEquipments.each do |e|
    e.setEndUseSubcategory("General")
    n_elec += 1
  end

  # ExteriorLights
  n_extlights = 0
  m.getExteriorLightss.each do |l|
    l.setEndUseSubcategory("General")
    n_extlights += 1
  end

  # GasEquipment
  n_gas = 0
  m.getGasEquipments.each do |g|
    g.setEndUseSubcategory("General")
    n_gas += 1
  end

  # HotWaterEquipment
  n_hw = 0
  m.getHotWaterEquipments.each do |h|
    h.setEndUseSubcategory("General")
    n_hw += 1
  end

  # Lights
  n_lights = 0
  m.getLightss.each do |l|
    l.setEndUseSubcategory("General")
    n_lights += 1
  end

  # Luminaire
  n_lum = 0
  m.getLuminaires.each do |l|
    l.setEndUseSubcategory("General")
    n_lun += 1
  end

  # People
  n_people = 0
  m.getPeoples.each do |p|
    p.setMultiplier(1.0)
    n_people += 1
  end

  # SteamEquipment
  n_steam = 0
  m.getSteamEquipments.each do |s|
    s.setEndUseSubcategory("General")
    n_steam +=1
  end

  puts "ElectricEquipment (#{n_elec}), ExteriorLights (#{n_extlights}), " +
       "GasEquipment (#{n_gas}), HotWaterEquipment (#{n_hw}), " +
       "Lights (#{n_lights}), Luminaire (#{n_lum}), " +
       "People (#{n_people}), SteamEquipment (#{n_steam})"

  m.save(f, true)
end
