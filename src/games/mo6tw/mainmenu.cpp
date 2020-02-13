#include "mainmenu.h"

#include "../../profile/hud/mainmenu.h"
#include "../../profile/games/mo6tw/mainmenu.h"
#include "../../renderer2d.h"
#include "../../mem.h"
#include "../../profile/scriptvars.h"
#include "../../inputsystem.h"

namespace Impacto {
namespace MO6TW {

using namespace Impacto::Profile::MainMenu;
using namespace Impacto::Profile::MO6TW::MainMenu;
using namespace Impacto::Profile::ScriptVars;

int CurrentChoice = 0;

void MainMenu::Show() {
  if (State != Shown) {
    State = Shown;
  }
}
void MainMenu::Hide() {
  if (State != Hidden) {
    State = Hidden;
  }
}
void MainMenu::Update(float dt) {
  if (GetFlag(SF_TITLEMODE)) {
    Show();
  } else {
    Hide();
  }
  if (ScrWork[SW_SYSMENUCT] == 100 && State == Shown) {
    Hide();
  }

  if (State == Shown) {
    if (Input::KeyboardButtonWentDown[SDL_SCANCODE_DOWN])
      CurrentChoice++;
    else if (Input::KeyboardButtonWentDown[SDL_SCANCODE_UP])
      CurrentChoice--;
    if (CurrentChoice < 0)
      CurrentChoice = 7;
    else if (CurrentChoice > 7)
      CurrentChoice = 0;
    ScrWork[SW_SYSMENUCNO] = CurrentChoice;
  }

  MenuEntriesHSprites[CurrentChoice].Bounds.Width =
      MenuEntriesTargetWidth;
}
void MainMenu::Render() {
  if (State != Hidden && ScrWork[SW_SYSMENUALPHA] > 0) {
    glm::vec4 black(0.0f);
    for (int i = 0; i < MenuEntriesNum; i++) {
      black.a = glm::smoothstep(
          0.0f, 1.0f, 1.0f - (i + 1) * (1.0f));
      Renderer2D::DrawSprite(
          MenuEntriesSprites[i],
          glm::vec2(MenuEntriesX,
                    MenuEntriesFirstY + (i * MenuEntriesYPadding)));
    }

    Renderer2D::DrawSprite(
          MenuEntriesHSprites[CurrentChoice],
          glm::vec2(MenuEntriesX,
                    MenuEntriesFirstY + (CurrentChoice * MenuEntriesYPadding)));
    
  }
}

}  // namespace MO6TW
}  // namespace Impacto