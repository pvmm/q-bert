OUTPUT := ../generated
GENERATED := $(OUTPUT)/tiles.h $(OUTPUT)/player.h $(OUTPUT)/enemy_ball.h $(OUTPUT)/map.h

.DELETE_ON_ERROR: $(GENERATED)
all: $(GENERATED)

$(OUTPUT)/tiles.h: tiles.png
	../../tools/png2tiles.py -i tiles --preferred-fg 15 $< > $@

$(OUTPUT)/player.h: player.png
	../../tools/png2sprites.py -c -i player_sprite $< > $@

$(OUTPUT)/enemy_ball.h: enemy_ball.png
	../../tools/png2sprites.py -c -i enemy_ball_sprite $< > $@

$(OUTPUT)/map.h: map.json map_conf.json
	../../tools/map.py --aplib --max-ents 11 --room-height 24 map.json map > $@

.PHONY: all clean
clean:
	rm -f $(OUTPUT)/*

