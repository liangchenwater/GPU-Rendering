#include <stdio.h>
#include <string.h>

#include "scene_parser.h"
#include "matrix.h"

#define DegreesToRadians(x) ((M_PI * x) / 180.0f)

// ====================================================================
// ====================================================================
// CONSTRUCTOR & DESTRUCTOR

SceneParser::SceneParser(const char* filename) {

  // initialize some reasonable default values
  group = NULL;
  camera = NULL;
  background_color = Vec3f(0.5,0.5,0.5);
  num_lights = 0;
  ambient_light = Vec3f(0,0,0);
  lights = NULL;
  num_materials = 0;
  materials = NULL;
  current_material = NULL;
  // parse the file
  assert(filename != NULL);
  const char *ext = &filename[strlen(filename)-4];
  assert(!strcmp(ext,".txt"));
  file = fopen(filename,"r");
  assert (file != NULL);
  parseFile();
  fclose(file);
  file = NULL;
  if (num_lights == 0) {
    printf ("WARNING:  No lights specified\n");
    ambient_light = Vec3f(1,1,1);
  }
}


// ====================================================================
// ====================================================================

void SceneParser::parseFile() {
  //
  // at the top level, the scene can have a camera,
  // background color and a group of objects
  // (we add lights and other things in future assignments)
  //
  char token[MAX_PARSER_TOKEN_LENGTH];
  while (getToken(token)) {
    if (!strcmp(token, "Camera")) {
      parseCamera();
    } else if (!strcmp(token, "Background")) {
      parseBackground();
    }else if (!strcmp(token, "Group")) {
      group = parseGroup();
    } else if(!strcmp(token, "Lights")) {
      parseLights();
    }else if(!strcmp(token, "Materials")) {
      parseMaterials();
} else {
      printf ("Unknown token in parseFile: '%s'\n", token);
      exit(0);
    }
  }
}

// ====================================================================
// ====================================================================

void SceneParser::parseCamera() {
  char token[MAX_PARSER_TOKEN_LENGTH];
  // read in the camera parameters
  getToken(token); assert (!strcmp(token, "{"));
  getToken(token); assert (!strcmp(token, "center"));
  Vec3f center = readVec3f();
  getToken(token); assert (!strcmp(token, "direction"));
  Vec3f direction = readVec3f();
  getToken(token); assert (!strcmp(token, "up"));
  Vec3f up = readVec3f();
  getToken(token); assert (!strcmp(token, "angle"));
  float angle_degrees = readFloat();
  float angle_radians = DegreesToRadians(angle_degrees);
  getToken(token); assert (!strcmp(token, "}"));
  camera = new Camera(center,direction,up,angle_radians);
}

void SceneParser::parseBackground() {
  char token[MAX_PARSER_TOKEN_LENGTH];
  // read in the background color
  getToken(token); assert (!strcmp(token, "{"));
  while (1) {
    getToken(token);
    if (!strcmp(token, "}")) {
      break;
    } else if (!strcmp(token, "color")) {
      background_color = readVec3f();
    } else if (!strcmp(token, "ambientLight")) {
      ambient_light = readVec3f();
    } else {
      printf ("Unknown token in parseBackground: '%s'\n", token);
      assert(0);
    }
  }
  cout<<"Background is set."<<endl;
}

void SceneParser::parseLights() {
  char token[MAX_PARSER_TOKEN_LENGTH];
  getToken(token); assert (!strcmp(token, "{"));
  // read in the number of objects
  getToken(token); assert (!strcmp(token, "numLights"));
  num_lights = readInt();
  lights = new Light*[num_lights];
  // read in the objects
  int count = 0;
  while (num_lights > count) {
    getToken(token); 
    if (!strcmp(token, "DirectionalLight")) {
      lights[count] = parseDirectionalLight();
    } else if (!strcmp(token, "PointLight")) {
      lights[count] = parsePointLight();
    } else {
      printf ("Unknown token in parseLight: '%s'\n", token); 
      exit(0);  
    }   
    count++;
  }
  getToken(token); assert (!strcmp(token, "}"));
}

Light* SceneParser::parseDirectionalLight() {
  char token[MAX_PARSER_TOKEN_LENGTH];
  getToken(token); assert (!strcmp(token, "{"));
  getToken(token); assert (!strcmp(token, "direction"));
  Vec3f direction = readVec3f();
  getToken(token); assert (!strcmp(token, "color"));
  Vec3f color = readVec3f();
  getToken(token); assert (!strcmp(token, "}"));
  return new DirectionalLight(direction,color);
}


Light* SceneParser::parsePointLight() {
  char token[MAX_PARSER_TOKEN_LENGTH];
  getToken(token); assert (!strcmp(token, "{"));
  getToken(token); assert (!strcmp(token, "position"));
  Vec3f position = readVec3f();
  getToken(token); assert (!strcmp(token, "color"));
  Vec3f color = readVec3f();
  float att[3] = { 1, 0, 0 };
  getToken(token); 
  if (!strcmp(token,"attenuation")) {
    att[0] = readFloat();
    att[1] = readFloat();
    att[2] = readFloat();
    getToken(token); 
  } 
  assert (!strcmp(token, "}"));
  return new PointLight(position,color,att[0], att[1], att[2]);
}

void SceneParser::parseMaterials() {
  char token[MAX_PARSER_TOKEN_LENGTH];
  getToken(token); assert (!strcmp(token, "{"));
  // read in the number of objects
  getToken(token); assert (!strcmp(token, "numMaterials"));
  num_materials = readInt();
  materials = new Material*[num_materials];
  // read in the objects
  int count = 0;
  while (num_materials > count) {
    getToken(token); 
    if (!strcmp(token, "Material") ||
        !strcmp(token, "PhongMaterial")) {
      materials[count] = parsePhongMaterial();
    } else {
      printf ("Unknown token in parseMaterial: '%s'\n", token); 
      exit(0);
    }
    count++;
  }
  getToken(token); assert (!strcmp(token, "}"));
}  


Material* SceneParser::parsePhongMaterial() {
  char token[MAX_PARSER_TOKEN_LENGTH];
  Vec3f diffuseColor(1,1,1);
  Vec3f specularColor(0,0,0);
  float exponent = 1;
  Vec3f reflectiveColor(0,0,0);
  Vec3f transparentColor(0,0,0);
  float indexOfRefraction = 1;
  getToken(token); assert (!strcmp(token, "{"));
  while (1) {
    getToken(token); 
    if (!strcmp(token, "diffuseColor")) {
      diffuseColor = readVec3f();
    } else if (!strcmp(token, "specularColor")) {
      specularColor = readVec3f();
    } else if  (!strcmp(token, "exponent")) {
      exponent = readFloat();
    } else if (!strcmp(token, "reflectiveColor")) {
      reflectiveColor = readVec3f();
    } else if (!strcmp(token, "transparentColor")) {
      transparentColor = readVec3f();
    } else if (!strcmp(token, "indexOfRefraction")) {
      indexOfRefraction = readFloat();
    } else {
      assert (!strcmp(token, "}"));
      break;
    }
  }
  Material *answer = new PhongMaterial(diffuseColor,specularColor,exponent,
				       reflectiveColor,transparentColor,
				       indexOfRefraction);
  return answer;
}


Group* SceneParser::parseGroup() {

  char token[MAX_PARSER_TOKEN_LENGTH];
  getToken(token); assert (!strcmp(token, "{"));

  Group *answer = new Group();
  
   getToken(token); assert (!strcmp(token, "MaterialIndex"));
 int mtridx=readInt();
  getToken(token); assert(!strcmp(token,"Mesh"));
  parseMesh(answer,mtridx);
  cout<<"Meshes are set"<<endl;
  getToken(token); assert(!strcmp(token,"}")||!strcmp(token,"Transform"));
    if(!strcmp(token,"}")) {
        printf("Scene is set. We are ready to render\n");
        return answer;
    }
  parseTransform(answer);
  getToken(token);
  assert(!strcmp(token,"}"));
    printf("Scene is set. We are ready to render.\n\n");
  return answer;
}

// ====================================================================
// ====================================================================


void SceneParser::parseMesh(Group* group,int idx) {
  char token[MAX_PARSER_TOKEN_LENGTH];
  char filename[MAX_PARSER_TOKEN_LENGTH];
  // get the filename
  getToken(token); assert (!strcmp(token, "{"));
  getToken(token); assert (!strcmp(token, "obj_file"));
  getToken(filename);
  getToken(token); assert (!strcmp(token, "}"));
  const char *ext = &filename[strlen(filename)-4];
  assert(!strcmp(ext,".obj"));
  
  vector<Vec3f> verts;
  // read it again, save it
  FILE* ffile = fopen(filename,"r");
  assert (ffile != NULL);
    char s[100];
    memset(s,0,sizeof(s));
  int fcnt=0;
    while (fscanf(ffile,"%s",s)!=EOF) {
    if (!strcmp(s,"v")) {
       float v0,v1,v2;
      fscanf (ffile,"%f %f %f",&v0,&v1,&v2);
      Vec3f tmp(v0,v1,v2);
      verts.push_back(tmp);
    }else if(!strcmp(s,"vt")){
        float t1,t2;
        fscanf(ffile," %f %f",&t1,&t2);
    }else if(!strcmp(s,"vn")){
        float t1,t2,t3;
        fscanf(ffile," %f %f %f",&t1,&t2,&t3);
    }else if(!strcmp(s,"usemtl")){
        fscanf(ffile,"%s",s);
    }else if(!strcmp(s,"s")){
        int k;
        fscanf(ffile,"%d",&k);
    }else if (!strcmp(s,"f")) {
      unsigned int f;
      vector<Vec3f> buffer;
      while(1){
      char c=fgetc(ffile);
      if(c!=' ') break;
          char tmp[100];
          fscanf(ffile,"%s",tmp);
          sscanf(tmp,"%d",&f);
      assert (f > 0 && f <= verts.size());
      buffer.push_back(verts[f-1]);
      }
      polygon* poly=new polygon(buffer,fcnt,materials[idx]);
      group->addNewPoly(poly);
      fcnt++;
      buffer.clear();
    }
   // otherwise, must be whitespace
  }
  fclose(ffile);
}


void SceneParser::parseTransform(Group* group) {
  char token[MAX_PARSER_TOKEN_LENGTH];
  Matrix matrix; matrix.SetToIdentity();
  getToken(token); assert (!strcmp(token, "{"));
  // read in transformations:
  // apply to the LEFT side of the current matrix (so the first
  // transform in the list is the last applied to the object)
    memset(token,0,sizeof(token));
  getToken(token);
  while (1) {
    if (!strcmp(token,"Scale")) {
      matrix=Matrix::MakeScale(readVec3f());
      group->leftMulMatrix(matrix);
    } else if (!strcmp(token,"UniformScale")) {
      float s = readFloat();
      matrix = Matrix::MakeScale(Vec3f(s,s,s));
      group->leftMulMatrix(matrix);
    } else if (!strcmp(token,"Translate")) {
      matrix = Matrix::MakeTranslation(readVec3f());
      group->leftMulMatrix(matrix);
    } else if (!strcmp(token,"XRotate")) {
      matrix = Matrix::MakeXRotation(DegreesToRadians(readFloat()));
      group->leftMulMatrix(matrix);
    } else if (!strcmp(token,"YRotate")) {
      matrix = Matrix::MakeYRotation(DegreesToRadians(readFloat()));
      group->leftMulMatrix(matrix);
    } else if (!strcmp(token,"ZRotate")) {
      matrix = Matrix::MakeZRotation(DegreesToRadians(readFloat()));
      group->leftMulMatrix(matrix);
    } else if (!strcmp(token,"Rotate")) {
      getToken(token); assert (!strcmp(token, "{"));
      Vec3f axis = readVec3f();
      float degrees = readFloat();
      matrix = Matrix::MakeAxisRotation(axis,DegreesToRadians(degrees));
      group->leftMulMatrix(matrix);
      getToken(token); assert (!strcmp(token, "}"));
    } else if (!strcmp(token,"Matrix")) {
      getToken(token); assert (!strcmp(token, "{"));
      for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 4; i++) {
      float v = readFloat();
      matrix.Set(i,j,v); } }
      getToken(token); assert (!strcmp(token, "}"));
      group->leftMulMatrix(matrix);
    } else if(!strcmp(token,"Transform")){
       parseTransform(group);
    }
    else if(!strcmp(token,"}")){
        printf("Transformations are set.\n");
    return;
    }
    else {
        printf("invalid token %s",token);
        assert(0);
    }
      memset(token,0,sizeof(token));
    getToken(token);
  }
}


// ====================================================================
// ====================================================================

int SceneParser::getToken(char token[MAX_PARSER_TOKEN_LENGTH]) {
  // for simplicity, tokens must be separated by whitespace
  assert (file != NULL);
  int success = fscanf(file,"%s ",token);
  if (success == EOF) {
    token[0] = '\0';
    return 0;
  }
  return 1;
}


Vec3f SceneParser::readVec3f() {
  float x,y,z;
  int count = fscanf(file,"%f %f %f",&x,&y,&z);
  if (count != 3) {
    printf ("Error trying to read 3 floats to make a Vec3f\n");
    assert (0);
  }
  return Vec3f(x,y,z);
}


Vec2f SceneParser::readVec2f() {
  float u,v;
  int count = fscanf(file,"%f %f",&u,&v);
  if (count != 2) {
    printf ("Error trying to read 2 floats to make a Vec2f\n");
    assert (0);
  }
  return Vec2f(u,v);
}


float SceneParser::readFloat() {
  float answer;
  int count = fscanf(file,"%f",&answer);
  if (count != 1) {
    printf ("Error trying to read 1 float\n");
    assert (0);
  }
  return answer;
}


int SceneParser::readInt() {
  int answer;
  int count = fscanf(file,"%d",&answer);
  if (count != 1) {
    printf ("Error trying to read 1 int\n");
    assert (0);
  }
  return answer;
}

// ====================================================================
// ====================================================================
