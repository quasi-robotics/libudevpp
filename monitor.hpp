////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2017 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef UDEV_MONITOR_HPP
#define UDEV_MONITOR_HPP

////////////////////////////////////////////////////////////////////////////////
#include "device.hpp"
#include "udev.hpp"

#include <chrono>
#include <memory>
#include <string>
#include <utility>

namespace impl
{

struct udev_monitor;
struct monitor_delete { void operator()(udev_monitor*); };

}

////////////////////////////////////////////////////////////////////////////////
namespace udev
{

////////////////////////////////////////////////////////////////////////////////
// Udev device addition/removal monitor.
//
// As needed, call one or more of the match_*() functions to only match specific
// devices. Then, repeatedly call any of the try_get*() functions to monitor for
// device additions and/or removals.
//
class monitor
{
public:
    monitor();

    monitor(const monitor&) = delete;
    monitor(monitor&& rhs) noexcept : monitor() { swap(rhs); }

    monitor& operator=(const monitor&) = delete;
    monitor& operator=(monitor&& rhs) noexcept { swap(rhs); return (*this); }

    void swap(monitor& rhs) noexcept
    {
        using std::swap;
        swap(udev_, rhs.udev_);
        swap(fd_, rhs.fd_);
    }

    void match_device(const std::string& subsystem, const std::string& type = std::string());
    void match_tag(const std::string&);

    // NB: first call to any of the try_*() functions will put monitor into
    // "active" state and further calls to match_*() functions will be ignored
    //
    bool active() const noexcept { return fd_ != -1; }

    device get();
    device try_get();

    template<typename Rep, typename Period>
    device try_get_for(const std::chrono::duration<Rep, Period>&);

    template<typename Clock, typename Duration>
    device try_get_until(const std::chrono::time_point<Clock, Duration>&);

private:
    udev udev_;
    std::unique_ptr<impl::udev_monitor, impl::monitor_delete> mon_;
    int fd_ = -1;

    using msec = std::chrono::milliseconds;
    device try_get_for_(const msec&);
};

////////////////////////////////////////////////////////////////////////////////
inline device monitor::get() { return try_get_for_(msec::max()); }
inline device monitor::try_get() { return try_get_for_(msec::zero()); }

template<typename Rep, typename Period>
inline device monitor::try_get_for(const std::chrono::duration<Rep, Period>& d)
{
    return try_get_for_(std::chrono::duration_cast<msec>(d));
}

template<typename Clock, typename Duration>
inline device monitor::try_get_until(const std::chrono::time_point<Clock, Duration>& tp)
{
    auto now = Clock::now();
    return try_get_for(tp - (tp < now ? tp : now));
}

////////////////////////////////////////////////////////////////////////////////
inline void swap(monitor& lhs, monitor& rhs) noexcept { lhs.swap(rhs); }

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
