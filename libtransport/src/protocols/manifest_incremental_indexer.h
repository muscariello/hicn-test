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

#include <hicn/transport/auth/common.h>
#include <implementation/socket.h>
#include <protocols/incremental_indexer.h>
#include <utils/suffix_strategy.h>

#include <list>

namespace transport {
namespace protocol {

class ManifestIncrementalIndexer : public IncrementalIndexer {
  static constexpr double alpha = 0.3;

 public:
  using SuffixQueue = std::queue<uint32_t>;
  using InterestContentPair =
      std::pair<core::Interest::Ptr, core::ContentObject::Ptr>;

  ManifestIncrementalIndexer(implementation::ConsumerSocket *icn_socket,
                             TransportProtocol *transport,
                             Reassembly *reassembly);

  ManifestIncrementalIndexer(IncrementalIndexer &&indexer)
      : IncrementalIndexer(std::move(indexer)),
        suffix_strategy_(utils::SuffixStrategyFactory::getSuffixStrategy(
            core::NextSegmentCalculationStrategy::INCREMENTAL,
            next_download_suffix_, 0)) {
    for (uint32_t i = first_suffix_; i < next_download_suffix_; i++) {
      suffix_queue_.push(i);
    }
  }

  virtual ~ManifestIncrementalIndexer() = default;

  void reset(std::uint32_t offset = 0) override;

  void onContentObject(core::Interest &interest,
                       core::ContentObject &content_object) override;

  uint32_t getNextSuffix() override;

  uint32_t getNextReassemblySegment() override;

  bool isFinalSuffixDiscovered() override;

  uint32_t getFinalSuffix() override;

 protected:
  std::unique_ptr<utils::SuffixStrategy> suffix_strategy_;
  SuffixQueue suffix_queue_;

  // Hash verification
  std::unordered_map<auth::Suffix, auth::HashEntry> suffix_map_;
  std::unordered_map<auth::Suffix, InterestContentPair> unverified_segments_;

 private:
  void onUntrustedManifest(core::Interest &interest,
                           core::ContentObject &content_object);
  void processTrustedManifest(core::Interest &interest,
                              std::unique_ptr<ContentObjectManifest> manifest);
  void onUntrustedContentObject(core::Interest &interest,
                                core::ContentObject &content_object);
  void applyPolicy(core::Interest &interest,
                   core::ContentObject &content_object,
                   auth::VerificationPolicy policy);
};

}  // end namespace protocol

}  // end namespace transport
