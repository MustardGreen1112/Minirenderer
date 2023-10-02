#ifndef _MINIRENDERER_MODEL_H_
#define _MINIRENDERER_MODEL_H_
#include "mesh.h"
#include <vector>
class Model
{
public:
	Model();
	~Model();
	inline int GetMeshCount() { return m_pmesharr.size();}
	void AddMesh(Mesh* pmesh);
	static Model LoadModel(const char* filename);
	std::vector<Mesh*> m_pmesharr;
};



#endif // !_MINIRENDERER_MODEL_H_
