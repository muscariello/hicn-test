/*
 * Copyright (c) 2021 Cisco and/or its affiliates.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <string>
#include <system_error>

namespace transport {
namespace core {
/**
 * @brief Get the default server error category.
 * @return The default server error category instance.
 *
 * @warning The first call to this function is thread-safe only starting with
 * C++11.
 */
const std::error_category& forwarder_category();

/**
 * The list of errors.
 */
enum class forwarder_error {
  success = 0,
  send_failed,
  receive_failed,
  disconnected,
  memory_allocation_error,
  invalid_connector_type,
  invalid_connector,
  interest_cache_miss
};

/**
 * @brief Create an error_code instance for the given error.
 * @param error The error.
 * @return The error_code instance.
 */
inline std::error_code make_error_code(forwarder_error error) {
  return std::error_code(static_cast<int>(error), forwarder_category());
}

/**
 * @brief Create an error_condition instance for the given error.
 * @param error The error.
 * @return The error_condition instance.
 */
inline std::error_condition make_error_condition(forwarder_error error) {
  return std::error_condition(static_cast<int>(error), forwarder_category());
}

/**
 * @brief A server error category.
 */
class forwarder_category_impl : public std::error_category {
 public:
  /**
   * @brief Get the name of the category.
   * @return The name of the category.
   */
  virtual const char* name() const throw();

  /**
   * @brief Get the error message for a given error.
   * @param ev The error numeric value.
   * @return The message associated to the error.
   */
  virtual std::string message(int ev) const;
};
}  // namespace core
}  // namespace transport

namespace std {
// namespace system {
template <>
struct is_error_code_enum<::transport::core::forwarder_error>
    : public std::true_type {};
// }  // namespace system
}  // namespace std
