# Bash script to run a bunch of 1ZoneEvapCooler.idf throughout versions of E+...

# Add your OAUTH token here (public access only)
token=
curl_token="-H \"Authorization: token $token\""

# All versions you want to run
declare -a all_versions=("8.5.0" "8.6.0" "8.7.0" "8.8.0" "8.9.0" "9.0.1" "9.1.0")

# declare -a all_versions=("9.1.0")


if [ ! -f USA_CO_Golden-NREL.724666_TMY3.epw ]; then
  wget https://github.com/NREL/EnergyPlus/raw/develop/weather/USA_CO_Golden-NREL.724666_TMY3.epw
fi;


for ep_version in "${all_versions[@]}"; do
  ep_version_dash=$(echo $ep_version | sed -r 's/\./-/g')

  # Special case
  if [ "$ep_version" == "9.1.0" ]; then
    ep_version=91AndBugFix4
  fi
  echo ""
  echo ""
  echo "$ep_version"

  idf_file="1ZoneEvapCooler-V$ep_version_dash.idf"
  if [ ! -f $idf_file ]; then
    wget https://raw.githubusercontent.com/NREL/EnergyPlus/v$ep_version/testfiles/1ZoneEvapCooler.idf
    mv 1ZoneEvapCooler.idf $idf_file

    echo "  Output:SQLite," >> $idf_file
    echo '    SimpleAndTabular;        !- Option Type' >> $idf_file
    echo "" >> $idf_file

    sed -i 's/Output:Variable,\*,Zone Mean Air Temperature,hourly;/Output:Variable,\*,Zone Mean Air Temperature,Annual;\n\n  Output:Variable,\*,Zone Mean Air Temperature,RunPeriod;\n\n  Output:Variable,\*,Zone Mean Air Temperature,Environment;\n\n  Output:Variable,\*,Zone Mean Air Temperature,Monthly;\n\n  Output:Variable,\*,Zone Mean Air Temperature,Daily;\n\n  Output:Variable,\*,Zone Mean Air Temperature,Hourly;\n\n  Output:Variable,\*,Zone Mean Air Temperature,Timestep;\n\n  Output:Variable,\*,Zone Mean Air Temperature,Detailed;\n/g' $idf_file

  fi;


  echo "curl https://api.github.com/repos/NREL/EnergyPlus/releases/tags/v$ep_version | jq -r '.assets | .[] | select(.name | contains("Linux")) | select(.name | contains("tar.gz")) | .browser_download_url'"
  tar_gz_link=$(curl -H "Authorization: token $token" https://api.github.com/repos/NREL/EnergyPlus/releases/tags/v$ep_version | jq -r '.assets | .[] | select(.name | contains("Linux")) | select(.name | contains("tar.gz")) | .browser_download_url')
  if [ -z "$tar_gz_link" ]; then
    echo "curl https://api.github.com/repos/NREL/EnergyPlus/releases/tags/v$ep_version | jq -r '.assets | .[] | select(.name | contains("Linux")) | select(.name | contains("sh")) | .browser_download_url'"
    sh_link=$(curl -H "Authorization: token $token" https://api.github.com/repos/NREL/EnergyPlus/releases/tags/v$ep_version | jq -r '.assets | .[] | select(.name | contains("Linux")) | select(.name | contains("sh")) | .browser_download_url')
    # sh_name=${sh_link##*/}
    echo $sh_link
    sh_name=$(basename -- $sh_link)

    if [ ! -f $sh_name ]; then
      echo "Downloading $sh_link"
      wget $sh_link
    else
      echo "$sh_name already exists"
    fi

    chmod +x $sh_name

  else
    #tar_gz_name=${tar_gz_link##*/}
    tar_gz_name=$(basename -- $tar_gz_link)
    if [ ! -f $tar_gz_name ]; then
      echo "Downloading $tar_gz_link"
      wget $tar_gz_link
    else
      echo "$tar_gz_name already exists"
    fi

    folder_name=${tar_gz_name%.tar.gz}
    if [ ! -d $folder_name ]; then
      echo "Extracting $tar_gz_name"
      tar xfz "$tar_gz_name"
    else
      echo "Already found $folder_name"
    fi

    ep_exe=$(find $folder_name/ -name "energyplus")
    if [ -z "$ep_exe" ]; then
      echo "Couldn't find Eplus EXE"
    else
      ep_exe="$(pwd)/$ep_exe"
      $ep_exe -w USA_CO_Golden-NREL.724666_TMY3.epw -d out-$ep_version $idf_file
      mv out-$ep_version/eplusout.sql 1ZoneEvapCooler-V$ep_version_dash.sql
    fi
  fi
done
