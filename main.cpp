#include <iostream>
#include <map>
 
#include <GL/gl.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/ext.hpp>
 
#include "main.h"
#include "obj_loader/objLoader.h"
#include <math.h>
 
using namespace glm;
using std::map;
 
unsigned int ViewPortWidth  = 512;
unsigned int ViewPortHeight = 512;
 
objLoader* objData;
 
map<int, vec4> getVertices(objLoader* obj)
{
  map<int, vec4> vertices;
  for (int i = 0; i < obj->vertexCount; i++) {
    float x, y, z, w;
 
    x = obj->vertexList[i]->e[0];
    y = obj->vertexList[i]->e[1];
    z = obj->vertexList[i]->e[2];
    w = obj->vertexList[i]->e[3];
 
    vertices[i] = vec4(x, y, z, 1);
  }
 
  return vertices;
}
 
void MyGlDraw()
{
 
  map<int, vec4> vertices = getVertices(objData);
 
  // model
 
  // mat4 m_model = mat4(1.0f);
  mat4 m_model = mat4(2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 1);
 
  // Camera
 
  vec3 camera_pos = vec3(0.0f, 0.0f, 5.0f);
  vec3 camera_lookat = vec3(0.0f, 0.0f, 0.0f);
  vec3 camera_up = vec3(0.0f, 1.0f, 0.0f);
 
  vec3 z_camera = normalize(camera_pos - camera_lookat);
  vec3 x_camera = normalize(cross(camera_up, z_camera));
  vec3 y_camera = cross(z_camera, x_camera);
 
 
  mat4 bt = mat4(x_camera.x, y_camera.x, z_camera.x, 0,
    x_camera.y, y_camera.y, z_camera.y, 0,
    x_camera.z, y_camera.z, z_camera.z, 0,
    0, 0, 0, 1);
 
  mat4 t = mat4(1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                -camera_pos.x, -camera_pos.y, -camera_pos.z, 1);
 
  mat4 m_view = bt * t;
 
  // Matrix ModelView
 
  mat4 m_model_view = m_view * m_model;
 
 
  // Matrix de Projeção
 
  int d = 1;
 
  mat4 m_projection = mat4(1, 0, 0, 0,
                           0, 1, 0, 0,
                           0, 0, 1, -1/d,
                            0, 0, d, 0);
 
  // Matrix ModelViewProjection
 
  mat4 m_model_view_projection = m_projection * m_model_view;
 
  // Aplica a matrix ModelViewProjection aos vértices do objeto
 
  /*for (auto& vec4 vertex: vertices) {
    vertex.second = m_model_view_projection * vertex.second;
  }*/
 
 
  // Homogeneizacao
 
  /*for (auto& vertex: vertices) {
    vertex.second = vertex.second / vertex.second.w;
  }*/
 
  // Conversão para o espaço de tela
 
  mat4 s1 = mat4(1, 0, 0, 0,
                 0, -1, 0, 0,
                 0, 0, 1, 0,
                 0, 0, 0, 1);
 
  mat4 trans = mat4(1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    1, 1, 0, 1);
 
  mat4 s2 = mat4((ViewPortWidth - 1) / 2, 0, 0, 0,
                0, (ViewPortHeight - 1) / 2, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1);
 
 
  mat4 m_viewport = s2 * trans * s1;
	
  mat4 MVPV = m_viewport * m_model_view_projection;
 
  /*for (auto& vertex: vertices) {
    vertex.second = m_viewport * vertex.second;
  }*/
 
  for (int i = 0; i < objData->faceCount; i++) {
 
    /*obj_face *o = objData->faceList[i];
 
    Color color{255, 255, 255, 255};
 
    vec4 vertex1 = vertices[o->vertex_index[0]];
    vec4 vertex2 = vertices[o->vertex_index[1]];
    vec4 vertex3 = vertices[o->vertex_index[2]];
 
    Point p1{(int) round(vertex1.x), (int) round(vertex1.y)};
    Point p2{(int) round(vertex2.x), (int) round(vertex2.y)};
    Point p3{(int) round(vertex3.x), (int) round(vertex3.y)};
 
    DrawTriangle(p1, p2, p3, color, color, color);*/

obj_face *o = objData->faceList[i];

Color color{255, 255, 255, 255};

		vec4 v1, v2, v3;

		v1 = vertices[o->vertex_index[0]];

		v2 = vertices[o->vertex_index[1]];
	
		v3 = vertices[o->vertex_index[2]];
		

		v1 = MVPV * v1;
		v2 = MVPV * v2;
		v3 = MVPV * v3;

		v1 /= v1.w;
		v2 /= v2.w;
		v3 /= v3.w;

Point p1 = {(int) round(v1.x), (int) round(v1.y)};
Point p2 = {(int) round(v2.x), (int) round(v2.y)};
Point p3 = {(int) round(v3.x), (int) round(v3.y)};

DrawTriangle(p1, p2, p3, color, color, color);
 
  }
 
}
 
 
int main(int argc, char **argv)
{
    // Inicializações.
    InitOpenGL(&argc, argv);
    InitCallBacks();
    InitDataStructures();
 
  objData = new objLoader();
    objData->load("monkey_head2.obj");
 
    // Ajusta a função que chama as funções do mygl.h
    DrawFunc = MyGlDraw;
 
    // Framebuffer scan loop.
    glutMainLoop();
 
  delete(objData);
    return 0;
}
