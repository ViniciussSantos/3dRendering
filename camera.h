#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>
#include <QVector3D>

class Camera {
public:
  Camera();
  QVector3D eye{QVector3D(0, 0, 2)};
  QVector3D center{QVector3D(0, 0, 0)};
  QVector3D up{QVector3D(0, 1, 0)};

  QMatrix4x4 viewMatrix;
  QMatrix4x4 projectionMatrix;

  void createViewMatrix();
  void resizeViewPort(int width, int height, bool ortho = false);
};

#endif // CAMERA_H
