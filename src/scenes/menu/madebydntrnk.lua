local dntrnkSprite = Image.load("assets/sprites/menu/dntrnk.jpg")
local dntrnkTextSprite = Image.load("assets/sprites/menu/dntrnkText.png")

for i = 1, 200 do
    screen.clear()
    
    Image.draw(dntrnkSprite, 176, 72)
    Image.draw(dntrnkTextSprite, 198, 216)

    screen.flip()
end

Image.unload(dntrnkSprite)
Image.unload(dntrnkTextSprite)

nextScene = "src/scenes/levels/s1ep1.lua"
