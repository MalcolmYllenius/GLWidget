#include "objmodel.h"

OBJModel::~OBJModel()
{

}

void OBJModel::LoadOBJ(QString path)
{
    isLoaded = false;

    QVector<unsigned int> temp_vertexIndices, temp_uvIndices, temp_normalIndices;
    QVector<QVector3D> temp_vertices;
    QVector<QVector2D> temp_uvs;
    QVector<QVector3D> temp_normals;

    QFile file(path);
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        hasUV = true;
        while(!stream.atEnd())
        {
            QString line = stream.readLine();
            if(line.isNull())
                break;
            if(line.startsWith("v "))
            {
                float x, y, z = .0f;
                sscanf(line.toStdString().c_str(), "v %f %f %f\n", &x, &y, &z);
                QVector3D vertex(x, y, z);
                temp_vertices.append(vertex);
            }
            else if(line.startsWith("vt"))
            {
                float x, y = .0f;
                sscanf(line.toStdString().c_str(), "vt %f %f\n", &x, &y);
                QVector2D uv(x, y);
                temp_uvs.append(uv);
            }
            else if(line.startsWith("vn"))
            {
                float x, y, z = .0f;
                sscanf(line.toStdString().c_str(), "vn %f %f %f\n", &x, &y, &z);
                QVector3D normal(x, y, z);
                temp_normals.append(normal);
            }
            else if(line.startsWith("f"))
            {
                unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

                int matches = sscanf(line.toStdString().c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d\n",
                        &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                        &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                        &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
                if(matches != 9)
                {
                    matches = sscanf(line.toStdString().c_str(), "f %d//%d %d//%d %d//%d\n",
                                     &vertexIndex[0], &normalIndex[0],
                                     &vertexIndex[1], &normalIndex[1],
                                     &vertexIndex[2], &normalIndex[2]);
                    if(matches != 6)
                    {
                        matches = sscanf(line.toStdString().c_str(), "f %d %d %d\n",
                                     &vertexIndex[0],
                                     &vertexIndex[1],
                                     &vertexIndex[2]);
                        if(matches != 3)
                        {
                            printf("Parser error\n");
                            return;
                        }
                        else
                        {
                            normalIndex[0] = vertexIndex[0];
                            normalIndex[1] = vertexIndex[1];
                            normalIndex[2] = vertexIndex[2];
                            uvIndex[0] = vertexIndex[0];
                            uvIndex[1] = vertexIndex[1];
                            uvIndex[2] = vertexIndex[2];
                        }
                    }
                }

                hasUV = temp_uvs.size() > 0;

                for(int i = 0; i < 3; i++)
                {
                    temp_vertexIndices.append(vertexIndex[i]);
                    if(hasUV)
                        temp_uvIndices.append(uvIndex[i]);
                    temp_normalIndices.append(normalIndex[i]);

                }
            }
        }
        file.close();
    }
    else
        return;
    for(int i = 0; i < temp_vertexIndices.size(); i++)
        vertices.append(temp_vertices[temp_vertexIndices[i] - 1]);
    if(hasUV)
    {
        for(int i = 0; i < temp_uvIndices.size(); i++)
            uvs.append(temp_uvs[temp_uvIndices[i] - 1]);
    }
    for(int i = 0; i < temp_normalIndices.size(); i++)
        normals.append(temp_normals[temp_normalIndices[i] - 1]);

    isLoaded = true;
}

bool OBJModel::IsLoaded()
{
    return isLoaded;
}

void OBJModel::Centralize()
{
    if(isLoaded)
    {
        QVector3D smallest, largest;
        smallest = largest = vertices[0];
        for(int i = 0; i < vertices.size(); i++)
        {
            QVector3D vertex = vertices[i];
            if(vertex.x() < smallest.x())
                smallest.setX(vertex.x());
            if(vertex.y() < smallest.y())
                smallest.setY(vertex.y());
            if(vertex.z() < smallest.z())
                smallest.setZ(vertex.z());

            if(vertex.x() > largest.x())
                largest.setX(vertex.x());
            if(vertex.y() > largest.y())
                largest.setY(vertex.y());
            if(vertex.z() > largest.z())
                largest.setZ(vertex.z());
        }

        QVector3D difference = largest - smallest;
        QVector3D center = smallest + (difference * 0.5f);

        for(int i = 0; i < vertices.size(); i++)
        {
            vertices[i] -= center;
        }
    }
}
