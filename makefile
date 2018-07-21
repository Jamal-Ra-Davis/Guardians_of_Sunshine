OBJS = 	./src/GuardiansOfSunshine.o ./src/SDL_Helpers.o ./src/GamePanel.o ./src/GameStateManager.o ./src/GameState.o ./src/Background.o\
		./src/TileMap.o ./src/Tile.o ./src/MapObject.o ./src/Animation.o ./src/Player.o ./src/FireBall.o ./src/Enemy.o ./src/Slugger.o\
		./src/HUD.o ./src/Explosion.o ./src/AudioPlayer.o ./src/BgObject.o ./src/GoS_Player.o ./src/Coin.o ./src/TriggerBox.o\
		./src/GoS_HUD.o ./src/Numbers.o ./src/Bomba.o ./src/BouncyBee.o ./src/Poof.o ./src/HoneyBunny.o ./src/SleepySam.o ./src/ComboMove.o\
		./src/InputBuffer.o ./src/Letters.o ./src/GoS_StartMenu.o

CC = g++
DEBUG = -g
#CFLAGS = -Wall -c $(DEBUG)
CFLAGS = -Wall $(DEBUG)
#LFLAGS = -Wall $(DEBUG)
LFLAGS = -lSDL2 -lSDL2_mixer
DS = ./src/
DH = ./headers/

#g++ SpriteAnimation.cpp -w -lSDL2 -o SpriteAnimation


GuardiansOfSunshine: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LFLAGS) -o GuardiansOfSunshine

#enemy.o: $(DS)enemy.cpp $(DH)enemy.h $(DH)skill.h $(DH)FileFunctions.h
#	$(CC) $(CFLAGS) $(DS)enemy.cpp -o $(DS)enemy.o

#player.o: $(DS)player.cpp $(DH)player.h $(DH)enemy.h $(DH)skill.h $(DH)item.h\
			 $(DH)Safe_Input.h
#	$(CC) $(CFLAGS) $(DS)player.cpp -o $(DS)player.o

SDL_Helpers.o: $(DS)SDL_Helpers.cpp $(DH)SDL_Helpers.h
	$(CC) -c $(CFLAGS) $(DS)SDL_Helpers.cpp -o $(DS)SDL_Helpers.o

GamePanel.o: $(DS)GamePanel.cpp $(DH)GamePanel.h $(DH)AudioPlayer.h
	$(CC) -c $(CFLAGS) $(DS)GamePanel.cpp -o $(DS)GamePanel.o

GameStateManager.o: $(DS)GameStateManager.cpp $(DH)GameStateManager.h $(DH)GameState.h
	$(CC) -c $(CFLAGS) $(DS)GameStateManager.cpp -o $(DS)GameStateManager.o

Background.o: $(DS)Background.cpp $(DH)Background.h $(DH)GamePanel.h $(DH)SDL_Helpers.h
	$(CC) -c $(CFLAGS) $(DS)Background.cpp -o $(DS)Background.o

GameState.o: $(DS)GameState.cpp $(DH)GameStateManager.h $(DH)GameState.h $(DH)Background.h\
			 $(DH)TileMap.h $(DH)SDL_Helpers.h $(DH)Player.h $(DH)Enemy.h $(DH)Slugger.h\
			 $(DH)HUD.h $(DH)Explosion.h $(DH)AudioPlayer.h $(DH)BgObject.h $(DH)GoS_Player.h\
			 $(DH)Coin.h $(DH)TriggerBox.h $(DH)GoS_HUD.h $(DH)BouncyBee.h $(DH)Poof.h\
			 $(DH)HoneyBunny.h $(DH)SleepySam.h $(DH)ComboMove.h $(DH)Letters.h
	$(CC) -c $(CFLAGS) $(DS)GameState.cpp -o $(DS)GameState.o

Tile.o: $(DS)Tile.cpp $(DH)Tile.h
	$(CC) -c $(CFLAGS) $(DS)Tile.cpp -o $(DS)Tile.o

TileMap.o: $(DS)TileMap.cpp $(DH)TileMap.h $(DH)Tile.h $(DH)GamePanel.h $(DH)SDL_Helpers.h
	$(CC) -c $(CFLAGS) $(DS)TileMap.cpp -o $(DS)TileMap.o

MapObject.o: $(DS)MapObject.cpp $(DH)MapObject.h $(DH)TileMap.h $(DH)Tile.h $(DH)GamePanel.h $(DH)SDL_Helpers.h\
			$(DH)Animation.h
	$(CC) -c $(CFLAGS) $(DS)MapObject.cpp -o $(DS)MapObject.o

Animation.o: $(DS)Animation.cpp $(DH)Animation.h $(DH)SDL_Helpers.h
	$(CC) -c $(CFLAGS) $(DS)Animation.cpp -o $(DS)Animation.o

Player.o: $(DS)Player.cpp $(DH)Player.h $(DH)MapObject.h $(DH)SDL_Helpers.h $(DH)FireBall.h\
		  $(DH)Enemy.h $(DH)AudioPlayer.h
	$(CC) -c $(CFLAGS) $(DS)Player.cpp -o $(DS)Player.o

FireBall.o: $(DS)FireBall.cpp $(DH)FireBall.h $(DH)MapObject.h $(DH)SDL_Helpers.h
	$(CC) -c $(CFLAGS) $(DS)FireBall.cpp -o $(DS)FireBall.o

Enemy.o: $(DS)Enemy.cpp $(DH)Enemy.h $(DH)MapObject.h $(DH)SDL_Helpers.h $(DH)Bomba.h $(DH)TriggerBox.h
	$(CC) -c $(CFLAGS) $(DS)Enemy.cpp -o $(DS)Enemy.o

Slugger.o: $(DS)Slugger.cpp $(DH)Slugger.h $(DH)MapObject.h $(DH)SDL_Helpers.h $(DH)Enemy.h
	$(CC) -c $(CFLAGS) $(DS)Slugger.cpp -o $(DS)Slugger.o

HUD.o: $(DS)HUD.cpp $(DH)HUD.h $(DH)Player.h $(DH)MapObject.h $(DH)SDL_Helpers.h $(DH)FireBall.h
	$(CC) -c $(CFLAGS) $(DS)HUD.cpp -o $(DS)HUD.o

Explosion.o: $(DS)Explosion.cpp $(DH)Explosion.h $(DH)SDL_Helpers.h $(DH)Animation.h
	$(CC) -c $(CFLAGS) $(DS)Explosion.cpp -o $(DS)Explosion.o

AudioPlayer.o: $(DS)AudioPlayer.cpp $(DH)AudioPlayer.h
	$(CC) -c $(CFLAGS) $(DS)AudioPlayer.cpp -o $(DS)AudioPlayer.o

BgObject.o: $(DS)BgObject.cpp $(DH)BgObject.h $(DH)MapObject.h $(DH)SDL_Helpers.h
	$(CC) -c $(CFLAGS) $(DS)BgObject.cpp -o $(DS)BgObject.o

GoS_Player.o: $(DS)GoS_Player.cpp $(DH)GoS_Player.h $(DH)MapObject.h $(DH)SDL_Helpers.h\
		  $(DH)Enemy.h $(DH)AudioPlayer.h $(DH)Bomba.h
	$(CC) -c $(CFLAGS) $(DS)GoS_Player.cpp -o $(DS)GoS_Player.o

Coin.o: $(DS)Coin.cpp $(DH)Coin.h $(DH)MapObject.h $(DH)SDL_Helpers.h $(DH)AudioPlayer.h
	$(CC) -c $(CFLAGS) $(DS)Coin.cpp -o $(DS)Coin.o

TriggerBox.o: $(DS)TriggerBox.cpp $(DH)TriggerBox.h $(DH)MapObject.h $(DH)SDL_Helpers.h
	$(CC) -c $(CFLAGS) $(DS)TriggerBox.cpp -o $(DS)TriggerBox.o

GoS_HUD.o: $(DS)GoS_HUD.cpp $(DH)GoS_HUD.h $(DH)GoS_Player.h $(DH)SDL_Helpers.h $(DH)Numbers.h 
	$(CC) -c $(CFLAGS) $(DS)GoS_HUD.cpp -o $(DS)GoS_HUD.o

Numbers.o: $(DS)Numbers.cpp $(DH)Numbers.h $(DH)SDL_Helpers.h
	$(CC) -c $(CFLAGS) $(DS)Numbers.cpp -o $(DS)Numbers.o

Bomba.o: $(DS)Bomba.cpp $(DH)Bomba.h $(DH)MapObject.h $(DH)SDL_Helpers.h
	$(CC) -c $(CFLAGS) $(DS)Bomba.cpp -o $(DS)Bomba.o

BouncyBee.o: $(DS)BouncyBee.cpp $(DH)BouncyBee.h $(DH)MapObject.h $(DH)SDL_Helpers.h $(DH)Enemy.h\
			 $(DH)AudioPlayer.h
	$(CC) -c $(CFLAGS) $(DS)BouncyBee.cpp -o $(DS)BouncyBee.o

Poof.o: $(DS)Poof.cpp $(DH)Poof.h $(DH)SDL_Helpers.h $(DH)Animation.h $(DH)Numbers.h
	$(CC) -c $(CFLAGS) $(DS)Poof.cpp -o $(DS)Poof.o

HoneyBunny.o: $(DS)HoneyBunny.cpp $(DH)HoneyBunny.h $(DH)MapObject.h $(DH)SDL_Helpers.h $(DH)Enemy.h\
			  $(DH)AudioPlayer.h
			  $(DH)Bomba.h
	$(CC) -c $(CFLAGS) $(DS)HoneyBunny.cpp -o $(DS)HoneyBunny.o

SleepySam.o: $(DS)SleepySam.cpp $(DH)SleepySam.h $(DH)MapObject.h $(DH)SDL_Helpers.h $(DH)Enemy.h\
			 $(DH)ComboMove.h $(DH)AudioPlayer.h
	$(CC) -c $(CFLAGS) $(DS)SleepySam.cpp -o $(DS)SleepySam.o

ComboMove.o: $(DS)ComboMove.cpp $(DH)ComboMove.h $(DH)SDL_Helpers.h $(DH)Animation.h $(DH)InputBuffer.h
	$(CC) -c $(CFLAGS) $(DS)ComboMove.cpp -o $(DS)ComboMove.o

InputBuffer.o: $(DS)InputBuffer.cpp $(DH)InputBuffer.h
	$(CC) -c $(CFLAGS) $(DS)InputBuffer.cpp -o $(DS)InputBuffer.o

Letters.o: $(DS)Letters.cpp $(DH)Letters.h $(DH)SDL_Helpers.h
	$(CC) -c $(CFLAGS) $(DS)Letters.cpp -o $(DS)Letters.o

GoS_StartMenu.o: $(DS)GoS_StartMenu.cpp $(DH)GoS_StartMenu.h
	$(CC) -c $(CFLAGS) $(DS)GoS_HUD.cpp -o $(DS)GoS_HUD.o

GuardiansOfSunshine.o: $(DS)GuardiansOfSunshine.cpp $(DH)SDL_Helpers.h $(DH)GamePanel.h
	$(CC) -c $(CFLAGS) $(DS)GuardiansOfSunshine.cpp -o $(DS)GuardiansOfSunshine.o


clean:
	rm $(OBJS)

