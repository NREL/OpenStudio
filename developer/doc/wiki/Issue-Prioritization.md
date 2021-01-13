OpenStudio uses github issues to track both bugs and enhancement requests. Properly labeling github issues is crucial for managing and prioritizing the resolution of these issues. Please use the following labels to classify OpenStudio issues in github. 

## Bug Severity
All bugs should be assigned a severity describing the impact of a bug to the end users. The key here is impartiality and objectivity in judging the bug’s severity. Remember that all bugs seem urgent to you if they impact your own work or deadlines. However, personal preferences must not be a criteria for giving a bug its correct severity! The severity classification must be an impartial and a fair judgment of the impact/scope that the issue has or will have to the end user community as a whole, not you! 

* Blocker - total loss of functionality making the software unusable to the majority of the users, file corruption, severe memory leak, or any crashes. No workarounds, the software is mostly unusable.
* Major - major loss of functionality, impacts a mainline (major) workflow or workflows (therefore impacts a large number of users), no workarounds available. If a workaround exists or is found, then the severity of the issue must be immediately downgraded to "Normal".
* Normal - some loss of functionality under specific circumstances or specific workflow/actions, may or may not have a workaround. State whether a workaround exists or not, and if one is found then provide details in the issue description.
* Minor - minor loss of functionality, easy workaround may be available (provide details). This severity category also includes cosmetic issues, like misspelled words, misaligned text or icons, visually annoying issues or issues that do not affect functionality. Documentation issues should be labeled as minor severity (not to be confused with a missing documentation request - see below).

## Enhancement Request
An enhancement request is used to label potential improvements to existing functionalities. Therefore, an enhancement request is not an issue, as the functionality was implemented according to the original intended design. A missing functionality which impedes a workflow must be labeled as an enhancement request and not a defect.  If a feature was supposedly delivered but does not work properly according to intended functionality, then that is an issue. Since enhancement requests are not bugs, do not assign a bug severity to enhancement requests.

## Documentation Request
A documentation request is used to label all issues related to missing documentation. A documentation issue related to existing documentation is treated the same as code issue (however, needs to be labeled with minor severity - see above).

## Component
Each issue should be tagged with the component that the bug impacts or that the feature request is for.  Each issue should be tagged with only one component, tagging an issue with multiple components leads to double counting and confusion about ownership. 

## Resolution
If the issue is closed but not fixed, label the final resolution and provide a note explaining the rationale.

* Can't Reproduce - cannot reproduce the bug as reported
* Duplicate - issue is a duplicate of another issue, add a note to the original issue
* Won't Fix - issue will not be addressed for some reason, add a note with explanation (the affected component is to be deprecated, etc)
* Works As Expected - the issue can be duplicated but the behavior is as designed, add a note explaining this

## EnergyPlus Issue
If the issue relates to a bug or missing feature in EnergyPlus then apply this label, link to the EnergyPlus issue if it exists. If it does not exist, take specific steps to open an EnergyPlus issue.

## Developer Priority 
For OpenStuduio developers, the priority should be given to work on bug fixes in the order of severity (Blocker -> Major -> Normal -> Minor) followed by Enhancement requests. This is the default order of priority and things change as some enhancement requests are necessary to improve OpenStudio so these can take precedence over bugs.  
