#include <udev++/udev++.hpp>

#include <iostream>

int main() 
{
  auto ctx = udev::udev::instance();

  udev::enumerate enumerate{ ctx };
  enumerate.match_subsystem("drm");

  auto devices = enumerate.get();
  for(auto const& device : devices)
  {
      using namespace std;

//      if(device.devtype() == "partition" && device.property("ID_BUS") == "ata")
      {
          cout << "Found hard disk" << endl
              << "Path: " << device.syspath() << endl
              << "Node: " << device.devnode() << endl
              << "  FS: " << device.property("ID_FS_TYPE") << endl
              << "Name: " << device.sysname() << endl
              << "   #: " << device.sysnum () << endl;
          cout << endl;
      }
  }
  return 0;
}