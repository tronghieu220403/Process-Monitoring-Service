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
│   │
│   pm_ctb.cpp                                           # code for client
│   pm_cta.cpp                                           # code for server
│   │
├── inlucde
│   └── communication                                            
│   │   └── pipeline                                     # dependencies for pipeline (IPC)
│   │   │   └── client.h
│   │   │   └── server.h
│   │   │   └── pipe.h
│   │
│   └── configuration                                     
│   │   └── processjsonconfiguration.h                           # JSON config reader
│   │   └── startupprogram.h                                     # add a program to startup
│   │
│   └── etw                                              # Event Tracing Windows
│   │   └── consumer.h
│   │   └── provider.h
│   │   └── wmieventclass.h                                      # Retrieve event infomation
│   │   └── event.h
│   │   └── diskioevent.h                                        # Handle disk IO event (read/write)
│   │   └── networkevent.h                                       # Handle network IO event (receive/send)
│   │   └── threadevent.h                                        # Handle thread event (start/end)
│   │
│   └── file
│   │   └── file.h
│   │                
│   └── logs
│   │   └── logger.h
│   │   └── processlogger.h
│   │                
│   └── mutex                                            
│   │   └── mutex.h
│   │                
│   └── nlohmann                                         # dependencies for JSON reader in Linux
│   │   └── process.h
│   │   └── process.cpp
│   │
│   └── pdh                                            
│   │   └── query.h
│   │   └── counter.h
│   │
│   └── process                                          # dependencies for process
│   │   └── process.h                                            # base classes for processes
│   │   └── processcpu.h
│   │   └── processdisk.h
│   │   └── processmem.h
│   │   └── processnetwork.h
│   │   └── processinfo.h                                        # contain resources stats of a process.
│   │   └── processcontroller.h                                  # full control of a process 
│   │   └── processsupervision.h                                 # monitoring resources stats of a process
│   │
│   └── program                                          # dependencies for the main program
│   │   └── cta.h
│   │   └── ctb.h
│   │
│   └── registry                                         # dependencies for Windows Registry
│   │   └── registry.h
│   │
│   └── service                                          # dependencies for Windows Service
│   │   └── processmonitorservice.h                              # Functions for the service
│   │   └── service.h
│   │   └── servicecontrol.h                                     # Add .exe to Service Database
│   │   └── serviceevent.h                                       # Handle service events (start/stop/continue/pause)
│   │
│   └── ulti                                             # dependencies for platform 
│   │   └── collections.h                                        # some structures/enum classes using between classes
│   │   └── everything.h                                         # everything needed (libraries, syntaxs) for the code
│   │                
├── release
│   └── x64
│   │   └── Linux                               # executable file for server and client on Linux
│   │   │   └── pm_cta		
│   │   │   └── pm_ctb			
│   │   └── Windows                             # executable file for server and client on Windows
│   │   │   └── pm_cta.exe			
│   │   │   └── pm_ctb.exe
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
