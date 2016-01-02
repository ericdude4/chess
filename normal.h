class normal {
	/*
		this class stores the normal of a triangle
	*/
private:
	float i, j, k;
public:
	normal() {}
	
	void setIJK (float i_in, float j_in, float k_in) {
		i = i_in;
		j = j_in;
		k = k_in;
	}

	float getI() { return i; }
	float getJ() { return j; }
	float getK() { return k; }
};