require("src/objects/woody")

cameraX, cameraY = 0, 0

nextScene = "src/scenes/levels/s0ep1.lua"

while true do
    dofile(nextScene)
    System.GC()
end
