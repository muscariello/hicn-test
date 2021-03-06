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

#include <hicn/transport/interfaces/callbacks.h>
#include <hicn/transport/interfaces/socket_producer.h>
#include <hicn/transport/interfaces/statistics.h>
#include <hicn/transport/utils/object_pool.h>
#include <implementation/socket.h>
#include <utils/content_store.h>

#include <atomic>
#include <thread>

namespace transport {

namespace protocol {

using namespace core;

class ProductionProtocol : public Portal::ProducerCallback {
 public:
  ProductionProtocol(implementation::ProducerSocket *icn_socket);
  virtual ~ProductionProtocol();

  bool isRunning() { return is_running_; }

  virtual int start();
  virtual void stop();

  virtual void produce(ContentObject &content_object);
  virtual uint32_t produceStream(const Name &content_name,
                                 std::unique_ptr<utils::MemBuf> &&buffer,
                                 bool is_last = true,
                                 uint32_t start_offset = 0) = 0;
  virtual uint32_t produceStream(const Name &content_name,
                                 const uint8_t *buffer, size_t buffer_size,
                                 bool is_last = true,
                                 uint32_t start_offset = 0) = 0;
  virtual uint32_t produceDatagram(const Name &content_name,
                                   std::unique_ptr<utils::MemBuf> &&buffer) = 0;
  virtual uint32_t produceDatagram(const Name &content_name,
                                   const uint8_t *buffer,
                                   size_t buffer_size) = 0;

  void setOutputBufferSize(std::size_t size) { output_buffer_.setLimit(size); }
  std::size_t getOutputBufferSize() { return output_buffer_.getLimit(); }

  virtual void registerNamespaceWithNetwork(const Prefix &producer_namespace);
  const std::list<Prefix> &getNamespaces() const { return served_namespaces_; }

 protected:
  // Producer callback
  virtual void onInterest(core::Interest &i) override = 0;
  virtual void onError(std::error_code ec) override{};

 protected:
  implementation::ProducerSocket *socket_;

  // Thread pool responsible for IO operations (send data / receive interests)
  std::vector<utils::EventThread> io_threads_;

  // TODO remove this thread
  std::thread listening_thread_;
  std::shared_ptr<Portal> portal_;
  std::atomic<bool> is_running_;
  interface::ProductionStatistics *stats_;

  // Callbacks
  interface::ProducerInterestCallback *on_interest_input_;
  interface::ProducerInterestCallback *on_interest_dropped_input_buffer_;
  interface::ProducerInterestCallback *on_interest_inserted_input_buffer_;
  interface::ProducerInterestCallback *on_interest_satisfied_output_buffer_;
  interface::ProducerInterestCallback *on_interest_process_;

  interface::ProducerContentObjectCallback *on_new_segment_;
  interface::ProducerContentObjectCallback *on_content_object_to_sign_;
  interface::ProducerContentObjectCallback *on_content_object_in_output_buffer_;
  interface::ProducerContentObjectCallback *on_content_object_output_;
  interface::ProducerContentObjectCallback
      *on_content_object_evicted_from_output_buffer_;

  interface::ProducerContentCallback *on_content_produced_;

  // Output buffer
  utils::ContentStore output_buffer_;

  // List ot routes served by current producer protocol
  std::list<Prefix> served_namespaces_;

  bool is_async_;
};

}  // end namespace protocol
}  // end namespace transport
