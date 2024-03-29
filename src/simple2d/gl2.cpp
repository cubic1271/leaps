// OpenGL 2.1

#include "simple2d/simple2d.h"

#if !GLES


/*
 * Applies the projection matrix
 */
void S2D_GL2_ApplyProjection(int w, int h) {

  // Initialize the projection matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // Multiply the current matrix with the orthographic matrix
  glOrtho(0.f, w, h, 0.f, -1.f, 1.f);

  // Initialize the model-view matrix
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}


/*
 * Initalize OpenGL
 */
int S2D_GL2_Init() {

  GLenum error = GL_NO_ERROR;

  // Enable transparency
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Check for errors
  error = glGetError();
  if (error != GL_NO_ERROR) {
    S2D_GL_PrintError("OpenGL initialization failed");
    return 1;
  } else {
    return 0;
  }
}


/*
 * Draw triangle
 */
void S2D_GL2_DrawTriangle(GLfloat x1, GLfloat y1,
                          GLfloat r1, GLfloat g1, GLfloat b1, GLfloat a1,
                          GLfloat x2, GLfloat y2,
                          GLfloat r2, GLfloat g2, GLfloat b2, GLfloat a2,
                          GLfloat x3, GLfloat y3,
                          GLfloat r3, GLfloat g3, GLfloat b3, GLfloat a3) {

  glBegin(GL_TRIANGLES);
    glColor4f(r1, g1, b1, a1); glVertex2f(x1, y1);
    glColor4f(r2, g2, b2, a2); glVertex2f(x2, y2);
    glColor4f(r3, g3, b3, a3); glVertex2f(x3, y3);
  glEnd();
}


/*
 * Draw texture
 */
static void S2D_GL2_DrawTexture(int x, int y, int w, int h,
                                GLfloat angle, GLfloat rx, GLfloat ry,
                                GLfloat r, GLfloat g, GLfloat b, GLfloat a,
                                GLfloat tx1, GLfloat ty1, GLfloat tx2, GLfloat ty2,
                                GLfloat tx3, GLfloat ty3, GLfloat tx4, GLfloat ty4,
                                GLuint texture_id) {

  S2D_GL_Point v1 = { .x = x,     .y = y     };
  S2D_GL_Point v2 = { .x = x + w, .y = y     };
  S2D_GL_Point v3 = { .x = x + w, .y = y + h };
  S2D_GL_Point v4 = { .x = x,     .y = y + h };

  // Rotate vertices
  if (angle != 0) {
    v1 = S2D_RotatePoint(v1, angle, rx, ry);
    v2 = S2D_RotatePoint(v2, angle, rx, ry);
    v3 = S2D_RotatePoint(v3, angle, rx, ry);
    v4 = S2D_RotatePoint(v4, angle, rx, ry);
  }

  glEnable(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, texture_id);

  glBegin(GL_QUADS);
    glColor4f(r, g, b, a);
    glTexCoord2f(tx1, ty1); glVertex2f(v1.x, v1.y);
    glTexCoord2f(tx2, ty2); glVertex2f(v2.x, v2.y);
    glTexCoord2f(tx3, ty3); glVertex2f(v3.x, v3.y);
    glTexCoord2f(tx4, ty4); glVertex2f(v4.x, v4.y);
  glEnd();

  glDisable(GL_TEXTURE_2D);
}


/*
 * Draw image
 */
void S2D_GL2_DrawImage(S2D_Image *img) {
  S2D_GL2_DrawTexture(
    img->x, img->y, img->width, img->height,
    img->rotate, img->rx, img->ry,
    img->color.r, img->color.g, img->color.b, img->color.a,
    0.f, 0.f, 1.f, 0.f, 1.f, 1.f, 0.f, 1.f,
    img->texture_id
  );
}


/*
 * Draw sprite
 */
void S2D_GL2_DrawSprite(S2D_Sprite *spr) {
  S2D_GL2_DrawTexture(
    spr->x, spr->y, spr->width, spr->height,
    spr->rotate, spr->rx, spr->ry,
    spr->color.r, spr->color.g, spr->color.b, spr->color.a,
    spr->tx1, spr->ty1, spr->tx2, spr->ty2, spr->tx3, spr->ty3, spr->tx4, spr->ty4,
    spr->img->texture_id
  );
}


/*
 * Draw text
 */
void S2D_GL2_DrawText(S2D_Text *txt) {
  S2D_GL2_DrawTexture(
    txt->x, txt->y, txt->width, txt->height,
    txt->rotate, txt->rx, txt->ry,
    txt->color.r, txt->color.g, txt->color.b, txt->color.a,
    0.f, 0.f, 1.f, 0.f, 1.f, 1.f, 0.f, 1.f,
    txt->texture_id
  );
}

#endif
