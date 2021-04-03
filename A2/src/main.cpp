//#define GLEW_STATIC
//#include<GL/glew.h>
//#include <GLFW/glfw3.h>
#include "loader.h"
#include "scene_parser.h"
#include "glCanvas.h"
#include "camera.h"
#include "light.h"
#include "material.h"
#include "group.h"

char* vertexShaderFile;
char* fragShaderFile;
char* inputFile;
inline void process_parameters(int argc, char** argv);

int main(int argc, char** argv)
{
    glutInit(&argc,argv);
    glutInitWindowSize(400,400);
    glutInitWindowPosition(100,100);
    glutCreateWindow("OpenGL Viewer");
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
    //glutCreateWindow("GLEW Test");
   //glewInit();
    process_parameters(argc,argv);
    cout<<0<<endl;
    GLuint pO=0;
  pO=  loadShader(vertexShaderFile,fragShaderFile);
    cout<<1<<endl;
    SceneParser* scene=new SceneParser(inputFile);
    GLCanvas* canvas=new GLCanvas();
    scene->getGroup()->applyTransform();
    scene->getGroup()->calEquation();
    canvas->initialize(scene,NULL);
    //unload(pO,vO,fO);
    return 0;
}
inline void process_parameters(int argc, char** argv)
{
    for(int i=1;i<argc;i++){
        if(!strcmp(argv[i], "-input")){
            i++;
            assert(i<argc);
            inputFile=argv[i];
        }
        else if(!strcmp(argv[i],"-vs")){
            i++;
            assert(i<argc);
            vertexShaderFile=argv[i];
         }
        else if(!strcmp(argv[i],"-fs")){
            i++;
            assert(i<argc);
            fragShaderFile=argv[i];
        }
        else{
             printf("whoops error with command line argument %d: '%s'\n",i,argv[i]);
            assert(0);
        }
    }
    assert(inputFile!=NULL);
    return;
}
