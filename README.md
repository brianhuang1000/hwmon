# hwmon
HWMon is an user friendly system monitor that allows the user to see its own
computer information and file system, see an interact with living processes and
provides a graphical representation about resource consumption.

### Prerequisites

HWMon needs a linux enviorment, qt standard and graphs libraries and C++
standard libraries and a compiler.

### Installing

To build program one have to run 'make'
```
make
```

# usage

After the program compiles, one can run 'hwmon' to start the program

```
hwmon
```

A window will pop-up with of tabs on it.

System tab:
  the system tab provides information about the system, such us OS, Kernel and
  hardware information.

Processes tab:
  the processes tab provides information about the living processes. One can
  click on any to get an interaction menu. With this menu, one can click on
  "Stop Process" to stop that process, "Continue Process" to continue the
  process if stopped, "End Process" to quit the process,  "Kill Process" to kill
  the process. Additionally one can click on "Memory Map", "Open files" or
  "Properties", these will pop up additional windows with the specific
  information requested.

Resources tab:
  The resources tab contains three graphs representing the CPU, Memory, and
  Network usage history.

File System tab:
  the file system tab provides the memory total, free, available, used of the
  computer file systems.

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available,
see the [tags on this repository](https://github.com/your/project/tags).

## Authors

* **Bryan Huang** - *Processes* - [bryanhuang1000](https://github.com/bryanhuang1000)
* **Alan Libby** - *Graphs and File system* - [PurpleBooth](https://github.com/PurpleBooth)
* **Lucianop Handal** - *System information and front end* - [lhandalb](https://github.com/lhandalb)
