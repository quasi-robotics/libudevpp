////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2017-2020 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "udev.hpp"

#include <cerrno>
#include <system_error>

////////////////////////////////////////////////////////////////////////////////
namespace impl
{

// move udev stuff into impl namespace
#include <libudev.h>

}

////////////////////////////////////////////////////////////////////////////////
udev::udev udev::udev::instance()
{
    static std::weak_ptr<impl::udev> weak;

    auto p = weak.lock();
    if(!p)
    {
        p = std::shared_ptr<impl::udev>{ impl::udev_new(), &impl::udev_unref };
        if(!p) throw std::system_error{
            std::error_code{ errno, std::generic_category() }
        };
        weak = p;
    }
    return udev{ std::move(p) };
}
