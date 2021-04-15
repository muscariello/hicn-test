/*
 * Copyright (c) 2017-2019 Cisco and/or its affiliates.
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

#include <hicn/transport/core/io_module.h>
#include <hicn/transport/core/prefix.h>

#ifdef always_inline
#undef always_inline
#endif
extern "C" {
#include <vapi/vapi_safe.h>
};

namespace transport {

namespace core {

class MemifConnector;

class VPPForwarderModule : public IoModule {
  static constexpr std::uint16_t interface_mtu = 1500;

 public:
  VPPForwarderModule();
  ~VPPForwarderModule();

  void connect(bool is_consumer) override;

  void send(Packet &packet) override;
  void send(const uint8_t *packet, std::size_t len) override;

  bool isConnected() override;

  void init(Connector::PacketReceivedCallback &&receive_callback,
            Connector::OnReconnectCallback &&reconnect_callback,
            asio::io_service &io_service,
            const std::string &app_name = "Libtransport") override;

  void registerRoute(const Prefix &prefix) override;

  std::uint32_t getMtu() override;

  bool isControlMessage(const uint8_t *message) override;

  void processControlMessageReply(utils::MemBuf &packet_buffer) override;

  void closeConnection() override;

 private:
  uint32_t getMemifConfiguration();
  void consumerConnection();
  void producerConnection();

 private:
  MemifConnector *connector_;
  uint32_t memif_id_;
  uint32_t sw_if_index_;
  // A consumer socket in vpp has two faces (ipv4 and ipv6)
  uint32_t face_id1_;
  uint32_t face_id2_;
  bool is_consumer_;
  vapi_ctx_t sock_;
};

extern "C" IoModule *create_module(void);

}  // namespace core

}  // namespace transport
