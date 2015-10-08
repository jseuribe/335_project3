class soldierPair
{
	private:
	int iD;
	int position;

	public:

	soldierPair()//default construct
	{
		iD = -1;
	}

	bool operator!=(const soldierPair rhs) const
	{
		if( iD != rhs.iD) return true;
		else return false;
	}

	int return_iD() {return iD;}

	int return_Position() {return position;}

	void set_iD(int inp) {iD = inp;}
	void set_Position(int inp) {position = inp;}

};
