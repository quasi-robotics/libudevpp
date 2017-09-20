////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2017 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef UDEV_DEVICE_HPP
#define UDEV_DEVICE_HPP

////////////////////////////////////////////////////////////////////////////////
#include <string>
#include <utility> // std::swap

namespace detail { struct udev; struct udev_device; }

////////////////////////////////////////////////////////////////////////////////
namespace udev
{

class enumerate;
class monitor;

////////////////////////////////////////////////////////////////////////////////
enum action { added, removed, other };

////////////////////////////////////////////////////////////////////////////////
// The udev device.
//
// Provides access to udev device attributes.
// Device instances are returned by the enumerate and monitor classes.
//
class device
{
public:
    ////////////////////
    device() noexcept = default;
    device(const device&) = delete;
    device(device&& other) noexcept { swap(other); }
    ~device() noexcept;

    device& operator=(const device&) = delete;
    device& operator=(device&& other) noexcept { swap(other); return (*this); }

    void swap(device& other) noexcept
    {
        using std::swap;
        swap(dev_, other.dev_);
    }

    ////////////////////
    bool valid() const noexcept { return dev_; }

    device parent() const noexcept;
    device parent(const std::string& subsystem, const std::string& type = std::string()) const noexcept;

    std::string subsystem() const noexcept;
    std::string type() const noexcept;
    std::string path() const noexcept;
    std::string name() const noexcept;
    std::string  num() const noexcept;
    std::string node() const noexcept;

    ////////////////////
    std::string property(const std::string&) const noexcept;
    std::string driver() const noexcept;
    udev::action action() const noexcept;
    std::string attribute(const std::string&) const noexcept;
    bool has_tag(const std::string&) const noexcept;

private:
    ////////////////////
    detail::udev_device* dev_ = nullptr;
    explicit device(detail::udev_device* dev) noexcept : dev_(dev) { }

    static device from_path(detail::udev*, const std::string&);

    ////////////////////
    friend class enumerate;
    friend class monitor;
};

////////////////////////////////////////////////////////////////////////////////
inline void swap(device& lhs, device& rhs) noexcept { return lhs.swap(rhs); }

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
