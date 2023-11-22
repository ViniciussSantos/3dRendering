#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget *parent) : QOpenGLWidget{parent} {}

OpenGLWidget::~OpenGLWidget() {
  destroyVBOs(model);
  destroyShaders(model);
}

void OpenGLWidget::initializeGL() {
  initializeOpenGLFunctions();
  glClearColor(1, 1, 1, 1);
  qDebug("OpenGL version: %s", glGetString(GL_VERSION));
  qDebug("GLSL %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
  glEnable(GL_DEPTH_TEST);
}

void OpenGLWidget::resizeGL(int w, int h) {}

void OpenGLWidget::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (!model) {
    return;
  }

  auto shaderProgramID{model->shaderProgram[model->currentShader]};
  glUseProgram(shaderProgramID);

  glBindVertexArray(model->vao);
  glDrawElements(GL_TRIANGLES, model->numFaces * 3, GL_UNSIGNED_INT, nullptr);
}

void OpenGLWidget::toggleDarkMode(bool changeToDarkMode) {
  makeCurrent();
  if (changeToDarkMode) {
    glClearColor(0, 0, 0, 1);
  } else {
    glClearColor(1, 1, 1, 1);
  }
  update();
}

void OpenGLWidget::destroyVBOs(std::shared_ptr<Model> model) {

  makeCurrent();
  if (!model) {
    return;
  }

  glDeleteBuffers(1, &model->vboVertices);
  glDeleteBuffers(1, &model->eboIndices);
  glDeleteVertexArrays(1, &model->vao);

  model->vboVertices = 0;
  model->eboIndices = 0;
  model->vao = 0;
}

void OpenGLWidget::createVBOs(std::shared_ptr<Model> model) {

  makeCurrent();
  if (!model) {
    return;
  }

  destroyVBOs(model);

  glGenVertexArrays(1, &model->vao);
  glBindVertexArray(model->vao);

  glGenBuffers(1, &model->vboVertices);
  glBindBuffer(GL_ARRAY_BUFFER, model->vboVertices);
  glBufferData(GL_ARRAY_BUFFER, model->numVertices * sizeof(QVector4D),
               model->vertices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
  glEnableVertexAttribArray(0);

  glGenBuffers(1, &model->eboIndices);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->eboIndices);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->numFaces * 3 * sizeof(GLuint),
               model->indices.data(), GL_STATIC_DRAW);
}

void OpenGLWidget::destroyShaders(std::shared_ptr<Model> model) {

  makeCurrent();
  if (!model) {
    return;
  }

  for (GLuint shaderProgramId : model->shaderProgram) {
    glDeleteProgram(shaderProgramId);
  }
}

void OpenGLWidget::createShaders(std::shared_ptr<Model> model) {

  makeCurrent();
  if (!model) {
    return;
  }
  destroyShaders(model);

  for (size_t i{0}; i < model->shaderProgram.size(); i++) {
    QFile vs(model->vertexShaderFileName[i]);
    QFile fs(model->fragmentShaderFileName[i]);

    if (!vs.open(QFile::ReadOnly | QFile::Text)) {
      qDebug("Vertex shader file not found!");
      return;
    }

    if (!fs.open(QFile::ReadOnly | QFile::Text)) {
      qDebug("Fragment shader file not found!");
      return;
    }

    auto byteArrayVs{vs.readAll()};
    auto byteArrayFs{fs.readAll()};

    const char *c_strVs{byteArrayVs};
    const char *c_strFs{byteArrayFs};

    vs.close();
    fs.close();

    GLuint vertexShader{glCreateShader(GL_VERTEX_SHADER)};
    glShaderSource(vertexShader, 1, &c_strVs, 0);
    glCompileShader(vertexShader);
    GLint isCompiled{0};

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);

    if (!isCompiled) {
      GLint maxLength{0};
      glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

      std::vector<GLchar> infoLog(maxLength);
      glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

      qDebug("%s", &infoLog[0]);
      glDeleteShader(vertexShader);
      return;
    }

    GLuint fragmentShader{glCreateShader(GL_FRAGMENT_SHADER)};
    glShaderSource(fragmentShader, 1, &c_strFs, 0);
    glCompileShader(fragmentShader);

    isCompiled = 0;

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);

    if (!isCompiled) {
      GLint maxLength{0};
      glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

      std::vector<GLchar> infoLog(maxLength);
      glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

      qDebug("%s", &infoLog[0]);
      glDeleteShader(fragmentShader);
      glDeleteShader(vertexShader);
      return;
    }

    model->shaderProgram[i] = glCreateProgram();
    glAttachShader(model->shaderProgram[i], vertexShader);
    glAttachShader(model->shaderProgram[i], fragmentShader);
    glLinkProgram(model->shaderProgram[i]);

    GLint isLinked{0};

    glGetProgramiv(model->shaderProgram[i], GL_LINK_STATUS, &isLinked);

    if (!isLinked) {
      GLint maxLength{0};
      glGetProgramiv(model->shaderProgram[i], GL_INFO_LOG_LENGTH, &maxLength);

      std::vector<GLchar> infoLog(maxLength);
      glGetProgramInfoLog(model->shaderProgram[i], maxLength, &maxLength,
                          &infoLog[0]);

      qDebug("%s", &infoLog[0]);
      glDeleteProgram(model->shaderProgram[i]);
      glDeleteShader(vertexShader);
      glDeleteShader(fragmentShader);
      return;
    }

    glDetachShader(model->shaderProgram[i], vertexShader);
    glDetachShader(model->shaderProgram[i], fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
  }
}

void OpenGLWidget::showFileOpenDialog() {

  QString filename = QFileDialog::getOpenFileName(
      this, "Open mesh file", QDir::homePath(), QString("OFF Files (*.off)"),
      nullptr, QFileDialog::DontUseNativeDialog);

  if (filename.isEmpty()) {
    return;
  }

  model = std::make_shared<Model>();
  model->readOFFFile(filename);

  createShaders(model);
  createVBOs(model);

  emit statusBarMessage(QString("Vetices: %1, Faces: %2")
                            .arg(model->numVertices)
                            .arg(model->numFaces));

  update();
}

void OpenGLWidget::keyPressEvent(QKeyEvent *event) {

  switch (event->key()) {
  case Qt::Key_Escape:
    QApplication::quit();
    break;
  }
}
