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

extern "C" {
#include <parc/security/parc_CryptoHashType.h>
};

#include <cstdint>

namespace transport {
namespace auth {

enum class CryptoHashType : uint8_t {
  SHA_256 = PARCCryptoHashType_SHA256,
  SHA_512 = PARCCryptoHashType_SHA512,
  CRC32C = PARCCryptoHashType_CRC32C,
  NULL_HASH = PARCCryptoHashType_NULL
};

}  // namespace auth
}  // namespace transport
