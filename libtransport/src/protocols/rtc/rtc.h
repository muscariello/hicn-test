/*
 * Copyright (c) 2017-2021 Cisco and/or its affiliates.
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

#include <protocols/datagram_reassembly.h>
#include <protocols/rtc/rtc_ldr.h>
#include <protocols/rtc/rtc_rc.h>
#include <protocols/rtc/rtc_state.h>
#include <protocols/transport_protocol.h>

#include <unordered_set>
#include <vector>

namespace transport {

namespace protocol {

namespace rtc {

class RTCTransportProtocol : public TransportProtocol,
                             public DatagramReassembly {
 public:
  RTCTransportProtocol(implementation::ConsumerSocket *icnet_socket);

  ~RTCTransportProtocol();

  using TransportProtocol::start;

  using TransportProtocol::stop;

  void resume() override;

 private:
  enum class SyncState { catch_up = 0, in_sync = 1, last };

 private:
  // setup functions
  void initParams();
  void reset() override;

  void inactiveProducer();

  // protocol functions
  void discoveredRtt();
  void newRound();

  // window functions
  void computeMaxSyncWindow();
  void updateSyncWindow();
  void decreaseSyncWindow();

  // packet functions
  void sendInterest(Name *interest_name);
  void sendRtxInterest(uint32_t seq);
  void sendProbeInterest(uint32_t seq);
  void scheduleNextInterests() override;
  void onTimeout(Interest::Ptr &&interest) override;
  void onNack(const ContentObject &content_object);
  void onProbe(const ContentObject &content_object);
  void reassemble(ContentObject &content_object) override;
  void onContentObject(Interest &interest,
                       ContentObject &content_object) override;
  void onPacketDropped(Interest &interest,
                       ContentObject &content_object) override {}
  void onReassemblyFailed(std::uint32_t missing_segment) override {}

  // interaction with app functions
  void sendStatsToApp(uint32_t retx_count, uint32_t received_bytes,
                      uint32_t sent_interests, uint32_t lost_data,
                      uint32_t recovered_losses, uint32_t received_nacks);
  // protocol state
  bool start_send_interest_;
  SyncState current_state_;
  // cwin vars
  uint32_t current_sync_win_;
  uint32_t max_sync_win_;

  // controller var
  std::unique_ptr<asio::steady_timer> round_timer_;
  std::unique_ptr<asio::steady_timer> scheduler_timer_;
  bool scheduler_timer_on_;

  // timeouts
  std::unordered_set<uint32_t> timeouts_or_nacks_;

  // names/packets var
  uint32_t next_segment_;

  std::shared_ptr<RTCState> state_;
  std::shared_ptr<RTCRateControl> rc_;
  std::shared_ptr<RTCLossDetectionAndRecovery> ldr_;

  uint32_t number_;
};

}  // namespace rtc

}  // namespace protocol

}  // namespace transport
