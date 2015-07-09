#include <emscripten/bind.h>
#include <emscripten.h>
#include <stdio.h>
#include "lzham.h"

using namespace emscripten;

val decompress_buffer(val mod, val in_buf) {
   auto buf = static_cast<uint8_t*>(malloc(in_buf["length"].as<int>()));
   val heapu8 = mod["HEAPU8"];
   heapu8.call<void>("set", in_buf, uint32_t(buf));

   EM_ASM(console.timeEnd('malloc and memcpy'));

   lzham_decompress_params decomp_params;
   memset(&decomp_params, 0, sizeof(decomp_params));
   decomp_params.m_struct_size = sizeof(decomp_params);
   decomp_params.m_dict_size_log2 = 26;

   lzham_uint32 decomp_adler32 = 0;
   int ibs = in_buf["length"].as<int>();

   if ((buf[0] != 'L') || (buf[1] != 'Z') || (buf[2] != 'H') || (buf[3] != '0')) {
      printf("Wrong magic number.\n");
   }

   decomp_params.m_dict_size_log2 = buf[4];

   size_t orig_file_size = 0;
   for (uint32_t i = 0; i < 4; i++) {
      orig_file_size |= (static_cast<size_t>(buf[5 + i]) << (i * 8));
   }

   lzham_uint8 decomp_buf[orig_file_size];
   size_t decomp_size = orig_file_size;

   EM_ASM(console.timeEnd('pre-decompress'));

   val rval = val::undefined();

   lzham_decompress_status_t decomp_status = lzham_decompress_memory(&decomp_params, decomp_buf, &decomp_size, &buf[13], ibs, &decomp_adler32);
   EM_ASM(console.timeEnd('decompress'));

   if (decomp_status != LZHAM_DECOMP_STATUS_SUCCESS)
   {
      printf("Decompression test failed with status %i!\n", decomp_status);
   } else {     
      rval = heapu8["buffer"].call<val>("slice", uint32_t(decomp_buf), decomp_size);
   }

   free (buf);
   return rval;
}

EMSCRIPTEN_BINDINGS(my_module) {
    function("decompress_buffer", &decompress_buffer);
}