#ifndef SHADOWMAP_H
#define SHADOWMAP_H

#include "../Resource/resource_manager.h"
#include "../global_param.h"


class Shadowmap {
public:
	Texture2D DepthMap;
	GLfloat near_plane, far_plane;
	GLuint DepthMapFBO;
	Shader depthShader;
	

	Shadowmap(Shader shader);
	~Shadowmap();
	void Init();
	void BeginMakeMap();
	void EndMakeMap();
	glm::mat4 getLightSpaceMat();
};

#endif // !1
