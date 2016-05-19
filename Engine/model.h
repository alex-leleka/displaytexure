// Tiny Renderer, https://github.com/ssloy/tinyrenderer
// Copyright Dmitry V. Sokolov
/*
	displaytexure demo https://github.com/alex-leleka/displaytexure 
	alex leleka (c) 2016
*/
#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "geometry.h"

class Model {
private:
    std::vector<Vec3f> verts_;
    std::vector<std::vector<Vec3i> > faces_; // attention, this Vec3i means vertex/uv/normal
    std::vector<Vec3f> norms_;
    std::vector<Vec2f> uv_;
    void load_texture(std::string filename, const char *suffix);
public:
    Model(const char *filename);
    ~Model();
    int nverts();
    int nfaces();
    Vec3f norm(int iface, int nvert);
    Vec3f vert(int i);
    Vec2f uv(int iface, int nvert);
    std::vector<int> face(int idx);
};

#endif //__MODEL_H__
