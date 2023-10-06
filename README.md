# Process Monitoring - a simple service and an user application

This is a simple process monitoring tool, include a service and an user application, works on both 64-bit Windows and Linux OS. Pipelines is used as a IPC communication between the service and our user application.

- [Folder structure](#folder-structure)
- [Requirements](#requirements)
- [Coding style](#coding-style)

Folder structure
----------------
```
.
├── Process-Monitoring-Report.pdf                        # Report in Vietnamese
│   │
├── Class diagram.png
├── Class diagram.cd
│   │                
│   pm_ctb.cpp                                           # code for client
│   pm_cta.cpp                                           # code for server
│   │                
├── inlucde
│   └── communication                                            
│   │   └── pipeline                                     # dependencies for pipeline (IPC)
│   │   │   └── client.h
│   │   │   └── server.h
│   │   │   └── pipe.cpp
│   └── configuration                                     
│   │   └── processjsonconfiguration.h                   # JSON config reader
│   │   └── processjsonconfiguration.cpp
│   │   └── startupprogram.h                             # add a program to startup
│   │   └── startupprogram.cpp
│   └── file                                             
│   │   └── file.h
│   │   └── file.cpp
│   └── logs                                             
│   │   └── logger.h
│   │   └── logger.cpp
│   │   └── processlogger.h
│   │   └── processlogger.cpp
│   └── mutex                                            
│   │   └── mutex.h
│   │   └── mutex.cpp
│   └── nlohmann                                         # dependencies for JSON reader in Linux
│   │   └── process.h
│   │   └── process.cpp
│   └── process                                          # dependencies for process
│   │   └── process.h                                            # base classes for processes
│   │   └── process.cpp                                          
│   │   └── processcpu.h
│   │   └── processcpu.cpp
│   │   └── processdisk.h
│   │   └── processdisk.cpp
│   │   └── processmem.h
│   │   └── processmem.cpp
│   │   └── processnetwork.h
│   │   └── processnetwork.cpp
│   │   └── processinfo.h                                        # contain resources stats of a process.
│   │   └── processinfo.cpp
│   │   └── processcontroller.h                                  # full control of a process 
│   │   └── processcontroller.cpp
│   │   └── processsupervision.h                                 # monitoring resources stats of a process
│   │   └── processsupervision.cpp
│   └── program                                          # dependencies for the main program
│   │   └── cta.h
│   │   └── cta.cpp
│   │   └── ctb.h
│   │   └── ctb.cpp
│   └── registry                                         # dependencies for Windows Registry
│   │   └── process.h
│   │   └── process.cpp
│   └── ulti                                             # dependencies for platform 
│   │   └── collections.h                                # some structures/enum classes using between classes
│   │   └── everything.h                                 # everything needed (libraries, syntaxs) for the code
│   │                
├── release binary
│   └── x64
│   │   └── Linux                               # executable file for server and client on Linux
│   │   │   └── client_main		
│   │   │   └── server_main			
│   │   └── Windows                             # executable file for server and client on Windows
│   │   │   └── client_main.exe			
│   │   │   └── server_main.exe
│   │
────────────	
```


Requirements
---
* C++ 20
* Supported Operating Systems (64-bit)
  * Windows
  * Linux

Coding style
------------
[Google C++ Style](https://google.github.io/styleguide/cppguide.html)
