////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2017-2020 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef UDEV_ENUMERATE_HPP
#define UDEV_ENUMERATE_HPP

////////////////////////////////////////////////////////////////////////////////
#include "device.hpp"
#include "udev.hpp"

#include <memory>
#include <string>
#include <vector>

namespace impl
{

struct udev_enumerate;
struct enumerate_delete { void operator()(udev_enumerate*); };

}

////////////////////////////////////////////////////////////////////////////////
namespace udev
{

////////////////////////////////////////////////////////////////////////////////
// Udev device enumerator.
//
// As needed, call one or more of the match_*() functions to only match specific
// devices. Then, call the get() function to get a list of matching devices.
//
class enumerate
{
public:
    enumerate();

    enumerate(const enumerate&) = delete;
    enumerate(enumerate&&) noexcept = default;

    enumerate& operator=(const enumerate&) = delete;
    enumerate& operator=(enumerate&&) noexcept = default;

    void match_subsystem(const std::string&);
    void nomatch_subsystem(const std::string&);

    void match_sysattr(const std::string& name, const std::string& value = std::string());
    void nomatch_sysattr(const std::string& name, const std::string& value = std::string());

    void match_property(const std::string& name, const std::string& value = std::string());

    void match_sysname(const std::string&);
    void match_tag(const std::string&);

    void match_parent(const device&);

    std::vector<device> get();

private:
    udev udev_;
    std::unique_ptr<impl::udev_enumerate, impl::enumerate_delete> enum_;
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
