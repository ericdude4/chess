class face {
	/*
		this class stores a face by referencing the corresponding
		vertices, normals and textures
	*/
private:
	int a1, b1, a2, b2, a3, b3;
public:
	face() {}
	
	void setAB (int a1_in, int b1_in,
		int a2_in, int b2_in,
		int a3_in, int b3_in) {
		a1 = a1_in;
		b1 = b1_in;
		a2 = a2_in;
		b2 = b2_in;
		a3 = a3_in;
		b3 = b3_in;
	}

	float getA1() { return a1; }
	float getB1() { return b1; }
	float getA2() { return a2; }
	float getB2() { return b2; }
	float getA3() { return a3; }
	float getB3() { return b3; }
};