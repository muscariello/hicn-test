/*
 * Copyright (c) 2017-2020 Cisco and/or its affiliates.
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

#include <core/local_connector.h>
#include <hicn/transport/core/content_object.h>
#include <hicn/transport/core/interest.h>
#include <hicn/transport/errors/not_implemented_exception.h>
#include <hicn/transport/utils/log.h>

#include <asio/io_service.hpp>

namespace transport {
namespace core {

LocalConnector::~LocalConnector() {}

void LocalConnector::close() { state_ = State::CLOSED; }

void LocalConnector::send(Packet &packet) {
  if (!isConnected()) {
    return;
  }

  TRANSPORT_LOGD("Sending packet to local socket.");
  io_service_.get().post([this, p{packet.shared_from_this()}]() mutable {
    receive_callback_(this, *p, std::make_error_code(std::errc(0)));
  });
}

void LocalConnector::send(const uint8_t *packet, std::size_t len) {
  throw errors::NotImplementedException();
}

}  // namespace core
}  // namespace transport