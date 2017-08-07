/******************************************************************************\

    CAMotics is an Open-Source simulation and CAM software.
    Copyright (C) 2011-2017 Joseph Coffland <joseph@cauldrondevelopment.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

\******************************************************************************/

#include "CuboidView.h"

#include "GL.h"

using namespace CAMotics;


CuboidView::~CuboidView() {
  if (haveVBOs()) {
    QOpenGLFunctions *glFuncs = QOpenGLContext::currentContext()->functions();

    if (vertexVBuf) glFuncs->glDeleteBuffers(1, &vertexVBuf);
    if (normalVBuf) glFuncs->glDeleteBuffers(1, &normalVBuf);
  }
}


void CuboidView::draw() {
  if (bounds == cb::Rectangle3D()) return;

  static float vertices[] = {
    1, 0, 0,  1, 0, 1,  1, 1, 1,  1, 1, 0,
    0, 0, 0,  0, 0, 1,  0, 1, 1,  0, 1, 0,

    0, 1, 0,  1, 1, 0,  1, 1, 1,  0, 1, 1,
    0, 0, 0,  1, 0, 0,  1, 0, 1,  0, 0, 1,

    0, 0, 1,  1, 0, 1,  1, 1, 1,  0, 1, 1,
    0, 0, 0,  1, 0, 0,  1, 1, 0,  0, 1, 0,
  };

  static float normals[] = {
     1,  0,  0,   1,  0,  0,   1,  0,  0,   1,  0,  0,
    -1,  0,  0,  -1,  0,  0,  -1,  0,  0,  -1,  0,  0,

     0,  1,  0,   0,  1,  0,   0,  1,  0,   0,  1,  0,
     0, -1,  0,   0, -1,  0,   0, -1,  0,   0, -1,  0,

     0,  0,  1,   0,  0,  1,   0,  0,  1,   0,  0,  1,
     0,  0, -1,   0,  0, -1,   0,  0, -1,   0,  0, -1,
  };

  if (haveVBOs()) {
    QOpenGLFunctions *glFuncs = QOpenGLContext::currentContext()->functions();

    if (!vertexVBuf) {
      glFuncs->glGenBuffers(1, &vertexVBuf);
      glFuncs->glBindBuffer(GL_ARRAY_BUFFER, vertexVBuf);
      glFuncs->glBufferData(GL_ARRAY_BUFFER, 24 * 3 * sizeof(float),
                            vertices, GL_STATIC_DRAW);

      glFuncs->glGenBuffers(1, &normalVBuf);
      glFuncs->glBindBuffer(GL_ARRAY_BUFFER, normalVBuf);
      glFuncs->glBufferData(GL_ARRAY_BUFFER, 24 * 3 * sizeof(float),
                            normals, GL_STATIC_DRAW);
    }

    glFuncs->glBindBuffer(GL_ARRAY_BUFFER, vertexVBuf);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    glFuncs->glBindBuffer(GL_ARRAY_BUFFER, normalVBuf);
    glNormalPointer(GL_FLOAT, 0, 0);

  } else {
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glNormalPointer(GL_FLOAT, 0, normals);
  }

  glEnable(GL_NORMALIZE);
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);

  glPushMatrix();

  cb::Vector3D bMin = bounds.getMin();
  cb::Vector3D bDim = bounds.getDimensions();
  glTranslated(bMin.x(), bMin.y(), bMin.z());
  glScaled(bDim.x(), bDim.y(), bDim.z());

  glDrawArrays(GL_QUADS, 0, 24);

  glPopMatrix();

  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
}
