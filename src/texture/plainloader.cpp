#include "plainloader.h"

namespace Impacto {
namespace TexLoad {

// This is for a very simple format used for some graphics by various console
// releases
// TODO: big-endian (360/PS3)

bool TextureIsPlain(SDL_RWops* stream) {
  SDL_RWseek(stream, 4, RW_SEEK_SET);
  uint32_t mode = SDL_ReadLE32(stream);
  bool result = (mode == 8 || mode == 32);
  SDL_RWseek(stream, 0, RW_SEEK_SET);
  return result;
}

bool TextureLoadPlain(SDL_RWops* stream, Texture* outTexture) {
  uint16_t width = SDL_ReadLE16(stream);
  uint16_t height = SDL_ReadLE16(stream);
  uint32_t mode = SDL_ReadLE32(stream);

  switch (mode) {
    case 8: {  // 8-bit paletted; palette is BGRA
      uint8_t palette[256 * 4];
      SDL_RWread(stream, palette, 4, 256);

      outTexture->Init(TexFmt_RGBA, width, height);

      uint8_t* inBuffer = (uint8_t*)malloc(width * height);
      SDL_RWread(stream, inBuffer, width * height, 1);

      uint8_t* reader = inBuffer;
      uint8_t* writer = outTexture->Buffer;

      for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
          uint8_t colorIdx = *reader;
          uint8_t* color = palette + 4 * colorIdx;

          writer[0] = color[2];
          writer[1] = color[1];
          writer[2] = color[0];
          writer[3] = color[3];

          reader++;
          writer += 4;
        }
      }

      free(inBuffer);
      return true;
    }

    case 32: {  // 32-bit ARGB
      outTexture->Init(TexFmt_RGBA, width, height);

      uint8_t* inBuffer = (uint8_t*)malloc(outTexture->BufferSize);
      SDL_RWread(stream, inBuffer, outTexture->BufferSize, 1);

      uint8_t* reader = inBuffer;
      uint8_t* writer = outTexture->Buffer;

      for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
          writer[0] = reader[1];
          writer[1] = reader[2];
          writer[2] = reader[3];
          writer[3] = reader[0];

          reader += 4;
          writer += 4;
        }
      }

      free(inBuffer);
      return true;
    }

    default:
      return false;
  }
}

}  // namespace TexLoad
}  // namespace Impacto