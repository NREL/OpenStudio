require 'json'

module OsLib_Reporting
  # setup - get model, sql, and setup web assets path
  def self.setup(runner)
    results = {}

    # get the last model
    model = runner.lastOpenStudioModel
    if model.empty?
      runner.registerError('Cannot find last model.')
      return false
    end
    model = model.get

    # get the last idf
    workspace = runner.lastEnergyPlusWorkspace
    if workspace.empty?
      runner.registerError('Cannot find last idf file.')
      return false
    end
    workspace = workspace.get

    # get the last sql file
    sqlFile = runner.lastEnergyPlusSqlFile
    if sqlFile.empty?
      runner.registerError('Cannot find last sql file.')
      return false
    end
    sqlFile = sqlFile.get
    model.setSqlFile(sqlFile)

    # populate hash to pass to measure
    results[:model] = model
    # results[:workspace] = workspace
    results[:sqlFile] = sqlFile
    results[:web_asset_path] = OpenStudio.getSharedResourcesPath / OpenStudio::Path.new('web_assets')

    return results
  end

  def self.ann_env_pd(sqlFile)
    # get the weather file run period (as opposed to design day run period)
    ann_env_pd = nil
    sqlFile.availableEnvPeriods.each do |env_pd|
      env_type = sqlFile.environmentType(env_pd)
      if env_type.is_initialized
        if env_type.get == OpenStudio::EnvironmentType.new('WeatherRunPeriod')
          ann_env_pd = env_pd
        end
      end
    end

    return ann_env_pd
  end

  def self.create_xls
    require 'rubyXL'
    book = ::RubyXL::Workbook.new

    # delete initial worksheet

    return book
  end

  def self.save_xls(book)
    file = book.write 'excel-file.xlsx'

    return file
  end

  # write an Excel file from table data
  # the Excel Functions are not currently being used, left in as example
  # Requires ruby Gem which isn't currently supported in OpenStudio GUIs.
  # Current setup is simple, creates new workbook for each table
  # Could be updated to have one section per workbook
  def self.write_xls(table_data, book)
    worksheet = book.add_worksheet table_data[:title]

    row_cnt = 0
    # write the header row
    header = table_data[:header]
    header.each_with_index do |h, i|
      worksheet.add_cell(row_cnt, i, h)
    end
    worksheet.change_row_fill(row_cnt, '0ba53d')

    # loop over data rows
    data = table_data[:data]
    data.each do |d|
      row_cnt += 1
      d.each_with_index do |c, i|
        worksheet.add_cell(row_cnt, i, c)
      end
    end

    return book
  end

  # cleanup - prep html and close sql
  def self.cleanup(html_in_path)
    # TODO: - would like to move code here, but couldn't get it working. May look at it again later on.

    return html_out_path
  end

  # create template section
  def self.template_section(model, sqlFile, runner, name_only = false)
    # array to hold tables
    template_tables = []

    # gather data for section
    @template_section = {}
    @template_section[:title] = 'Tasty Treats'
    @template_section[:tables] = template_tables

    # stop here if only name is requested this is used to populate display name for arguments
    if name_only == true
      return @template_section
    end

    # notes:
    # The data below would typically come from the model or simulation results
    # You can loop through objects to make a table for each item of that type, such as air loops
    # If a section will only have one table you can leave the table title blank and just rely on the section title
    # these will be updated later to support graphs

    # create table
    template_table_01 = {}
    template_table_01[:title] = 'Fruit'
    template_table_01[:header] = %w(Definition Value)
    template_table_01[:units] = ['', '$/pound']
    template_table_01[:data] = []

    # add rows to table
    template_table_01[:data] << ['Banana', 0.23]
    template_table_01[:data] << ['Apple', 0.75]
    template_table_01[:data] << ['Orange', 0.50]

    # add table to array of tables
    template_tables << template_table_01

    # using helper method that generates table for second example
    template_tables << OsLib_Reporting.template_table(model, sqlFile, runner)

    return @template_section
  end

  # create template section
  def self.template_table(model, sqlFile, runner)
    # create a second table
    template_table = {}
    template_table[:title] = 'Ice Cream'
    template_table[:header] = ['Definition', 'Base Flavor', 'Toppings', 'Value']
    template_table[:units] = ['', '', '', 'scoop']
    template_table[:data] = []

    # add rows to table
    template_table[:data] << ['Vanilla', 'Vanilla', 'NA', 1.5]
    template_table[:data] << ['Rocky Road', 'Chocolate', 'Nuts', 1.5]
    template_table[:data] << ['Mint Chip', 'Mint', 'Chocolate Chips', 1.5]

    return template_table
  end

  # building_summary section
  def self.building_summary_section(model, sqlFile, runner, name_only = false)
    # array to hold tables
    general_tables = []

    # gather data for section
    @building_summary_section = {}
    @building_summary_section[:title] = 'Model Summary'
    @building_summary_section[:tables] = general_tables

    # stop here if only name is requested this is used to populate display name for arguments
    if name_only == true
      return @building_summary_section
    end

    # add in general information from method
    general_tables << OsLib_Reporting.general_building_information_table(model, sqlFile, runner)
    general_tables << OsLib_Reporting.weather_summary_table(model, sqlFile, runner)
    general_tables << OsLib_Reporting.design_day_table(model, sqlFile, runner)
    general_tables << OsLib_Reporting.setpoint_not_met_summary_table(model, sqlFile, runner)
    general_tables << OsLib_Reporting.setpoint_not_met_criteria_table(model, sqlFile, runner)
    # general_tables << OsLib_Reporting.site_performance_table(model,sqlFile,runner)
    site_power_generation_table = OsLib_Reporting.site_power_generation_table(model, sqlFile, runner)
    if site_power_generation_table
      general_tables << OsLib_Reporting.site_power_generation_table(model, sqlFile, runner)
    end

    return @building_summary_section
  end

  # annual_overview section
  def self.annual_overview_section(model, sqlFile, runner, name_only = false)
    # array to hold tables
    annual_tables = []

    # gather data for section
    @annual_overview_section = {}
    @annual_overview_section[:title] = 'Annual Overview'
    @annual_overview_section[:tables] = annual_tables

    # stop here if only name is requested this is used to populate display name for arguments
    if name_only == true
      return @annual_overview_section
    end

    # add in annual overview from method
    annual_tables << OsLib_Reporting.output_data_end_use_table(model, sqlFile, runner)
    annual_tables << OsLib_Reporting.output_data_energy_use_table(model, sqlFile, runner)
    annual_tables << OsLib_Reporting.output_data_end_use_electricity_table(model, sqlFile, runner)
    annual_tables << OsLib_Reporting.output_data_end_use_gas_table(model, sqlFile, runner)

    return @annual_overview_section
  end

  # create table with general building information
  # this just makes a table, and not a full section. It feeds into another method that makes a full section
  def self.general_building_information_table(model, sqlFile, runner)
    # general building information type data output
    general_building_information = {}
    general_building_information[:title] = 'Building Summary' # name will be with section
    general_building_information[:header] = %w(Information Value Units)
    general_building_information[:units] = [] # won't populate for this table since each row has different units
    general_building_information[:data] = []

    # structure ID / building name
    display = 'Building Name'
    target_units = 'building_name'
    value = model.getBuilding.name.to_s
    general_building_information[:data] << [display, value, target_units]
    runner.registerValue(display.downcase.gsub(" ","_"), value, target_units)

    # net site energy
    display = 'Net Site Energy'
    source_units = 'GJ'
    target_units = 'kBtu'
    value = OpenStudio.convert(sqlFile.netSiteEnergy.get, source_units, target_units).get
    value_neat = OpenStudio.toNeatString(value, 0, true)
    general_building_information[:data] << [display, value_neat, target_units]
    runner.registerValue(display.downcase.gsub(" ","_"), value, target_units)

    # total building area
    query = 'SELECT Value FROM tabulardatawithstrings WHERE '
    query << "ReportName='AnnualBuildingUtilityPerformanceSummary' and "
    query << "ReportForString='Entire Facility' and "
    query << "TableName='Building Area' and "
    query << "RowName='Total Building Area' and "
    query << "ColumnName='Area' and "
    query << "Units='m2';"
    query_results = sqlFile.execAndReturnFirstDouble(query)
    if query_results.empty?
      runner.registerWarning('Did not find value for total building area.')
      return false
    else
      display = 'Total Building Area'
      source_units = 'm^2'
      target_units = 'ft^2'
      value = OpenStudio.convert(query_results.get, source_units, target_units).get
      value_neat = OpenStudio.toNeatString(value, 0, true)
      general_building_information[:data] << [display, value_neat, target_units]
      runner.registerValue(display.downcase.gsub(" ","_"), value, target_units)
    end

    # temp code to check OS vs. E+ area
    energy_plus_area = query_results.get
    open_studio_area = model.getBuilding.floorArea
    if not energy_plus_area == open_studio_area
      runner.registerWarning("EnergyPlus reported area is #{query_results.get} (m^2). OpenStudio reported area is #{model.getBuilding.floorArea} (m^2).")
    end

    # EUI
    eui =  sqlFile.netSiteEnergy.get / query_results.get
    display = 'EUI'
    source_units = 'GJ/m^2'
    target_units = 'kBtu/ft^2'
    if query_results.get > 0.0 # don't calculate EUI if building doesn't have any area
      value = OpenStudio.convert(eui, source_units, target_units).get
      value_neat = OpenStudio.toNeatString(value, 2, true)
      runner.registerValue(display.downcase.gsub(" ","_"), value, target_units) # is it ok not to calc EUI if no area in model
    else
      value_neat = "can't calculate EUI."
    end
    general_building_information[:data] << ["#{display} (Based on Net Site Energy and Total Building Area)", value_neat, target_units]

    # get standards building type
    building_type = ''
    if model.getBuilding.standardsBuildingType.is_initialized
      building_type = model.getBuilding.standardsBuildingType.get
    end
    general_building_information[:data] << ['OpenStudio Standards Building Type',building_type,'']

    return general_building_information
  end

  # create table of space type breakdown
  def self.space_type_breakdown_section(model, sqlFile, runner, name_only = false)
    # space type data output
    output_data_space_type_breakdown = {}
    output_data_space_type_breakdown[:title] = ''
    output_data_space_type_breakdown[:header] = ['Space Type Name', 'Floor Area', 'Standards Building Type', 'Standards Space Type']
    units = 'ft^2'
    output_data_space_type_breakdown[:units] = ['', units,'','']
    output_data_space_type_breakdown[:data] = []
    output_data_space_type_breakdown[:chart_type] = 'simple_pie'
    output_data_space_type_breakdown[:chart] = []

    # gather data for section
    @output_data_space_type_breakdown_section = {}
    @output_data_space_type_breakdown_section[:title] = 'Space Type Breakdown'
    @output_data_space_type_breakdown_section[:tables] = [output_data_space_type_breakdown] # only one table for this section

    # stop here if only name is requested this is used to populate display name for arguments
    if name_only == true
      return @output_data_space_type_breakdown_section
    end

    space_types = model.getSpaceTypes

    space_types.sort.each do |spaceType|
      next if spaceType.floorArea == 0

      # get color
      color = spaceType.renderingColor
      if !color.empty?
        color = color.get
        red = color.renderingRedValue
        green = color.renderingGreenValue
        blue = color.renderingBlueValue
        color = "rgb(#{red},#{green},#{blue})"
      else
        # TODO: - this should set red green and blue as separate values
        color = 'rgb(20,20,20)' # maybe do random or let d3 pick color instead of this?
      end

      # data for space type breakdown
      display = spaceType.name.get
      floor_area_si = spaceType.floorArea
      floor_area_si = 0
      # loop through spaces so I can skip if not included in floor area
      spaceType.spaces.each do |space|
        next if not space.partofTotalFloorArea
        floor_area_si += space.floorArea * space.multiplier
      end

      value = OpenStudio.convert(floor_area_si, 'm^2', units).get
      num_people = nil
      value_neat = OpenStudio.toNeatString(value, 0, true)

      # get standards information
      if spaceType.standardsBuildingType.is_initialized
        standards_building_type = spaceType.standardsBuildingType.get
      else
        standards_building_type = ''
      end
      if spaceType.standardsSpaceType.is_initialized
        standards_space_type = spaceType.standardsSpaceType.get
      else
        standards_space_type = ''
      end

      output_data_space_type_breakdown[:data] << [display, value_neat,standards_building_type,standards_space_type]
      runner.registerValue("space_type_#{display.downcase.gsub(" ","_")}", value, units)

      # data for graph
      output_data_space_type_breakdown[:chart] << JSON.generate(label: display, value: value, color: color)
    end

    spaces = model.getSpaces

    # count area of spaces that have no space type
    no_space_type_area_counter = 0

    spaces.each do |space|
      if space.spaceType.empty?
        next if not space.partofTotalFloorArea
        no_space_type_area_counter += space.floorArea * space.multiplier
      end
    end

    if no_space_type_area_counter > 0
      display = 'No Space Type'
      value = OpenStudio.convert(no_space_type_area_counter, 'm^2', units).get
      value_neat = OpenStudio.toNeatString(value, 0, true)
      output_data_space_type_breakdown[:data] << [display, value_neat]
      runner.registerValue("space_type_#{display.downcase.gsub(" ","_")}", value, units)

      # data for graph
      color = 'rgb(20,20,20)' # maybe do random or let d3 pick color instead of this?
      output_data_space_type_breakdown[:chart] << JSON.generate(label: 'No SpaceType Assigned', value: OpenStudio.convert(no_space_type_area_counter, 'm^2', 'ft^2'), color: color)
    end

    return @output_data_space_type_breakdown_section
  end

  # create table with general building information
  # this just makes a table, and not a full section. It feeds into another method that makes a full section
  def self.output_data_end_use_table(model, sqlFile, runner)
    # end use data output
    output_data_end_use = {}
    output_data_end_use[:title] = 'End Use'
    output_data_end_use[:header] = ['End Use', 'Consumption']
    target_units = 'kBtu'
    output_data_end_use[:units] = ['', target_units]
    output_data_end_use[:data] = []
    output_data_end_use[:chart_type] = 'simple_pie'
    output_data_end_use[:chart] = []

    end_use_colors = ['#EF1C21', '#0071BD', '#F7DF10', '#DEC310', '#4A4D4A', '#B5B2B5', '#FF79AD', '#632C94', '#F75921', '#293094', '#CE5921', '#FFB239', '#29AAE7', '#8CC739']

    # loop through fuels for consumption tables
    counter = 0
    OpenStudio::EndUseCategoryType.getValues.each do |end_use|
      # get end uses
      end_use = OpenStudio::EndUseCategoryType.new(end_use).valueDescription
      query_elec = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='AnnualBuildingUtilityPerformanceSummary' and TableName='End Uses' and RowName= '#{end_use}' and ColumnName= 'Electricity'"
      query_gas = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='AnnualBuildingUtilityPerformanceSummary' and TableName='End Uses' and RowName= '#{end_use}' and ColumnName= 'Natural Gas'"
      query_add = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='AnnualBuildingUtilityPerformanceSummary' and TableName='End Uses' and RowName= '#{end_use}' and ColumnName= 'Additional Fuel'"
      query_dc = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='AnnualBuildingUtilityPerformanceSummary' and TableName='End Uses' and RowName= '#{end_use}' and ColumnName= 'District Cooling'"
      query_dh = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='AnnualBuildingUtilityPerformanceSummary' and TableName='End Uses' and RowName= '#{end_use}' and ColumnName= 'District Heating'"
      results_elec = sqlFile.execAndReturnFirstDouble(query_elec).get
      results_gas = sqlFile.execAndReturnFirstDouble(query_gas).get
      results_add = sqlFile.execAndReturnFirstDouble(query_add).get
      results_dc = sqlFile.execAndReturnFirstDouble(query_dc).get
      results_dh = sqlFile.execAndReturnFirstDouble(query_dh).get
      total_end_use = results_elec + results_gas + results_add + results_dc + results_dh
      value = OpenStudio.convert(total_end_use, 'GJ', target_units).get
      value_neat = OpenStudio.toNeatString(value, 0, true)
      output_data_end_use[:data] << [end_use, value_neat]
      runner.registerValue("end_use_#{end_use.downcase.gsub(" ","_")}", value, target_units)
      if value > 0
        output_data_end_use[:chart] << JSON.generate(label: end_use, value: value, color: end_use_colors[counter])
      end

      counter += 1
    end

    return output_data_end_use
  end

  # create table with general building information
  # this just makes a table, and not a full section. It feeds into another method that makes a full section
  def self.output_data_end_use_electricity_table(model, sqlFile, runner)
    # end use data output
    output_data_end_use_electricity = {}
    output_data_end_use_electricity[:title] = 'EUI - Electricity'
    output_data_end_use_electricity[:header] = ['End Use', 'Consumption']
    target_units = 'kWh'
    output_data_end_use_electricity[:units] = ['', target_units]
    output_data_end_use_electricity[:data] = []
    output_data_end_use_electricity[:chart_type] = 'simple_pie'
    output_data_end_use_electricity[:chart] = []

    end_use_colors = ['#EF1C21', '#0071BD', '#F7DF10', '#DEC310', '#4A4D4A', '#B5B2B5', '#FF79AD', '#632C94', '#F75921', '#293094', '#CE5921', '#FFB239', '#29AAE7', '#8CC739']

    # loop through fuels for consumption tables
    counter = 0
    OpenStudio::EndUseCategoryType.getValues.each do |end_use|
      # get end uses
      end_use = OpenStudio::EndUseCategoryType.new(end_use).valueDescription
      query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='AnnualBuildingUtilityPerformanceSummary' and TableName='End Uses' and RowName= '#{end_use}' and ColumnName= 'Electricity'"
      results = sqlFile.execAndReturnFirstDouble(query)
      value = OpenStudio.convert(results.get, 'GJ', target_units).get
      value_neat = OpenStudio.toNeatString(value, 0, true)
      output_data_end_use_electricity[:data] << [end_use, value_neat]
      runner.registerValue("end_use_electricity_#{end_use.downcase.gsub(" ","_")}", value, target_units)
      if value > 0
        output_data_end_use_electricity[:chart] << JSON.generate(label: end_use, value: value, color: end_use_colors[counter])
      end

      counter += 1
    end

    return output_data_end_use_electricity
  end

  # create table with general building information
  # this just makes a table, and not a full section. It feeds into another method that makes a full section
  def self.output_data_end_use_gas_table(model, sqlFile, runner)
    # end use data output
    output_data_end_use_gas = {}
    output_data_end_use_gas[:title] = 'EUI - Gas'
    output_data_end_use_gas[:header] = ['End Use', 'Consumption']
    target_units = 'therms'
    output_data_end_use_gas[:units] = ['', target_units]
    output_data_end_use_gas[:data] = []
    output_data_end_use_gas[:chart_type] = 'simple_pie'
    output_data_end_use_gas[:chart] = []
    output_data_end_use_gas[:chart_type] = 'simple_pie'
    output_data_end_use_gas[:chart] = []

    end_use_colors = ['#EF1C21', '#0071BD', '#F7DF10', '#DEC310', '#4A4D4A', '#B5B2B5', '#FF79AD', '#632C94', '#F75921', '#293094', '#CE5921', '#FFB239', '#29AAE7', '#8CC739']

    # loop through fuels for consumption tables
    counter = 0
    OpenStudio::EndUseCategoryType.getValues.each do |end_use|
      # get end uses
      end_use = OpenStudio::EndUseCategoryType.new(end_use).valueDescription
      query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='AnnualBuildingUtilityPerformanceSummary' and TableName='End Uses' and RowName= '#{end_use}' and ColumnName= 'Natural Gas'"
      results = sqlFile.execAndReturnFirstDouble(query)
      value = results.get * 9.48 # manual conversion from GJ to therms
      value_neat = OpenStudio.toNeatString(value, 0, true)
      output_data_end_use_gas[:data] << [end_use, value_neat]
      runner.registerValue("end_use_natural_gas_#{end_use.downcase.gsub(" ","_")}", value, target_units)
      if value > 0
        output_data_end_use_gas[:chart] << JSON.generate(label: end_use, value: value, color: end_use_colors[counter])
      end

      counter += 1
    end

    return output_data_end_use_gas
  end

  # create table with general building information
  # this just makes a table, and not a full section. It feeds into another method that makes a full section
  def self.output_data_energy_use_table(model, sqlFile, runner)
    # energy use data output
    output_data_energy_use = {}
    output_data_energy_use[:title] = 'Energy Use'
    output_data_energy_use[:header] = ['Fuel', 'Consumption']
    output_data_energy_use[:units] = ['', 'kBtu']
    output_data_energy_use[:data] = []
    output_data_energy_use[:chart_type] = 'simple_pie'
    output_data_energy_use[:chart] = []

    # list of colors for fuel. Also used for cash flow chart
    color = []
    color << '#DDCC77' # Electricity
    color << '#999933' # Natural Gas
    color << '#AA4499' # Additional Fuel
    color << '#88CCEE' # District Cooling
    color << '#CC6677' # District Heating
    # color << "#332288" # Water (not used here but is in cash flow chart)
    # color << "#117733" # Capital and O&M (not used here but is in cash flow chart)

    # loop through fuels for consumption tables
    counter = 0
    OpenStudio::EndUseFuelType.getValues.each do |fuel_type|
      # get fuel type and units
      fuel_type = OpenStudio::EndUseFuelType.new(fuel_type).valueDescription
      next if fuel_type == 'Water'
      query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='AnnualBuildingUtilityPerformanceSummary' and TableName='End Uses' and RowName= 'Total End Uses' and ColumnName= '#{fuel_type}'"
      results = sqlFile.execAndReturnFirstDouble(query)
      target_units = 'kBtu'
      value = OpenStudio.convert(results.get, 'GJ', target_units).get
      value_neat = OpenStudio.toNeatString(value, 0, true)
      output_data_energy_use[:data] << [fuel_type, value_neat]
      runner.registerValue("fuel_#{fuel_type.downcase.gsub(" ","_")}", value, target_units)
      if value > 0
        output_data_energy_use[:chart] << JSON.generate(label: fuel_type, value: value, color: color[counter])
      end

      counter += 1
    end

    return output_data_energy_use
  end

  # create table for advisory messages
  def self.setpoint_not_met_summary_table(model, sqlFile, runner)
    # unmet hours data output
    setpoint_not_met_summary = {}
    setpoint_not_met_summary[:title] = 'Unmet Hours Summary'
    setpoint_not_met_summary[:header] = ['Time Setpoint Not Met', 'Time']
    target_units = 'hr'
    setpoint_not_met_summary[:units] = ['', target_units]
    setpoint_not_met_summary[:data] = []

    # create string for rows (transposing from what is in tabular data)
    setpoint_not_met_cat = []
    setpoint_not_met_cat << 'During Heating'
    setpoint_not_met_cat << 'During Cooling'
    setpoint_not_met_cat << 'During Occupied Heating'
    setpoint_not_met_cat << 'During Occupied Cooling'

    # loop through  messages
    setpoint_not_met_cat.each do |cat|
      # Retrieve end use percentages from  table
      query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='SystemSummary' and TableName = 'Time Setpoint Not Met' and RowName= 'Facility' and ColumnName='#{cat}';"
      setpoint_not_met_cat_value = sqlFile.execAndReturnFirstDouble(query)
      if setpoint_not_met_cat_value.empty?
        runner.registerWarning("Did not find value for #{cat}.")
        return false
      else
        # net site energy
        display = cat
        source_units = 'hr'
        value = setpoint_not_met_cat_value.get
        value_neat = value # OpenStudio::toNeatString(value,0,true)
        setpoint_not_met_summary[:data] << [display, value_neat]
        runner.registerValue("unmet_hours_#{display.downcase.gsub(" ","_")}", value, target_units)

      end
    end # setpoint_not_met_cat.each do

    return setpoint_not_met_summary
  end

  # create table for setpoint_not_met_criteria
  def self.setpoint_not_met_criteria_table(model, sqlFile, runner)
    # unmet hours data output
    tolerance_summary = {}
    tolerance_summary[:title] = 'Unmet Hours Tolerance'
    tolerance_summary[:header] = ['Tolerance for Time Setpoint Not Met', 'Temperature']
    target_units = 'F'
    tolerance_summary[:units] = ['', target_units]
    tolerance_summary[:data] = []

    # create string for rows (transposing from what is in tabular data)
    setpoint_not_met_cat = []
    setpoint_not_met_cat << 'Heating'
    setpoint_not_met_cat << 'Cooling'

    # loop through  messages
    setpoint_not_met_cat.each do |cat|
      # Retrieve end use percentages from  table
      query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='AnnualBuildingUtilityPerformanceSummary' and TableName = 'Setpoint Not Met Criteria' and RowName= 'Tolerance for Zone #{cat} Setpoint Not Met Time' and ColumnName='Degrees';"
      setpoint_not_met_cat_value = sqlFile.execAndReturnFirstDouble(query)
      if setpoint_not_met_cat_value.empty?
        runner.registerWarning("Did not find value for #{cat}.")
        return false
      else
        # net site energy
        display = cat
        source_units = 'C'
        value = OpenStudio.convert(setpoint_not_met_cat_value.get.to_f,'K','R').get
        value_neat = value.round(2)
        tolerance_summary[:data] << [display, value_neat]
        runner.registerValue("unmet_hours_tolerance_#{cat.downcase}", value, target_units)
      end
    end # setpoint_not_met_cat.each do

    return tolerance_summary
  end

  # summary of what to show for each type of air loop component
  def self.air_loop_component_summary_logic(component, model)

    data_arrays = []

    if component.to_AirLoopHVACOutdoorAirSystem.is_initialized
      component = component.to_AirLoopHVACOutdoorAirSystem.get
      # get ControllerOutdoorAir
      controller_oa = component.getControllerOutdoorAir

      sizing_source_units = 'm^3/s'
      sizing_target_units = 'cfm'
      if controller_oa.maximumOutdoorAirFlowRate.is_initialized
        sizing_ip = OpenStudio.convert(controller_oa.maximumOutdoorAirFlowRate.get, sizing_source_units, sizing_target_units).get
        sizing_ip_neat = OpenStudio.toNeatString(sizing_ip, 2, true)
      else
        sizing_ip_neat = 'Autosized'
      end
      value_source_units = 'm^3/s'
      value_target_units = 'cfm'
      if controller_oa.minimumOutdoorAirFlowRate.is_initialized
        value_ip = OpenStudio.convert(controller_oa.minimumOutdoorAirFlowRate.get, value_source_units, value_target_units).get
        value_ip_neat = OpenStudio.toNeatString(value_ip, 2, true)
      else
        value_ip_neat = 'Autosized'
      end
      data_arrays << [component.iddObject.name, sizing_ip_neat, sizing_target_units, 'Minimum Outdoor Air Flow Rate', value_ip_neat, value_target_units, '']

    elsif component.to_CoilCoolingDXSingleSpeed.is_initialized
      component = component.to_CoilCoolingDXSingleSpeed.get
      sizing_source_units = 'W'
      sizing_target_units = 'Btu/h'
      if component.ratedTotalCoolingCapacity.is_initialized
        sizing_ip = OpenStudio.convert(component.ratedTotalCoolingCapacity.get, sizing_source_units, sizing_target_units).get
        sizing_ip_neat = OpenStudio.toNeatString(sizing_ip, 2, true)
      else
        sizing_ip_neat = 'Autosized'
      end
      value_source_units = 'COP'
      value_target_units = 'COP'
      value_ip = component.ratedCOP.get
      value_ip_neat = OpenStudio.toNeatString(value_ip, 2, true)
      description = 'Rated COP'
      data_arrays << [component.iddObject.name, sizing_ip_neat, sizing_target_units, description, value_ip_neat, value_target_units, '']

    elsif component.to_CoilCoolingDXTwoSpeed.is_initialized
      component = component.to_CoilCoolingDXTwoSpeed.get

      # high speed
      sizing_source_units = 'W'
      sizing_target_units = 'Btu/h'
      if component.ratedHighSpeedTotalCoolingCapacity.is_initialized
        sizing_ip = OpenStudio.convert(component.ratedHighSpeedTotalCoolingCapacity.get, sizing_source_units, sizing_target_units).get
        sizing_ip_neat = OpenStudio.toNeatString(sizing_ip, 2, true)
      else
        sizing_ip_neat = 'Autosized'
      end
      value_source_units = 'COP'
      value_target_units = 'COP'
      value_ip = component.ratedHighSpeedCOP.get
      value_ip_neat = OpenStudio.toNeatString(value_ip, 2, true)
      description = 'Rated COP'
      data_arrays << ["#{component.iddObject.name} - HighSpeed", sizing_ip_neat, sizing_target_units, description, value_ip_neat, value_target_units, '']

      # low speed
      sizing_source_units = 'W'
      sizing_target_units = 'Btu/h'
      if component.ratedLowSpeedTotalCoolingCapacity.is_initialized
        sizing_ip = OpenStudio.convert(component.ratedLowSpeedTotalCoolingCapacity.get, sizing_source_units, sizing_target_units).get
        sizing_ip_neat = OpenStudio.toNeatString(sizing_ip, 2, true)
      else
        sizing_ip_neat = 'Autosized'
      end
      value_source_units = 'COP'
      value_target_units = 'COP'
      value_ip = component.ratedLowSpeedCOP.get
      value_ip_neat = OpenStudio.toNeatString(value_ip, 2, true)
      description = 'Rated COP'
      data_arrays << ["#{component.iddObject.name} (cont) - LowSpeed", sizing_ip_neat, sizing_target_units, description, value_ip_neat, value_target_units, '']

    elsif component.iddObject.name == 'OS:Coil:Cooling:Water'
      component = component.to_CoilCoolingWater.get
      sizing_source_units = 'm^3/s'
      sizing_target_units = 'gal/min'
      if component.designWaterFlowRate.is_initialized
        sizing_ip = OpenStudio.convert(component.designWaterFlowRate.get, sizing_source_units, sizing_target_units).get
        sizing_ip_neat = OpenStudio.toNeatString(sizing_ip, 2, true)
      else
        sizing_ip_neat = 'Autosized'
      end
      value = component.plantLoop.get.name
      description = 'Plant Loop'
      data_arrays << [component.iddObject.name, sizing_ip_neat, sizing_target_units, description, value, '', '']

    elsif component.to_CoilHeatingGas.is_initialized
      component = component.to_CoilHeatingGas.get
      sizing_source_units = 'W'
      sizing_target_units = 'Btu/h'
      if component.nominalCapacity.is_initialized
        sizing_ip = OpenStudio.convert(component.nominalCapacity.get, sizing_source_units, sizing_target_units).get
        sizing_ip_neat = OpenStudio.toNeatString(sizing_ip, 2, true)
      else
        sizing_ip_neat = 'Autosized'
      end
      value_source_units = ''
      value_target_units = ''
      value_ip = component.gasBurnerEfficiency
      value_ip_neat = OpenStudio.toNeatString(value_ip, 2, true)
      description = 'Gas Burner Efficiency'
      data_arrays << [component.iddObject.name, sizing_ip_neat, sizing_target_units, description, value_ip_neat, value_target_units, '']

    elsif component.to_CoilHeatingElectric.is_initialized
      component = component.to_CoilHeatingElectric.get
      sizing_source_units = 'W'
      sizing_target_units = 'Btu/h'
      if component.nominalCapacity.is_initialized
        sizing_ip = OpenStudio.convert(component.nominalCapacity.get, sizing_source_units, sizing_target_units).get
        sizing_ip_neat = OpenStudio.toNeatString(sizing_ip, 2, true)
      else
        sizing_ip_neat = 'Autosized'
      end
      value_source_units = ''
      value_target_units = ''
      value_ip = component.efficiency
      value_ip_neat = OpenStudio.toNeatString(value_ip, 2, true)
      description = 'Efficiency'
      data_arrays << [component.iddObject.name, sizing_ip_neat, sizing_target_units, description, value_ip_neat, value_target_units, '']

    elsif component.to_CoilHeatingDXSingleSpeed.is_initialized
      component = component.to_CoilHeatingDXSingleSpeed.get
      sizing_source_units = 'W'
      sizing_target_units = 'Btu/h'
      if component.ratedTotalHeatingCapacity.is_initialized
        sizing_ip = OpenStudio.convert(component.ratedTotalHeatingCapacity.get, sizing_source_units, sizing_target_units).get
        sizing_ip_neat = OpenStudio.toNeatString(sizing_ip, 2, true)
      else
        sizing_ip_neat = 'Autosized'
      end
      value_source_units = 'COP'
      value_target_units = 'COP'
      value_ip = component.ratedCOP # is optional for CoilCoolingDXSingleSpeed but is just a double for CoilHeatingDXSingleSpeed
      value_ip_neat = OpenStudio.toNeatString(value_ip, 2, true)
      description = 'Rated COP'
      data_arrays << [component.iddObject.name, sizing_ip_neat, sizing_target_units, description, value_ip_neat, value_target_units, '']

    elsif component.to_CoilHeatingWater.is_initialized
      component = component.to_CoilHeatingWater.get
      sizing_source_units = 'm^3/s'
      sizing_target_units = 'gal/min'
      if component.maximumWaterFlowRate.is_initialized
        sizing_ip = OpenStudio.convert(component.maximumWaterFlowRate.get, sizing_source_units, sizing_target_units).get
        sizing_ip_neat = OpenStudio.toNeatString(sizing_ip, 2, true)
      else
        sizing_ip_neat = 'Autosized'
      end
      value = component.plantLoop.get.name
      description = 'Plant Loop'
      data_arrays << [component.iddObject.name, sizing_ip_neat, sizing_target_units, description, value, '', '']

    elsif component.to_FanConstantVolume.is_initialized
      component = component.to_FanConstantVolume.get
      sizing_source_units = 'm^3/s'
      sizing_target_units = 'cfm'
      if component.maximumFlowRate.is_initialized
        sizing_ip = OpenStudio.convert(component.maximumFlowRate.get, sizing_source_units, sizing_target_units).get
        sizing_ip_neat = OpenStudio.toNeatString(sizing_ip, 2, true)
      else
        sizing_ip_neat = 'Autosized'
      end
      value_source_units = 'Pa'
      value_target_units = 'inH_{2}O'
      value_ip = OpenStudio.convert(component.pressureRise, value_source_units, value_target_units).get
      value_ip_neat = OpenStudio.toNeatString(value_ip, 2, true)
      data_arrays << [component.iddObject.name, sizing_ip_neat, sizing_target_units, 'Pressure Rise', value_ip_neat, value_target_units, '']

    elsif component.to_FanVariableVolume.is_initialized
      component = component.to_FanVariableVolume.get
      sizing_source_units = 'm^3/s'
      sizing_target_units = 'cfm'
      if component.maximumFlowRate.is_initialized
        sizing_ip = OpenStudio.convert(component.maximumFlowRate.get, sizing_source_units, sizing_target_units).get
        sizing_ip_neat = OpenStudio.toNeatString(sizing_ip, 2, true)
      else
        sizing_ip_neat = 'Autosized'
      end
      value_source_units = 'Pa'
      value_target_units = 'inH_{2}O'
      value_ip = OpenStudio.convert(component.pressureRise, value_source_units, value_target_units).get
      value_ip_neat = OpenStudio.toNeatString(value_ip, 2, true)
      data_arrays << [component.iddObject.name, sizing_ip_neat, sizing_target_units, 'Pressure Rise', value_ip_neat, value_target_units, '']

    elsif component.iddObject.name == 'OS:SetpointManager:Scheduled'
      setpoint = component.to_SetpointManagerScheduled.get
      supply_air_temp_schedule = setpoint.schedule
      schedule_values = OsLib_Schedules.getMinMaxAnnualProfileValue(model, supply_air_temp_schedule)
      if schedule_values.nil?
        schedule_values_pretty = "can't inspect schedule"
        target_units = ''
      else
        if setpoint.controlVariable.to_s == 'Temperature'
          source_units = 'C'
          target_units = 'F'
          schedule_values_pretty = "#{OpenStudio.convert(schedule_values['min'], source_units, target_units).get.round(1)} to #{OpenStudio.convert(schedule_values['max'], source_units, target_units).get.round(1)}"
        else # TODO: - add support for other control variables
          schedule_values_pretty = "#{schedule_values['min']} to #{schedule_values['max']}"
          target_units = 'raw si values'
        end
      end
      data_arrays << [setpoint.iddObject.name, '', '', "Control Variable - #{setpoint.controlVariable}", schedule_values_pretty, target_units, '']

    elsif component.iddObject.name == 'OS:SetpointManager:SingleZone:Reheat'
      setpoint = component.to_SetpointManagerSingleZoneReheat.get
      control_zone = setpoint.controlZone
      if control_zone.is_initialized
        control_zone_name = control_zone.get.name
      else
        control_zone_name = ''
      end
      data_arrays << [component.iddObject.name, '', '', 'Control Zone', control_zone_name, '', '']

    else
      data_arrays << [component.iddObject.name, '', '', '', '', '', '']
    end

    # TODO: - add support for more types of objects

    # thermal zones and terminals are handled directly in the air loop helper
    # since they operate over a collection of objects vs. a single component

    return data_arrays
  end

  # create table air loop summary
  def self.air_loops_detail_section(model, sqlFile, runner, name_only = false)
    # array to hold tables
    output_data_air_loop_tables = []

    # gather data for section
    @output_data_air_loop_section = {}
    @output_data_air_loop_section[:title] = 'Air Loops Detail'
    @output_data_air_loop_section[:tables] = output_data_air_loop_tables

    # stop here if only name is requested this is used to populate display name for arguments
    if name_only == true
      return @output_data_air_loop_section
    end

    # loop through air loops
    model.getAirLoopHVACs.sort.each do |air_loop|
      # air loop data output
      output_data_air_loops = {}
      output_data_air_loops[:title] = air_loop.name.get # TODO: - confirm first that it has name
      output_data_air_loops[:header] = ['Object', 'Sizing', 'Sizing Units', 'Description', 'Value', 'Value Units', 'Count']
      output_data_air_loops[:units] = [] # not using units for these tables
      output_data_air_loops[:data] = []

      output_data_air_loops[:data] << [{ sub_header: 'supply' }, '', '', '', '', '', '']

      # hold values for later use
      dcv_setting = 'na' # should hit this if there isn't an outdoor air object on the loop
      economizer_setting = 'na' # should hit this if there isn't an outdoor air object on the loop

      # loop through components
      air_loop.supplyComponents.each do |component|
        # skip some object types, but look for node with setpoint manager
        if component.to_Node.is_initialized
          setpoint_managers = component.to_Node.get.setpointManagers
          if setpoint_managers.size > 0
            # setpoint type
            setpoint = setpoint_managers[0] # TODO: - could have more than one in some situations
            data_arrays = OsLib_Reporting.air_loop_component_summary_logic(setpoint, model)
            data_arrays.each do |data_array|
              output_data_air_loops[:data] << data_array
            end
          end
        else
          # populate table for everything but setpoint managers, which are added above.
          data_arrays = OsLib_Reporting.air_loop_component_summary_logic(component, model)
          data_arrays.each do |data_array|
            output_data_air_loops[:data] << data_array
          end

        end

        # gather controls information to use later
        if component.to_AirLoopHVACOutdoorAirSystem.is_initialized
          hVACComponent = component.to_AirLoopHVACOutdoorAirSystem.get

          # get ControllerOutdoorAir
          controller_oa = hVACComponent.getControllerOutdoorAir
          # get ControllerMechanicalVentilation
          controller_mv = controller_oa.controllerMechanicalVentilation
          # get dcv value
          dcv_setting = controller_mv.demandControlledVentilation
          # get economizer setting
          economizer_setting =  controller_oa.getEconomizerControlType
        end
      end

      output_data_air_loops[:data] << [{ sub_header: 'demand' }, '', '', '', '', '', '']
      # demand side summary, list of terminal types used, and number of zones
      thermal_zones = []
      terminals = []
      cooling_temp_ranges = []
      heating_temps_ranges = []
      air_loop.demandComponents.each do |component|
        # gather array of thermal zones and terminals
        if component.to_ThermalZone.is_initialized
          thermal_zone = component.to_ThermalZone.get
          thermal_zones << thermal_zone
          thermal_zone.equipment.each do |zone_equip|
            next if zone_equip.to_ZoneHVACComponent.is_initialized # should only find terminals
            terminals << zone_equip.iddObject.name
          end

          # populate thermostat ranges
          if thermal_zone.thermostatSetpointDualSetpoint.is_initialized
            thermostat = thermal_zone.thermostatSetpointDualSetpoint.get
            if thermostat.coolingSetpointTemperatureSchedule.is_initialized
              schedule_values = OsLib_Schedules.getMinMaxAnnualProfileValue(model, thermostat.coolingSetpointTemperatureSchedule.get)
              unless schedule_values.nil?
                cooling_temp_ranges << schedule_values['min']
                cooling_temp_ranges << schedule_values['max']
              end
            end
            if thermostat.heatingSetpointTemperatureSchedule.is_initialized
              schedule_values = OsLib_Schedules.getMinMaxAnnualProfileValue(model, thermostat.heatingSetpointTemperatureSchedule.get)
              unless schedule_values.nil?
                heating_temps_ranges << schedule_values['min']
                heating_temps_ranges << schedule_values['max']
              end
            end
          end

        end
      end

      # get floor area of thermal zones
      total_loop_floor_area =  0
      thermal_zones.each do |zone|
        total_loop_floor_area += zone.floorArea
      end
      total_loop_floor_area_ip = OpenStudio.convert(total_loop_floor_area, 'm^2', 'ft^2').get
      total_loop_floor_area_ip_neat = OpenStudio.toNeatString(total_loop_floor_area_ip, 0, true)

      # output zone and terminal data
      output_data_air_loops[:data] << ['Thermal Zones', '', '', 'Total Floor Area', total_loop_floor_area_ip_neat, 'ft^2', thermal_zones.size]
      if cooling_temp_ranges.size == 0
        cooling_temp_ranges_pretty = "can't inspect schedules"
      else
        cooling_temp_ranges_pretty = "#{OpenStudio.convert(cooling_temp_ranges.min, 'C', 'F').get.round(1)} to #{OpenStudio.convert(cooling_temp_ranges.max, 'C', 'F').get.round(1)}"
      end
      if heating_temps_ranges.size == 0
        heating_temps_ranges_pretty = "can't inspect schedules"
      else
        heating_temps_ranges_pretty = "#{OpenStudio.convert(heating_temps_ranges.min, 'C', 'F').get.round(1)} to #{OpenStudio.convert(heating_temps_ranges.max, 'C', 'F').get.round(1)}"
      end
      output_data_air_loops[:data] << ['Thermal Zones', '', '', 'thermostat ranges for cooling', cooling_temp_ranges_pretty, 'F', '']
      output_data_air_loops[:data] << ['Thermal Zones', '', '', 'thermostat ranges for heating', heating_temps_ranges_pretty, 'F', '']
      output_data_air_loops[:data] << ['Terminal Types Used', '', '', terminals.uniq.sort.join(', '), '', '', terminals.size]

      # controls summary
      output_data_air_loops[:data] << [{ sub_header: 'controls' }, '', '', '', '', '', '']

      output_data_air_loops[:data] << ['HVAC Operation Schedule', '', '', '', air_loop.availabilitySchedule.name, '', ''] # I think this is a bool
      output_data_air_loops[:data] << ['Night Cycle Setting', '', '', '', air_loop.nightCycleControlType, 'Choice', '']
      output_data_air_loops[:data] << ['Economizer Setting', '', '', '', economizer_setting, 'Choice', '']
      output_data_air_loops[:data] << ['Demand Controlled Ventilation Status', '', '', '', dcv_setting, 'Bool', '']

      # populate tables for section
      output_data_air_loop_tables << output_data_air_loops
    end

    return @output_data_air_loop_section
  end

  # summary of what to show for each type of plant loop component
  def self.plant_loop_component_summary_logic(component, model)
    data_arrays = []
    if component.to_PumpConstantSpeed.is_initialized
      component = component.to_PumpConstantSpeed.get
      sizing_source_units = 'm^3/s'
      sizing_target_units = 'gal/min'
      if component.ratedFlowRate.is_initialized
        sizing_ip = OpenStudio.convert(component.ratedFlowRate.get, sizing_source_units, sizing_target_units).get
        sizing_ip_neat = OpenStudio.toNeatString(sizing_ip, 2, true)
      else
        sizing_ip_neat = 'Autosized'
      end
      value_source_units = 'Pa'
      value_target_units = 'W'
      if component.ratedFlowRate.is_initialized
        value_ip = OpenStudio.convert(component.ratedFlowRate.get, sizing_source_units, sizing_target_units).get
        value_ip_neat = OpenStudio.toNeatString(value_ip, 2, true)
      else
        value_ip_neat = 'Autosized'
      end
      description = 'Rated Power Consumption'
      data_arrays <<  [component.iddObject.name, sizing_ip_neat, sizing_target_units, description, value_ip_neat, value_target_units, '']

    elsif component.to_PumpVariableSpeed.is_initialized
      component = component.to_PumpVariableSpeed.get
      sizing_source_units = 'm^3/s'
      sizing_target_units = 'gal/min'
      if component.ratedFlowRate.is_initialized
        sizing_ip = OpenStudio.convert(component.ratedFlowRate.get, sizing_source_units, sizing_target_units).get
        sizing_ip_neat = OpenStudio.toNeatString(sizing_ip, 2, true)
      else
        sizing_ip_neat = 'Autosized'
      end
      value_source_units = 'Pa'
      value_target_units = 'W'
      if component.ratedFlowRate.is_initialized
        value_ip = OpenStudio.convert(component.ratedFlowRate.get, sizing_source_units, sizing_target_units).get
        value_ip_neat = OpenStudio.toNeatString(value_ip, 2, true)
      else
        value_ip_neat = 'Autosized'
      end
      description = 'Rated Power Consumption'
      data_arrays <<  [component.iddObject.name, sizing_ip_neat, sizing_target_units, description, value_ip_neat, value_target_units, '']

    elsif component.to_BoilerHotWater.is_initialized
      component = component.to_BoilerHotWater.get
      sizing_source_units = 'W'
      sizing_target_units = 'Btu/h'
      if component.nominalCapacity.is_initialized
        sizing_ip = OpenStudio.convert(component.nominalCapacity.get, sizing_source_units, sizing_target_units).get
        sizing_ip_neat = OpenStudio.toNeatString(sizing_ip, 2, true)
      else
        sizing_ip_neat = 'Autosized'
      end
      value_source_units = 'fraction'
      value_target_units = 'fraction'
      value = component.nominalThermalEfficiency
      value_neat = OpenStudio.toNeatString(value, 2, true)
      description = 'Nominal Thermal Efficiency'
      data_arrays <<  [component.iddObject.name, sizing_ip_neat, sizing_target_units, description, value_neat, value_target_units, '']

    elsif component.to_WaterHeaterMixed.is_initialized
      component = component.to_WaterHeaterMixed.get
      sizing_source_units = 'm^3'
      sizing_target_units = 'gal'
      if component.tankVolume.is_initialized
        sizing_ip = OpenStudio.convert(component.tankVolume.get, sizing_source_units, sizing_target_units).get
        sizing_ip_neat = OpenStudio.toNeatString(sizing_ip, 0, true)
      else
        sizing_ip_neat = 'Autosized'
      end
      value_source_units = 'fraction'
      value_target_units = 'fraction'
      value = component.heaterThermalEfficiency
      if value.is_initialized
        value_neat = OpenStudio.toNeatString(value.get, 2, true)
      else
        value_neat = '' # not sure what that would default to if it wasn't there
      end
      description = 'Heater Thermal Efficiency'
      data_arrays <<  [component.iddObject.name, sizing_ip_neat, sizing_target_units, description, value_neat, value_target_units, '']

    elsif component.to_ChillerElectricEIR.is_initialized
      component = component.to_ChillerElectricEIR.get
      sizing_source_units = 'W'
      sizing_target_units = 'Btu/h'
      if component.referenceCapacity.is_initialized
        sizing_ip = OpenStudio.convert(component.referenceCapacity.get, sizing_source_units, sizing_target_units).get
        sizing_ip_neat = OpenStudio.toNeatString(sizing_ip, 2, true)
      else
        sizing_ip_neat = 'Autosized'
      end
      value = component.referenceCOP
      value_neat = OpenStudio.toNeatString(value, 2, true)
      description = 'Reference COP'
      data_arrays << [component.iddObject.name, sizing_ip_neat, sizing_target_units, description, value_neat, '', '']

      # second line to indicate if water or air cooled
      if component.secondaryPlantLoop.is_initialized
        data_arrays << ["#{component.iddObject.name} (cont)", '', '', 'Chiller Source', component.secondaryPlantLoop.get.name, '', '']
      else
        data_arrays << ["#{component.iddObject.name} (cont)", '', '', 'Chiller Source', 'Air Cooled', '', '']
      end

    elsif component.to_CoolingTowerSingleSpeed.is_initialized

      # data for water
      component = component.to_CoolingTowerSingleSpeed.get
      sizing_source_units = 'm^3/s'
      sizing_target_units = 'gal/min'
      if component.designWaterFlowRate.is_initialized
        sizing_ip = OpenStudio.convert(component.designWaterFlowRate.get, sizing_source_units, sizing_target_units).get
        sizing_ip_neat = OpenStudio.toNeatString(sizing_ip, 2, true)
      else
        sizing_ip_neat = 'Autosized'
      end
      data_arrays << ["#{component.iddObject.name} - Air", sizing_ip_neat, sizing_target_units, '', '', '', '']

      # data for air
      component = component.to_CoolingTowerSingleSpeed.get
      sizing_source_units = 'm^3/s'
      sizing_target_units = 'cfm'
      if component.designAirFlowRate.is_initialized
        sizing_ip = OpenStudio.convert(component.designAirFlowRate.get, sizing_source_units, sizing_target_units).get
        sizing_ip_neat = OpenStudio.toNeatString(sizing_ip, 2, true)
      else
        sizing_ip_neat = 'Autosized'
      end
      value_source_units = 'W'
      value_target_units = 'W'
      if component.fanPoweratDesignAirFlowRate.is_initialized
        value_ip = OpenStudio.convert(component.fanPoweratDesignAirFlowRate.get, sizing_source_units, sizing_target_units).get
        value_ip_neat = OpenStudio.toNeatString(value_ip, 2, true)
      else
        value_ip_neat = 'Autosized'
      end
      description = 'Fan Power at Design Air Flow Rate'
      data_arrays << ["#{component.iddObject.name} (cont) - Water", sizing_ip_neat, sizing_target_units, description, value_ip_neat, value_target_units, '']

    elsif component.to_SetpointManagerScheduled.is_initialized
      setpoint = component.to_SetpointManagerScheduled.get
      supply_air_temp_schedule = setpoint.schedule
      schedule_values = OsLib_Schedules.getMinMaxAnnualProfileValue(model, supply_air_temp_schedule)
      if schedule_values.nil?
        schedule_values_pretty = "can't inspect schedule"
        target_units = ''
      else
        if setpoint.controlVariable.to_s == 'Temperature'
          source_units = 'C'
          target_units = 'F'
          schedule_values_pretty = "#{OpenStudio.convert(schedule_values['min'], source_units, target_units).get.round(1)} to #{OpenStudio.convert(schedule_values['max'], source_units, target_units).get.round(1)}"
        else # TODO: - add support for other control variables
          schedule_values_pretty = "#{schedule_values['min']} to #{schedule_values['max']}"
          target_units = 'raw si values'
        end
      end
      data_arrays << [setpoint.iddObject.name, '', '', "Control Variable - #{setpoint.controlVariable}", schedule_values_pretty, target_units, '']

    elsif component.to_SetpointManagerFollowOutdoorAirTemperature.is_initialized
      setpoint = component.to_SetpointManagerFollowOutdoorAirTemperature.get
      ref_temp_type = setpoint.referenceTemperatureType
      data_arrays << [setpoint.iddObject.name, '', '', 'Reference Temperature Type', ref_temp_type, 'Choice', '']

    else
      data_arrays << [component.iddObject.name, '', '', '', '', '', '']
    end

    return data_arrays
  end

  # create table plant loop summary
  def self.plant_loops_detail_section(model, sqlFile, runner, name_only = false)
    # array to hold tables
    output_data_plant_loop_tables = []

    # gather data for section
    @output_data_plant_loop_section = {}
    @output_data_plant_loop_section[:title] = 'Plant Loops Detail'
    @output_data_plant_loop_section[:tables] = output_data_plant_loop_tables

    # stop here if only name is requested this is used to populate display name for arguments
    if name_only == true
      return @output_data_plant_loop_section
    end

    # loop through plant loops
    model.getPlantLoops.sort.each do |plant_loop|
      # plant loop data output
      output_data_plant_loops = {}
      output_data_plant_loops[:title] = plant_loop.name.get # TODO: - confirm first that it has name
      output_data_plant_loops[:header] = ['Object', 'Sizing', 'Sizing Units', 'Description', 'Value', 'Value Units', 'Count']
      output_data_plant_loops[:units] = [] # not using units for these tables
      output_data_plant_loops[:data] = []

      output_data_plant_loops[:data] << [{ sub_header: 'supply' }, '', '', '', '', '', '']

      plant_loop.supplyComponents.each do |component|
        if component.to_ThermalZone.is_initialized
        end
        # skip some object types
        next if component.to_PipeAdiabatic.is_initialized
        next if component.to_Splitter.is_initialized
        next if component.to_Mixer.is_initialized
        if component.to_Node.is_initialized
          setpoint_managers = component.to_Node.get.setpointManagers
          if setpoint_managers.size > 0
            # setpoint type
            setpoint = setpoint_managers[0] # TODO: - could have more than one in some situations
            data_arrays = OsLib_Reporting.plant_loop_component_summary_logic(setpoint, model)
            data_arrays.each do |data_array| # typically just one, but in some cases there are a few
              output_data_plant_loops[:data] << data_array
            end
          end
        else
          # populate table for everything but setpoint managers, which are added above.
          data_arrays = OsLib_Reporting.plant_loop_component_summary_logic(component, model)
          data_arrays.each do |data_array| # typically just one, but in some cases there are a few
            output_data_plant_loops[:data] << data_array
          end
        end
      end

      # loop through demand components
      output_data_plant_loops[:data] << [{ sub_header: 'demand' }, '', '', '', '', '', '']

      # keep track of terminal count to report later
      terminal_connections = [] # Not sure how I want to list in display

      # loop through plant demand components
      plant_loop.demandComponents.each do |component|
        # flag for terminal connecxtions
        terminal_connection = false

        # skip some object types
        next if component.to_PipeAdiabatic.is_initialized
        next if component.to_Splitter.is_initialized
        next if component.to_Mixer.is_initialized
        next if component.to_Node.is_initialized

        # determine if water to air
        if component.to_WaterToAirComponent.is_initialized
          component = component.to_WaterToAirComponent.get
          if component.airLoopHVAC.is_initialized
            description = 'Air Loop'
            value = component.airLoopHVAC.get.name
          else
            # this is a terminal connection
            terminal_connection = true
            terminal_connections << component
          end
        elsif component.to_WaterToWaterComponent.is_initialized
          description = 'Plant Loop'
          component = component.to_WaterToWaterComponent.get
          ww_loop = component.plantLoop
          if ww_loop.is_initialized
            value = ww_loop.get.name
          else
            value = ''
          end
        else # water use connections would go here
          description = component.name
          value = ''
        end

        # don't report here if this component is connected to a terminal
        next if terminal_connection == true

        output_data_plant_loops[:data] << [component.iddObject.name, '', '', description, value, '', '']
      end

      # report terminal connections
      if terminal_connections.size > 0
        output_data_plant_loops[:data] << ['Air Terminal Connections', '', '', '', '', '', terminal_connections.size]
      end

      output_data_plant_loops[:data] << [{ sub_header: 'controls' }, '', '', '', '', '', '']

      # loop flow rates
      sizing_source_units = 'm^3/s'
      sizing_target_units = 'gal/min'
      if plant_loop.maximumLoopFlowRate.is_initialized
        sizing_ip = OpenStudio.convert(plant_loop.maximumLoopFlowRate.get, sizing_source_units, sizing_target_units).get
        sizing_ip_neat = OpenStudio.toNeatString(sizing_ip, 2, true)
      else
        sizing_ip_neat = 'Autosized'
      end
      value_source_units = 'm^3/s'
      value_target_units = 'gal/min'
      if plant_loop.maximumLoopFlowRate.is_initialized
        value_ip = OpenStudio.convert(plant_loop.minimumLoopFlowRate.get, value_source_units, value_target_units).get
        value_ip_neat = OpenStudio.toNeatString(value_ip, 2, true)
      else
        value_ip_neat = 0.0
      end
      output_data_plant_loops[:data] << ['Loop Flow Rate Range', sizing_ip_neat, sizing_target_units, 'Minimum Loop Flow Rate', value_ip_neat, value_target_units, '']

      # loop temperatures
      source_units = 'C'
      target_units = 'F'
      min_temp = plant_loop.minimumLoopTemperature
      max_temp = plant_loop.maximumLoopTemperature
      value_neat = "#{OpenStudio.convert(min_temp, source_units, target_units).get.round(1)} to #{OpenStudio.convert(max_temp, source_units, target_units).get.round(1)}"
      output_data_plant_loops[:data] << ['Loop Temperature Range', '', '', '', value_neat, target_units, '']

      # get values out of sizing plant
      sizing_plant = plant_loop.sizingPlant
      source_units = 'C'
      target_units = 'F'
      loop_exit_temp = sizing_plant.designLoopExitTemperature
      value_neat = OpenStudio.toNeatString(OpenStudio.convert(loop_exit_temp, source_units, target_units).get, 2, true)

      output_data_plant_loops[:data] << ['Design Loop Exit Temperature', '', '', '', value_neat, target_units, '']
      source_units = 'K'
      target_units = 'R'
      loop_design_temp_diff = sizing_plant.loopDesignTemperatureDifference
      value_neat = OpenStudio.toNeatString(OpenStudio.convert(loop_design_temp_diff, source_units, target_units).get, 2, true)
      output_data_plant_loops[:data] << ['Loop Design Temperature Difference', '', '', '', value_neat, target_units, '']

      # push tables
      output_data_plant_loop_tables << output_data_plant_loops
    end

    return @output_data_plant_loop_section
  end

  # summary of what to show for each type of zone equipment component
  def self.zone_equipment_component_summary_logic(component, model)
    if component.to_FanZoneExhaust.is_initialized
      component = component.to_FanZoneExhaust.get

      sizing_source_units = 'm^3/s'
      sizing_target_units = 'cfm'
      if component.maximumFlowRate.is_initialized
        sizing_ip = OpenStudio.convert(component.maximumFlowRate.get, sizing_source_units, sizing_target_units).get
        sizing_ip_neat = OpenStudio.toNeatString(sizing_ip, 2, true)
      else
        sizing_ip_neat = 0.0 # is that the proper default
      end
      value_source_units = 'fraction'
      value_target_units = 'fraction'
      value_ = component.fanEfficiency
      value_neat = OpenStudio.toNeatString(value_, 2, true)

      description = 'Fan Efficiency'
      data_array = [component.iddObject.name, sizing_ip_neat, sizing_target_units, description, value_neat, value_target_units, '']

    elsif component.to_ZoneHVACPackagedTerminalHeatPump.is_initialized
      component = component.to_ZoneHVACPackagedTerminalHeatPump.get

      # report outdoor air when not heating or cooling
      sizing_source_units = 'm^3/s'
      sizing_target_units = 'cfm'
      if component.outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded.is_initialized
        sizing_ip = OpenStudio.convert(component.outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded.get, sizing_source_units, sizing_target_units).get
        sizing_ip_neat = OpenStudio.toNeatString(sizing_ip, 2, true)
      else
        sizing_ip_neat = 'Autosized'
      end
      value = component.availabilitySchedule.name
      description = 'Availability Schedule'
      data_array = ["#{component.iddObject.name} - Outdoor Air When No Clg. or Htg", sizing_ip_neat, sizing_target_units, description, value, '', '']

      # get cooling coil
      if component.coolingCoil.to_CoilCoolingDXSingleSpeed.is_initialized
        cooling_coil = component.coolingCoil.to_CoilCoolingDXSingleSpeed.get
        sizing_source_units = 'W'
        sizing_target_units = 'Btu/h'
        if cooling_coil.ratedTotalCoolingCapacity.is_initialized
          sizing_ip = OpenStudio.convert(cooling_coil.ratedTotalCoolingCapacity.get, sizing_source_units, sizing_target_units).get
          sizing_ip_neat = OpenStudio.toNeatString(sizing_ip, 2, true)
        else
          sizing_ip_neat = 'Autosized'
        end
        value_source_units = 'COP'
        value_target_units = 'COP'
        value_ip = cooling_coil.ratedCOP.get
        value_ip_neat = OpenStudio.toNeatString(value_ip, 2, true)
        description = 'Rated COP'
        data_array = ["#{component.iddObject.name} - #{cooling_coil.iddObject.name}", sizing_ip_neat, sizing_target_units, description, value_ip_neat, value_target_units, '']
      else
        cooling_coil = component.coolingCoil
        data_array = ["#{component.iddObject.name} - #{cooling_coil.iddObject.name}", '', '', '', '', '', '']
      end

      # get heating coil
      if component.coolingCoil.to_CoilHeatingDXSingleSpeed.is_initialized
        heating_coil = component.heatingCoil.to_CoilHeatingDXSingleSpeed.get
        sizing_source_units = 'W'
        sizing_target_units = 'Btu/h'
        if heating_coil.ratedTotalHeatingCapacity.is_initialized
          sizing_ip = OpenStudio.convert(heating_coil.ratedTotalHeatingCapacity.get, sizing_source_units, sizing_target_units).get
          sizing_ip_neat = OpenStudio.toNeatString(sizing_ip, 2, true)
        else
          sizing_ip_neat = 'Autosized'
        end
        value_source_units = 'COP'
        value_target_units = 'COP'
        value_ip = heating_coil.ratedCOP # is optional for CoilCoolingDXSingleSpeed but is just a double for CoilHeatingDXSingleSpeed
        value_ip_neat = OpenStudio.toNeatString(value_ip, 2, true)
        description = 'Rated COP'
        data_array = ["#{component.iddObject.name} - #{heating_coil.iddObject.name}", sizing_ip_neat, sizing_target_units, description, value_ip_neat, value_target_units, '']
      else
        heating_coil = component.heatingCoil
        data_array = ["#{component.iddObject.name} - #{heating_coil.iddObject.name}", '', '', '', '', '', '']
      end

      # get fan
      if component.supplyAirFan.to_FanConstantVolume.is_initialized
        fan = component.supplyAirFan.to_FanConstantVolume.get
        sizing_source_units = 'm^3/s'
        sizing_target_units = 'cfm'
        if fan.maximumFlowRate.is_initialized
          sizing_ip = OpenStudio.convert(fan.maximumFlowRate.get, sizing_source_units, sizing_target_units).get
          sizing_ip_neat = OpenStudio.toNeatString(sizing_ip, 2, true)
        else
          sizing_ip_neat = 'Autosized'
        end
        value_source_units = 'Pa'
        value_target_units = 'inH_{2}O'
        value_ip = OpenStudio.convert(fan.pressureRise, value_source_units, value_target_units).get
        value_ip_neat = OpenStudio.toNeatString(value_ip, 2, true)
        data_array = ["#{component.iddObject.name} - #{fan.iddObject.name}", sizing_ip_neat, sizing_target_units, 'Pressure Rise', value_ip_neat, value_target_units, '']
      else
        fan = component.supplyAirFan
        data_array = ["#{component.iddObject.name} - #{fan.iddObject.name}", '', '', '', '', '', '']
      end

      # get supplemental heat
      if component.supplementalHeatingCoil.to_CoilHeatingElectric.is_initialized
        supplemental_heating_coil = component.supplementalHeatingCoil.to_CoilHeatingElectric.get
        sizing_source_units = 'W'
        sizing_target_units = 'Btu/h'
        if supplemental_heating_coil.nominalCapacity.is_initialized
          sizing_ip = OpenStudio.convert(supplemental_heating_coil.nominalCapacity.get, sizing_source_units, sizing_target_units).get
          sizing_ip_neat = OpenStudio.toNeatString(sizing_ip, 2, true)
        else
          sizing_ip_neat = 'Autosized'
        end
        value_source_units = ''
        value_target_units = ''
        value_ip = supplemental_heating_coil.efficiency
        value_ip_neat = OpenStudio.toNeatString(value_ip, 2, true)
        description = 'Efficiency'
        data_array = ["#{component.iddObject.name} - #{supplemental_heating_coil.iddObject.name}", sizing_ip_neat, sizing_target_units, description, value_ip_neat, value_target_units, '']
      else
        supplemental_heating_coil = component.supplyAirFan
        data_array = ["#{component.iddObject.name} - #{supplemental_heating_coil.iddObject.name}", '', '', '', '', '', '']
      end

    elsif component.to_ZoneHVACPackagedTerminalAirConditioner.is_initialized
      component = component.to_ZoneHVACPackagedTerminalAirConditioner.get

      # report outdoor air when not heating or cooling
      sizing_source_units = 'm^3/s'
      sizing_target_units = 'cfm'
      if component.outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded.is_initialized
        sizing_ip = OpenStudio.convert(component.outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded.get, sizing_source_units, sizing_target_units).get
        sizing_ip_neat = OpenStudio.toNeatString(sizing_ip, 2, true)
      else
        sizing_ip_neat = 'Autosized'
      end
      value = component.availabilitySchedule.name
      description = 'Availability Schedule'
      data_array = ["#{component.iddObject.name} - Outdoor Air When No Clg. or Htg", sizing_ip_neat, sizing_target_units, description, value, '', '']

      # get cooling coil
      if component.coolingCoil.to_CoilCoolingDXSingleSpeed.is_initialized
        cooling_coil = component.coolingCoil.to_CoilCoolingDXSingleSpeed.get
        sizing_source_units = 'W'
        sizing_target_units = 'Btu/h'
        if cooling_coil.ratedTotalCoolingCapacity.is_initialized
          sizing_ip = OpenStudio.convert(cooling_coil.ratedTotalCoolingCapacity.get, sizing_source_units, sizing_target_units).get
          sizing_ip_neat = OpenStudio.toNeatString(sizing_ip, 2, true)
        else
          sizing_ip_neat = 'Autosized'
        end
        value_source_units = 'COP'
        value_target_units = 'COP'
        value_ip = cooling_coil.ratedCOP.get
        value_ip_neat = OpenStudio.toNeatString(value_ip, 2, true)
        description = 'Rated COP'
        data_array = ["#{component.iddObject.name} - #{cooling_coil.iddObject.name}", sizing_ip_neat, sizing_target_units, description, value_ip_neat, value_target_units, '']
      else
        cooling_coil = component.coolingCoil
        data_array = ["#{component.iddObject.name} - #{cooling_coil.iddObject.name}", '', '', '', '', '', '']
      end

      # get heating coil
      if component.heatingCoil.to_CoilHeatingWater.is_initialized
        heating_coil = component.heatingCoil.to_CoilHeatingWater.get
        sizing_source_units = 'm^3/s'
        sizing_target_units = 'gal/min'
        if heating_coil.maximumWaterFlowRate.is_initialized
          sizing_ip = OpenStudio.convert(heating_coil.maximumWaterFlowRate.get, sizing_source_units, sizing_target_units).get
          sizing_ip_neat = OpenStudio.toNeatString(sizing_ip, 2, true)
        else
          sizing_ip_neat = 'Autosized'
        end
        value = heating_coil.plantLoop.get.name
        description = 'Plant Loop'
        data_array = ["#{component.iddObject.name} - #{heating_coil.iddObject.name}", sizing_ip_neat, sizing_target_units, description, value, '', '']
      else
        heating_coil = component.heatingCoil
        data_array = ["#{component.iddObject.name} - #{heating_coil.iddObject.name}", '', '', '', '', '', '']
      end

      # get fan
      if component.supplyAirFan.to_FanConstantVolume.is_initialized
        fan = component.supplyAirFan.to_FanConstantVolume.get
        sizing_source_units = 'm^3/s'
        sizing_target_units = 'cfm'
        if fan.maximumFlowRate.is_initialized
          sizing_ip = OpenStudio.convert(fan.maximumFlowRate.get, sizing_source_units, sizing_target_units).get
          sizing_ip_neat = OpenStudio.toNeatString(sizing_ip, 2, true)
        else
          sizing_ip_neat = 'Autosized'
        end
        value_source_units = 'Pa'
        value_target_units = 'inH_{2}O'
        value_ip = OpenStudio.convert(fan.pressureRise, value_source_units, value_target_units).get
        value_ip_neat = OpenStudio.toNeatString(value_ip, 2, true)
        data_array = ["#{component.iddObject.name} - #{fan.iddObject.name}", sizing_ip_neat, sizing_target_units, 'Pressure Rise', value_ip_neat, value_target_units, '']
      else
        fan = component.supplyAirFan
        data_array = ["#{component.iddObject.name} - #{fan.iddObject.name}", '', '', '', '', '', '']
      end

    else
      data_array = [component.iddObject.name, '', '', '', '', '', '']
    end

    return data_array
  end

  # create table plant loop summary
  def self.zone_equipment_detail_section(model, sqlFile, runner, name_only = false)
    # array to hold tables
    output_data_zone_equipment = []

    # gather data for section
    @output_data_zone_equipment_section = {}
    @output_data_zone_equipment_section[:title] = 'Zone Equipment Detail'
    @output_data_zone_equipment_section[:tables] = output_data_zone_equipment

    # stop here if only name is requested this is used to populate display name for arguments
    if name_only == true
      return @output_data_zone_equipment_section
    end

    # loop through thermal zones
    model.getThermalZones.sort.each do |zone|
      # plant loop data output
      output_data_zone_equipment = {}
      output_data_zone_equipment[:title] = zone.name.get # TODO: - confirm that zone has a name
      output_data_zone_equipment[:header] = ['Object', 'Sizing', 'Sizing Units', 'Description', 'Value', 'Value Units', 'Count']
      output_data_zone_equipment[:units] = [] # not using units for these tables
      output_data_zone_equipment[:data] = []

      zone.equipment.sort.each do |zone_equip|
        next unless zone_equip.to_ZoneHVACComponent.is_initialized # skip any terminals
        output_data_zone_equipment[:data] << OsLib_Reporting.zone_equipment_component_summary_logic(zone_equip, model)
      end

      # push table to array
      if output_data_zone_equipment[:data].size > 0
        @output_data_zone_equipment_section[:tables] << output_data_zone_equipment
      end
    end

    return @output_data_zone_equipment_section
  end

  # create table for constructions
  def self.envelope_section_section(model, sqlFile, runner, name_only = false)
    # array to hold tables
    envelope_tables = []

    # gather data for section
    @envelope_section = {}
    @envelope_section[:title] = 'Envelope'
    @envelope_section[:tables] = envelope_tables

    # stop here if only name is requested this is used to populate display name for arguments
    if name_only == true
      return @envelope_section
    end

    # summary of exterior constructions used in the model for base surfaces
    surface_data = {}
    surface_data[:title] = 'Base Surface Constructions'
    surface_data[:header] = ['Construction', 'Net Area', 'Surface Count', 'R Value']
    area_units = 'ft^2'
    target_units = 'ft^2*h*R/Btu'
    surface_data[:units] = ['', area_units, '', target_units]
    surface_data[:data] = []

    # loop through surfaces to get constructions
    ext_const_base = {}
    model.getSurfaces.each do |surface|
      next if surface.outsideBoundaryCondition != 'Outdoors'
      if ext_const_base.include? surface.construction.get
        ext_const_base[surface.construction.get] += 1
      else
        ext_const_base[surface.construction.get] = 1
      end
    end
    ext_const_base.sort.each do |construction, count|
      net_area = construction.getNetArea
      net_area_ip = OpenStudio.convert(net_area, 'm^2', 'ft^2').get
      net_area_ip_neat = OpenStudio.toNeatString(net_area_ip, 0, true)
      surface_count = count
      if  construction.thermalConductance.is_initialized
        thermal_conductance = construction.thermalConductance.get
        source_units = 'm^2*K/W'
        r_value_ip = OpenStudio.convert(1 / thermal_conductance, source_units, target_units).get
        r_value_ip_neat = OpenStudio.toNeatString(r_value_ip, 2, true)
      else
        r_value_ip_neat = ''
      end
      surface_data[:data] << [construction.name, net_area_ip_neat, surface_count, r_value_ip_neat]
      runner.registerValue(construction.name.to_s.downcase.gsub(" ","_"), net_area_ip, area_units)
    end
    envelope_tables << surface_data

    # summary of exterior constructions used in the model for sub surfaces
    sub_surface_data = {}
    sub_surface_data[:title] = 'Sub Surface Constructions'
    sub_surface_data[:header] = ['Construction', 'Area', 'Surface Count', 'U-Factor']
    area_units = 'ft^2'
    u_factor_units = 'Btu/ft^2*h*R'
    sub_surface_data[:units] = ['', area_units, '', u_factor_units]
    sub_surface_data[:data] = []

    # loop through sub_surfaces to get constructions
    ext_const_sub = {}
    model.getSubSurfaces.each do |sub_surface|
      next if sub_surface.outsideBoundaryCondition != 'Outdoors'
      if ext_const_sub.include? sub_surface.construction.get
        ext_const_sub[sub_surface.construction.get] += 1
      else
        ext_const_sub[sub_surface.construction.get] = 1
      end
    end
    ext_const_sub.sort.each do |construction, count|
      net_area = construction.getNetArea
      net_area_ip = OpenStudio.convert(net_area, 'm^2', 'ft^2').get
      net_area_ip_neat = OpenStudio.toNeatString(net_area_ip, 0, true)
      surface_count = count
      source_units = 'm^2*K/W'
      target_units = 'ft^2*h*R/Btu'
      if construction.uFactor.is_initialized
        u_factor = construction.uFactor.get
        u_factor_ip = 1 / OpenStudio.convert(1 / u_factor, source_units, target_units).get
        u_factor_ip_neat = OpenStudio.toNeatString(u_factor_ip, 4, true)
      else
        u_factor_ip_neat = ''
      end
      sub_surface_data[:data] << [construction.name, net_area_ip_neat, surface_count, u_factor_ip_neat]
      runner.registerValue(construction.name.to_s.downcase.gsub(" ","_"), net_area_ip, area_units)
    end
    envelope_tables << sub_surface_data

    # Conditioned Window-Wall Ratio and Skylight-Roof Ratio
    fenestration_data = {}
    fenestration_data[:title] = 'WWR & Skylight Ratio'
    fenestration_data[:header] = %w(Description Total North East South West)
    target_units = '%' # it is a bit odd, but eplusout.htm calls the tale ratio, but shows as percentage. I'll match that here for now.
    fenestration_data[:units] = ['', target_units, target_units, target_units, target_units, target_units]
    fenestration_data[:data] = []

    # create string for rows
    fenestrations = []
    fenestrations << 'Gross Window-Wall Ratio' # [%]

    # loop rows
    fenestrations.each do |fenestration|
      query0 = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='InputVerificationandResultsSummary' and TableName='Window-Wall Ratio' and RowName='#{fenestration}' and ColumnName='Total'"
      query1 = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='InputVerificationandResultsSummary' and TableName='Window-Wall Ratio' and RowName='#{fenestration}' and ColumnName='North (315 to 45 deg)'"
      query2 = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='InputVerificationandResultsSummary' and TableName='Window-Wall Ratio' and RowName='#{fenestration}' and ColumnName='East (45 to 135 deg)'"
      query3 = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='InputVerificationandResultsSummary' and TableName='Window-Wall Ratio' and RowName='#{fenestration}' and ColumnName='South (135 to 225 deg)'"
      query4 = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='InputVerificationandResultsSummary' and TableName='Window-Wall Ratio' and RowName='#{fenestration}' and ColumnName='West (225 to 315 deg)'"
      query5 = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='InputVerificationandResultsSummary' and TableName='Skylight-Roof Ratio'  and RowName='Skylight-Roof Ratio'"
      query6 = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='InputVerificationandResultsSummary' and TableName='Conditioned Window-Wall Ratio' and RowName='#{fenestration}' and ColumnName='Total'"
      query7 = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='InputVerificationandResultsSummary' and TableName='Conditioned Window-Wall Ratio' and RowName='#{fenestration}' and ColumnName='North (315 to 45 deg)'"
      query8 = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='InputVerificationandResultsSummary' and TableName='Conditioned Window-Wall Ratio' and RowName='#{fenestration}' and ColumnName='East (45 to 135 deg)'"
      query9 = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='InputVerificationandResultsSummary' and TableName='Conditioned Window-Wall Ratio' and RowName='#{fenestration}' and ColumnName='South (135 to 225 deg)'"
      query10 = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='InputVerificationandResultsSummary' and TableName='Conditioned Window-Wall Ratio' and RowName='#{fenestration}' and ColumnName='West (225 to 315 deg)'"

      total = sqlFile.execAndReturnFirstDouble(query0)
      north = sqlFile.execAndReturnFirstDouble(query1)
      east = sqlFile.execAndReturnFirstDouble(query2)
      south = sqlFile.execAndReturnFirstDouble(query3)
      west = sqlFile.execAndReturnFirstDouble(query4)
      skylight = sqlFile.execAndReturnFirstDouble(query5)
      total_cond = sqlFile.execAndReturnFirstDouble(query6)
      north_cond = sqlFile.execAndReturnFirstDouble(query7)
      east_cond = sqlFile.execAndReturnFirstDouble(query8)
      south_cond = sqlFile.execAndReturnFirstDouble(query9)
      west_cond = sqlFile.execAndReturnFirstDouble(query10)
      if total.empty? || north.empty? || east.empty? || south.empty? || west.empty? || total_cond.empty? || north_cond.empty? || east.empty? || south_cond.empty? || west_cond.empty? || skylight.empty?
        runner.registerWarning('Did not find value for Window or Skylight Ratio')
        return false
      else
        # add data
        display = fenestration
        fenestration_data[:data] << [display, total.get, north.get, east.get, south.get, west.get]
        fenestration_data[:data] << ["#{display} (Conditioned)", total_cond.get, north_cond.get, east_cond.get, south_cond.get, west_cond.get]
        runner.registerValue("#{display.downcase.gsub(" ","_")}", total.get, target_units)
        runner.registerValue("#{display.downcase.gsub(" ","_")}_conditioned", total_cond.get, target_units)

        # skylight
        # skylight seems to provide back percentage vs. fraction. Changing to fraction to match vertical fenestration.
        fenestration_data[:data] << ['Skylight-Roof Ratio', skylight.get, '', '', '', '']
        runner.registerValue('skylight_roof_ratio', skylight.get, target_units)

      end
    end

    envelope_tables << fenestration_data

    return @envelope_section
  end

  # create table for service water heating
  def self.water_use_section(model, sqlFile, runner, name_only = false)
    # water use equipment from model
    water_use_data = {}
    water_use_data[:title] = ''
    water_use_data[:header] = ['Instance', 'Plant Loop', 'Definition', 'Space', 'Peak Flow Rate', 'Flow Rate Schedule', 'Target Temp Range']
    source_units = 'm^3/s'
    target_units = 'gal/min'
    source_units_temp = 'C'
    target_units_temp = 'F'
    water_use_data[:units] = ['', '', '', '', target_units, '', target_units_temp]
    water_use_data[:data] = []

    # gather data for section
    @water_use_data_section = {}
    @water_use_data_section[:title] = 'Water Use Equipment'

    # stop here if only name is requested this is used to populate display name for arguments
    if name_only == true
      return @water_use_data_section
    end

    # loop through water use equipment
    water_use_equipment = model.getWaterUseEquipments
    water_use_equipment.sort.each do |instance|
      water_use_equipment_def = instance.waterUseEquipmentDefinition
      if instance.waterUseConnections.is_initialized && instance.waterUseConnections.get.plantLoop.is_initialized
        plant_loop = instance.waterUseConnections.get.plantLoop.get.name
      else
        plant_loop = ''
      end
      if instance.flowRateFractionSchedule.is_initialized
        water_use_equipment_flow_rate_sch = instance.flowRateFractionSchedule.get.name
      else
        water_use_equipment_flow_rate_sch = ''
      end
      if instance.space.is_initialized
        space = instance.space.get.name
      else
        space = ''
      end
      peak_flow_rate = water_use_equipment_def.peakFlowRate
      peak_flow_rate_ip = OpenStudio.convert(peak_flow_rate, source_units, target_units).get
      peak_flow_rate_ip_neat = OpenStudio.toNeatString(peak_flow_rate_ip, 2, true)
      if water_use_equipment_def.targetTemperatureSchedule.is_initialized
        target_temp_sch = water_use_equipment_def.targetTemperatureSchedule.get
        schedule_values = OsLib_Schedules.getMinMaxAnnualProfileValue(model, target_temp_sch)
        if not schedule_values.nil?
          min_ip = OpenStudio.convert(schedule_values['min'], 'C', 'F').get
          max_ip = OpenStudio.convert(schedule_values['max'], 'C', 'F').get
          target_temp_range = "#{min_ip.round(1)} to #{max_ip.round(1)}"
        else
          target_temp_range = "can't inspect schedule."
        end
      else
        target_temp_range = ''
      end
      water_use_data[:data] << [instance.name, plant_loop, water_use_equipment_def.name, space, peak_flow_rate_ip_neat, water_use_equipment_flow_rate_sch, target_temp_range]
      runner.registerValue(instance.name.to_s.downcase.gsub(" ","_"), peak_flow_rate_ip, target_units)
    end

    # don't create empty table
    if water_use_data[:data].size > 0
      @water_use_data_section[:tables] = [water_use_data] # only one table for this section
    else
      @water_use_data_section[:tables] = []
    end

    return @water_use_data_section
  end

  # create table for exterior lights
  def self.exterior_light_section(model, sqlFile, runner, name_only = false)
    # Exterior Lighting from output
    ext_light_data = {}
    ext_light_data[:title] = ''
    ext_light_data[:header] = ['Description', 'Total Power', 'Consumption']
    power_units = 'W'
    consumption_units = 'kWh'
    ext_light_data[:units] = ['', power_units, consumption_units]
    ext_light_data[:data] = []

    # gather data for section
    @ext_light_data_section = {}
    @ext_light_data_section[:title] = 'Exterior Lighting'

    # stop here if only name is requested this is used to populate display name for arguments
    if name_only == true
      return @ext_light_data_section
    end

    query0 = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='LightingSummary' and TableName='Exterior Lighting'  and RowName='Exterior Lighting Total' and ColumnName='Total Watts'"
    query1 = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='LightingSummary' and TableName='Exterior Lighting'  and RowName='Exterior Lighting Total' and ColumnName='Consumption'"
    total_watts = sqlFile.execAndReturnFirstDouble(query0)
    consumption = sqlFile.execAndReturnFirstDouble(query1)
    if total_watts.empty? || consumption.empty?
      runner.registerWarning('Did not find value for Exterior Lighting Total.')
      return false
    else
      # add data
      total_watts_ip = total_watts.get
      consumption_ip = consumption.get * 0.2778 # value * energy conversion GJ to kWh todo - use real conversion
      total_watts_ip_neat = OpenStudio.toNeatString(total_watts_ip, 2, true)
      consumption_ip_neat = OpenStudio.toNeatString(consumption_ip, 2, true)
      ext_light_data[:data] << ['Exterior Lighting Total', total_watts_ip_neat, consumption_ip_neat]
      runner.registerValue('exterior_lighting_total_power', total_watts_ip, power_units)
      runner.registerValue('exterior_lighting_total_consumption', consumption_ip, consumption_units)
    end

    # don't create empty table
    if total_watts.to_f > 0 || consumption.to_f > 0
      @ext_light_data_section[:tables] = [ext_light_data] # only one table for this section
    else
      @ext_light_data_section[:tables] = []
    end

    return @ext_light_data_section
  end

  # create table for elevators
  # TODO: - update this to be custom load with user supplied string (or strings)
  def self.elevator_data_table(model, sqlFile, runner, name_only = false)
    # elevators from model
    @elevator_data = {}
    @elevator_data[:title] = 'Elevators'
    @elevator_data[:header] = ['Instance', 'Definition', 'Thermal Zone', 'Power Per Elevator', 'Units', 'Count']
    @elevator_data[:data] = []
    elec_equip_instances = model.getElectricEquipments
    elec_equip_instances.sort.each do |instance|
      elec_equip_def = instance.electricEquipmentDefinition

      # see if it is expected and valid object
      next if elec_equip_def.name.to_s != 'ElevatorElecEquipDef'
      unless instance.space.is_initialized
        runner.registerWarning("#{instance.name} doesn't have a space.")
        next
      end

      # get other data
      elev_space = instance.space.get
      elev_zone = elev_space.thermalZone.get # should check this
      elev_power = elec_equip_def.designLevel.get # should check this
      elev_power_neat = OpenStudio.toNeatString(elev_power, 0, true)
      units = 'W'
      count = instance.multiplier

      @elevator_data[:data] << [instance.name.to_s, elec_equip_def.name, elev_zone.name.get, elev_power_neat, units, OpenStudio.toNeatString(count, 2, true)]
      runner.registerValue(instance.name.to_s.downcase.gsub(" ","_"), elev_power, units)
    end

    return @elevator_data
  end

  # create table of space type details
  def self.space_type_details_section(model, sqlFile, runner, name_only = false)
    # array to hold tables
    output_data_space_type_detail_tables = []

    # gather data for section
    @output_data_space_type_section = {}
    @output_data_space_type_section[:title] = 'Space Type Summary'
    @output_data_space_type_section[:tables] = output_data_space_type_detail_tables

    # stop here if only name is requested this is used to populate display name for arguments
    if name_only == true
      return @output_data_space_type_section
    end

    # loop through space types
    model.getSpaceTypes.sort.each do |spaceType|
      next if spaceType.floorArea == 0

      # get floor area
      floor_area_si = spaceType.floorArea

      # create variable for number of people
      num_people = nil

      # gather list of spaces and zones in space type
      zone_name_list = []
      space_name_list = []
      spaceType.spaces.each do |space|
        # grabspace and zone names
        space_name_list << space.name.to_s
        if space.thermalZone.is_initialized
          zone_name_list << space.thermalZone.get.name.to_s
        end
      end
      # output_data_space_type_details[:data] << [space_name_list.uniq.join(","),space_name_list.uniq.size,"Spaces",""]
      # output_data_space_type_details[:data] << [zone_name_list.uniq.join(","),zone_name_list.uniq.size,"Thermal Zones",""]

      # space type details data output
      output_data_space_type_details = {}
      output_data_space_type_details[:title] = "#{spaceType.name}<br>(#{space_name_list.uniq.size} spaces and #{zone_name_list.uniq.size} thermal zones)"
      output_data_space_type_details[:header] = ['Definition', 'Value', 'Unit', 'Inst. Multiplier']
      output_data_space_type_details[:units] = []  # won't use this for these tables since units change
      output_data_space_type_details[:data] = []

      # data for space type details
      instances = spaceType.internalMass
      instances.each do |instance|
        def_display = instance.definition.name
        if instance.surfaceArea.is_initialized && instance.surfaceArea.get > 0
          def_value = OpenStudio.convert(instance.surfaceArea.get, 'm^2', 'ft^2').get
          def_value_neat = OpenStudio.toNeatString(def_value, 0, true)
          def_units = 'ft^2'
        elsif instance.surfaceAreaPerFloorArea.is_initialized && instance.surfaceAreaPerFloorArea.get > 0
          def_value = instance.surfaceAreaPerFloorArea.get
          def_value_neat = OpenStudio.toNeatString(def_value, 0, true)
          def_units = 'ft^2/floor area ft^2'
        elsif instance.surfaceAreaPerPerson.is_initialized && instance.surfaceAreaPerPerson.get > 0
          def_value = OpenStudio.convert(instance.surfaceAreaPerPerson.get, 'm^2', 'ft^2').get
          def_value_neat = OpenStudio.toNeatString(def_value, 0, true)
          def_units = 'ft^2/person'
        end
        count = instance.multiplier
        output_data_space_type_details[:data] << [def_display, def_value_neat, def_units, count]
      end

      instances = spaceType.people
      instances.each do |instance|
        def_display = instance.definition.name
        if instance.numberOfPeople.is_initialized && instance.numberOfPeople.get > 0
          def_value = instance.numberOfPeople.get
          def_value_neat = OpenStudio.toNeatString(def_value, 0, true)
          def_units = 'people'
        elsif instance.peoplePerFloorArea.is_initialized && instance.peoplePerFloorArea.get > 0
          def_value = instance.peoplePerFloorArea.get / OpenStudio.convert(1, 'm^2', 'ft^2').get
          def_value_neat = OpenStudio.toNeatString(def_value, 4, true)
          def_units = 'people/ft^2'
        elsif instance.spaceFloorAreaPerPerson.is_initialized && instance.spaceFloorAreaPerPerson.get > 0
          def_value = OpenStudio.convert(instance.spaceFloorAreaPerPerson.get, 'm^2', 'ft^2').get
          def_value_neat = OpenStudio.toNeatString(def_value, 0, true)
          def_units = 'ft^2/person'
        end
        count = instance.multiplier
        output_data_space_type_details[:data] << [def_display, def_value_neat, def_units, count]
      end

      instances = spaceType.electricEquipment
      instances.each do |instance|
        def_display = instance.definition.name
        if instance.designLevel.is_initialized && instance.designLevel.get > 0
          def_value = instance.designLevel.get
          def_value_neat = OpenStudio.toNeatString(def_value, 0, true)
          def_units = 'W'
        elsif instance.powerPerFloorArea.is_initialized && instance.powerPerFloorArea.get > 0
          def_value = instance.powerPerFloorArea.get / OpenStudio.convert(1, 'm^2', 'ft^2').get
          def_value_neat = OpenStudio.toNeatString(def_value, 4, true)
          def_units = 'W/ft^2'
        elsif instance.powerPerPerson .is_initialized && instance.powerPerPerson .get > 0
          def_value = OpenStudio.convert(instance.powerPerPerson .get, 'm^2', 'ft^2').get
          def_value_neat = OpenStudio.toNeatString(def_value, 0, true)
          def_units = 'W/person'
        end
        count = instance.multiplier
        output_data_space_type_details[:data] << [def_display, def_value_neat, def_units, count]
      end

      instances = spaceType.gasEquipment
      instances.each do |instance|
        def_display = instance.definition.name
        if instance.designLevel.is_initialized && instance.designLevel.get > 0
          def_value = instance.designLevel.get
          def_value_neat = OpenStudio.toNeatString(def_value, 0, true)
          def_units = 'W'
        elsif instance.powerPerFloorArea.is_initialized && instance.powerPerFloorArea.get > 0
          def_value = instance.powerPerFloorArea.get / OpenStudio.convert(1, 'm^2', 'ft^2').get
          def_value_neat = OpenStudio.toNeatString(def_value, 4, true)
          def_units = 'W/ft^2'
        elsif instance.powerPerPerson .is_initialized && instance.powerPerPerson .get > 0
          def_value = OpenStudio.convert(instance.powerPerPerson .get, 'm^2', 'ft^2').get
          def_value_neat = OpenStudio.toNeatString(def_value, 0, true)
          def_units = 'W/person'
        end
        count = instance.multiplier
        output_data_space_type_details[:data] << [def_display, def_value_neat, def_units, count]
      end

      instances = spaceType.lights
      instances.each do |instance|
        def_display = instance.definition.name
        if instance.lightingLevel.is_initialized && instance.lightingLevel.get > 0
          def_value = instance.lightingLevel.get
          def_value_neat = OpenStudio.toNeatString(def_value, 0, true)
          def_units = 'W'
        elsif instance.powerPerFloorArea.is_initialized && instance.powerPerFloorArea.get > 0
          def_value = instance.powerPerFloorArea.get / OpenStudio.convert(1, 'm^2', 'ft^2').get
          def_value_neat = OpenStudio.toNeatString(def_value, 4, true)
          def_units = 'W/ft^2'
        elsif instance.powerPerPerson .is_initialized && instance.powerPerPerson .get > 0
          def_value = OpenStudio.convert(instance.powerPerPerson .get, 'm^2', 'ft^2').get
          def_value_neat = OpenStudio.toNeatString(def_value, 0, true)
          def_units = 'W/person'
        end
        count = instance.multiplier
        output_data_space_type_details[:data] << [def_display, def_value_neat, def_units, count]
      end

      instances = spaceType.spaceInfiltrationDesignFlowRates
      instances.each do |instance|
        instance_display = instance.name
        if instance.designFlowRate.is_initialized
          inst_value = OpenStudio.convert(instance.designFlowRate.get, 'm^3/s', 'ft^3/min').get
          inst_value_neat = OpenStudio.toNeatString(inst_value, 4, true)
          inst_units = 'cfm'
          count = ''
          output_data_space_type_details[:data] << [instance_display, inst_value_neat, inst_units, count]
        end
        if instance.flowperSpaceFloorArea.is_initialized
          inst_value = OpenStudio.convert(instance.flowperSpaceFloorArea.get, 'm/s', 'ft/min').get
          inst_value_neat = OpenStudio.toNeatString(inst_value, 4, true)
          inst_units = 'cfm/ floor area ft^2'
          count = ''
          output_data_space_type_details[:data] << [instance_display, inst_value_neat, inst_units, count]
        end
        if instance.flowperExteriorSurfaceArea.is_initialized
          inst_value = OpenStudio.convert(instance.flowperExteriorSurfaceArea.get, 'm/s', 'ft/min').get
          inst_value_neat = OpenStudio.toNeatString(inst_value, 4, true)
          inst_units = 'cfm/ext surf area ft^2'
          count = ''
          output_data_space_type_details[:data] << [instance_display, inst_value_neat, inst_units, count]
        end
        if instance.flowperExteriorWallArea.is_initialized # uses same input as exterior surface area but different calc method
          inst_value = OpenStudio.convert(instance.flowperExteriorWallArea.get, 'm/s', 'ft/min').get
          inst_value_neat = OpenStudio.toNeatString(inst_value, 4, true)
          inst_units = 'cfm/ext wall area ft^2'
          count = ''
          output_data_space_type_details[:data] << [instance_display, inst_value_neat, inst_units, count]
        end
        if instance.airChangesperHour.is_initialized
          inst_value = instance.airChangesperHour.get
          inst_value_neat = OpenStudio.toNeatString(inst_value, 4, true)
          inst_units = 'ach'
          count = ''
          output_data_space_type_details[:data] << [instance_display, inst_value_neat, inst_units, count]
        end
      end

      if spaceType.designSpecificationOutdoorAir.is_initialized
        instance = spaceType.designSpecificationOutdoorAir.get
        instance_display = instance.name
        if instance.to_DesignSpecificationOutdoorAir.is_initialized
          instance = instance.to_DesignSpecificationOutdoorAir.get
          outdoor_air_method = instance.outdoorAirMethod
          count = ''

          # calculate and report various methods
          if instance.outdoorAirFlowperPerson > 0
            inst_value = OpenStudio.convert(instance.outdoorAirFlowperPerson, 'm^3/s', 'ft^3/min').get
            inst_value_neat = OpenStudio.toNeatString(inst_value, 4, true)
            inst_units = 'cfm/person'
            output_data_space_type_details[:data] << ["#{instance_display} (outdoor air method #{outdoor_air_method})", inst_value_neat, inst_units, count]
          end
          if instance.outdoorAirFlowperFloorArea > 0
            inst_value = OpenStudio.convert(instance.outdoorAirFlowperFloorArea, 'm/s', 'ft/min').get
            inst_value_neat = OpenStudio.toNeatString(inst_value, 4, true)
            inst_units = 'cfm/floor area ft^2'
            output_data_space_type_details[:data] << ["#{instance_display} (outdoor air method #{outdoor_air_method})", inst_value_neat, inst_units, count]
          end
          if instance.outdoorAirFlowRate > 0
            inst_value = OpenStudio.convert(instance.outdoorAirFlowRate, 'm^3/s', 'ft^3/min').get
            inst_value_neat = OpenStudio.toNeatString(inst_value, 4, true)
            inst_units = 'cfm'
            output_data_space_type_details[:data] << ["#{instance_display} (outdoor air method #{outdoor_air_method})", inst_value_neat, inst_units, count]
          end
          if instance.outdoorAirFlowAirChangesperHour > 0
            inst_value = instance.outdoorAirFlowAirChangesperHour
            inst_value_neat = OpenStudio.toNeatString(inst_value, 4, true)
            inst_units = 'ach'
            output_data_space_type_details[:data] << ["#{instance_display} (outdoor air method #{outdoor_air_method})", inst_value_neat, inst_units, count]
          end

        end
      end

      # add table to array of tables
      output_data_space_type_detail_tables << output_data_space_type_details
    end

    return @output_data_space_type_section
  end

  # create template section
  def self.weather_summary_table(model, sqlFile, runner)
    # data for query
    report_name = 'InputVerificationandResultsSummary'
    table_name = 'General'
    columns = ['', 'Value']
    rows = ['Weather File', 'Latitude', 'Longitude', 'Elevation', 'Time Zone', 'North Axis Angle']

    # create table
    table = {}
    table[:title] = 'Weather Summary'
    table[:header] = columns
    table[:units] = []
    table[:data] = []

    # run query and populate table
    rows.each do |row|
      row_data = [row]
      column_counter = -1
      table[:header].each do |header|
        column_counter += 1
        next if header == ''
        query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='#{report_name}' and TableName='#{table_name}' and RowName= '#{row}' and ColumnName= '#{header}'"
        results = sqlFile.execAndReturnFirstString(query) # this is first time I needed string vs. double for weather file
        # TODO: - would be nice to get units from first column
        if row == "Elevation" then results = "#{OpenStudio::convert(results.get.to_f,"m","ft").get.round} (ft)" end
        #if row == "Elevation" then results = "#{results.class} (f)" end
        row_data << results
      end

      table[:data] << row_data
    end

    # add in climate zone from OpenStudio model
    # get ashrae climate zone from model
    climate_zone = ''
    climateZones = model.getClimateZones
    climateZones.climateZones.each do |climateZone|
      if climateZone.institution == "ASHRAE"
        climate_zone = climateZone.value
        next
      end
    end

    table[:data] << ['ASHRAE Climate Zone',climate_zone]

    return table
  end

  # create design_day_table
  def self.design_day_table(model, sqlFile, runner)
    # data for query
    report_name = 'ClimaticDataSummary'
    table_name = 'SizingPeriod:DesignDay'
    columns = ['', 'Maximum Dry Bulb', 'Daily Temperature Range', 'Humidity Value', 'Humidity Type', 'Wind Speed', 'Wind Direction']

    # populate dynamic rows
    rows_name_query = "SELECT DISTINCT  RowName FROM tabulardatawithstrings WHERE ReportName='#{report_name}' and TableName='#{table_name}'"
    row_names = sqlFile.execAndReturnVectorOfString(rows_name_query).get
    rows = []
    row_names.each do |row_name|
      rows << row_name
    end

    # create table
    table = {}
    table[:title] = 'Sizing Period Design Days'
    table[:header] = columns
    table[:source_units] = ['', 'C', 'K', '', '', 'm/s', '']
    table[:units] = ['', 'F', 'R', '', '', 'mph', '']
    table[:data] = []

    # run query and populate table
    rows.each do |row|
      row_data = [row]
      column_counter = -1
      table[:header].each do |header|
        column_counter += 1
        next if header == ''
        query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='#{report_name}' and TableName='#{table_name}' and RowName= '#{row}' and ColumnName= '#{header}'"
        if header == 'Humidity Type'
          results = sqlFile.execAndReturnFirstString(query).get
          results = results.gsub('[C]', '[F]')
          results = results.gsub('[J/kg]', '[Btu/lb]')
          # any other types?
        elsif header == 'Humidity Value'
          results = sqlFile.execAndReturnFirstDouble(query).get
          # get humidity units for conversion
          query_humidity_type = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='#{report_name}' and TableName='#{table_name}' and RowName= '#{row}' and ColumnName= 'Humidity Type'"
          results_units = sqlFile.execAndReturnFirstString(query_humidity_type).get
          if results_units.include?('[C]')
            results = OpenStudio.convert(results, 'C', 'F').get.round(2)
          elsif results_units.include?('[J/kg]')
            results = OpenStudio.convert(results, 'J/kg', 'Btu/lb').get.round(2)
            # any other types?
          end
        else
          results_si = sqlFile.execAndReturnFirstDouble(query).get
          results = OpenStudio.convert(results_si, table[:source_units][column_counter], table[:units][column_counter]).get.round(2)
        end
        row_data << results
      end

      table[:data] << row_data
    end

    return table
  end

  # create template section
  def self.building_performance_table(model, sqlFile, runner)
    # create a second table
    building_performance_table = {}
    building_performance_table[:title] = 'Building Performance'
    building_performance_table[:header] = %w(Description Value)
    building_performance_table[:units] = []
    building_performance_table[:data] = []

    # add rows to table
    # building_performance_table[:data] << ["Vanilla",1.5]

    return building_performance_table
  end

  # create template section
  def self.site_performance_table(model, sqlFile, runner)
    # create a second table
    site_performance_table = {}
    site_performance_table[:title] = 'Site Performance'
    site_performance_table[:header] = %w(Description Value)
    site_performance_table[:units] = []
    site_performance_table[:data] = []

    # add rows to table
    # site_performance_table[:data] << ["Vanilla",1.5]

    return site_performance_table
  end

  # create template section
  def self.site_power_generation_table(model, sqlFile, runner)
    # create a second table
    site_power_generation_table = {}
    site_power_generation_table[:title] = 'Renewable Energy Source Summary'
    site_power_generation_table[:header] = ['', 'Rated Capacity', 'Annual Energy Generated']
    site_power_generation_table[:source_units] = ['', 'kW', 'GJ']
    site_power_generation_table[:units] = ['', 'kW', 'kWh']
    site_power_generation_table[:data] = []

    # create string for LEED advisories
    rows = []
    rows << 'Photovoltaic'
    rows << 'Wind'

    # loop through advisory messages
    value_found = false
    rows.each do |row|
      row_data = [row]
      column_counter = -1
      site_power_generation_table[:header].each do |header|
        column_counter += 1
        next if column_counter == 0
        query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='LEEDsummary' and RowName= '#{row}' and ColumnName='#{header}';"
        data = sqlFile.execAndReturnFirstDouble(query).get
        data_ip = OpenStudio.convert(data, site_power_generation_table[:source_units][column_counter], site_power_generation_table[:units][column_counter]).get
        if data > 0 then value_found = true end

        row_data << data_ip.round(2)
      end
      site_power_generation_table[:data] << row_data
    end

    if value_found
      return site_power_generation_table
    else
      return false
    end
  end

  # create template section
  def self.monthly_overview_section(model, sqlFile, runner, name_only = false)
    # array to hold tables
    monthly_tables = []

    # gather data for section
    @monthly_overview_section = {}
    @monthly_overview_section[:title] = 'Monthly Overview'
    @monthly_overview_section[:tables] = monthly_tables

    # stop here if only name is requested this is used to populate display name for arguments
    if name_only == true
      return @monthly_overview_section
    end

    # end use colors by index
    end_use_colors = ['#EF1C21', '#0071BD', '#F7DF10', '#DEC310', '#4A4D4A', '#B5B2B5', '#FF79AD', '#632C94', '#F75921', '#293094', '#CE5921', '#FFB239', '#29AAE7', '#8CC739']

    # sorted end use array to pass in for stacked bar chart order
    end_use_order = []
    month_order = %w(Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec)

    # loop through fuels for consumption tables
    OpenStudio::EndUseFuelType.getValues.each do |fuel_type|
      # get fuel type and units
      fuel_type = OpenStudio::EndUseFuelType.new(fuel_type).valueDescription
      if fuel_type == 'Electricity'
        units = "\"kWh\""
        unit_str = 'kWh'
      else
        units = "\"Million Btu\""
        unit_str = 'MBtu'
      end

      # create table
      monthly_fuel = {}
      monthly_fuel[:title] = "#{fuel_type} Consumption (#{unit_str})"
      monthly_fuel[:header] = ['', 'Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec', 'Total']
      monthly_fuel[:units] = []
      monthly_fuel[:data] = []
      monthly_fuel[:chart_type] = 'vertical_stacked_bar'
      monthly_fuel[:chart_attributes] = { value: monthly_fuel[:title], label_x: 'Month', sort_yaxis: end_use_order, sort_xaxis: month_order }
      monthly_fuel[:chart] = []

      # has to hold monthly totals for fuel
      monthly_total = {}

      # rest counter for each fuel type
      site_energy_use = 0.0
      fuel_type_aggregation = 0.0

      # loop through end uses
      OpenStudio::EndUseCategoryType.getValues.each do |category_type|
        category_str = OpenStudio::EndUseCategoryType.new(category_type).valueDescription
        end_use_order << category_str
        row_data = [category_str]
        fuel_and_category_aggregation = 0.0

        OpenStudio::MonthOfYear.getValues.each do |month|
          if month >= 1 && month <= 12
            if !sqlFile.energyConsumptionByMonth(OpenStudio::EndUseFuelType.new(fuel_type),
                                                 OpenStudio::EndUseCategoryType.new(category_type),
                                                 OpenStudio::MonthOfYear.new(month)).empty?
              valInJ = sqlFile.energyConsumptionByMonth(OpenStudio::EndUseFuelType.new(fuel_type),
                                                        OpenStudio::EndUseCategoryType.new(category_type),
                                                        OpenStudio::MonthOfYear.new(month)).get
              fuel_and_category_aggregation += valInJ
              valInUnits = OpenStudio.convert(valInJ, 'J', unit_str).get

              # do we want to register every value?
              # month_str = OpenStudio::MonthOfYear.new(month).valueDescription
              # prefix_str = OpenStudio::toUnderscoreCase("#{fuel_type}_#{category_str}_#{month_str}")
              # runner.registerValue("#{prefix_str.downcase.gsub(" ","_")}_ip",valInUnits,unit_str)

              # populate hash for monthly totals
              month = monthly_fuel[:header][month]
              if monthly_total[month]
                monthly_total[month] += valInJ
              else
                monthly_total[month] = valInJ
              end

              monthly_fuel[:chart] << JSON.generate(label: category_str, label_x: month, value: valInUnits, color: end_use_colors[category_type])
              # for some reason sometimes 0 comes through here, show as blank of 0
              if valInUnits > 0
                row_data << valInUnits.round(2)
              else
                row_data << ''
              end

            else
              row_data << ''
              # populate hash for monthly totals
              month = monthly_fuel[:header][month]
              if monthly_total[month]
                # do nothing
              else
                monthly_total[month] = 0.0
              end

            end
          end
        end

        prefix_str = OpenStudio.toUnderscoreCase("#{fuel_type}_#{category_str}")
        runner.registerValue("#{prefix_str}_ip", OpenStudio.convert(fuel_and_category_aggregation, 'J', unit_str).get, unit_str)

        fuel_type_aggregation += fuel_and_category_aggregation
        row_total = OpenStudio.convert(fuel_and_category_aggregation, 'J', unit_str).get
        if row_total == 0
          row_data << ''
        else
          row_data << row_total.round(2)
        end
        monthly_fuel[:data] << row_data
      end

      runner.registerValue(OpenStudio.toUnderscoreCase("#{fuel_type}_ip"),
                           OpenStudio.convert(fuel_type_aggregation, 'J', unit_str).get,
                           unit_str)
      site_energy_use += fuel_type_aggregation

      # add row for totals
      row_data = ['Total']
      monthly_total.each do |k, v|
        if OpenStudio.convert(v, 'J', unit_str).get == 0
          row_data << ''
        else
          row_data << OpenStudio.convert(v, 'J', unit_str).get.round(2)
        end
      end

      table_total = OpenStudio.convert(site_energy_use, 'J', unit_str).get.round(2)
      row_data << table_total
      monthly_fuel[:data] << row_data

      # add table to array of tables if table total is > 0
      if table_total > 0
        monthly_tables << monthly_fuel
      end
    end

    # loop through fuels for peak demand tables
    OpenStudio::EndUseFuelType.getValues.each do |fuel_type|
      # get fuel type and units
      fuel_type = OpenStudio::EndUseFuelType.new(fuel_type).valueDescription
      if fuel_type == 'Electricity'
        unit_str = 'kW'
      else
        unit_str = 'kBtu/hr' # TODO: - update units
      end

      # create table
      monthly_fuel = {}
      monthly_fuel[:title] = "#{fuel_type} Peak Demand (#{unit_str})"
      monthly_fuel[:header] = ['', 'Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec']
      monthly_fuel[:units] = []
      monthly_fuel[:data] = []
      monthly_fuel[:chart_type] = 'vertical_stacked_bar'
      monthly_fuel[:chart_attributes] = { value: monthly_fuel[:title], label_x: 'Month', sort_yaxis: end_use_order, sort_xaxis: month_order }
      monthly_fuel[:chart] = []

      # has to hold monthly totals for fuel
      monthly_total = {}

      # test for non 0 value in table
      value_found = false

      # loop through end uses
      OpenStudio::EndUseCategoryType.getValues.each do |category_type|
        category_str = OpenStudio::EndUseCategoryType.new(category_type).valueDescription
        row_data = [category_str]

        OpenStudio::MonthOfYear.getValues.each do |month|
          if month >= 1 && month <= 12
            if !sqlFile.peakEnergyDemandByMonth(OpenStudio::EndUseFuelType.new(fuel_type),
                                                OpenStudio::EndUseCategoryType.new(category_type),
                                                OpenStudio::MonthOfYear.new(month)).empty?
              valInJ = sqlFile.peakEnergyDemandByMonth(OpenStudio::EndUseFuelType.new(fuel_type),
                                                       OpenStudio::EndUseCategoryType.new(category_type),
                                                       OpenStudio::MonthOfYear.new(month)).get
              valInUnits = OpenStudio.convert(valInJ, 'W', unit_str).get

              # populate hash for monthly totals
              month = monthly_fuel[:header][month]
              if monthly_total[month]
                monthly_total[month] += valInJ
              else
                monthly_total[month] = valInJ
              end

              monthly_fuel[:chart] << JSON.generate(label: category_str, label_x: month, value: valInUnits, color: end_use_colors[category_type])
              # for some reason sometimes 0 comes through here, show as blank of 0
              if valInUnits > 0
                row_data << valInUnits.round(4)
                value_found = true
              else
                row_data << ''
              end

            else
              row_data << ''
              month = monthly_fuel[:header][month]
              if monthly_total[month]
                # do nothing
              else
                monthly_total[month] = 0.0
              end

            end
          end
        end

        monthly_fuel[:data] << row_data
      end

      # add row for totals
      row_data = ['Total']
      monthly_total.each do |k, v|
        if OpenStudio.convert(v, 'W', unit_str).get == 0
          row_data << ''
        else
          row_data << OpenStudio.convert(v, 'W', unit_str).get.round(2)
        end
      end

      monthly_fuel[:data] << row_data

      # add table if value found
      if value_found
        monthly_tables << monthly_fuel
      end
    end

    return @monthly_overview_section
  end

  # create utility section
  def self.utility_bills_rates_section(model, sqlFile, runner, name_only = false)
    # array to hold tables
    utility_bills_rates_tables = []

    # gather data for section
    @utility_bills_rates_section = {}
    @utility_bills_rates_section[:title] = 'Utility Bills/Rates'
    @utility_bills_rates_section[:tables] = utility_bills_rates_tables

    # stop here if only name is requested this is used to populate display name for arguments
    if name_only == true
      return @utility_bills_rates_section
    end

    # create table
    utility_table = {}
    utility_table[:title] = 'Energy Type Summary'
    utility_table[:header] = ['', 'Utility Rate', 'Average Rate', 'Units of Energy', 'Units of Demand']
    utility_table[:query_column] = ['', 'Utility Rate', 'Virtual Rate', 'Units of Energy', 'Units of Demand']
    utility_table[:units] = ['', '', '$/unit energy', '', '']
    utility_table[:data] = []

    # populate dynamic rows
    rows_name_query = "SELECT DISTINCT  RowName FROM tabulardatawithstrings WHERE ReportName='LEEDsummary' and TableName='EAp2-3. Energy Type Summary'"
    row_names = sqlFile.execAndReturnVectorOfString(rows_name_query).get
    rows = []
    row_names.each do |row_name|
	  next if row_name == 'Other' # other currently mixes water and dist htg/clg. Don't want to show that
      rows << row_name
    end

    # loop through rows
    value_found = false
    rows.each do |row|
      data = [row]
      utility_table[:query_column].each do |header|
        next if header == ''
        query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='LEEDsummary'  and TableName='EAp2-3. Energy Type Summary' and RowName= '#{row}' and ColumnName='#{header}';"
        data << sqlFile.execAndReturnFirstString(query).get
        if sqlFile.execAndReturnFirstString(query).get.to_f > 0
          value_found = true
        end
      end
      utility_table[:data] << data
    end

    # add table to array of tables
    if value_found
      utility_bills_rates_tables << utility_table
    end

    # create table
    energy_cost_table = {}
    energy_cost_table[:title] = 'Energy Cost Summary'
    energy_cost_table[:header] = ['', 'Process Subtotal', 'Total Energy Cost']
    energy_cost_table[:units] = ['', '$', '$']
    energy_cost_table[:data] = []

    # populate dynamic rows
    rows_name_query = "SELECT DISTINCT  RowName FROM tabulardatawithstrings WHERE ReportName='LEEDsummary' and TableName='EAp2-7. Energy Cost Summary'"
    row_names = sqlFile.execAndReturnVectorOfString(rows_name_query).get
    rows = []
    row_names.each do |row_name|
      rows << row_name
    end

    # loop through rows
    value_found = false
    rows.each do |row|
      data = [row]
      energy_cost_table[:header].each do |header|
        next if header == ''
        query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='LEEDsummary' and tableName = 'EAp2-7. Energy Cost Summary' and RowName= '#{row}' and ColumnName='#{header}';"
        data << sqlFile.execAndReturnFirstDouble(query).get
        if sqlFile.execAndReturnFirstDouble(query).get > 0
          value_found = true
        end
      end
      energy_cost_table[:data] << data
    end

    # add table to array of tables
    if value_found
      utility_bills_rates_tables << energy_cost_table
    end

    return @utility_bills_rates_section
  end

  # create unmet horus
  def self.zone_condition_section(model, sqlFile, runner, name_only = false)
    # array to hold tables
    zone_condition_tables = []

    # gather data for section
    @zone_condition_section = {}
    @zone_condition_section[:title] = 'Zone Conditions'
    @zone_condition_section[:tables] = zone_condition_tables

    # stop here if only name is requested this is used to populate display name for arguments
    if name_only == true
      return @zone_condition_section
    end

    temperature_bins_temps_ip = [56, 61, 66, 68, 70, 72, 74, 76, 78, 83, 88]
    temperature_bins_temps_si = []
    temperature_bins_temps_ip.each do |i|
      temperature_bins_temps_si << OpenStudio.convert(i, 'F', 'C').get
    end

    # hash to store hours
    temperature_bins = {}
    for i in 0..(temperature_bins_temps_ip.size - 1)
      if i == 0
        temperature_bins["< #{temperature_bins_temps_ip[i]}"] = 0
      elsif i == temperature_bins_temps_ip.size - 1
        temperature_bins[">= #{temperature_bins_temps_ip[i]}"] = 0
      else
        temperature_bins["#{temperature_bins_temps_ip[i - 1]}-#{temperature_bins_temps_ip[i]}"] = 0
      end
    end

    # create table
    temperature_table = {}
    temperature_table[:title] = 'Temperature (Table values represent hours spent in each temperature range)'
    temperature_table[:header] = ['Zone', 'Unmet Htg', 'Unmet Htg - Occ']
    temperature_bins.each do |k, v|
      temperature_table[:header] << k
    end
    temperature_table[:header] += ['Unmet Clg','Unmet Clg - Occ', 'Mean Temp']
    temperature_table[:units] = ['', 'hr','hr']
    temperature_bins.each do |k, v|
      temperature_table[:units] << 'F'
    end
    temperature_table[:units] += ['hr','hr','F']
    temperature_table[:data] = []
    temperature_table[:data_color] = []

    # get time series data for each zone
    ann_env_pd = OsLib_Reporting.ann_env_pd(sqlFile)
    if ann_env_pd

      # get keys
      keys = sqlFile.availableKeyValues(ann_env_pd, 'Hourly', 'Zone Air Temperature')
      keys.each do |key|
        # reset bin values
        temperature_bins.each do |k, v|
          temperature_bins[k] = 0
        end

        # get desired variable
        output_timeseries = sqlFile.timeSeries(ann_env_pd, 'Hourly', 'Zone Air Temperature', key)
        # loop through timeseries and move the data from an OpenStudio timeseries to a normal Ruby array (vector)
        if output_timeseries.is_initialized # checks to see if time_series exists
          output_timeseries = output_timeseries.get.values
          temp_counter = 0
          temp_sum = 0
          for i in 0..(output_timeseries.size - 1)
            # add to counter and sum
            temp_counter += 1
            temp_sum += output_timeseries[i]

            found_bin = false
            for j in 0..(temperature_bins_temps_si.size - 1)
              if found_bin == false && output_timeseries[i] < temperature_bins_temps_si[j]
                temperature_bins[temperature_bins.keys[j]] += 1
                found_bin = true
              end
            end
          end # end of for i in 0..(output_timeseries.size - 1)
        else
          runner.registerWarning("Didn't find data for Zone Air Temperature")
        end # end of if output_timeseries.is_initialized

        # get unmet hours for each zone from tabular data
        query_htg = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='SystemSummary' and TableName = 'Time Setpoint Not Met' and RowName= '#{key}' and ColumnName='During Heating';"
        unmet_htg = sqlFile.execAndReturnFirstDouble(query_htg).get
        query_clg = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='SystemSummary' and TableName = 'Time Setpoint Not Met' and RowName= '#{key}' and ColumnName='During Cooling';"
        unmet_clg = sqlFile.execAndReturnFirstDouble(query_clg).get
        query_htg_occ = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='SystemSummary' and TableName = 'Time Setpoint Not Met' and RowName= '#{key}' and ColumnName='During Occupied Heating';"
        unmet_htg_occ = sqlFile.execAndReturnFirstDouble(query_htg_occ).get
        query_clg_occ = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='SystemSummary' and TableName = 'Time Setpoint Not Met' and RowName= '#{key}' and ColumnName='During Occupied Cooling';"
        unmet_clg_occ = sqlFile.execAndReturnFirstDouble(query_clg_occ).get

        # get mean temp
        mean = OpenStudio.convert(temp_sum / temp_counter.to_f, 'C', 'F').get

        # add rows to table
        row_data = [key, unmet_htg.round,unmet_htg_occ.round]
        row_color = ['','','']
        temperature_bins.each do |k, v|
          row_data << v
          if v > 2000
            row_color << 'indianred'
          elsif v > 1000
            row_color << 'orange'
          elsif v > 500
            row_color << 'yellow'
          else
            row_color << ''
          end
        end
        row_data += [unmet_clg.round, unmet_clg_occ.round, "#{mean.round(1)} (F)"]
        row_color += ['','','']
        temperature_table[:data] << row_data
        temperature_table[:data_color] << row_color
      end
    else
      runner.registerWarning('An annual simulation was not run. Cannot get annual timeseries data')
      return false
    end

    # add table to array of tables
    zone_condition_tables << temperature_table

    humidity_bins_temps_ip = [30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80]
    humidity_bins_temps_si = humidity_bins_temps_ip

    # hash to store hours
    humidity_bins = {}
    for i in 0..(humidity_bins_temps_ip.size - 1)
      if i == 0
        humidity_bins["< #{humidity_bins_temps_ip[i]}"] = 0
      elsif i == humidity_bins_temps_ip.size - 1
        humidity_bins[">= #{humidity_bins_temps_ip[i]}"] = 0
      else
        humidity_bins["#{humidity_bins_temps_ip[i - 1]}-#{humidity_bins_temps_ip[i]}"] = 0
      end
    end

    # create table
    humidity_table = {}
    humidity_table[:title] = 'Humidity (Table values represent hours spent in each Humidity range)'
    humidity_table[:header] = ['Zone']
    humidity_bins.each do |k, v|
      humidity_table[:header] << k
    end
    humidity_table[:header] += ['Mean Relative Humidity']
    humidity_table[:units] = ['']
    humidity_bins.each do |k, v|
      humidity_table[:units] << '%'
    end
    humidity_table[:units] += ['%']
    humidity_table[:data] = []
    humidity_table[:data_color] = []

    # get time series data for each zone
    ann_env_pd = OsLib_Reporting.ann_env_pd(sqlFile)
    if ann_env_pd

      # get keys
      keys = sqlFile.availableKeyValues(ann_env_pd, 'Hourly', 'Zone Air Relative Humidity')
      keys.each do |key|
        # reset bin values
        humidity_bins.each do |k, v|
          humidity_bins[k] = 0
        end

        # get desired variable
        output_timeseries = sqlFile.timeSeries(ann_env_pd, 'Hourly', 'Zone Air Relative Humidity', key)
        # loop through timeseries and move the data from an OpenStudio timeseries to a normal Ruby array (vector)
        if output_timeseries.is_initialized # checks to see if time_series exists
          output_timeseries = output_timeseries.get.values
          humidity_counter = 0
          humidity_sum = 0
          for i in 0..(output_timeseries.size - 1)
            # add to counter and sum
            humidity_counter += 1
            humidity_sum += output_timeseries[i]

            found_bin = false
            for j in 0..(humidity_bins_temps_si.size - 1)
              if found_bin == false && output_timeseries[i] < humidity_bins_temps_si[j]
                humidity_bins[humidity_bins.keys[j]] += 1
                found_bin = true
              end
            end
          end # end of for i in 0..(output_timeseries.size - 1)
        else
          runner.registerWarning("Didn't find data for Zone Air Relative Humidity")
        end # end of if output_timeseries.is_initialized

        # get mean humidity
        mean = humidity_sum / humidity_counter.to_f

        # add rows to table
        row_data = [key]
        row_color = ['']
        humidity_bins.each do |k, v|
          row_data << v
          if v > 2000
            row_color << 'indianred'
          elsif v > 1000
            row_color << 'orange'
          elsif v > 500
            row_color << 'yellow'
          else
            row_color << ''
          end
        end
        row_data += ["#{mean.round(1)} (%)"]
        row_color += ['']
        humidity_table[:data] << row_data
        humidity_table[:data_color] << row_color
      end
    else
      runner.registerWarning('An annual simulation was not run. Cannot get annual timeseries data')
      return false
    end

    # add table to array of tables
    zone_condition_tables << humidity_table

    return @zone_condition_section
  end

  # create interior_lighting section
  def self.interior_lighting_section(model, sqlFile, runner, name_only = false)
    # array to hold tables
    interior_lighting_tables = []

    # gather data for section
    @interior_lighting_section = {}
    @interior_lighting_section[:title] = 'Interior Lighting Summary'
    @interior_lighting_section[:tables] = interior_lighting_tables

    # stop here if only name is requested this is used to populate display name for arguments
    if name_only == true
      return @interior_lighting_section
    end

    # data for query
    report_name = 'LightingSummary'
    table_name = 'Interior Lighting'
    columns = ['', 'Zone', 'Lighting Power Density', 'Total Power', 'Schedule Name', 'Scheduled Hours/Week', 'Actual Load Hours/Week', 'Return Air Fraction', 'Consumption']
    columns_query = ['', 'Zone', 'Lighting Power Density', 'Total Power', 'Schedule Name', 'Scheduled Hours/Week', 'Full Load Hours/Week', 'Return Air Fraction', 'Consumption']

    # populate dynamic rows
    rows_name_query = "SELECT DISTINCT  RowName FROM tabulardatawithstrings WHERE ReportName='#{report_name}' and TableName='#{table_name}'"
    row_names = sqlFile.execAndReturnVectorOfString(rows_name_query).get
    rows = []
    row_names.each do |row_name|
      next if row_name == 'Interior Lighting Total' # skipping this on purpose, may give odd results in some instances
      rows << row_name
    end

    # create table
    table = {}
    table[:title] = ''
    table[:header] = columns
    source_units_lpd = 'W/m^2'
    source_units_energy = 'GJ'
    target_units_lpd = 'W/ft^2'
    target_units_energy = 'kWh'
    table[:source_units] = ['', '', source_units_lpd, 'W', '', 'hr', 'hr', '', source_units_energy] # used for conversation, not needed for rendering.
    table[:units] = ['', '', target_units_lpd, 'W', '', 'hr', 'hr', '', target_units_energy]
    table[:data] = []

    # run query and populate table
    rows.each do |row|
      row_data = [row]
      column_counter = -1
      columns_query.each do |header|
        column_counter += 1
        next if header == ''
        query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='#{report_name}' and TableName='#{table_name}' and RowName= '#{row}' and ColumnName= '#{header}'"
        if not table[:source_units][column_counter] == ''
          results = sqlFile.execAndReturnFirstDouble(query)
          row_data_ip = OpenStudio.convert(results.to_f, table[:source_units][column_counter], table[:units][column_counter]).get
          row_data << row_data_ip.round(2)
        else
          results = sqlFile.execAndReturnFirstString(query)
          row_data << results
        end
      end

      table[:data] << row_data
    end

    # add table to array of tables
    interior_lighting_tables << table

    return @interior_lighting_section
  end

  # create plug_loads section
  def self.plug_loads_section(model, sqlFile, runner, name_only = false)
    # array to hold tables
    plug_loads_tables = []

    # gather data for section
    @plug_loads_section = {}
    @plug_loads_section[:title] = 'Plug Loads Summary'
    @plug_loads_section[:tables] = plug_loads_tables

    # stop here if only name is requested this is used to populate display name for arguments
    if name_only == true
      return @plug_loads_section
    end

    # data for query
    report_name = 'EnergyMeters'
    table_name = 'Annual and Peak Values - Electricity'
    columns = ['', 'Electricity Annual Value'] # TODO: - would be nice to make this more like lighting summary

    # populate dynamic rows
    rows_name_query = "SELECT DISTINCT  RowName FROM tabulardatawithstrings WHERE ReportName='#{report_name}' and TableName='#{table_name}'"
    row_names = sqlFile.execAndReturnVectorOfString(rows_name_query).get
    rows = []
    row_names.each do |row_name|
      next unless row_name.include?('InteriorEquipment:Electricity:Zone:')
      rows << row_name
    end

    # create table
    table = {}
    table[:title] = 'Electric Plug Load Consumption'
    table[:header] = columns
    source_units_energy = 'GJ'
    target_units_energy = 'kWh'
    table[:source_units] = ['', source_units_energy] # used for conversation, not needed for rendering.
    table[:units] = ['', target_units_energy]
    table[:data] = []

    # run query and populate table
    rows.each do |row|
      row_data = [row]
      column_counter = -1
      table[:header].each do |header|
        column_counter += 1
        next if header == ''
        query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='#{report_name}' and TableName='#{table_name}' and RowName= '#{row}' and ColumnName= '#{header}'"
        results = sqlFile.execAndReturnFirstDouble(query)
        row_data_ip = OpenStudio.convert(results.to_f, table[:source_units][column_counter], table[:units][column_counter]).get
        row_data << row_data_ip.round(2)
      end

      table[:data] << row_data
    end

    # add table to array of tables
    if table[:data].size > 0 then plug_loads_tables << table end

    # data for query
    # TODO: - need to test this in model with gas equipment
    report_name = 'EnergyMeters'
    table_name = 'Annual and Peak Values - Gas'
    columns = ['', 'Gas Annual Value'] # TODO: - would be nice to make this more like lighting summary

    # populate dynamic rows
    rows_name_query = "SELECT DISTINCT  RowName FROM tabulardatawithstrings WHERE ReportName='#{report_name}' and TableName='#{table_name}'"
    row_names = sqlFile.execAndReturnVectorOfString(rows_name_query).get
    rows = []
    row_names.each do |row_name|
      next unless row_name.include?('InteriorEquipment:Gas:Zone:')
      rows << row_name
    end

    # create table
    table = {}
    table[:title] = 'Gas Plug Load Consumption'
    table[:header] = columns
    source_units_energy = 'GJ'
    target_units_energy = 'kBtu'
    table[:source_units] = ['', source_units_energy] # used for conversation, not needed for rendering.
    table[:units] = ['', target_units_energy]
    table[:data] = []

    # run query and populate table
    rows.each do |row|
      row_data = [row]
      column_counter = -1
      table[:header].each do |header|
        column_counter += 1
        next if header == ''
        query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='#{report_name}' and TableName='#{table_name}' and RowName= '#{row}' and ColumnName= '#{header}'"
        results = sqlFile.execAndReturnFirstDouble(query)
        row_data_ip = OpenStudio.convert(results.to_f, table[:source_units][column_counter], table[:units][column_counter]).get
        row_data << row_data_ip.round(2)
      end

      table[:data] << row_data
    end

    # add table to array of tables
    if table[:data].size > 0 then plug_loads_tables << table end

    return @plug_loads_section
  end

  # create unmet horus
  def self.hvac_load_profile(model, sqlFile, runner, name_only = false)
    # array to hold tables
    hvac_load_profile_tables = []

    # gather data for section
    @hvac_load_profile_section = {}
    @hvac_load_profile_section[:title] = 'HVAC Load Profiles'
    @hvac_load_profile_section[:tables] = hvac_load_profile_tables

    # stop here if only name is requested this is used to populate display name for arguments
    if name_only == true
      return @hvac_load_profile_section
    end

    month_order = %w(Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec)

    # create table
    hvac_load_profile_monthly_table = {}
    hvac_load_profile_monthly_table[:title] = 'Monthly Load Profiles'
    hvac_load_profile_monthly_table[:header] = ['', 'Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec']
    hvac_load_profile_monthly_table[:units] = []
    hvac_load_profile_monthly_table[:data] = []
    hvac_load_profile_monthly_table[:chart_type] = 'vertical_grouped_bar_with_comp_line'
    hvac_load_profile_monthly_table[:chart_attributes] = { value_left: 'Cooling/Heating Load (MBtu)', label_x: 'Month', value_right: 'Average Outdoor Air Dry Bulb (F)', sort_xaxis: month_order }
    hvac_load_profile_monthly_table[:chart] = []

    # hash to store monthly values
    cooling_monthly = { 'Jan' => 0, 'Feb' => 0, 'Mar' => 0, 'Apr' => 0, 'May' => 0, 'Jun' => 0, 'Jul' => 0, 'Aug' => 0, 'Sep' => 0, 'Oct' => 0, 'Nov' => 0, 'Dec' => 0 }
    heating_monthly = { 'Jan' => 0, 'Feb' => 0, 'Mar' => 0, 'Apr' => 0, 'May' => 0, 'Jun' => 0, 'Jul' => 0, 'Aug' => 0, 'Sep' => 0, 'Oct' => 0, 'Nov' => 0, 'Dec' => 0 }

    # units for conversion
    source_units = 'J'
    target_units = 'MBtu'

    # loop through fuel types
    OpenStudio::EndUseFuelType.getValues.each do |fuel_type|
      OpenStudio::MonthOfYear.getValues.each do |month|
        if month >= 1 && month <= 12

          # get cooling value for this fuel and month
          if !sqlFile.energyConsumptionByMonth(OpenStudio::EndUseFuelType.new(fuel_type),
                                               OpenStudio::EndUseCategoryType.new('Cooling'),
                                               OpenStudio::MonthOfYear.new(month)).empty?
            cooling_valInJ = sqlFile.energyConsumptionByMonth(OpenStudio::EndUseFuelType.new(fuel_type),
                                                              OpenStudio::EndUseCategoryType.new('Cooling'),
                                                              OpenStudio::MonthOfYear.new(month)).get

            cooling_valInUnits = OpenStudio.convert(cooling_valInJ, source_units, target_units).get

          else
            cooling_valInUnits = 0
          end

          # get heating value for this fuel and month
          if !sqlFile.energyConsumptionByMonth(OpenStudio::EndUseFuelType.new(fuel_type),
                                               OpenStudio::EndUseCategoryType.new('Heating'),
                                               OpenStudio::MonthOfYear.new(month)).empty?
            heating_valInJ = sqlFile.energyConsumptionByMonth(OpenStudio::EndUseFuelType.new(fuel_type),
                                                              OpenStudio::EndUseCategoryType.new('Heating'),
                                                              OpenStudio::MonthOfYear.new(month)).get

            heating_valInUnits = OpenStudio.convert(heating_valInJ, source_units, target_units).get

          else
            heating_valInUnits = 0
          end

          # create or add to hash to sum across fuel types
          month = hvac_load_profile_monthly_table[:header][month]
          if cooling_monthly.key?(month)
            cooling_monthly[month] = cooling_monthly[month].to_f + cooling_valInUnits
          else
            cooling_monthly[month] = cooling_valInUnits
          end
          if heating_monthly.key?(month)
            heating_monthly[month] = heating_monthly[month].to_f + heating_valInUnits
          else
            heating_monthly[month] = heating_monthly
          end

        end
      end
    end

    # populate dry bulb data
    dry_bulb_monthly = ['Average Outdoor Air Dry Bulb (F)']
    ann_env_pd = OsLib_Reporting.ann_env_pd(sqlFile)
    if ann_env_pd
      # get desired variable
      output_timeseries = sqlFile.timeSeries(ann_env_pd, 'Monthly', 'Site Outdoor Air Drybulb Temperature', 'Environment')
      # loop through timeseries and move the data from an OpenStudio timeseries to a normal Ruby array (vector)
      if output_timeseries.is_initialized # checks to see if time_series exists

        # see if filler needed at start or end of table/chart
        num_blanks_start = output_timeseries.get.dateTimes[0].date.monthOfYear.value - 2
        num_blanks_end = 12 - output_timeseries.get.values.size - num_blanks_start

        # fill in blank data for partial year simulations
        for i in 0..(num_blanks_start-1)
          month = hvac_load_profile_monthly_table[:header][i + 1]
          dry_bulb_monthly << ''
        end

        output_timeseries = output_timeseries.get.values
        for i in 0..(output_timeseries.size - 1)
          month = hvac_load_profile_monthly_table[:header][i + 1 + num_blanks_start]
          value = OpenStudio.convert(output_timeseries[i], 'C', 'F').get
          dry_bulb_monthly << value.round(1)
          hvac_load_profile_monthly_table[:chart] << JSON.generate(label: 'Outdoor Temp', label_x: month, value2: value, color: 'green')
        end # end of for i in 0..(output_timeseries.size - 1)

        # fill in blank data for partial year simulations
        for i in 0..(num_blanks_end-1)
          month = hvac_load_profile_monthly_table[:header][i]
          dry_bulb_monthly << ''
        end

      else
        runner.registerWarning("Didn't find data for Site Outdoor Air Drybulb Temperature")
      end # end of if output_timeseries.is_initialized
    else
      runner.registerWarning('An annual simulation was not run.  Cannot get annual timeseries data')
      return false
    end

    # populate tables
    hvac_load_profile_monthly_table[:data] << dry_bulb_monthly
    cooling_array = ['Cooling Load (MBtu)']
    cooling_monthly.each do |k, v|
      cooling_array << v.round(2)

      # populate chart
      hvac_load_profile_monthly_table[:chart] << JSON.generate(label: 'Cooling Load', label_x: k, value: v, color: '#0071BD')
    end
    hvac_load_profile_monthly_table[:data] << cooling_array

    heating_array = ['Heating Load (MBtu)']
    heating_monthly.each do |k, v|
      heating_array << v.round(2)

      # populate chart
      hvac_load_profile_monthly_table[:chart] << JSON.generate(label: 'Heating Load', label_x: k, value: v, color: '#EF1C21')
    end
    hvac_load_profile_monthly_table[:data] << heating_array

    # add table to array of tables
    hvac_load_profile_tables << hvac_load_profile_monthly_table

    # create table
    hvac_part_load_profile_table = {}
    hvac_part_load_profile_table[:title] = 'Part Load Profiles'
    hvac_part_load_profile_table[:header] = ['Load', 'Clg: Cutoff', 'Clg: Hours', 'Clg: Hours', 'Htg: Cutoff', 'Htg: Hours', 'Htg: Hours']
    hvac_part_load_profile_table[:units] = ['%', 'MBtu', '%', 'hr', 'MBtu', '%', 'hr']
    hvac_part_load_profile_table[:data] = []

    # add rows to table
    hvac_part_load_profile_table[:data] << ['0-5', '', '', '', '', '', '']
    hvac_part_load_profile_table[:data] << ['5-10', '', '', '', '', '', '']
    hvac_part_load_profile_table[:data] << ['10-25', '', '', '', '', '', '']
    hvac_part_load_profile_table[:data] << ['15-20', '', '', '', '', '', '']
    hvac_part_load_profile_table[:data] << ['20-25', '', '', '', '', '', '']
    hvac_part_load_profile_table[:data] << ['25-30', '', '', '', '', '', '']
    hvac_part_load_profile_table[:data] << ['30-35', '', '', '', '', '', '']
    hvac_part_load_profile_table[:data] << ['35-40', '', '', '', '', '', '']
    hvac_part_load_profile_table[:data] << ['40-45', '', '', '', '', '', '']
    hvac_part_load_profile_table[:data] << ['45-50', '', '', '', '', '', '']
    hvac_part_load_profile_table[:data] << ['50-55', '', '', '', '', '', '']
    hvac_part_load_profile_table[:data] << ['55-60', '', '', '', '', '', '']
    hvac_part_load_profile_table[:data] << ['60-65', '', '', '', '', '', '']
    hvac_part_load_profile_table[:data] << ['65-70', '', '', '', '', '', '']
    hvac_part_load_profile_table[:data] << ['70-75', '', '', '', '', '', '']
    hvac_part_load_profile_table[:data] << ['75-80', '', '', '', '', '', '']
    hvac_part_load_profile_table[:data] << ['80-85', '', '', '', '', '', '']
    hvac_part_load_profile_table[:data] << ['85-90', '', '', '', '', '', '']
    hvac_part_load_profile_table[:data] << ['90-95', '', '', '', '', '', '']
    hvac_part_load_profile_table[:data] << ['95-100', '', '', '', '', '', '']

    # TODO: - add table to array of tables
    # hvac_load_profile_tables << hvac_part_load_profile_table

    return @hvac_load_profile_section
  end

  def self.zone_summary_section(model, sqlFile, runner, name_only = false)
    # array to hold tables
    template_tables = []

    # gather data for section
    @zone_summary_section = {}
    @zone_summary_section[:title] = 'Zone Overview'
    @zone_summary_section[:tables] = template_tables

    # stop here if only name is requested this is used to populate display name for arguments
    if name_only == true
      return @zone_summary_section
    end

    # data for query
    report_name = 'InputVerificationandResultsSummary'
    table_name = 'Zone Summary'
    columns = ['', 'Area', 'Conditioned (Y/N)', 'Part of Total Floor Area (Y/N)', 'Volume', 'Multiplier', 'Gross Wall Area', 'Window Glass Area', 'Lighting', 'People', 'Plug and Process']

    # test looking at getting entire table to get rows
    # query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='#{report_name}' and TableName='#{table_name}'"
    # results = sqlFile.execAndReturnVectorOfString(query).get

    # populate dynamic rows
    rows_name_query = "SELECT DISTINCT  RowName FROM tabulardatawithstrings WHERE ReportName='#{report_name}' and TableName='#{table_name}'"
    row_names = sqlFile.execAndReturnVectorOfString(rows_name_query).get
    rows = []
    row_names.each do |row_name|
      rows << row_name
    end

    # rows = ['Total','Conditioned Total','Unconditioned Total','Not Part of Total']

    # create zone_summary_table
    zone_summary_table = {}
    zone_summary_table[:title] = table_name
    zone_summary_table[:header] = columns
    source_units_area = 'm^2'
    target_units_area = 'ft^2'
    source_units_area_per_person = 'm^2/person'
    target_units_area_per_person = 'ft^2/person'
    source_units_volume = 'm^3'
    target_units_volume = 'ft^3'
    source_units_pd = 'W/m^2'
    target_units_pd = 'W/ft^2'
    zone_summary_table[:units] = ['', target_units_area, '', '', target_units_volume, '', target_units_area, target_units_area, target_units_pd, target_units_area_per_person, target_units_pd]
    zone_summary_table[:source_units] = ['', source_units_area, '', '', source_units_volume, '', source_units_area, source_units_area, source_units_pd, source_units_area_per_person, source_units_pd] # used for conversation, not needed for rendering.
    zone_summary_table[:data] = []

    # run query and populate zone_summary_table
    rows.each do |row|
      row_data = [row]
      column_counter = -1
      zone_summary_table[:header].each do |header|
        column_counter += 1
        next if header == ''
        if header == 'Multiplier' then header = 'Multipliers' end # what we want to show is different than what is in E+ table
        query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='#{report_name}' and TableName='#{table_name}' and RowName= '#{row}' and ColumnName= '#{header}'"
        if not zone_summary_table[:source_units][column_counter] == ''
          results = sqlFile.execAndReturnFirstDouble(query)
          row_data_ip = OpenStudio.convert(results.to_f, zone_summary_table[:source_units][column_counter], zone_summary_table[:units][column_counter]).get
          row_data << row_data_ip.round(2)
        else
          results = sqlFile.execAndReturnFirstString(query)
          row_data << results
        end
      end

      zone_summary_table[:data] << row_data
    end

    # add zone_summary_table to array of tables
    template_tables << zone_summary_table

    # data for query
    report_name = 'HVACSizingSummary'
    table_01_name = 'Zone Sensible Cooling'
    table_02_name = 'Zone Sensible Heating'
    columns = ['', 'Heating/Cooling', 'Calculated Design Load', 'Design Load With Sizing Factor', 'Calculated Design Air Flow', 'Design Air Flow  With Sizing Factor', 'Date/Time Of Peak', 'Outdoor Temperature at Peak Load', 'Outdoor Humidity Ratio at Peak Load']
    columns_query = ['', 'Heating/Cooling', 'Calculated Design Load', 'User Design Load', 'Calculated Design Air Flow', 'User Design Air Flow', 'Date/Time Of Peak', 'Outdoor Temperature at Peak Load', 'Outdoor Humidity Ratio at Peak Load']

    # populate dynamic rows
    rows_name_query = "SELECT DISTINCT  RowName FROM tabulardatawithstrings WHERE ReportName='#{report_name}' and TableName='#{table_01_name}'"
    row_names = sqlFile.execAndReturnVectorOfString(rows_name_query).get
    rows = []
    row_names.each do |row_name|
      rows << row_name
    end

    # create zone_dd_table
    zone_dd_table = {}
    zone_dd_table[:title] = 'Zone Sensible Cooling and Heating Sensible Sizing'
    zone_dd_table[:header] = columns
    source_units_power = 'W'
    target_units_power_clg = 'ton'
    target_units_power_htg = 'kBtu/h'
    source_units_air_flow = 'm^3/s'
    target_units_air_flow = 'ft^3/min'
    source_units_temp = 'C'
    target_units_temp = 'F'
    zone_dd_table[:units] = ['', '', '', '', target_units_air_flow, target_units_air_flow, '', target_units_temp, 'lbWater/lbAir']
    zone_dd_table[:source_units] = ['', '', '', '', source_units_air_flow, source_units_air_flow, '', source_units_temp, 'lbWater/lbAir'] # used for conversation, not needed for rendering.
    zone_dd_table[:data] = []

    # run query and populate zone_dd_table
    rows.each do |row|
      # populate cooling row
      row_data = [row, 'Cooling']
      column_counter = -1
      columns_query.each do |header|
        column_counter += 1
        next if header == '' || header == 'Heating/Cooling'
        query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='#{report_name}' and TableName='#{table_01_name}' and RowName= '#{row}' and ColumnName= '#{header}'"
        if not zone_dd_table[:source_units][column_counter] == ''
          results = sqlFile.execAndReturnFirstDouble(query)
          row_data_ip = OpenStudio.convert(results.to_f, zone_dd_table[:source_units][column_counter], zone_dd_table[:units][column_counter]).get
          row_data << row_data_ip.round(2)
        elsif header == 'Calculated Design Load' || header == 'User Design Load'
          results = sqlFile.execAndReturnFirstDouble(query)
          row_data_ip = OpenStudio.convert(results.to_f, source_units_power, target_units_power_clg).get
          row_data << "#{row_data_ip.round(2)} (#{target_units_power_clg})"
        else
          results = sqlFile.execAndReturnFirstString(query)
          row_data << results
        end
      end
      zone_dd_table[:data] << row_data

      # populate heating row
      row_data = [row, 'Heating']
      column_counter = -1
      columns_query.each do |header|
        column_counter += 1
        next if header == '' || header == 'Heating/Cooling'
        query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='#{report_name}' and TableName='#{table_02_name}' and RowName= '#{row}' and ColumnName= '#{header}'"
        if not zone_dd_table[:source_units][column_counter] == ''
          results = sqlFile.execAndReturnFirstDouble(query)
          row_data_ip = OpenStudio.convert(results.to_f, zone_dd_table[:source_units][column_counter], zone_dd_table[:units][column_counter]).get
          row_data << row_data_ip.round(2)
        elsif header == 'Calculated Design Load' || header == 'User Design Load'
          results = sqlFile.execAndReturnFirstDouble(query)
          row_data_ip = OpenStudio.convert(results.to_f, source_units_power, target_units_power_htg).get
          row_data << "#{row_data_ip.round(2)} (#{target_units_power_htg})"
        else
          results = sqlFile.execAndReturnFirstString(query)
          row_data << results
        end
      end
      zone_dd_table[:data] << row_data
    end

    # add zone_dd_table to array of tables
    template_tables << zone_dd_table

    return @zone_summary_section
  end

  # create air_loop_summary section
  def self.air_loop_summary_section(model, sqlFile, runner, name_only = false)
    # array to hold tables
    air_loop_summary_tables = []

    # gather data for section
    @air_loop_summary_section = {}
    @air_loop_summary_section[:title] = 'Air Loops Summary'
    @air_loop_summary_section[:tables] = air_loop_summary_tables

    # stop here if only name is requested this is used to populate display name for arguments
    if name_only == true
      return @air_loop_summary_section
    end

    # create table
    air_loop_summary_table = {}
    air_loop_summary_table[:title] = 'Part load histograms for fans; graphical for annual, tabular for annual and monthly'
    air_loop_summary_table[:header] = []
    air_loop_summary_table[:units] = []
    air_loop_summary_table[:data] = []

    # add table to array of tables
    air_loop_summary_tables << air_loop_summary_table

    return @air_loop_summary_section
  end

  # create plant_loop_summary section
  def self.plant_loop_summary_section(model, sqlFile, runner, name_only = false)
    # array to hold tables
    plant_loop_summary_tables = []

    # gather data for section
    @outdoor_air_section = {}
    @outdoor_air_section[:title] = 'Plant Loops Summary'
    @outdoor_air_section[:tables] = plant_loop_summary_tables

    # stop here if only name is requested this is used to populate display name for arguments
    if name_only == true
      return @outdoor_air_section
    end

    # create table
    plant_loop_summary_table = {}
    plant_loop_summary_table[:title] = 'Part load histograms for chillers, boilers, pumps'
    plant_loop_summary_table[:header] = []
    plant_loop_summary_table[:units] = []
    plant_loop_summary_table[:data] = []

    # add table to array of tables
    plant_loop_summary_tables << plant_loop_summary_table

    return @outdoor_air_section
  end

  # create outdoor_air_section
  def self.outdoor_air_section(model, sqlFile, runner, name_only = false)
    # array to hold tables
    outdoor_air_section_tables = []

    # gather data for section
    @outdoor_air_section = {}
    @outdoor_air_section[:title] = 'Outdoor Air'
    @outdoor_air_section[:tables] = outdoor_air_section_tables

    # stop here if only name is requested this is used to populate display name for arguments
    if name_only == true
      return @outdoor_air_section
    end

    # data for query
    report_name = 'OutdoorAirSummary'
    table_name = 'Average Outdoor Air During Occupied Hours'
    min_table_name = 'Minimum Outdoor Air During Occupied Hours'
    columns = ['', 'Average Number of Occupants', 'Nominal Number of Occupants', 'Zone Volume', 'Avg. Mechanical Ventilation', 'Min. Mechanical Ventilation', 'Avg. Infiltration', 'Min. Infiltration', 'Avg. Simple Ventilation', 'Min. Simple Ventilation']

    # populate dynamic rows
    rows_name_query = "SELECT DISTINCT  RowName FROM tabulardatawithstrings WHERE ReportName='#{report_name}' and TableName='#{table_name}'"
    row_names = sqlFile.execAndReturnVectorOfString(rows_name_query).get
    rows = []
    row_names.each do |row_name|
      rows << row_name
    end

    # create table
    table = {}
    table[:title] = 'Average and Minimum Outdoor Air During Occupied Hours'
    table[:header] = columns
    source_units_volume = 'm^3'
    target_units_volume = 'ft^3'
    table[:units] = ['', '', '', target_units_volume, 'ach', 'ach', 'ach', 'ach','ach', 'ach']
    table[:source_units] = ['', '', '', source_units_volume, 'ach', 'ach', 'ach', 'ach', 'ach', 'ach']
    table[:data] = []

    # run query and populate table
    rows.each do |row|
      row_data = [row]
      column_counter = -1
      table[:header].each do |header|
        column_counter += 1
        next if header == ''
        if header.include? 'Avg. '
          query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='#{report_name}' and TableName='#{table_name}' and RowName= '#{row}' and ColumnName= '#{header.gsub('Avg. ', '')}'"
          results = sqlFile.execAndReturnFirstDouble(query)
          row_data_ip = OpenStudio.convert(results.to_f, table[:source_units][column_counter], table[:units][column_counter]).get
          row_data << row_data_ip.round(4)
        elsif header.include? 'Min. '
          query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='#{report_name}' and TableName='#{min_table_name}' and RowName= '#{row}' and ColumnName= '#{header.gsub('Min. ', '')}'"
          results = sqlFile.execAndReturnFirstDouble(query)
          row_data_ip = OpenStudio.convert(results.to_f, table[:source_units][column_counter], table[:units][column_counter]).get
          row_data << row_data_ip.round(4)
        elsif header == 'Zone Volume'
          query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='#{report_name}' and TableName='#{table_name}' and RowName= '#{row}' and ColumnName= '#{header}'"
          results = sqlFile.execAndReturnFirstDouble(query)
          row_data_ip = OpenStudio.convert(results.to_f, table[:source_units][column_counter], table[:units][column_counter]).get
          row_data << row_data_ip.round(0)
        else
          query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='#{report_name}' and TableName='#{table_name}' and RowName= '#{row}' and ColumnName= '#{header}'"
          results = sqlFile.execAndReturnFirstDouble(query)
          row_data_ip = OpenStudio.convert(results.to_f, table[:source_units][column_counter], table[:units][column_counter]).get
          row_data << row_data_ip.round(4)
        end
      end

      table[:data] << row_data
    end

    # add table to array of tables
    outdoor_air_section_tables << table
    return @outdoor_air_section
  end

  # create cost_summary_section
  def self.cost_summary_section(model, sqlFile, runner, name_only = false)
    # array to hold tables
    cost_summary_section_tables = []

    # gather data for section
    @cost_summary_section = {}
    @cost_summary_section[:title] = 'Cash Flow'
    @cost_summary_section[:tables] = cost_summary_section_tables

    # stop here if only name is requested this is used to populate display name for arguments
    if name_only == true
      return @cost_summary_section
    end

    # order for fuel types
    yaxis_order = ['Electricity Purchased', 'Natural Gas', 'District Heating', 'District Cooling', 'Additional', 'Water', 'Capital and O&M']

    # create table
    cost_summary_table = {}
    cost_summary_table[:title] = 'Annual Cash Flow <br>(Not adjusted for inflation or utility escalation)'
    cost_summary_table[:header] = ['Year', 'Capital and O&M', 'Electricity', 'Natural Gas', 'District Heating', 'District Cooling', 'Additional', 'Water']
    cost_summary_table[:units] = ['', '$', '$', '$', '$', '$', '$', '$']
    cost_summary_table[:data] = []
    cost_summary_table[:chart_type] = 'vertical_stacked_bar'
    cost_summary_table[:chart_attributes] = { value: 'Annual Cash Flow ($)', label_x: 'Date', sort_yaxis: yaxis_order }
    cost_summary_table[:chart] = []

    # inflation approach
    inf_appr_query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='Life-Cycle Cost Report' AND ReportForString='Entire Facility' AND TableName='Life-Cycle Cost Parameters' AND RowName='Inflation Approach' AND ColumnName='Value'"
    inf_appr = sqlFile.execAndReturnFirstString(inf_appr_query)
    if inf_appr.is_initialized
      if inf_appr.get == 'ConstantDollar'
        inf_appr = 'Constant Dollar'
      elsif inf_appr.get == 'CurrentDollar'
        inf_appr = 'Current Dollar'
      else
        runner.registerWarning("Inflation approach: #{inf_appr.get} not recognized")
        return false
      end
      runner.registerValue("inflation_approach", inf_appr)
    else
      runner.registerWarning('Could not determine inflation approach used')
      return false
    end

    # base year
    base_yr_query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='Life-Cycle Cost Report' AND ReportForString='Entire Facility' AND TableName='Life-Cycle Cost Parameters' AND RowName='Base Date' AND ColumnName='Value'"
    base_yr = sqlFile.execAndReturnFirstString(base_yr_query)
    if base_yr.is_initialized
      if base_yr.get.match(/\d\d\d\d/)
        base_yr = base_yr.get.match(/\d\d\d\d/)[0].to_f
      else
        runner.registerWarning("Could not determine the analysis start year from #{base_yr.get}")
        return false
      end
    else
      runner.registerWarning('Could not determine analysis start year')
      return false
    end

    # analysis length
    length_yrs_query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='Life-Cycle Cost Report' AND ReportForString='Entire Facility' AND TableName='Life-Cycle Cost Parameters' AND RowName='Length of Study Period in Years' AND ColumnName='Value'"
    length_yrs = sqlFile.execAndReturnFirstInt(length_yrs_query)
    if length_yrs.is_initialized
      length_yrs = length_yrs.get
      runner.registerValue("analysis_length", length_yrs, "yrs")
    else
      runner.registerWarning('Could not determine analysis length')
      return false
    end

    # record the cash flow in these hashes
    cap_cash_flow = {}
    om_cash_flow = {}
    energy_cash_flow = {}
    electricity_cash_flow = {}
    gas_cash_flow = {}
    water_cash_flow = {}
    tot_cash_flow = {}
    additional_cash_flow = {}

    data_cashFlow = []
    data_running_total = []
    running_total = 0

    color = []
    color << '#DDCC77' # Electricity
    color << '#999933' # Natural Gas
    color << '#AA4499' # Additional Fuel
    color << '#88CCEE' # District Cooling
    color << '#CC6677' # District Heating
    color << '#332288' # Water (not used here but is in cash flow chart)
    color << '#117733' # Capital and O&M (not used here but is in cash flow chart)

    # loop through each year and record the cash flow
    for i in 0..(length_yrs - 1) do
      new_yr = base_yr + i
      yr = "January         #{new_yr.round}" # note: two spaces removed from earlier version of sql file
      ann_cap_cash = 0.0
      ann_om_cash = 0.0
      ann_energy_cash = 0.0
      ann_electricity_cash = 0.0
      ann_gas_cash = 0.0
      ann_dist_htg_cash = 0.0
      ann_dist_clg_cash = 0.0
      ann_water_cash = 0.0
      ann_tot_cash = 0.0

      # capital cash flow
      cap_cash_query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='Life-Cycle Cost Report' AND ReportForString='Entire Facility' AND TableName='Capital Cash Flow by Category (Without Escalation)' AND RowName='#{yr}' AND ColumnName='Total'"
      cap_cash = sqlFile.execAndReturnFirstDouble(cap_cash_query)
      if cap_cash.is_initialized
        ann_cap_cash += cap_cash.get
        ann_tot_cash += cap_cash.get
      end

      # o&m cash flow (excluding utility costs)
      om_types = %w(Maintenance Repair Operation Replacement MinorOverhaul MajorOverhaul OtherOperational)
      om_types.each do |om_type|
        om_cash_query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='Life-Cycle Cost Report' AND ReportForString='Entire Facility' AND TableName='Operating Cash Flow by Category (Without Escalation)' AND RowName='#{yr}' AND ColumnName='#{om_type}'"
        om_cash = sqlFile.execAndReturnFirstDouble(om_cash_query)
        if om_cash.is_initialized
          ann_om_cash += om_cash.get
          ann_tot_cash += om_cash.get
        end
      end

      # energy cost cash flows (by fuel)
      electricity_purchased_query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='Life-Cycle Cost Report' AND ReportForString='Entire Facility' AND TableName='Energy Cost Cash Flows (Without Escalation)' AND RowName='#{yr}' AND ColumnName='ElectricityPurchased'"
      electricity_purchased = sqlFile.execAndReturnFirstDouble(electricity_purchased_query)
      if electricity_purchased.is_initialized
        ann_electricity_cash += electricity_purchased.get
      end

      gas_query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='Life-Cycle Cost Report' AND ReportForString='Entire Facility' AND TableName='Energy Cost Cash Flows (Without Escalation)' AND RowName='#{yr}' AND ColumnName='Gas'"
      gas = sqlFile.execAndReturnFirstDouble(gas_query)
      if gas.is_initialized
        ann_gas_cash += gas.get
      end

      dist_htg_query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='Life-Cycle Cost Report' AND ReportForString='Entire Facility' AND TableName='Energy Cost Cash Flows (Without Escalation)' AND RowName='#{yr}' AND ColumnName='DistrictHeating'"
      dist_htg = sqlFile.execAndReturnFirstDouble(dist_htg_query)
      if dist_htg.is_initialized
        ann_dist_htg_cash += dist_htg.get
      end

      dist_clg_query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='Life-Cycle Cost Report' AND ReportForString='Entire Facility' AND TableName='Energy Cost Cash Flows (Without Escalation)' AND RowName='#{yr}' AND ColumnName='DistrictCooling'"
      dist_clg = sqlFile.execAndReturnFirstDouble(dist_clg_query)
      if dist_clg.is_initialized
        ann_dist_clg_cash += dist_clg.get
      end

      water_query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='Life-Cycle Cost Report' AND ReportForString='Entire Facility' AND TableName='Energy Cost Cash Flows (Without Escalation)' AND RowName='#{yr}' AND ColumnName='Water'"
      water = sqlFile.execAndReturnFirstDouble(water_query)
      if water.is_initialized
        ann_water_cash += water.get
      end

      # energy cash flow
      energy_cash_query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='Life-Cycle Cost Report' AND ReportForString='Entire Facility' AND TableName='Operating Cash Flow by Category (Without Escalation)' AND RowName='#{yr}' AND ColumnName='Energy'"
      energy_cash = sqlFile.execAndReturnFirstDouble(energy_cash_query)
      if energy_cash.is_initialized
        ann_energy_cash += energy_cash.get
        ann_tot_cash += energy_cash.get
      end

      # water cash flow (not currently used but should be in future)
      water_cash_query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='Life-Cycle Cost Report' AND ReportForString='Entire Facility' AND TableName='Operating Cash Flow by Category (Without Escalation)' AND RowName='#{yr}' AND ColumnName='Water'"
      water_cash = sqlFile.execAndReturnFirstDouble(water_cash_query)
      if water_cash.is_initialized
        ann_water_cash += water_cash.get
        ann_tot_cash += water_cash.get
      end

      # log the values for this year
      cap_cash_flow[yr] = ann_cap_cash
      om_cash_flow[yr] = ann_om_cash
      electricity_cash_flow[yr] = ann_electricity_cash
      gas_cash_flow[yr] = ann_gas_cash
      energy_cash_flow[yr] = ann_energy_cash
      water_cash_flow[yr] = ann_water_cash
      tot_cash_flow[yr] = ann_tot_cash
      ann_additional_cash = ann_energy_cash - ann_electricity_cash - ann_gas_cash - ann_water_cash - ann_dist_htg_cash - ann_dist_clg_cash
      additional_cash_flow[yr] = ann_additional_cash

      # populate table row
      cost_summary_table[:data] << [yr, ann_cap_cash + ann_om_cash, ann_electricity_cash, ann_gas_cash, ann_dist_htg_cash, ann_dist_clg_cash, ann_additional_cash.round(2), ann_water_cash]

      # gather graph data
      if ann_electricity_cash > 0
        cost_summary_table[:chart] << JSON.generate(label: 'Electricity Purchased', label_x: yr, value: ann_electricity_cash, color: color[0])
      end
      if ann_gas_cash > 0
        cost_summary_table[:chart] << JSON.generate(label: 'Natural Gas', label_x: yr, value: ann_gas_cash, color: color[1])
      end
      if ann_additional_cash > 0
        cost_summary_table[:chart] << JSON.generate(label: 'Additional', label_x: yr, value: ann_additional_cash, color: color[2])
      end
      if ann_dist_clg_cash > 0
        cost_summary_table[:chart] << JSON.generate(label: 'District Cooling', label_x: yr, value: ann_dist_clg_cash, color: color[3])
      end
      if ann_dist_htg_cash > 0
        cost_summary_table[:chart] << JSON.generate(label: 'District Heating', label_x: yr, value: ann_dist_htg_cash, color: color[4])
      end
      if ann_water_cash > 0
        cost_summary_table[:chart] << JSON.generate(label: 'Water', label_x: yr, value: ann_water_cash, color: color[5])
      end
      if ann_cap_cash + ann_om_cash > 0
        cost_summary_table[:chart] << JSON.generate(label: 'Capital and O&M', label_x: yr, value: ann_cap_cash + ann_om_cash, color: color[6])
      end

      # gather running total data for line plot
      running_total += ann_tot_cash

    end # next year

    # add table to array of tables
    if running_total > 0
      cost_summary_section_tables << cost_summary_table
    else
      # don't make chart of no data to add to it.
      cost_summary_table[:chart] = []
    end

    return @cost_summary_section
  end

  # create source_energy_section
  def self.source_energy_section(model, sqlFile, runner, name_only = false)
    # array to hold tables
    source_energy_section_tables = []

    # gather data for section
    @source_energy_section = {}
    @source_energy_section[:title] = 'Site and Source Summary'
    @source_energy_section[:tables] = source_energy_section_tables

    # stop here if only name is requested this is used to populate display name for arguments
    if name_only == true
      return @source_energy_section
    end

    # data for query
    report_name = 'AnnualBuildingUtilityPerformanceSummary'
    table_name = 'Site and Source Energy'
    columns = ['', 'Total Energy', 'Energy Per Total Building Area', 'Energy Per Conditioned Building Area']
    rows = ['Total Site Energy', 'Net Site Energy', 'Total Source Energy', 'Net Source Energy']

    # create table
    source_energy_table = {}
    source_energy_table[:title] = table_name
    source_energy_table[:header] = columns
    source_units_total = 'GJ'
    source_units_area = 'MJ/m^2'
    target_units_total = 'kBtu'
    target_units_area = 'kBtu/ft^2'
    source_energy_table[:units] = ['', target_units_total, target_units_area, target_units_area]
    source_energy_table[:source_units] = ['', source_units_total, source_units_area, source_units_area] # used for conversation, not needed for rendering.
    source_energy_table[:data] = []

    # run query and populate table
    rows.each do |row|
      row_data = [row]
      column_counter = -1
      source_energy_table[:header].each do |header|
        column_counter += 1
        next if header == ''
        query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='#{report_name}' and TableName='#{table_name}' and RowName= '#{row}' and ColumnName= '#{header}'"
        results = sqlFile.execAndReturnFirstDouble(query)
        row_data_ip = OpenStudio.convert(results.to_f, source_energy_table[:source_units][column_counter], source_energy_table[:units][column_counter]).get
        row_data << row_data_ip.round(1)
      end
      source_energy_table[:data] << row_data
    end

    # add table to array of tables
    source_energy_section_tables << source_energy_table

    # data for query
    report_name = 'AnnualBuildingUtilityPerformanceSummary'
    table_name = 'Site to Source Energy Conversion Factors'
    columns = ['', 'Site=>Source Conversion Factor']
    rows = ['Electricity', 'Natural Gas', 'District Cooling', 'District Heating'] # TODO: - complete this and add logic to skip row if not used in model

    # create table
    source_energy_table = {}
    source_energy_table[:title] = table_name
    source_energy_table[:header] = columns
    source_energy_table[:units] = []
    source_energy_table[:data] = []

    # run query and populate table
    rows.each do |row|
      row_data = [row]
      column_counter = -1
      source_energy_table[:header].each do |header|
        column_counter += 1
        next if header == ''
        query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='#{report_name}' and TableName='#{table_name}' and RowName= '#{row}' and ColumnName= '#{header}'"
        results = sqlFile.execAndReturnFirstDouble(query).to_f
        row_data << results.round(3)
      end
      source_energy_table[:data] << row_data
    end

    # add table to array of tables
    source_energy_section_tables << source_energy_table

    return @source_energy_section
  end

  # create co2_and_other_emissions_section
  def self.co2_and_other_emissions_section(model, sqlFile, runner, name_only = false)
    # array to hold tables
    co2_and_other_emissions_section_tables = []

    # gather data for section
    @co2_and_other_emissions_section = {}
    @co2_and_other_emissions_section[:title] = 'CO2 and Other Emissions'
    @co2_and_other_emissions_section[:tables] = co2_and_other_emissions_section_tables

    # stop here if only name is requested this is used to populate display name for arguments
    if name_only == true
      return @co2_and_other_emissions_section
    end

    # data for query
    report_name = 'EnergyMeters'
    table_name = 'Annual and Peak Values - Other by Weight/Mass'
    columns = ['', 'Annual Value', 'Minimum Value', 'Timestamp of Minimum', 'Maximum Value', 'Timestamp of Maximum']
    rows = ['Carbon Equivalent:Facility', 'CarbonEquivalentEmissions:Carbon Equivalent']

    # create table
    table = {}
    table[:title] = table_name
    table[:header] = columns
    source_units_total = 'kg'
    source_units_rate = 'kg/s'
    target_units_total = 'lb'
    target_units_rate = 'lb/s'
    table[:units] = ['', target_units_total, target_units_rate, '', target_units_rate, '']
    table[:source_units] = ['', source_units_total, source_units_rate, '', source_units_rate, ''] # used for conversation, not needed for rendering.
    table[:data] = []

    # run query and populate table
    rows.each do |row|
      row_data = [row]
      column_counter = -1
      table[:header].each do |header|
        column_counter += 1
        next if header == ''
        query = "SELECT Value FROM tabulardatawithstrings WHERE ReportName='#{report_name}' and TableName='#{table_name}' and RowName= '#{row}' and ColumnName= '#{header}'"
        if not table[:source_units][column_counter] == ''
          results = sqlFile.execAndReturnFirstDouble(query)
          row_data_ip = OpenStudio.convert(results.to_f, table[:source_units][column_counter], table[:units][column_counter]).get
          row_data << row_data_ip.round(2)
        else
          results = sqlFile.execAndReturnFirstString(query)
          row_data << results
        end
      end

      table[:data] << row_data
    end

    # add table to array of tables
    co2_and_other_emissions_section_tables << table

    return @co2_and_other_emissions_section
  end

  # create typical_load_profiles_section
  def self.typical_load_profiles_section(model, sqlFile, runner, name_only = false)
    # array to hold tables
    typical_load_profiles_section_tables = []

    # gather data for section
    @typical_load_profiles_section = {}
    @typical_load_profiles_section[:title] = 'Typical Load Profiles'
    @typical_load_profiles_section[:tables] = typical_load_profiles_section_tables

    # stop here if only name is requested this is used to populate display name for arguments
    if name_only == true
      return @typical_load_profiles_section
    end

    # create table
    typical_load_profiles_table = {}
    typical_load_profiles_table[:title] = 'Content To Be Determined'
    typical_load_profiles_table[:header] = []
    typical_load_profiles_table[:units] = []
    typical_load_profiles_table[:data] = []

    # add table to array of tables
    typical_load_profiles_section_tables << typical_load_profiles_table

    return @typical_load_profiles_section
  end

  # create schedules_overview_section
  def self.schedules_overview_section(model, sqlFile, runner, name_only = false)
    # array to hold tables
    schedules_overview_section_tables = []

    # gather data for section
    @schedules_overview_section = {}
    @schedules_overview_section[:title] = 'Schedule Overview'
    @schedules_overview_section[:tables] = schedules_overview_section_tables

    # stop here if only name is requested this is used to populate display name for arguments
    if name_only == true
      return @schedules_overview_section
    end

    # create table
    schedules_overview_table = {}
    schedules_overview_table[:title] = ''
    schedules_overview_table[:header] = ['Schedule', 'Type Limits', 'Rules', 'Use Count']
    schedules_overview_table[:units] = []
    schedules_overview_table[:data] = []
    schedules_overview_table[:chart_type] = 'multi_step_line_grid'
    schedules_overview_table[:chart] = [] # for this chart type, this contains an array of charts

    # add table to array of tables
    schedules_overview_section_tables << schedules_overview_table

    model.getSchedules.sort.each do |schedule|
      next unless schedule.to_ScheduleRuleset.is_initialized
      schedule = schedule.to_ScheduleRuleset.get
      next if schedule.nonResourceObjectUseCount(true) == 0 # true excludes children

      # hash to hold chart
      chart = { chart_data: [], chart_attributes: {} }

      # get schedule and type limits
      type_limits = nil
      if schedule.scheduleTypeLimits.is_initialized
        type_limits = schedule.scheduleTypeLimits.get.unitType
      end

      # populate table with high level schedule information
      schedules_overview_table[:data] << [schedule.name, type_limits, schedule.scheduleRules.size, schedule.nonResourceObjectUseCount(true)]

      # array to hold profiles
      profiles = []

      # get default profile
      profiles << [schedule.defaultDaySchedule, 'default profile']

      # get design days
      summer_design = schedule.summerDesignDaySchedule
      profiles << [summer_design, 'summer design day']
      winter_design = schedule.winterDesignDaySchedule
      profiles << [winter_design, 'winter design day']

      # get rules
      schedule.scheduleRules.each do |rule|
        # add days of week to text
        if rule.applySunday then sun = 'Sun' else sun = '' end
        if rule.applyMonday then mon = 'Mon' else mon = '' end
        if rule.applyTuesday then tue = 'Tue' else tue = '' end
        if rule.applyWednesday then wed = 'Wed' else wed = '' end
        if rule.applyThursday then thu = 'Thu' else thu = '' end
        if rule.applyFriday then fri = 'Fri' else fri = '' end
        if rule.applySaturday then sat = 'Sat' else sat = '' end

        # add dates to text
        if rule.startDate.is_initialized
          date = rule.startDate.get
          start = date
        else
          start = ''
        end
        if rule.endDate.is_initialized
          date = rule.endDate.get
          finish = date
        else
          finish = ''
        end

        text = "(#{sun}#{mon}#{tue}#{wed}#{thu}#{fri}#{sat}) #{start}-#{finish}"
        profiles << [rule.daySchedule, text]
      end

      # rule colors by index (starting with default, then summer, winter, then p1,p2, etc)
      rule_colors = ['#88CCEE', 'red', 'blue', '#AA4499', '#332286', '#117733', '#99995B', '#DDCC77', '#CC6677', '#882255', '#6699CC', '#661100', '#AA4466', '#505050']

      # temp test of profiles
      profile_counter = -2
      profiles.each do |array|
        profile = array[0]
        text = array[1]

        if profile_counter == -2
          name = "#{text}"
        elsif profile_counter < 1
          name = " #{text}"
        else
          name = "Priority #{profile_counter} - #{text}"
        end

        # update counter
        profile_counter += 1

        times = profile.times
        values = profile.values
        (1..times.size).each do |index|
          # add for this index value
          time_double = times[index - 1].hours + times[index - 1].minutes / 60.0
          value = values[index - 1]

          # populate chart with datapoint
          # chart[:chart_data] << JSON.generate({:label => name, :label_x => time_double, :value => value, :color => rule_colors[profile_counter+1]})
        end

        # add datapoint for 24
        time = OpenStudio::Time.new(0, 24, 0, 0)
        val = profile.getValue(time)
        # chart[:chart_data] << JSON.generate({:label => name, :label_x => 24.0, :value => val, :color => rule_colors[profile_counter+1]})

        # add datapoint for 0
        time = OpenStudio::Time.new(0, 0, 0, 0)
        val = profile.getValue(time)
        # chart[:chart_data] << JSON.generate({:label => name, :label_x => 0.0, :value => val, :color => rule_colors[profile_counter+1]})

        # get datapoint every 15min (doing this because I could get step to work just with input profile values)
        (1..24).each do |i|
          fractional_hours = i / 1.0

          hr = fractional_hours.truncate
          min = ((fractional_hours - fractional_hours.truncate) * 60.0).truncate

          time = OpenStudio::Time.new(0, hr, min, 0)
          val = profile.getValue(time)

          # add unit conversion depending on type limits
          if type_limits == 'Temperature'
            val = OpenStudio.convert(val, 'C', 'F').get
          end

          # populate chart with datapoint
          chart[:chart_data] << JSON.generate(label: name, label_x: fractional_hours, value: val, color: rule_colors[profile_counter + 1])
        end
      end

      # populate chart attributes
      if type_limits == 'Temperature'
        chart[:chart_attributes][:value] = "#{type_limits} (F)"
      elsif type_limits == 'ActivityLevel'
        chart[:chart_attributes][:value] = "#{type_limits} (W)"
      else
        chart[:chart_attributes][:value] = type_limits
      end
      chart[:chart_attributes][:label_x] = 'Hours'
      chart[:chart_attributes][:title] = schedule.name

      # push chart to array of charts
      schedules_overview_table[:chart] << chart
    end

    return @schedules_overview_section
  end
end
