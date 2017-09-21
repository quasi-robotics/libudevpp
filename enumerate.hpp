////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2017 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef UDEV_ENUMERATE_HPP
#define UDEV_ENUMERATE_HPP

////////////////////////////////////////////////////////////////////////////////
#include "udev/device.hpp"
#include "udev/udev.hpp"

#include <string>
#include <utility> // std::swap
#include <vector>

namespace detail { struct udev_enumerate; }

////////////////////////////////////////////////////////////////////////////////
namespace udev
{

////////////////////////////////////////////////////////////////////////////////
// Udev device enumerator.
//
// As needed, call one or more of the match_*() functions
// to only match specific devices. Then, call the get() function
// to get a list of matching devices.
//
class enumerate
{
public:
    ////////////////////
    enumerate();
    ~enumerate() noexcept;

    enumerate(const enumerate&) = delete;
    enumerate(enumerate&&) = delete;

    enumerate& operator=(const enumerate&) = delete;
    enumerate& operator=(enumerate&&) = delete;

    ////////////////////
    void match_subsystem(const std::string&);
    void nomatch_subsystem(const std::string&);

    void match_attribute(const std::string& name, const std::string& value = std::string());
    void nomatch_attribute(const std::string& name, const std::string& value = std::string());

    void match_property(const std::string& name, const std::string& value = std::string());

    void match_name(const std::string&);
    void match_tag(const std::string&);

    void match_parent(const device&);

    ////////////////////
    std::vector<device> get();

private:
    ////////////////////
    udev udev_;
    detail::udev_enumerate* enum_;
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
