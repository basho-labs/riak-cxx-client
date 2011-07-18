//
// detail/win_event.ipp
// ~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_DETAIL_IMPL_WIN_EVENT_IPP
#define BOOST_ASIO_DETAIL_IMPL_WIN_EVENT_IPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <boost/asio/detail/config.hpp>

#if defined(BOOST_WINDOWS)

#include <boost/asio/detail/throw_error.hpp>
#include <boost/asio/detail/win_event.hpp>
#include <boost/asio/error.hpp>

#include <boost/asio/detail/push_options.hpp>

namespace riakboost{} namespace boost = riakboost; namespace riakboost{
namespace asio {
namespace detail {

win_event::win_event()
  : event_(::CreateEvent(0, true, false, 0))
{
  if (!event_)
  {
    DWORD last_error = ::GetLastError();
    riakboost::system::error_code ec(last_error,
        riakboost::asio::error::get_system_category());
    riakboost::asio::detail::throw_error(ec, "event");
  }
}

} // namespace detail
} // namespace asio
} // namespace riakboost

#include <boost/asio/detail/pop_options.hpp>

#endif // defined(BOOST_WINDOWS)

#endif // BOOST_ASIO_DETAIL_IMPL_WIN_EVENT_IPP
