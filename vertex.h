class vertex {
	/*
		this class stores the vertices of a triangle
	*/
private:
	
public:
	float x, y, z;
	
	vertex() {}

	void setXYZ (float x_in, float y_in, float z_in) {
		x = x_in;
		y = y_in;
		z = z_in;
	}

	float getX() { return x; }
	float getY() { return y; }
	float getZ() { return z; }
};