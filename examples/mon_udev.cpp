#include <udev++/udev++.hpp>

#include <iostream>

int main() 
{
auto ctx = udev::udev::instance();

udev::monitor monitor{ ctx };
monitor.match_device("drm");

for(;;)
{
  using namespace std::chrono_literals;
  using namespace std;

  if(auto device = monitor.try_get_for(30s)) {
    switch(device.action())
    {
      case udev::added:
          // if(device.type() == "partition" && device.property("ID_BUS") == "usb")
          {
              cout << "Device added plugged in" << endl
                    << "Path: " << device.syspath() << endl
                    << "Node: " << device.devnode() << endl
                    << "  FS: " << device.property("ID_FS_TYPE") << endl
                    << "Name: " << device.sysname() << endl
                    << "   #: " << device.sysnum () << endl;
              cout << endl;
          }
          break;

      case udev::removed:
          // if(device.type() == "partition" && device.property("ID_BUS") == "usb")
          {
              cout << "Device removed in" << endl
                    << "Path: " << device.syspath() << endl
                    << "Node: " << device.devnode() << endl
                    << "  FS: " << device.property("ID_FS_TYPE") << endl
                    << "Name: " << device.sysname() << endl
                    << "   #: " << device.sysnum () << endl;
              cout << endl;
          }
          break;
      case udev::other: 
          {
              cout << "Device changed" << endl
                    << "Path: " << device.syspath() << endl
                    << "Node: " << device.devnode() << endl
                    << "  FS: " << device.property("ID_FS_TYPE") << endl
                    << "Name: " << device.sysname() << endl
                    << "   #: " << device.sysnum () << endl;
              cout << endl;

          }
          break;
      }
    } else {
      cout << "Nothing seems to be happening" << endl;
      break;
    }
  }

  return 0;
}
