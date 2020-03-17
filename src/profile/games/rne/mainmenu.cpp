#include "mainmenu.h"
#include "../../profile_internal.h"
#include "tilebackground.h"
#include "../../../log.h"
#include "../../../window.h"

namespace Impacto {
namespace Profile {
namespace RNE {
namespace MainMenu {

static void GetMemberSpriteArray(Sprite* arr, uint32_t count,
                                 char const* name) {
  EnsurePushMemberOfType(name, kArrayType);

  if (TopVal().Size() != count) {
    ImpLog(LL_Fatal, LC_Profile, "Expected to have %d sprites for %s\n", count,
           name);
    Window::Shutdown();
  }

  for (uint32_t i = 0; i < count; i++) {
    arr[i] = EnsureGetArrayElementSprite(i);
  }

  Pop();
}

Sprite ButtonBackgroundSprite;
Sprite ButtonPromptsSprite;
Sprite SkyBackgroundSprite;
Sprite SkyArrowSprite;
Sprite SkyTextSprite;
Sprite MenuEntriesHSprites[MenuEntriesNum];
Sprite MenuEntriesSprites[MenuEntriesNum];

float SkyBackgroundBeginX;
float SkyBackgroundY;
float SkyTextBeginX;
float SkyTextY;
float MenuEntriesXSkew;
float MenuEntriesTargetWidth;
float SkyInStartProgress;
float SkyOutStartProgress;
float SkyMoveDurationIn;
float SkyMoveDurationOut;
float EntriesMoveDurationIn;
float EntriesMoveDurationOut;
float HighlightDurationIn;
float HighlightDurationOut;
float ButtonBackgroundStartX;
float ButtonBackgroundX;
float ButtonBackgroundY;
float ButtonBackgroundTargetWidth;
float ButtonBackgroundSprStartX;

Impacto::MainMenu::MainMenuBase* Configure() {
  Impacto::RNE::MainMenu* result = new Impacto::RNE::MainMenu();

  EnsurePushMemberOfType("Background", kObjectType);
  result->BackgroundAnimation = RNE::ParseTileBackground();
  Pop();

  ButtonBackgroundSprite = EnsureGetMemberSprite("ButtonBackgroundSprite");
  ButtonPromptsSprite = EnsureGetMemberSprite("ButtonPromptsSprite");
  SkyBackgroundSprite = EnsureGetMemberSprite("SkyBackgroundSprite");
  SkyArrowSprite = EnsureGetMemberSprite("SkyArrowSprite");
  SkyTextSprite = EnsureGetMemberSprite("SkyTextSprite");
  SkyBackgroundBeginX = EnsureGetMemberFloat("SkyBackgroundBeginX");
  SkyBackgroundY = EnsureGetMemberFloat("SkyBackgroundY");
  SkyTextBeginX = EnsureGetMemberFloat("SkyTextBeginX");
  SkyTextY = EnsureGetMemberFloat("SkyTextY");
  MenuEntriesXSkew = EnsureGetMemberFloat("MenuEntriesXSkew");
  MenuEntriesTargetWidth = EnsureGetMemberFloat("MenuEntriesTargetWidth");
  SkyInStartProgress = EnsureGetMemberFloat("SkyInStartProgress");
  SkyOutStartProgress = EnsureGetMemberFloat("SkyOutStartProgress");
  SkyMoveDurationIn = EnsureGetMemberFloat("SkyMoveDurationIn");
  SkyMoveDurationOut = EnsureGetMemberFloat("SkyMoveDurationOut");
  EntriesMoveDurationIn = EnsureGetMemberFloat("EntriesMoveDurationIn");
  EntriesMoveDurationOut = EnsureGetMemberFloat("EntriesMoveDurationOut");
  HighlightDurationIn = EnsureGetMemberFloat("HighlightDurationIn");
  HighlightDurationOut = EnsureGetMemberFloat("HighlightDurationOut");
  ButtonBackgroundStartX = EnsureGetMemberFloat("ButtonBackgroundStartX");
  ButtonBackgroundX = EnsureGetMemberFloat("ButtonBackgroundX");
  ButtonBackgroundY = EnsureGetMemberFloat("ButtonBackgroundY");
  ButtonBackgroundTargetWidth =
      EnsureGetMemberFloat("ButtonBackgroundTargetWdith");
  ButtonBackgroundSprStartX = EnsureGetMemberFloat("ButtonBackgroundSprStartX");
  GetMemberSpriteArray(MenuEntriesHSprites, MenuEntriesNum,
                       "MenuEntriesHighlightedSprites");
  GetMemberSpriteArray(MenuEntriesSprites, MenuEntriesNum,
                       "MenuEntriesSprites");

  result->SkyMoveAnimation.DurationIn = SkyMoveDurationIn;
  result->SkyMoveAnimation.DurationOut = SkyMoveDurationOut;

  result->EntriesMoveAnimation.DurationIn = EntriesMoveDurationIn;
  result->EntriesMoveAnimation.DurationOut = EntriesMoveDurationOut;

  result->HighlightAnimation.DurationIn = HighlightDurationIn;
  result->HighlightAnimation.DurationOut = HighlightDurationOut;

  return result;
}

}  // namespace MainMenu
}  // namespace RNE
}  // namespace Profile
}  // namespace Impacto