 #include <stdio.h>
#include <math.h>
#include "screen.h"
#include "display.h"

static int player_position[2] = {128,-1024-128};
static int player_direction = 0;
static int sin_player_direction;
static int cos_player_direction;

static int cos_table[256];
static int sin_table[256];

#define PI 3.141592653589793284

static int maze_width;
static int maze_height;
static char *maze;
static char *screenp;

typedef struct {
  int start[2];
  int end[2];
  int front;
  int back;
  int same;
} Wall;

#define MAX_WALLS 200
static Wall walls[MAX_WALLS];
static int wall_count = 0;

#define MAX_LINES 100
static int line_count = 0;
typedef struct {
  int x1,y1,x2,y2;
} Line_t;
static Line_t lines[MAX_LINES];

static int PointInFront(const Wall *const wall1_ptr,const int pos[2])
{
  int *wall1_end = wall1_ptr->end;
  int *wall1_start = wall1_ptr->start;
  if (wall1_start[0]==wall1_end[0]) {
    if (wall1_start[1]>wall1_end[1]) {
      if (pos[0]>wall1_start[0])
        return 1;
      if (pos[0]<wall1_start[0])
        return -1;
      return 0;
    }
    else {
      if (pos[0]<wall1_start[0])
        return 1;
      if (pos[0]>wall1_start[0])
        return -1;
      return 0;
    }
  }
  else if (wall1_start[1]==wall1_end[1]) {
    if (wall1_start[0]>wall1_end[0]) {
      if (pos[1]<wall1_start[1])
        return 1;
      if (pos[1]>wall1_start[1])
        return -1;
      return 0;
    }
    else {
      if (pos[1]>wall1_start[1])
        return 1;
      else if(pos[1]<wall1_start[1])
        return -1;
      return 0;
    }
  }
  {
    long vect1[2];
    long vect2[2];
    long val1,val2;
    vect1[0] = wall1_end[0]-wall1_start[0];
    vect1[1] = wall1_end[1]-wall1_start[1];
    vect2[0] = pos[0]-wall1_start[0];
    vect2[1] = pos[1]-wall1_start[1];
    val1 = vect1[0]*vect2[1];
    val2 = vect1[1]*vect2[0];
    if (val1>val2)
      return 1;
    if (val1==val2)
      return 0;
    return -1;
  }
}

static int WallInFront(
  const Wall *const wall1_ptr,const Wall *const wall2_ptr)
{
  int cmp = PointInFront(wall1_ptr,wall2_ptr->start);
  if (cmp!=0)
    return cmp;
  return PointInFront(wall1_ptr,wall2_ptr->end);
}

static void AddWall(const int x1,const int y1,const int x2,const int y2)
{
  if (wall_count>=MAX_WALLS) {
    printf("Wall overflow\n");
    exit(0);
  }
  {
    Wall *new_wall = &walls[wall_count];
    int *start = new_wall->start;
    int *end = new_wall->end;
    start[0] = x1;
    start[1] = y1;
    end[0] = x2;
    end[1] = y2;
    new_wall->front = -1;
    new_wall->back = -1;
    new_wall->same = -1;
    if (wall_count>0) {
      int wallnum = 0;
      for (;;) {
        Wall *wall = &walls[wallnum];
        int cmp = WallInFront(wall,new_wall);
        if (cmp>0) {
          if (wall->front==-1) {
            wall->front = wall_count;
            break;
          }
          wallnum = wall->front;
        }
        else if (cmp<0) {
          if (wall->back == -1) {
            wall->back = wall_count;
            break;
          }
          wallnum = wall->back;
        }
        else {
          new_wall->same = wall->same;
          wall->same = wall_count;
          break;
        }
      }
    }
  }
  ++wall_count;  
}

static void RdMazeLine(FILE *infile,char *line)
{
  int col = 0;
  while (col<maze_width) {
    int c = fgetc(infile);
    if (c==EOF) {
      printf("hit eof on maze.dat\n");
      exit(0);
    }
    if (c=='\n') {
      while (col<maze_width) {
        *line++ = ' ';
        ++col;
      }
      return;
    }
    *line++ = c;
    ++col;
  }
  while (fgetc(infile) != '\n') { }
}

static void ReadMaze(void)
{
  FILE *infile = fopen("maze.dat","r");
  if (!infile) {
    printf("Cannot open maze.dat\n");
    exit(0);
  }
  fscanf(infile,"%d %d",&maze_width,&maze_height);
  while (fgetc(infile)!='\n') { }
  printf("Maze size %dx%d\n",maze_width,maze_height);
  maze = (char *)malloc(maze_width*maze_height);
  {
    int row = 0;
    while (row<maze_height) {
      RdMazeLine(infile,maze+row*maze_width);
      ++row;
    }
  }
  fclose(infile);
}

static int MazePoint(int row,int col)
{
  if (row>=maze_height || row<0 || col>=maze_width || col<0)
    return ' ';
  return maze[row*maze_width+col];
}

static void BuildWalls(void)
{
  int rownum = 0;
  while (rownum<maze_height) {
    int colnum = 0;
    printf("%d\n",rownum);
    while (colnum<maze_width) {
      char c = maze[rownum*maze_width+colnum];
      if (c!=' ') {
        int col1 = colnum*512;
        int col2 = col1+512;
        int row1 = rownum*512;
        int row2 = row1+512;
        if (MazePoint(rownum,colnum-1)==' ')
          AddWall(col1,row1,col1,row2);
        if (MazePoint(rownum+1,colnum)==' ')
          AddWall(col1,row2,col2,row2);
        if (MazePoint(rownum,colnum+1)==' ')
          AddWall(col2,row2,col2,row1);
        if (MazePoint(rownum-1,colnum)==' ')
          AddWall(col2,row1,col1,row1);
      }
      ++colnum;
    }
    ++rownum;
  }
}

static void MovePlayer(const int dx,const int dy)
{
  player_position[0] += dx*cos_player_direction+dy*sin_player_direction;
  player_position[1] += -dx*sin_player_direction+dy*cos_player_direction;
}

static void UpdPlayerDir(void)
{
  sin_player_direction = sin_table[player_direction];
  cos_player_direction = cos_table[player_direction];
}

static void TurnPlayer(const int dd)
{
  player_direction+=dd;
  if (player_direction>255)
    player_direction -= 256;
  else if (player_direction<0)
    player_direction += 256;
  UpdPlayerDir();
}

static void GenerateSinCosTable(void) 
{
  sin_table[0] = 0;
  cos_table[0] = 256;
  sin_table[64] = 256;
  cos_table[64] = 0;
  sin_table[128] = 0;
  cos_table[128] = -256;
  sin_table[192] = -256;
  cos_table[192] = 0;
  {
    int i = 1;
    while (i<64) {
      float ang = i*PI*(1.0/128.0);
      int val = sin(ang)*256;
      sin_table[i] = val;
      sin_table[128-i] = val;
      cos_table[64-i] = val;
      cos_table[192+i] = val;
      sin_table[256-i] = -val;
      sin_table[128+i] = -val;
      cos_table[64+i] = -val;
      cos_table[192-i] = -val;
      ++i;
    }
  }
}

static void WorldToScreen(
  const int world_x,const int world_y,
  int *const screen_x_ptr,int *const screen_y_ptr
)
{
#ifdef IGNORE
  long rel_x = world_x-player_position[0];
  long rel_y = world_y-player_position[1];
  long new_x,new_y;
  new_x = rel_x*cos_player_direction-rel_y*sin_player_direction;
  new_y = rel_y*cos_player_direction+rel_x*sin_player_direction;
  *screen_x_ptr = new_x>>8;
  *screen_y_ptr = new_y>>8;
#else
  int rel_x = world_x-player_position[0];
  int rel_y = world_y-player_position[1];
  long temp1,temp2;
  LMulII(&temp1,rel_x,cos_player_direction);
  LMulII(&temp2,rel_y,sin_player_direction);
  *screen_x_ptr = m턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬턬�