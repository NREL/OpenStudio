pipeline {

  agent {label 'openstudio-ubuntu-1604'}

 if(env.BRANCH_NAME == 'develop') {
 
  stage('build openstudio') {
    
      dir('/srv/data/OpenStudio/build') { 
        sh 'git pull'
        sh 'ninja'
      }

    }
    
  }

}


