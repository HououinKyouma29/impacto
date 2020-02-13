#include "titlemenu.h"
#include "../../../log.h"
#include "../../../window.h"
#include "../../profile_internal.h"

namespace Impacto {
namespace Profile {
namespace MO6TW {
namespace TitleMenu {

Sprite BackgroundSprite;
Sprite CopyrightSprite;
Sprite LogoSprite;

float LogoX;
float LogoY;
float CopyrightX;
float CopyrightY;

Impacto::TitleMenu::TitleMenuBase* Configure() {
  Impacto::MO6TW::TitleMenu* result = new Impacto::MO6TW::TitleMenu();

  BackgroundSprite = EnsureGetMemberSprite("BackgroundSprite");
  CopyrightSprite = EnsureGetMemberSprite("CopyrightSprite");
  LogoSprite = EnsureGetMemberSprite("LogoSprite");
  LogoX = EnsureGetMemberFloat("LogoX");
  LogoY = EnsureGetMemberFloat("LogoY");
  CopyrightX = EnsureGetMemberFloat("CopyrightX");
  CopyrightY = EnsureGetMemberFloat("CopyrightY");

      return result;
}

}  // namespace TitleMenu
}  // namespace MO6TW
}  // namespace Profile
}  // namespace Impacto