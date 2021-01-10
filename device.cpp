////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2017 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "device.hpp"
#include "udev.hpp"

#include <cerrno>
#include <system_error>

////////////////////////////////////////////////////////////////////////////////
namespace impl
{

// move udev stuff into impl namespace
#include <libudev.h>

void device_delete::operator()(udev_device* x) { udev_device_unref(x); }

}

////////////////////////////////////////////////////////////////////////////////
namespace
{

auto str(const char* s) noexcept
{
    return s ? std::string(s) : std::string();
}

}

////////////////////////////////////////////////////////////////////////////////
namespace udev
{

////////////////////////////////////////////////////////////////////////////////
device::device(const udev& udev_, const std::string& path) :
    dev_(impl::udev_device_new_from_syspath(udev_.get(), path.data()))
{
    if(!dev_) throw std::system_error{
        std::error_code{ errno, std::generic_category() }
    };
}

////////////////////////////////////////////////////////////////////////////////
device device::parent() const noexcept
{
    auto dev = impl::udev_device_get_parent(dev_.get());

    // udev_device_get_parent() returns reference whose lifetime is tied to the
    // child's lifetime, so we have to make a "deep copy"

    if(dev) dev = impl::udev_device_new_from_syspath(
        impl::udev_device_get_udev(dev),
        impl::udev_device_get_syspath(dev)
    );
    return device{ dev };
}

////////////////////////////////////////////////////////////////////////////////
device device::parent(const std::string& subsystem, const std::string& type) const noexcept
{
    auto dev = impl::udev_device_get_parent_with_subsystem_devtype(dev_.get(),
        subsystem.data(), type.size() ? type.data() : nullptr
    );

    // udev_device_get_parent() returns reference
    // whose lifetime is tied to the child's lifetime,
    // so we have to make a "deep copy"

    if(dev) dev = impl::udev_device_new_from_syspath(
        impl::udev_device_get_udev(dev),
        impl::udev_device_get_syspath(dev)
    );
    return device{ dev };
}

////////////////////////////////////////////////////////////////////////////////
std::string device::subsystem() const noexcept
{
    return str(impl::udev_device_get_subsystem(dev_.get()));
}

////////////////////////////////////////////////////////////////////////////////
std::string device::devtype() const noexcept
{
    return str(impl::udev_device_get_devtype(dev_.get()));
}

////////////////////////////////////////////////////////////////////////////////
std::string device::syspath() const noexcept
{
    return str(impl::udev_device_get_syspath(dev_.get()));
}

////////////////////////////////////////////////////////////////////////////////
std::string device::sysname() const noexcept
{
    return str(impl::udev_device_get_sysname(dev_.get()));
}

////////////////////////////////////////////////////////////////////////////////
std::string device::sysnum() const noexcept
{
    return str(impl::udev_device_get_sysnum(dev_.get()));
}

////////////////////////////////////////////////////////////////////////////////
std::string device::devnode() const noexcept
{
    return str(impl::udev_device_get_devnode(dev_.get()));
}

////////////////////////////////////////////////////////////////////////////////
std::string device::property(const std::string& name) const noexcept
{
    return str(impl::udev_device_get_property_value(dev_.get(), name.data()));
}

////////////////////////////////////////////////////////////////////////////////
std::string device::driver() const noexcept
{
    return str(impl::udev_device_get_driver(dev_.get()));
}

////////////////////////////////////////////////////////////////////////////////
enum action device::action() const noexcept
{
    auto action = str(impl::udev_device_get_action(dev_.get()));
    return action == "add" ? added : action == "remove" ? removed : other;
}

////////////////////////////////////////////////////////////////////////////////
std::string device::sysattr(const std::string& name) const noexcept
{
    return str(impl::udev_device_get_sysattr_value(dev_.get(), name.data()));
}

////////////////////////////////////////////////////////////////////////////////
bool device::has_tag(const std::string& name) const noexcept
{
    return impl::udev_device_has_tag(dev_.get(), name.data());
}

////////////////////////////////////////////////////////////////////////////////
}
