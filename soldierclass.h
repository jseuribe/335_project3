class soldier
{
	public:

		soldier() { faction = false; lifePoints = 0; actionTime = 0; iD = -1;}//default construct

    
		soldier( const soldier& source)//Cpy construct
		{ 
			faction = source.faction;
			lifePoints = source.lifePoints;
			actionTime = source.actionTime;
			iD = source.iD;
		}

		soldier& operator=(const soldier& rhs)
		{

    			if(this != &rhs)
    			{

        			faction = rhs.faction; //1=Spartan, 0=Persian
				lifePoints = rhs.lifePoints;//3 for Spartans, 1 for Persians
				actionTime = rhs.actionTime;//time until they act
				iD = rhs.iD; //1-300 for spartans, 301-52999 for Persians

    			}
    			else
    			{
       				return *this;//If we're just copying the same thing to the same thing, the calling object is returned.
   			}
		}


		bool operator<(const soldier soldier_two) const
		{
			if(actionTime < soldier_two.actionTime) return true;
			else return false;
		}

		bool operator==(const soldier soldier_two) const
		{
			if(actionTime == soldier_two.actionTime) return true;	
			else return false;

		}

		soldier(bool teamParameter, int actionTime) 
			: faction{teamParameter} , actionTime(actionTime) {
			if(faction == 1) lifePoints = 3;
			else lifePoints = 1;}

		void takeHit(int damageDealt) 
		{
			lifePoints -= damageDealt;
		}

		bool return_Faction() {return faction;}

		int return_actionTime() {return actionTime;}

		int return_iD() {return iD;}

		int return_lifePoints() {return lifePoints;}

		void set_Faction(bool input)
		{
			faction = input;

			if(faction == true) lifePoints = 3;
			else lifePoints = 1;
		}

		void set_actionTime(int input){ actionTime = input;}
		
		void set_iD(int input){iD = input;}

	private:
		bool faction; //1=Spartan, 0=Persian
		int lifePoints;//3 for Spartans, 1 for Persians
		int actionTime;//time until they act
		int iD; //1-300 for spartans, 301-52999 for Persians
	
};

