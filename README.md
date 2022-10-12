# Q*Bert

warning: SDCC 4.1.0 is buggy, use newer compiler version.

TODO:

 - "Pyramid" boundary check;
 - Frenemies
   - green ball (green, pickable, freezes enemies)
   - red balls (jumps down randomly until end of screen starting from any of the blocks
     from the second tier)
   - Coily (purple, homing, snake)
   - Sam (green, change colors back, talks gibberish, pickable)
   - Slick (green, shades, change colors back, talks gibberish, pickable)
   - Wrong Way (purple, side runner, talks gibberish)
   - Ugg (purple, horny dragon type, side runner, talks gibberish)
 - Escape disk
 - New level introduction screen
 - Original music
 - Detect enemy hit
 - Q*Bert when it hits enemy: talks gibberish with text baloon
 - Possible extra game modes
   - Competitive/Collaborative two players mode?
   - never-ending scrollable mode?
   - Story mode?

## External tools

 - [Tiled](https://www.mapeditor.org/) was used to edit the game map, via JSON
   export (see `map.json`)
 - All the graphics were drawn with [GIMP](https://www.gimp.org/)
 - The music engine is the [lvgm player](https://github.com/aoineko-fr/MSXgl/tree/main/engine/src/vgm)
   modified to work with Ubox
