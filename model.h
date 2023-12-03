#ifndef MODEL_H
#define MODEL_H

#include <QFile>
#include <QMatrix4x4>
#include <QString>
#include <QTextStream>
#include <QVector3D>
#include <QVector4D>
#include <vector>

class Model {
public:
  Model();
  ~Model() = default;

  std::vector<QVector4D> vertices;
  std::vector<unsigned> indices;

  unsigned numVertices{0};
  unsigned numFaces{0};

  unsigned vao{0};
  unsigned vboVertices{0};
  unsigned eboIndices{0};

  void readOFFFile(const QString &);

  void computeBBox();
  void rescaleModel();

  QVector3D centroidBB, minBB, maxBB;
  float diagonalBB;

  int currentShader{0};

  std::vector<QString> vertexShaderFileName = {":/shaders/vzdepth.glsl"};
  std::vector<QString> fragmentShaderFileName = {":/shaders/fzdepth.glsl"};

  std::vector<unsigned> shaderProgram;

  QMatrix4x4 modelMatrix;
};

#endif // MODEL_H
