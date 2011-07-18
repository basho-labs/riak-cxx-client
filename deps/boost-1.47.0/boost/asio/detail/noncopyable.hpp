//
// detail/noncopyable.hpp
// ~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_DETAIL_NONCOPYABLE_HPP
#define BOOST_ASIO_DETAIL_NONCOPYABLE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <boost/asio/detail/config.hpp>
#include <boost/noncopyable.hpp>
#include <boost/detail/workaround.hpp>

#include <boost/asio/detail/push_options.hpp>

namespace riakboost{} namespace boost = riakboost; namespace riakboost{
namespace asio {
namespace detail {

#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
// Redefine the noncopyable class for Borland C++ since that compiler does not
// apply the empty base optimisation unless the base class contains a dummy
// char data member.
class noncopyable
{
protected:
  noncopyable() {}
  ~noncopyable() {}
private:
  noncopyable(const noncopyable&);
  const noncopyable& operator=(const noncopyable&);
  char dummy_;
};
#else
using riakboost::noncopyable;
#endif

} // namespace detail

using riakboost::asio::detail::noncopyable;

} // namespace asio
} // namespace riakboost

#include <boost/asio/detail/pop_options.hpp>

#endif // BOOST_ASIO_DETAIL_NONCOPYABLE_HPP
