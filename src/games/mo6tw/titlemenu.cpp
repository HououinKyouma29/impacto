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
}
void TitleMenu::Render() {
  if (State != Hidden && GetFlag(SF_TITLEMODE)) {
    glm::vec4 col = glm::vec4(1.0f);
    col.a = glm::smoothstep(0.0f, 1.0f, PressToStartAnimation.Progress);
    Renderer2D::DrawSprite(BackgroundSprite, glm::vec2(0.0f));
    Renderer2D::DrawSprite(PressToStartSprite,
                           glm::vec2(PressToStartX, PressToStartY), col);
    Renderer2D::DrawSprite(LogoSprite, glm::vec2(LogoX, LogoY));
    Renderer2D::DrawSprite(CopyrightSprite, glm::vec2(CopyrightX, CopyrightY));
  }
}

}  // namespace mo6tw
}  // namespace Impacto