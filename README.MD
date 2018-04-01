# RetroSpy Server

RetroSpy is a GameSpy Project that aims to create GameSpy 2011/2012 services.

The server is written in C++ and it's the spiritual successor to OpenSpy Core (It shares the same architecture)

### Architecture

###### RetroSpy Common
	The common library is used across all the projects and contains common functions like Server binding
	
###### RetroSpy
	This is the main executable. It works by creating multiple threads of the "Modules", witch are the parts of the server
	If you want to host GP-only server, just use RetroSpy, GP and edit the ini file
	
###### Modules
	The modules are dynamic libraries that are called when the server is starting. They contains the server module
	(like binding to GPSP port). This modules are also able to the specific configuration from the INI (See INI Syntax for more)
	
### INI Syntax
A general RetroSpy.ini looks like this

'''
[Database] # We setup database
Host=Your host
Username=Your username
Password=Your password
Port=Your port
Name=your database
[Modules] # Here we define the modules to be loaded
Load0=Module 1
Load1=Module 2
LoadX=Module
'''

ALWAYS change the number otherwise the server will load only one module
Then, specific module

'''
[Module name]
Field=Value
Field2=Value
'''

Reefer to each module for the list of fields you can use
ALWAYS use the same module name used in LoadX calls otherwise the section won't be loaded
