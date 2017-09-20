////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2017 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "posix/error.hpp"
#include "udev/device.hpp"

////////////////////////////////////////////////////////////////////////////////
namespace detail
{

// move udev stuff into detail namespace 8-o
#include <libudev.h>

}

////////////////////////////////////////////////////////////////////////////////
namespace
{

auto string(const char* s) noexcept
{ return s ? std::string(s) : std::string(); }

}

////////////////////////////////////////////////////////////////////////////////
namespace udev
{

////////////////////////////////////////////////////////////////////////////////
device::~device() noexcept { detail::udev_device_unref(dev_); }

////////////////////////////////////////////////////////////////////////////////
device device::from_path(detail::udev* udv, const std::string& path)
{
    auto dev = detail::udev_device_new_from_syspath(udv, path.data());
    if(!dev) throw posix::errno_error();

    return device(dev);
}

////////////////////////////////////////////////////////////////////////////////
device device::parent() const noexcept
{
    auto dev = detail::udev_device_get_parent(dev_);

    // udev_device_get_parent() returns reference
    // whose lifetime is tied to the child's lifetime,
    // so we have to make a "deep copy"

    if(dev) dev = detail::udev_device_new_from_syspath(
        detail::udev_device_get_udev(dev),
        detail::udev_device_get_syspath(dev)
    );
    return device(dev);
}

////////////////////////////////////////////////////////////////////////////////
device device::parent(const std::string& subsystem, const std::string& type) const noexcept
{
    auto dev = detail::udev_device_get_parent_with_subsystem_devtype(dev_,
        subsystem.data(), type.size() ? type.data() : nullptr
    );

    // udev_device_get_parent() returns reference
    // whose lifetime is tied to the child's lifetime,
    // so we have to make a "deep copy"

    if(dev) dev = detail::udev_device_new_from_syspath(
        detail::udev_device_get_udev(dev),
        detail::udev_device_get_syspath(dev)
    );
    return device(dev);
}

////////////////////////////////////////////////////////////////////////////////
std::string device::subsystem() const noexcept
{ return string(detail::udev_device_get_subsystem(dev_)); }

////////////////////////////////////////////////////////////////////////////////
std::string device::type() const noexcept
{ return string(detail::udev_device_get_devtype(dev_)); }

////////////////////////////////////////////////////////////////////////////////
std::string device::path() const noexcept
{ return string(detail::udev_device_get_syspath(dev_)); }

////////////////////////////////////////////////////////////////////////////////
std::string device::name() const noexcept
{ return string(detail::udev_device_get_sysname(dev_)); }

////////////////////////////////////////////////////////////////////////////////
std::string device::num() const noexcept
{ return string(detail::udev_device_get_sysnum(dev_)); }

////////////////////////////////////////////////////////////////////////////////
std::string device::node() const noexcept
{ return string(detail::udev_device_get_devnode(dev_)); }

////////////////////////////////////////////////////////////////////////////////
std::string device::property(const std::string& name) const noexcept
{ return string(detail::udev_device_get_property_value(dev_, name.data())); }

////////////////////////////////////////////////////////////////////////////////
std::string device::driver() const noexcept
{ return string(detail::udev_device_get_driver(dev_)); }

////////////////////////////////////////////////////////////////////////////////
udev::action device::action() const noexcept
{
    auto action = string(detail::udev_device_get_action(dev_));
    return action == "add" ? added : action == "remove" ? removed : other;
}

////////////////////////////////////////////////////////////////////////////////
std::string device::attribute(const std::string& name) const noexcept
{ return string(detail::udev_device_get_sysattr_value(dev_, name.data())); }

////////////////////////////////////////////////////////////////////////////////
bool device::has_tag(const std::string& name) const noexcept
{ return detail::udev_device_has_tag(dev_, name.data()); }

////////////////////////////////////////////////////////////////////////////////
}
