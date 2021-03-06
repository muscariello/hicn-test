/*
 * Copyright (c) 2017-2019 Cisco and/or its affiliates.
 * Copyright 2017 Facebook, Inc.
 *
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

#include <type_traits>

namespace utils {

template <typename F>
struct MoveWrapper : F {
  MoveWrapper(F&& f) : F(std::move(f)) {}

  MoveWrapper(MoveWrapper&&) = default;
  MoveWrapper& operator=(MoveWrapper&&) = default;

  MoveWrapper(const MoveWrapper&);
  MoveWrapper& operator=(const MoveWrapper&);
};

template <typename T>
auto moveHandler(T&& t) -> MoveWrapper<typename std::decay<T>::type> {
  return std::move(t);
}
}  // namespace utils