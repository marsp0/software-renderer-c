#include "test_png.h"

#include "test_utils.h"
#include "../parsers/png.h"

#include <string.h>

// abaabbbabaababbaababaaaabaaabbbbbaa

// sub filter
// a baa b   b   b 
// 	  196 195 195
// a baa b   a   b 
// 	  196 194 195
// b aab a   b   a 
// 	  194 195 195
// a aab a   a   a
// 	  194 194 195
// b bbb b   a   a
// 	  196 195 195

// up filter
// a b   a   a   b   b   b 
//   98  97  97  98  98  98
// a b   a   a   b   a   b 
//   196 194 194 196 195 196
// b a   a   b   a   b   a 
//   37  35  36  37  37  37
// a a   a   b   a   a   a
//   134 132 134 134 134 134
// b b   b   b   b   a   a
//   232 230 232 232 231 231

// average filter
// a b   a   a   b   b   b 
//   98  97  97  147 146 146  
// a b   a   a   b   a   b 
//   147 145 145 245 242 243
// b a   a   b   a   b   a 
//   170 169 170 48  47  47
// a a   a   b   a   a   a
//   182 181 183 212 211 212
// b b   b   b   b   a   a
//   189 188 189 42  40  41

// paeth filter
// a b   a   a   b   b   b 
//   98  97  97  196 195 195
// a b   a   a   b   a   b 
//   196 194 194 38  36  37
// b a   a   b   a   b   a 
//   37  35  36  134 133 133
// a a   a   b   a   a   a
//   134 132 134 231 230 231
// b b   b   b   b   a   a
//   232 230 232 74  71  73

static void test_simple_png(void)
{
    unsigned char buffer[] = {
        0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a,     // png magic number
        
        0x00, 0x00, 0x00, 0x0d, 0x49, 0x48, 0x44, 0x52,     // header size + header type
        0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x05,     // width and height
        0x08, 0x02, 0x00, 0x00, 0x00,                       // bits per pixel, color type, 
                                                            // compression, filter, interlace
        0xe0, 0xd1, 0xaa, 0xcb,                             // header crc
        
        0x00, 0x00, 0x00, 0x1d, 0x49, 0x44, 0x41, 0x54,     // data chunk size + type
        0x78, 0xda,                                         // zlib header
        0x1d, 0xc6, 0x49, 0x01, 0x00, 0x00, 0x10, 0x40,     // deflate stream
        0xc0, 0xac, 0xa3, 0x7f, 0x88, 0x3d, 0x3c, 0x20, 
        0x2a, 0x97, 0x9d, 0x37, 0x5e, 0x1d, 0x0c,
        0x00, 0x00, 0x00, 0x00,                             // zlib trailer (adler 32)
        0xfc, 0xca, 0x79, 0xd8,                             // data chunk crc

        0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4e, 0x44,     // end chunk size + type
        0xae, 0x42, 0x60, 0x82                              // end chunk crc
    };

    texture_t* tex = parse_png_buffer(buffer, sizeof(buffer - 1));
    // baa bbb 
    // baa bab 
    // aab aba 
    // aab aaa 
    // bbb baa
    ASSERT_STRING("baabbbbaababaababaaabaaabbbbaa", tex->data, 30);

    texture_free(tex);
}

void test_png(void)
{
    TEST_CASE(test_simple_png);
}