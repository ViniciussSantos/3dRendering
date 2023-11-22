#include "model.h"

Model::Model() { shaderProgram.resize(vertexShaderFileName.size()); }

void Model::readOFFFile(const QString &filename) {
  QFile data(filename);
  data.open(QFile::ReadOnly);

  if (!data.isOpen()) {
    qDebug("Could not open file for reading");
    return;
  }

  QTextStream stream(&data);

  QString notUsed;

  stream >> notUsed;
  stream >> numVertices >> numFaces >> notUsed;

  vertices.resize(numVertices);
  indices.resize(numFaces * 3);

  float x, y, z;

  for (size_t i = 0; i < numVertices; ++i) {
    stream >> x >> y >> z;
    vertices[i] = QVector4D(x, y, z, 1);
  }

  unsigned a, b, c;
  for (size_t i = 0; i < numFaces; ++i) {
    stream >> notUsed >> a >> b >> c;
    indices[3 * i] = a;
    indices[3 * i + 1] = b;
    indices[3 * i + 2] = c;
  }

  rescaleModel();
  data.close();
}

void Model::computeBBox() {
  auto minLim = std::numeric_limits<float>::lowest();
  auto maxLim = std::numeric_limits<float>::max();

  maxBB = QVector3D(minLim, minLim, minLim);
  minBB = QVector3D(maxLim, maxLim, maxLim);

  for (size_t i{0}; i < numVertices; i++) {
    maxBB.setX(std::max(maxBB.x(), vertices[i].x()));
    maxBB.setY(std::max(maxBB.y(), vertices[i].y()));
    maxBB.setZ(std::max(maxBB.z(), vertices[i].z()));

    minBB.setX(std::min(minBB.x(), vertices[i].x()));
    minBB.setY(std::min(minBB.y(), vertices[i].y()));
    minBB.setZ(std::min(minBB.z(), vertices[i].z()));
  }

  centroidBB = (minBB + maxBB) / 2.0;
  diagonalBB = (maxBB - minBB).length();
}

void Model::rescaleModel() {
  computeBBox();
  float scale = 2.5f / diagonalBB;
  for (size_t i = 0; i < numVertices; ++i) {
    vertices[i] = QVector4D((vertices[i].toVector3D() - centroidBB) * scale, 1);
  }
  computeBBox();
}
