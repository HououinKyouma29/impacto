#include "gxtloader.h"

#include "../log.h"
#include "../util.h"

namespace Impacto {
namespace Gxm {
enum SceGxmTextureType : uint32_t {
  Swizzled = 0x00000000,
  Cube = 0x40000000,
  Linear = 0x60000000,
  Tiled = 0x80000000,
  SwizzledArbitrary = 0xA0000000,
  LinearStrided = 0xC0000000,
  CubeArbitrary = 0xE0000000
};

enum SceGxmTextureBaseFormat : uint32_t {
  U8 = 0x00000000,
  S8 = 0x01000000,
  U4U4U4U4 = 0x02000000,
  U8U3U3U2 = 0x03000000,
  U1U5U5U5 = 0x04000000,
  U5U6U5 = 0x05000000,
  S5S5U6 = 0x06000000,
  U8U8 = 0x07000000,
  S8S8 = 0x08000000,
  U16 = 0x09000000,
  S16 = 0x0A000000,
  F16 = 0x0B000000,
  U8U8U8U8 = 0x0C000000,
  S8S8S8S8 = 0x0D000000,
  U2U10U10U10 = 0x0E000000,
  U16U16 = 0x0F000000,
  S16S16 = 0x10000000,
  F16F16 = 0x11000000,
  F32 = 0x12000000,
  F32M = 0x13000000,
  X8S8S8U8 = 0x14000000,
  X8U24 = 0x15000000,
  U32 = 0x17000000,
  S32 = 0x18000000,
  SE5M9M9M9 = 0x19000000,
  F11F11F10 = 0x1A000000,
  F16F16F16F16 = 0x1B000000,
  U16U16U16U16 = 0x1C000000,
  S16S16S16S16 = 0x1D000000,
  F32F32 = 0x1E000000,
  U32U32 = 0x1F000000,
  PVRT2BPP = 0x80000000,
  PVRT4BPP = 0x81000000,
  PVRTII2BPP = 0x82000000,
  PVRTII4BPP = 0x83000000,
  UBC1 = 0x85000000,
  UBC2 = 0x86000000,
  UBC3 = 0x87000000,
  YUV420P2 = 0x90000000,
  YUV420P3 = 0x91000000,
  YUV422 = 0x92000000,
  P4 = 0x94000000,
  P8 = 0x95000000,
  U8U8U8 = 0x98000000,
  S8S8S8 = 0x99000000,
  U2F10F10F10 = 0x9A000000
};

enum SceGxmTextureSwizzle4Mode : uint32_t {
  ABGR = 0x00000000,
  ARGB = 0x00001000,
  RGBA = 0x00002000,
  BGRA = 0x00003000,
  _1BGR = 0x00004000,
  _1RGB = 0x00005000,
  RGB1 = 0x00006000,
  BGR1 = 0x00007000
};

enum SceGxmTextureSwizzle3Mode : uint32_t { BGR = 0x0000, RGB = 0x1000 };
}  // namespace Gxm

struct SubtextureHeader {
  uint32_t Offset;
  uint32_t Size;
  uint32_t PaletteIdx;
  uint32_t Flags;  // or unused
  uint32_t PixelOrder;
  uint32_t Format;

  uint16_t Width;
  uint16_t Height;
  uint16_t MipmapCount;
};

#define TexfmtCheck(condition)                                              \
  if (!((condition))) {                                                     \
    ImpLog(LL_Error, LC_TextureLoad, "Unsupported texture format 0x%08x\n", \
           stx->Format);                                                    \
    return false;                                                           \
  }                                                                         \
  (void)0

// Vita unswizzle
//
// Thanks @xdanieldzd, @FireyFly and ryg
// https://github.com/xdanieldzd/Scarlet/blob/d8aabf430307d35a81b131e40bb3c9a4828bdd7b/Scarlet/Drawing/ImageBinary.cs
// http://xen.firefly.nu/up/rearrange.c.html
// https://fgiesen.wordpress.com/2009/12/13/decoding-morton-codes/

/* clang-format off */

// Inverse of Part1By1 - "delete" all odd-indexed bits
uint32_t Compact1By1(uint32_t x) {
  x &= 0x55555555;                  // x = -f-e -d-c -b-a -9-8 -7-6 -5-4 -3-2 -1-0
  x = (x ^ (x >>  1)) & 0x33333333; // x = --fe --dc --ba --98 --76 --54 --32 --10
  x = (x ^ (x >>  2)) & 0x0f0f0f0f; // x = ---- fedc ---- ba98 ---- 7654 ---- 3210
  x = (x ^ (x >>  4)) & 0x00ff00ff; // x = ---- ---- fedc ba98 ---- ---- 7654 3210
  x = (x ^ (x >>  8)) & 0x0000ffff; // x = ---- ---- ---- ---- fedc ba98 7654 3210
  return x;
}
uint32_t DecodeMorton2X(uint32_t code) { return Compact1By1(code >> 0); }
uint32_t DecodeMorton2Y(uint32_t code) { return Compact1By1(code >> 1); }

void Unswizzle(int* x, int* y, int width, int height) {
  // TODO: verify this is even sensible
  int origX = *x, origY = *y;
  if (width == 0) width = 16;
  if (height == 0) height = 16;

  int i = (origY * width) + origX;
  int min = width < height ? width : height;
  int k = (int)log2f(min);

  if (height < width) {
    // XXXyxyxyx -> XXXxxxyyy
    int j = i >> (2 * k) << (2 * k)
        | (DecodeMorton2Y(i) & (min - 1)) << k
        | (DecodeMorton2X(i) & (min - 1)) << 0;
    *x = j / height;
    *y = j % height;
  }
  else {
    // YYYyxyxyx -> YYYyyyxxx
    int j = i >> (2 * k) << (2 * k)
        | (DecodeMorton2X(i) & (min - 1)) << k
        | (DecodeMorton2Y(i) & (min - 1)) << 0;
    *x = j % width;
    *y = j / width;
  }
}

/* clang-format on */

bool GXTLoadSubtexture(SDL_RWops* stream, Texture* outTexture,
                       SubtextureHeader* stx, uint8_t* p4Palettes,
                       uint8_t* p8Palettes, uint32_t p4count) {
  memset(outTexture, 0, sizeof(*outTexture));
  outTexture->Width = stx->Width;
  outTexture->Height = stx->Height;
  SDL_RWseek(stream, stx->Offset, RW_SEEK_SET);
  uint32_t baseFormat = (stx->Format & 0xFF000000U);
  uint32_t channelOrder = (stx->Format & 0x0000FFFFU);

  if (stx->PixelOrder != Gxm::Swizzled && stx->PixelOrder != Gxm::Linear) {
    ImpLog(LL_Error, LC_TextureLoad, "Unsupported pixel order 0x%08x\n",
           stx->PixelOrder);
    return false;
  }

  switch (baseFormat) {
    // 24bpp RGB
    case Gxm::U8U8U8: {
      TexfmtCheck(channelOrder == Gxm::BGR || channelOrder == Gxm::RGB);

      outTexture->Format = TexFmt_RGB;
      outTexture->BufferSize = (3 * stx->Width * stx->Height);
      outTexture->Buffer = (uint8_t*)malloc(outTexture->BufferSize);
      for (int y = 0; y < stx->Height; y++) {
        for (int x = 0; x < stx->Width; x++) {
          int outX = x, outY = y;
          if (stx->PixelOrder == Gxm::Swizzled) {
            Unswizzle(&outX, &outY, stx->Width, stx->Height);
          }

          uint8_t r, g, b;
          if (channelOrder == Gxm::RGB) {
            b = SDL_ReadU8(stream);
            g = SDL_ReadU8(stream);
            r = SDL_ReadU8(stream);
          } else if (channelOrder == Gxm::BGR) {
            r = SDL_ReadU8(stream);
            g = SDL_ReadU8(stream);
            b = SDL_ReadU8(stream);
          }

          outTexture->Buffer[(outX + stx->Width * outY) * 3 + 0] = r;
          outTexture->Buffer[(outX + stx->Width * outY) * 3 + 1] = g;
          outTexture->Buffer[(outX + stx->Width * outY) * 3 + 2] = b;
        }
      }
      break;
    }

    // 32bpp RGBA
    case Gxm::U8U8U8U8: {
      TexfmtCheck(channelOrder == Gxm::ARGB);

      outTexture->Format = TexFmt_RGBA;
      outTexture->BufferSize = (4 * stx->Width * stx->Height);
      outTexture->Buffer = (uint8_t*)malloc(outTexture->BufferSize);
      for (int y = 0; y < stx->Height; y++) {
        for (int x = 0; x < stx->Width; x++) {
          int outX = x, outY = y;
          if (stx->PixelOrder == Gxm::Swizzled) {
            Unswizzle(&outX, &outY, stx->Width, stx->Height);
          }

          uint8_t r, g, b, a;
          b = SDL_ReadU8(stream);
          g = SDL_ReadU8(stream);
          r = SDL_ReadU8(stream);
          a = SDL_ReadU8(stream);

          outTexture->Buffer[(outX + stx->Width * outY) * 4 + 0] = r;
          outTexture->Buffer[(outX + stx->Width * outY) * 4 + 1] = g;
          outTexture->Buffer[(outX + stx->Width * outY) * 4 + 2] = b;
          outTexture->Buffer[(outX + stx->Width * outY) * 4 + 3] = a;
        }
      }
      break;
    }

    // 256 color paletted
    case Gxm::P8: {
      TexfmtCheck(channelOrder == Gxm::_1RGB);

      // PaletteIdx is into *all* palettes (P8s following all P4s), we have P4
      // and P8 separate
      uint8_t* palette = p8Palettes + 4 * 256 * (stx->PaletteIdx - p4count);

      outTexture->Format = TexFmt_RGB;
      outTexture->BufferSize = (3 * stx->Width * stx->Height);
      outTexture->Buffer = (uint8_t*)malloc(outTexture->BufferSize);

      for (int y = 0; y < stx->Height; y++) {
        for (int x = 0; x < stx->Width; x++) {
          int outX = x, outY = y;
          if (stx->PixelOrder == Gxm::Swizzled) {
            Unswizzle(&outX, &outY, stx->Width, stx->Height);
          }

          uint8_t colorIdx = SDL_ReadU8(stream);
          uint8_t* color = palette + 4 * colorIdx;

          // TODO check what the order is on paletted textures that actually
          // have color
          uint8_t r = color[0];
          uint8_t g = color[1];
          uint8_t b = color[2];

          outTexture->Buffer[(outX + stx->Width * outY) * 3 + 0] = r;
          outTexture->Buffer[(outX + stx->Width * outY) * 3 + 1] = g;
          outTexture->Buffer[(outX + stx->Width * outY) * 3 + 2] = b;
        }
      }
      break;
    }

    default: { TexfmtCheck(0); }
  }

  return true;
}

bool TextureLoadGXT(SDL_RWops* stream, Texture* outTexture) {
  ImpLog(LL_Debug, LC_TextureLoad, "Loading GXT texture\n");

  // Read metadata

  const uint32_t magic = 0x47585400;
  if (SDL_ReadBE32(stream) != magic) {
    ImpLog(LL_Error, LC_TextureLoad, "GXT loader called on non GXT data\n");
    return false;
  }
  uint32_t version = SDL_ReadLE32(stream);
  uint32_t subtextureCount = SDL_ReadLE32(stream);
  assert(subtextureCount == 1);
  uint32_t subtexturesOffset = SDL_ReadLE32(stream);
  uint32_t totalTexSize = SDL_ReadLE32(stream);
  uint32_t p4Count = SDL_ReadLE32(stream);
  uint32_t p8Count = SDL_ReadLE32(stream);
  // padding
  SDL_RWseek(stream, 4, RW_SEEK_CUR);

  ImpLog(
      LL_Debug, LC_TextureLoad,
      "GXT version=0x%08x, subtextureCount=0x%08x, subtexturesOffset=0x%08x, "
      "totalTexSize=0x%08x, p4Count=0x%08x, p8Count=0x%08x\n",
      version, subtextureCount, subtexturesOffset, totalTexSize, p4Count,
      p8Count);

  SubtextureHeader stx;
  stx.Offset = SDL_ReadLE32(stream);
  stx.Size = SDL_ReadLE32(stream);
  stx.PaletteIdx = SDL_ReadLE32(stream);
  stx.Flags = SDL_ReadLE32(stream);
  stx.PixelOrder = SDL_ReadLE32(stream);
  stx.Format = SDL_ReadLE32(stream);

  stx.Width = SDL_ReadLE16(stream);
  stx.Height = SDL_ReadLE16(stream);
  stx.MipmapCount = SDL_ReadLE16(stream);
  assert(stx.MipmapCount == 1);
  // subtexture header padding
  SDL_RWseek(stream, 2, RW_SEEK_CUR);

  SDL_RWseek(stream, stx.Size, RW_SEEK_CUR);

  ImpLog(LL_Debug, LC_TextureLoad,
         "Subtexture Offset=0x%08x, Size=0x%08x, PaletteIdx=0x%08x, "
         "Flags=0x%08x, PixelOrder=0x%08x, Format=0x%08x, Width=0x%08x, "
         "Height=0x%08x, MipmapCount=0x%08x\n",
         stx.Offset, stx.Size, stx.PaletteIdx, stx.Flags, stx.PixelOrder,
         stx.Format, stx.Width, stx.Height, stx.MipmapCount);

  // Read palettes

  // 4bpp/8bpp => 32bpp palettes
  uint8_t* P4Palettes = NULL;
  uint8_t* P8Palettes = NULL;
  if (p4Count > 0) {
    P4Palettes = (uint8_t*)ImpStackAlloc(4 * 16 * p4Count);
    SDL_RWread(stream, P4Palettes, 4 * 16, p4Count);
  }
  if (p8Count > 0) {
    P8Palettes = (uint8_t*)ImpStackAlloc(4 * 256 * p8Count);
    SDL_RWread(stream, P8Palettes, 4 * 256, p8Count);
  }

  // Get result
  bool result = GXTLoadSubtexture(stream, outTexture, &stx, P4Palettes,
                                  P8Palettes, p4Count);

  if (P4Palettes) ImpStackFree(P4Palettes);
  if (P8Palettes) ImpStackFree(P8Palettes);

  return result;
}

}  // namespace Impacto