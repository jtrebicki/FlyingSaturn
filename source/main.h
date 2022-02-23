namespace JGame
{
	//projectile class
	class Projectile {
		public:
			float posX;
			float posY;
			float size=12;
			float p_spd=0;
	};

	//the class for an enemy
	class Enemy {
	public:
		float X;
		float Y;
		float size;
		float spd=0;
	//resets the enemy (initialisation, on hitting the player, on getting hit by projectile)
	public: void reset() {
			size=(rand() % 24 + 8);
			if((rand() % 2 + 1)==1) {
				X = 0;
				spd=+(rand() % 3);
			} else {
				X = 256;
				spd=-(rand() % 3);
			}
							
			if (spd==0) {
				spd=2.5;
			}
			Y = (rand() % 192);
		}
		
	};
}