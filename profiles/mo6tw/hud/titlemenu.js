root.TitleMenu = {
    Type: TitleMenuType.MO6TW,
    PressToStartX:506,
    PressToStartY: 404,
    PressToStartAnimDurationIn:0.5,
    PressToStartAnimDurationOut: 0.5,
    PressToStartSprite:"TitleMenuPressToStart",
    BackgroundSprite: "TitleMenuBackground",
    CopyrightSprite: "TitleMenuCopyright",
    LogoSprite: "TitleMenuLogo",
    CopyrightX:468,
    CopyrightY: 663,
    LogoX: 342,
    LogoY: 32
};

root.Sprites["TitleMenuBackground"] = {
    Sheet: "Titlebg",
    Bounds: { X: 0, Y: 0, Width: 1280, Height: 720 },

};

root.Sprites["TitleMenuPressToStart"] = {
    Sheet: "Title",
    Bounds: { X: 264, Y: 194, Width: 290, Height: 30 },
    
};

root.Sprites["TitleMenuCopyright"] = {
    Sheet: "Title",
    Bounds: { X: 3, Y: 229, Width: 345, Height: 24 },
    
};

root.Sprites["TitleMenuLogo"] = {
    Sheet: "Title",
    Bounds: { X: 18, Y: 1, Width: 608, Height: 189 },
   
};