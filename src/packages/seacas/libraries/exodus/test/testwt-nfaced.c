/*
 * Copyright(C) 1999-2021 National Technology & Engineering Solutions
 * of Sandia, LLC (NTESS).  Under the terms of Contract DE-NA0003525 with
 * NTESS, the U.S. Government retains certain rights in this software.
 *
 * See packages/seacas/LICENSE for details
 */
/*****************************************************************************
 *
 * testwt - test write an ExodusII database file (testwt-nfaced.exo)
 *
 *****************************************************************************/

#include "exodusII.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
  ex_opts(EX_VERBOSE | EX_ABORT);

  /* Specify compute and i/o word size */
  int CPU_word_size = 0; /* sizeof(float) */
  int IO_word_size  = 4; /* (4 bytes) */

  /* create EXODUS II file */
  int exoid = ex_create("test-nfaced.exo", /* filename path */
                        EX_CLOBBER,        /* create mode */
                        &CPU_word_size,    /* CPU float word size in bytes */
                        &IO_word_size);    /* I/O float word size in bytes */
  printf("after ex_create for test.exo, exoid = %d\n", exoid);
  printf(" cpu word size: %d io word size: %d\n", CPU_word_size, IO_word_size);

  /* initialize file with parameters */
  {
    ex_init_params par;

    int num_dim       = 3;
    int num_nodes     = 14;
    int num_elem      = 3;
    int num_elem_blk  = 1;
    int num_node_sets = 0;

    char *title = "This is a test";
    ex_copy_string(par.title, title, MAX_LINE_LENGTH + 1);
    par.num_dim       = num_dim;
    par.num_nodes     = num_nodes;
    par.num_edge      = 0;
    par.num_edge_blk  = 0;
    par.num_face      = 15;
    par.num_face_blk  = 1;
    par.num_elem      = num_elem;
    par.num_elem_blk  = num_elem_blk;
    par.num_node_sets = num_node_sets;
    par.num_edge_sets = 0;
    par.num_face_sets = 0;
    par.num_side_sets = 0;
    par.num_elem_sets = 0;
    par.num_node_maps = 0;
    par.num_edge_maps = 0;
    par.num_face_maps = 0;
    par.num_elem_maps = 0;

    int error = ex_put_init_ext(exoid, &par);

    printf("after ex_put_init_ext, error = %d\n", error);

    if (error) {
      ex_close(exoid);
      exit(-1);
    }
  }

  /* write nodal coordinates values and names to database */
  float x[100], y[100], z[100];
  x[0]  = 0.00000e+00;
  y[0]  = 0.00000e+00;
  z[0]  = 0.00000e+00;
  x[1]  = 2.00000e+00;
  y[1]  = 0.00000e+00;
  z[1]  = 0.00000e+00;
  x[2]  = 0.00000e+00;
  y[2]  = 2.00000e+00;
  z[2]  = 0.00000e+00;
  x[3]  = 2.00000e+00;
  y[3]  = 2.00000e+00;
  z[3]  = 0.00000e+00;
  x[4]  = 0.00000e+00;
  y[4]  = 0.00000e+00;
  z[4]  = 2.00000e+00;
  x[5]  = 2.00000e+00;
  y[5]  = 0.00000e+00;
  z[5]  = 2.00000e+00;
  x[6]  = 0.00000e+00;
  y[6]  = 2.00000e+00;
  z[6]  = 2.00000e+00;
  x[7]  = 2.00000e+00;
  y[7]  = 2.00000e+00;
  z[7]  = 2.00000e+00;
  x[8]  = 0.00000e+00;
  y[8]  = 3.50000e+00;
  z[8]  = 1.00000e+00;
  x[9]  = 2.00000e+00;
  y[9]  = 3.50000e+00;
  z[9]  = 1.00000e+00;
  x[10] = 0.00000e+00;
  y[10] = 3.00000e+00;
  z[10] = 1.50000e+00;
  x[11] = 2.00000e+00;
  y[11] = 3.00000e+00;
  z[11] = 1.50000e+00;
  x[12] = 0.00000e+00;
  y[12] = 3.00000e+00;
  z[12] = 0.50000e+00;
  x[13] = 2.00000e+00;
  y[13] = 3.00000e+00;
  z[13] = 0.50000e+00;

  int error = ex_put_coord(exoid, x, y, z);
  printf("after ex_put_coord, error = %d\n", error);

  if (error) {
    ex_close(exoid);
    exit(-1);
  }

  char *coord_names[3];
  coord_names[0] = "x";
  coord_names[1] = "y";
  coord_names[2] = "z";

  error = ex_put_coord_names(exoid, coord_names);
  printf("after ex_put_coord_names, error = %d\n", error);

  if (error) {
    ex_close(exoid);
    exit(-1);
  }

  /* Write the face block parameters */
  char *block_names[10];
  block_names[0] = "face_block_1";

  int num_face_in_block[10];
  num_face_in_block[0] = 15;

  int num_total_nodes_per_blk[10];
  num_total_nodes_per_blk[0] = 58;
  int bids                   = 10;

  error = ex_put_block(exoid, EX_FACE_BLOCK, bids, "nsided", num_face_in_block[0],
                       num_total_nodes_per_blk[0], 0, 0, 0);
  printf("after ex_put_block, error = %d\n", error);

  if (error) {
    ex_close(exoid);
    exit(-1);
  }

  /* Write face block names */
  error = ex_put_names(exoid, EX_FACE_BLOCK, block_names);
  printf("after ex_put_names, error = %d\n", error);

  if (error) {
    ex_close(exoid);
    exit(-1);
  }

  /* write face connectivity */

  int *connect = (int *)calloc(num_total_nodes_per_blk[0], sizeof(int));

  int i = 0;
  int j = 0;

  int nnpe[20];
  connect[i++] = 5;
  connect[i++] = 6;
  connect[i++] = 8; /* connectivity of face 1 of element 1 */
  nnpe[j++]    = 3;

  connect[i++] = 2;
  connect[i++] = 1;
  connect[i++] = 4; /* face 2 of element 1 */
  nnpe[j++]    = 3;

  connect[i++] = 6;
  connect[i++] = 2;
  connect[i++] = 4;
  connect[i++] = 8; /* face 3 of element 1 */
  nnpe[j++]    = 4;

  connect[i++] = 8;
  connect[i++] = 4;
  connect[i++] = 1;
  connect[i++] = 5; /* face 4 of element 1 */
  nnpe[j++]    = 4;

  connect[i++] = 1;
  connect[i++] = 2;
  connect[i++] = 6;
  connect[i++] = 5; /*  face 5 of element 1 */
  nnpe[j++]    = 4;

  connect[i++] = 5;
  connect[i++] = 8;
  connect[i++] = 7; /* connectivity of face 1 of element 2 */
  nnpe[j++]    = 3;

  connect[i++] = 1;
  connect[i++] = 3;
  connect[i++] = 4; /*  face 2 of element 2 */
  nnpe[j++]    = 3;

  connect[i++] = 7;
  connect[i++] = 8;
  connect[i++] = 4;
  connect[i++] = 3; /*  face 3 of element 2 */
  nnpe[j++]    = 4;

  connect[i++] = 7;
  connect[i++] = 3;
  connect[i++] = 1;
  connect[i++] = 5; /*  face 4 of element 2 */
  nnpe[j++]    = 4;

  connect[i++] = 8;
  connect[i++] = 4;
  connect[i++] = 14;
  connect[i++] = 10;
  connect[i++] = 12; /* connectivity of face 1 of element 3 */
  nnpe[j++]    = 5;

  connect[i++] = 7;
  connect[i++] = 11;
  connect[i++] = 9;
  connect[i++] = 13;
  connect[i++] = 3; /*  face 2 of element 3 */
  nnpe[j++]    = 5;

  connect[i++] = 7;
  connect[i++] = 8;
  connect[i++] = 12;
  connect[i++] = 11; /* face 3 of element 3 */
  nnpe[j++]    = 4;

  connect[i++] = 11;
  connect[i++] = 12;
  connect[i++] = 10;
  connect[i++] = 9; /* face 4 of element 3 */
  nnpe[j++]    = 4;

  connect[i++] = 9;
  connect[i++] = 10;
  connect[i++] = 14;
  connect[i++] = 13; /*  face 5 of element 3 */
  nnpe[j++]    = 4;

  connect[i++] = 13;
  connect[i++] = 14;
  connect[i++] = 4;
  connect[i++] = 3; /* face 6 of element 3 */
  nnpe[j++]    = 4;

  assert(i == num_total_nodes_per_blk[0]);
  assert(j == num_face_in_block[0]);

  error = ex_put_conn(exoid, EX_FACE_BLOCK, bids, connect, NULL, NULL);
  printf("after ex_put_conn, error = %d\n", error);

  if (error) {
    ex_close(exoid);
    exit(-1);
  }

  free(connect);
  connect = NULL;

  error = ex_put_entity_count_per_polyhedra(exoid, EX_FACE_BLOCK, bids, nnpe);
  printf("after ex_put_entity_count_per_polyhedra, error = %d\n", error);

  if (error) {
    ex_close(exoid);
    exit(-1);
  }

  /* write element block parameters */
  block_names[0] = "nfaced_1";

  int num_elem_in_block[10];
  num_elem_in_block[0] = 3;
  int num_total_faces_per_blk[10];
  num_total_faces_per_blk[0] = 5 + 5 + 7;

  bids = 10;

  error = ex_put_block(exoid, EX_ELEM_BLOCK, bids, "nfaced", num_elem_in_block[0], 0, 0,
                       num_total_faces_per_blk[0], 0);
  printf("after ex_put_block, error = %d\n", error);

  if (error) {
    ex_close(exoid);
    exit(-1);
  }

  /* Write element block names */
  error = ex_put_names(exoid, EX_ELEM_BLOCK, block_names);
  printf("after ex_put_names, error = %d\n", error);

  if (error) {
    ex_close(exoid);
    exit(-1);
  }

  /* write element-face connectivity */
  connect = (int *)calloc(num_total_faces_per_blk[0], sizeof(int));

  i            = 0;
  j            = 0;
  connect[i++] = 1;
  connect[i++] = 2;
  connect[i++] = 3;
  connect[i++] = 4;
  connect[i++] = 5;
  nnpe[j++]    = 5; /* Number of faces per element 1 */

  connect[i++] = 4;
  connect[i++] = 6;
  connect[i++] = 7;
  connect[i++] = 8;
  connect[i++] = 9;
  nnpe[j++]    = 5; /* Number of faces per element 2 */

  connect[i++] = 8;
  connect[i++] = 10;
  connect[i++] = 11;
  connect[i++] = 12;
  connect[i++] = 13;
  connect[i++] = 14;
  connect[i++] = 15;
  nnpe[j++]    = 7; /* Number of faces per element 3 */

  assert(i == num_total_faces_per_blk[0]);
  assert(j == num_elem_in_block[0]);

  error = ex_put_conn(exoid, EX_ELEM_BLOCK, bids, NULL, NULL, connect);
  printf("after ex_put_conn, error = %d\n", error);

  if (error) {
    ex_close(exoid);
    exit(-1);
  }

  free(connect);

  error = ex_put_entity_count_per_polyhedra(exoid, EX_ELEM_BLOCK, bids, nnpe);
  printf("after ex_put_entity_count_per_polyhedra, error = %d\n", error);

  if (error) {
    ex_close(exoid);
    exit(-1);
  }

  /* write QA records; test empty and just blank-filled records */
  int num_qa_rec = 2;

  char *qa_record[2][4];
  qa_record[0][0] = "TESTWT-NFACED";
  qa_record[0][1] = "testwt-nfaced";
  qa_record[0][2] = "2010/02/15";
  qa_record[0][3] = "06:35:15";
  qa_record[1][0] = "";
  qa_record[1][1] = "                            ";
  qa_record[1][2] = "";
  qa_record[1][3] = "                        ";

  error = ex_put_qa(exoid, num_qa_rec, qa_record);
  printf("after ex_put_qa, error = %d\n", error);

  if (error) {
    ex_close(exoid);
    exit(-1);
  }

  /* write information records; test empty and just blank-filled records */
  int num_info = 3;

  char *info[3];
  info[0] = "This is the first information record.";
  info[1] = "";
  info[2] = "                                     ";

  error = ex_put_info(exoid, num_info, info);
  printf("after ex_put_info, error = %d\n", error);

  if (error) {
    ex_close(exoid);
    exit(-1);
  }

  /* close the EXODUS files
   */
  error = ex_close(exoid);
  printf("after ex_close, error = %d\n", error);
  if (error) {
    ex_close(exoid);
    exit(-1);
  }
  return 0;
}
