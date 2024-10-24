////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2017-2021 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef UDEV_UDEV_HPP
#define UDEV_UDEV_HPP

////////////////////////////////////////////////////////////////////////////////
#include <memory>

////////////////////////////////////////////////////////////////////////////////
namespace impl
{

struct udev;
struct udev_delete { void operator()(udev*); };

}

////////////////////////////////////////////////////////////////////////////////
namespace udev
{

////////////////////////////////////////////////////////////////////////////////
// Udev context.
//
// Used by the `enumerate` and `monitor` classes to get access to udev.
//
class udev
{
public:
    udev() noexcept = default;

    udev(const udev& rhs) noexcept { *this = rhs; }
    udev(udev&& rhs) noexcept = default;

    udev& operator=(const udev&) noexcept;
    udev& operator=(udev&&) noexcept = default;

    auto get() const noexcept { return udev_.get(); }

    static udev instance();

private:
    std::unique_ptr<impl::udev, impl::udev_delete> udev_;
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif