#include "Vec3f.h"
#include "Triangle.h"
#include <iostream>

std::ostream& operator<<(std::ostream& ostr, cg1::Vec3f const& v)  { 
	ostr << "( " << v.x << " | " << v.y << " | " << v.z << " )";
	return ostr;
}

int main() {
	float pArray[] = {2.0f,2.0f,0.0f};

	cg1::Vec3f v1(1.0f),
		       v2(0.0f, -1.0f, 0.0f),
			   v3(pArray);

	std::cout << "Vectors:" << std::endl
		      << "v1 = " << v1 << std::endl
			  << "v2 = " << v2 << std::endl
			  << "v3 = " << v3 << std::endl;

	std::cout << "v1 + v2 = " << v1+v2 << std::endl;
	std::cout << "v2 - v3 = " << v2-v3 << std::endl;
	std::cout << "v1 * v3 (dot) = " << v1*v3 << std::endl;
	std::cout << "Length of v3 = " << v3.length() << std::endl << std::endl;


	std::cout << "v1,v2,v3 are defining a triangle t. " << std::endl;
	cg1::Triangle t(v1,v2,v3);
	std::cout << "Normal vector of t: " << t.getNormal() << std::endl;

	return 0;
}