node("openstudio-ubuntu-1604") { 
   stage("build openstudio") {
     dir("/srv/data/OpenStudio/build")  {
      sh("printenv")
      sh("git fetch origin +refs/pull/*/merge:refs/remotes/origin/pr/*")
      sh("git checkout origin/pr/${env.CHANGE_ID}")
      sh('git merge develop')
      sh('ninja')
     }
   }
 }

