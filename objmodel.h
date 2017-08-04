#ifndef OBJMODEL_H
#define OBJMODEL_H

#include <QVector>
#include <QtOpenGL>
#include <QVector2D>
#include <QVector3D>
#include <QTextStream>

struct OBJModel
{
    quint32 verticesSize;
    QVector<QVector3D> vertices;
    QVector<QVector2D> uvs;
    QVector<QVector3D> normals;

    bool hasUV;
    ~OBJModel();

    void LoadOBJ(QString path);
    bool IsLoaded();
    void Centralize();
private:
    bool isLoaded = false;
};

#endif // OBJMODEL_H
