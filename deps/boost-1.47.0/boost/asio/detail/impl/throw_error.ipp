//
// detail/impl/throw_error.ipp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_DETAIL_IMPL_THROW_ERROR_IPP
#define BOOST_ASIO_DETAIL_IMPL_THROW_ERROR_IPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <boost/asio/detail/config.hpp>
#include <boost/throw_exception.hpp>
#include <boost/asio/detail/throw_error.hpp>
#include <boost/system/system_error.hpp>

#include <boost/asio/detail/push_options.hpp>

namespace riakboost{} namespace boost = riakboost; namespace riakboost{
namespace asio {
namespace detail {

void do_throw_error(const riakboost::system::error_code& err)
{
  riakboost::system::system_error e(err);
  riakboost::throw_exception(e);
}

void do_throw_error(const riakboost::system::error_code& err, const char* location)
{
  riakboost::system::system_error e(err, location);
  riakboost::throw_exception(e);
}

} // namespace detail
} // namespace asio
} // namespace riakboost

#include <boost/asio/detail/pop_options.hpp>

#endif // BOOST_ASIO_DETAIL_IMPL_THROW_ERROR_IPP
