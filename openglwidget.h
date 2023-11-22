#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QApplication>
#include <QFile>
#include <QFileDialog>
#include <QKeyEvent>
#include <QOpenGLExtraFunctions>
#include <QOpenGLWidget>
#include <QTimer>
#include <QVector4D>
#include <memory>
#include <model.h>

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions {
  Q_OBJECT
public:
  explicit OpenGLWidget(QWidget *parent = nullptr);
  ~OpenGLWidget();

  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;
  void keyPressEvent(QKeyEvent *event) override;

  std::shared_ptr<Model> model;

  void createVBOs(std::shared_ptr<Model>);
  void destroyVBOs(std::shared_ptr<Model>);

  void createShaders(std::shared_ptr<Model>);
  void destroyShaders(std::shared_ptr<Model>);
signals:
  void statusBarMessage(QString);

public slots:
  void toggleDarkMode(bool);
  void showFileOpenDialog();

signals:
};

#endif // OPENGLWIDGET_H
