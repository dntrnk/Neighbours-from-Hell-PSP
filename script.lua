require("src/lib/NFHSound")

require("src/objects/neighbour")
require("src/objects/woody")

cameraX, cameraY = 0, 0

nextScene = "src/scenes/menu/madebydntrnk.lua"

while true do
    dofile(nextScene)
    System.GC()
end
