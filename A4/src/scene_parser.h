#ifndef _SceneParser_H_
#define _SceneParser_H_

#include "vectors.h"
#include <assert.h>
#include "scene_parser.h"
#include "matrix.h"
#include "camera.h" 
#include "light.h"
#include "material.h"
//#include "object3d.h"
#include "group.h" 
//#include "sphere.h"
//#include "plane.h"
//#include "triangle.h"

#define MAX_PARSER_TOKEN_LENGTH 100

// ====================================================================
// ====================================================================

class SceneParser {

public:

  // CONSTRUCTOR & DESTRUCTOR
  SceneParser(const char *filename);
  ~SceneParser(){
      if(camera!=NULL){
          delete camera;
          camera=NULL;
        }
        if(group!=NULL){
            delete group;
            group=NULL;
        }
        int i;
  for (i = 0; i < num_materials; i++) {
    delete materials[i]; }
  delete [] materials;
  for (i = 0; i < num_lights; i++) {
    delete lights[i]; }
  delete [] lights;
    }

  // ACCESSORS
  Camera* getCamera() const { return camera; }
  Vec3f getBackgroundColor() const { return background_color; }
    int getNumLights() const { return num_lights; }
  Light* getLight(int i) const { 
    assert(i >= 0 && i < num_lights); 
    return lights[i]; }  
  int getNumMaterials() const { return num_materials; }
  Material* getMaterial(int i) const { 
    assert(i >= 0 && i < num_materials); 
    return materials[i]; }  
Vec3f getAmbientLight() const { return ambient_light; }

  Group* getGroup() const { return group; }

private:

  SceneParser() { assert(0); } // don't use

  // PARSING
  void parseFile();
  /* parseOrthographicCamera();
  void parsePerspectiveCamera();*/
  void parseCamera();
  void parseBackground();
  void parseLights();
   Light* parseDirectionalLight();
  Light* parsePointLight();
  void parseMaterials();
  Material* parsePhongMaterial();
  Group* parseGroup();
  /*Sphere* parseSphere();
  Plane* parsePlane();
  Triangle* parseTriangle();*/
  void parseMesh(Group* group,int idx);
  void parseTransform(Group* group);

  // HELPER FUNCTIONS
  int getToken(char token[MAX_PARSER_TOKEN_LENGTH]);
  Vec3f readVec3f();
  Vec2f readVec2f();
  float readFloat();
  int readInt();

  // ==============
  // REPRESENTATION
  FILE *file;
  Camera *camera;
  Vec3f background_color;
  Vec3f ambient_light;
    int num_lights;
  Light **lights;
  int num_materials;
  Material **materials;
  Material *current_material;
  Group *group;
};

// ====================================================================
// ====================================================================

#endif