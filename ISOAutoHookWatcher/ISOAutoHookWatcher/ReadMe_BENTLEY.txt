This project builds a Custom Module.  You must register the custom module (x86) in order for the DLL to get loaded and for the hooks to be set. 

Please see the ProjectWise V8i SDK Documentation for information about Custom Modules and hooking.

You may need to modify some of the solution or project properties to match your development environment.

You may also want to set the command line arguments specified on Configuration Properties -> Debugging -> Command Arguments.

Something like this:
-d "servername:datasourcename" -u username -p password