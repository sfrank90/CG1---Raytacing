//#ifndef __TEXTURE_H__
//#define __TEXTURE_H__
//
//#include <string>
//#include <glm/glm.hpp>
//
//class Texture {
//public:
//	enum eTextureMode {REPEAT, CLAMP_TO_EDGE};
//	Texture() : mMode(REPEAT) {}
//	virtual ~Texture() {}
//
//	bool isLoaded() const { return !mImage.isNull(); }
//   
//	bool load(const std::string &filename);
//	//filter methods to right color
//    glm::vec4 filterNearest(float u, float v) const;
//    glm::vec4 filterLinear(float u, float v) const;
//protected:
//	QImage mImage;
//	eTextureMode mMode;
//};
//
//#endif