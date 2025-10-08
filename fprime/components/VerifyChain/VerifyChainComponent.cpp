// FILE: fprime/components/VerifyChain/VerifyChainComponent.cpp
#include "VerifyChainComponent.hpp"
extern "C" { #include "reuse/libverify/verify.h" }
VerifyChain::Stats VerifyChain::Verify(const uint8_t* ndjson, size_t len, bool strict){
  byte_span_t sp{ndjson,len}; verify_stats_t st; verify_chain_ex(sp, strict?1:0, &st);
  return {st.lines,st.ok_head,st.ok_parent,st.ok_body,st.bad_head,st.bad_parent,st.bad_body};
}
