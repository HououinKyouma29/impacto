#include "game.h"

#include "window.h"
#include "../vendor/nuklear/nuklear_sdl_gl3.h"
#include "workqueue.h"
#include "modelviewer.h"
#include "log.h"
#include "inputsystem.h"
#include "audio/audiosystem.h"
#include "audio/audiochannel.h"
#include "audio/audiostream.h"

namespace Impacto {

static int const NkMaxVertexMemory = 256 * 1024;
static int const NkMaxElementMemory = 128 * 1024;

static int const GameScrWorkSize = 32000;
static int const GameFlagWorkSize = 1000;

Game::Game(GameFeatureConfig const& config) : Config(config) {
  WindowInit();

  memset(DrawComponents, TD_None, sizeof(DrawComponents));

  if (!Config.SystemArchiveName.empty()) {
    IoError err =
        VfsArchive::Mount(Config.SystemArchiveName.c_str(), &SystemArchive);
    if (err != IoError_OK) {
      ImpLog(LL_Fatal, LC_General, "Failed to load system archive!\n");
      WindowShutdown();
      return;
    }
  }
  if (!Config.BgmArchiveName.empty()) {
    IoError err = VfsArchive::Mount(Config.BgmArchiveName.c_str(), &BgmArchive);
    if (err != IoError_OK) {
      ImpLog(LL_Fatal, LC_General, "Failed to load BGM archive!\n");
      WindowShutdown();
      return;
    }
  }
  if (!Config.SeArchiveName.empty()) {
    IoError err = VfsArchive::Mount(Config.SeArchiveName.c_str(), &SeArchive);
    if (err != IoError_OK) {
      ImpLog(LL_Fatal, LC_General, "Failed to load SE archive!\n");
      WindowShutdown();
      return;
    }
  }
  if (!Config.SysseArchiveName.empty()) {
    IoError err =
        VfsArchive::Mount(Config.SysseArchiveName.c_str(), &SysseArchive);
    if (err != IoError_OK) {
      ImpLog(LL_Fatal, LC_General, "Failed to load SYSSE archive!\n");
      WindowShutdown();
      return;
    }
  }
  if (!Config.VoiceArchiveName.empty()) {
    IoError err =
        VfsArchive::Mount(Config.VoiceArchiveName.c_str(), &VoiceArchive);
    if (err != IoError_OK) {
      ImpLog(LL_Fatal, LC_General, "Failed to load VOICE archive!\n");
      WindowShutdown();
      return;
    }
  }

  if (Config.GameFeatures & GameFeature::Nuklear) {
    Nk = nk_sdl_init(g_SDLWindow, NkMaxVertexMemory, NkMaxElementMemory);
    struct nk_font_atlas* atlas;
    nk_sdl_font_stash_begin(&atlas);
    // no fonts => default font used, but we still have do the setup
    nk_sdl_font_stash_end();
  }

  if (Config.GameFeatures & GameFeature::Audio) {
    Audio::AudioInit();
  }

  if (Config.GameFeatures & GameFeature::Scene3D) {
    SceneInit();
    Scene3D = new Scene;
  }

  if (Config.GameFeatures & GameFeature::ModelViewer) {
    ModelViewerComponent = new ModelViewer(this);
  }

  if (Config.GameFeatures & GameFeature::Sc3VirtualMachine) {
    VmComponent = new Vm::Vm(this);
    ScrWork = (uint32_t*)calloc(GameScrWorkSize, sizeof(uint32_t));
    FlagWork = (uint8_t*)calloc(GameFlagWorkSize, sizeof(uint8_t));
  }

  if (Config.GameFeatures & GameFeature::Renderer2D) {
    Renderer2D::Init();
    R2D = new Renderer2D;
  }

  for (int i = 0; i < DialoguePageCount; i++) {
    DialoguePages[i].Clear();
    DialoguePages[i].Mode = DPM_NVL;
    DialoguePages[i].Id = i;
    DialoguePages[i].GameCtx = this;
  }
}

void Game::Init() {
  if (Config.GameFeatures & GameFeature::Scene3D) {
    Scene3D->Init();
  }
  if (Config.GameFeatures & GameFeature::ModelViewer) {
    ModelViewerComponent->Init();
  }

  if (Config.GameFeatures & GameFeature::Sc3VirtualMachine) {
    VmComponent->Init(4, 0);
  }
}

Game* Game::CreateModelViewer() {
  GameFeatureConfig config;
  config.LayerCount = 1;
  config.GameFeatures = GameFeature::Nuklear | GameFeature::Scene3D |
                        GameFeature::ModelViewer | GameFeature::Audio |
                        GameFeature::Input;
  config.BgmArchiveName = "bgm.cpk";
  config.Scene3D_BackgroundCount = 1;
  config.Scene3D_CharacterCount = 1;

  Game* result = new Game(config);
  result->Init();
  return result;
}

Game* Game::CreateVmTest() {
  GameFeatureConfig config;
  config.LayerCount = 1;
  config.GameFeatures = GameFeature::Sc3VirtualMachine |
                        GameFeature::Renderer2D | GameFeature::Scene3D |
                        GameFeature::Input | GameFeature::Audio;
  config.SystemArchiveName = "system.cpk";
  config.BgmArchiveName = "bgm.cpk";
  config.SeArchiveName = "se.cpk";
  config.SysseArchiveName = "sysse.cpk";
  config.VoiceArchiveName = "voice.cpk";
  config.Dlg = DialoguePageFeatureConfig_RNE;
  config.Scene3D_BackgroundCount = 1;
  config.Scene3D_CharacterCount = 8;

  Game* result = new Game(config);
  result->Init();

  // Font
  void* texFile;
  int64_t texSz;
  result->SystemArchive->Slurp(12, &texFile, &texSz);
  SDL_RWops* stream = SDL_RWFromConstMem(texFile, (int)texSz);
  Texture tex;
  tex.Load(stream);
  result->Config.Dlg.DialogueFont.Sheet.Texture = tex.Submit();
  SDL_RWclose(stream);
  free(texFile);

  // Data sprite sheet
  void* dataTexFile;
  int64_t dataTexSz;
  result->SystemArchive->Slurp(9, &dataTexFile, &dataTexSz);
  SDL_RWops* dataStream = SDL_RWFromConstMem(dataTexFile, (int)dataTexSz);
  Texture dataTex;
  dataTex.Load(dataStream);
  result->Config.Dlg.DataSpriteSheet.DesignHeight = dataTex.Height;
  result->Config.Dlg.DataSpriteSheet.DesignWidth = dataTex.Width;
  result->Config.Dlg.DataSpriteSheet.Texture = dataTex.Submit();
  SDL_RWclose(dataStream);
  free(dataTexFile);

  return result;
}

Game* Game::CreateDialogueTest() {
  GameFeatureConfig config;
  config.LayerCount = 1;
  config.GameFeatures = GameFeature::Renderer2D;
  config.SystemArchiveName = "system.cpk";
  config.Dlg = DialoguePageFeatureConfig_RNE;

  Game* result = new Game(config);
  result->Init();

  void* texFile;
  int64_t texSz;
  result->SystemArchive->Slurp(12, &texFile, &texSz);
  SDL_RWops* stream = SDL_RWFromConstMem(texFile, (int)texSz);
  Texture tex;
  tex.Load(stream);
  result->Config.Dlg.DialogueFont.Sheet.Texture = tex.Submit();
  SDL_RWclose(stream);
  free(texFile);

  result->DrawComponents[0] = TD_Text;

  Vm::Sc3VmThread dummy;
  uint8_t string[] = {0x11, 0x01, 0x18, 0x12, 0x00, 0xF0, 0x82, 0x22, 0x82,
                      0x23, 0x81, 0xA6, 0x82, 0x24, 0x81, 0x61, 0x82, 0x25,
                      0x81, 0x60, 0x81, 0x79, 0x81, 0x80, 0x80, 0xBE, 0x82,
                      0x26, 0x81, 0x64, 0x81, 0x65, 0x82, 0x27, 0x80, 0xED,
                      0x81, 0x78, 0x80, 0xBF, 0x08, 0xFF};
  uint8_t string2[] = {0x01, 0x82, 0x20, 0x82, 0x21, 0x02, 0x80,
                       0xD8, 0x80, 0xE3, 0x80, 0xE3, 0x80, 0xE3,
                       0x80, 0xE3, 0x80, 0xD9, 0x03, 0xFF};
  dummy.Ip = string2;
  result->DialoguePages[0].Mode = DPM_ADV;
  result->DialoguePages[0].ADVBoxOpacity = 0.0f;
  result->DialoguePages[0].AnimState = DPAS_Showing;
  result->DialoguePages[0].AddString(&dummy);

  return result;
}

Game::~Game() {
  if (Config.GameFeatures & GameFeature::ModelViewer) {
    delete ModelViewerComponent;
  }

  if (Config.GameFeatures & GameFeature::Sc3VirtualMachine) {
    if (ScrWork) free(ScrWork);
    if (FlagWork) free(FlagWork);
    delete VmComponent;
  }

  if (Config.GameFeatures & GameFeature::Audio) {
    Audio::AudioShutdown();
  }

  if (Config.GameFeatures & GameFeature::Scene3D) {
    if (Scene3D) {
      delete Scene3D;
    }
  }

  if (Config.GameFeatures & GameFeature::Renderer2D) {
    if (R2D) {
      delete R2D;
    }
  }

  if (Config.GameFeatures & GameFeature::Nuklear) {
    nk_sdl_shutdown();
  }

  WindowShutdown();
}

void Game::Update(float dt) {
  SDL_Event e;
  if (Config.GameFeatures & GameFeature::Nuklear) {
    nk_input_begin(Nk);
  }
  if (Config.GameFeatures & GameFeature::Input) {
    Input::BeginFrame();
  }
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      ShouldQuit = true;
    }

    if (Config.GameFeatures & GameFeature::Nuklear) {
      SDL_Event e_nk;
      memcpy(&e_nk, &e, sizeof(SDL_Event));
      WindowAdjustEventCoordinatesForNk(&e_nk);
      if (nk_sdl_handle_event(&e_nk)) continue;
    }

    if (Config.GameFeatures & GameFeature::Input) {
      if (Input::HandleEvent(&e)) continue;
    }

    WorkQueueHandleEvent(&e);
  }
  if (Config.GameFeatures & GameFeature::Input) {
    Input::EndFrame();
  }
  if (Config.GameFeatures & GameFeature::Nuklear) {
    nk_input_end(Nk);
  }

  if (Config.GameFeatures & GameFeature::ModelViewer) {
    ModelViewerComponent->Update(dt);
  }

  if (Config.GameFeatures & GameFeature::Sc3VirtualMachine) {
    VmComponent->Update();
  }

  if (Config.GameFeatures & GameFeature::Audio) {
    Audio::AudioUpdate(dt);
  }

  if (Config.GameFeatures & GameFeature::Scene3D) {
    Scene3D->Update(dt);
  }

  if (Config.GameFeatures & GameFeature::Renderer2D) {
    for (int i = 0; i < DialoguePageCount; i++) DialoguePages[i].Update(dt);
  }
}

void Game::Render() {
  WindowUpdate();

  Rect viewport = WindowGetViewport();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  if (Config.GameFeatures & GameFeature::Scene3D) {
    Scene3D->Render();
  }

  if (Config.GameFeatures & GameFeature::Renderer2D) {
    R2D->Begin();
    Vm::SetDateDisplay(this);
    for (int i = 0; i < Vm::VmMaxThreads; i++) {
      if (DrawComponents[i] == TD_None) break;

      switch (DrawComponents[i]) {
        case TD_Text: {
          DialoguePages[0].Render();
          break;
        }
        case TD_Main: {
          for (uint32_t layer = 0; layer < Config.LayerCount; layer++) {
            // TODO
          }
        }
        default: {
          ImpLog(LL_Error, LC_General,
                 "Encountered unknown draw component type %02X\n",
                 DrawComponents[i]);
          break;
        }
      }
    }
    R2D->Finish();
  }

  if (Config.GameFeatures & GameFeature::Nuklear) {
    if (g_GLDebug) {
      // Nuklear spams these
      glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0,
                               NULL, GL_FALSE);
    }
    nk_sdl_render(NK_ANTI_ALIASING_OFF, viewport.Width, viewport.Height);
    if (g_GLDebug) {
      glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0,
                               NULL, GL_TRUE);
    }
  }

  WindowDraw();
}

void Game::SetFlag(uint32_t flagId, uint32_t value) {
  uint32_t flagIndex = flagId >> 3;
  int flagValue = 1 << (flagId - 8 * (flagId >> 3));
  FlagWork[flagIndex] |= flagValue;
  if (!value) {
    FlagWork[flagIndex] ^= flagValue;
  }
}

bool Game::GetFlag(uint32_t flagId) {
  return ((uint8_t)(1 << (flagId - 8 * (flagId >> 3))) &
          *((uint8_t*)FlagWork + (flagId >> 3))) != 0;
}

}  // namespace Impacto