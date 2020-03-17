#include "titlemenu.h"

#include "../../profile/hud/titlemenu.h"
#include "../../profile/games/mo6tw/titlemenu.h"
#include "../../renderer2d.h"
#include "../../texture/texture.h"
#include "../../spritesheet.h"
#include "../../io/vfs.h"
#include "../../mem.h"
#include "../../profile/scriptvars.h"
#include "../../profile/game.h"
#include "../../inputsystem.h"

namespace Impacto {
namespace MO6TW {

using namespace Impacto::Profile::TitleMenu;
using namespace Impacto::Profile::MO6TW::TitleMenu;
using namespace Impacto::Profile::ScriptVars;

int SecondaryChoiceCount = 0;

void TitleMenu::Show() {
  if (State != Shown) {
    State = Shown;
  }
}
void TitleMenu::Hide() {
  if (State != Hidden) {
    State = Hidden;
  }
}
void TitleMenu::Update(float dt) {
  PressToStartAnimation.Update(dt);
  if (GetFlag(SF_TITLEMODE)) {
    Show();
  } else {
    Hide();
  }

  if (State == Shown) {
    if (PressToStartAnimation.State == AS_Stopped)
      PressToStartAnimation.StartIn();
  }

  if (Input::KeyboardButtonWentDown[SDL_SCANCODE_DOWN]) {
    if (ScrWork[SW_TITLEDISPCT] >= 7) {
      SecondaryChoice++;
      if (SecondaryChoice > SecondaryChoiceCount) SecondaryChoice = 0;
    } else {
      CurrentChoice++;
      if (CurrentChoice > 6) CurrentChoice = 0;
    }
  } else if (Input::KeyboardButtonWentDown[SDL_SCANCODE_UP]) {
    if (ScrWork[SW_TITLEDISPCT] >= 7) {
      SecondaryChoice--;
      if (SecondaryChoice < 0) SecondaryChoice = SecondaryChoiceCount;
    } else {
      CurrentChoice--;
      if (CurrentChoice < 0) CurrentChoice = 6;
    }
  }
}
void TitleMenu::Render() {
  if (State != Hidden && GetFlag(SF_TITLEMODE)) {
    int maskAlpha = ScrWork[SW_TITLEMASKALPHA];
    glm::vec4 col = ScrWorkGetColor(SW_TITLEMASKCOLOR);
    col.a = glm::min(maskAlpha / 255.0f, 1.0f);
    Renderer2D::DrawRect(
        RectF(0.0f, 0.0f, Profile::DesignWidth, Profile::DesignHeight), col);

    if (ScrWork[SW_TITLEDISPCT] == 0)
      DrawTitleMenuBackGraphics();
    else
      DrawTitleMenuItems();
  }
}

void TitleMenu::DrawTitleMenuBackGraphics() {
  glm::vec4 col = glm::vec4(1.0f);
  col.a = glm::smoothstep(0.0f, 1.0f, PressToStartAnimation.Progress);
  Renderer2D::DrawSprite(BackgroundSprite, glm::vec2(0.0f));
  Renderer2D::DrawSprite(LogoSprite, glm::vec2(LogoX, LogoY));
  Renderer2D::DrawSprite(CopyrightSprite, glm::vec2(CopyrightX, CopyrightY));
  Renderer2D::DrawSprite(PressToStartSprite,
                         glm::vec2(PressToStartX, PressToStartY), col);
}

void TitleMenu::DrawTitleMenuItems() {
  Renderer2D::DrawSprite(MenuBackgroundSprite, glm::vec2(0.0f));
  glm::vec4 black(0.0f);
  for (int i = 0; i < MenuEntriesNum; i++) {
    black.a = glm::smoothstep(0.0f, 1.0f, 1.0f - (i + 1) * (1.0f));
    Renderer2D::DrawSprite(
        MenuEntriesSprites[i],
        glm::vec2(MenuEntriesX, MenuEntriesFirstY + (i * MenuEntriesYPadding)));
  }

  Renderer2D::DrawSprite(
      MenuEntriesHSprites[CurrentChoice],
      glm::vec2(MenuEntriesX,
                MenuEntriesFirstY + (CurrentChoice * MenuEntriesYPadding)));
}

}  // namespace MO6TW
}  // namespace Impacto