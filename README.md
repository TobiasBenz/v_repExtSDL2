# v_repExtSDL2
SDL2 plugin for Vrep

Implementation of the following SDL functions:
General functions:

	bool init_sdl(bool USE_HAPTIC);
	bool quit_sdl();
	bool refresh();
	bool isPressed(int button);
	int numButtons();
	int hatPosition();

Haptic functions:

	bool createDirectionalEffect(int dir_deg, int level);
	bool createGroovesEffect(int dir_deg, int level, int length);
	bool createRumbleEffect();
	bool playRumbleEffect(int strength, int duration);
	bool updateDirEffect(int dir_deg, int level);
	bool updateGroovesEffect(int dir_deg, int level, int length);
	bool playDirEffect();
	bool playGroovesEffect();
	bool destroyDirEffect();
	bool destroyGroovesEffect();
	bool stopDirEffect();
	bool stopGroovesEffect();

Easy to extend and supposed to be cross-platfrom.

It is imporant to have link SDL2 and put SDL.dll into VRep folder.
