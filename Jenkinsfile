 if(env.BRANCH_NAME == 'develop') {
  node('openstudio-ubuntu-1604') { 
     stage('build openstudio') {
       dir('/srv/data/OpenStudio/build')  
        sh('git pull')
        sh('ninja')
     }
  }
}
