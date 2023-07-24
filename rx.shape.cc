
rxmatrix_t
rxmatrix_rotation(double xangle, double yangle, double zangle)
{
  rxmatrix_t r =
    rxmatrix_multiply(
      rxmatrix_rotX(xangle),
      rxmatrix_rotY(yangle));
  return r;
}

void rxdraw_globe(float xori, float yori, float r)
{
  r    =  10;
  xori = .5;
  yori = .5;

  int  rings = 32;
  int slices = 32;

  int xslice,yslice;
  double x,y,z;

  rxtexture_bind(world_texture);
  rxsampler_bind(rx.linear_sampler);

  rxvertex_mode();
  rxvertex_color(RX_COLOR_WHITE);

#define DEG2RAD (rxPI_F/180)

  int index = 0;
  for (int i = 0; i < (rings + 2); i++)
  { for (int j = 0; j < (slices + 0); j++)
    {
      rxvec3_t vertices[] =
      {
        {.5 * cosf(DEG2RAD*(270 + (180.0f/(rings + 1))*(i + 0.)))*sinf(DEG2RAD*(360.* (j + 0.)/slices)),
         .5 * sinf(DEG2RAD*(270 + (180.0f/(rings + 1))*(i + 0.))),
         .5 * cosf(DEG2RAD*(270 + (180.0f/(rings + 1))*(i + 0.)))*cosf(DEG2RAD*(360.* (j + 0.)/slices))},

        {.5 * cosf(DEG2RAD*(270 + (180.0f/(rings + 1))*(i + 1.)))*sinf(DEG2RAD*(360.* (j + 1.)/slices)),
         .5 * sinf(DEG2RAD*(270 + (180.0f/(rings + 1))*(i + 1.))),
         .5 * cosf(DEG2RAD*(270 + (180.0f/(rings + 1))*(i + 1.)))*cosf(DEG2RAD*(360.* (j + 1.)/slices))},

        {.5 * cosf(DEG2RAD*(270 + (180.0f/(rings + 1))*(i + 1.)))*sinf(DEG2RAD*(360.* (j + 0.)/slices)),
         .5 * sinf(DEG2RAD*(270 + (180.0f/(rings + 1))*(i + 1.))),
         .5 * cosf(DEG2RAD*(270 + (180.0f/(rings + 1))*(i + 1.)))*cosf(DEG2RAD*(360.* (j + 0.)/slices))},

        {.5 * cosf(DEG2RAD*(270 + (180.0f/(rings + 1))*(i + 0.)))*sinf(DEG2RAD*(360.* (j + 0.)/slices)),
         .5 * sinf(DEG2RAD*(270 + (180.0f/(rings + 1))*(i + 0.))),
         .5 * cosf(DEG2RAD*(270 + (180.0f/(rings + 1))*(i + 0.)))*cosf(DEG2RAD*(360.* (j + 0.)/slices))},

        {.5 * cosf(DEG2RAD*(270 + (180.0f/(rings + 1))*(i + 0.)))*sinf(DEG2RAD*(360.* (j + 1.)/slices)),
         .5 * sinf(DEG2RAD*(270 + (180.0f/(rings + 1))*(i + 0.))),
         .5 * cosf(DEG2RAD*(270 + (180.0f/(rings + 1))*(i + 0.)))*cosf(DEG2RAD*(360.* (j + 1.)/slices))},

        {.5 * cosf(DEG2RAD*(270 + (180.0f/(rings + 1))*(i + 1.)))*sinf(DEG2RAD*(360.* (j + 1.)/slices)),
         .5 * sinf(DEG2RAD*(270 + (180.0f/(rings + 1))*(i + 1.))),
         .5 * cosf(DEG2RAD*(270 + (180.0f/(rings + 1))*(i + 1.)))*cosf(DEG2RAD*(360.* (j + 1.)/slices))}
      };

      rxvec2_t uvs[] =
      { {(j + 0.) / slices, (i + 0.) / (rings + 1.)},
        {(j + 1.) / slices, (i + 1.) / (rings + 1.)},
        {(j + 0.) / slices, (i + 1.) / (rings + 1.)},
        {(j + 0.) / slices, (i + 0.) / (rings + 1.)},
        {(j + 1.) / slices, (i + 0.) / (rings + 1.)},
        {(j + 1.) / slices, (i + 1.) / (rings + 1.)}
      };

      for(int k = 0; k < ccCarrlenL(vertices); k += 1)
      {
        rxvec4_t v;
        v.xyz = vertices[k];
        v.w   = 1.;

        v = rxmul_matvec(rxmatrix_rotation(rot_x,rot_y,0),v);

        v.z  += cam_z;

        rx.vertex_xtexel = uvs[k].x;
        rx.vertex_ytexel = uvs[k].y;
        rx_vertex_xyz(v.x,v.y,v.z);
      }

      rxsubmit_index(index + 0);
      rxsubmit_index(index + 1);
      rxsubmit_index(index + 2);
      rxsubmit_index(index + 3);
      rxsubmit_index(index + 4);
      rxsubmit_index(index + 5);

      index += 6;
    }
  }

  rxvertex_mode_end();
}

void rxdraw_cube(rxcolor_t color, float x, float y, float r)
{
  rxtexture_bind(texture);
  rxsampler_bind(rx.point_sampler);

  rxvertex_mode();
  rxvertex_color(color);

  const MC_RECT *M = MC_CHAR_SKIN.Head.M;

  rxvertex_t vertices[] =
  { /* FRONT */
    (rxvertex_t){(rxvec4_t){-.5, -.5, -.5, 1.0},rxvec2_xy(0x00000 + M[4].x0,M[4].y0 + M[4].y1)},
    (rxvertex_t){(rxvec4_t){-.5, 0.5, -.5, 1.0},rxvec2_xy(0x00000 + M[4].x0,M[4].y0 + 0x00000)},
    (rxvertex_t){(rxvec4_t){0.5, 0.5, -.5, 1.0},rxvec2_xy(M[4].x0 + M[4].x1,M[4].y0 + 0x00000)},
    (rxvertex_t){(rxvec4_t){0.5, -.5, -.5, 1.0},rxvec2_xy(M[4].x0 + M[4].x1,M[4].y0 + M[4].y1)},
    /* RIGHT */
    (rxvertex_t){(rxvec4_t){+.5, -.5, -.5, 1.0},rxvec2_xy(0x00000 + M[1].x0,M[1].y0 + M[1].y1)},
    (rxvertex_t){(rxvec4_t){+.5, +.5, -.5, 1.0},rxvec2_xy(0x00000 + M[1].x0,M[1].y0 + 0x00000)},
    (rxvertex_t){(rxvec4_t){+.5, +.5, +.5, 1.0},rxvec2_xy(M[1].x0 + M[1].x1,M[1].y0 + 0x00000)},
    (rxvertex_t){(rxvec4_t){+.5, -.5, +.5, 1.0},rxvec2_xy(M[1].x0 + M[1].x1,M[1].y0 + M[1].y1)},
    /* BACK */
    (rxvertex_t){(rxvec4_t){+.5, -.5, +.5, 1.0},rxvec2_xy(0x00000 + M[5].x0,M[5].y0 + M[5].y1)},
    (rxvertex_t){(rxvec4_t){+.5, +.5, +.5, 1.0},rxvec2_xy(0x00000 + M[5].x0,M[5].y0 + 0x00000)},
    (rxvertex_t){(rxvec4_t){-.5, +.5, +.5, 1.0},rxvec2_xy(M[5].x0 + M[5].x1,M[5].y0 + 0x00000)},
    (rxvertex_t){(rxvec4_t){-.5, -.5, +.5, 1.0},rxvec2_xy(M[5].x0 + M[5].x1,M[5].y0 + M[5].y1)},
    /* LEFT */
    (rxvertex_t){(rxvec4_t){-.5, -.5, +.5, 1.0},rxvec2_xy(0x00000 + M[0].x0,M[0].y0 + M[0].y1)},
    (rxvertex_t){(rxvec4_t){-.5, +.5, +.5, 1.0},rxvec2_xy(0x00000 + M[0].x0,M[0].y0 + 0x00000)},
    (rxvertex_t){(rxvec4_t){-.5, +.5, -.5, 1.0},rxvec2_xy(M[0].x0 + M[0].x1,M[0].y0 + 0x00000)},
    (rxvertex_t){(rxvec4_t){-.5, -.5, -.5, 1.0},rxvec2_xy(M[0].x0 + M[0].x1,M[0].y0 + M[0].y1)},
    /* TOP */
    (rxvertex_t){(rxvec4_t){-.5, +.5, -.5, 1.0},rxvec2_xy(0x00000 + M[3].x0,M[3].y0 + M[3].y1)},
    (rxvertex_t){(rxvec4_t){-.5, +.5, +.5, 1.0},rxvec2_xy(0x00000 + M[3].x0,M[3].y0 + 0x00000)},
    (rxvertex_t){(rxvec4_t){+.5, +.5, +.5, 1.0},rxvec2_xy(M[3].x0 + M[3].x1,M[3].y0 + 0x00000)},
    (rxvertex_t){(rxvec4_t){+.5, +.5, -.5, 1.0},rxvec2_xy(M[3].x0 + M[3].x1,M[3].y0 + M[3].y1)},
    /* BOTTOM */
    (rxvertex_t){(rxvec4_t){-.5, -.5, +.5, 1.0},rxvec2_xy(0x00000 + M[2].x0,M[2].y0 + M[2].y1)},
    (rxvertex_t){(rxvec4_t){-.5, -.5, -.5, 1.0},rxvec2_xy(0x00000 + M[2].x0,M[2].y0 + 0x00000)},
    (rxvertex_t){(rxvec4_t){+.5, -.5, -.5, 1.0},rxvec2_xy(M[2].x0 + M[2].x1,M[2].y0 + 0x00000)},
    (rxvertex_t){(rxvec4_t){+.5, -.5, +.5, 1.0},rxvec2_xy(M[2].x0 + M[2].x1,M[2].y0 + M[2].y1)} };


  for(int i = 0; i < ccCarrlenL(vertices); i += 1)
  {
    rxvertex_t v = vertices[i];
    v.rgba = RX_COLOR_WHITE;
    v.xyzw = rxmul_matvec(rxmatrix_rotation(rot_x,rot_y,0),v.xyzw);
    v.  z += 4;
    v.  u /= (float) texture.size_x;
    v.  v /= (float) texture.size_y;
    vertices[i] = v;
  }

  rxsubmit_vertex_array(vertices,ccCarrlenL(vertices));

  for(int i = 0; i < ccCarrlenL(vertices) / 4; i += 1)
  {
    rxsubmit_index(i*4 + 0);rxsubmit_index(i*4 + 1);rxsubmit_index(i*4 + 2);
    rxsubmit_index(i*4 + 0);rxsubmit_index(i*4 + 2);rxsubmit_index(i*4 + 3);
  }

  rxvertex_mode_end();
}