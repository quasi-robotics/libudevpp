////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2017-2021 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include <udev++/udev.hpp>

#include <cerrno>
#include <system_error>

////////////////////////////////////////////////////////////////////////////////
namespace impl
{

// move udev stuff into impl namespace
#include <libudev.h>

void udev_delete::operator()(udev* x) { udev_unref(x); }

}

////////////////////////////////////////////////////////////////////////////////
namespace udev
{

////////////////////////////////////////////////////////////////////////////////
udev& udev::operator=(const udev& rhs) noexcept
{
    if(this != &rhs)
        udev_.reset(rhs.udev_ ? impl::udev_ref(rhs.udev_.get()) : nullptr);

    return *this;
}

////////////////////////////////////////////////////////////////////////////////
udev udev::instance()
{
    udev ctx;

    ctx.udev_.reset(impl::udev_new());
    if(!ctx.udev_) throw std::system_error{
        std::error_code{ errno, std::generic_category() }
    };

    return ctx;
}

////////////////////////////////////////////////////////////////////////////////
}
