// FILE: fprime/components/VerifyChain/VerifyChainComponent.hpp
#ifndef VERIFY_CHAIN_COMPONENT_HPP
#define VERIFY_CHAIN_COMPONENT_HPP
#include <cstddef>
#include <cstdint>
class VerifyChain {
public:
  struct Stats{ uint32_t lines,ok_head,ok_parent,ok_body,bad_head,bad_parent,bad_body; };
  static Stats Verify(const uint8_t* ndjson, size_t len, bool strict);
};
#endif
